---
tag: G97EHD
kind: feature
owner: python
slug: static-and-class-methods
name: Static and class methods
aliases: [staticmethod, classmethod, static method, class method, cls, alternate constructor, factory method]
area: languages/python/user-types
level: core
summary: >-
  @classmethod makes a method that gets the class, not the instance, as its first argument. @staticmethod makes a method that gets no implicit argument. Both can be called on the class or an instance.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#static-method-objects", locator: "3.2.13.5. Static method objects; 3.2.13.6. Class method objects"}
see_also: [python/method-objects, python/object-creation, python/descriptors, python/inheritance]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#classmethod
    title: Built-in Functions
    locator: "@classmethod; @staticmethod"
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#static-method-objects
    title: The Python Language Reference, 3. Data model
    locator: 3.2.13.1. Instance methods; 3.2.13.5. Static method objects; 3.2.13.6. Class method objects
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#composite_literals
    title: Effective Go
    locator: Constructors and composite literals
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Method_declarations
    title: The Go Programming Language Specification
    locator: Method declarations
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    @classmethod
    def from_fahrenheit(cls, f):
        return cls((f - 32) * 5 / 9)

    @staticmethod
    def is_valid(celsius):
        return celsius >= -273.15

    def __repr__(self):
        return f"{type(self).__name__}({self.celsius})"

class Reading(Temperature):
    pass

print(Temperature.from_fahrenheit(212))
print(Reading.from_fahrenheit(32))
print(Temperature.is_valid(-300), Reading(5).is_valid(5))
```

```output for=basics
Temperature(100.0)
Reading(0.0)
False True
```

`from_fahrenheit` gets the class it was called on as `cls`, so the subclass call makes a Reading. `is_valid` gets only its own argument.

## Semantics

**r1.** "A class method receives the class as an implicit first argument, just like an instance method receives the instance."[^1] "A class method can be called either on the class (such as C.f()) or on an instance (such as C().f()). The instance is ignored except for its class."[^1]

**r2.** "If a class method is called for a derived class, the derived class object is passed as the implied first argument."[^1] Example: [`hard-coded`](#hard-coded).

**r3.** "When an instance method object is created by retrieving a classmethod object from a class or instance, its __self__ attribute is the class itself, and its __func__ attribute is the function object underlying the class method."[^2] Example: [`objects`](#objects).

**r4.** "A static method does not receive an implicit first argument."[^1] "A static method can be called either on the class (such as C.f()) or on an instance (such as C().f())."[^1] Example: [`forgot`](#forgot).

**r5.** "When a static method object is retrieved from a class or a class instance, the object actually returned is the wrapped object, which is not subject to any further transformation."[^2] "Moreover, the static method descriptor is also callable, so it can be used in the class definition (such as f())."[^1] Example: [`in-body`](#in-body).

**r6.** "Class methods are different than C++ or Java static methods. If you want those, see staticmethod() in this section."[^1]

## Examples

```python run id=objects
class C:
    @classmethod
    def make(cls):
        return cls()

    @staticmethod
    def helper(x):
        return x * 2

print(C.make.__self__ is C, C().make.__self__ is C)
print(type(C.__dict__["helper"]).__name__, type(C.helper).__name__)
```

```output for=objects
True True
staticmethod function
```

The class method is bound to the class, also through an instance. The static method comes back as the plain function (rules r3 and r5).

```python run id=in-body
class Grid:
    @staticmethod
    def clamp(n):
        return max(0, min(9, n))

    corners = [clamp(-5), clamp(20)]

print(Grid.corners, Grid.clamp(4))
```

```output for=in-body
[0, 9] 4
```

The class body calls `clamp` while the class is being made (rule r5).

```python run id=counter
class Ticket:
    issued = 0

    def __init__(self):
        type(self).record()

    @classmethod
    def record(cls):
        cls.issued += 1

Ticket(); Ticket()
print(Ticket.issued)
```

```output for=counter
2
```

The class method changes the class attribute through `cls`, and `__init__` reaches it through `type(self)`.

## Pitfalls

A factory that names its own class makes the base type for every subclass:

```python wrong id=hard-coded
class Shape:
    @staticmethod
    def unit():
        return Shape()

class Square(Shape):
    pass

print(type(Square.unit()).__name__)
```

```output for=hard-coded
Shape
```

Use `@classmethod` and `return cls()`, so `Square.unit()` makes a Square (rule r2).

A helper without `@staticmethod` gets the instance as its first argument:

```python fails id=forgot expect=exception:TypeError
class Mathx:
    def double(x):
        return x * 2

Mathx().double(4)
```

The call passes the instance and 4, but `double` takes one parameter. Add `@staticmethod` (rule r4).

## Comparison

In Go, "A method is a function with a receiver."[^4] Code that needs no receiver is a plain function in the package. Effective Go shows a constructor as a package function, `func NewFile(fd int, name string) *File`.[^3] NewFile names its result type, `*File`, in its signature. A Python class method instead gets the class through `cls`, so one method serves every subclass (rule r2).
