```cpp run id=references-bindings-and-init-statement
#include <map>
#include <print>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words{"red", "green"};
    for (auto& w : words) w += "!";  // a reference changes the element
    for (auto w : words) w += "?";   // a copy changes nothing
    for (const auto& w : words) std::println("{}", w);

    // Structured bindings take each pair of the map apart.
    std::map<std::string, int> ages{{"bob", 27}, {"ann", 31}};
    for (const auto& [name, age] : ages) std::println("{} is {}", name, age);

    // An init statement declares the range in the loop header.
    int total = 0;
    for (std::vector nums{4, 5, 6}; int n : nums) total += n;
    std::println("total: {}", total);
}
```

```output for=references-bindings-and-init-statement
red!
green!
ann is 31
bob is 27
total: 15
```

- Verdict: native. `auto&` binds to each element, so `w += "!"` changes the vector, and `auto` makes a copy, so `w += "?"` is lost. A `std::map` keeps its keys sorted, so `ann` comes first. The init statement keeps `nums` inside the loop.
