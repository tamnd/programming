```cpp run id=map-filter-reduce-cpp
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6};
    std::vector<int> evens;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
                 [](int x) { return x % 2 == 0; });
    std::vector<int> squares(evens.size());
    std::transform(evens.begin(), evens.end(), squares.begin(),
                   [](int x) { return x * x; });
    for (std::size_t i = 0; i < squares.size(); ++i) {
        std::cout << (i ? " " : "") << squares[i];
    }
    std::cout << '\n';
    std::cout << std::accumulate(squares.begin(), squares.end(), 0) << '\n';
}
```

```output for=map-filter-reduce-cpp
4 16 36
56
```

- Verdict: stdlib. `std::copy_if` filters, `std::transform` maps and `std::accumulate` reduces. Each one works on iterators, and the caller gives the place for the output.
