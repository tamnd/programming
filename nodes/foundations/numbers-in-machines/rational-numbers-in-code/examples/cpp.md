```cpp run id=ratio
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ratio>
#include <stdexcept>

struct Ratio {
    std::int64_t n;
    std::int64_t d;
};

Ratio make(std::int64_t n, std::int64_t d) {
    if (d == 0) {
        throw std::domain_error("denominator is 0");
    }
    if (d < 0) {
        n = -n;
        d = -d;
    }
    std::int64_t g = std::gcd(n, d);
    return {n / g, d / g};
}

Ratio add(Ratio a, Ratio b) {
    return make(a.n * b.d + b.n * a.d, a.d * b.d);
}

int main() {
    using sum = std::ratio_add<std::ratio<1, 10>, std::ratio<2, 10>>;
    std::cout << "std::ratio 1/10 + 2/10: " << sum::num << '/' << sum::den << '\n';
    using r = std::ratio<6, -8>;
    std::cout << "std::ratio<6, -8>: " << r::num << '/' << r::den << '\n';
    static_assert(std::ratio_equal_v<sum, std::ratio<3, 10>>);

    Ratio s = add(make(1, 10), make(2, 10));
    std::cout << "runtime 1/10 + 2/10: " << s.n << '/' << s.d << '\n';
    try {
        make(1, 0);
    } catch (const std::domain_error &e) {
        std::cout << "make(1, 0): " << e.what() << '\n';
    }
}
```

```output for=ratio
std::ratio 1/10 + 2/10: 3/10
std::ratio<6, -8>: -3/4
runtime 1/10 + 2/10: 3/10
make(1, 0): denominator is 0
```

- Verdict: library. `std::ratio` computes with fractions at compile time only, and it keeps them in lowest terms with a positive denominator. For values at run time, GMP has the `mpq_class` class.
- The `Ratio` struct here has `std::int64_t` parts, so its products can overflow without a check.
