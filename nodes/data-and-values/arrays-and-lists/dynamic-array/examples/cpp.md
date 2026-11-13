```cpp run id=vector-capacity
#include <iostream>
#include <vector>

int main() {
    // std::vector grows its storage when size() reaches capacity().
    std::vector<int> v;
    std::vector<std::size_t> caps{v.capacity()};
    for (int i = 0; i < 100; i++) {
        v.push_back(i);
        if (caps.back() != v.capacity()) {
            caps.push_back(v.capacity());
        }
    }
    std::cout << v.size() << ':';
    for (auto c : caps) {
        std::cout << ' ' << c;
    }
    std::cout << '\n';

    // reserve gets the memory once, so the pushes do not move the items.
    std::vector<int> w;
    w.reserve(50);
    const int* start = w.data();
    for (int i = 0; i < 50; i++) {
        w.push_back(i);
    }
    std::cout << (start == w.data()) << '\n';

    // A push past the capacity moves the items. Old pointers then dangle.
    std::vector<int> x{1, 2, 3};
    x.shrink_to_fit();
    const int* old = x.data();
    x.push_back(4);
    std::cout << (old == x.data()) << ' ' << x.size() << '\n';
}
```

```output
100: 0 1 2 4 8 16 32 64 128
1
0 4
```

- Verdict: stdlib. std::vector in libstdc++ doubles its capacity from 1 to 128 while 100 items are added. After reserve(50), 50 pushes do not move the items. A push past the capacity moves the items to new memory, so the old pointer no longer equals data().
