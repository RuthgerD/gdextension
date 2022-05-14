#pragma once

#include <cstdint>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "gdextension/utils/TypeT.hxx"

#include "gdextension/core/CallError.hxx"
#include "gdextension/core/Extension.hxx"
#include "gdextension/core/ExtensionScript.hxx"
#include "gdextension/core/InstanceBinding.hxx"
#include "gdextension/object/ObjectHandle.hxx"

#include "gdextension/variant/Array.hxx"
#include "gdextension/variant/Color.hxx"
#include "gdextension/variant/Dictionary.hxx"
#include "gdextension/variant/PackedArray.hxx"
#include "gdextension/variant/String.hxx"
#include "gdextension/variant/Vector2.hxx"
#include "gdextension/variant/Vector3.hxx"

#include "gdextension/variant/Variant.hxx"

class StringName;
class MethodBind;

// clang-format off
struct Interface {
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    const char* version_string;

    /* GODOT CORE */
    void* (*mem_alloc)(size_t bytes);
    void* (*mem_realloc)(void* ptr, size_t bytes);
    void (*mem_free)(void* ptr);

    void (*print_error)(const char* description, const char* function, const char* file, int32_t line);
    void (*print_warning)(const char* description, const char* function, const char* file, int32_t line);
    void (*print_script_error)(const char* description, const char* function, const char* file, int32_t line);

    uint64_t (*get_native_struct_size)(const char* name);

    /* GODOT VARIANT */
    void (*variant_new_copy)(Variant* r_dest, const Variant* src);
    void (*variant_new_nil)(Variant* r_dest);
    void (*variant_destroy)(Variant* self);

    void (*variant_call)(Variant* self, const StringName* method, const Variant** args, const std::int64_t argument_count, Variant* r_return, CallError* r_error);
    void (*variant_call_static)(VariantType type, const StringName* method, const Variant** args, const std::int64_t argument_count, Variant* r_return, CallError* r_error);

    void (*variant_evaluate)(VariantOperator op, const Variant* a, const Variant* b, Variant* r_return, bool* r_valid);

    void (*variant_set)(Variant* self, const Variant* key, const Variant* value, bool* r_valid);
    void (*variant_set_named)(Variant* self, const StringName* key, const Variant* value, bool* r_valid);
    void (*variant_set_keyed)(Variant* self, const Variant* key, const Variant* value, bool* r_valid);
    void (*variant_set_indexed)(Variant* self, std::int64_t index, const Variant* value, bool* r_valid, bool* r_oob);

    void (*variant_get)(const Variant* self, const Variant* key, Variant* r_ret, bool* r_valid);
    void (*variant_get_named)(const Variant* self, const StringName* key, Variant* r_ret, bool* r_valid);
    void (*variant_get_keyed)(const Variant* self, const Variant* key, Variant* r_ret, bool* r_valid);
    void (*variant_get_indexed)(const Variant* self, std::int64_t index, Variant* r_ret, bool* r_valid, bool* r_oob);

    bool (*variant_iter_init)(const Variant* self, Variant* r_iter, bool* r_valid);
    bool (*variant_iter_next)(const Variant* self, Variant* r_iter, bool* r_valid);
    void (*variant_iter_get)(const Variant* self, Variant* r_iter, Variant* r_ret, bool* r_valid);
    bool (*variant_hash_compare)(const Variant* self, const Variant* other);
    bool (*variant_booleanize)(const Variant* self);
    void (*variant_sub)(const Variant* a, const Variant* b, Variant* r_dst);
    void (*variant_blend)(const Variant* a, const Variant* b, float c, Variant* r_dst);
    void (*variant_interpolate)(const Variant* a, const Variant* b, float c, Variant* r_dst);
    void (*variant_duplicate)(const Variant* self, Variant* r_ret, bool deep);
    void (*variant_stringify)(const Variant* self, String* r_ret);

    VariantType (*variant_get_type)(const Variant* self);
    bool (*variant_has_method)(const Variant* self, const StringName* method);
    bool (*variant_has_member)(VariantType type, const StringName* member);
    bool (*variant_has_key)(const Variant* self, const Variant* key, bool* r_valid);
    void (*variant_get_type_name)(VariantType type, String* r_name);
    bool (*variant_can_convert)(VariantType from, VariantType to);
    bool (*variant_can_convert_strict)(VariantType from, VariantType to);

    auto (*get_variant_from_type_constructor)(VariantType type) -> void (*)(Variant*, TypeT);
    auto (*get_variant_to_type_constructor)(VariantType type) -> void (*)(TypeT, Variant*);

