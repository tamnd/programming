```cpp run id=checks
#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>

int main() {
    unsigned u = UINT_MAX;
    std::cout << "UINT_MAX + 1: " << u + 1u << '\n';

    int x = INT_MAX;
    int r = 0;
    bool over = __builtin_add_overflow(x, 1, &r);
    std::cout << "__builtin_add_overflow(x, 1): " << (over ? "overflow" : "ok") << ", result " << r << '\n';

    std::int64_t wide = std::int64_t{x} + 1;
    std::int64_t lo = INT_MIN;
    std::int64_t hi = INT_MAX;
    std::cout << "saturated: " << std::clamp(wide, lo, hi) << '\n';
}
```

```output for=checks
UINT_MAX + 1: 0
__builtin_add_overflow(x, 1): overflow, result -2147483648
saturated: 2147483647
```

- Verdict: stdlib. Unsigned arithmetic wraps, and signed overflow is undefined behavior. C++26 adds `std::saturating_add` and the other saturating operations in `<numeric>`. This example uses `std::clamp` on a wider type because GCC 13 does not have them.
- `__builtin_add_overflow` is a GCC and Clang builtin, not standard C++.
