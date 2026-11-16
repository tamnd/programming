```cpp run id=fixed
#include <cstdint>
#include <iostream>

template <int F>
class Fixed {
public:
    static Fixed from_ratio(std::int32_t num, std::int32_t den) {
        return Fixed(static_cast<std::int32_t>((std::int64_t{num} << F) / den));
    }

    Fixed operator+(Fixed o) const { return Fixed(raw_ + o.raw_); }

    Fixed operator*(Fixed o) const {
        std::int64_t p = std::int64_t{raw_} * o.raw_ + (std::int64_t{1} << (F - 1));
        return Fixed(static_cast<std::int32_t>(p >> F));
    }

    double to_double() const { return static_cast<double>(raw_) / (std::int64_t{1} << F); }
    std::int32_t raw() const { return raw_; }

private:
    explicit Fixed(std::int32_t raw) : raw_(raw) {}
    std::int32_t raw_;
};

int main() {
    using Q16 = Fixed<16>;
    Q16 a = Q16::from_ratio(3, 2);
    Q16 b = Q16::from_ratio(9, 4);
    std::cout << "1.5 * 2.25: " << (a * b).to_double() << '\n';
    std::cout << "1.5 + 2.25: " << (a + b).to_double() << '\n';
    Q16 tenth = Q16::from_ratio(1, 10);
    std::cout << "1/10 raw: " << tenth.raw() << '\n';
    Q16 big = Q16::from_ratio(300, 1);
    std::cout << "300 * 300: " << (big * big).to_double() << '\n';
}
```

```output for=fixed
1.5 * 2.25: 3.375
1.5 + 2.25: 3.75
1/10 raw: 6553
300 * 300: 24464
```

- Verdict: idiom. C++ has no fixed-point type in the standard library, so a program writes a class like `Fixed<F>`, where `F` is the number of fraction bits.
- `from_ratio` divides with truncation, so 1/10 is 6553, not 6554. The product is computed in 64 bits, but the cast back to 32 bits wraps 90000 to 24464 with no error.
