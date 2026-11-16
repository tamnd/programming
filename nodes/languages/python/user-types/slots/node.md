---
tag: W9FG9P
kind: feature
owner: python
slug: slots
name: __slots__
aliases: [slots, __slots__, slotted class, fixed attributes, no __dict__, dataclass slots]
area: languages/python/user-types
level: advanced
summary: >-
  A class that sets __slots__ lists the attribute names of its instances. The instances then have no __dict__, so they use less memory and refuse assignment to any name not in the list.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#slots", locator: "3.3.2.4. __slots__"}
see_also: [python/descriptors, python/properties, python/class-and-instance-variables, python/instance-objects]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#slots
    title: The Python Language Reference, 3. Data model
    locator: 3.3.2.4. __slots__; Notes on using __slots__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/dataclasses.html#dataclasses.dataclass
    title: dataclasses, Data Classes
    locator: "@dataclasses.dataclass, parameter slots"
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types; Selectors
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Point:
    __slots__ = ("x", "y")

    def __init__(self, x, y):
        self.x = x
        self.y = y

p = Point(1, 2)
p.x = 10
print(p.x, p.y, hasattr(p, "__dict__"))
try:
    p.z = 3
except AttributeError:
    print("no attribute z")
print(type(Point.__dict__["x"]).__name__)
```

```output for=basics
10 2 False
no attribute z
member_descriptor
```

Point instances can hold only `x` and `y`. Each name in `__slots__` becomes a descriptor in the class.

## Semantics

**r1.** `__slots__` "can be assigned a string, iterable, or sequence of strings with variable names used by instances. __slots__ reserves space for the declared variables and prevents the automatic creation of __dict__ and __weakref__ for each instance."[^1] "The space saved over using __dict__ can be significant."[^1]

**r2.** "Without a __dict__ variable, instances cannot be assigned new variables not listed in the __slots__ definition. Attempts to assign to an unlisted variable name raises AttributeError. If dynamic assignment of new variables is desired, then add '__dict__' to the sequence of strings in the __slots__ declaration."[^1] Example: [`with-dict`](#with-dict).

**r3.** "__slots__ are implemented at the class level by creating descriptors for each variable name. As a result, class attributes cannot be used to set default values for instance variables defined by __slots__".[^1] Example: [`default`](#default).

**r4.** "__slots__ declared in parents are available in child classes. However, instances of a child subclass will get a __dict__ and __weakref__ unless the subclass also defines __slots__ (which should only contain names of any additional slots)."[^1] Example: [`child`](#child).

**r5.** "Multiple inheritance with multiple slotted parent classes can be used, but only one parent is allowed to have attributes created by slots (the other bases must have empty slot layouts) - violations raise TypeError."[^1] Example: [`two-bases`](#two-bases).

**r6.** "Without a __weakref__ variable for each instance, classes defining __slots__ do not support weak references to its instances."[^1] Example: [`weakref`](#weakref).

**r7.** With `@dataclass(slots=True)`, a "__slots__ attribute will be generated and new class will be returned instead of the original one."[^2] Example: [`dataclass`](#dataclass).

## Examples

```python run id=with-dict
class Flexible:
    __slots__ = ("x", "__dict__")

f = Flexible()
f.x = 1
f.note = "extra"
print(f.x, f.__dict__)
```

```output for=with-dict
1 {'note': 'extra'}
```

`x` uses the slot, and `note` goes in `__dict__` (rule r2).

```python run id=dataclass
from dataclasses import dataclass

@dataclass(slots=True)
class Pixel:
    x: int
    y: int
    color: str = "black"

px = Pixel(3, 4)
print(px, Pixel.__slots__, hasattr(px, "__dict__"))
```

```output for=dataclass
Pixel(x=3, y=4, color='black') ('x', 'y', 'color') False
```

The default `color = 'black'` works here. In a plain slotted class the same line is rejected (rules r3 and r7).

```python run id=child
class Base:
    __slots__ = ("a",)

class Slotted(Base):
    __slots__ = ("b",)

class Plain(Base):
    pass

s, q = Slotted(), Plain()
s.a, s.b = 1, 2
q.a, q.c = 1, 3
print(hasattr(s, "__dict__"), q.__dict__)
```

```output for=child
False {'c': 3}
```

Plain does not set `__slots__`, so its instances get a `__dict__` again (rule r4).

## Pitfalls

A class attribute with the same name as a slot is rejected:

```python fails id=default expect=exception:ValueError
class Config:
    __slots__ = ("level",)
    level = 1
```

Set the default in `__init__`, or use `@dataclass(slots=True)` (rules r3 and r7).

Two bases that both have slot attributes cannot be combined:

```python fails id=two-bases expect=exception:TypeError
class A:
    __slots__ = ("a",)

class B:
    __slots__ = ("b",)

class C(A, B):
    pass
```

Give one of the bases an empty `__slots__ = ()` and keep its data in the other base, or in the child (rule r5).

A slotted class without `__weakref__` refuses weak references:

```python fails id=weakref expect=exception:TypeError
import weakref

class Node:
    __slots__ = ("value",)

weakref.ref(Node())
```

Add `'__weakref__'` to `__slots__`, or use `weakref_slot=True` with a slotted dataclass (rule r6).

A subclass without `__slots__` quietly loses the protection:

```python wrong id=lost
class Point:
    __slots__ = ("x", "y")

class Point3(Point):
    pass

p = Point3()
p.x, p.y = 1, 2
p.zz = 3
print(p.zz)
```

```output for=lost
3
```

The typo `zz` is accepted. Write `__slots__ = ('z',)` in Point3 (rule r4).

## Comparison

In Go, "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^3] For a selector `x.f`, "If there is not exactly one f with shallowest depth, the selector expression is illegal."[^3] So `p.zz` is an illegal Go expression when the struct has no field `zz`. A Python class with `__slots__` rejects the assignment when the statement runs, with an AttributeError (rule r2).
