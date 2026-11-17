```cpp run id=typed-containers
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<int> v{3, 1, 2};
    v.push_back(4);
    int sum = 0;
    for (int x : v) {
        sum += x;
    }
    std::cout << v.size() << " " << sum << "\n";

    std::unordered_map<std::string, std::vector<int>> m;
    m["a"].push_back(1);
    m["a"].push_back(2);
    std::cout << m["a"].size() << " " << m.count("b") << "\n";
}
```

```output for=typed-containers
4 10
2 0
```

- Verdict: stdlib. `std::vector<T>` and `std::unordered_map<K, V>` are class templates. The compiler makes a version for each type argument, so a `std::vector<int>` holds the `int` values themselves.
