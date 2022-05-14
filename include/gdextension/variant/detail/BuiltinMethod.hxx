#pragma once
#include <cstdint>
#include <type_traits>
#include "gdextension/core/Interface.hxx"
#include "gdextension/utils/FixedString.hxx"

template <class Signature, FixedString, std::uint64_t> struct builtin_method;

template <FixedString name, std::uint64_t hash, class Ret, class Self, class... Args>
struct builtin_method<Ret(Self, Args...), name, hash> {

    static Ret call(Self self, Args... args) {
        thread_local void (*fptr)(TypeT, const TypeT*, TypeT, int) = nullptr;
        if (!fptr && !(fptr = interface.variant_get_ptr_builtin_method(
                           std::remove_pointer_t<Self>::variant_type, name.data(), hash))) {
            throw 0;
        }

        const TypeT packed_args[] = {args..., nullptr};
        Ret ret;
        fptr(self, static_cast<const TypeT*>(packed_args), &ret, sizeof(packed_args) - 1);

        return ret;
    }
};