

#include <any>
#include <cwchar>
#include <iostream>
#include <string>
#include "gdextension/core/Initialization.hxx"
#include "gdextension/object/Ptr.hxx"
#include "gdextension/object/Register.hxx"
#include "gdextension/variant/String.hxx"
#include "gdextension/variant/Variant.hxx"

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

        reg.register_method<"instance_method", [](Self* self, const String& text) {
            std::cout << "instance method!? " << self << "\n";

            for (const auto c : text)
                std::cout << static_cast<char>(c) << "\n";
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

    // variant_traits<Ptr<Object>>::from(Variant{});

    // for (auto c : variant) {
    //     std::cout << (char)c << "\n";
    // }

    // auto standard = std::string{something};

    // std::cout << standard << "\n";

    return true;
}
