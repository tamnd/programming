```cpp run id=sequence-containers-and-views
#include <deque>
#include <iostream>
#include <list>
#include <ranges>
#include <string>
#include <vector>

// One template for every sequence container: they all have insert, front, back and begin.
template <typename Seq>
std::string demo(Seq s) {
    s.insert(s.begin(), 0);
    s.push_back(9);
    std::string out;
    for (int x : s) {
        out += std::to_string(x);
    }
    return out + " front " + std::to_string(s.front()) + " back " + std::to_string(s.back());
}

int main() {
    std::cout << demo(std::vector<int>{1, 2}) << '\n';
    std::cout << demo(std::deque<int>{1, 2}) << '\n';
    std::cout << demo(std::list<int>{1, 2}) << '\n';

    // A range view is a lazy sequence. iota without an end is infinite; take makes it finite.
    auto odd_squares = std::views::iota(1) | std::views::transform([](int n) { return n * n; }) |
                       std::views::filter([](int n) { return n % 2 == 1; }) | std::views::take(4);
    for (int n : odd_squares) {
        std::cout << n << ' ';
    }
    std::cout << '\n';

    // Views compose without copies. reverse and drop work on the vector in place.
    std::vector<int> v{1, 2, 3, 4, 5};
    for (int n : v | std::views::reverse | std::views::drop(1)) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
}
```

```output
0129 front 0 back 9
0129 front 0 back 9
0129 front 0 back 9
1 9 25 49
4 3 2 1
```

- Verdict: stdlib. One template works on vector, deque and list, because all three are sequence containers with begin, end, front and back. The ranges views iota, transform, filter and take make a lazy pipeline. reverse and drop are views over a vector, so they copy nothing.
