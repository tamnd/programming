```cpp run id=countdown
#include <iostream>
#include <vector>

struct Countdown {
    int start;

    struct iterator {
        int current;
        int operator*() const { return current; }
        iterator& operator++() {
            --current;
            return *this;
        }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    iterator begin() const { return {start}; }
    iterator end() const { return {0}; }
};

int main() {
    for (int n : Countdown{3}) {
        std::cout << n << '\n';
    }
    std::vector<int> v{10, 20, 30};
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}
```

```output for=countdown
3
2
1
10 20 30
```

- Verdict: native. A C++ iterator is a position, not a stream. A range has `begin()` and `end()`, and the loop compares the two positions, reads with `*` and moves with `++`.
- The range-based `for` statement is a short form of the second loop. It uses only `begin`, `end`, `!=`, `++` and `*`, so the small struct above is enough.
- Iterators of `std::vector` are random access: `it + 2` and `it[1]` are valid. Iterators of `std::list` are bidirectional only.
- Most operations that change a container invalidate some of its iterators. Using an invalid iterator is undefined behaviour (see Pitfalls on the concept page).
