```cpp run id=tree
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <vector>

class Tree {
    struct Node {
        int key;
        std::unique_ptr<Node> left, right;
    };
    std::unique_ptr<Node> root_;

public:
    // A forward iterator: a copy is an independent position in the same tree.
    class iterator {
        std::vector<const Node*> stack_;

        void push_left(const Node* node) {
            for (; node != nullptr; node = node->left.get()) {
                stack_.push_back(node);
            }
        }

    public:
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::forward_iterator_tag;

        iterator() = default;
        explicit iterator(const Node* root) { push_left(root); }

        int operator*() const { return stack_.back()->key; }

        iterator& operator++() {
            const Node* node = stack_.back();
            stack_.pop_back();
            push_left(node->right.get());
            return *this;
        }

        iterator operator++(int) {
            iterator old = *this;
            ++*this;
            return old;
        }

        bool operator==(const iterator&) const = default;
    };

    Tree(std::initializer_list<int> keys) {
        for (int key : keys) {
            insert(key);
        }
    }

    void insert(int key) {
        std::unique_ptr<Node>* link = &root_;
        while (*link) {
            link = key < (*link)->key ? &(*link)->left : &(*link)->right;
        }
        *link = std::make_unique<Node>(Node{key, nullptr, nullptr});
    }

    iterator begin() const { return iterator(root_.get()); }
    iterator end() const { return iterator(); }
};

static_assert(std::forward_iterator<Tree::iterator>);
static_assert(std::ranges::forward_range<Tree>);

int main() {
    const Tree a{5, 3, 8, 1, 4, 9};
    const Tree b{1, 3, 4, 5, 8, 9};
    const Tree c{1, 3, 4, 5, 8};

    std::cout << "in order:";
    int sum = 0;
    for (int key : a) {
        std::cout << ' ' << key;
        sum += key;
    }
    std::cout << "\nsum: " << sum << "\nfirst two:";
    for (int key : a | std::views::take(2)) {
        std::cout << ' ' << key;
    }
    std::cout << "\nsame keys as b: " << (std::ranges::equal(a, b) ? "yes" : "no")
              << "\nsame keys as c: " << (std::ranges::equal(a, c) ? "yes" : "no") << '\n';
}
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. A type with `begin()` and `end()` is a range. The range-based `for` statement and the `std::ranges` algorithms work on it.
- The iterator holds the stack of nodes that are left. The end iterator has an empty stack, so the defaulted `operator==` compares the stacks and finds the end.
- The `static_assert` lines ask the compiler to check the iterator against the C++20 concepts. A missing `difference_type` or a wrong return type fails here, not deep inside an algorithm.
- `std::views::take(2)` is lazy, and `std::ranges::equal` stops at the first difference. C++23 adds `std::generator`, a coroutine type that is a range, which would let `begin` be written with `co_yield`.
