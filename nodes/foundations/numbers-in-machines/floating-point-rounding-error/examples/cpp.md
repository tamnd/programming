```cpp run id=money
#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
    const double a = 1.0 + 1e-8;
    const double b = 1.0;
    const double x = a * a - b * b;
    const double y = (a - b) * (a + b);
    std::cout << std::setprecision(17);
    std::cout << "a*a - b*b:     " << x << '\n';
    std::cout << "(a-b)*(a+b):   " << y << '\n';

    const double small = 1e-10;
    std::cout << "log(1 + x):    " << std::log(1.0 + small) << '\n';
    std::cout << "log1p(x):      " << std::log1p(small) << '\n';
    std::cout << "expm1(x):      " << std::expm1(small) << '\n';
    std::cout << "exp(x) - 1:    " << std::exp(small) - 1.0 << '\n';
}
```

```output for=money
a*a - b*b:     1.9999999878450581e-08
(a-b)*(a+b):   1.9999999978450579e-08
log(1 + x):    1.000000082690371e-10
log1p(x):      9.9999999995000007e-11
expm1(x):      1.00000000005e-10
exp(x) - 1:    1.000000082740371e-10
```

- Verdict: native. C++ `double` is binary64 on common platforms.
- `a*a - b*b` subtracts two rounded products, so the cancellation exposes their errors. `(a-b)*(a+b)` subtracts exact values first.
- `std::log1p` and `std::expm1` avoid the cancellation in `log(1 + x)` and `exp(x) - 1` for small `x`.
