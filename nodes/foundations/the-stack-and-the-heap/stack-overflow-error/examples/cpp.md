```cpp run id=deep
#include <iostream>
#include <optional>

constexpr int limit = 10000; // well below the depth where the stack runs out

// Returns no value when the depth passes limit, so the stack never overflows.
std::optional<long> depth(long n, int level = 1) {
    if (level > limit) return std::nullopt;
    if (n == 0) return 0;
    auto rest = depth(n - 1, level + 1);
    if (!rest) return std::nullopt;
    return 1 + *rest;
}

long loop(long n) {
    long count = 0;
    while (n > 0) { // one frame for the whole loop
        n--;
        count++;
    }
    return count;
}

int main() {
    std::cout << "depth(100) = " << *depth(100) << "\n";
    if (!depth(1000000)) {
        std::cout << "depth(1000000): stopped at the limit of " << limit << "\n";
    }
    std::cout << "loop(1000000) = " << loop(1000000) << "\n";
}
```

```output for=deep
depth(100) = 100
depth(1000000): stopped at the limit of 10000
loop(1000000) = 1000000
```

- Verdict: native. As in C, nothing checks the depth, and an overflow usually crashes the process.
