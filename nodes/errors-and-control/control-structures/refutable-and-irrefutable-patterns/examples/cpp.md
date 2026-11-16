```cpp run id=refutable-cpp
#include <iostream>
#include <string>
#include <utility>

int main() {
    for (std::string line : {"width=80", "oops", "height=24"}) {
        auto eq = line.find('=');
        if (eq == std::string::npos) {
            std::cout << "skip: " << line << "\n";
            continue;
        }
        // A structured binding takes the pair apart; it has no test.
        auto [key, value] = std::pair{line.substr(0, eq), line.substr(eq + 1)};
        std::cout << key << " -> " << value << "\n";
    }
}
```

```output for=refutable-cpp
width -> 80
skip: oops
height -> 24
```

- Verdict: absent. C++ has no patterns that can fail. A structured binding names the parts of a pair or an array without a test, so the program tests `find` before the binding.
