```cpp run id=function-wrappers
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

int inc(int n) { return n + 1; }

int twice(int (*f)(int), int x) { return f(f(x)); }

int main() {
    std::cout << twice(inc, 5) << '\n';

    // Each lambda has its own closure type.
    auto a = [](int n) { return n * 2; };
    auto b = [](int n) { return n * 2; };
    std::cout << std::is_same_v<decltype(a), decltype(b)> << '\n';

    // std::function holds any callable with a matching signature.
    int k = 10;
    std::vector<std::function<int(int)>> ops{inc, a, [k](int n) { return n + k; }};
    for (const auto& f : ops) {
        std::cout << f(3) << '\n';
    }

    // A lambda without captures converts to a function pointer.
    std::cout << twice(a, 3) << '\n';

    std::function<int(int)> empty;
    try {
        empty(1);
    } catch (const std::bad_function_call&) {
        std::cout << "bad_function_call\n";
    }
}
```

```output for=function-wrappers
7
0
4
6
13
12
bad_function_call
```

- Verdict: native. `int (*)(int)` is a function pointer type, and `int(int)` is the function type inside `std::function<int(int)>`. Two lambdas with the same text have two closure types, so `is_same_v` gives 0. A `std::vector` of `std::function` holds a function, a lambda and a lambda with a capture. A lambda without captures converts to a function pointer. A call to an empty `std::function` throws `std::bad_function_call`.
