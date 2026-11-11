```cpp run id=bind
#include <functional>
#include <iostream>
#include <string>

int power(int base, int exponent) {
    int r = 1;
    for (int i = 0; i < exponent; ++i) r *= base;
    return r;
}

int main() {
    auto two_to = std::bind_front(power, 2);
    auto add = [](int a) { return [a](int b) { return a + b; }; };
    std::cout << two_to(10) << " " << add(1)(2) << "\n";
}
```

```output for=bind
1024 3
```

- Verdict: stdlib. `std::bind_front` fixes the first arguments of a callable, and `std::bind_back` fixes the last ones. A lambda that returns a lambda is a curried function.
- The older `std::bind` uses placeholders such as `std::placeholders::_1`. A lambda is usually clearer.

```cpp wrong id=bind-copies
#include <functional>
#include <iostream>

void add_to(int& total, int x) { total += x; }

int main() {
    int total = 0;
    auto add = std::bind_front(add_to, total);
    add(5);
    std::cout << total << "\n";
}
```

```output for=bind-copies
0
```

`std::bind_front` copies its arguments into the call wrapper. The call adds 5 to the copy, and `total` stays 0. Pass `std::ref(total)`, or capture it by reference in a lambda.
