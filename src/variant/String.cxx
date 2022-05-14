#include <array>
#include <codecvt>
#include <cstddef>
#include <cstdint>
#include <locale>
#include <string_view>
#include <type_traits>
#include "gdextension/core/Interface.hxx"
#include "gdextension/utils/FixedString.hxx"
#include "gdextension/variant/String.hxx"
#include "gdextension/variant/detail/BuiltinMethod.hxx"

String::String() { interface.variant_get_ptr_constructor(VariantType::STRING, 0)(this, nullptr); }

String::String(const String& str) {
    const TypeT args[] = {&const_cast<String&>(str)};
    interface.variant_get_ptr_constructor(VariantType::STRING, 1)(this, args);
}

String& String::operator=(const String& str) {
    const TypeT args[] = {&const_cast<String&>(str)};
    interface.variant_get_ptr_constructor(VariantType::STRING, 1)(this, args);
    return *this;
}

String::String(const char32_t* str) { interface.string_new_with_utf32_chars(this, str); }

String::String(std::u32string_view str) {
    interface.string_new_with_utf32_chars_and_len(this, str.data(), static_cast<std::int64_t>(str.size()));
}

String::operator std::u32string_view() const { return {data(), size()}; }

template <class Facet> struct deletable_facet : Facet {
    template <class... Args> deletable_facet(Args&&... args) : Facet(std::forward<Args>(args)...) {}
    ~deletable_facet() {}
};

char32_t* String::data() { return interface.string_operator_index(const_cast<String*>(this), 0); }
const char32_t* String::data() const {
    return interface.string_operator_index_const(const_cast<String*>(this), 0);
}

char32_t* String::begin() { return data(); }
const char32_t* String::cbegin() const { return data(); }

char32_t* String::end() { return data() + size(); }
const char32_t* String::cend() const { return data() + size(); }

String::operator std::string() const {
    auto conv =
        std::wstring_convert<deletable_facet<std::codecvt<char32_t, char, std::mbstate_t>>, char32_t>{};

    const auto bytes = static_cast<std::u32string_view>(*this);

    return conv.to_bytes(bytes.data(), bytes.data() + bytes.size());
}

std::size_t String::size() const {
    return builtin_method<std::int64_t(String*), "length", 171192809>::call(const_cast<String*>(this));
}

String::~String() { interface.variant_get_ptr_destructor(variant_type)(this); }
