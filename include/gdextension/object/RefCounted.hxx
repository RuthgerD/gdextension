#pragma once

#include "gdextension/object/Object.hxx"
#include "gdextension/utils/FixedString.hxx"

struct RefCounted : public Object {
    constexpr static FixedString name = "RefCounted";

    RefCounted(ObjectHandle handle) : Object{handle} {}

    bool init_ref();
    bool reference();
    bool unreference();
};