```python run id=total-ordering-and-key
from functools import total_ordering


@total_ordering
class Version:
    def __init__(self, major, minor):
        self.major, self.minor = major, minor

    def _key(self):
        return (self.major, self.minor)

    def __eq__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._key() == other._key()

    def __lt__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._key() < other._key()

    def __hash__(self):
        return hash(self._key())

    def __repr__(self):
        return f"{self.major}.{self.minor}"


vs = [Version(1, 10), Version(1, 2), Version(0, 9)]
print(sorted(vs), Version(1, 2) >= Version(1, 2))
print(sorted(vs, key=lambda v: v.minor))
try:
    Version(1, 0) < "1.0"
except TypeError as e:
    print("TypeError:", e)
```

```output for=total-ordering-and-key
[0.9, 1.2, 1.10] True
[1.2, 0.9, 1.10]
TypeError: '<' not supported between instances of 'Version' and 'str'
```

- Verdict: native. The rich comparison methods `__lt__`, `__le__`, `__gt__` and `__ge__` give a class its order, and `sorted` and `list.sort` use `<`.
- `functools.total_ordering` writes the other three methods from `__eq__` and `__lt__`. Comparing tuples of the fields gives a lexicographic order with no extra code.
- A `key` function gives a different order without a change to the class. When both methods return `NotImplemented`, Python raises `TypeError`, so a `Version` and a string are not ordered.
