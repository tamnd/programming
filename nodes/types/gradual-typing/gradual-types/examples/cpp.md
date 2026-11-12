```cpp run id=std-any
#include <any>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::any> values{21, std::string("hello"), 1.5};
    for (const auto& v : values) {
        if (const int* n = std::any_cast<int>(&v)) {
            std::cout << "int, doubled " << *n * 2 << '\n';
        } else if (const auto* s = std::any_cast<std::string>(&v)) {
            std::cout << "string of length " << s->size() << '\n';
        } else {
            std::cout << "other type\n";
        }
    }
    try {
        std::cout << std::any_cast<int>(values[1]) << '\n';
    } catch (const std::bad_any_cast&) {
        std::cout << "bad_any_cast: the value is not an int\n";
    }
}
```

```output for=std-any
int, doubled 42
string of length 5
other type
bad_any_cast: the value is not an int
```

- Verdict: stdlib. `std::any`, from C++17, holds one value of any copyable type and remembers its type. `std::any_cast` on a pointer returns null when the type does not match, and on a value it throws `std::bad_any_cast`. `std::variant` is the alternative when the set of types is known.
