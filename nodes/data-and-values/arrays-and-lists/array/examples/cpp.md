```cpp run id=std-array
#include <array>
#include <iostream>
#include <stdexcept>

void show(const std::array<int, 5>& a) {
    for (int x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

int main() {
    // std::array<T, N> holds N items. Value initialization with {} sets them to 0.
    std::array<int, 5> a{};
    a[0] = 7;
    a[4] = 9;
    show(a);
    std::cout << a.size() << ' ' << sizeof a << '\n';

    // Unlike a C array, a std::array is copied by assignment and compared with ==.
    auto b = a;
    b[1] = 1;
    std::cout << b[1] << ' ' << a[1] << ' ' << (a == b) << '\n';

    // at() checks the index and throws. operator[] does not check.
    try {
        a.at(5) = 1;
    } catch (const std::out_of_range&) {
        std::cout << "out_of_range\n";
    }

    // std::to_array counts the items of a C array or a list.
    auto c = std::to_array({1.5, 2.5, 3.5});
    std::cout << c.size() << ' ' << c.back() << '\n';
}
```

```output
7 0 0 0 9
5 20
1 0 0
out_of_range
3 3.5
```

- Verdict: native. std::array<int, 5> holds the items inline, so sizeof is 20 bytes. Unlike a C array, it is copied by assignment and compared with ==. at checks the index and throws std::out_of_range, while operator[] does not check. std::to_array counts the items of a list.
