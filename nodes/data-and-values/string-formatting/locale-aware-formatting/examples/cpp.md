```cpp run id=format-l-option
#include <format>
#include <iostream>
#include <locale>

int main() {
    long n = 1234567;
    double x = 1234567.891;
    std::cout << std::format("{} {:L} {:.2Lf}\n", n, n, x);

    std::locale us("en_US.UTF-8");
    std::cout << std::format(us, "{} {:L} {:.2Lf}\n", n, n, x);

    std::locale::global(us);
    std::cout << std::format("{} {:L}\n", n, n);
    std::cout << n << '\n';
    std::cout.imbue(us);
    std::cout << n << '\n';
}
```

```output for=format-l-option
1234567 1234567 1234567.89
1234567 1,234,567 1,234,567.89
1234567 1,234,567
1234567
1,234,567
```

- Verdict: stdlib. `std::format` ignores the locale unless a specifier has the option `L`. Then it uses the locale argument, or the global locale. A stream such as `std::cout` keeps the locale it had when it was made, so a change of the global locale does not change it until `imbue`. The constructor `std::locale("en_US.UTF-8")` throws `std::runtime_error` if the system does not have the locale.
