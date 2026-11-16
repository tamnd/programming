```cpp run id=first-class-function-cpp
#include <functional>
#include <iostream>
#include <map>
#include <string>

int twice(int x) { return x * 2; }

int apply_twice(const std::function<int(int)>& g, int x) { return g(g(x)); }

std::function<int(int)> adder(int n) {
    return [n](int x) { return x + n; };
}

int main() {
    std::function<int(int)> f = twice;
    std::cout << f(5) << '\n';

    std::map<std::string, std::function<int(int)>> ops = {
        {"double", twice},
        {"square", [](int x) { return x * x; }},
    };
    std::cout << ops["square"](4) << '\n';

    std::cout << apply_twice(twice, 3) << '\n';

    auto add5 = adder(5);
    std::cout << add5(10) << '\n';
}
```

```output for=first-class-function-cpp
10
16
12
15
```

- Verdict: native. `std::function<int(int)>` holds a plain function or a lambda. The lambda `[n](int x) { return x + n; }` captures `n` by value.
