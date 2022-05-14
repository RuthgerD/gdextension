#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "gdextension/variant/detail/VariantOps.hxx"

#define VARIANT_SIZE 24

template <class T> struct variant_traits;

template <class T>
concept FromVariant = std::default_initializable<T> && requires(Variant& v, T& t) {
    typename variant_traits<T>::value_type;
    { variant_traits<T>::variant_type } -> std::same_as<const VariantType&>;
    { variant_traits<T>::from(v, t) } -> std::same_as<void>;
    { variant_traits<T>::into(t, v) } -> std::same_as<void>;
};

// mimics std::variant
class Variant {
    const char opaque[VARIANT_SIZE] = {0};

  public:
    Variant();
    Variant(const Variant& rhs);
    Variant& operator=(const Variant& rhs);

    Variant(Variant&&);

    template <FromVariant T> Variant(const T& v) { variant_traits<T>::into(v, *const_cast<Variant*>(this)); }

    template <FromVariant T> explicit operator T() const {
        T ret;
        variant_traits<T>::from(*const_cast<Variant*>(this), ret);
        return ret;
    }
};
