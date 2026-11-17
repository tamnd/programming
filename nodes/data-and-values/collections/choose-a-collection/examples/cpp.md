```cpp run id=five-needs
#include <deque>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main() {
    std::istringstream in("the cat saw the dog and the cat ran");
    std::vector<std::string> words;
    for (std::string w; in >> w;) words.push_back(w);

    std::unordered_set<std::string> seen(words.begin(), words.end());
    std::unordered_map<std::string, int> counts;
    for (const auto& w : words) ++counts[w];
    std::deque<std::string> line(words.begin(), words.begin() + 3);
    std::string first = line.front();
    line.pop_front();
    std::string second = line.front();
    line.pop_front();
    std::map<std::string, int> sorted(counts.begin(), counts.end());

    std::cout << "list: " << words.size() << " words, third is " << words[2] << '\n';
    std::cout << "set: " << seen.size() << " distinct\n";
    std::cout << "map: the=" << counts["the"] << " cat=" << counts["cat"] << '\n';
    std::cout << "queue: " << first << ", then " << second << '\n';
    std::cout << "sorted:";
    for (const auto& kv : sorted) std::cout << ' ' << kv.first;
    std::cout << '\n';
}
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

- `std::vector`, `std::unordered_set`, `std::unordered_map`, `std::deque` and `std::map` are the standard containers for the five needs. The `std::map` keeps its keys sorted.
