```cpp run id=fixpoint
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

int main() {
    using Fact = std::pair<std::string, std::string>;
    const std::vector<Fact> parent{{"zeus", "ares"}, {"ares", "harmonia"}, {"harmonia", "semele"}, {"semele", "dionysus"}};

    std::set<Fact> ancestor(parent.begin(), parent.end());
    for (bool changed = true; changed;) {
        changed = false;
        std::vector<Fact> derived;
        for (const auto& [x, z] : parent)
            for (const auto& [z2, y] : ancestor)
                if (z == z2) derived.emplace_back(x, y);
        for (auto& f : derived) changed |= ancestor.insert(std::move(f)).second;
    }
    std::cout << ancestor.size() << "\n";
    for (const auto& [x, y] : ancestor)
        if (y == "dionysus") std::cout << x << " ";
    std::cout << "\n";
}
```

```output for=fixpoint
10
ares harmonia semele zeus 
```

- Verdict: absent. C++ has no logic engine. Template metaprogramming and the overload resolution of the compiler perform a kind of search, but programs that need logic programming embed a Prolog or Datalog engine.
