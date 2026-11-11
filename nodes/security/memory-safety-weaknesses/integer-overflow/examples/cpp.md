```cpp wrong id=unsigned-size
#include <cstddef>
#include <iostream>
#include <vector>

// Returns the number of items after the first skip items.
std::size_t remaining(const std::vector<int>& items, std::size_t skip) {
    return items.size() - skip;
}

int main() {
    std::vector<int> items{1, 2, 3};
    std::cout << remaining(items, 2) << '\n';
    std::cout << remaining(items, 5) << '\n';
}
```

```output for=unsigned-size
1
18446744073709551614
```

`size()` returns an unsigned type. 3 − 5 does not give −2. It wraps to a value near 2⁶⁴, and a loop that uses it as a count runs far past the end of the vector. The behavior is defined, so no sanitizer in the default set reports it.

```cpp fails expect=compile flags="-Werror=narrowing" id=narrowing
#include <cstdint>
#include <iostream>

int main() {
    std::int64_t total = 4294967300;
    std::int32_t small{total};
    std::cout << small << '\n';
}
```

Brace initialization does not allow a narrowing conversion. Clang rejects the conversion of a 64-bit value to a 32-bit one. GCC rejects a narrowing conversion of a constant, but gives only a warning when the value is not a constant, so this build adds `-Werror=narrowing`. An initialization with `=` compiles and truncates the value.

```cpp run flags="-fsanitize=address,undefined" id=check
#include <cstddef>
#include <iostream>
#include <optional>
#include <vector>

// Returns the number of items after the first skip items, or nothing.
std::optional<std::size_t> remaining(const std::vector<int>& items, std::size_t skip) {
    if (skip > items.size()) {
        return std::nullopt;
    }
    return items.size() - skip;
}

int main() {
    std::vector<int> items{1, 2, 3};
    for (std::size_t skip : {2, 5}) {
        if (auto left = remaining(items, skip)) {
            std::cout << "skip " << skip << ": " << *left << '\n';
        } else {
            std::cout << "skip " << skip << ": more than the size\n";
        }
    }
}
```

```output for=check
skip 2: 1
skip 5: more than the size
```

- Verdict: absent. C++ has the arithmetic rules of C: signed overflow is undefined behavior, and unsigned arithmetic wraps.
- Sizes and indices in the standard library are unsigned, so a subtraction of sizes is a common source of wraparound. Compare before you subtract.
- Brace initialization rejects narrowing conversions at compile time. Use it for conversions between integer types of different widths.
- CWE-190 names SafeInt as a safe integer library for C++. It is a third-party library.
