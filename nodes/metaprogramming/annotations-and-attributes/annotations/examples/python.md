```python run id=registry
from dataclasses import dataclass

COMMANDS = {}


def command(name):
    def register(func):
        COMMANDS[name] = func
        return func
    return register


@command("hello")
def hello():
    return "hello, world"


@command("time")
def time_of_day():
    return "noon"


@dataclass(frozen=True)
class Point:
    x: int
    y: int


for name in sorted(COMMANDS):
    print(name, "->", COMMANDS[name]())

p = Point(1, 2)
print(p, p == Point(1, 2))
try:
    p.x = 5
except AttributeError as e:
    print(type(e).__name__)
```

```output for=registry
hello -> hello, world
time -> noon
Point(x=1, y=2) True
FrozenInstanceError
```

A decorator is an expression after `@`. Python evaluates it when the function is defined, calls the result with the function, and binds the name to the return value. `command("hello")` returns `register`, which records the function and returns it unchanged. `@dataclass` reads the field annotations of the class and adds `__init__`, `__repr__` and `__eq__`. With `frozen=True`, assignment to a field raises an exception.

```python wrong id=no-wraps
def logged(func):
    def wrapper(*args, **kwargs):
        print("calling", func.__name__)
        return func(*args, **kwargs)
    return wrapper


@logged
def area(w, h):
    """Return the area of a rectangle."""
    return w * h


print(area(3, 4))
print(area.__name__, area.__doc__)
```

```output for=no-wraps
calling area
12
wrapper None
```

The decorator returns `wrapper`, so the name `area` now refers to `wrapper`. Its name and its docstring are those of `wrapper`. Tools that read them, such as `help` and test reports, show the wrong function.

```python run id=wraps
import functools


def logged(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print("calling", func.__name__)
        return func(*args, **kwargs)
    return wrapper


@logged
def area(w, h):
    """Return the area of a rectangle."""
    return w * h


print(area(3, 4))
print(area.__name__, area.__doc__)
print(area.__wrapped__(5, 6))
```

```output for=wraps
calling area
12
area Return the area of a rectangle.
30
```

`functools.wraps` copies the name, the docstring and other attributes of `func` to `wrapper`, and it adds `__wrapped__`, a reference to the original function.

- Verdict: idiom. Python has no annotations in the sense of Java. A decorator is a function that the program calls when it defines a function or a class, so it can register, wrap or change the decorated object.
- In Python, the word annotation means something else: a type hint on a parameter, a return value or a variable.
