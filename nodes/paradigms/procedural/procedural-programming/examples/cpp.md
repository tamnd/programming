```cpp run id=namespaces
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace words {

std::vector<std::string> split(const std::string& text) {
    std::istringstream in(text);
    std::vector<std::string> out;
    for (std::string w; in >> w;) {
        std::transform(w.begin(), w.end(), w.begin(), [](unsigned char c) { return std::tolower(c); });
        out.push_back(w);
    }
    return out;
}

std::map<std::string, int> count(const std::vector<std::string>& ws) {
    std::map<std::string, int> counts;
    for (const auto& w : ws) ++counts[w];
    return counts;
}

std::vector<std::pair<std::string, int>> top(const std::map<std::string, int>& counts, std::size_t n) {
    std::vector<std::pair<std::string, int>> all(counts.begin(), counts.end());
    std::stable_sort(all.begin(), all.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    all.resize(std::min(n, all.size()));
    return all;
}

}  // namespace words

int main() {
    for (const auto& [w, c] : words::top(words::count(words::split("The cat and the hat and the bat")), 2)) {
        std::cout << std::left << std::setw(5) << w << " " << c << "\n";
    }
}
```

```output for=namespaces
the   3
and   2
```

- Verdict: native. C++ keeps the free functions of C and adds namespaces, overloading and references, so a procedural design can be written with no classes.
