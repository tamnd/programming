```cpp run id=shared-parent
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

// An object: own slots and a shared pointer to a parent.
struct Obj {
    using Method = std::function<std::string(const Obj&)>;
    using Slot = std::variant<int, Method>;

    std::shared_ptr<const Obj> parent;
    std::map<std::string, Slot> slots;

    const Slot& get(const std::string& name) const
    {
        for (const Obj* o = this; o != nullptr; o = o->parent.get()) {
            if (auto it = o->slots.find(name); it != o->slots.end())
                return it->second;
        }
        throw std::out_of_range("no slot " + name);
    }

    int num(const std::string& name) const { return std::get<int>(get(name)); }
    std::string send(const std::string& name) const { return std::get<Method>(get(name))(*this); }
};

int main()
{
    auto traits = std::make_shared<Obj>();
    traits->slots["describe"] = Obj::Method([](const Obj& o) {
        return "(" + std::to_string(o.num("x")) + ", " + std::to_string(o.num("y")) + ")";
    });
    auto origin = std::make_shared<Obj>(Obj{traits, {{"x", 0}, {"y", 0}}});
    Obj p{origin, {{"x", 3}}};  // p delegates to origin

    std::cout << origin->send("describe") << ' ' << p.send("describe") << '\n';
    traits->slots["describe"] = Obj::Method([](const Obj& o) {
        return "<" + std::to_string(o.num("x")) + " " + std::to_string(o.num("y")) + ">";
    });
    std::cout << origin->send("describe") << ' ' << p.send("describe") << '\n';

    Obj copy = p;  // cloning copies the own slots and shares the parent
    copy.slots["x"] = 9;
    std::cout << copy.send("describe") << ' ' << p.send("describe") << '\n';
}
```

```output for=shared-parent
(0, 0) (3, 0)
<0 0> <3 0>
<9 0> <3 0>
```

- Verdict: absent. C++ classes fix the members of their objects at compile time. A prototype object holds a map of slots, with `std::variant` for the kinds of value and `std::function` for methods, and a `std::shared_ptr` to its parent. Copying the struct copies the own slots and shares the parent, which is the clone operation of a prototype language: the copy changes its own `x`, and both objects still use the one `describe` of the shared traits object.
