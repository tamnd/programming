```cpp run id=views
#include <iostream>
#include <ranges>

int main() {
    auto odd_squares = std::views::iota(1)
        | std::views::transform([](int n) { return n * n; })
        | std::views::filter([](int n) { return n % 2 == 1; })
        | std::views::take(3);

    for (int n : odd_squares) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}
```

```output for=views
1 9 25 
```

- Verdict: stdlib. C++ evaluates eagerly. The range adaptors in `<ranges>` make views, which compute each element when the loop reads it, so an infinite `iota` is safe.
- `&&`, `||` and `?:` evaluate only the operands that they need. A lambda with no parameters is a thunk, and a function-local `static` variable is initialized on the first call.
