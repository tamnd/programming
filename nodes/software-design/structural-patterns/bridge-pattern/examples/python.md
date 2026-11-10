```python run id=shapes
from abc import ABC, abstractmethod


class Renderer(ABC):
    """The implementor: the primitive drawing operations of one output format."""

    name: str

    @abstractmethod
    def circle(self, x: int, y: int, radius: int) -> str: ...

    @abstractmethod
    def polygon(self, points: list[tuple[int, int]]) -> str: ...


class TextRenderer(Renderer):
    name = "text"

    def circle(self, x, y, radius):
        return f"circle at ({x}, {y}) radius {radius}"

    def polygon(self, points):
        return "polygon " + " ".join(f"({x}, {y})" for x, y in points)


class SvgRenderer(Renderer):
    name = "svg"

    def circle(self, x, y, radius):
        return f'<circle cx="{x}" cy="{y}" r="{radius}"/>'

    def polygon(self, points):
        return '<polygon points="' + " ".join(f"{x},{y}" for x, y in points) + '"/>'


class Shape(ABC):
    """The abstraction: it holds a renderer and draws itself with the primitives."""

    def __init__(self, renderer: Renderer):
        self.renderer = renderer

    @abstractmethod
    def draw(self) -> str: ...

    @abstractmethod
    def scale(self, factor: int) -> None: ...


class Circle(Shape):
    def __init__(self, renderer: Renderer, x: int, y: int, radius: int):
        super().__init__(renderer)
        self.x, self.y, self.radius = x, y, radius

    def draw(self):
        return self.renderer.circle(self.x, self.y, self.radius)

    def scale(self, factor):
        self.radius *= factor


class Square(Shape):
    def __init__(self, renderer: Renderer, x: int, y: int, side: int):
        super().__init__(renderer)
        self.x, self.y, self.side = x, y, side

    def draw(self):
        x, y, s = self.x, self.y, self.side
        return self.renderer.polygon([(x, y), (x + s, y), (x + s, y + s), (x, y + s)])

    def scale(self, factor):
        self.side *= factor


for renderer in (TextRenderer(), SvgRenderer()):
    print(f"{renderer.name}:")
    for shape in (Circle(renderer, 1, 1, 2), Square(renderer, 0, 0, 3)):
        print(" ", shape.draw())
        shape.scale(2)
        print(" ", shape.draw())

circle = Circle(TextRenderer(), 1, 1, 2)
circle.renderer = SvgRenderer()  # the implementation changes at run time
print("switched:", circle.draw())
```

```output for=shapes
text:
  circle at (1, 1) radius 2
  circle at (1, 1) radius 4
  polygon (0, 0) (3, 0) (3, 3) (0, 3)
  polygon (0, 0) (6, 0) (6, 6) (0, 6)
svg:
  <circle cx="1" cy="1" r="2"/>
  <circle cx="1" cy="1" r="4"/>
  <polygon points="0,0 3,0 3,3 0,3"/>
  <polygon points="0,0 6,0 6,6 0,6"/>
switched: <circle cx="1" cy="1" r="2"/>
```

- Verdict: idiom. Two class hierarchies, `Shape` and `Renderer`, and a reference from the first to the second. `abc.ABC` marks the methods that each subclass must define.
- Two shapes and two renderers need four classes. A third shape or a third renderer adds one class, not two or three.
- Duck typing makes the `Renderer` base class optional. Any object with `circle` and `polygon` methods works. A `typing.Protocol` lets a type checker confirm this.
- `Shape` does not test which renderer it has. A test such as `isinstance(renderer, SvgRenderer)` in a shape couples the two hierarchies again. The node page shows that bug.
