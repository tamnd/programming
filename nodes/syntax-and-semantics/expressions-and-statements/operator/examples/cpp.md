```cpp run id=overloading-and-spaceship
#include <compare>
#include <format>
#include <iostream>
#include <string>

struct Money {
    long cents;
    // <=> with = default gives <, <=, > and >= for the type.
    auto operator<=>(const Money&) const = default;
    bool operator==(const Money&) const = default;
};

Money operator+(Money a, Money b) { return {a.cents + b.cents}; }

std::ostream& operator<<(std::ostream& os, Money m) {  // << for output is an overload
    return os << std::format("{}.{:02}", m.cents / 100, m.cents % 100);
}

// operator() makes a function object.
struct Scale {
    int k;
    int operator()(int x) const { return k * x; }
};

// A user-defined literal operator.
constexpr long operator""_usd(unsigned long long d) { return static_cast<long>(d) * 100; }

int main() {
    Money a{150}, b{275};
    std::cout << a + b << "\n";
    std::cout << (a < b) << " " << (a == b) << "\n";
    Scale triple{3};
    std::cout << triple(7) << "\n";
    std::cout << Money{12_usd} << "\n";
    std::string s = std::string("ab") + "cd";
    std::cout << s << "\n";
}
```

```output for=overloading-and-spaceship
4.25
1 0
21
12.00
abcd
```

- Verdict: native. A defaulted `<=>` gives the relational operators, and a defaulted `==` gives equality. `operator+` and `operator<<` are free functions. `operator()` makes `Scale` a function object. `operator""_usd` is a user-defined literal. `+` joins a `std::string` and a string literal.
