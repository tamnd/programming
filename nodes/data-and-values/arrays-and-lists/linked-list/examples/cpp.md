```cpp run id=list-splice
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>

template <typename C>
void show(const char* name, const C& c) {
    std::cout << name << ':';
    for (const auto& x : c) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    std::list<int> a{1, 2, 3};
    std::list<int> b{10, 20};
    auto two = std::next(a.begin());

    // splice moves the nodes of b into a. No item is copied, and iterators stay valid.
    auto ten = b.begin();
    a.splice(two, b);
    show("a", a);
    std::cout << "b size " << b.size() << ", *two " << *two << ", *ten " << *ten << '\n';

    // erase returns the iterator after the removed node, so a loop can remove as it walks.
    for (auto it = a.begin(); it != a.end();) {
        if (*it % 2 == 1) {
            it = a.erase(it);
        } else {
            ++it;
        }
    }
    show("even", a);

    // forward_list is singly linked, so it inserts and erases after a position.
    std::forward_list<int> f{1, 2, 3};
    f.insert_after(f.before_begin(), 0);
    f.erase_after(f.begin());
    f.reverse();
    show("forward_list", f);

    // A list sorts by relinking its nodes. std::sort needs random access and does not accept it.
    std::list<int> s{5, 3, 4, 1};
    s.sort();
    show("sorted", s);
}
```

```output
a: 1 10 20 2 3
b size 0, *two 2, *ten 10
even: 10 20 2
forward_list: 3 2 0
sorted: 1 3 4 5
```

- Verdict: stdlib. std::list::splice moves the nodes of b into a without a copy, and the iterators two and ten stay valid. erase returns the next iterator, so a loop can remove as it walks. std::forward_list is singly linked and works after a position. list::sort relinks the nodes, because std::sort needs random access.
