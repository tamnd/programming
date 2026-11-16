```python run id=unhashable-without-hash
from dataclasses import dataclass


class Tag:
    def __init__(self, name):
        self.name = name

    def __eq__(self, other):
        return isinstance(other, Tag) and self.name == other.name


print(Tag.__hash__)
try:
    {Tag("x")}
except TypeError as e:
    print("TypeError:", e)


@dataclass(frozen=True)
class FrozenTag:
    name: str


s = {FrozenTag("x"), FrozenTag("x")}
print(len(s), hash(FrozenTag("x")) == hash(FrozenTag("x")))
```

```output for=unhashable-without-hash
None
TypeError: unhashable type: 'Tag'
1 True
```

- Verdict: native. A class that defines `__eq__` and not `__hash__` gets `__hash__` set to `None`, so Python refuses to put it in a set instead of letting it break the set.
- `@dataclass(frozen=True)` generates `__eq__` and `__hash__` from the same fields, and the frozen fields cannot change after the object goes into a set.
- A plain `@dataclass` with `eq=True` and no `frozen` also sets `__hash__` to `None`, because its fields can change.
