```cpp run id=bignum
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class BigNat {
public:
    explicit BigNat(std::uint32_t v) : d_{v} {}

    BigNat &operator*=(std::uint32_t m) {
        std::uint64_t carry = 0;
        for (auto &d : d_) {
            std::uint64_t t = std::uint64_t{d} * m + carry;
            d = static_cast<std::uint32_t>(t % kBase);
            carry = t / kBase;
        }
        while (carry > 0) {
            d_.push_back(static_cast<std::uint32_t>(carry % kBase));
            carry /= kBase;
        }
        return *this;
    }

    std::string str() const {
        std::ostringstream out;
        out << d_.back();
        for (auto it = d_.rbegin() + 1; it != d_.rend(); ++it) {
            out << std::setw(9) << std::setfill('0') << *it;
        }
        return out.str();
    }

private:
    static constexpr std::uint64_t kBase = 1000000000;
    std::vector<std::uint32_t> d_;
};

int main() {
    unsigned long long w = 1;
    for (unsigned long long i = 1; i <= 21; ++i) {
        w *= i;
    }
    std::cout << "21! in unsigned long long: " << w << '\n';

    BigNat f(1);
    for (std::uint32_t i = 1; i <= 30; ++i) {
        f *= i;
    }
    std::cout << "30!: " << f.str() << '\n';
    BigNat p(1);
    for (int i = 0; i < 100; ++i) {
        p *= 2;
    }
    std::cout << "2^100: " << p.str() << '\n';
    BigNat h(1);
    for (std::uint32_t i = 1; i <= 100; ++i) {
        h *= i;
    }
    std::cout << "decimal digits of 100!: " << h.str().size() << '\n';
}
```

```output for=bignum
21! in unsigned long long: 14197454024290336768
30!: 265252859812191058636308480000000
2^100: 1267650600228229401496703205376
decimal digits of 100!: 158
```

- Verdict: library. The C++ standard library has no bignum type. GMP has a C++ interface with the class `mpz_class`.
- `21!` does not fit in 64 bits, so the unsigned product wraps modulo 2^64 without an error.
- The class keeps a `std::vector` of base 10^9 digits, which grows when a carry is left over.
