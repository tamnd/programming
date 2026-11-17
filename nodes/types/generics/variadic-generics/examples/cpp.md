```cpp run id=packs-and-folds
#include <iostream>
#include <string>
#include <tuple>

// A fold expression over a pack of values.
template <typename... Ts>
auto sum(Ts... xs) {
    return (xs + ... + 0);
}

template <typename... Ts>
void print_all(const Ts&... xs) {
    ((std::cout << xs << " "), ...);
    std::cout << "(" << sizeof...(Ts) << " values)\n";
}

int main() {
    std::cout << sum(1, 2, 3.5) << " " << sum() << "\n";
    print_all(1, std::string("two"), 3.5);
    std::tuple<int, char, double> t{1, 'x', 2.5};
    auto n = std::apply([](auto... xs) { return sizeof...(xs); }, t);
    std::cout << n << "\n";
}
```

```output for=packs-and-folds
6.5 0
1 two 3.5 (3 values)
3
```

- Verdict: native. `typename... Ts` is a template parameter pack and `Ts... xs` is a function parameter pack. `(xs + ... + 0)` folds the pack with `+`, and `sizeof...` counts its elements. `std::apply` expands a tuple into a pack of arguments.
