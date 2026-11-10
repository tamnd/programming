```python run id=shapes
from dataclasses import dataclass
from math import pi
from typing import assert_never


@dataclass(frozen=True)
class Circle:
    radius: float


@dataclass(frozen=True)
class Rect:
    width: float
    height: float


@dataclass(frozen=True)
class Triangle:
    base: float
    height: float


type Shape = Circle | Rect | Triangle


def area(shape: Shape) -> float:
    match shape:
        case Circle(r):
            return pi * r * r
        case Rect(w, h):
            return w * h
        case Triangle(b, h):
            return b * h / 2
        case _:
            assert_never(shape)


for shape in [Circle(1), Rect(2, 3), Triangle(4, 5)]:
    print(f"{area(shape):.2f}")
print(Rect(2, 3))

try:
    area("square")
except AssertionError as e:
    print(type(e).__name__, e)
```

```output for=shapes
3.14
6.00
10.00
Rect(width=2, height=3)
AssertionError Expected code to be unreachable, but got: 'square'
```

- Verdict: idiom. A union of dataclasses, such as `Circle | Rect | Triangle`, plays the role of a sum type, and `match` takes each case apart.
- The interpreter does not check that the cases are complete. A type checker reports a reachable `assert_never` call, and at run time the call raises an exception.
- A dataclass sets `__match_args__`, so a class pattern can take the fields by position, as in `Circle(r)`. [[python/match-statement]] describes the patterns.
