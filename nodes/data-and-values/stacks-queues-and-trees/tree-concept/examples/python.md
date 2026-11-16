```python run id=tree
from dataclasses import dataclass, field


@dataclass
class Node:
    name: str
    size: int = 0
    children: list = field(default_factory=list)


def show(node, depth=0):
    indent = "  " * depth
    if node.children:
        print(f"{indent}{node.name}/")
        for child in node.children:
            show(child, depth + 1)
    else:
        print(f"{indent}{node.name} {node.size}")


def count_files(node):
    if not node.children:
        return 1
    return sum(count_files(child) for child in node.children)


def total_size(node):
    return node.size + sum(total_size(child) for child in node.children)


def height(node):
    return max((1 + height(child) for child in node.children), default=0)


root = Node("project", children=[
    Node("README.md", 120),
    Node("src", children=[Node("main.py", 300), Node("util.py", 80)]),
    Node("tests", children=[Node("test_main.py", 150)]),
])
show(root)
print(f"files {count_files(root)}, total size {total_size(root)}, height {height(root)}")
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

- Verdict: idiom. Python has no general tree type. A dataclass with a `children` list is a node.
- `field(default_factory=list)` gives each node its own list.
- Each function calls itself on the children.
