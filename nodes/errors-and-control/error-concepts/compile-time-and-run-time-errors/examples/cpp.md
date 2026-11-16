```cpp run id=when-cpp
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    std::vector<int> items{10, 20, 30};
    for (std::size_t i : {1, 5}) {
        try {
            int v = items.at(i);
            std::cout << "items[" << i << "] = " << v << "\n";
        } catch (const std::out_of_range&) {
            std::cout << "items[" << i << "]: run-time error (std::out_of_range)\n";
        }
    }
}
```

```output for=when-cpp
items[1] = 20
items[5]: run-time error (std::out_of_range)
```

- Verdict: idiom. The C++ compiler finds syntax and type errors, and the linker finds a function that has no definition. `at` tests the index and throws `std::out_of_range`. The operator `[]` does not test it: an index past the end has undefined behavior.
