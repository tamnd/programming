```cpp run id=deduce
#include <print>
#include <type_traits>
#include <vector>

int main() {
    const int limit = 10;
    int values[3] = {1, 2, 3};
    std::vector<int> v{4, 5};
    int& first = v[0];

    auto a = limit;
    auto b = values;
    auto c = first;
    auto& d = first;
    decltype(auto) e = (v[1]);
    const auto& f = limit;

    std::println("{}", std::is_same_v<decltype(a), int>);
    std::println("{}", std::is_same_v<decltype(b), int*>);
    std::println("{}", std::is_same_v<decltype(c), int>);
    std::println("{}", std::is_same_v<decltype(d), int&>);
    std::println("{}", std::is_same_v<decltype(e), int&>);
    std::println("{}", std::is_same_v<decltype(f), const int&>);
}
```

```output for=deduce
true
true
true
true
true
true
```

- Verdict: native. `auto` follows the rules of template argument deduction for a parameter passed by value.
- Plain `auto` drops the top-level `const` and the reference, and turns an array into a pointer.
- `auto&` and `const auto&` keep a reference. `decltype(auto)` keeps the type that `decltype` gives, here `int&` for the expression `(v[1])`.
