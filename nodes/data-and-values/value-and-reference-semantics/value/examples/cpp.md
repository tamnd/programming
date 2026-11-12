```cpp run id=value-categories
#include <print>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// decltype((e)) gives T& for an lvalue, T&& for an xvalue and T for a prvalue.
template <typename T>
constexpr const char* category() {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return "lvalue";
    } else if constexpr (std::is_rvalue_reference_v<T>) {
        return "xvalue";
    } else {
        return "prvalue";
    }
}
#define CATEGORY(...) category<decltype((__VA_ARGS__))>()

int main() {
    int x = 1;
    std::string s = "text";
    std::vector<int> v{1, 2};
    std::println("{} {} {} {}", CATEGORY(x), CATEGORY(x + 1), CATEGORY(42), CATEGORY(std::move(x)));
    std::println("{} {} {}", CATEGORY(v[0]), CATEGORY(std::string("a")), CATEGORY("literal"));

    // Moving from s transfers its value to t, and s is left in a valid but unspecified state.
    std::string t = std::move(s);
    std::println("{}", t);

    // A copy has its own value.
    std::vector<int> w = v;
    w[0] = 9;
    std::println("{} {}", v[0], w[0]);
}
```

```output for=value-categories
lvalue prvalue prvalue xvalue
lvalue prvalue lvalue
text
1 9
```

- Verdict: native. `decltype((e))` shows the value category of each expression: a variable, an element and a string literal are lvalues, `x + 1`, `42` and a temporary `std::string` are prvalues, and `std::move(x)` is an xvalue. Moving a string transfers its value to another variable. A copy of a vector is an independent value.
