

#include <any>
#include <cwchar>
#include <iostream>
#include <string>
#include "gdextension/core/Initialization.hxx"
#include "gdextension/object/Ptr.hxx"
#include "gdextension/object/Register.hxx"
#include "gdextension/variant/Vector.hxx"
#include "gdextension/variant/VectorTraits.hxx"

#include "gdextension/variant/String.hxx"
#include "gdextension/variant/Variant.hxx"
#include "gdextension/variant/detail/VariantOps.hxx"

template <class T, FixedString name, PropertyHint hint, FixedString hint_string, PropertyUsageFlags usage>
struct Type {};

struct BasicExtension : public Extends<"BasicExtension", RefCounted> {
    using Self = BasicExtension;

    std::string str;
    int specific = 420;

    // using Extension::Extension;
    BasicExtension(ObjectHandle owner)
        : Extends{owner}, str{"this is a long string that needs to be allocated"} {
        std::cout << "BasicExtension created at " << this << '\n';
    }

    void class_func() {}

    ~BasicExtension() { std::cout << name.data() << " deleted! with " << instance_id() << "\n"; }

    static void register_class(Register<Self>& reg) {
        // b.register_method<"something_fun">(+[](This* self) {
        //     // do something?
        // });

        reg.register_method<"instance_method", [](Self* self, String text, Vector<Variant> arr) {
            std::cout << "instance method!? " << self << "\n";

            std::cout << static_cast<std::string>(text) << "\n";

            for (const auto& str : arr) {
                std::cout << static_cast<std::string>(static_cast<String>(str)) << "\n";
            }
        }>();

        reg.register_method<"static_method", [](Ptr<Object> obj) -> String {
            std::cout << "static method!? " << obj->instance_id() << "\n";

            return U"Yes we are unicode 🤖";
        }>();

        // b.register_method<"something_else">(&This::class_func);
    }
};

extern "C" auto extension(const Interface* iface, const ExtensionClassLibrary* lib,
                          Initialization* initialization) -> bool {

    initialization->minimum_initialization_level = Initialization::Level::CORE;
    initialization->initialize = [](auto, auto level) {
        std::cout << "Initialize at " << static_cast<int>(level) << "\n";

        if (Initialization::Level::SCENE == level) {
            Register{&BasicExtension::register_class};

            // auto object = Ptr<BasicExtension>(std::in_place_t{});

            // std::cout << "instance id:" << object->instance_id() << "\n";

            // std::cout << "ext: " << object->str << "\n";
        }
    };

    initialization->deinitialize = [](auto, auto level) {
        std::cout << "Deinitialize at " << static_cast<int>(level) << "\n";
    };

    interface = *iface;
    library = lib;

    auto something = String{U"Hello world!"};

    auto arr = Vector<String>{};

    arr.push_back(U"12345");
    arr.push_back(U"12345");
    arr.emplace_back(U"12345");

    (void)arr;

    for (const auto& str : arr) {
        std::cout << static_cast<std::string>(str) << "\n";
    }

    return true;
}
