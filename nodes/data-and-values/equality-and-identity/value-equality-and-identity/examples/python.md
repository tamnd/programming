```python run id=points
from dataclasses import dataclass


@dataclass(frozen=True)
class Point:
    x: int
    y: int


class Box:
    def __init__(self, x, y):
        self.x = x
        self.y = y


a = Point(1, 2)
b = Point(1, 2)
c = a
print("a == b:", a == b)
print("a is b:", a is b)
print("a is c:", a is c)
print("distinct points:", len({a, b, c}))

p = Box(1, 2)
q = Box(1, 2)
print("Box p == q:", p == q)

nan = float("nan")
print("nan == nan:", nan == nan)
print("nan in [nan]:", nan in [nan])
print("None is None:", None is None)
```

```output for=points
a == b: True
a is b: False
a is c: True
distinct points: 1
Box p == q: False
nan == nan: False
nan in [nan]: True
None is None: True
```

- Verdict: native. `==` asks the objects. It calls `__eq__`, and the default `__eq__` compares identity. `is` compares identity and nothing else.
- `@dataclass(frozen=True)` writes `__eq__`, which compares the fields in order, and `__hash__`, so a point can go in a set. `Box` defines neither, so two boxes with the same fields are not equal.
- `nan in [nan]` is true although `nan == nan` is false. A list tests `x is e or x == e` for each element, so the identity test finds the same NaN object.
- PEP 8 says to compare with `None` by `is`.

```python fails expect=exception:TypeError id=eq-without-hash
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return isinstance(other, Point) and (self.x, self.y) == (other.x, other.y)


seen = {Point(1, 2)}
print(len(seen))
```

A class that defines `__eq__` and no `__hash__` gets `__hash__ = None`, so its objects cannot go in a set or be dictionary keys. The fix is a `__hash__` that hashes the same fields that `__eq__` compares, such as `hash((self.x, self.y))`.

```python wrong id=is-number
a = 1000
b = int("1000")
print(a == b)
print(a is b)
```

```output for=is-number
True
False
```

The two values are equal, but they are two objects. CPython shares some small integers, so the same test with `10` can print `True`. The language does not promise this, and since Python 3.8 the compiler warns about `is` with a literal. Use `==` for values and `is` only for `None` and other single objects.
