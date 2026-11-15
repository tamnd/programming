```cpp run id=pair
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

std::pair<int, int> min_max(const std::vector<int>& values) {
    auto [lo, hi] = std::minmax_element(values.begin(), values.end());
    return {*lo, *hi};
}

int main() {
    auto pair = min_max({7, 2, 9, 4});
    std::cout << "pair: (" << pair.first << ", " << pair.second << ")\n";
    auto [lo, hi] = pair;
    std::cout << "lo: " << lo << " hi: " << hi << "\n";
    std::cout << "second part: " << std::get<1>(pair) << "\n";
    std::cout << "equal pairs: " << (pair == std::pair{2, 9} ? "yes" : "no") << "\n";

    std::tuple<std::string, int, bool> t{"Ada", 36, true};
    std::cout << "tuple size: " << std::tuple_size_v<decltype(t)> << " first: " << std::get<0>(t) << "\n";
}
```

```output for=pair
pair: (2, 9)
lo: 2 hi: 9
second part: 9
equal pairs: yes
tuple size: 3 first: Ada
```

- Verdict: native. `std::pair` holds two values, and `std::tuple` holds any fixed number of values.
- `auto [lo, hi] = pair` is a structured binding (C++17). `std::get<1>` reads a part by position.
- `==` compares the parts in order.
