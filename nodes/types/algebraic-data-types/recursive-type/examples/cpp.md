```cpp run id=list
#include <iostream>
#include <memory>

struct Node {
    int value;
    std::unique_ptr<Node> next;
    static inline int destroyed = 0;
    ~Node() { ++destroyed; }
};

std::unique_ptr<Node> cons(int value, std::unique_ptr<Node> next) {
    auto n = std::make_unique<Node>();
    n->value = value;
    n->next = std::move(next);
    return n;
}

int total(const Node *n) { return n == nullptr ? 0 : n->value + total(n->next.get()); }

int length(const Node *n) { return n == nullptr ? 0 : 1 + length(n->next.get()); }

int main() {
    {
        auto xs = cons(1, cons(2, cons(3, nullptr)));
        std::cout << "sum: " << total(xs.get()) << '\n';
        std::cout << "length: " << length(xs.get()) << '\n';
        std::cout << "list:";
        for (const Node *n = xs.get(); n != nullptr; n = n->next.get()) {
            std::cout << (n == xs.get() ? " " : " -> ") << n->value;
        }
        std::cout << '\n';
        Node::destroyed = 0;
    }
    std::cout << "destroyed: " << Node::destroyed << '\n';
}
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
destroyed: 3
```

- Verdict: native. `Node` holds a `std::unique_ptr<Node>`, which owns the next node. A field of type `Node` itself would not compile, because the class is incomplete inside its own body.
- When the head goes out of scope, each node destroys the next one, so the whole list is freed.
- The destructor of a long list calls itself once for each node, so a very long list can overflow the stack. A loop that releases the nodes one by one avoids this.
