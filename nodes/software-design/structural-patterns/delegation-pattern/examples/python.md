```python run id=window-shape
from typing import Protocol


class Window(Protocol):
    title: str


class Shape(Protocol):
    def area(self) -> int: ...
    def describe(self, owner: Window) -> str: ...


class Rectangle:
    def __init__(self, width: int, height: int) -> None:
        self.width, self.height = width, height

    def area(self) -> int:
        return self.width * self.height

    def describe(self, owner: Window) -> str:
        return f"{owner.title}: a rectangle of {self.width} by {self.height}"


class Triangle:
    def __init__(self, base: int, height: int) -> None:
        self.base, self.height = base, height

    def area(self) -> int:
        return self.base * self.height // 2

    def describe(self, owner: Window) -> str:
        return f"{owner.title}: a triangle of {self.base} by {self.height}"


class AppWindow:
    def __init__(self, title: str, shape: Shape) -> None:
        self.title = title
        self.shape = shape  # the delegate

    def area(self) -> int:
        return self.shape.area()  # forward

    def describe(self) -> str:
        return self.shape.describe(self)  # delegate, and pass the receiver


w = AppWindow("main", Rectangle(3, 4))
print("area:", w.area())
print(w.describe())
w.shape = Triangle(3, 4)  # change the behaviour at run time
print("area:", w.area())
print(w.describe())
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: idiom. The window holds the shape in a field and calls it by hand.
- For a wrapper that forwards every attribute, `__getattr__` runs only when normal lookup fails, so one method can forward all the missing names to the delegate.
