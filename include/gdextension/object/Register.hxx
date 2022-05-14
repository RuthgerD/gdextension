#pragma once

#include <algorithm>
#include <any>
#include <bit>
#include <cstdint>
#include <span>
#include <tuple>
#include <type_traits>
#include <bits/utility.h>
#include "gdextension/core/Extension.hxx"
#include "gdextension/core/Interface.hxx"
#include "gdextension/object/ObjectHandle.hxx"
#include "gdextension/object/Register.hxx"
#include "gdextension/utils/FixedString.hxx"
#include "gdextension/variant/String.hxx"
#include "gdextension/variant/Variant.hxx"
#include "gdextension/variant/VariantTraits.hxx"

#include "gdextension/variant/detail/VariantOps.hxx"

#include <string>

template <class... Ts> struct visitor : Ts... { using Ts::operator()...; };

template <FixedString class_name, class T> struct Extends : public T {
    constexpr static FixedString name = class_name;

    using Extended = T;
    Extends(ObjectHandle owner) : T{owner} {}
};

constexpr static auto test = InstanceBindingCallbacks{
    nullptr, [](void*, void*, void* instance) { delete reinterpret_cast<std::any*>(instance); }, nullptr};

template <class T, class... Args> auto instance_extension_class(Args... args) -> T* {

    std::any* instance;
    if constexpr (requires { typename T::Extended; }) {
        auto obj = interface.classdb_construct_object(T::Extended::name.data());
        instance = new std::any{std::in_place_type_t<T>{}, obj, args...};
        interface.object_set_instance(obj, T::name.data(),
                                      reinterpret_cast<GDExtensionClassInstance*>(instance));
        interface.object_set_instance_binding(obj, token(), instance, &noop_instance_binding_callback);
    } else {
        auto obj = interface.classdb_construct_object(T::name.data());
        instance = new std::any{std::in_place_type_t<T>{}, obj, args...};
        interface.object_set_instance_binding(obj, token(), instance, &test);
    }
    return &std::any_cast<T&>(*instance);
}

// clang-format off
template <auto _function, auto _is_const, auto _is_static, class Ret, class This, class... Args>
struct wrap_func_info {
    constexpr static auto is_const = _is_const;
    constexpr static auto is_static = _is_static;
    constexpr static auto has_return = !std::same_as<Ret, void>;
    constexpr static auto argc = sizeof...(Args);

    constexpr auto argument_type(std::int32_t n) const {
        constexpr VariantType types[] = {
            variant_traits<std::remove_cvref_t<Ret>>::variant_type,
            variant_traits<std::remove_cvref_t<Args>>::variant_type...
        };
        return types[n + 1];
    }

    template <class From> constexpr Ret operator()(This* t, std::span<From, argc> arguments) const {
        return [=]<class T, T... i>(std::integer_sequence<T, i...>) {
            if constexpr (!is_static)
                return _function(t, static_cast<std::remove_cvref_t<Args>>((arguments[i]))...);
            else
                return _function(static_cast<std::remove_cvref_t<Args>>((arguments[i]))...);
        }
        (std::index_sequence_for<Args...>{});
    }
};

template <class This, auto function> constexpr auto extract() {
    return visitor{
        []<class Ret, class... Args>(Ret(*)(Args...)){
            return wrap_func_info<function, true, true, Ret, This, Args...>{};
        },
        []<class Ret, class... Args>(Ret (*)(const This*, Args...)) {
            return wrap_func_info<function, true, false, Ret, This, Args...>{};
        },
        []<class Ret, class... Args>(Ret (*)(This*, Args...)) {
            return wrap_func_info<function, false, false, Ret, This, Args...>{};
        }
    }(+function);
}
// clang-format on

template <class This> struct Register {

    Register(void (*register_func)(Register<This>&)) {

        constexpr auto create_func = +[](void*) {
            if constexpr (requires(ObjectHandle obj) { This{obj}; }) {
                return instance_extension_class<This>()->handle();
            } else {
                // TODO: NOOOOOO
                return std::bit_cast<ObjectHandle>(nullptr);
            }
        };

        constexpr auto free_func =
            +[](void*, GDExtensionClassInstance* instance) { delete reinterpret_cast<std::any*>(instance); };

        auto info = ExtensionClassCreationInfo{};

        info.create_instance_func = create_func;
        info.free_instance_func = free_func;

        interface.classdb_register_extension_class(library, This::name.data(), This::Extended::name.data(),
                                                   &info);

        register_func(*this);
    }

    template <FixedString name, auto instance_method> // TODO: make instance method concept
    auto register_method() {                          // TODO: take default args as runtime args :)

        constexpr static auto fun = extract<This, instance_method>();
        constexpr static auto variant_call =
            +[](void*, GDExtensionClassInstance* instance, const Variant** args, const std::int64_t argc,
                Variant* ret, CallError* error) {
                if (argc != fun.argc) {
                    *error = CallError{.error = CallErrorType::ERROR_TOO_FEW_ARGUMENTS,
                                       .argument = static_cast<std::int32_t>(argc),
                                       .expected = fun.argc};
                    return;
                }

                // TODO: return value
                const auto arguments = std::span<const Variant, fun.argc>{*args, fun.argc};
                const auto self = reinterpret_cast<This*>(instance);

                *ret = Variant{};
                if constexpr (fun.has_return)
                    *ret = static_cast<Variant>(fun(self, arguments));
                else
                    fun(self, arguments);
            };

        constexpr static auto ptr_call =
            +[](void*, GDExtensionClassInstance* instance, const TypeT* args, TypeT ret) {
                const auto arguments = std::span<const TypeT, fun.argc>{args, fun.argc};
                const auto self = reinterpret_cast<This*>(instance);

                if constexpr (fun.has_return)
                    ret = fun(self, arguments);
                else
                    fun(self, arguments);
            };

        constexpr static auto argument_n_type =
            +[](void*, std::int32_t argument) { return fun.argument_type(argument); };

        constexpr static auto argument_n_info = +[](void*, std::int32_t argument, PropertyInfo* info) {
            info->type = fun.argument_type(argument);
            info->name = "arg";
            info->class_name = "";
            info->hint_string = "";
            info->hint = static_cast<PropertyHint>(0);
            info->usage = static_cast<PropertyUsageFlags>(7);
        };

        constexpr static auto argument_n_metadata =
            +[](void*, std::int32_t) { return ExtensionClassMethodArgumentMetadata::NONE; };

        auto flags = 1 | (fun.is_const ? static_cast<uint32_t>(ExtensionClassMethodFlags::CONST) : 0) |
                     (fun.is_static ? static_cast<uint32_t>(ExtensionClassMethodFlags::STATIC) : 0);

        const auto method_info = ExtensionClassMethodInfo{
            name.data(),     nullptr,         variant_call,        ptr_call, flags,  fun.argc, fun.has_return,
            argument_n_type, argument_n_info, argument_n_metadata, 0,        nullptr};

        interface.classdb_register_extension_class_method(library, This::name.data(), &method_info);
    }
};
