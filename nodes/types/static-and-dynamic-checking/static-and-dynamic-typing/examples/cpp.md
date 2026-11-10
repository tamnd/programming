```cpp run id=types
#include <any>
#include <print>
#include <string>

struct Shape {
    virtual ~Shape() = default;
};
struct Circle : Shape {
    double radius = 1.5;
};
struct Square : Shape {};

int main() {
    Circle circle;
    Square square;
    Shape* shapes[] = {&circle, &square};
    for (Shape* shape : shapes) {
        if (auto* c = dynamic_cast<Circle*>(shape)) {
            std::println("circle, radius {}", c->radius);
        } else {
            std::println("not a circle");
        }
    }

    std::any box = std::string("abc");
    try {
        std::println("{}", std::any_cast<int>(box));
    } catch (const std::bad_any_cast&) {
        std::println("bad_any_cast");
    }
    std::println("{}", std::any_cast<std::string>(box));
}
```

```output
circle, radius 1.5
not a circle
bad_any_cast
abc
```

- Verdict: native. C++ is statically typed. `auto` infers the type of a variable at compile time.
- `dynamic_cast` checks the class of an object of a polymorphic class at run time. A failed cast to a pointer gives `nullptr`, and a failed cast to a reference throws `std::bad_cast`.
- `std::any` holds a value of any copyable type together with its type. `std::any_cast` to the wrong type throws `std::bad_any_cast`.
- `static_cast` and `reinterpret_cast` do no check at run time.
