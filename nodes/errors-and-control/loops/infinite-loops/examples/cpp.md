```cpp run id=endless-range-and-a-read-loop
#include <print>
#include <ranges>
#include <sstream>
#include <string>

int main() {
    // iota(1) is an endless range; take_while bounds it.
    std::string out;
    auto squares = std::views::iota(1) | std::views::transform([](int n) { return n * n; }) |
                   std::views::take_while([](int s) { return s < 50; });
    for (int sq : squares) {
        if (!out.empty()) out += ' ';
        out += std::to_string(sq);
    }
    std::println("squares under 50: {}", out);

    // for (;;) with a read in the body: the loop ends when the input ends.
    std::istringstream in("3 1 4 1 5");
    int sum = 0;
    for (;;) {
        int v;
        if (!(in >> v)) break;
        sum += v;
    }
    std::println("sum: {}", sum);
}
```

```output for=endless-range-and-a-read-loop
squares under 50: 1 4 9 16 25 36 49
sum: 14
```

- Verdict: idiom. C++ writes an infinite loop as `for (;;)` or `while (true)`, and the second loop leaves with `break` when a read fails. `std::views::iota(1)` has no end, but a view makes values only on request, and `take_while` stops at the first square of 50 or more.
