```cpp run id=strategy-cpp
#include <iostream>
#include <stdexcept>
#include <utility>

int divide(int a, int b) {
    if (b == 0) {
        throw std::domain_error("division by zero");
    }
    return a / b;
}

int main() {
    for (auto [a, b] : {std::pair{10, 2}, std::pair{10, 0}}) {
        try {
            int q = divide(a, b);
            std::cout << a << " / " << b << " = " << q << "\n";
        } catch (const std::domain_error& e) {
            std::cout << a << " / " << b << ": error: " << e.what() << "\n";
        }
    }
}
```

```output for=strategy-cpp
10 / 2 = 5
10 / 0: error: division by zero
```

- Verdict: native. Strategy: exceptions, which the C++ Core Guidelines recommend when a function cannot do its task. Code that cannot use exceptions returns an error code instead, for example a `std::errc` value or, since C++23, a `std::expected`.
