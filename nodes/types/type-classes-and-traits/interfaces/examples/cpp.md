```cpp run id=abstract-class
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

struct Shape {
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
    std::string describe() const { return name() + " " + std::to_string(area()); }
};

struct Square : Shape {
    explicit Square(double s) : side(s) {}
    double area() const override { return side * side; }
    std::string name() const override { return "square"; }
    double side;
};

struct Circle : Shape {
    explicit Circle(double r) : radius(r) {}
    double area() const override { return 3.141592653589793 * radius * radius; }
    std::string name() const override { return "circle"; }
    double radius;
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Square>(2));
    shapes.push_back(std::make_unique<Circle>(1));
    for (const auto& shape : shapes) {
        std::cout << shape->describe() << "\n";
    }
    std::cout << std::is_abstract_v<Shape> << " " << std::is_abstract_v<Square> << "\n";
}
```

```output
square 4.000000
circle 3.141593
1 0
```

```cpp run id=concept
#include <concepts>
#include <iostream>
#include <vector>

template <typename T>
concept HasArea = requires(const T& t) {
    { t.area() } -> std::convertible_to<double>;
};

struct Square {
    double side;
    double area() const { return side * side; }
};

struct Rect {
    double w, h;
    double area() const { return w * h; }
};

template <HasArea T>
double total(const std::vector<T>& items) {
    double sum = 0;
    for (const T& item : items) {
        sum += item.area();
    }
    return sum;
}

static_assert(HasArea<Square>);
static_assert(!HasArea<int>);

int main() {
    std::cout << total(std::vector<Square>{{1}, {2}}) << "\n";
    std::cout << total(std::vector<Rect>{{2, 3}}) << "\n";
}
```

```output
5
6
```

- Verdict: native. C++ has no `interface` keyword. A class with only pure virtual functions serves as an interface. A derived class must override them all before a program can create an object of it.
- `describe` is an ordinary member function of the base. It calls the virtual functions, so it acts as a default method.
- A concept states requirements on a template argument. `Square` and `Rect` derive from nothing, and `total` accepts both. The check happens at compile time, and each call to `total` is a separate instantiation with direct calls.
- A concept gives no run-time dispatch. A `std::vector<HasArea>` does not exist. Mixed types in one container need a virtual base class or type erasure.
