```cpp run id=cmp
#include <iostream>
#include <utility>
#include <vector>

const char* yes_no(bool b) { return b ? "yes" : "no"; }

int main() {
    int minus_one = -1;
    unsigned int zero = 0;
    std::cout << "-1 > 0u: " << yes_no(minus_one > zero) << '\n';
    std::cout << "std::cmp_greater(-1, 0u): " << yes_no(std::cmp_greater(minus_one, zero)) << '\n';

    std::vector<int> empty;
    std::cout << "empty.size() - 1 > 0: " << yes_no(empty.size() - 1 > 0) << '\n';
    std::cout << "std::cmp_less(-1, empty.size()): " << yes_no(std::cmp_less(-1, empty.size())) << '\n';
}
```

```output for=cmp
-1 > 0u: yes
std::cmp_greater(-1, 0u): no
empty.size() - 1 > 0: yes
std::cmp_less(-1, empty.size()): yes
```

- Verdict: native. C++ has the unsigned types of C, and `size()` of the standard containers returns an unsigned type.
- The built-in operators convert -1 to unsigned, so `-1 > 0u` is true.
- `std::cmp_greater`, `std::cmp_less` and the other functions of `<utility>` compare the mathematical values. They need C++20.
- `empty.size() - 1` wraps to the largest `size_t`. Compare with `std::cmp_less`, or convert the size to a signed type first.
