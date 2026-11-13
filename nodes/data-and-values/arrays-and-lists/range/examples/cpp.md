```cpp run id=views-iota
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    // views::iota(a, b) gives a, a+1, ... and stops before b. It stores only the bounds.
    auto r = std::views::iota(2, 7);
    for (int i : r) std::cout << i << ' ';
    std::cout << "size " << r.size() << '\n';

    // views::iota(a) has no end, so take makes it finite.
    for (int i : std::views::iota(1) | std::views::take(3)) std::cout << i << ' ';
    std::cout << '\n';

    // stride (C++23) gives a step, and reverse goes down.
    for (int i : std::views::iota(0, 10) | std::views::stride(3)) std::cout << i << ' ';
    for (int i : std::views::iota(0, 5) | std::views::reverse) std::cout << i << ' ';
    std::cout << '\n';

    // A range in the library sense is any begin and end pair, such as a vector.
    std::vector<int> v{10, 20, 30, 40};
    auto part = std::ranges::subrange(v.begin() + 1, v.begin() + 3);
    for (int x : part) std::cout << x << ' ';
    std::cout << "empty " << std::ranges::empty(std::views::iota(5, 5)) << '\n';
}
```

```output
2 3 4 5 6 size 5
1 2 3
0 3 6 9 4 3 2 1 0
20 30 empty 1
```

- Verdict: stdlib. views::iota(2, 7) gives 2 to 6 and has a size of 5. views::iota(1) has no end, and take makes it finite. stride (C++23) gives a step, and reverse counts down. A subrange of a vector is a range in the library sense, and iota(5, 5) is empty.
