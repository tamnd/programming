```cpp run id=const-from-lambda
#include <iostream>
#include <string>

int main() {
    const int n = 7;
    const std::string kind = [&] {
        if (n % 2 == 0) {
            return std::string("even");
        }
        return std::string("odd");
    }();
    std::cout << n << ' ' << kind << '\n';
}
```

```output for=const-from-lambda
7 odd
```

- Verdict: idiom. A `const` variable gets its value once, in the initializer. When the value needs an `if` or a loop, a lambda that the program calls at once computes it, so the variable can still be `const` instead of a variable that the code assigns in two branches.
