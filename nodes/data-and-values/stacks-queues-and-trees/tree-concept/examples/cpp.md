```cpp run id=tree
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    std::string name;
    int size = 0;
    std::vector<Node> children;
};

void show(const Node& node, int depth) {
    std::string indent(depth * 2, ' ');
    if (node.children.empty()) {
        std::cout << indent << node.name << ' ' << node.size << '\n';
        return;
    }
    std::cout << indent << node.name << "/\n";
    for (const Node& child : node.children) {
        show(child, depth + 1);
    }
}

int count_files(const Node& node) {
    if (node.children.empty()) {
        return 1;
    }
    int count = 0;
    for (const Node& child : node.children) {
        count += count_files(child);
    }
    return count;
}

int total_size(const Node& node) {
    int total = node.size;
    for (const Node& child : node.children) {
        total += total_size(child);
    }
    return total;
}

int height(const Node& node) {
    int best = 0;
    for (const Node& child : node.children) {
        best = std::max(best, 1 + height(child));
    }
    return best;
}

int main() {
    Node root{"project", 0, {
        {"README.md", 120, {}},
        {"src", 0, {{"main.py", 300, {}}, {"util.py", 80, {}}}},
        {"tests", 0, {{"test_main.py", 150, {}}}},
    }};
    show(root, 0);
    std::cout << "files " << count_files(root) << ", total size " << total_size(root)
              << ", height " << height(root) << '\n';
}
```

```output for=tree
project/
  README.md 120
  src/
    main.py 300
    util.py 80
  tests/
    test_main.py 150
files 4, total size 650, height 2
```

- Verdict: idiom. A struct can hold a `std::vector` of its own type, because the vector stores the children on the heap.
- Aggregate initialization builds the whole tree in one expression.
