```python run id=parts
from dataclasses import dataclass


@dataclass(frozen=True)
class Point:
    x: int
    y: int


def yn(b: bool) -> str:
    return "yes" if b else "no"


t = (3, 4)  # parts by position
x, y = t
print("tuple:", t, "x =", x, "y =", y)

p = Point(3, 4)  # parts by name
print("record:", p, "x =", p.x, "y =", p.y)
print("same parts equal:", yn(p == Point(3, 4)))
print("swapped parts equal:", yn(p == Point(4, 3)))
print("unit:", (), "equal to unit:", yn(() == ()))
```

```output for=parts
tuple: (3, 4) x = 3 y = 4
record: Point(x=3, y=4) x = 3 y = 4
same parts equal: yes
swapped parts equal: no
unit: () equal to unit: yes
```

- Verdict: native. A `tuple` is a product by position, and a dataclass is a product by name.
- The dataclass generates `__init__`, `__repr__` and `__eq__`, so two points with the same fields are equal.
- The empty tuple `()` has one value, so it acts as the unit.
