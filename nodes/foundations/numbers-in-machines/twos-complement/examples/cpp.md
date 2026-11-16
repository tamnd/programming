```cpp run id=bits
#include <bit>
#include <climits>
#include <cstdint>
#include <iostream>

int main() {
    std::int8_t x = -6;
    std::cout << "bits of -6: " << std::hex << +std::bit_cast<std::uint8_t>(x) << '\n';
    std::cout << "static_cast<std::int8_t>(250): " << std::dec
              << +static_cast<std::int8_t>(250) << '\n';
    int y = INT_MIN;
    std::cout << "-(INT_MIN + 1): " << -(y + 1) << '\n';
    std::cout << "INT_MIN as unsigned: " << static_cast<unsigned int>(y) << '\n';
}
```

```output for=bits
bits of -6: fa
static_cast<std::int8_t>(250): -6
-(INT_MIN + 1): 2147483647
INT_MIN as unsigned: 2147483648
```

- Verdict: native. C++20 allows only two's complement for signed integers.
- `std::bit_cast` shows the bits of a signed value as an unsigned value of the same size.
- Since C++20, a conversion to a signed type that cannot hold the value gives the value modulo 2ᴺ, so 250 becomes -6.
- `-INT_MIN` is still undefined behavior. `-(INT_MIN + 1)` is the largest `int`.
