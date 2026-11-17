```cpp run id=instantiation
#include <array>
#include <iostream>
#include <string>
#include <vector>

// The body is checked for each type that uses the template.
template <typename C>
std::size_t twice_size(const C& c) {
    return 2 * c.size();
}

// N is a non-type template parameter.
template <typename T, std::size_t N>
T sum(const std::array<T, N>& a) {
    T total{};
    for (const T& x : a) {
        total += x;
    }
    return total;
}

int main() {
    std::cout << twice_size(std::string("abc")) << " " << twice_size(std::vector<int>{1, 2}) << "\n";
    std::cout << sum(std::array<int, 4>{1, 2, 3, 4}) << "\n";
    // twice_size(5) would not compile: int has no size().
}
```

```output for=instantiation
6 4
10
```

- Verdict: native. The compiler makes `twice_size<std::string>` and `twice_size<std::vector<int>>`, and it checks `c.size()` for each of them. A call with an `int` fails inside the template. `std::array<int, 4>` has a number as a template argument.
