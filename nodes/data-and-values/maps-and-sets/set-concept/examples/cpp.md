```cpp run id=std-set
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

template <class C>
void show(const std::string& name, const C& c) {
    std::cout << name;
    for (int v : c) std::cout << " " << v;
    std::cout << "\n";
}

int main() {
    // std::set keeps unique values in sorted order.
    std::set<int> s{3, 1, 3, 2, 1};
    show("set", s);

    // insert returns a pair: an iterator and whether the value was new.
    auto [it, added] = s.insert(2);
    std::cout << *it << " " << added << " " << s.insert(4).second << "\n";

    // contains (C++20), and erase returns the number of values removed.
    std::cout << s.contains(4) << " " << s.erase(4) << " " << s.erase(9) << "\n";

    // The set algorithms work on sorted ranges, so they fit std::set.
    std::set<int> a{1, 2, 3}, b{2, 3, 4};
    std::vector<int> out;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(out));
    show("intersection", out);
    out.clear();
    std::set_symmetric_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(out));
    show("symmetric", out);
    std::set<int> sub{2, 3};
    std::cout << std::includes(a.begin(), a.end(), sub.begin(), sub.end()) << "\n";

    // An unordered_set is a hash set. Copy it to a sorted vector to print it.
    std::unordered_set<int> h{5, 1, 5, 3};
    std::vector<int> v(h.begin(), h.end());
    std::sort(v.begin(), v.end());
    show("unordered", v);

    // A sorted set can find the first value not less than x.
    std::cout << *a.lower_bound(2) << "\n";
}
```

```output
set 1 2 3
2 0 1
1 1 0
intersection 2 3
symmetric 1 4
1
unordered 1 3 5
2
```

- Verdict: stdlib. std::set keeps sorted unique values, and std::unordered_set is a hash set. insert returns whether the value was new, and erase returns the number removed. set_intersection, set_symmetric_difference and includes work on sorted ranges.
