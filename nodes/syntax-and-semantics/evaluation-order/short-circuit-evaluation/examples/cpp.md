```cpp run id=overloaded-and-fold
#include <iostream>
#include <string>
#include <vector>

int calls = 0;

bool check(bool v) {
    ++calls;
    return v;
}

// A type with its own && operator.
struct Flag {
    bool v;
};

Flag make(bool v) {
    ++calls;
    return Flag{v};
}

bool operator&&(Flag a, Flag b) { return a.v && b.v; }

// A fold over && keeps short-circuit evaluation.
template <typename... Fs>
bool all_of(Fs... fs) {
    return (... && fs());
}

int main() {
    bool r = check(false) && check(true);
    std::cout << std::boolalpha << r << " calls=" << calls << '\n';

    // An overloaded && is a function call, so both operands are evaluated.
    calls = 0;
    bool r2 = make(false) && make(true);
    std::cout << r2 << " calls=" << calls << '\n';

    calls = 0;
    bool r3 = all_of([] { return check(true); }, [] { return check(false); }, [] { return check(true); });
    std::cout << r3 << " calls=" << calls << '\n';

    // The guard pattern with a bounds test.
    std::vector<std::string> words{"alpha", "beta"};
    for (std::size_t i : {1u, 2u})
        std::cout << i << ": " << (i < words.size() && words[i].size() > 3) << '\n';
}
```

```output for=overloaded-and-fold
false calls=1
false calls=2
false calls=2
1: true
2: false
```

- Verdict: native. The built-in `&&` and `||` short-circuit as in C. An overloaded `operator&&` is an ordinary function call, so both of its operands are evaluated, and `make` runs twice. A fold expression over `&&` uses the built-in operator, so it stops at the second lambda. The bounds test `i < words.size()` guards the index.
