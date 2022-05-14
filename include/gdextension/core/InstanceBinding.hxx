#pragma once

struct InstanceBindingCallbacks {
    void* (*create_callback)(void* p_token, void* p_instance);
    void (*free_callback)(void* p_token, void* p_instance, void* p_binding);
    bool (*reference_callback)(void* p_token, void* p_binding, bool p_reference);
};

constexpr static auto noop_instance_binding_callback =
    InstanceBindingCallbacks{[](void*, void*) -> void* { return nullptr; }, [](void*, void*, void*) {},
                             [](void*, void*, bool) -> bool { return true; }};
