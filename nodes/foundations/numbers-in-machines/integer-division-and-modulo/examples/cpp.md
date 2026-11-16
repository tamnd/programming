```cpp run id=truncated
#include <cstdlib>
#include <iostream>

int floor_mod(int a, int n) {
    int r = a % n;
    if (r != 0 && (r < 0) != (n < 0)) {
        r += n;
    }
    return r;
}

int main() {
    int a = -7;
    int n = 3;
    std::cout << "-7 / 3: " << a / n << '\n';
    std::cout << "-7 % 3: " << a % n << '\n';
    std::div_t d = std::div(a, n);
    std::cout << "std::div(-7, 3): quot " << d.quot << ", rem " << d.rem << '\n';
    std::cout << "floor_mod(-7, 3): " << floor_mod(a, n) << '\n';
    std::cout << "floor_mod(7, -3): " << floor_mod(7, -3) << '\n';
}
```

```output for=truncated
-7 / 3: -2
-7 % 3: -1
std::div(-7, 3): quot -2, rem -1
floor_mod(-7, 3): 2
floor_mod(7, -3): -2
```

- Verdict: native. `/` truncates toward zero, and `%` has the sign of the dividend. `std::div` gives both results.
- The standard library has no floored remainder for integers, so a program writes a helper.
