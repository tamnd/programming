```python run id=tree
from collections.abc import Iterator
from itertools import islice, zip_longest


class Node:
    def __init__(self, key: int) -> None:
        self.key = key
        self.left: Node | None = None
        self.right: Node | None = None


class Tree:
    """A binary search tree. Iteration gives the keys in ascending order."""

    def __init__(self, *keys: int) -> None:
        self._root: Node | None = None
        for key in keys:
            self.insert(key)

    def insert(self, key: int) -> None:
        if self._root is None:
            self._root = Node(key)
            return
        node = self._root
        while True:
            if key < node.key:
                if node.left is None:
                    node.left = Node(key)
                    return
                node = node.left
            else:
                if node.right is None:
                    node.right = Node(key)
                    return
                node = node.right

    def __iter__(self) -> Iterator[int]:
        # A generator: each call makes a fresh iterator with its own stack.
        stack: list[Node] = []
        node = self._root
        while stack or node is not None:
            while node is not None:
                stack.append(node)
                node = node.left
            node = stack.pop()
            yield node.key
            node = node.right


def same_keys(a: Tree, b: Tree) -> bool:
    missing = object()
    return all(x == y for x, y in zip_longest(a, b, fillvalue=missing))


a = Tree(5, 3, 8, 1, 4, 9)
b = Tree(1, 3, 4, 5, 8, 9)  # the same keys, a different shape
c = Tree(1, 3, 4, 5, 8)
print("in order:", *a)
print("sum:", sum(a))
print("first two:", *islice(a, 2))
print("same keys as b:", "yes" if same_keys(a, b) else "no")
print("same keys as c:", "yes" if same_keys(a, c) else "no")
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. A class is iterable when `__iter__` returns an iterator, and an iterator has `__next__`. A generator function returns an object that has both.
- `__iter__` is a generator, so each `for` loop, `sum` and `islice` call gets a fresh iterator with its own stack. Two traversals of the same tree can run at once, as `zip_longest(a, b)` shows.
- `zip_longest` with a sentinel `fillvalue` makes trees of different sizes compare unequal. Plain `zip` stops at the shorter tree.
- The explicit stack keeps the depth of the Python call stack constant. A recursive generator with `yield from` is shorter, but it nests one generator frame for each level of the tree.
