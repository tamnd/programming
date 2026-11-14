```cpp run id=unordered-map
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> m{{"x", 1}, {"y", 2}};
    m["z"] = 3;
    m["x"] += 10;
    std::cout << m["x"] << " " << m.size() << " " << m.contains("y") << "\n";

    // operator[] on a missing key inserts the key with a value of 0.
    std::cout << m["w"] << " " << m.size() << "\n";

    // find and contains do not insert. at throws for a missing key.
    std::cout << (m.find("q") == m.end()) << " " << m.contains("q") << " " << m.size() << "\n";
    try {
        std::cout << m.at("q") << "\n";
    } catch (const std::out_of_range &) {
        std::cout << "out_of_range\n";
    }

    // insert does not replace a value; insert_or_assign does.
    auto [it, inserted] = m.insert({"x", 99});
    std::cout << inserted << " " << it->second << "\n";
    m.insert_or_assign("x", 99);
    std::cout << m["x"] << "\n";

    // erase returns the number of removed items. Copy to a std::map to print sorted.
    std::cout << m.erase("y") << " " << m.erase("y") << "\n";
    const char *sep = "";
    for (const auto &[k, v] : std::map<std::string, int>(m.begin(), m.end())) {
        std::cout << sep << k << "=" << v;
        sep = " ";
    }
    std::cout << "\n";
}
```

```output
11 3 1
0 4
1 0 4
out_of_range
0 11
99
1 0
w=0 x=99 z=3
```

- Verdict: stdlib. operator[] inserts a missing key with a value of 0, so a read with [] changes the size. find and contains do not insert, and at throws out_of_range. insert does not replace a value, and insert_or_assign does.
