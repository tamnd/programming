```cpp run id=anonymous-function-cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words = {"house", "car", "bike"};
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
    std::cout << words[0] << ' ' << words[1] << ' ' << words[2] << '\n';

    std::vector<int> nums = {1, 2, 3};
    std::transform(nums.begin(), nums.end(), nums.begin(), [](int x) { return x * x; });
    std::cout << nums[0] << ' ' << nums[1] << ' ' << nums[2] << '\n';

    int n = 5;
    auto add = [n](int x) { return x + n; };
    std::cout << add(10) << '\n';
}
```

```output for=anonymous-function-cpp
car bike house
1 4 9
15
```

- Verdict: native. The lambdas start with a capture list. `[]` captures nothing, and `[n]` captures `n` by value.
