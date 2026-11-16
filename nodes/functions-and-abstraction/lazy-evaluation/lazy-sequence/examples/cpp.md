```cpp run id=pipeline
#include <iostream>
#include <ranges>
#include <vector>

int square(int n) {
    std::cout << "square " << n << '\n';
    return n * n;
}

int main() {
    auto odd_squares = std::views::iota(1)
        | std::views::transform(square)
        | std::views::filter([](int s) { return s % 2 == 1; })
        | std::views::take(2);
    std::vector<int> first;
    for (int s : odd_squares) {
        first.push_back(s);
    }
    std::cout << '[' << first[0] << ", " << first[1] << "]\n";
}
```

```output for=pipeline
square 1
square 1
square 2
square 3
square 3
square 4
square 5
[1, 9]
```

- Verdict: stdlib. `views::iota(1)` is infinite, and the adaptors `transform`, `filter` and `take` build a lazy view. The loop pulls the elements.
- The trace shows two costs of views. `filter` calls `transform` to test an element, and the loop calls it again to read the element, so `square` runs twice for 1 and 3. After the second element, the loop advances the iterator before it compares it with the end, so `filter` searches on to 5. Keep the function of `transform` cheap and free of side effects, or store its results.
