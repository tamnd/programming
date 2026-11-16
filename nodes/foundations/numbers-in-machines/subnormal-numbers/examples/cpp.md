```cpp run id=tiny
#include <cmath>
#include <iostream>
#include <limits>

int main() {
    using limits = std::numeric_limits<double>;
    std::cout << "min():        " << limits::min() << "\n";
    std::cout << "denorm_min(): " << limits::denorm_min() << "\n";
    std::cout << std::boolalpha;
    std::cout << "isnormal(min() / 2): " << std::isnormal(limits::min() / 2) << "\n";
    std::cout << "subnormal: " << (std::fpclassify(limits::min() / 2) == FP_SUBNORMAL) << "\n";

    double v = 1.0;
    int steps = 0;
    int first_subnormal = 0;
    while (v != 0) {
        v *= 0.5;
        ++steps;
        if (first_subnormal == 0 && std::fpclassify(v) == FP_SUBNORMAL) {
            first_subnormal = steps;
        }
    }
    std::cout << "halvings to the first subnormal: " << first_subnormal << "\n";
    std::cout << "halvings to zero: " << steps << "\n";

    float f = std::numeric_limits<float>::min();
    std::cout << "float min(): " << f << ", denorm_min(): " << std::numeric_limits<float>::denorm_min() << "\n";
}
```

```output for=tiny
min():        2.22507e-308
denorm_min(): 4.94066e-324
isnormal(min() / 2): false
subnormal: true
halvings to the first subnormal: 1023
halvings to zero: 1075
float min(): 1.17549e-38, denorm_min(): 1.4013e-45
```

- Verdict: native. `std::numeric_limits<double>::min()` is the smallest normal value, and `denorm_min()` is the smallest subnormal.
- The name `min()` is a trap for floats: it is not the most negative value. That is `lowest()`.
- A value that decays toward zero passes through 52 subnormal steps before it becomes 0. On some processors those steps are slow.
