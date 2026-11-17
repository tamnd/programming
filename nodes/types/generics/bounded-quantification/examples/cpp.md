```cpp run id=concepts
#include <concepts>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
concept Shape = requires(const T& s) {
    { s.area() } -> std::convertible_to<double>;
};

struct Square {
    double side;
    double area() const { return side * side; }
};

template <Shape S>
double total_area(const std::vector<S>& shapes) {
    double sum = 0;
    for (const auto& s : shapes) {
        sum += s.area();
    }
    return sum;
}

template <typename T>
    requires std::totally_ordered<T>
const T& larger(const T& a, const T& b) {
    return a < b ? b : a;
}

int main() {
    std::cout << total_area(std::vector<Square>{{2}, {3}}) << "\n";
    std::cout << larger(3, 9) << " " << larger(std::string("pear"), std::string("apple")) << "\n";
    std::cout << std::boolalpha << Shape<Square> << " " << Shape<int> << "\n";
}
```

```output for=concepts
13
9 pear
true false
```

- Verdict: native. `Shape` is a concept: a type meets it when `s.area()` gives a value that converts to `double`. `template <Shape S>` and `requires std::totally_ordered<T>` are two ways to put a concept on a template. `Shape<int>` is `false`, so a call of `total_area` with a vector of `int` does not compile.