    auto (*variant_get_ptr_operator_evaluator)(VariantOperator op, VariantType type_a, VariantType type_b) -> void (*)(const TypeT left, const TypeT right, TypeT r_result);

    auto (*variant_get_ptr_builtin_method)(VariantType type, const char* method, std::int64_t hash) -> void (*)(TypeT base, const TypeT* args, TypeT r_return, int argument_count);
    
    auto (*variant_get_ptr_constructor)(VariantType type, int32_t constructor) -> void (*)(TypeT base, const TypeT* args);
    auto (*variant_get_ptr_destructor)(VariantType type) -> void (*)(TypeT base);
    void (*variant_construct)(VariantType type, Variant* base, const Variant** args, int32_t argument_count, CallError* r_error);

    auto (*variant_get_ptr_setter)(VariantType type, const char* member) -> void (*)(TypeT base, const TypeT value);
    auto (*variant_get_ptr_getter)(VariantType type, const char* member) -> void (*)(const TypeT base, TypeT r_value);
    auto (*variant_get_ptr_indexed_setter)(VariantType type) -> void (*)(TypeT base, std::int64_t index, const TypeT value);
    auto (*variant_get_ptr_indexed_getter)(VariantType type) -> void (*)(const TypeT base, std::int64_t index, TypeT r_value);
    auto (*variant_get_ptr_keyed_setter)(VariantType type) -> void (*)(TypeT base, const TypeT key, const TypeT value);
    auto (*variant_get_ptr_keyed_getter)(VariantType type) -> void (*)(const TypeT base, const TypeT key, TypeT r_value);
    auto (*variant_get_ptr_keyed_checker)(VariantType type) -> uint32_t (*)(const Variant* base, const Variant* key);
    void (*variant_get_constant_value)(VariantType type, const char* constant, Variant* r_ret);
    auto (*variant_get_ptr_utility_function)(const char* function, std::int64_t hash) -> void (*)(TypeT r_return, const TypeT* arguments, int argument_count);

    /*  extra utilities */
    void (*string_new_with_latin1_chars)(String* r_dest, const char* contents);
    void (*string_new_with_utf8_chars)(String* r_dest, const char* contents);
    void (*string_new_with_utf16_chars)(String* r_dest, const char16_t* contents);
    void (*string_new_with_utf32_chars)(String* r_dest, const char32_t* contents);
    void (*string_new_with_wide_chars)(String* r_dest, const wchar_t* contents);

    void (*string_new_with_latin1_chars_and_len)(String* r_dest, const char* contents, const std::int64_t size);
    void (*string_new_with_utf8_chars_and_len)(String* r_dest, const char* contents, const std::int64_t size);
    void (*string_new_with_utf16_chars_and_len)(String* r_dest, const char16_t* contents, const std::int64_t size);
    void (*string_new_with_utf32_chars_and_len)(String* r_dest, const char32_t* contents, const std::int64_t size);
    void (*string_new_with_wide_chars_and_len)(String* r_dest, const wchar_t* contents, const std::int64_t size);

    std::int64_t (*string_to_latin1_chars)(const String* self, char* r_text, std::int64_t max_write_length);
    std::int64_t (*string_to_utf8_chars)(const String* self, char* r_text, std::int64_t max_write_length);
    std::int64_t (*string_to_utf16_chars)(const String* self, char16_t* r_text, std::int64_t max_write_length);
    std::int64_t (*string_to_utf32_chars)(const String* self, char32_t* r_text, std::int64_t max_write_length);
    std::int64_t (*string_to_wide_chars)(const String* self, wchar_t* r_text, std::int64_t max_write_length);

    char32_t* (*string_operator_index)(String* self, std::int64_t index);
    const char32_t* (*string_operator_index_const)(const String* self, std::int64_t index);

    /* Packed array functions */
    uint8_t* (*packed_byte_array_operator_index)(PackedArray<std::uint8_t>* self, std::int64_t index);
    const uint8_t* (*packed_byte_array_operator_index_const)(const PackedArray<std::uint8_t>* self, std::int64_t index);

    Color* (*packed_color_array_operator_index)(PackedArray<Color>* self, std::int64_t index);
    Color* (*packed_color_array_operator_index_const)(const PackedArray<Color>* self, std::int64_t index);

    float* (*packed_float32_array_operator_index)(PackedArray<float>* self, std::int64_t index);
    const float* (*packed_float32_array_operator_index_const)(const PackedArray<float>* self, std::int64_t index);
    double* (*packed_float64_array_operator_index)(PackedArray<double>* self, std::int64_t index);
    const double* (*packed_float64_array_operator_index_const)(const PackedArray<double>* self, std::int64_t index);

