```cpp run id=traps
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

int main() {
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    std::cout << std::boolalpha;
    std::cout << "nan == nan: " << (nan == nan) << ", isnan: " << std::isnan(nan) << '\n';
    std::cout << "has signaling NaN: " << std::numeric_limits<double>::has_signaling_NaN << '\n';
    std::cout << "inf * 0: " << inf * 0 << '\n';

    std::vector<double> values{3.0, 1.0, 2.0};
    values.insert(values.begin() + 1, nan);
    auto max_it = std::max_element(values.begin(), values.end());
    std::cout << "max_element: " << *max_it << '\n';

    std::set<double> s{nan, 1.0, 2.0};
    std::cout << "set count(1.0): " << s.count(1.0) << ", size: " << s.size() << '\n';

    std::vector<double> no_nan;
    std::copy_if(values.begin(), values.end(), std::back_inserter(no_nan), [](double x) { return !std::isnan(x); });
    std::sort(no_nan.begin(), no_nan.end());
    std::cout << "sorted without NaN:";
    for (double x : no_nan) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}
```

```output for=traps
nan == nan: false, isnan: true
has signaling NaN: true
inf * 0: -nan
max_element: 3
set count(1.0): 1, size: 1
sorted without NaN: 1 2 3
```

- Verdict: native. C++ `double` has the IEEE special values through `std::numeric_limits`.
- `std::set` orders with `<`. A NaN is "equivalent" to every value under `<`, so the set sees 1.0 and 2.0 as duplicates of the NaN and keeps one element, the NaN. For the same reason, `count(1.0)` returns 1, although 1.0 is not in the set.
- The safe way is to remove the NaNs before a sort or a `std::set`.
