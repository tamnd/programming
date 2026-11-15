```cpp run id=count
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

enum class Light { Red, Amber, Green };

const char* name(Light l) {
    switch (l) {
    case Light::Red: return "red";
    case Light::Amber: return "amber";
    case Light::Green: return "green";
    }
    return "?";
}

const char* yn(bool b) { return b ? "yes" : "no"; }

using Pair = std::pair<bool, Light>;       // product: a flag AND a light
using Choice = std::variant<bool, Light>;  // sum: a flag OR a light

int main() {
    const bool flags[] = {false, true};
    const Light lights[] = {Light::Red, Light::Amber, Light::Green};
    std::vector<Pair> pairs;
    std::vector<Choice> choices;
    for (bool f : flags) {
        for (Light l : lights) pairs.emplace_back(f, l);
        choices.emplace_back(f);
    }
    for (Light l : lights) choices.emplace_back(l);

    std::cout << "pairs: " << pairs.size() << "\n";
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        std::cout << (i ? " " : "") << yn(pairs[i].first) << "/" << name(pairs[i].second);
    }
    std::cout << "\nchoices: " << choices.size() << "\n";
    for (std::size_t i = 0; i < choices.size(); ++i) {
        std::cout << (i ? " " : "");
        std::visit([](auto v) {
            if constexpr (std::is_same_v<decltype(v), bool>) {
                std::cout << "flag:" << yn(v);
            } else {
                std::cout << "light:" << name(v);
            }
        }, choices[i]);
    }
    std::cout << "\n";
}
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: stdlib. `std::pair` and `std::tuple` are products, and `std::variant` is a sum.
- `std::visit` calls the lambda with the alternative that the variant holds. The `if constexpr` selects the code for each type at compile time.
- The cases of a `std::variant` are types, not names. A sum with two cases of the same type needs a wrapper struct for each case.
