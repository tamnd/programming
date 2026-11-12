```cpp run id=contextual-names-and-overloads
#include <iostream>

namespace geo {
double area(double r) { return 3.0 * r * r; }
}

// One name can name several functions; the types of the arguments choose one.
int twice(int x) { return 2 * x; }
double twice(double x) { return 2.0 * x; }

struct Base {
    virtual ~Base() = default;
    virtual int f() const { return 1; }
};
struct Derived final : Base {
    int f() const override { return 2; }
};

int main() {
    // final and override are names with a special meaning only in a class head
    // or after a member declarator, so they can still name variables.
    int final = 3, override = 4;
    std::cout << final + override << "\n";

    // and, or and not are alternative spellings of operators, not names.
    std::cout << std::boolalpha << (true and not false) << "\n";

    // A namespace adds a qualifier, so geo::area and a global area do not clash.
    std::cout << geo::area(1.0) << "\n";
    std::cout << twice(2) << " " << twice(2.5) << "\n";

    Derived d;
    const Base& b = d;
    std::cout << b.f() << " " << __func__ << "\n";
}
```

```output for=contextual-names-and-overloads
7
true
3
4 5
2 main
```

- Verdict: native. `final` and `override` have a special meaning only in some places, so they can name variables. `and`, `or` and `not` are alternative spellings of operators, not names. A namespace qualifies a name, and overloading lets one name refer to several functions that the types of the arguments choose between.
