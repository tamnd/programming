```cpp run id=index-forever-and-iterator-loops
#include <print>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> names{"ann", "bob", "cy"};

    // The counter i lives only in the loop.
    for (std::size_t i = 0; i < names.size(); ++i) {
        std::println("{}: {}", i, names[i]);
    }

    // An empty condition is true, so for (;;) loops until break.
    int tries = 0;
    for (;;) {
        if (++tries == 3) {
            break;
        }
    }
    std::println("tries: {}", tries);

    // The header can declare a variable of any type, here a reverse iterator.
    for (auto it = names.rbegin(); it != names.rend(); ++it) {
        std::println("reverse: {}", *it);
    }
}
```

```output for=index-forever-and-iterator-loops
0: ann
1: bob
2: cy
tries: 3
reverse: cy
reverse: bob
reverse: ann
```

- Verdict: native. C++ has the three-part loop of C. The index loop compares with `names.size()`, `for (;;)` has an empty condition and stops at `break`, and the last loop steps a reverse iterator instead of a number.
