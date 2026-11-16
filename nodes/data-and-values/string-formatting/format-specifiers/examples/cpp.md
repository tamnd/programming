```cpp run id=std-format-spec
#include <cstdio>
#include <format>
#include <iostream>

int main() {
    double x = 3.14159;
    std::cout << std::format("[{:8.2f}] [{:<8.2f}] [{:08.2f}] [{:+.1f}]\n", x, x, x, x);
    std::cout << std::format("[{:>6}] [{:*^9}] [{:x}] [{:#06x}] [{:b}]\n", "abc", "abc", 255, 255, 5);
    std::cout << std::format("[{1}-{0}-{1}] [{2:e}]\n", "a", "b", 1234.5);
    std::printf("[%8.2f] [%-6s] [%05d]\n", x, "abc", 42);
}
```

```output for=std-format-spec
[    3.14] [3.14    ] [00003.14] [+3.1]
[   abc] [***abc***] [ff] [0x00ff] [101]
[b-a-b] [1.234500e+03]
[    3.14] [abc   ] [00042]
```

- Verdict: stdlib. `std::format` takes the brace syntax of Python, and the compiler checks a literal format string against the types. C++ keeps `printf` from C as well.
