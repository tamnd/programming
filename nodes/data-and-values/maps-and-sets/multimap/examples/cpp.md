```cpp run id=std-multimap
#include <iostream>
#include <map>
#include <string>

int main() {
    // std::multimap keeps the pairs sorted by key and allows equal keys.
    std::multimap<std::string, int> m;
    m.insert({"yellow", 1});
    m.insert({"blue", 2});
    m.insert({"yellow", 3});
    m.insert({"blue", 4});
    m.insert({"red", 1});

    // equal_range gives all pairs for one key, in insertion order.
    auto [first, last] = m.equal_range("yellow");
    for (auto it = first; it != last; ++it) std::cout << it->second << " ";
    std::cout << "count " << m.count("yellow") << " size " << m.size() << "\n";

    // A multimap has no operator[], because a key has no single value.
    // erase(key) removes every pair with that key.
    std::cout << m.erase("blue") << " " << m.size() << "\n";

    // To remove one pair, erase an iterator.
    m.erase(m.find("yellow"));
    for (const auto& [k, v] : m) std::cout << k << "=" << v << " ";
    std::cout << "end\n";
}
```

```output
1 3 count 2 size 5
2 3
red=1 yellow=3 end
```

- Verdict: stdlib. std::multimap and std::unordered_multimap allow equal keys. equal_range gives all values of a key, count counts them, and erase with a key removes them all. A multimap has no operator[].
