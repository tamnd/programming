```cpp run id=callback-cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

void repeat(int n, const std::function<void(int)>& callback) {
    for (int i = 1; i <= n; ++i) {
        callback(i);
    }
}

void tick(int i) {
    std::cout << "tick " << i << '\n';
}

int main() {
    std::vector<int> xs = {3, 1, 2};
    std::sort(xs.begin(), xs.end(), [](int a, int b) { return a > b; });
    for (std::size_t i = 0; i < xs.size(); ++i) {
        std::cout << (i ? " " : "") << xs[i];
    }
    std::cout << '\n';
    repeat(2, tick);
}
```

```output for=callback-cpp
3 2 1
tick 1
tick 2
```

- Verdict: native. `std::sort` takes the comparison as a template parameter, so a lambda works. `repeat` takes a `std::function`, which can hold the function `tick`, a lambda or another function object.
