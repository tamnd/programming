```cpp run id=return-value-cpp
#include <iostream>

int absolute(int n) {
    if (n < 0) {
        return -n;
    }
    return n;
}

int main() {
    std::cout << absolute(-7) << "\n";
    std::cout << absolute(5) + 1 << "\n";
}
```

```output for=return-value-cpp
7
6
```

- Verdict: native. `return` ends the function and gives the value, as in C.
