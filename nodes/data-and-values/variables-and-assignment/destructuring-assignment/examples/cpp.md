```cpp run id=structured-bindings
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>

struct Point {
    int x;
    int y;
};

std::pair<int, int> divmod(int n, int d) {
    return {n / d, n % d};
}

int main() {
    auto [q, r] = divmod(17, 5);
    std::cout << q << " " << r << "\n";

    Point p{3, 4};
    auto& [x, y] = p;
    x = 10;
    std::cout << p.x << " " << y << "\n";

    auto [cx, cy] = p;
    cx = 99;
    std::cout << p.x << " " << cx << " " << cy << "\n";

    std::map<std::string, int> ages{{"Ada", 36}, {"Alan", 41}};
    for (const auto& [name, age] : ages) {
        std::cout << name << " " << age << "\n";
    }

    auto [a, b, c] = std::tuple{1, 'z', 2.5};
    std::cout << a << b << c << "\n";
}
```

```output for=structured-bindings
3 2
10 4
10 99 4
Ada 36
Alan 41
1z2.5
```

- Verdict: native. A structured binding gives names to the members of a struct, the items of an array, or the elements of a pair or tuple. With `auto&` the names refer to the original object, and with `auto` they refer to a copy.
