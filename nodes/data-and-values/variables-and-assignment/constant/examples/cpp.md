```cpp run id=constexpr
#include <array>
#include <iostream>
#include <string>

constexpr int square(int n) { return n * n; }

constexpr int kSize = square(3);

int main() {
    std::array<int, kSize> cells{};
    const std::string name = "Ada";
    static_assert(kSize == 9);
    std::cout << cells.size() << ' ' << name.size() << '\n';
}
```

```output for=constexpr
9 3
```

- Verdict: native. `constexpr` asks the compiler to compute the value, here with a call to a `constexpr` function, so `kSize` can be the size of an array and the operand of `static_assert`. A `const` object, such as `name`, gets its value at run time and cannot change after that.
