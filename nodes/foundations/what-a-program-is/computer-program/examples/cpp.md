```cpp run id=gcd-with-std-exchange
#include <iostream>
#include <numeric>
#include <utility>

long gcd(long a, long b) {
    while (b != 0) {
        a = std::exchange(b, a % b);
        std::cout << a << ' ' << b << '\n';
    }
    return a;
}

int main() {
    std::cout << "1071 462\n";
    long g = gcd(1071, 462); // the call prints its steps first
    std::cout << "gcd " << g << '\n';
    std::cout << "std::gcd " << std::gcd(1071L, 462L) << '\n';
}
```

```output for=gcd-with-std-exchange
1071 462
462 147
147 21
21 0
gcd 21
std::gcd 21
```

- Verdict: native. A C++ program is a set of translation units linked together, and it starts at `main`, after the constructors of global objects run. `std::exchange(b, a % b)` stores the new value in `b` and returns the old one, which updates both parts of the state in one expression. The standard library also has `std::gcd` in `<numeric>`, which gives the same answer.
