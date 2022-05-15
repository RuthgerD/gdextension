#pragma once

#include "gdextension/variant/String.hxx"
#include "gdextension/variant/Vector.hxx"

template <> struct VectorTraits<String> {
    constexpr static auto variant_type = VariantType::PACKED_STRING_ARRAY;
    constexpr static unsigned int opaque_size = 16;
    constexpr static auto index = &Interface::packed_string_array_operator_index;
    constexpr static auto const_index = &Interface::packed_string_array_operator_index;
};

// Note: Godot's Array type is literally just a packed one except behind an extra pointer
template <> struct VectorTraits<Variant> {
    constexpr static auto variant_type = VariantType::ARRAY;
    constexpr static unsigned int opaque_size = 8;
    constexpr static auto index = &Interface::array_operator_index;
    constexpr static auto const_index = &Interface::array_operator_index_const;
};
