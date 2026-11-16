```cpp run id=defaulted-three-way-comparison
#include <algorithm>
#include <compare>
#include <iostream>
#include <limits>
#include <vector>

struct Version {
    int major;
    int minor;
    auto operator<=>(const Version&) const = default;
};

int main() {
    std::vector<Version> vs{{1, 10}, {1, 2}, {0, 9}};
    std::sort(vs.begin(), vs.end());
    for (std::size_t i = 0; i < vs.size(); ++i) {
        std::cout << (i ? " " : "") << vs[i].major << '.' << vs[i].minor;
    }
    std::cout << '\n';

    auto c = Version{1, 2} <=> Version{1, 10};
    std::cout << (c < 0) << ' ' << (Version{1, 2} == Version{1, 2}) << '\n';

    double nan = std::numeric_limits<double>::quiet_NaN();
    auto f = 1.0 <=> nan;
    std::cout << (f == std::partial_ordering::unordered) << '\n';
}
```

```output for=defaulted-three-way-comparison
0.9 1.2 1.10
1 1
1
```

- Verdict: native. A class can default `operator<=>`, which compares the members in the order of their declaration, and the compiler then rewrites `<`, `<=`, `>` and `>=` to use it.
- A defaulted `operator<=>` also declares a defaulted `operator==`, so `==` works with no more code.
- The result type tells the strength of the order: `int` members give `std::strong_ordering`, and `double` gives `std::partial_ordering`, where a `NaN` compares as `unordered`.
