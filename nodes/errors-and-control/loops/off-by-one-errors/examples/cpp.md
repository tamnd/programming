```cpp run id=end-iterator-iota-and-at
#include <print>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30, 40};

    // end() is one past the last element; the range is [begin, end).
    std::println("size {} = end - begin {}", v.size(), v.end() - v.begin());
    for (auto it = v.begin(); it != v.end(); ++it)
        std::print("{}{}", it == v.begin() ? "" : " ", *it);
    std::println("");

    // iota(a, b) excludes b.
    for (int i : std::views::iota(1, 5))
        std::print("{}{}", i == 1 ? "" : " ", i);
    std::println("");

    // substr takes a start and a count, not an end.
    std::string s = "hamburger";
    std::println("{} {}", s.substr(4, 4), s.substr(4, 8 - 4));

    // at() checks the index; operator[] does not.
    try {
        std::println("{}", v.at(v.size()));
    } catch (const std::out_of_range&) {
        std::println("at({}) is out of range", v.size());
    }
    std::println("last: {}", v.back());
}
```

```output for=end-iterator-iota-and-at
size 4 = end - begin 4
10 20 30 40
1 2 3 4
urge urge
at(4) is out of range
last: 40
```

- Verdict: stdlib. The iterator range of a container is half-open: `end()` points one past the last element, so a loop stops when the iterator equals it, and `end() - begin()` is the size. `std::views::iota(1, 5)` gives 1 to 4. `substr` takes a count, so the end index 8 becomes the count `8 - 4`. `at()` throws `std::out_of_range` for the index 4, where `v[4]` is undefined behavior.
