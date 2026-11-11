```cpp run id=virtual
#include <iostream>
#include <memory>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual const char* name() const { return "shape"; }
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    const char* name() const override { return "circle"; }
};

class Square final : public Shape {
    double side_;
public:
    explicit Square(double side) : side_(side) {}
    double area() const override { return side_ * side_; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Square>(2.0));
    for (const auto& s : shapes) {
        std::cout << s->name() << " " << s->area() << "\n";
    }
}
```

```output for=virtual
circle 3.14159
shape 4
```

```cpp wrong id=slicing
#include <iostream>

struct Animal {
    virtual ~Animal() = default;
    virtual const char* speak() const { return "..."; }
};

struct Dog : Animal {
    const char* speak() const override { return "woof"; }
};

void by_value(Animal a) { std::cout << a.speak() << "\n"; }
void by_ref(const Animal& a) { std::cout << a.speak() << "\n"; }

int main() {
    Dog d;
    by_value(d);
    by_ref(d);
}
```

```output for=slicing
...
woof
```

- Verdict: native. C++ has classes, multiple inheritance, access control and `virtual` functions. Functions are not virtual by default, so a call costs no more than a plain call unless the class asks for dynamic dispatch.
- `override` asks the compiler to check that a base function with the same signature exists. `final` stops further derivation. A base class that is deleted through a pointer needs a virtual destructor.
- In the wrong block, `by_value` copies only the `Animal` part of the `Dog`. This is object slicing, and the copy has the `Animal` behavior. Pass polymorphic objects by reference or pointer.
