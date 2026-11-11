```python run id=shapes
from abc import ABC, abstractmethod
from functools import singledispatch
import math


class Shape(ABC):
    @abstractmethod
    def area(self): ...

    def describe(self):  # calls area through self, so a subclass decides
        return f"{type(self).__name__} with area {self.area():.2f}"


class Circle(Shape):
    def __init__(self, r):
        self.r = r

    def area(self):
        return math.pi * self.r ** 2


class Square(Shape):
    def __init__(self, side):
        self.side = side

    def area(self):
        return self.side ** 2


for s in [Circle(1), Square(2)]:
    print(s.describe())
print([c.__name__ for c in Square.__mro__])  # the order of the lookup


@singledispatch  # dispatch on the type of the first argument
def to_json(x):
    raise TypeError(f"no rule for {type(x).__name__}")


@to_json.register
def _(x: int):
    return str(x)


@to_json.register
def _(x: list):
    return "[" + ",".join(to_json(v) for v in x) + "]"


@to_json.register
def _(x: str):
    return '"' + x + '"'


print(to_json([1, "a", [2, 3]]))
```

```output for=shapes
Circle with area 3.14
Square with area 4.00
['Square', 'Shape', 'ABC', 'object']
[1,"a",[2,3]]
```

```python wrong id=misspelled-override
class Shape:
    def area(self):
        return 0.0

    def describe(self):
        return f"{type(self).__name__} with area {self.area()}"


class Square(Shape):
    def __init__(self, side):
        self.side = side

    def aera(self):  # a typo: this method overrides nothing
        return float(self.side ** 2)


print(Square(3).describe())
```

```output for=misspelled-override
Square with area 0.0
```

- Verdict: native. Every method call in Python looks up the name of the method at run time, first in the object and then in each class of the method resolution order, `__mro__`, so every method can be overridden. `describe` is written once in `Shape` and calls `self.area()`, which runs the method of the subclass. `functools.singledispatch` dispatches a plain function on the type of its first argument, which adds operations to types without changing their classes.
- The wrong block misspells the name of the overriding method. Python finds no `area` in `Square`, so it runs the default in `Shape`, and the area comes out as 0.0. The `typing.override` decorator lets a type checker report a method that overrides nothing.
