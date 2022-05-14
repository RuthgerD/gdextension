#pragma once

#include "gdextension/core/Extension.hxx"
#include "gdextension/variant/detail/VariantOps.hxx"

struct ExtensionScriptInstanceData;
struct ScriptInstance;
struct ExtensionScriptLanguage;
class Variant;
class StringName;

struct ExtensionScriptInstanceInfo {

    bool (*set_func)(ExtensionScriptInstanceData* p_instance, const StringName* p_name,
                     const Variant* p_value);

    bool (*get_func)(ExtensionScriptInstanceData* p_instance, const StringName* p_name, Variant* r_ret);
    const PropertyInfo* (*get_property_list_func)(ExtensionScriptInstanceData* p_instance, uint32_t* r_count);
    void (*free_property_list_func)(ExtensionScriptInstanceData* p_instance, const PropertyInfo* p_list);

    VariantType (*get_property_type_func)(ExtensionScriptInstanceData* p_instance, const StringName* p_name,
                                          bool* r_is_valid);

    ObjectHandle (*get_owner_func)(ExtensionScriptInstanceData* p_instance);

    void (*get_property_state_func)(ExtensionScriptInstanceData* p_instance,
                                    void (*p_add_func)(const StringName* p_name, const Variant* p_value,
                                                       void* p_userdata),
                                    void* p_userdata);

    const MethodInfo* (*get_method_list_func)(ExtensionScriptInstanceData* p_instance, uint32_t* r_count);
    void (*free_method_list_func)(ExtensionScriptInstanceData* p_instance, const MethodInfo* p_list);

    bool (*has_method_func)(ExtensionScriptInstanceData* p_instance, const StringName* p_name);

    void (*call_func)(ExtensionScriptInstanceData* p_self, const StringName* p_method, const Variant** p_args,
                      const std::int64_t p_argument_count, Variant* r_return, CallError* r_error);
    void (*notification_func)(ExtensionScriptInstanceData* p_instance, int32_t p_what);
    const char* (*to_string_func)(ExtensionScriptInstanceData* p_instance, bool* r_is_valid);

    void (*refcount_incremented_func)(ExtensionScriptInstanceData* p_instance);
    bool (*refcount_decremented_func)(ExtensionScriptInstanceData* p_instance);

    ObjectHandle (*get_script_func)(ExtensionScriptInstanceData* p_instance);
    bool (*is_placeholder_func)(ExtensionScriptInstanceData* p_instance);

    decltype(set_func) set_fallback_func;
    decltype(get_func) get_fallback_func;

    ExtensionScriptLanguage* (*get_language_func)(ExtensionScriptInstanceData* p_instance);

    ExtensionScriptLanguage* (*free_func)(ExtensionScriptInstanceData* p_instance);
};
