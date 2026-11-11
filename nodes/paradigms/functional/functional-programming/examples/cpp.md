```cpp run id=ranges
#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

int main() {
    const std::vector<std::string> words{"map", "filter", "reduce", "fold", "zip"};

    auto lengths = words | std::views::filter([](const auto& w) { return w.size() > 3; })
                         | std::views::transform([](const auto& w) { return w.size(); });
    for (auto n : lengths) std::cout << n << " ";
    std::cout << "\n";

    auto total = std::accumulate(words.begin(), words.end(), std::size_t{0},
                                 [](std::size_t acc, const std::string& w) { return acc + w.size(); });
    std::cout << total << "\n";

    auto compose = [](auto f, auto g) { return [=](auto x) { return g(f(x)); }; };
    auto inc_then_double = compose([](int x) { return x + 1; }, [](int x) { return x * 2; });
    std::cout << inc_then_double(5) << "\n";
}
```

```output for=ranges
6 6 4 
22
12
```

- Verdict: native. C++ has lambdas, `std::function`, the algorithms of `<algorithm>` and `<numeric>`, and lazy range views. `const` and `constexpr` mark values that do not change.
- A lambda that captures by reference can outlive the variables it refers to. Capture by value when the lambda escapes.
