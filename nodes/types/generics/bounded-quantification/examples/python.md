```python run id=protocol-bound
from dataclasses import dataclass
from typing import Protocol


class HasArea(Protocol):
    def area(self) -> float: ...


@dataclass
class Square:
    side: float

    def area(self) -> float:
        return self.side * self.side


@dataclass
class Rect:
    w: float
    h: float

    def area(self) -> float:
        return self.w * self.h


def largest[T: HasArea](shapes: list[T]) -> T:
    return max(shapes, key=lambda s: s.area())


print(largest([Square(2), Square(3)]))
print(largest([Rect(1, 5), Rect(2, 2)]))
```

```output for=protocol-bound
Square(side=3)
Rect(w=1, h=5)
```

- Verdict: native. `[T: HasArea]` gives `T` the upper bound `HasArea`, a `Protocol`. A type checker accepts any type with an `area` method, and the result has the type of the argument, `Square` or `Rect`. Python does not check the bound when the program runs.
