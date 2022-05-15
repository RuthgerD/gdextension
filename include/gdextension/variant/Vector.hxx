#pragma once

#include <stdexcept>
#include "gdextension/core/Interface.hxx"
#include "gdextension/variant/String.hxx"
#include "gdextension/variant/detail/BuiltinMethod.hxx"
#include "gdextension/variant/detail/VariantOps.hxx"

template <class T> struct VectorTraits;

template <class T> class Vector {
    using traits = VectorTraits<T>;
    using This = Vector<T>;

    char opaque[traits::opaque_size];

  public:
    using value_type = T;

    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using iterator = T*;
    using const_iterator = const T*;

    constexpr static auto variant_type = traits::variant_type;

    Vector() { interface.variant_get_ptr_constructor(variant_type, 0)(this, nullptr); }

    T* data() noexcept { return (interface.*traits::index)(this, 0); }
    const T* data() const noexcept { return (interface.*traits::const_index)(this, 0); }

    size_type size() const {
        return builtin_method<size_type(This*), "size", 171192809>::call(const_cast<This*>(this));
    }

    reference front() { return *data(); }
    const_reference front() const { return *data(); }

    reference back() { return *(data() + size() - 1); }
    const_reference back() const { *(data() + size() - 1); }

    iterator begin() { return data(); }
    const_iterator begin() const { return data(); }
    const_iterator cbegin() const { return begin(); }

    iterator end() { return begin() + size(); }
    const_iterator end() const { return begin() + size(); }
    const_iterator cend() const { return end(); }

    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    void resize(size_type size);

    String* erase(size_type pos) {
        auto signed_pos = static_cast<std::int64_t>(pos);
        builtin_method<void(This*, std::int64_t*), "remove_at", 2>::call(const_cast<This*>(this),
                                                                         &signed_pos);
        return end();
    }

    void insert(size_type pos, const value_type& value) {
        auto signed_pos = static_cast<std::int64_t>(pos);
        builtin_method<size_type(This*, std::int64_t*, String*), "insert", 4>::call(
            const_cast<This*>(this), &signed_pos, const_cast<value_type*>(&value));
    }

    void push_back(const value_type& value) { insert(size(), value); }

    template <class... Args> reference emplace_back(Args&&... args) {
        push_back(value_type{std::forward<Args>(args)...});
        return back();
    }

    void pop_back() { erase(size() - 1); }

    bool empty() { return size(); }

    reference at(size_type pos) {
        if (pos >= size())
            throw std::out_of_range{""};
        return (*this)[pos];
    }
    reference operator[](size_type pos) noexcept { return *(data() + pos); }

    const_reference at(size_type pos) const {
        if (pos >= size())
            throw std::out_of_range{""};
        return (*this)[pos];
    }

    const_reference operator[](size_type pos) const noexcept { return *(data() + pos); }
};