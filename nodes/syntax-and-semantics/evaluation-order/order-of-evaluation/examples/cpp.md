```cpp run id=cxx17
#include <iostream>
#include <map>
#include <string>

int f(const std::string& name, int value) {
    std::cout << "evaluate " << name << "\n";
    return value;
}

int main() {
    std::cout << f("a", 1) << " " << f("b", 2) << "\n";

    std::map<int, std::size_t> sizes;
    sizes[7] = sizes.size();
    std::cout << sizes[7] << "\n";
}
```

```output for=cxx17
evaluate a
1 evaluate b
2
0
```

- Verdict: absent. C++ has no general left-to-right rule. Most operands and all function arguments are evaluated in an unspecified order.
- C++17 added an order for some operators. In `E1 << E2` and `E1 >> E2`, `E1` is sequenced first. In `E1 = E2`, `E2` is sequenced first. In `E1[E2]` and `E1.E2`, `E1` is sequenced first.
- So `sizes.size()` runs before `sizes[7]` inserts the key, and the value is 0. Before C++17, the value could be 0 or 1.
- The output shows that `<<` orders only the operands that are chained. `f("a", 1)` is evaluated, then its value is printed, then `f("b", 2)` is evaluated.

```cpp norun reason=platform id=arguments
#include <iostream>

int f(const char* name) {
    std::cout << name;
    return 0;
}

void g(int, int) {}

int main() {
    g(f("a"), f("b"));
    std::cout << "\n";
}
```

The arguments of a call are still indeterminately sequenced in C++17. GCC on x86-64 usually prints `ba`, and Clang usually prints `ab`. Both are correct.
