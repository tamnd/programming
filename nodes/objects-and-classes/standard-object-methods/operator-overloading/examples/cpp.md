```cpp run id=operator-functions
#include <algorithm>
#include <compare>
#include <iostream>
#include <vector>

class Money {
public:
    explicit Money(long long cents) : cents_(cents) {}

    Money& operator+=(const Money& other) {
        cents_ += other.cents_;
        return *this;
    }

    // == and <=> from the member; C++20 derives !=, <, <=, > and >=.
    friend bool operator==(const Money&, const Money&) = default;
    friend auto operator<=>(const Money&, const Money&) = default;

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        return os << "Money(" << m.cents_ << ")";
    }

private:
    long long cents_;
};

// A non-member + that uses +=.
Money operator+(Money a, const Money& b) {
    a += b;
    return a;
}

int main() {
    Money a{150}, b{250};
    std::cout << a + b << '\n';
    std::cout << std::boolalpha << (a + b == Money{400}) << ' ' << (a < b)
              << ' ' << (a != b) << '\n';
    std::vector<Money> v{b, a};
    std::sort(v.begin(), v.end());
    std::cout << v.front() << '\n';
}
```

```output for=operator-functions
Money(400)
true true true
Money(150)
```

- Verdict: native. Operators are functions with names such as `operator+=` and `operator==`. The member `+=` changes the object, and the non-member `+` copies its left operand and uses `+=`.
- A defaulted `operator<=>` compares the members, and the compiler rewrites `<`, `>`, `<=` and `>=` to use it; a defaulted `operator==` gives `==` and `!=`. `std::sort` uses `<`.
- `operator<<` for `std::ostream` lets `std::cout` print the object.
