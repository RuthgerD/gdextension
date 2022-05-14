#include "gdextension/core/Interface.hxx"
#include "gdextension/variant/Variant.hxx"

Variant::Variant() { interface.variant_new_nil(this); }
Variant::Variant(const Variant& rhs) { interface.variant_new_copy(this, &rhs); }
Variant& Variant::operator=(const Variant& rhs) {
    interface.variant_new_copy(this, &rhs);
    return *this;
}
