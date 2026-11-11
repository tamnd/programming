```cpp run id=references
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void add_to(int& total, int amount) { total += amount; }

int main() {
    std::vector<int> data{1, 2, 3, 4, 5};
    for (std::size_t i = 0, j = data.size() - 1; i < j; ++i, --j) {
        std::swap(data[i], data[j]);
    }
    for (int x : data) std::cout << x << " ";
    std::cout << "\n";

    int total = 0;
    add_to(total, 5);
    add_to(total, 7);
    std::cout << total << "\n";

    std::string word = "imperative";
    auto count = std::count_if(word.begin(), word.end(),
                               [](char c) { return std::string("aeiou").find(c) != std::string::npos; });
    std::cout << count << "\n";
}
```

```output for=references
5 4 3 2 1 
12
5
```

- Verdict: native. C++ has the statements of C, references that act as other names for variables, and `const` to mark what must not change.
- A non-const reference parameter such as `int& total` lets the function change the variable of its caller, but the call `add_to(total, 5)` does not show this. Some style guides prefer pointers or return values for this reason.
