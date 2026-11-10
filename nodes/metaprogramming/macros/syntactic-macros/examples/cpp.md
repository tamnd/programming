```cpp fails expect=compile id=no-scope
#include <iostream>
#include <limits>

#define max(a, b) ((a) > (b) ? (a) : (b))

int main() {
    std::cout << std::numeric_limits<int>::max() << '\n';
}
```

A macro has no scope and no namespace. The name `max` in `std::numeric_limits<int>::max()` is followed by `(`, so the preprocessor treats it as a call of the macro with too few arguments, and the compiler stops.

```cpp run id=alternatives
#include <algorithm>
#include <iostream>
#include <source_location>

template <typename T>
constexpr T square(T x) {
    return x * x;
}

static_assert(square(1 + 2) == 9);

void log(const char *msg, std::source_location where = std::source_location::current()) {
    std::cout << "line " << where.line() << ": " << msg << '\n';
}

int main() {
    int i = 5;
    int m = std::max(i++, 0);
    std::cout << "m = " << m << ", i = " << i << '\n';
    std::cout << "square(2.5) = " << square(2.5) << '\n';
    log("called without a macro");
}
```

```output for=alternatives
m = 5, i = 6
square(2.5) = 6.25
line 21: called without a macro
```

```cpp run id=stringize
#include <iostream>

void check(bool ok, const char *text, int line) {
    std::cout << (ok ? "ok:   " : "FAIL: ") << text << " (line " << line << ")\n";
}

#define CHECK(expr) check((expr), #expr, __LINE__)

int main() {
    int a = 2;
    int b = 3;
    CHECK(a + b == 5);
    CHECK(a * b == 5);
}
```

```output for=stringize
ok:   a + b == 5 (line 12)
FAIL: a * b == 5 (line 13)
```

- Verdict: native. C++ has the preprocessor of C, with the same `#` and `##` operators and the same lack of scope.
- A function template replaces a function-like macro: it has a type, evaluates each argument once, and obeys namespaces. A `constexpr` function also runs at compile time, as the `static_assert` shows.
- `std::source_location::current()` as a default argument gives the line of the caller, which once needed `__LINE__` in a macro.
- The text of an expression (`#expr`) is the main thing that still needs a macro, as in `CHECK` and the standard `assert`.
