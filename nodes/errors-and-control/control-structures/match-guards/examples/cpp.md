```cpp run id=match-guards-cpp
#include <iostream>
#include <string_view>

static std::string_view describe(int n) {
    if (n == 0) {
        return "zero";
    }
    if (n < 0) {
        return "negative";
    }
    if (n < 10 && n % 2 == 0) {
        return "small even";
    }
    if (n < 10) {
        return "small odd";
    }
    return "large";
}

int main() {
    const char *sep = "";
    for (int n : {-5, 0, 4, 7, 12}) {
        std::cout << sep << describe(n);
        sep = ", ";
    }
    std::cout << "\n";
}
```

```output for=match-guards-cpp
negative, zero, small even, small odd, large
```

- Verdict: absent. C++ has no pattern matching. A `switch` case takes only a constant expression, so a chain of `if` statements tests the conditions in order.
