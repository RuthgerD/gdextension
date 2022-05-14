#pragma once

#include <cstddef>
template <std::size_t N> struct FixedString {
    char content[N] = {0};

    constexpr FixedString(const char (&input)[N]) {
        for (std::size_t i = 0; i < N; ++i)
            content[i] = input[i];
    }

    [[nodiscard]] constexpr const char* data() const { return +content; }
};

template <> struct FixedString<0> {
    char content[1] = {0};

    constexpr FixedString(const char*) {}
};

template <FixedString _name> struct Named { constexpr static auto name = _name; };
