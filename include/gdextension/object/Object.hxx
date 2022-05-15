#pragma once

#include <any>
#include <optional>
#include "gdextension/core/Interface.hxx"
#include "gdextension/object/ObjectHandle.hxx"
// #include "gdextension/core/detail/core.hxx"
#include "gdextension/utils/FixedString.hxx"
#include "gdextension/variant/NodePath.hxx"
#include "gdextension/variant/String.hxx"
#include "gdextension/variant/StringName.hxx"
#include "gdextension/variant/Variant.hxx"

class Object {

    ObjectHandle obj_handle;

  public:
    constexpr static FixedString name = "Object";

    Object(ObjectHandle handle) : obj_handle{handle} {
        // somehow assert handle is our type?
    }

    Object(const Object&) = default;

    bool is_queued_for_deletion() const;

    Variant get(const String& property);
    Variant get(const NodePath& property);

    void set(const String& property, const Variant& value);
    void set(NodePath property, const Variant& value);
    void set_deferred(NodePath name, const Variant& value);

    bool has_meta(StringName name);
    void set_meta(StringName name, const Variant& value);
    void remove_meta(StringName name);

    void set_script(const Variant& script);
    Variant get_script();

    bool has_signal(StringName signal) const;
    bool has_user_signal(StringName signal) const;
    bool is_blocking_signals() const;

    bool has_method(StringName name);
    Variant call(StringName method, Vector<Variant> args);
    Variant call_deferred(StringName method, Vector<Variant> args);

    std::uint64_t instance_id() const;

    ObjectHandle handle() { return obj_handle; }

    operator String() const;
};
