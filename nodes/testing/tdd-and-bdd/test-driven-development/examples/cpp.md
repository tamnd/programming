```cpp run id=leap
#include <functional>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

constexpr bool is_leap(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

// The same tests, checked by the compiler.
static_assert(is_leap(2024));
static_assert(!is_leap(2023));
static_assert(!is_leap(1900));
static_assert(is_leap(2000));

int main() {
    // The tests in the order of the to-do list. Each one added one clause.
    std::vector<std::pair<std::string_view, std::function<bool()>>> tests{
        {"2024 is a leap year", [] { return is_leap(2024); }},
        {"2023 is a common year", [] { return !is_leap(2023); }},
        {"1900 is a common year", [] { return !is_leap(1900); }},
        {"2000 is a leap year", [] { return is_leap(2000); }},
    };
    int failed = 0;
    for (const auto& [name, check] : tests) {
        bool ok = check();
        failed += ok ? 0 : 1;
        std::cout << (ok ? "pass  " : "FAIL  ") << name << '\n';
    }
    std::cout << (failed == 0 ? "green" : "red") << '\n';
}
```

```output
pass  2024 is a leap year
pass  2023 is a common year
pass  1900 is a common year
pass  2000 is a leap year
green
```

- Verdict: library. The usual runners, GoogleTest and Catch2, are libraries. This example uses a vector of named checks, so that it runs with the standard library only.
- A `constexpr` function can run at compile time. Each `static_assert` is a test that the compiler runs, and the program does not build if one of them fails.
- The tests in `main` repeat the `static_assert` checks at run time, so that the program prints a report. A function that reads a file or the clock cannot be `constexpr`, so most tests run at run time.

With `static_assert`, red in C++ is a compile error for a wrong answer, not only for a missing name. Here the fake from the first cycle meets the second test.

```cpp fails expect=compile id=red
constexpr bool is_leap(int) {
    return true;  // Fake It from the first cycle
}

static_assert(is_leap(2024));
static_assert(!is_leap(2023));  // the second test: red at compile time

int main() {}
```

- The compiler reports that the static assertion failed. In the other languages on this page, a wrong answer is red only when the program runs.
