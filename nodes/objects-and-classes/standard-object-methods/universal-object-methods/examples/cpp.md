```cpp run id=defaulted-comparisons-and-hash
#include <compare>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

struct Money {
    std::string currency;
    std::int64_t cents;

    bool operator==(const Money&) const = default;
    auto operator<=>(const Money&) const = default;
};

std::ostream& operator<<(std::ostream& out, const Money& m) {
    return out << m.cents / 100 << '.' << std::setw(2) << std::setfill('0') << m.cents % 100 << ' ' << m.currency;
}

template <>
struct std::hash<Money> {
    std::size_t operator()(const Money& m) const noexcept {
        std::size_t h = std::hash<std::string>{}(m.currency);
        return h ^ (std::hash<std::int64_t>{}(m.cents) + 0x9e3779b9 + (h << 6) + (h >> 2));
    }
};

int main() {
    Money a{"EUR", 1250}, b{"EUR", 1250};
    std::cout << std::boolalpha << (a == b) << ' ' << (a < Money{"USD", 5}) << '\n';
    std::unordered_map<Money, std::string> names{{a, "lunch"}};
    std::cout << names.at(b) << ' ' << a << '\n';
}
```

```output for=defaulted-comparisons-and-hash
true true
lunch 12.50 EUR
```

- Verdict: idiom. C++ has no root class. A type gets equality and ordering from operators: `= default` on `operator==` and `operator<=>` compares the members in the order of their declaration.
- `operator<<` on `std::ostream` is the usual string form. `std::unordered_map` needs a specialization of `std::hash`, which the code must write by hand; it must give equal hashes for equal objects.
- The compiler also writes the copy constructor and the copy assignment, which copy each member.
