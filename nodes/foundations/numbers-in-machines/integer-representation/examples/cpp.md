```cpp run id=bit-cast-and-cmp-less
#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>

int main() {
    // The same 8 bits read as unsigned and as two's complement.
    std::uint8_t u = 251;
    std::cout << +u << ' ' << +std::bit_cast<std::int8_t>(u) << '\n';

    using L = std::numeric_limits<std::int16_t>;
    std::cout << L::min() << ' ' << L::max() << ' ' << L::digits << " value bits\n";

    // The built-in comparison converts -1 to unsigned first; std::cmp_less compares the values.
    int neg = -1;
    unsigned one = 1;
    std::cout << std::boolalpha << (static_cast<unsigned>(neg) < one) << ' ' << std::cmp_less(neg, one) << '\n';
    std::cout << std::in_range<std::uint8_t>(300) << ' ' << std::in_range<std::int8_t>(-5) << '\n';

    // The byte order of the machine, and a byte swap.
    std::cout << (std::endian::native == std::endian::little ? "little" : "big") << "-endian\n";
    std::cout << std::hex << std::byteswap(std::uint32_t{0x11223344}) << '\n';
}
```

```output for=bit-cast-and-cmp-less
251 -5
-32768 32767 15 value bits
false true
false true
little-endian
44332211
```

- Verdict: native. C++ has the integer types of C and the same undefined signed overflow. `std::bit_cast` reads the bits of a `uint8_t` as an `int8_t`, and `std::numeric_limits` gives the range and the number of value bits. `std::cmp_less` and `std::in_range` compare mathematical values, so they give the right answer where the built-in comparison converts −1 to unsigned. `std::endian` names the byte order of the machine, and `std::byteswap` reverses the bytes.
