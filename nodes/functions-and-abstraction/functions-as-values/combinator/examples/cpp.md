```cpp run id=combinator-cpp
#include <functional>
#include <iostream>

auto constant = [](auto x) { return [x](auto&&...) { return x; }; };
auto flip = [](auto f) { return [f](auto a, auto b) { return f(b, a); }; };
auto both = [](auto p, auto q) {
    return [p, q](auto x) { return p(x) && q(x); };
};

int main() {
    std::cout << std::identity{}(5) << '\n';
    std::cout << constant(7)(99) << '\n';
    auto sub = [](int a, int b) { return a - b; };
    std::cout << flip(sub)(2, 10) << '\n';
    auto is_odd = [](int x) { return x % 2 != 0; };
    auto keep = both(std::not_fn(is_odd), [](int x) { return x > 0; });
    const char* sep = "";
    for (int x = -2; x <= 4; ++x) {
        if (keep(x)) {
            std::cout << sep << x;
            sep = " ";
        }
    }
    std::cout << '\n';
}
```

```output for=combinator-cpp
5
7
8
2 4
```

- Verdict: stdlib. `std::identity` and `std::not_fn` are in the header `<functional>`. A generic lambda that returns a lambda gives `constant`, `flip` and `both`.
