```python run id=keep
from dataclasses import dataclass, replace
from typing import Protocol, Self


class HasXY(Protocol):
    @property
    def x(self) -> int: ...

    @property
    def y(self) -> int: ...

    def with_x(self, x: int) -> Self: ...


@dataclass(frozen=True)
class Point:
    x: int
    y: int

    def with_x(self, x: int) -> Self:
        return replace(self, x=x)


@dataclass(frozen=True)
class Labelled:
    x: int
    y: int
    label: str

    def with_x(self, x: int) -> Self:
        return replace(self, x=x)


def shift[P: HasXY](p: P) -> P:
    return p.with_x(p.x + 1)


a = shift(Point(1, 2))
b = shift(Labelled(1, 2, "home"))
print(f"point: x={a.x} y={a.y}")
print(f"labelled: x={b.x} y={b.y} label={b.label}")
print("same class:", "yes" if type(b) is Labelled else "no")
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
same class: yes
```

- Verdict: idiom. Python has no row types. A `Protocol` lists the members that `shift` needs, and the type parameter `P` (Python 3.12 syntax) makes the result type the same as the argument type.
- `Point` and `Labelled` do not name `HasXY`. A type checker matches them against the protocol by their members.
- At run time, Python checks none of this. The idea block shows the same function on plain dictionaries.
