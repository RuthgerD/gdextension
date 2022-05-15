#pragma once

#include <bit>
#include <type_traits>
#include <utility>
#include "gdextension/core/Interface.hxx"
#include "gdextension/object/Object.hxx"
#include "gdextension/object/ObjectHandle.hxx"
#include "gdextension/object/RefCounted.hxx"
#include "gdextension/object/Register.hxx"
#include "gdextension/variant/detail/VariantOps.hxx"

template <std::derived_from<Object> T> class Ptr {
    RawPtr<T> managed;

    friend variant_traits<Ptr<T>>;

  public:
    constexpr static auto refcounted = std::is_base_of_v<RefCounted, T>;

    template <class... Args> Ptr(std::in_place_t, Args... args) requires requires(ObjectHandle handle) {
        T{handle, args...};
    } : Ptr{instance_extension_class<T>(args...)} {
    }

    Ptr(const Ptr& other) : managed{other.managed} {
        if constexpr (refcounted) {
            managed->reference();
        }
    }

    Ptr& operator=(const Ptr& other) {
        managed = other.managed;
        if constexpr (refcounted) {
            managed->reference();
        }
        return *this;
    }

    Ptr(RawPtr<T> ptr) : managed{ptr} {
        if constexpr (refcounted) {
            managed->init_ref();
        }
    }

    Ptr(ObjectHandle handle)
        : managed{[=] {
              if constexpr (ExtensionObject<T>) {
                  if (auto* ext = reinterpret_cast<std::any*>(interface.object_get_instance_binding(
                          handle, token(), &noop_instance_binding_callback)))
                      return RawPtr<T>{&std::any_cast<T&>(*ext)};
              } else {
                  return RawPtr<T>{std::bit_cast<T>(handle)};
              }
          }()} {}

    T* operator->() { return *managed; }
    T* operator*() { return managed; }

    T* get() { return managed; }

    void free() {
        if constexpr (refcounted)
            if (!managed->unreference())
                return;

        interface.object_destroy(managed->handle());
    }

    ~Ptr() {
        if constexpr (refcounted)
            free();
    }
};

template <std::derived_from<Object> T> struct variant_traits<Ptr<T>> {
    using value_type = Ptr<T>;
    constexpr static auto variant_type = VariantType::OBJECT;

    static value_type from(const Variant& v) {
        const thread_local auto ctor = interface.get_variant_to_type_constructor(variant_type);
        ObjectHandle handle;
        ctor(&handle, const_cast<Variant*>(&v));

        return {handle};
    }

    static void into(const value_type& v, Variant& out) {
        const thread_local auto ctor = interface.get_variant_from_type_constructor(variant_type);

        auto handle = v.managed->handle();
        ctor(&out, &handle);
    }
};
