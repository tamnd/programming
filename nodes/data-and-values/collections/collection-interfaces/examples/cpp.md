```cpp run id=to-the-interface
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <string>
#include <vector>

int total(std::ranges::input_range auto&& xs) {
    int sum = 0;
    for (int x : xs) sum += x;
    return sum;
}

std::string describe(std::ranges::sized_range auto&& c) {
    return "size " + std::to_string(std::ranges::size(c)) + ", total " + std::to_string(total(c));
}

int main() {
    std::vector<int> list{1, 2, 3};
    std::set<int> set{4, 5};
    std::map<std::string, int> prices{{"tea", 3}, {"cake", 4}};

    std::cout << "list: " << describe(list) << '\n';
    std::cout << "set: " << describe(set) << '\n';
    std::cout << "map values: " << describe(prices | std::views::values) << '\n';
}
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
```

- Verdict: stdlib. C++ names collection interfaces as concepts, such as `std::ranges::input_range` and `std::ranges::sized_range`, and as named requirements such as Container. The compiler checks them on template arguments.
- `prices | std::views::values` is a view of the values of the map. It is a sized range, so `describe` takes it.
