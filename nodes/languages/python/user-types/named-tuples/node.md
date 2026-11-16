---
tag: F2BB5A
kind: feature
owner: python
slug: named-tuples
name: Named tuples
aliases: [namedtuple, NamedTuple, named tuple, typing.NamedTuple, _replace, _asdict, record tuple]
area: languages/python/user-types
level: core
summary: >-
  A named tuple is a tuple subclass whose items also have names. collections.namedtuple and typing.NamedTuple make one. The items are read by index or by attribute, and they cannot be changed.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/collections.html#collections.namedtuple", locator: "collections.namedtuple()"}
see_also: [python/dataclasses, python/slots, python/enums, python/classes]
sources:
  - url: https://docs.python.org/3.15/library/collections.html#collections.namedtuple
    title: collections, Container datatypes
    locator: "namedtuple() Factory Function for Tuples with Named Fields"
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/typing.html#typing.NamedTuple
    title: typing, Support for type hints
    locator: class typing.NamedTuple
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-named-tuple
    title: Glossary
    locator: named tuple
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types; Comparison operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
from collections import namedtuple
from typing import NamedTuple

Point = namedtuple("Point", ["x", "y"])

class Employee(NamedTuple):
    name: str
    dept: str = "staff"

p = Point(11, y=22)
x, y = p
print(p, p[0] + p.y, x)
e = Employee("Ann")
print(e, e._replace(dept="ops"), e._asdict())
```

```output for=basics
Point(x=11, y=22) 33 11
Employee(name='Ann', dept='staff') Employee(name='Ann', dept='ops') {'name': 'Ann', 'dept': 'staff'}
```

Both forms make a tuple subclass. The class form with annotations also gives defaults in the class body.

## Semantics

**r1.** "The term “named tuple” applies to any type or class that inherits from tuple and whose indexable elements are also accessible using named attributes."[^3] Built-in examples include "the values returned by time.localtime() and os.stat()".[^3]

**r2.** `namedtuple` "Returns a new tuple subclass named typename. The new subclass is used to create tuple-like objects that have fields accessible by attribute lookup as well as being indexable and iterable."[^1] Example: [`tuple-equal`](#tuple-equal).

**r3.** "Any valid Python identifier may be used for a fieldname except for names starting with an underscore."[^1] "If rename is true, invalid fieldnames are automatically replaced with positional names."[^1] Example: [`underscore`](#underscore).

**r4.** "Named tuple instances do not have per-instance dictionaries, so they are lightweight and require no more memory than regular tuples."[^1]

**r5.** "named tuples support three additional methods and two attributes. To prevent conflicts with field names, the method and attribute names start with an underscore."[^1] `_replace` will "Return a new instance of the named tuple replacing specified fields with new values".[^1] Example: [`fields`](#fields).

**r6.** typing.NamedTuple is the "Typed version of collections.namedtuple()."[^2] "Fields with a default value must come after any fields without a default."[^2] "NamedTuple subclasses can also have docstrings and methods".[^2] Example: [`methods`](#methods).

## Examples

```python run id=fields
from collections import namedtuple

Account = namedtuple("Account", "type balance", defaults=[0])
a = Account._make(["premium", 50])
print(Account._fields, Account._field_defaults, Account("basic"))
print(a, getattr(a, "balance"))
```

```output for=fields
('type', 'balance') {'balance': 0} Account(type='basic', balance=0)
Account(type='premium', balance=50) 50
```

`_make` builds an instance from an iterable, and `defaults` apply to the rightmost fields (rule r5).

```python run id=methods
from typing import NamedTuple

class Size(NamedTuple):
    w: int
    h: int

    def area(self):
        return self.w * self.h

s = Size(3, 4)
print(s.area(), max([Size(1, 9), Size(2, 1)]), isinstance(s, tuple))
```

```output for=methods
12 Size(w=2, h=1) True
```

The method works on the fields. `max` compares the sizes as tuples, item by item (rule r6).

```python run id=as-key
from collections import namedtuple

Cell = namedtuple("Cell", "row col")
grid = {Cell(0, 0): "start", Cell(2, 3): "goal"}
print(grid[Cell(2, 3)], grid[(0, 0)])
```

```output for=as-key
goal start
```

A named tuple of hashable items is hashable, so it can be a dict key. The plain tuple `(0, 0)` finds the same entry.

## Pitfalls

A named tuple cannot be changed:

```python fails id=frozen expect=exception:AttributeError
from collections import namedtuple

Point = namedtuple("Point", "x y")
p = Point(1, 2)
p.x = 5
```

Use `p = p._replace(x=5)`, or a dataclass when the fields must change (rule r5).

A named tuple is equal to a plain tuple, and to other named tuples, with the same items:

```python wrong id=tuple-equal
from collections import namedtuple

Point = namedtuple("Point", "x y")
Size = namedtuple("Size", "w h")
print(Point(3, 4) == Size(3, 4), Point(3, 4) == (3, 4))
```

```output for=tuple-equal
True True
```

The comparison ignores the names. Use a dataclass when two record types must not compare equal (rule r2).

A field name that starts with an underscore is rejected:

```python fails id=underscore expect=exception:ValueError
from collections import namedtuple

Row = namedtuple("Row", ["id", "_hidden"])
```

Rename the field, or pass `rename=True` to replace it with a positional name (rule r3).

## Comparison

In Go, "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^4] A Go struct gives the names, like a named tuple. For equality, "Two struct values are equal if their corresponding non-blank field values are equal."[^4] In any Go comparison, "the first operand must be assignable to the type of the second operand, or vice versa."[^4] A Python named tuple has no such rule: it is equal to a plain tuple, and to a different named tuple, with the same items (rule r2).
