```cpp run id=deque
#include <deque>
#include <iostream>

void print(const std::deque<int>& d) {
    for (std::size_t i = 0; i < d.size(); i++) {
        std::cout << (i == 0 ? "" : " ") << d[i];
    }
    std::cout << '\n';
}

int main() {
    std::deque<int> d = {2, 3};
    d.push_front(1);
    d.push_back(4);
    print(d);
    std::cout << "front " << d.front() << " back " << d.back() << " d[2] " << d[2] << '\n';

    // A reference to an item stays valid after inserts at the ends.
    const int& second = d[1];
    for (int i = 0; i < 1000; i++) {
        d.push_front(-i);
        d.push_back(i);
    }
    std::cout << "second item still " << second << ", size " << d.size() << '\n';

    d.pop_front();
    d.pop_back();
    std::cout << "front " << d.front() << " back " << d.back() << '\n';
}
```

```output for=deque
1 2 3 4
front 1 back 4 d[2] 3
second item still 2, size 2004
front -998 back 998
```

- Verdict: stdlib. `std::deque` has `push_front`, `push_back`, `pop_front`, `pop_back` and index access with `[]`.
- An insert at either end keeps references to the other items valid. A `std::vector` does not give this guarantee.
