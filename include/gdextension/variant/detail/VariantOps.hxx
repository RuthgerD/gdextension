#pragma once

#include <cstdint>
class Variant;

enum class VariantType : std::uint32_t {
    NIL,
    BOOL,
    INT,
    FLOAT,
    STRING,
    VECTOR2,
    VECTOR2I,
    RECT2,
    RECT2I,
    VECTOR3,
    VECTOR3I,
    TRANSFORM2D,
    PLANE,
    QUATERNION,
    AABB,
    BASIS,
    TRANSFORM3D,
    COLOR,
    STRING_NAME,
    NODE_PATH,
    RID,
    OBJECT,
    CALLABLE,
    SIGNAL,
    DICTIONARY,
    ARRAY,
    PACKED_BYTE_ARRAY,
    PACKED_INT32_ARRAY,
    PACKED_INT64_ARRAY,
    PACKED_FLOAT32_ARRAY,
    PACKED_FLOAT64_ARRAY,
    PACKED_STRING_ARRAY,
    PACKED_VECTOR2_ARRAY,
    PACKED_VECTOR3_ARRAY,
    PACKED_COLOR_ARRAY,
    MAX
};

// Probably
enum class VariantOperator {
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE,
    POSITIVE,
    MODULE,
    POWER,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    BIT_AND,
    BIT_OR,
    BIT_XOR,
    BIT_NEGATE,
    AND,
    OR,
    XOR,
    NOT,
    IN,
    MAX
};
