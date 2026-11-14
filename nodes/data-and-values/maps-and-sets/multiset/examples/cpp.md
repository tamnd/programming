```cpp run id=std-multiset
#include <iostream>
#include <iterator>
#include <set>
#include <string>

int main() {
    // std::multiset keeps every copy, in sorted order.
    std::multiset<std::string> m{"the", "cat", "and", "the", "hat", "and", "the", "bat"};
    for (const auto& w : m) std::cout << w << " ";
    std::cout << "end\n";
    std::cout << m.count("the") << " " << m.count("dog") << " " << m.size() << "\n";

    // erase(value) removes every copy. erase(iterator) removes one.
    m.erase(m.find("the"));
    std::cout << m.count("the") << "\n";
    std::cout << m.erase("the") << " " << m.count("the") << " " << m.size() << "\n";

    // equal_range gives the run of copies of one value.
    auto [lo, hi] = m.equal_range("and");
    std::cout << std::distance(lo, hi) << "\n";

    // Two multisets compare equal when they hold the same values the same number of times.
    std::multiset<char> a{'l', 'i', 's', 't', 'e', 'n'}, b{'s', 'i', 'l', 'e', 'n', 't'};
    std::cout << (a == b) << "\n";
}
```

```output
and and bat cat hat the the the end
3 0 8
2
2 0 5
2
1
```

- Verdict: stdlib. std::multiset and std::unordered_multiset keep every copy. count gives the number of copies, erase with a value removes all copies, and erase with an iterator removes one. A std::map<K, int> of counts is the other common form.
