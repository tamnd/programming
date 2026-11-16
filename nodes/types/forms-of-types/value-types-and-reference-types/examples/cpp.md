```cpp run id=copy-ref-shared
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::vector<int> a{1, 2};
    std::vector<int> b = a; // copies the elements
    b.push_back(3);
    std::cout << a.size() << " " << b.size() << "\n";

    std::vector<int>& r = a; // r is another name for a
    r.push_back(9);
    std::cout << a.size() << " " << &r[0] - &a[0] << "\n";

    auto p = std::make_shared<std::vector<int>>(std::vector<int>{1});
    auto q = p; // shares one vector and counts the owners
    q->push_back(2);
    std::cout << p->size() << " " << p.use_count() << "\n";
}
```

```output for=copy-ref-shared
2 3
3 0
2 2
```

- Verdict: idiom. A `std::vector` is a value: its copy constructor copies the elements, so `b` changes alone. A reference is an alias, so a change through `r` is a change to `a`. A `std::shared_ptr` shares one object between owners and counts them.
