```cpp run id=two-layouts
#include <array>
#include <iostream>
#include <memory>

struct Node {
    int value;
    std::unique_ptr<Node> next;
};

int main() {
    std::array<int, 5> array{10, 20, 30, 40, 50};
    std::unique_ptr<Node> head;
    for (auto it = array.rbegin(); it != array.rend(); ++it) {
        head = std::make_unique<Node>(Node{*it, std::move(head)});
    }

    std::cout << "array[3] = " << array[3] << " (0 steps)\n";
    const Node* n = head.get();
    int steps = 0;
    for (int i = 3; i > 0; --i, ++steps) {
        n = n->next.get();
    }
    std::cout << "linked[3] = " << n->value << " (" << steps << " steps)\n";

    head = std::make_unique<Node>(Node{5, std::move(head)});
    std::cout << "after push_front:";
    for (const Node* m = head.get(); m != nullptr; m = m->next.get()) {
        std::cout << ' ' << m->value;
    }
    std::cout << '\n';
}
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: stdlib. The containers library has `std::array`, `std::vector`, `std::deque`, `std::list`, `std::forward_list`, `std::map` and `std::unordered_map`.
- Each node owns the next one through `std::unique_ptr`, so the list frees itself when `head` goes out of scope.
