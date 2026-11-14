```cpp run id=list-and-unordered-map
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

// std::unordered_map has no order. A std::list holds the order and the map points
// into the list. This is the usual LRU cache: a hit splices the entry to the back.
class Lru {
    std::size_t cap;
    std::list<std::pair<std::string, int>> order;
    std::unordered_map<std::string, std::list<std::pair<std::string, int>>::iterator> pos;

public:
    explicit Lru(std::size_t c) : cap(c) {}

    std::optional<int> get(const std::string& k) {
        auto it = pos.find(k);
        if (it == pos.end()) return std::nullopt;
        order.splice(order.end(), order, it->second);  // constant time, no copy
        return it->second->second;
    }

    void put(const std::string& k, int v) {
        if (auto it = pos.find(k); it != pos.end()) {
            it->second->second = v;
            order.splice(order.end(), order, it->second);
            return;
        }
        order.emplace_back(k, v);
        pos[k] = std::prev(order.end());
        if (order.size() > cap) {
            pos.erase(order.front().first);
            order.pop_front();
        }
    }

    void show() const {
        for (const auto& [k, v] : order) std::cout << k << "=" << v << " ";
        std::cout << "end\n";
    }
};

int main() {
    Lru c(3);
    c.put("a", 1);
    c.put("b", 2);
    c.put("c", 3);
    c.show();
    c.get("a");
    c.show();
    c.put("d", 4);
    c.show();
    std::cout << c.get("b").has_value() << " " << c.get("c").value() << "\n";
    c.show();
}
```

```output
a=1 b=2 c=3 end
b=2 c=3 a=1 end
c=3 a=1 d=4 end
0 3
a=1 d=4 c=3 end
```

- Verdict: idiom. std::unordered_map has no order, and the standard library has no insertion-ordered map. A std::list holds the order and the map holds iterators into the list. splice moves an entry to the back in constant time, which gives an LRU cache.
