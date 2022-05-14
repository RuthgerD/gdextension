#include "gdextension/core/Interface.hxx"
#include "gdextension/object/Object.hxx"

std::uint64_t Object::instance_id() const { return interface.object_get_instance_id(obj_handle); }
