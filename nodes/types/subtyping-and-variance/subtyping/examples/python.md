```python run id=hierarchy
from abc import ABC, abstractmethod
from typing import Protocol, runtime_checkable


class Shape(ABC):
    @abstractmethod
    def area(self) -> float: ...


class Square(Shape):
    def __init__(self, side: float) -> None:
        self.side = side

    def area(self) -> float:
        return self.side * self.side


class Circle:
    def __init__(self, radius: float) -> None:
        self.radius = radius

    def area(self) -> float:
        return 3.14159 * self.radius * self.radius


@runtime_checkable
class HasArea(Protocol):
    def area(self) -> float: ...


class Fake:
    area = "not a method"


print(isinstance(Square(2), Shape))
print(isinstance(Circle(1), Shape))
Shape.register(Circle)
print(isinstance(Circle(1), Shape))
print(Shape in Circle.__mro__)
print(isinstance(Circle(1), HasArea))
print(isinstance(Fake(), HasArea))
```

```output for=hierarchy
True
False
True
False
True
True
```

- Verdict: native. At run time, `isinstance` follows the class hierarchy. `Square` is a `Shape` because it names `Shape` as a base class.
- `Shape.register(Circle)` makes `Circle` a virtual subclass. `isinstance` accepts it, but `Shape` is not in its method resolution order.
- A type checker such as mypy treats `HasArea` structurally: any class with an `area` method that returns `float` matches.
- At run time, `runtime_checkable` checks only that the attribute exists. `Fake` passes, although its `area` is a string.
