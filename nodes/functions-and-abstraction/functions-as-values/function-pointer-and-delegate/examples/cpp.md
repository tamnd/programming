```cpp run id=function-pointer-and-delegate-cpp
#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }

int apply(const std::function<int(int, int)>& f, int a, int b) { return f(a, b); }

int main() {
    int (*op)(int, int) = add;
    std::cout << op(6, 7) << '\n';

    std::function<int(int, int)> g = [](int a, int b) { return a * b; };
    std::cout << g(6, 7) << '\n';

    std::cout << apply([](int a, int b) { return a - b; }, 6, 7) << '\n';
}
```

```output for=function-pointer-and-delegate-cpp
13
42
-1
```

- Verdict: native. `op` is a C function pointer. `std::function<int(int, int)>` holds a lambda, and `apply` takes any callable with that signature.
