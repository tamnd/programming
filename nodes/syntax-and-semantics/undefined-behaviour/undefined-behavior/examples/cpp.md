```cpp fails expect=sanitizer:undefined timeout=30 id=always-true
#include <climits>
#include <iostream>

// An optimizer may fold this to "return true".
bool can_increment(int x) {
    return x + 1 > x;
}

int main() {
    std::cout << can_increment(1) << '\n';
    std::cout << can_increment(INT_MAX) << '\n';
}
```

The test `x + 1 > x` looks like an overflow check, but it is not one. For every `x` where the addition is defined, the result is true. The compiler may assume that the addition is defined, so it may replace the whole function with `true`. UndefinedBehaviorSanitizer reports the overflow at the second call.

```cpp fails expect=compile id=constexpr-overflow
#include <climits>

constexpr int next(int x) {
    return x + 1;
}

static_assert(next(41) == 42);
static_assert(next(INT_MAX) > 0); // not a constant expression

int main() {}
```

In a constant expression, the compiler must reject an operation that has undefined behavior. The same function that compiles to unchecked code at run time is checked during constant evaluation.

```cpp fails expect=sanitizer:address timeout=30 id=off-by-one
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3};
    int sum = 0;
    for (std::size_t i = 0; i <= v.size(); ++i) { // <= reads one past the end
        sum += v[i];
    }
    std::cout << sum << '\n';
}
```

`operator[]` does not check the index. AddressSanitizer reports a heap-buffer-overflow at the read of `v[3]`.

```cpp run id=defined
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

bool can_increment(int x) {
    return x < std::numeric_limits<int>::max(); // test before the addition
}

int main() {
    std::cout << std::boolalpha;
    std::cout << can_increment(1) << ' ' << can_increment(std::numeric_limits<int>::max()) << '\n';
    std::vector<int> v = {1, 2, 3};
    try {
        std::cout << v.at(3) << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "at(3) threw std::out_of_range\n";
    }
}
```

```output for=defined
true false
at(3) threw std::out_of_range
```

- Verdict: native. The C++ standard defines undefined behavior in [defns.undefined] and gives the rule for a whole execution in [intro.abstract].
- A check for overflow must come before the operation. Compare against `std::numeric_limits<int>::max()` minus the other operand.
- `v.at(i)` checks the index and throws `std::out_of_range`. `v[i]` does not check it.
- A `constexpr` function called in a constant expression is a cheap way to test code for undefined behavior at compile time, for the inputs that the test uses.
