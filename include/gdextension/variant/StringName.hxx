#pragma once

#include <cstddef>
#include <string_view>
#include "gdextension/variant/String.hxx"

constexpr std::size_t STRINGNAME_SIZE = 8;

class String;
class StringName {
    unsigned char opaque[STRINGNAME_SIZE];

  public:
    StringName();
    StringName(const StringName& str);
    StringName(const String& str);

    explicit operator String() const;

    bool operator<=>(const String& rhs) const;

    ~StringName();
};