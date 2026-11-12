```cpp run id=chrono-and-dimension-templates
#include <chrono>
#include <iostream>

// A quantity whose type holds the exponents of length (L) and time (T)
template <int L, int T>
struct Q {
    double v;
};
template <int L, int T>
Q<L, T> operator+(Q<L, T> a, Q<L, T> b) { return {a.v + b.v}; }
template <int L1, int T1, int L2, int T2>
Q<L1 - L2, T1 - T2> operator/(Q<L1, T1> a, Q<L2, T2> b) { return {a.v / b.v}; }

using Metres = Q<1, 0>;
using Seconds = Q<0, 1>;
using MetresPerSecond = Q<1, -1>;

int main() {
    using namespace std::chrono;
    using namespace std::chrono_literals;
    auto timeout = 90s + 500ms;  // the type is milliseconds, chosen at compile time
    std::cout << timeout.count() << " ms = " << duration<double>(timeout).count() << " s\n";
    std::cout << "2 min / 30 s = " << 2min / 30s << "\n";
    std::cout << "truncated to seconds: " << duration_cast<seconds>(timeout).count() << "\n";

    MetresPerSecond v = (Metres{5000} + Metres{304.8}) / Seconds{1800};
    std::cout << "speed " << v.v << " m/s, sizeof " << sizeof v << " bytes\n";
}
```

```output for=chrono-and-dimension-templates
90500 ms = 90.5 s
2 min / 30 s = 4
truncated to seconds: 90
speed 2.94711 m/s, sizeof 8 bytes
```

```cpp fails expect=compile id=metres-plus-seconds
template <int L, int T>
struct Q {
    double v;
};
template <int L, int T>
Q<L, T> operator+(Q<L, T> a, Q<L, T> b) { return {a.v + b.v}; }

using Metres = Q<1, 0>;
using Seconds = Q<0, 1>;

int main() {
    auto total = Metres{100} + Seconds{9.58};
    return static_cast<int>(total.v);
}
```

- Verdict: library. `std::chrono::duration` puts the unit of time in the type as a compile-time ratio, so `90s + 500ms` has the type of milliseconds and converts without loss, while `duration_cast` makes a lossy conversion explicit. Templates with integer parameters for the exponents of each dimension check physical units at compile time: `Q<1, 0> / Q<0, 1>` has the type `Q<1, -1>`, and the value is a single `double`. The fails block adds metres and seconds, and no `operator+` matches. The mp-units library, proposed for a future C++ standard, and Boost.Units do this for all SI quantities.
