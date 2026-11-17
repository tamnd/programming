```cpp run id=template-template
#include <iostream>
#include <list>
#include <string>
#include <vector>

// Table takes two types and a type constructor C, such as std::vector.
template <typename K, typename V, template <typename...> typename C>
struct Table {
    C<K> keys;
    C<V> values;

    void add(const K& k, const V& v) {
        keys.push_back(k);
        values.push_back(v);
    }
};

int main() {
    Table<std::string, int, std::vector> a;
    Table<std::string, int, std::list> b;
    a.add("x", 1);
    b.add("y", 2);
    std::cout << a.keys[0] << ' ' << a.values.size() << ' ' << b.values.front() << '\n';

    // std::vector<int> is a type; std::vector alone is a class template.
    std::vector<std::vector<int>> grid{{1, 2}, {3}};
    std::cout << grid.size() << ' ' << grid[0].size() << '\n';
}
```

```output for=template-template
x 1 2
2 2
```

- Verdict: native. A class template such as `std::vector` is a type constructor, and `std::vector<int>` is a type. `Table` has a template template parameter `C`, so it takes a type constructor as an argument. This is a higher-kinded parameter: the same `Table` works with `std::vector` and with `std::list`.
