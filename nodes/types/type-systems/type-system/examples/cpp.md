```cpp run id=concepts-and-checked-casts
#include <any>
#include <concepts>
#include <print>
#include <string>
#include <variant>

struct Shape {
    virtual ~Shape() = default;
};
struct Circle : Shape {};
struct Square : Shape {};

// A concept states what a template needs, and the compiler checks it.
template <std::integral T>
T twice(T x) { return x * 2; }

int main() {
    std::println("{} {}", twice(21), twice(10L));
    // twice(1.5) does not compile: double does not satisfy std::integral.

    // dynamic_cast checks the dynamic type at run time.
    Circle c;
    Shape* s = &c;
    std::println("{} {}", dynamic_cast<Circle*>(s) != nullptr, dynamic_cast<Square*>(s) != nullptr);

    // std::variant holds one of several types, and std::get checks which.
    std::variant<int, std::string> v = std::string("five");
    std::println("{} {}", v.index(), std::holds_alternative<std::string>(v));
    try {
        std::println("{}", std::get<int>(v));
    } catch (const std::bad_variant_access&) {
        std::println("bad_variant_access");
    }

    // std::any holds a value of any type, and any_cast checks the type.
    std::any a = 3.5;
    try {
        std::println("{}", std::any_cast<int>(a));
    } catch (const std::bad_any_cast&) {
        std::println("bad_any_cast {}", std::any_cast<double>(a));
    }

    // Implicit conversions from C remain: a double converts to int.
    double d = 3.99;
    int n = d;
    std::println("{}", n);
}
```

```output for=concepts-and-checked-casts
42 20
true false
1 true
bad_variant_access
bad_any_cast 3.5
3
```

- Verdict: native. A concept such as `std::integral` states what a template needs, and the compiler rejects other types. `dynamic_cast` checks the dynamic type and gives `nullptr` when it does not match. `std::get` on a `std::variant` and `std::any_cast` check the type at run time and throw `bad_variant_access` and `bad_any_cast`. The implicit conversions of C remain, so a `double` converts to `int`.
