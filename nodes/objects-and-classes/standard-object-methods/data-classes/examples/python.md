```python run id=dataclass-frozen-order
from dataclasses import dataclass, field, replace


@dataclass(frozen=True, order=True)
class Version:
    major: int
    minor: int
    note: str = field(default="", compare=False)

    def __post_init__(self):
        if self.major < 0 or self.minor < 0:
            raise ValueError("negative part")


a = Version(1, 4, "first")
b = Version(1, 10)
print(a)
print(a < b, a == Version(1, 4), len({a, Version(1, 4)}))
print(sorted([b, a])[0])
print(replace(b, minor=11))
```

```output for=dataclass-frozen-order
Version(major=1, minor=4, note='first')
True True 1
Version(major=1, minor=4, note='first')
Version(major=1, minor=11, note='')
```

- Verdict: stdlib. `@dataclass` writes `__init__`, `__repr__` and `__eq__` from the annotated fields; `frozen=True` adds a hash, and `order=True` adds `<` and the other order methods.
- The methods compare the fields as a tuple in their order, so `Version(1, 4)` is before `Version(1, 10)`.
- `field(compare=False)` leaves `note` out of equality, the hash and the order. `__post_init__` runs after the generated `__init__` to check the values.
