```python run id=move
from typing import Protocol


class Movable(Protocol):
    def get_position(self) -> tuple[int, int]: ...
    def set_position(self, p: tuple[int, int]) -> None: ...


class MoveServant:
    """The moving code is here, in one class only."""

    def move_to(self, serviced: Movable, where: tuple[int, int]) -> None:
        serviced.set_position(where)

    def move_by(self, serviced: Movable, dx: int, dy: int) -> None:
        x, y = serviced.get_position()
        serviced.set_position((x + dx, y + dy))


class Shape:
    def __init__(self, name: str, position: tuple[int, int]):
        self.name, self._position = name, position

    def get_position(self) -> tuple[int, int]:
        return self._position

    def set_position(self, p: tuple[int, int]) -> None:
        self._position = p

    def __str__(self) -> str:
        return f"{self.name} at ({self._position[0]}, {self._position[1]})"


class Rectangle(Shape): pass
class Ellipse(Shape): pass
class Triangle(Shape): pass


servant = MoveServant()
shapes = [Rectangle("rectangle", (0, 0)), Ellipse("ellipse", (10, 0)), Triangle("triangle", (0, 10))]
for s in shapes:
    servant.move_by(s, 3, 4)
    print(s)
servant.move_to(shapes[2], (1, 1))
print(shapes[2])
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. A `typing.Protocol` states what the servant needs, and any class with the two methods can be served.
