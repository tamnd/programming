```cpp run id=shapes
#include <cstdio>
#include <numbers>
#include <variant>
#include <vector>

struct Circle { double radius; };
struct Rect { double width, height; };
struct Triangle { double base, height; };

using Shape = std::variant<Circle, Rect, Triangle>;

template <class... Fs>
struct overloaded : Fs... {
    using Fs::operator()...;
};

double area(const Shape& shape) {
    return std::visit(overloaded{
        [](const Circle& c) { return std::numbers::pi * c.radius * c.radius; },
        [](const Rect& r) { return r.width * r.height; },
        [](const Triangle& t) { return t.base * t.height / 2; },
    }, shape);
}

int main() {
    std::vector<Shape> shapes{Circle{1}, Rect{2, 3}, Triangle{4, 5}};
    for (const Shape& s : shapes) {
        std::printf("%.2f\n", area(s));
    }
    std::printf("index %zu\n", shapes[1].index());
    std::printf("rect? %d\n", std::holds_alternative<Rect>(shapes[1]));
    if (const Rect* r = std::get_if<Rect>(&shapes[1])) {
        std::printf("width %.1f\n", r->width);
    }
}
```

```output for=shapes
3.14
6.00
10.00
index 1
rect? 1
width 2.0
```

- Verdict: stdlib. `std::variant` holds a value of one of its alternative types, and `index` tells which one.
- `std::visit` calls the visitor with the current alternative. The call must be valid for every alternative, so a visitor that misses a type does not compile.
- `std::get` with the wrong alternative throws `std::bad_variant_access` (see Pitfalls on the concept page). `std::get_if` returns a null pointer instead.