    int32_t* (*packed_int32_array_operator_index)(PackedArray<std::int32_t>* self, std::int64_t index);
    const int32_t* (*packed_int32_array_operator_index_const)(const PackedArray<std::int32_t>* self, std::int64_t index);
    int64_t* (*packed_int64_array_operator_index)(PackedArray<std::int64_t>* self, std::int64_t index);
    const int64_t* (*packed_int64_array_operator_index_const)(const PackedArray<std::int64_t>* self, std::int64_t index);

    String* (*packed_string_array_operator_index)(PackedArray<String>* self, std::int64_t index);
    String* (*packed_string_array_operator_index_const)(const PackedArray<String>* self, std::int64_t index);

    Vector2* (*packed_vector2_array_operator_index)(PackedArray<Vector2>* self, std::int64_t index);
    Vector2* (*packed_vector2_array_operator_index_const)(const PackedArray<Vector2>* self, std::int64_t index);
    Vector3* (*packed_vector3_array_operator_index)(PackedArray<Vector3>* self, std::int64_t index);
    Vector3* (*packed_vector3_array_operator_index_const)(const PackedArray<Vector3>* self, std::int64_t index);

    Variant* (*array_operator_index)(Array* self, std::int64_t index);
    Variant* (*array_operator_index_const)(const Array* self, std::int64_t index);

    Variant* (*dictionary_operator_index)(Dictionary* self, const Variant* key);
    Variant* (*dictionary_operator_index_const)(const Dictionary* self, const Variant* key);

    /* OBJECT */

    void (*object_method_bind_call)(const MethodBind* method_bind, ObjectHandle instance, const Variant** args, std::int64_t arg_count, Variant* r_ret, CallError* r_error);
    void (*object_method_bind_ptrcall)(const MethodBind* method_bind, ObjectHandle instance, const void* args, void* r_ret);
    void (*object_destroy)(ObjectHandle o);

    ObjectHandle (*global_get_singleton)(const char* name);

    void* (*object_get_instance_binding)(ObjectHandle o, void* token, const InstanceBindingCallbacks* callbacks);
    void (*object_set_instance_binding)(ObjectHandle o, void* token, void* binding, const InstanceBindingCallbacks* callbacks);
    void (*object_set_instance)(ObjectHandle o, const char* classname, GDExtensionClassInstance* instance);

    ObjectHandle (*object_cast_to)(const ObjectHandle object, void* class_tag);
    ObjectHandle (*object_get_instance_from_id)(std::uint64_t instance_id);
    std::uint64_t (*object_get_instance_id)(const ObjectHandle object);

    /* SCRIPT INSTANCE */
    ScriptInstance* (*script_instance_create)(const ExtensionScriptInstanceInfo* info, ExtensionScriptInstanceData* instance_data);

    /* CLASSDB */
    ObjectHandle (*classdb_construct_object)(const char* classname);
    MethodBind* (*classdb_get_method_bind)(const char* classname, const char* methodname, std::int64_t hash);

    void* (*classdb_get_class_tag)(const char* classname);
    void (*classdb_register_extension_class)(const ExtensionClassLibrary* library, const char* class_name, const char* parent_class_name, const ExtensionClassCreationInfo* extension_funcs);
    void (*classdb_register_extension_class_method)(const ExtensionClassLibrary* library, const char* class_name, const ExtensionClassMethodInfo* method_info);
    void (*classdb_register_extension_class_integer_constant)(const ExtensionClassLibrary* library, const char* class_name, const char* enum_name, const char* constant_name, std::int64_t constant_value);
    void (*classdb_register_extension_class_property)(const ExtensionClassLibrary* library, const char* class_name, const PropertyInfo* info, const char* setter, const char* getter);
    void (*classdb_register_extension_class_property_group)(const ExtensionClassLibrary* library, const char* class_name, const char* grouname, const char* prefix);
    void (*classdb_register_extension_class_property_subgroup)(const ExtensionClassLibrary* library, const char* class_name, const char* subgrouname, const char* prefix);
    void (*classdb_register_extension_class_signal)(const ExtensionClassLibrary* library, const char* class_name, const char* signal_name, const PropertyInfo* argument_info, std::int64_t argument_count);
    void (*classdb_unregister_extension_class)(const ExtensionClassLibrary* library, const char* class_name);
};

extern Interface interface;
extern const ExtensionClassLibrary* library;

inline auto token() {
    return reinterpret_cast<void*>(const_cast<ExtensionClassLibrary*>(library));
}

