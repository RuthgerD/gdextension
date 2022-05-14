# gdextension++ (WIP: not ready, so don't look yet)

Abstracting away the ugly "GDExtensions" C api (and by extension some of Godot 🤐).

### Differences to [godot-cpp](https://github.com/godotengine/godot-cpp)
* Method binds and class registration are all done at compile time. (0 allocation)
* Reimagined Variant (&co) interfaces that try to conform to standard container requirements.
* Lean and mean, by default only the bare minimum is available; ideal if you just want to bind an external library and don't want to compile a dozens of unrelated godot class bindings.
* Let's you bind arbitrary functions, meta data derived from arguments (see first point)
* Hand written variant types, only objects will be code generated (if you need them)

### Goals
* Make writing C++ with godot actually enjoyable, inspired by the excellent godot-rust implementation.
* Fast compile times.
* Use C++20 and beyond, constexpr everything.

### A little taste:
Inside our class registration function:
```cpp
reg.register_method<"instance_method", [](Self* self, String text) {
    std::cout << "instance method!? " << self->instance_id() << "\n";

    for(auto c : text)
        std::cout << static_cast<char>(c) << "\n";
}>();

reg.register_method<"static_method", []() -> String {
    std::cout << "static method!?\n";

    return U"Yes we are unicode 🤖";
}>();
```
Yields us this in gdscript:
```ruby
func _ready():
    var a = BasicExtension.new()
    a.instance_method("an instance method")
    
    print(BasicExtension.static_method())
```