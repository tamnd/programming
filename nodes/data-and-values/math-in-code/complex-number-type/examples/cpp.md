```cpp run id=std-complex
#include <complex>
#include <iostream>

int main() {
    using namespace std::complex_literals;
    std::complex<double> z{3.0, 4.0};
    auto w = 1.0 - 2.0i;
    std::cout << z * w << ' ' << z / w << '\n';
    std::cout << std::abs(z) << ' ' << std::norm(z) << ' ' << std::arg(1.0i) << '\n';
    std::cout << std::sqrt(std::complex<double>{-4.0, 0.0}) << '\n';
    std::cout << z.real() << ' ' << z.imag() << ' ' << std::conj(z) << '\n';
}
```

```output for=std-complex
(11,-2) (-1,2)
5 25 1.5708
(0,2)
3 4 (3,-4)
```

- Verdict: stdlib. The class template `std::complex<T>` in `<complex>` has the operators, and the functions `std::abs`, `std::arg`, `std::norm`, `std::conj` and `std::sqrt` accept it. The literal `2.0i` comes from `std::complex_literals`. A stream prints a complex number as `(re,im)`. `std::norm` is the square of the absolute value, 25 for `3 + 4i`, not the length 5.
