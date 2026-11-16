```cpp run id=reference
#include <iostream>

int main() {
    int n = 1;
    int& alias = n;
    alias = 2;
    auto m = n;
    m = 3;
    std::cout << n << ' ' << alias << ' ' << m << '\n';
}
```

```output for=reference
2 2 3
```

- Verdict: native. A C++ variable is an object with a type, as in C. A reference such as `int& alias = n` is a second name for the same object, so `alias = 2` changes `n`. `auto m = n` declares a new object with a copy of the value.
