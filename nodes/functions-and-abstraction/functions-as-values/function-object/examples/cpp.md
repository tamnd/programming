```cpp run id=function-object-cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

struct Multiplier {
    int factor;
    int calls = 0;

    int operator()(int x) {
        ++calls;
        return x * factor;
    }
};

int main() {
    Multiplier triple{3};
    std::cout << triple(5) << '\n';

    std::vector<int> xs = {1, 2, 3};
    std::vector<int> out(xs.size());
    std::transform(xs.begin(), xs.end(), out.begin(), std::ref(triple));
    for (std::size_t i = 0; i < out.size(); ++i) {
        std::cout << (i ? " " : "") << out[i];
    }
    std::cout << '\n';
    std::cout << triple.calls << '\n';
}
```

```output for=function-object-cpp
15
3 6 9
4
```

- Verdict: native. The struct overloads `operator()`, so `triple(5)` is a call. `std::transform` takes its function object by value, so the program passes `std::ref(triple)` to keep the count in `triple` and not in a copy.
