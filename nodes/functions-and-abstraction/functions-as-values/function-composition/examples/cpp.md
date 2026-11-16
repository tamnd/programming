```cpp run id=function-composition-cpp
#include <iostream>

auto compose = [](auto f, auto g) {
    return [=](auto x) { return f(g(x)); };
};

int main() {
    auto inc = [](int x) { return x + 1; };
    auto twice = [](int x) { return x * 2; };

    std::cout << compose(twice, inc)(5) << '\n';
    std::cout << compose(inc, twice)(5) << '\n';
    std::cout << compose(inc, compose(twice, inc))(5) << '\n';
}
```

```output for=function-composition-cpp
12
11
13
```

- Verdict: idiom. The standard library has no compose function for plain functions. A generic lambda returns a lambda that captures `f` and `g` by copy and calls both.
