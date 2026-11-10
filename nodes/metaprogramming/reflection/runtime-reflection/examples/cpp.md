```cpp run id=rtti
#include <iostream>
#include <memory>
#include <typeinfo>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    double area() const override { return side * side; }
};

struct Circle : Shape {
    double r;
    explicit Circle(double radius) : r(radius) {}
    double area() const override { return 3.0 * r * r; }
};

int main() {
    std::unique_ptr<Shape> shapes[] = {std::make_unique<Square>(2.0), std::make_unique<Circle>(1.0)};
    for (const auto& s : shapes) {
        const Shape& ref = *s;
        std::cout << "is a Square: " << std::boolalpha << (typeid(ref) == typeid(Square));
        if (auto* sq = dynamic_cast<const Square*>(&ref)) {
            std::cout << ", side = " << sq->side;
        }
        std::cout << '\n';
    }
}
```

```output for=rtti
is a Square: true, side = 2
is a Square: false
```

`typeid` on a reference to a polymorphic class gives the dynamic type, and `dynamic_cast` tests it. Neither can list the members of `Square`. The text of `typeid(...).name()` is implementation-defined, so this program compares `type_info` objects instead of printing names.

- Verdict: absent. C++ has run-time type identification (`typeid`, `std::type_info` and `dynamic_cast`) but no run-time access to fields or methods by name.
- Programs that need the members of a class write the list by hand or generate it with a tool.
