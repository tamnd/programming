```cpp run id=operator-bool-and-vector-bool
#include <iostream>
#include <memory>
#include <print>
#include <vector>

// explicit operator bool works in a condition but does not convert to int.
struct Connection {
    bool open;
    explicit operator bool() const { return open; }
};

int main() {
    // A pointer, a number or a smart pointer converts to bool in a condition.
    int* p = nullptr;
    auto q = std::make_unique<int>(1);
    std::println("{} {} {}", static_cast<bool>(p), static_cast<bool>(q), static_cast<bool>(0.0));

    Connection c{true};
    if (c) std::println("connected");
    // int n = c; does not compile, because the conversion is explicit.

    // bool takes part in arithmetic as 0 or 1.
    bool t = true;
    std::println("{} {}", t + t, sizeof(bool));

    // Streams print 1 and 0 unless std::boolalpha is set.
    std::cout << true << ' ' << std::boolalpha << true << '\n';

    // std::vector<bool> packs bits, so an element is a proxy, not a bool&.
    std::vector<bool> bits(10, false);
    bits[3] = true;
    auto ref = bits[3];
    ref = false;
    std::println("{} {}", static_cast<bool>(bits[3]), bits.size());
}
```

```output for=operator-bool-and-vector-bool
false true false
connected
2 1
1 true
false 10
```

- Verdict: native. A null pointer, an owning `unique_ptr` and `0.0` convert to `bool` with `static_cast`. `explicit operator bool` lets `Connection` work in an `if` but not convert to `int`. `true + true` is `2`. Streams print `1` for `true` until `std::boolalpha` is set. An element of `std::vector<bool>` is a proxy, so a copy of `bits[3]` still writes to the vector.
