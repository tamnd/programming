```cpp run id=erase-returns-next-and-erase-if
#include <list>
#include <print>
#include <vector>

int main() {
    // erase returns the next valid iterator.
    std::vector<int> v{1, 2, 2, 3, 4};
    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 == 0)
            it = v.erase(it);
        else
            ++it;
    }
    std::println("erase loop: {}", v.size());

    // std::erase_if removes all matches in one call.
    std::vector<int> w{1, 2, 2, 3, 4};
    auto removed = std::erase_if(w, [](int x) { return x % 2 == 0; });
    std::println("erase_if removed {}, left {}", removed, w.size());

    // push_back past the capacity reallocates and invalidates all iterators.
    std::vector<int> g;
    g.reserve(2);
    g.push_back(1);
    g.push_back(2);
    auto cap_before = g.capacity();
    g.push_back(3);
    std::println("capacity grew: {}", g.capacity() > cap_before);

    // A list keeps iterators to the other elements valid.
    std::list<int> l{1, 2, 3};
    auto first = l.begin();
    l.push_back(4);
    l.erase(std::next(first));
    std::println("list front still valid: {}", *first);
}
```

```output for=erase-returns-next-and-erase-if
erase loop: 2
erase_if removed 3, left 2
capacity grew: true
list front still valid: 1
```

- Verdict: native. `v.erase(it)` invalidates `it`, so the loop takes the iterator that `erase` returns and steps only when it erases nothing. `std::erase_if` returns the number of removed elements. The third `push_back` grows the capacity, which reallocates the buffer and invalidates every iterator into `g`. A `std::list` keeps the iterator `first` valid through an insertion and through the erasure of another element.
