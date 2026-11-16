```cpp run id=local-function-cpp
#include <iostream>
#include <vector>

int sum_of_scaled(const std::vector<int>& values, int factor) {
    auto scale = [factor](int x) { return x * factor; };

    int total = 0;
    for (int v : values) {
        total += scale(v);
    }
    return total;
}

int main() {
    std::cout << sum_of_scaled({1, 2, 3}, 10) << "\n";
    std::cout << sum_of_scaled({4}, 2) << "\n";
}
```

```output for=local-function-cpp
60
8
```

- Verdict: idiom. C++ has no nested functions. A lambda in the function body captures `factor` and works as a local function.
