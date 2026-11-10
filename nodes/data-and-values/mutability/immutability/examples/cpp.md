```cpp run id=const-member
#include <print>
#include <vector>

class Temperature {
public:
    explicit Temperature(double celsius) : celsius_(celsius) {}

    double fahrenheit() const {
        ++reads_;
        return celsius_ * 9.0 / 5.0 + 32.0;
    }

    int reads() const { return reads_; }

private:
    const double celsius_;
    mutable int reads_ = 0;
};

int main() {
    const Temperature t{100.0};
    std::println("{} {}", t.fahrenheit(), t.fahrenheit());
    std::println("{}", t.reads());

    constexpr int cells = 4 * 4;
    std::println("{}", cells);

    const std::vector<int> v{1, 2, 3};
    std::vector<int> w = v;
    w.push_back(4);
    std::println("{} {}", v.size(), w.size());
}
```

```output for=const-member
212 212
2
16
3 4
```

- Verdict: native. `const` applies to objects, references, pointers and member functions. `constexpr` asks for a value at compile time.
- In a `const` member function, `this` points to a `const` object, so the function cannot change ordinary members.
- A `mutable` member can change even in a `const` object. The usual use is a cache or a counter that does not change the visible value.
- A `const` reference or pointer is a read-only access path. Another path can still change the object.
- A change to an object that was defined `const`, for example after a `const_cast`, is undefined behaviour.
