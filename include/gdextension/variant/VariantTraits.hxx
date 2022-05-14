#pragma once

#include "gdextension/core/Interface.hxx"
#include "gdextension/variant/Variant.hxx"

template <class T>
concept FromNativeVariant = std::is_standard_layout_v<T> && std::default_initializable<T> && requires {
    { T::variant_type } -> std::same_as<const VariantType&>;
};

template <FromNativeVariant T> struct variant_traits<T> {
    using value_type = T;
    constexpr static auto variant_type = T::variant_type;

    static void from(const Variant& v, value_type& out) {
        const thread_local auto ctor = interface.get_variant_to_type_constructor(variant_type);
        ctor(&out, const_cast<Variant*>(&v));
    }

    static void into(const value_type& v, Variant& out) {
        const thread_local auto ctor = interface.get_variant_from_type_constructor(variant_type);
        ctor(&out, const_cast<T*>(&v));
    }
};

template <> struct variant_traits<void> {
    using value_type = std::nullptr_t;
    constexpr static auto variant_type = VariantType::NIL;

    static void from(const Variant&, value_type& out) { out = nullptr; }
    static void into(const value_type&, Variant& out) { out = Variant{}; }
};
