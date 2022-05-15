#pragma once

#include <bit>
#include <concepts>
#include "gdextension/object/Object.hxx"
#include "gdextension/object/ObjectHandle.hxx"

template <class T>
concept PlainObject = std::derived_from<T, Object> && std::is_standard_layout_v<T> &&
    requires(ObjectHandle handle) {
    T{handle};
    std::bit_cast<T>(handle);
};

template <class T>
concept ExtensionObject = !PlainObject<T> && std::derived_from<T, Object> && requires {
    typename T::Extended;
};

template <class T> struct RawPtr;

template <PlainObject T> struct RawPtr<T> {
    T ptr;

    T* operator*() { return &ptr; }
    const T* operator*() const { return &ptr; }

    T* operator->() { return &ptr; }
    const T* operator->() const { return &ptr; }
};

template <ExtensionObject T> struct RawPtr<T> {
    T* ptr;

    T* operator*() { return ptr; }
    const T* operator*() const { return ptr; }

    T* operator->() { return ptr; }
    const T* operator->() const { return ptr; }
};
