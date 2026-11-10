```python run id=abc-and-protocol
import math
from abc import ABC, abstractmethod
from typing import Protocol, runtime_checkable


class Shape(ABC):
    @abstractmethod
    def area(self) -> float: ...

    def describe(self) -> str:
        return f"{type(self).__name__} {self.area():.2f}"


class Square(Shape):
    def __init__(self, side: float) -> None:
        self.side = side

    def area(self) -> float:
        return self.side * self.side


@runtime_checkable
class HasArea(Protocol):
    def area(self) -> float: ...


class Circle:
    def __init__(self, radius: float) -> None:
        self.radius = radius

    def area(self) -> float:
        return math.pi * self.radius * self.radius


def total(items: list[HasArea]) -> float:
    return sum(item.area() for item in items)


print(Square(2).describe())
print(f"{total([Square(2), Circle(1)]):.2f}")
print(isinstance(Circle(1), HasArea), isinstance(Circle(1), Shape))
print(isinstance(Square(2), HasArea), isinstance(Square(2), Shape))
```

```output
Square 4.00
7.14
True False
True True
```

- Verdict: native. The `abc` module gives nominal interfaces: a class names the ABC as a base class. `typing.Protocol` gives structural interfaces for a static type checker.
- `Shape` has an abstract method `area` and a concrete method `describe`, which plays the part of a default method.
- `Circle` does not name `HasArea`, but it has `area`, so a type checker accepts it in `total`. With `runtime_checkable`, `isinstance` checks only that the attribute is present.
- Python does not check the types at run time. A call to a missing method fails with `AttributeError` when it runs.
