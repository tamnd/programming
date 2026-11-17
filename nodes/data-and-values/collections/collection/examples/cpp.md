```cpp run id=three-kinds
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words{"red", "blue", "red", "green"};
    std::set<std::string> unique(words.begin(), words.end());
    std::map<std::string, int> counts;
    for (const auto& w : words) {
        ++counts[w];
    }

    std::cout << words.size() << ' ' << words[1] << '\n';
    std::cout << unique.size() << ' ' << unique.contains("red") << '\n';
    const char* sep = "";
    for (const auto& [word, n] : counts) {
        std::cout << sep << word << '=' << n;
        sep = " ";
    }
    std::cout << '\n';
}
```

```output for=three-kinds
4 blue
3 1
blue=1 green=1 red=2
```

- Verdict: stdlib. The containers library has sequence containers such as `std::vector` and `std::list`, associative containers such as `std::set` and `std::map`, and unordered containers such as `std::unordered_map`.
- `counts[w]` adds the key with the value 0 when the key is not there, so `++counts[w]` counts each word.
- `std::set` and `std::map` keep their keys sorted. `contains` returns a `bool`, which `std::cout` prints as `1`.
