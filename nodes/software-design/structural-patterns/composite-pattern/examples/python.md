```python run id=tree
from abc import ABC, abstractmethod


class Entry(ABC):
    """The component: what a file and a directory have in common."""

    def __init__(self, name: str) -> None:
        self.name = name

    @abstractmethod
    def size(self) -> int: ...

    @abstractmethod
    def lines(self, depth: int = 0) -> list[str]: ...


class File(Entry):
    def __init__(self, name: str, size: int) -> None:
        super().__init__(name)
        self._size = size

    def size(self) -> int:
        return self._size

    def lines(self, depth=0):
        return [f"{'  ' * depth}{self.name} {self._size}"]


class Directory(Entry):
    def __init__(self, name: str, *children: Entry) -> None:
        super().__init__(name)
        self.children = list(children)

    def size(self) -> int:
        return sum(child.size() for child in self.children)

    def lines(self, depth=0):
        result = [f"{'  ' * depth}{self.name}/ {self.size()}"]
        for child in self.children:
            result.extend(child.lines(depth + 1))
        return result


project = Directory(
    "project",
    File("readme.md", 200),
    Directory("src", File("main.go", 1200), File("util.go", 300)),
)
print("\n".join(project.lines()))
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. The classic form. `Entry` is the component, `File` is the leaf and `Directory` is the composite. `size` and `lines` work on both, and `Directory` forwards them to its children.
- The child list lives only in `Directory`. A `File` has no `add` method, so code cannot add a child to a file. This is the safe form of the pattern (see the node).
- The recursion depth equals the depth of the tree. CPython raises `RecursionError` above its recursion limit, so a tree that is thousands of levels deep needs an explicit stack.
