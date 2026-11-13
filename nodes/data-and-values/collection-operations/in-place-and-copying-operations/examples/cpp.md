```cpp run id=sort-copy-and-remove
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

static void show(const char *label, const std::vector<int> &v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    // std::ranges::sort sorts the vector in place.
    std::vector<int> nums{3, 1, 2};
    std::ranges::sort(nums);
    show("sorted:", nums);

    // A copy of a vector is a new vector, so sort the copy.
    const std::vector<int> src{3, 1, 2};
    std::vector<int> copy = src;
    std::ranges::sort(copy);
    show("src:", src);
    show("copy:", copy);

    // The _copy algorithms write the result to another range.
    std::vector<int> back;
    std::ranges::reverse_copy(src, std::back_inserter(back));
    show("reverse_copy:", back);

    // std::remove moves the kept items forward, but the size stays the same.
    std::vector<int> v{1, 0, 2, 0, 3};
    auto tail = std::remove(v.begin(), v.end(), 0);
    std::cout << "after remove: size " << v.size() << ", kept " << (tail - v.begin()) << '\n';
    v.erase(tail, v.end());
    show("after erase:", v);

    // C++20 std::erase does both steps.
    std::vector<int> w{1, 0, 2, 0, 3};
    std::erase(w, 0);
    show("std::erase:", w);
}
```

```output
sorted: 1 2 3
src: 3 1 2
copy: 1 2 3
reverse_copy: 2 1 3
after remove: size 5, kept 3
after erase: 1 2 3
std::erase: 1 2 3
```

- Verdict: native. `std::ranges::sort` sorts in place, and a copy of a `std::vector` is a new vector to sort. `reverse_copy` writes the result to another range. `std::remove` moves the kept items to the front but keeps the size, so the program must call `erase`, and C++20 `std::erase` does both steps.
