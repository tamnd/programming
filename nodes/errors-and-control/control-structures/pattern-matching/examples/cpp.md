```cpp run id=points
#include <cstdio>
#include <string>
#include <utility>

std::string describe(std::pair<int, int> point) {
    auto [x, y] = point;
    if (x == 0 && y == 0) return "origin";
    if (y == 0) return "on the x axis at " + std::to_string(x);
    if (x == 0) return "on the y axis at " + std::to_string(y);
    if (x == y) return "on the diagonal at " + std::to_string(x);
    return "at " + std::to_string(x) + ", " + std::to_string(y);
}

int main() {
    std::pair<int, int> points[] = {{0, 0}, {3, 0}, {0, -2}, {4, 4}, {1, 5}};
    for (auto point : points) {
        std::printf("(%d, %d): %s\n", point.first, point.second, describe(point).c_str());
    }
}
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
```

- Verdict: absent. Standard C++ has no `match`. A structured binding, `auto [x, y] = point`, takes a pair, a tuple, an array or a struct apart, and a chain of `if` tests does the matching.
- A structured binding must name each element. A count that does not match the type is a compile error.
- `std::visit` on a `std::variant` selects a branch by the type of the value. [[sum-types]] shows it.
