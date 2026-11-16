```cpp run id=noreturn-and-throw
#include <iostream>
#include <stdexcept>
#include <string>

[[noreturn]] void fail(const std::string& msg) {
    throw std::invalid_argument(msg);
}

int port(const std::string& s) {
    // A throw expression may be one operand of the conditional operator.
    return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos
               ? std::stoi(s)
               : throw std::invalid_argument("bad port: " + s);
}

int checked_div(int a, int b) {
    if (b == 0) {
        fail("division by zero");
    }
    return a / b;
}

int main() {
    std::cout << port("8080") << "\n";
    try {
        port("x");
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << "\n";
    }
    std::cout << checked_div(10, 2) << "\n";
    try {
        checked_div(1, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << e.what() << "\n";
    }
}
```

```output for=noreturn-and-throw
8080
bad port: x
5
division by zero
```

- Verdict: idiom. C++ has no bottom type. `[[noreturn]]` marks `fail`, whose type is still `void`. A `throw` expression can be one operand of `?:`, and the conditional expression then has the type of the other operand, `int`.
