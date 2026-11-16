```cpp run id=forms
#include <bitset>
#include <cmath>
#include <iostream>

unsigned to_sign_magnitude(int n) { return n < 0 ? 0x80u | static_cast<unsigned>(-n) : static_cast<unsigned>(n); }
unsigned to_ones(int n) { return n < 0 ? ~static_cast<unsigned>(-n) & 0xFFu : static_cast<unsigned>(n); }
int from_sign_magnitude(unsigned b) { return (b & 0x80u) ? -static_cast<int>(b & 0x7Fu) : static_cast<int>(b); }
int from_ones(unsigned b) { return (b & 0x80u) ? -static_cast<int>(~b & 0xFFu) : static_cast<int>(b); }

int main() {
    std::cout << "-43 in sign-magnitude: " << std::bitset<8>(to_sign_magnitude(-43)) << '\n';
    std::cout << "-43 in ones' complement: " << std::bitset<8>(to_ones(-43)) << '\n';
    std::cout << "10101011 in sign-magnitude: " << from_sign_magnitude(0b10101011u) << '\n';
    std::cout << "11010100 in ones' complement: " << from_ones(0b11010100u) << '\n';
    std::cout << "10000000 in sign-magnitude: " << from_sign_magnitude(0b10000000u) << '\n';
    std::cout << "11111111 in ones' complement: " << from_ones(0b11111111u) << '\n';

    double z = -0.0;
    std::cout << "-0.0 == 0.0: " << (z == 0.0 ? "yes" : "no") << '\n';
    std::cout << "std::signbit(-0.0): " << (std::signbit(z) ? "yes" : "no") << '\n';
    std::cout << "-0.0 prints as: " << z << '\n';
}
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
-0.0 == 0.0: yes
std::signbit(-0.0): yes
-0.0 prints as: -0
```

- Verdict: idiom. C++20 allows only two's complement for signed integers, so a program makes the other two forms by hand with unsigned operations.
- `std::bitset<8>` prints the low 8 bits of a value.
- `std::signbit` reads the sign of a float, and the stream prints `-0.0` as `-0`.
