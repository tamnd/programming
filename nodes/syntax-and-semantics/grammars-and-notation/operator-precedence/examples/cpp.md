```cpp run id=overloaded-and-conditional
#include <cmath>
#include <iostream>

// A type that uses ^ for power. The overloaded operator keeps the precedence of ^, which is below + and *.
struct Num {
    double v;
};
Num operator^(Num a, int b) { return Num{std::pow(a.v, b)}; }
Num operator*(Num a, int b) { return Num{a.v * b}; }

int main() {
    // << for output keeps the precedence of the shift operator: + binds more tightly, == less tightly.
    std::cout << 1 + 2 << ' ' << (1 == 1) << '\n';

    Num two{2};
    std::cout << (two ^ 3 * 2).v << ' ' << ((two ^ 3) * 2).v << '\n';

    // In C++, ?: and = have the same precedence and group to the right,
    // so flag ? a : b = 9 means flag ? a : (b = 9). If it meant (flag ? a : b) = 9, a would become 9.
    int a = 1, b = 2;
    bool flag = true;
    flag ? a : b = 9;
    std::cout << a << ' ' << b << '\n';

    // The comma operator has the lowest precedence of all.
    int k = (a = 5, a + 1);
    std::cout << k << '\n';
}
```

```output for=overloaded-and-conditional
3 1
64 16
1 2
6
```

- Verdict: native. An overloaded operator keeps the level of the built-in one, so `two ^ 3 * 2` is `two ^ (3 * 2)`, which is 64, not 16. Output with `<<` keeps the level of the shift operator. In C++, `?:` and `=` are at the same level and group to the right, so `flag ? a : b = 9` assigns to `b` only when `flag` is false, and `a` stays 1. The comma operator has the lowest level of all.
