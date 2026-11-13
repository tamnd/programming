```cpp run id=vector-and-list
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

template <typename C>
void show(const char* name, const C& c) {
    std::cout << name << ':';
    for (const auto& x : c) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    // In C++, std::vector is the usual list. std::list is a doubly linked list.
    std::vector<int> v{3, 1, 2};
    std::list<int> l{3, 1, 2};
    std::forward_list<int> f{3, 1, 2};

    v.insert(v.begin() + 1, 9);
    l.insert(std::next(l.begin()), 9);
    f.insert_after(f.begin(), 9);
    show("vector", v);
    show("list", l);
    show("forward_list", f);

    // vector has operator[]. std::list has no index operator, so a program walks to item i.
    std::cout << v[2] << ' ' << *std::next(l.begin(), 2) << '\n';

    // An iterator into a std::list stays valid when other items are inserted.
    auto it = std::next(l.begin(), 2);
    for (int i = 0; i < 100; i++) {
        l.push_front(i);
    }
    std::cout << *it << ' ' << l.size() << '\n';
}
```

```output
vector: 3 9 1 2
list: 3 9 1 2
forward_list: 3 9 1 2
1 1
1 104
```

- Verdict: stdlib. std::vector is the usual list. std::list and std::forward_list are linked lists and hold the same items after one insert. std::list has no index operator, so std::next walks to item 2. An iterator into a std::list stays valid after 100 inserts at the front.
