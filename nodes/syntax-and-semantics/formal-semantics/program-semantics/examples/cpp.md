```cpp run id=constant-evaluation
#include <iostream>

// The compiler runs constexpr functions on its model of the abstract machine.
constexpr long factorial(int n) {
    long r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

constexpr int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

static_assert(factorial(5) == 120);
static_assert(gcd(1071, 462) == 21);

int main() {
    constexpr long f20 = factorial(20); // computed at compile time
    std::cout << "20! = " << f20 << '\n';
    std::cout << "gcd(1071, 462) = " << gcd(1071, 462) << '\n';
}
```

```output for=constant-evaluation
20! = 2432902008176640000
gcd(1071, 462) = 21
```

```cpp fails expect=compile id=undefined-behavior-in-a-constant
#include <limits>

constexpr int next(int n) { return n + 1; }

// Signed overflow is undefined behavior, and undefined behavior is not
// allowed in a constant expression, so the compiler rejects this line.
constexpr int big = next(std::numeric_limits<int>::max());

int main() { return big; }
```

- Verdict: absent. The C++ standard, like C's, is written in English and defines programs by an abstract machine. Constant evaluation makes the compiler an interpreter of that machine: it runs `constexpr` functions at compile time, and `static_assert` checks their results before the program exists.
- The fails block calls a `constexpr` function that overflows a signed `int`. At run time this is undefined behavior that the compiler need not detect. In a constant expression, undefined behavior is not allowed, so the compiler must reject the program, and Clang reports the value that does not fit.
