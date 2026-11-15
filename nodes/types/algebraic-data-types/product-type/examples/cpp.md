```cpp run id=parts
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

struct Point {
    int x;
    int y;
    bool operator==(const Point&) const = default;
};

struct Empty {};

const char* yn(bool b) { return b ? "yes" : "no"; }

int main() {
    std::pair<int, int> t{3, 4};  // parts by position
    auto [x, y] = t;
    std::cout << "pair: x = " << x << " y = " << y << " first = " << t.first << "\n";

    std::tuple<int, std::string, bool> r{3, "four", true};
    std::cout << "tuple: " << std::get<0>(r) << " " << std::get<1>(r) << " " << yn(std::get<2>(r)) << "\n";

    Point p{3, 4};  // parts by name
    std::cout << "struct: x = " << p.x << " y = " << p.y << "\n";
    std::cout << "same parts equal: " << yn(p == Point{3, 4}) << "\n";
    std::cout << "swapped parts equal: " << yn(p == Point{4, 3}) << "\n";
    std::cout << "empty tuples equal: " << yn(std::tuple<>{} == std::tuple<>{}) << "\n";
    std::cout << "size of an empty struct: " << sizeof(Empty) << "\n";
}
```

```output for=parts
pair: x = 3 y = 4 first = 3
tuple: 3 four yes
struct: x = 3 y = 4
same parts equal: yes
swapped parts equal: no
empty tuples equal: yes
size of an empty struct: 1
```

- Verdict: stdlib. `std::pair` and `std::tuple` are products by position, and a `struct` is a product by name.
- A defaulted `operator==` compares the members in order.
- An empty struct is allowed, but an object of it uses at least one byte.
