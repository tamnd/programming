```python run id=switch-to-classes
import math
from abc import ABC, abstractmethod


# Before: each function matches on the kind.
def area_before(s):
    match s["kind"]:
        case "circle":
            return math.pi * s["r"] ** 2
        case "square":
            return s["side"] ** 2
        case "triangle":
            return s["base"] * s["height"] / 2
        case _:
            return 0


def perimeter_before(s):
    match s["kind"]:
        case "circle":
            return 2 * math.pi * s["r"]
        case "square":
            return 4 * s["side"]
        case _:
            return 0  # the triangle case was never added


t = {"kind": "triangle", "base": 3, "height": 4}
assert area_before(t) == 6
assert perimeter_before(t) == 0
print("before: the perimeter of a triangle is 0")


# After: each kind is a class, and the base class lists the methods.
class Shape(ABC):
    @abstractmethod
    def area(self): ...

    @abstractmethod
    def perimeter(self): ...


class Circle(Shape):
    def __init__(self, r):
        self.r = r

    def area(self):
        return math.pi * self.r**2

    def perimeter(self):
        return 2 * math.pi * self.r


class Square(Shape):
    def __init__(self, side):
        self.side = side

    def area(self):
        return self.side**2

    def perimeter(self):
        return 4 * self.side


class Triangle(Shape):
    def __init__(self, a, b, c):
        self.sides = (a, b, c)

    def area(self):
        a, b, c = self.sides
        s = (a + b + c) / 2
        return math.sqrt(s * (s - a) * (s - b) * (s - c))

    def perimeter(self):
        return sum(self.sides)


shapes = [Circle(1), Square(2), Triangle(3, 4, 5)]
assert [s.perimeter() for s in shapes] == [2 * math.pi, 8, 12]
assert [s.area() for s in shapes] == [math.pi, 4, 6]


class HalfDone(Shape):
    def area(self):
        return 0


try:
    HalfDone()
except TypeError:
    pass
else:
    raise AssertionError("a class without perimeter was created")
print("after: tests pass")
```

```output
before: the perimeter of a triangle is 0
after: tests pass
```

- The `case _:` branch matches any value. Without it, a `match` with no matching case runs no branch, and the function returns `None`.
- `ABC` and `@abstractmethod` make the missing method an error when the object is created, as the last test shows. Without them, the error would come only at the call to `perimeter`.
