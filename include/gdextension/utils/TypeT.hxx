#pragma once

#include <utility>

// Pointer to specfic type (but can't be named in the interface)
struct TypeT {
    void* ptr;
    template <class T> TypeT(T* t_ptr) : ptr{t_ptr} {}
    TypeT(std::nullptr_t) : ptr{nullptr} {}

    template <class T> explicit operator const T&() const { return *static_cast<const T*>(ptr); }
    template <class T> explicit operator const T() const { return *static_cast<const T*>(ptr); }

    template <class T> TypeT& operator=(T&& value) {
        *static_cast<T*>(ptr) = std::forward<T>(value);
        return *this;
    }
};
