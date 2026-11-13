```cpp run id=span
#include <array>
#include <iostream>
#include <span>
#include <vector>

void show(const char* name, std::span<const int> s) {
    std::cout << name << ':';
    for (int x : s) {
        std::cout << ' ' << x;
    }
    std::cout << " (size " << s.size() << ")\n";
}

int main() {
    // std::span is a view: a pointer and a size. It does not own the items.
    std::vector<int> v{10, 20, 30, 40, 50};
    std::span<int> all(v);
    std::span<int> mid = all.subspan(1, 3);
    mid[0] = 99;
    show("mid", mid);
    show("v", v);

    // first and last take items from either end.
    show("first2", all.first(2));
    show("last2", all.last(2));

    // A span can have a size fixed at compile time.
    std::array<int, 4> a{1, 2, 3, 4};
    std::span<int, 4> fixed(a);
    std::span<int, 2> head = fixed.first<2>();
    std::cout << head.extent << ' ' << std::span<int>::extent << '\n';

    // To copy a part, build a new container from a range of iterators.
    std::vector<int> copy(v.begin() + 1, v.begin() + 3);
    copy[0] = 1;
    show("copy", copy);
    show("v", v);
}
```

```output
mid: 99 30 40 (size 3)
v: 10 99 30 40 50 (size 5)
first2: 10 99 (size 2)
last2: 40 50 (size 2)
2 18446744073709551615
copy: 1 30 (size 2)
v: 10 99 30 40 50 (size 5)
```

- Verdict: stdlib. std::span is a view, so a write through subspan changes the vector. first and last take items from either end. A span can have a fixed extent in its type; a dynamic span has the extent std::dynamic_extent, the largest size_t. A copy needs a new container built from iterators.
