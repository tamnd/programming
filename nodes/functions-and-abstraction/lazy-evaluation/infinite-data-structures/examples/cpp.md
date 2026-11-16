```cpp run id=infinite
#include <iostream>
#include <ranges>

bool is_prime(int n) {
    for (int d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    auto primes = std::views::iota(2) | std::views::filter(is_prime);
    for (int p : primes | std::views::take(10)) {
        std::cout << p << ' ';
    }
    std::cout << '\n';

    auto powers = std::views::iota(0) | std::views::transform([](int n) { return 1 << n; });
    for (int x : powers | std::views::take(8)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    auto small_squares = std::views::iota(1)
        | std::views::transform([](int n) { return n * n; })
        | std::views::take_while([](int s) { return s < 50; });
    for (int s : small_squares) {
        std::cout << s << ' ';
    }
    std::cout << '\n';
}
```

```output for=infinite
2 3 5 7 11 13 17 19 23 29 
1 2 4 8 16 32 64 128 
1 4 9 16 25 36 49 
```

- Verdict: stdlib. `std::views::iota(2)` with one argument is an unbounded range. The views `filter`, `transform`, `take` and `take_while` are lazy, so a loop over the result reads a finite prefix.
- C++23 has no `iterate` view, so the powers come from `iota(0)` and a shift.
