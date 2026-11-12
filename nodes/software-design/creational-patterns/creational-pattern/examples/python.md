```python run id=language-features
import copy
import importlib
import json
from dataclasses import dataclass, replace


class Circle:
    def __init__(self, size):
        self.size = size


class Square:
    def __init__(self, size):
        self.size = size


# A class is a value, so a dict of classes is a factory.
kinds = {"circle": Circle, "square": Square}
shape = kinds["square"](2)
print("factory:", type(shape).__name__, shape.size)

# A copy of an object that you have is a prototype.
template = Square(3)
clone = copy.copy(template)
print("prototype:", type(clone).__name__, clone.size, clone is template)


# Keyword arguments with defaults do the work of a builder.
@dataclass(frozen=True)
class Request:
    url: str
    method: str = "GET"
    timeout: float = 30.0
    retries: int = 0


r = Request("https://example.com/", retries=2)
print("builder:", r)
print("changed copy:", replace(r, method="POST"))

# A module is created once, so an object in a module is a singleton.
print("singleton:", importlib.import_module("json") is json)
```

```output
factory: Square 2
prototype: Square 3 False
builder: Request(url='https://example.com/', method='GET', timeout=30.0, retries=2)
changed copy: Request(url='https://example.com/', method='POST', timeout=30.0, retries=2)
singleton: True
```

- Verdict: idiom. Python makes most creational patterns small. A class is a value, so a table of classes is a factory. `copy.copy` copies an object, keyword arguments with defaults replace most builders, and `dataclasses.replace` makes a changed copy. A module is imported once, so a module-level object is a singleton.
