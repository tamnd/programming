```cpp run id=hash-for-unordered-set
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>

struct Point {
    int x;
    int y;
    bool operator==(const Point&) const = default;
};

// The hash reads the same members that operator== compares.
struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t h = std::hash<int>{}(p.x);
        return h ^ (std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2));
    }
};

int main() {
    std::unordered_set<Point, PointHash> s{{1, 2}, {1, 2}, {2, 1}};
    std::cout << s.size() << ' ' << s.count({1, 2}) << ' ' << s.count({3, 3}) << '\n';
}
```

```output for=hash-for-unordered-set
2 1 0
```

- Verdict: stdlib. `std::unordered_set` takes a hash type and an equality type as template parameters; the defaults are `std::hash<T>` and `std::equal_to<T>`, which calls `operator==`.
- The standard library has no `std::hash` for a user type, so the code gives one. It mixes the same two members that the defaulted `operator==` compares.
- A hash that reads a member that `operator==` ignores lets equal points go to different buckets, and the set then keeps duplicates.
