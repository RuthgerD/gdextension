#include "gdextension/core/Extension.hxx"
#include "gdextension/core/Interface.hxx"
#include "gdextension/object/RefCounted.hxx"

#include <cstdint>
#include <type_traits>

constexpr std::uint32_t hash_djb2_one_32(std::uint32_t p_in, std::uint32_t p_prev = 5381) {
    return ((p_prev << 5) + p_prev) + p_in;
}

template <class Signature, FixedString> struct object_method;

template <FixedString name, class Ret, class Self, class... Args>
struct object_method<Ret(Self, Args...), name> {

    static Ret call(ObjectHandle self, Args... args) {
        constexpr auto hash = []() {
            auto h = hash_djb2_one_32(true);          // has return
            h = hash_djb2_one_32(sizeof...(args), h); // argc
            h = hash_djb2_one_32(0, h);               // default args
            h = hash_djb2_one_32(false, h);           // const
            h = hash_djb2_one_32(false, h);           // vararg

            return h;
        }();

        thread_local MethodBind* method_bind = nullptr;
        if (!method_bind &&
            !(method_bind = interface.classdb_get_method_bind(Self::name.data(), name.data(), hash))) {
            throw 0;
        }

        const TypeT packed_args[] = {args..., nullptr};
        Ret ret;
        interface.object_method_bind_ptrcall(method_bind, self, static_cast<const TypeT*>(packed_args), &ret);

        // static_assert(h == hash);

        return ret;
    }
};

template <std::uint64_t hash, std::derived_from<Object> This, class Ret, class... Args> auto something() {
    return []() {};
}

template <class Ret, class Args, auto... DefaultArgs> class sig {};

bool RefCounted::init_ref() {
    // thread_local auto func = something<135338150, RefCounted, bool>;

    return object_method<bool(RefCounted), "init_ref">::call(handle());
}

bool RefCounted::reference() { return object_method<bool(RefCounted), "reference">::call(handle()); }
bool RefCounted::unreference() { return object_method<bool(RefCounted), "unreference">::call(handle()); }
