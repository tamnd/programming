```python run id=inspect
import inspect
from dataclasses import dataclass, fields


@dataclass
class Point:
    x: int
    y: int

    def total(self, scale: int = 1) -> int:
        return (self.x + self.y) * scale


p = Point(3, 4)
print("type:", type(p).__name__)
for f in fields(p):
    print(f.name, ":", f.type.__name__, "=", getattr(p, f.name))
method = getattr(p, "total")
print("signature:", inspect.signature(method))
print("total(2) =", method(2))
print("attributes:", vars(p))
```

```output for=inspect
type: Point
x : int = 3
y : int = 4
signature: (scale: int = 1) -> int
total(2) = 14
attributes: {'x': 3, 'y': 4}
```

Every object carries its class, and the class carries its methods. `getattr` looks up an attribute by a name that is a string. `inspect.signature` reads the parameters and their annotations.

```python fails expect=exception:AttributeError id=typo
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def total(self):
        return self.x + self.y


p = Point(3, 4)
print(getattr(p, "totl")())
```

A misspelled name is an error only when the line runs. In Python this is true of ordinary attribute access as well, so reflection changes less than in a statically typed language.

```python run id=change
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y


p = Point(3, 4)
setattr(p, "z", 5)
Point.norm1 = lambda self: abs(self.x) + abs(self.y)
print(vars(p))
print(p.norm1())
```

```output for=change
{'x': 3, 'y': 4, 'z': 5}
7
```

Python reflection can change the program as well as read it. `setattr` adds an attribute to one object, and an assignment to the class adds a method to every instance.

- Verdict: native. `type`, `getattr`, `setattr`, `hasattr`, `vars` and `dir` are built-in functions, and the module `inspect` reads signatures, source code and stack frames.
- Annotations are kept at run time, so `inspect.signature` and `dataclasses.fields` can report the declared types. Python does not enforce them.
