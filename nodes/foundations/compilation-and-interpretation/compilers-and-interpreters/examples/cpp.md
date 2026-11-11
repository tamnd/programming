```cpp run id=constexpr-evaluation
#include <array>
#include <iostream>

constexpr unsigned long long fib(unsigned n) {
    unsigned long long a = 0, b = 1;
    for (unsigned i = 0; i < n; i++) {
        auto t = a + b;
        a = b;
        b = t;
    }
    return a;
}

constexpr auto table = [] {
    std::array<unsigned long long, 8> t{};
    for (unsigned i = 0; i < t.size(); i++) t[i] = fib(i);
    return t;
}();

static_assert(fib(10) == 55); // checked while the program compiles
static_assert(table[7] == 13);

int main(int argc, char**) {
    constexpr auto f50 = fib(50); // computed by the compiler
    std::cout << f50 << '\n';
    for (auto v : table) std::cout << v << (v == table.back() ? '\n' : ' ');
    std::cout << fib(argc + 9) << '\n'; // the same function at run time
}
```

```output for=constexpr-evaluation
12586269025
0 1 1 2 3 5 8 13
55
```

```cpp fails expect=compile id=constexpr-division-by-zero
constexpr int divide(int a, int b) { return a / b; }

constexpr int ratio = divide(10, 0); // the compiler evaluates the call and rejects it

int main() { return ratio; }
```

- Verdict: native. C++ compiles ahead of time to machine code, and much of the language runs inside the compiler: templates, `constexpr` functions and `static_assert`. A `constexpr` variable must get its value during compilation, so `f50` and `table` cost nothing at run time, while the same `fib` can still run with a value known only at run time.
- Undefined behavior is not allowed in a constant expression. The fails block divides by zero during constant evaluation, and the compiler rejects the program, where the same division at run time would be undefined behavior.
