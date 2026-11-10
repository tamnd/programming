```python run id=first
from collections.abc import Sequence


def first[T](items: Sequence[T]) -> T:
    return items[0]


class Box[T]:
    def __init__(self, item: T) -> None:
        self.item = item


print(first([3, 1, 2]))
print(first("abc"))
print(first.__type_params__)
box: Box[int] = Box("not an int")
print(type(box.item).__name__)
```

```output for=first
3
a
(T,)
str
```

- Verdict: native. A function or a class declares its type parameters in square brackets after its name, as in `first[T]` and `Box[T]`.
- The type parameters are hints for a static type checker. The interpreter does not enforce them, so the line `box: Box[int] = Box("not an int")` runs without an error. A type checker reports it.
- The function keeps its type parameters in the attribute `__type_params__`, but no call uses them to check an argument.
- A bound or a list of allowed types goes after the name of the parameter: `[T: Hashable]` or `[T: (int, str)]`.
