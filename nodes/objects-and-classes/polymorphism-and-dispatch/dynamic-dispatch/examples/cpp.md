```cpp run id=shapes
#include <cstdio>
#include <memory>
#include <vector>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual const char* name() const { return "shape"; }
    void describe() const { std::printf("%s with area %.2f\n", name(), area()); }
};

struct Circle final : Shape {
    double r;
    explicit Circle(double r) : r(r) {}
    double area() const override { return 3.141592653589793 * r * r; }
    const char* name() const override { return "circle"; }
};

struct Square final : Shape {
    double side;
    explicit Square(double side) : side(side) {}
    double area() const override { return side * side; }
};

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1));
    shapes.push_back(std::make_unique<Square>(2));
    for (const auto& s : shapes) s->describe();
    std::printf("%zu %zu\n", sizeof(double), sizeof(Square)); // the hidden vtable pointer
}
```

```output for=shapes
circle with area 3.14
shape with area 4.00
8 16
```

```cpp wrong id=slicing
#include <cstdio>

struct Shape {
    virtual ~Shape() = default;
    virtual const char* name() const { return "shape"; }
};

struct Circle : Shape {
    const char* name() const override { return "circle"; }
};

void by_reference(const Shape& s) { std::printf("by reference: %s\n", s.name()); }
void by_value(Shape s) { std::printf("by value: %s\n", s.name()); } // copies only the Shape part

int main()
{
    Circle c;
    by_reference(c);
    by_value(c);
}
```

```output for=slicing
by reference: circle
by value: shape
```

- Verdict: native. A member function declared `virtual` dispatches on the dynamic type of the object when the program calls it through a pointer or a reference. Each object of a class with virtual functions holds a hidden pointer to the vtable of its class, so `Square` takes 16 bytes for one `double`. `override` makes the compiler check that a function overrides something, and `final` lets it call the function directly.
- The wrong block passes a `Circle` by value as a `Shape`. The copy contains only the `Shape` part and has the vtable of `Shape`, so the call runs `Shape::name`. This is object slicing. Pass polymorphic objects by reference or by pointer, and delete the copy operations of a base class to make slicing a compile error.
