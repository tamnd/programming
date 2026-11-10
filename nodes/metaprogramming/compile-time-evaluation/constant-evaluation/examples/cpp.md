```cpp run id=both-times
#include <array>
#include <iostream>

constexpr long factorial(int n) {
    long r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

static_assert(factorial(5) == 120, "5! is 120");

int main(int argc, char**) {
    constexpr long f10 = factorial(10);
    std::array<int, factorial(4)> slots{};
    std::cout << "factorial(10) = " << f10 << "\n";
    std::cout << "slots: " << slots.size() << "\n";
    int n = argc + 5;
    std::cout << "factorial(" << n << ") = " << factorial(n) << "\n";
}
```

```output for=both-times
factorial(10) = 3628800
slots: 24
factorial(6) = 720
```

One `constexpr` function serves three uses. The compiler evaluates `factorial(5)` for the `static_assert`, `factorial(10)` for a `constexpr` variable and `factorial(4)` for an array bound. The last call takes a value that exists only at run time, so the program evaluates it at run time.

```cpp fails expect=compile id=consteval-runtime
#include <iostream>

consteval int square(int x) {
    return x * x;
}

int main(int argc, char**) {
    std::cout << square(4) << "\n";
    std::cout << square(argc) << "\n";
}
```

A `consteval` function is an immediate function. Each call must produce a constant. `square(4)` is acceptable, but `square(argc)` depends on a run-time value, so the compiler rejects the program.

```cpp fails expect=compile id=overflow
#include <iostream>

constexpr int factorial(int n) {
    int r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}

int main() {
    constexpr int f13 = factorial(13);
    std::cout << f13 << "\n";
}
```

13! is 6227020800 and does not fit in a 32-bit `int`. Signed overflow is undefined behavior, and an expression with undefined behavior is not a core constant expression. The compiler must diagnose it here. The same call at run time compiles and has undefined behavior.

```cpp fails expect=compile id=depth
constexpr int depth(int n) {
    return n == 0 ? 0 : 1 + depth(n - 1);
}

int main() {
    constexpr int d = depth(2000);
    return d == 2000 ? 0 : 1;
}
```

The function is correct, but the evaluation is too deep. The compilers stop at a fixed number of nested calls, and the program fails to compile. A loop in place of the recursion compiles.

```cpp run id=heap
#include <iostream>
#include <numeric>
#include <vector>

constexpr int sum_of_squares(int n) {
    std::vector<int> v;
    for (int i = 1; i <= n; ++i) {
        v.push_back(i * i);
    }
    return std::accumulate(v.begin(), v.end(), 0);
}

static_assert(sum_of_squares(4) == 30);

int main() {
    constexpr int s = sum_of_squares(10);
    std::cout << "sum of squares 1..10 = " << s << "\n";
}
```

```output for=heap
sum of squares 1..10 = 385
```

A constant evaluation can allocate memory with `new` if it frees the memory before it ends. Here `std::vector` allocates and frees inside `sum_of_squares`, and only the `int` result leaves the evaluation.

- Verdict: native. `constexpr` marks variables and functions that can take part in constant expressions, `consteval` marks functions that must, and `static_assert` checks a constant condition at compile time.
- A `constexpr` function is not always evaluated at compile time. It is evaluated at compile time when a constant expression needs its value.
- An evaluation that reaches undefined behavior in the core language, a call to a function that is not `constexpr`, a `reinterpret_cast`, a `throw` expression or a `goto` statement is not a constant expression.
