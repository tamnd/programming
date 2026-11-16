```cpp run id=monostate
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>

struct Empty {};

int main() {
    // std::monostate gives a variant an empty first alternative.
    std::variant<std::monostate, int, std::string> v;
    std::cout << v.index() << " " << std::holds_alternative<std::monostate>(v) << "\n";
    v = 7;
    std::cout << v.index() << "\n";
    std::cout << (std::monostate{} == std::monostate{}) << "\n";

    // void is an incomplete type, so no object can have it.
    std::cout << std::is_void_v<void> << " " << std::is_void_v<std::monostate> << "\n";

    // An empty struct has one value, but a complete object still takes one byte.
    std::cout << sizeof(Empty) << " " << sizeof(std::monostate) << "\n";
}
```

```output for=monostate
0 1
1
1
1 0
1 1
```

- Verdict: native. `void` works as in C, and `std::is_void_v` tells it apart from other types. `std::monostate` is a real unit type: it has one value, all its values are equal, and a default `std::variant` holds it, so `index()` is 0. An empty struct such as `Empty` is also a unit type, but each complete object has the size 1, so that two objects have two addresses.
