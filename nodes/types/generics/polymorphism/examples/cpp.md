```cpp run id=virtual-template-overload
#include <iostream>
#include <memory>
#include <numbers>
#include <string>
#include <vector>

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
    explicit Circle(double r) : r(r) {}
    double area() const override { return std::numbers::pi * r * r; }
};

// A template: the compiler makes a copy for each type argument.
template <typename T>
T first(const std::vector<T>& items) {
    return items[0];
}

// Overloads: the compiler picks one by the types of the arguments.
std::string show(int x) { return "int " + std::to_string(x); }
std::string show(const std::string& x) { return "string " + x; }

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Square>(2));
    shapes.push_back(std::make_unique<Circle>(1));
    std::cout.precision(3);
    for (const auto& s : shapes) {
        std::cout << s->area() << '\n';
    }

    std::cout << first(std::vector<int>{3, 4}) << ' '
              << first(std::vector<std::string>{"a", "b"}) << '\n';
    std::cout << show(1) << ", " << show(std::string("a")) << '\n';
}
```

```output for=virtual-template-overload
4
3.14
3 a
int 1, string a
```

- Verdict: native. `area` is virtual, so the call through a `Shape` pointer runs the code of the object's class. `first` is a template, and the compiler makes `first<int>` and `first<std::string>`. The two `show` functions are overloads, and the compiler picks one by the type of the argument.
