```cpp run id=max
#include <array>
#include <concepts>
#include <print>
#include <string>
#include <vector>

template <std::totally_ordered T>
T max_of(const std::vector<T>& items) {
    T result = items.front();
    for (const T& item : items) {
        if (item > result) {
            result = item;
        }
    }
    return result;
}

int main() {
    std::println("{}", max_of(std::vector{3, 9, 4}));
    std::println("{}", max_of(std::vector<std::string>{"pear", "apple", "plum"}));
    std::array<int, 3> fixed{1, 2, 3};
    std::println("{}", fixed.size());
}
```

```output for=max
9
plum
3
```

- Verdict: native. Function templates, class templates, variable templates and alias templates take template parameters.
- The compiler makes a separate specialization for each set of template arguments that the program uses.
- `std::totally_ordered T` is a concept. A call with a type that has no ordering fails at the call, not inside the body (see Pitfalls on the concept page).
- A template parameter can also be a value. In `std::array<int, 3>`, the size 3 is part of the type.
