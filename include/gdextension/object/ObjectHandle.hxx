#pragma once

#include "gdextension/variant/detail/VariantOps.hxx"

class ObjectHandle {
    void* _ptr;

  public:
    constexpr static auto variant_type = VariantType::OBJECT;

    operator bool() const { return _ptr; }
};
