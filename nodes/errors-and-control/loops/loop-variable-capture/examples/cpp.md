```cpp run id=capture-by-value-against-by-reference
#include <functional>
#include <print>
#include <vector>

int main() {
    std::vector<std::function<int()>> by_value, by_ref, init;

    // [i] copies the value of each pass.
    for (int i = 0; i < 3; i++)
        by_value.push_back([i] { return i; });

    // [&n] refers to one variable declared before the loop.
    int n = 0;
    for (n = 0; n < 3; n++)
        by_ref.push_back([&n] { return n; });

    // An init capture can compute the stored value.
    for (int i = 0; i < 3; i++)
        init.push_back([sq = i * i] { return sq; });

    for (auto* fs : {&by_value, &by_ref, &init}) {
        for (std::size_t k = 0; k < fs->size(); k++)
            std::print("{}{}", k == 0 ? "" : " ", (*fs)[k]());
        std::println("");
    }
}
```

```output for=capture-by-value-against-by-reference
0 1 2
3 3 3
0 1 4
```

- Verdict: idiom. A C++ lambda lists its captures. `[i]` copies the counter, so each lambda keeps its pass. `[&n]` refers to the one variable `n`, so all three lambdas see 3 after the loop. A capture by reference of a variable declared in the loop would refer to a dead variable after the pass, which is undefined behavior, so the code captures `n`, which lives until the end of `main`.
