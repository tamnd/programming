```python run id=stack-adt
from typing import Protocol


class Stack(Protocol):
    def push(self, v: int) -> None: ...
    def pop(self) -> int: ...
    def is_empty(self) -> bool: ...


class ArrayStack:
    def __init__(self) -> None:
        self._items: list[int] = []

    def push(self, v: int) -> None:
        self._items.append(v)

    def pop(self) -> int:
        return self._items.pop()

    def is_empty(self) -> bool:
        return not self._items


class LinkedStack:
    def __init__(self) -> None:
        self._top: tuple[int, object] | None = None

    def push(self, v: int) -> None:
        self._top = (v, self._top)

    def pop(self) -> int:
        assert self._top is not None
        v, self._top = self._top
        return v

    def is_empty(self) -> bool:
        return self._top is None


def drain(s: Stack) -> str:
    for v in (1, 2, 3):
        s.push(v)
    out = []
    while not s.is_empty():
        out.append(str(s.pop()))
    return " ".join(out)


for s in (ArrayStack(), LinkedStack()):
    print(f"{type(s).__name__}: {drain(s)}")
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: idiom. A class gives the operations as methods. `Protocol` names the interface for a type checker, and neither class inherits from it.
- The leading underscore in `_items` and `_top` marks the representation as private by convention. Python does not stop other code from using it.
