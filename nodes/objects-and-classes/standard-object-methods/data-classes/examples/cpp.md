```cpp run id=aggregate-with-defaulted-comparisons
#include <algorithm>
#include <compare>
#include <iostream>
#include <set>
#include <vector>

struct Version {
    int major = 0;
    int minor = 0;

    friend bool operator==(const Version&, const Version&) = default;
    friend auto operator<=>(const Version&, const Version&) = default;
};

std::ostream& operator<<(std::ostream& os, const Version& v) {
    return os << "Version{" << v.major << ", " << v.minor << "}";
}

int main() {
    Version a{.major = 1, .minor = 4};
    Version b = a;
    b.minor = 10;
    std::cout << a << ' ' << b << '\n';
    std::cout << std::boolalpha << (a == Version{1, 4}) << ' ' << (a < b) << '\n';

    std::set<Version> seen{a, b, a};
    std::cout << seen.size() << '\n';
    std::vector<Version> all{b, a};
    std::sort(all.begin(), all.end());
    std::cout << all.front() << '\n';
}
```

```output for=aggregate-with-defaulted-comparisons
Version{1, 4} Version{1, 10}
true true
2
Version{1, 4}
```

- Verdict: idiom. An aggregate struct gets construction from a brace list, here with designated initializers, and a member-wise copy. Defaulted `operator==` and `operator<=>` give equality and order from the members.
- `std::set` uses `<`, so the struct can be a key without more code.
- C++ generates no string form; the type needs its own `operator<<`.
