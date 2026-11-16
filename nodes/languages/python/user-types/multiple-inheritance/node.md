---
tag: 08MQ1M
kind: feature
owner: python
slug: multiple-inheritance
name: Multiple Inheritance
aliases: [multiple base classes, method resolution order, MRO, __mro__, C3 linearization, diamond inheritance, mixin]
area: languages/python/user-types
level: core
summary: >-
  A class can have several base classes. Python searches them in the method resolution order (MRO), made by the C3 algorithm, which keeps the order of the bases and visits each class once.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#multiple-inheritance", locator: "8.8.1. Multiple inheritance"}
see_also: [python/inheritance, python/classes, python/class-definition-syntax]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#multiple-inheritance
    title: The Python Tutorial, 9. Classes
    locator: 9.5.1. Multiple Inheritance
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#multiple-inheritance
    title: The Python Language Reference, 8. Compound statements
    locator: 8.8.1. Multiple inheritance
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/mro.html#the-c3-method-resolution-order
    title: The Python 2.3 Method Resolution Order
    locator: The C3 Method Resolution Order
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#super
    title: Built-in Functions, Python 3.15 documentation
    locator: super
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#inheritance
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why is there no type inheritance?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class A:
    def who(self):
        return "A"

class B(A):
    def who(self):
        return "B"

class C(A):
    def who(self):
        return "C"

class D(B, C):
    pass

print(D().who())
print([k.__name__ for k in D.__mro__])
```

```output for=basics
B
['D', 'B', 'C', 'A', 'object']
```

D has two bases, and both come from A. The MRO puts C before A, so A comes after all of its subclasses.

## Semantics

**r1.** "Python classes may have multiple base classes, a technique known as multiple inheritance. The base classes are specified in the class definition by listing them in parentheses after the class name, separated by commas."[^2]

**r2.** "The method resolution order (MRO) is the order in which base classes are searched when looking up an attribute on a class."[^2] "In the resolved MRO of a class, the class’s bases appear in the order they were specified in the class’s bases list. Additionally, the MRO always lists a child class before any of its bases."[^2]

**r3.** "For most purposes, in the simplest cases, you can think of the search for attributes inherited from a parent class as depth-first, left-to-right, not searching twice in the same class where there is an overlap in the hierarchy."[^1] "In fact, it is slightly more complex than that; the method resolution order changes dynamically to support cooperative calls to super()."[^1]

**r4.** The algorithm "linearizes the search order in a way that preserves the left-to-right ordering specified in each class, that calls each parent only once, and that is monotonic (meaning that a class can be subclassed without affecting the precedence order of its parents)."[^1] It is C3: "the linearization of C is the sum of C plus the merge of the linearizations of the parents and the list of the parents."[^3]

**r5.** super() follows the MRO. "The search starts from the class right after the type."[^4] This "makes it possible to implement “diamond diagrams” where multiple base classes implement the same method."[^4] Example: [`cooperative`](#cooperative).

**r6.** "Multiple inheritance is not always allowed. Attempting to define a class with multiple inheritance will raise an error if one of the bases does not allow subclassing, if a consistent MRO cannot be created, if no valid metaclass can be determined, or if there is an instance layout conflict."[^2] Examples: [`bad-mro`](#bad-mro), [`layout`](#layout).

## Examples

```python run id=cooperative
class Base:
    def hello(self):
        return ["Base"]

class Left(Base):
    def hello(self):
        return ["Left"] + super().hello()

class Right(Base):
    def hello(self):
        return ["Right"] + super().hello()

class Both(Left, Right):
    def hello(self):
        return ["Both"] + super().hello()

print(Both().hello())
print(Left().hello())
```

```output for=cooperative
['Both', 'Left', 'Right', 'Base']
['Left', 'Base']
```

In a Both instance, super() in Left goes to Right, the next class in the MRO of Both. In a Left instance, it goes to Base. Each class runs once (rules r4 and r5).

```python run id=mixin
import json

class JsonMixin:
    def to_json(self):
        return json.dumps(vars(self), sort_keys=True)

class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

class JsonPoint(JsonMixin, Point):
    pass

print(JsonPoint(1, 2).to_json())
```

```output for=mixin
{"x": 1, "y": 2}
```

A small base class can add one method to many classes.

## Pitfalls

The bases must agree with the order of their own bases:

```python fails id=bad-mro expect=exception:TypeError
class Base: pass
class Child(Base): pass
class Grandchild(Base, Child): pass
```

"In the MRO of Grandchild, Base must appear before Child because it is first in the base class list, but it must also appear after Child because it is a parent of Child."[^2] Write `class Grandchild(Child, Base)`, or only `class Grandchild(Child)`.

Two bases with different instance layouts cannot be joined:

```python fails id=layout expect=exception:TypeError
class Solid1:
    __slots__ = ("solid1",)

class Solid2:
    __slots__ = ("solid2",)

class C4(Solid1, Solid2):
    pass
```

In CPython, "a class is a solid base if it has a nonempty __slots__ definition."[^2] C4 has two solid bases, and neither is a subclass of the other (rule r6).

## Comparison

Go has no inheritance, so it has no MRO. The Go FAQ says that a type that satisfies interfaces gets a part of the same benefit: "Types can satisfy many interfaces at once, without the complexities of traditional multiple inheritance."[^5] In Python a class gets code from all its bases, and the MRO decides which base wins for each name (rule r2).
