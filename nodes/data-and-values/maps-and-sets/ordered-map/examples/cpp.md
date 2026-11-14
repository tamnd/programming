```cpp run id=std-map
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

// A comparator that ignores case. Keys that compare equivalent are one key.
struct NoCase {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), [](unsigned char x, unsigned char y) {
            return std::tolower(x) < std::tolower(y);
        });
    }
};

int main() {
    // std::map keeps its keys sorted, usually in a red-black tree.
    std::map<int, std::string> m{{30, "c"}, {10, "a"}, {50, "e"}, {20, "b"}, {40, "d"}};
    for (const auto& [k, v] : m) std::cout << k << "=" << v << (k == 50 ? "\n" : " ");
    std::cout << m.begin()->first << " " << m.rbegin()->first << "\n";

    // lower_bound gives the first key >= x, and upper_bound the first key > x.
    auto ceil = m.lower_bound(35);
    auto floor = std::prev(m.upper_bound(35));
    std::cout << floor->first << " " << ceil->first << "\n";
    std::cout << (m.upper_bound(5) == m.begin()) << " " << (m.lower_bound(99) == m.end()) << "\n";

    // A range of keys is a pair of iterators. erase can remove the whole range.
    for (auto it = m.lower_bound(20); it != m.lower_bound(45); ++it) std::cout << it->first << ",";
    std::cout << " end\n";
    m.erase(m.begin(), m.lower_bound(30));
    std::cout << m.size() << " " << m.begin()->first << "\n";

    // A custom order: "Apple" and "APPLE" are equivalent, so the map keeps one key.
    std::map<std::string, int, NoCase> ci;
    ci["Apple"] = 1;
    ci["APPLE"] = 2;
    std::cout << ci.size() << " " << ci.begin()->first << "=" << ci.begin()->second << "\n";
}
```

```output
10=a 20=b 30=c 40=d 50=e
10 50
30 40
1 1
20,30,40, end
3 30
1 Apple=2
```

- Verdict: stdlib. std::map keeps its keys sorted, usually in a red-black tree. lower_bound and upper_bound give ceiling and floor, a range is a pair of iterators, and erase removes a range. A comparator that ignores case makes Apple and APPLE equivalent, so the map keeps one key.
