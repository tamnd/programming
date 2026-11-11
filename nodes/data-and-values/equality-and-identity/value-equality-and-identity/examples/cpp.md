```cpp run id=points
#include <cmath>
#include <cstring>
#include <iostream>
#include <set>
#include <string>

struct Point {
    int x;
    int y;
    bool operator==(const Point&) const = default;
    auto operator<=>(const Point&) const = default;
};

int main() {
    Point a{1, 2};
    Point b{1, 2};
    Point* pc = &a;
    std::cout << std::boolalpha;
    std::cout << "a == b: " << (a == b) << "\n";
    std::cout << "&a == &b: " << (&a == &b) << "\n";
    std::cout << "&a == pc: " << (&a == pc) << "\n";
    std::set<Point> points{a, b, *pc};
    std::cout << "distinct points: " << points.size() << "\n";

    std::string s = "hello";
    char buffer[] = "hello";
    const char* literal = "hello";
    std::cout << "s == literal: " << (s == literal) << "\n";
    std::cout << "buffer == literal: " << (buffer == literal) << "\n";
    std::cout << "strcmp(buffer, literal) == 0: " << (std::strcmp(buffer, literal) == 0) << "\n";

    double nan = std::nan("");
    std::cout << "nan == nan: " << (nan == nan) << "\n";
}
```

```output for=points
a == b: true
&a == &b: false
&a == pc: true
distinct points: 1
s == literal: true
buffer == literal: false
strcmp(buffer, literal) == 0: true
nan == nan: false
```

- Verdict: native. A class can declare `operator==` as defaulted. The defaulted operator compares the subobjects in order and stops at the first pair that is not equal. Comparing addresses with `&a == &b` is identity.
- `std::string` compares characters, also against a `const char*`. Two `const char*` or array operands compare addresses.
- `std::set` uses `<`, not `==`. Two elements are the same when neither is less than the other. The defaulted `<=>` gives `Point` that order.
