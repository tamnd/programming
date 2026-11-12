```cpp run id=comparable-mixin
#include <iostream>
#include <string>

// CRTP: the mixin knows the type of the class that mixes it in.
template <class Derived>
struct Comparable {
    bool less_than(const Derived& other) const { return self().compare(other) < 0; }
    bool between(const Derived& lo, const Derived& hi) const {
        return lo.compare(self()) <= 0 && self().compare(hi) <= 0;
    }

private:
    const Derived& self() const { return static_cast<const Derived&>(*this); }
};

struct Version : Comparable<Version> {
    int major, minor;
    Version(int a, int b) : major(a), minor(b) {}
    int compare(const Version& o) const { return major != o.major ? major - o.major : minor - o.minor; }
    std::string str() const { return std::to_string(major) + "." + std::to_string(minor); }
};

const char* yes(bool flag) { return flag ? "yes" : "no"; }

int main() {
    Version v12(1, 2), v110(1, 10), v20(2, 0);
    std::cout << v12.str() << " < " << v110.str() << ": " << yes(v12.less_than(v110)) << "\n";
    std::cout << v110.str() << " between " << v12.str() << " and " << v20.str() << ": " << yes(v110.between(v12, v20)) << "\n";
    std::cout << v20.str() << " between " << v12.str() << " and " << v110.str() << ": " << yes(v20.between(v12, v110)) << "\n";
}
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: idiom. The curiously recurring template pattern passes the class to its own base, so the mixin can call `compare` with no virtual call.
