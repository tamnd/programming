```cpp run id=std-any
#include <any>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

int main() {
    std::vector<std::any> values{42, std::string("hi"), 3.5};
    for (const auto& v : values) {
        if (const int* n = std::any_cast<int>(&v)) {
            std::cout << "int " << *n + 1 << "\n";
        } else if (const auto* s = std::any_cast<std::string>(&v)) {
            std::cout << "string " << *s << "\n";
        } else {
            std::cout << "other " << (v.type() == typeid(double)) << "\n";
        }
    }

    std::any a = 7;
    try {
        std::cout << std::any_cast<std::string>(a) << "\n";
    } catch (const std::bad_any_cast&) {
        std::cout << "bad_any_cast\n";
    }
    a = std::string("text");
    std::cout << std::any_cast<std::string>(a) << " " << a.has_value() << "\n";
}
```

```output for=std-any
int 43
string hi
other 1
bad_any_cast
text 1
```

- Verdict: stdlib. C++ has no root class. `std::any` holds one value of any copyable type and stores its type, so `any_cast` to a pointer returns `nullptr` for the wrong type, and `any_cast` to a value throws `std::bad_any_cast`. `type()` gives the stored type. A `std::any` can take a value of another type later.
