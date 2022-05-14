#pragma once

#include <cstdint>

enum class CallErrorType { // TODO: specify size
    OK,
    ERROR_INVALID_METHOD,
    ERROR_INVALID_ARGUMENT,
    ERROR_TOO_MANY_ARGUMENTS,
    ERROR_TOO_FEW_ARGUMENTS,
    ERROR_INSTANCE_IS_NULL,

};

struct CallError {
    CallErrorType error;
    std::int32_t argument;
    std::int32_t expected;
};
