```cpp run id=bitset-and-bit-header
#include <bit>
#include <bitset>
#include <charconv>
#include <cstdint>
#include <format>
#include <iostream>
#include <string_view>

int main() {
    std::uint32_t n = 0b1010'1100; // binary literal; the quote groups digits
    std::cout << std::format("{} {:b} {:o} {:x} {:08b} {:#b}\n", n, n, n, n, 5, 5);
    std::cout << std::bitset<8>(n) << ' ' << std::bitset<8>(static_cast<std::uint8_t>(-5)) << '\n';

    std::string_view s = "101101";
    int v = 0;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), v, 2);
    std::cout << v << ' ' << (ec == std::errc{}) << '\n';

    std::cout << std::popcount(n) << ' ' << std::countl_zero(n) << ' ' << std::bit_width(200u) << ' '
              << std::has_single_bit(64u) << '\n';
}
```

```output for=bitset-and-bit-header
172 10101100 254 ac 00000101 0b101
10101100 11111011
45 1
4 24 8 1
```

- Verdict: native. `std::format` has the type `b` for binary, and `#` adds the prefix. `std::bitset<8>` prints a value as exactly 8 bits. `std::from_chars` parses binary text without exceptions or locales and reports errors in `ec`. The header `<bit>` has `popcount`, `countl_zero`, `bit_width` and `has_single_bit`, which compile to single instructions where the processor has them.
