```cpp run id=lower-bound-and-projection
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<int> xs{90, 12, 58, 31, 47};
    std::ranges::sort(xs); // O(n log n) comparisons, not stable
    for (int t : {58, 50}) {
        auto it = std::ranges::lower_bound(xs, t);
        bool found = it != xs.end() && *it == t;
        std::cout << "search " << t << ": index " << (it - xs.begin())
                  << ", found " << std::boolalpha << found << '\n';
    }
    std::cout << "binary_search 50: " << std::ranges::binary_search(xs, 50) << '\n';

    struct Person { std::string name; int born; };
    std::vector<Person> ps{{"Grace", 1906}, {"Ada", 1815}, {"Alan", 1912}};
    std::ranges::stable_sort(ps, {}, &Person::born); // sort by a projection
    for (const auto& p : ps) std::cout << p.name << ' ' << p.born << '\n';
}
```

```output for=lower-bound-and-projection
search 58: index 3, found true
search 50: index 3, found false
binary_search 50: false
Ada 1815
Grace 1906
Alan 1912
```

- Verdict: stdlib. `<algorithm>` has dozens of algorithms over ranges of iterators, and C++20 added versions in `std::ranges` that take a whole container and a projection. `std::ranges::sort` takes O(n log n) comparisons and is not stable; `stable_sort` is. `lower_bound` returns an iterator to the first item that is not less than the value, and `binary_search` returns only a `bool`. The projection `&Person::born` sorts by one field with no comparison function.
