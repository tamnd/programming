```python run id=frozen
from dataclasses import FrozenInstanceError, dataclass, replace


@dataclass(frozen=True)
class Point:
    x: int
    y: int


p = Point(1, 2)
q = replace(p, y=5)
print(p, q)
try:
    p.x = 9
except FrozenInstanceError as error:
    print(type(error).__name__)

word = "abc"
print(word.upper(), word)
tags = frozenset({"a", "b"})
print(sorted(tags | {"c"}), sorted(tags))
print({(1, 2): "point"}[(1, 2)])
```

```output for=frozen
Point(x=1, y=2) Point(x=1, y=5)
FrozenInstanceError
ABC abc
['a', 'b', 'c'] ['a', 'b']
point
```

- Verdict: native. Numbers, `str`, `bytes`, `tuple` and `frozenset` are immutable types. An operation on them returns a fresh object.
- `@dataclass(frozen=True)` adds `__setattr__` and `__delattr__` methods that raise `FrozenInstanceError`. `dataclasses.replace` returns a changed copy.
- The language has no keyword for a constant binding. Any name can get a different object by assignment.
- Immutability is shallow. A tuple can hold a list, and the list can change. Such a tuple is not hashable (see the pitfalls in the node).
