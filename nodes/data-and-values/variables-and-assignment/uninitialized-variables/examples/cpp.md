```cpp run id=optional-result
#include <iostream>
#include <optional>
#include <string>

std::optional<int> parse_digit(const std::string &s) {
    if (s.empty() || s[0] < '0' || s[0] > '9') {
        return std::nullopt;
    }
    return s[0] - '0';
}

int main() {
    int total{};
    for (const char *s : {"4", "x", "5"}) {
        if (auto d = parse_digit(s)) {
            total += *d;
        }
    }
    std::cout << total << '\n';
    std::cout << parse_digit("x").value_or(-1) << '\n';
}
```

```output for=optional-result
9
-1
```

- Verdict: native. As in C, `int total;` in a function has no known value. `int total{};` gives 0. `std::optional` returns "no value" in the type, instead of an output variable that stays unset when the function fails, and `value_or` gives a value for that case.
