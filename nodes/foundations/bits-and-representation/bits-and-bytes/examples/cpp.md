```cpp run id=bit-header
#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>

enum Perm : std::uint8_t { Read = 1 << 0, Write = 1 << 1, Exec = 1 << 2 };

int main() {
    std::uint8_t perm = Read | Write;
    perm &= ~Write;
    perm ^= Exec;
    std::cout << std::bitset<3>(perm) << ' ' << std::popcount(perm) << '\n';

    std::uint8_t b = 0b1001'0110;
    std::cout << int(std::rotl(b, 1)) << ' ' << std::countl_zero(std::uint32_t{1})
              << ' ' << std::has_single_bit(64u) << ' ' << std::bit_width(255u) << '\n';

    std::cout << (-8 >> 1) << ' ' << (0xF0u >> 4) << ' '
              << (std::endian::native == std::endian::little ? "little" : "big") << '\n';

    float f = 1.0f;
    std::cout << std::hex << std::bit_cast<std::uint32_t>(f) << '\n';
}
```

```output for=bit-header
101 2
45 31 1 8
-4 15 little
3f800000
```

- Verdict: native. C++ has the operators of C and the header `<bit>` with `popcount`, `rotl`, `countl_zero`, `has_single_bit`, `bit_width`, `bit_cast` and `endian`. `std::bitset` holds a fixed number of bits and prints them. Right shift of a negative value is arithmetic, and the rules for undefined shift counts are the same as in C.
- `std::bit_cast` copies the bits of one type into another type of the same size, here the bits of the `float` 1.0, without the undefined behavior of a cast between pointer types.
