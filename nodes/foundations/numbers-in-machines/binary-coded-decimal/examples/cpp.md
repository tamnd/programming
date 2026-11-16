```cpp run id=packed
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

std::vector<std::uint8_t> to_packed(long long n) {
    std::string s = std::to_string(n < 0 ? -n : n);
    std::vector<std::uint8_t> nib;
    if (s.size() % 2 == 0) {
        nib.push_back(0);
    }
    for (char c : s) {
        nib.push_back(static_cast<std::uint8_t>(c - '0'));
    }
    nib.push_back(n < 0 ? 0xD : 0xC);
    std::vector<std::uint8_t> out;
    for (std::size_t i = 0; i < nib.size(); i += 2) {
        out.push_back(static_cast<std::uint8_t>(nib[i] << 4 | nib[i + 1]));
    }
    return out;
}

std::optional<long long> from_packed(const std::vector<std::uint8_t> &b) {
    long long v = 0;
    for (std::size_t i = 0; i < b.size(); ++i) {
        unsigned hi = b[i] >> 4;
        unsigned lo = b[i] & 0x0F;
        bool last = i + 1 == b.size();
        if (hi > 9 || (!last && lo > 9)) {
            return std::nullopt;
        }
        v = v * 10 + hi;
        if (!last) {
            v = v * 10 + lo;
        } else if (lo == 0xD) {
            v = -v;
        }
    }
    return v;
}

void print_hex(const char *label, const std::vector<std::uint8_t> &b) {
    std::cout << label << ':';
    for (auto x : b) {
        std::cout << ' ' << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned>(x);
    }
    std::cout << std::dec << '\n';
}

int main() {
    auto p = to_packed(-12345);
    print_hex("packed -12345", p);
    std::cout << "back: " << from_packed(p).value() << '\n';
    print_hex("packed 2024", to_packed(2024));
    std::cout << "1A 3C valid: " << (from_packed({0x1A, 0x3C}) ? "yes" : "no") << '\n';
}
```

```output for=packed
packed -12345: 12 34 5D
back: -12345
packed 2024: 02 02 4C
1A 3C valid: no
```

- Verdict: idiom. C++ has no BCD type. The block packs the nibbles into a `std::vector<std::uint8_t>`.
- `from_packed` returns an empty `std::optional` for an invalid nibble.
