#pragma once

#include <cstddef>
#include <limits>
#include <string>
#include <string_view>
#include "gdextension/variant/detail/VariantOps.hxx"

constexpr unsigned int STRING_SIZE = 8;

// UTF-32 string
class String {
    char opaque[STRING_SIZE];

  public:
    using value_type = char32_t;
    using reference = char32_t&;
    using const_reference = const char32_t&;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using iterator = char32_t*;
    using const_iterator = const char32_t*;

    constexpr static VariantType variant_type = VariantType::STRING;

    String();
    String(const String&);

    String& operator=(const String&);

    String(String&&);

    String(std::u32string_view);
    String(const char32_t*);

    iterator begin();
    const_iterator begin() const { return cbegin(); }

    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const { return cend(); }

    const_iterator cend() const;

    char32_t* data();
    const char32_t* data() const;

    char32_t& at(std::size_t pos);
    const char32_t& at(std::size_t pos) const;

    size_type size() const;
    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    bool empty() const noexcept { return size(); }

    void clear() noexcept;

    // TODO: implement SequenceContainer requirements

    char32_t& operator[](std::size_t pos);
    const char32_t& operator[](std::size_t pos) const;

    operator std::u32string_view() const;
    explicit operator std::string() const;

    // explicit operator Variant() const;

    ~String();
};
