```cpp run id=decltype-and-typeid
#include <iostream>
#include <print>
#include <type_traits>
#include <typeinfo>

struct Shape {
    virtual ~Shape() = default;
};
struct Circle : Shape {};

// enum class makes a new type that does not convert to int.
enum class Meters : int {};

int main() {
    // decltype gives the static type of an expression.
    int i = 1;
    static_assert(std::is_same_v<decltype(i + 1L), long>);
    static_assert(std::is_same_v<decltype('a' + 'b'), int>);
    std::println("{} {} {}", sizeof(i), sizeof(long long), sizeof(double));

    // auto takes the type of the initializer.
    auto u = 3u;
    auto x = 2.5f;
    std::println("{} {}", std::is_same_v<decltype(u), unsigned>, std::is_same_v<decltype(x), float>);

    // The static type is Shape, and the dynamic type is Circle.
    Circle c;
    Shape& s = c;
    std::println("{} {}", typeid(s) == typeid(Circle), typeid(s) == typeid(Shape));

    // A type alias is the same type, and an enum class is a new one.
    using Count = int;
    std::println("{} {}", std::is_same_v<Count, int>, std::is_same_v<Meters, int>);
    Meters m{5};
    std::println("{}", static_cast<int>(m) * 2);

    // Type traits ask questions about a type at compile time.
    std::println("{} {} {}", std::is_integral_v<char>, std::is_floating_point_v<int>, std::is_class_v<Circle>);
}
```

```output for=decltype-and-typeid
4 8 8
true true
true false
true false
10
true false true
```

- Verdict: native. `decltype` gives the static type of an expression, and `static_assert` checks it at compile time. `auto` takes the type of the initializer. `typeid` gives the dynamic type `Circle` through a reference of type `Shape&`. A `using` alias is the same type as `int`, and an `enum class` is a new type that needs `static_cast`. Type traits answer questions about a type.
