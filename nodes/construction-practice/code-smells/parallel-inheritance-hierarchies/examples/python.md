```python run id=merge-the-twins
from abc import ABC, abstractmethod


# Before: a shape hierarchy and a renderer hierarchy that mirrors it.
class CircleBefore:
    def __init__(self, r):
        self.r = r


class SquareBefore:
    def __init__(self, side):
        self.side = side


class TriangleBefore:
    def __init__(self, base, height):
        self.base, self.height = base, height


class CircleRenderer:
    def svg(self, c):
        return f'<circle r="{c.r}"/>'


class SquareRenderer:
    def svg(self, s):
        return f'<rect width="{s.side}" height="{s.side}"/>'


RENDERERS = {CircleBefore: CircleRenderer(), SquareBefore: SquareRenderer()}


def svg_before(shape):
    try:
        return RENDERERS[type(shape)].svg(shape)
    except KeyError:
        raise LookupError(f"no renderer for {type(shape).__name__}") from None


# After: each shape draws itself, and the base class requires it.
class Shape(ABC):
    @abstractmethod
    def svg(self) -> str: ...


class Circle(Shape):
    def __init__(self, r):
        self.r = r

    def svg(self):
        return f'<circle r="{self.r}"/>'


class Square(Shape):
    def __init__(self, side):
        self.side = side

    def svg(self):
        return f'<rect width="{self.side}" height="{self.side}"/>'


class Triangle(Shape):
    def __init__(self, base, height):
        self.base, self.height = base, height

    def svg(self):
        return f'<polygon points="0,{self.height} {self.base},{self.height} {self.base / 2:g},0"/>'


assert Circle(5).svg() == svg_before(CircleBefore(5))
assert Square(4).svg() == svg_before(SquareBefore(4))
try:
    svg_before(TriangleBefore(6, 3))
    raise AssertionError("the missing twin must fail")
except LookupError as e:
    assert "TriangleBefore" in str(e)
assert Triangle(6, 3).svg() == '<polygon points="0,3 6,3 3,0"/>'


class Hexagon(Shape):  # forgot svg
    pass


try:
    Hexagon()
    raise AssertionError("an abstract method must block the instance")
except TypeError:
    pass
print("svg: tests pass")
```

```output
svg: tests pass
```

- `Shape` is an abstract base class. A subclass that does not define `svg` cannot be instantiated, so the missing method is found when the object is made, not when it is drawn.
- `{self.base / 2:g}` formats `3.0` as `3`, so the output matches the JavaScript version.
