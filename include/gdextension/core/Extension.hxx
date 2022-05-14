#pragma once

#include <cstdint>
#include "gdextension/core/CallError.hxx"
#include "gdextension/object/ObjectHandle.hxx"
#include "gdextension/utils/TypeT.hxx"
#include "gdextension/variant/detail/VariantOps.hxx"

class String;
class StringName;

enum class ExtensionClassMethodFlags : std::uint32_t {
    NORMAL = 1,
    EDITOR = 2,
    NOSCRIPT = 4,
    CONST = 8,
    REVERSE = 16,
    VIRTUAL = 32,
    FROM_SCRIPT = 64,
    VARARG = 128,
    STATIC = 256,
    DEFAULT = NORMAL,
};

enum class PropertyHint : std::uint32_t { NONE };       // Generated
enum class PropertyUsageFlags : std::uint32_t { NONE }; // Generated

enum ExtensionClassMethodArgumentMetadata {
    NONE,
    INT_IS_INT8,
    INT_IS_INT16,
    INT_IS_INT32,
    INT_IS_INT64,
    INT_IS_UINT8,
    INT_IS_UINT16,
    INT_IS_UINT32,
    INT_IS_UINT64,
    REAL_IS_FLOAT,
    REAL_IS_DOUBLE
};

// Userdata attached to real engine class
struct GDExtensionClassInstance;

struct PropertyInfo {
    VariantType type;
    const char* name;
    const char* class_name;
    PropertyHint hint;
    const char* hint_string;
    PropertyUsageFlags usage;
};

struct MethodInfo {
    const char* name;
    PropertyInfo return_value;
    ExtensionClassMethodFlags flags;
    int32_t id;
    PropertyInfo* arguments;
    uint32_t argument_count;
    Variant* default_arguments = nullptr;
    uint32_t default_argument_count = 0;
};

struct ExtensionClassCreationInfo {
    // utilized by gdscript, why??
    bool (*set_func)(GDExtensionClassInstance* instance, const StringName* name, const Variant* value);
    bool (*get_func)(GDExtensionClassInstance* instance, const StringName* name, Variant* r_ret);

    // virtual overrides?
    const PropertyInfo* (*get_property_list_func)(GDExtensionClassInstance* instance, uint32_t* r_count);
    void (*free_property_list_func)(GDExtensionClassInstance* instance, const PropertyInfo* list);
    void (*notification_func)(GDExtensionClassInstance* instance, int32_t what);
    void (*to_string_func)(GDExtensionClassInstance* instance, String* out);

    // seemingly not used in the engine
    void (*reference_func)(GDExtensionClassInstance* instance);
    void (*unreference_func)(GDExtensionClassInstance* instance);

    // required
    ObjectHandle (*create_instance_func)(void* userdata);

    // required
    void (*free_instance_func)(void* userdata, GDExtensionClassInstance* instance);

    // seemingly not used in the engine
    auto (*get_virtual_func)(void* userdata, const char* name)
        -> void (*)(GDExtensionClassInstance* instance, const TypeT* args, TypeT r_ret);

    uint64_t (*get_rid_func)(GDExtensionClassInstance* instance);
    void* class_userdata;
};

struct ExtensionClassMethodInfo {
    const char* name;
    void* method_userdata;

    void (*call_func)(void* method_userdata, GDExtensionClassInstance* instance, const Variant** args,
                      const std::int64_t argument_count, Variant* r_return, CallError* r_error);

    void (*ptrcall_func)(void* method_userdata, GDExtensionClassInstance* instance, const TypeT* args,
                         TypeT r_ret);

    uint32_t method_flags;
    uint32_t argument_count;
    bool has_return_value;

    VariantType (*get_argument_type_func)(void* method_userdata, int32_t argument);
    void (*get_argument_info_func)(void* method_userdata, int32_t argument, PropertyInfo* r_info);

    ExtensionClassMethodArgumentMetadata (*get_argument_metadata_func)(void* method_userdata,
                                                                       int32_t argument);

    uint32_t default_argument_count;
    Variant** default_arguments;
};

struct ExtensionClassLibrary; // probably needs to be opaque