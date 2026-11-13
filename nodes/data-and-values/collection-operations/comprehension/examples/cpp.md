```cpp run id=ranges-views
#include <cctype>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <vector>

template <class R>
void print(R&& r) {
    const char* sep = "";
    for (auto&& x : r) {
        std::cout << sep << x;
        sep = " ";
    }
    std::cout << '\n';
}

int main() {
    std::vector<std::string> words{"apple", "fig", "banana", "kiwi", "cherry"};

    // A view pipeline: a source, a filter and a transform.
    auto lengths = words
        | std::views::filter([](const std::string& w) { return w.size() > 3; })
        | std::views::transform([](const std::string& w) { return w.size(); });
    print(lengths);

    // The iterator pair constructors build a set or a vector from a view.
    auto heads = words | std::views::transform([](const std::string& w) { return w[0]; });
    std::set<char> firsts(heads.begin(), heads.end());
    print(firsts);

    // A map is filled with a loop over a filtered view.
    std::map<std::string, std::size_t> withI;
    for (const auto& w : words | std::views::filter([](const std::string& w) { return w.contains('i'); })) {
        withI[w] = w.size();
    }
    for (const auto& [k, v] : withI) {
        std::cout << k << '=' << v << (k == withI.rbegin()->first ? '\n' : ' ');
    }

    // Views are lazy: the transform runs only when the items are read.
    int calls = 0;
    auto upper = words
        | std::views::transform([&calls](const std::string& w) {
              ++calls;
              std::string u = w;
              for (auto& c : u) {
                  c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
              }
              return u;
          })
        | std::views::take(2);
    std::cout << "calls before reading: " << calls << '\n';
    std::vector<std::string> firstTwo(upper.begin(), upper.end());
    print(firstTwo);
    std::cout << "calls after reading: " << calls << '\n';
}
```

```output
5 6 4 6
a b c f k
fig=3 kiwi=4
calls before reading: 0
APPLE FIG
calls after reading: 2
```

- Verdict: stdlib. C++ has no comprehension syntax, but a view pipeline with std::views::filter and std::views::transform has the same parts. The iterator pair constructors build a std::set or a std::vector from a view. A view is lazy, so the transform has run 0 times when the program makes the view, and 2 times after the program reads the two items that std::views::take(2) gives.
