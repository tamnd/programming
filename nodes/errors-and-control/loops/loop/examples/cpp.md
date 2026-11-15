```cpp run id=range-for-and-algorithms
#include <numeric>
#include <print>
#include <vector>

int main() {
    std::vector<int> v{3, 1, 4, 1, 5};

    // The range-based for visits each element.
    int sum = 0;
    for (int x : v) {
        sum += x;
    }
    std::println("range for: {}", sum);

    // The classic three-part for with an index.
    std::vector<int> picked;
    for (std::size_t i = 0; i < v.size(); i += 2) {
        picked.push_back(v[i]);
    }
    std::println("step 2: {} {} {}", picked[0], picked[1], picked[2]);

    // An algorithm hides the loop.
    std::println("accumulate: {}", std::accumulate(v.begin(), v.end(), 0));
}
```

```output for=range-for-and-algorithms
range for: 14
step 2: 3 4 5
accumulate: 14
```

- Verdict: native. C++ has the loops of C and a range-based `for` that visits each element of a container. The index loop with a step of 2 visits the elements at 0, 2 and 4. `std::accumulate` gets the same sum as the range-based `for` with no loop statement in the caller.
