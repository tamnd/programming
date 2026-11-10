```cpp run id=casts
#include <print>
#include <stdexcept>
#include <string>
#include <typeinfo>

struct Shape {
    virtual ~Shape() = default;
};
struct Circle : Shape {
    double radius = 2.0;
};
struct Square : Shape {};

int main() {
    double d = 3.99;
    int i = static_cast<int>(d);
    long wide{i};
    std::println("{} {}", i, wide);

    Circle circle;
    Shape& shape = circle;
    if (auto* c = dynamic_cast<Circle*>(&shape)) {
        std::println("circle {}", c->radius);
    }
    std::println("square? {}", dynamic_cast<Square*>(&shape) != nullptr);
    try {
        [[maybe_unused]] Square& s = dynamic_cast<Square&>(shape);
    } catch (const std::bad_cast&) {
        std::println("bad_cast");
    }

    std::println("{}", std::stoi("42") + 1);
    try {
        std::stoi("abc");
    } catch (const std::invalid_argument&) {
        std::println("invalid_argument");
    }
}
```

```output for=casts
3 3
circle 2
square? false
bad_cast
43
invalid_argument
```

- Verdict: native. C++ has the implicit conversions of C, plus four named casts: `static_cast`, `dynamic_cast`, `const_cast` and `reinterpret_cast`.
- `dynamic_cast` checks a downcast at run time. A failed pointer cast gives `nullptr`, and a failed reference cast throws `std::bad_cast`.
- Braces reject a narrowing conversion, so `long wide{i}` compiles but `int n{3.99}` does not (see Pitfalls on the concept page).
- `std::stoi` parses text and throws `std::invalid_argument` when no conversion is possible.
