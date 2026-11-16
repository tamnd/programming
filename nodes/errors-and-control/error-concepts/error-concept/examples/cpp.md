```cpp run id=error-cpp
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    for (std::string text : {"42", "x42"}) {
        try {
            int n = std::stoi(text);
            std::cout << text << " -> " << n << "\n";
        } catch (const std::invalid_argument&) {
            std::cout << text << " -> error (std::invalid_argument)\n";
        }
    }
}
```

```output for=error-cpp
42 -> 42
x42 -> error (std::invalid_argument)
```

- Verdict: native. C++ has exceptions, and the standard library also uses error codes. `std::stoi` throws `std::invalid_argument` when no conversion is possible. `std::from_chars` reports the same error with a `std::errc` value and throws nothing.
