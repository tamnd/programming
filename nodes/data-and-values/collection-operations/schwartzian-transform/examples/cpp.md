```cpp run id=projection-and-pairs
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

static int calls = 0;

static int vowels(const std::string &w) {
    ++calls;
    return static_cast<int>(std::ranges::count_if(w, [](char c) { return std::string("aeiou").find(c) != std::string::npos; }));
}

static void show(const std::vector<std::string> &v) {
    for (const auto &w : v) std::cout << w << ' ';
}

int main() {
    const std::vector<std::string> words{"banana", "kiwi", "apple", "fig", "cherry", "date", "plum", "mango"};

    // A projection is called at each comparison, so the key runs many times.
    auto slow = words;
    std::ranges::stable_sort(slow, {}, vowels);
    show(slow);
    std::cout << (calls > static_cast<int>(words.size())) << '\n';

    // Decorate, sort the pairs by the key, undecorate.
    calls = 0;
    std::vector<std::pair<int, std::string>> d;
    for (const auto &w : words) d.emplace_back(vowels(w), w);
    std::ranges::stable_sort(d, {}, &std::pair<int, std::string>::first);
    std::vector<std::string> fast;
    for (auto &p : d) fast.push_back(std::move(p.second));
    show(fast);
    std::cout << calls << '\n';
}
```

```output
fig cherry plum kiwi apple date mango banana 1
fig cherry plum kiwi apple date mango banana 8
```

A projection passed to `std::ranges::stable_sort` runs at each comparison, so the key runs more than 8 times. A vector of pairs holds each key once, the projection `&std::pair<int, std::string>::first` reads the stored key, and a loop moves the words out.
