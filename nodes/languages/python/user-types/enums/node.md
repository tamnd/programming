---
tag: N9CWTZ
kind: feature
owner: python
slug: enums
name: Enumerations
aliases: [enum, Enum, IntEnum, StrEnum, Flag, auto, enumeration, enum member]
area: languages/python/user-types
level: core
summary: >-
  An enumeration is a class that derives from enum.Enum. Its class attributes become members, which are constants with a name and a value. Members are singletons, compared by identity.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/enum.html", locator: "enum, Support for enumerations"}
see_also: [python/classes, python/dataclasses, python/named-tuples, python/rich-comparison-methods]
sources:
  - url: https://docs.python.org/3.15/library/enum.html#enum.Enum
    title: enum, Support for enumerations
    locator: class enum.Enum; class enum.IntEnum; class enum.Flag; class enum.auto; @enum.unique
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/enum.html#comparisons
    title: Enum HOWTO
    locator: Duplicating enum members and values; Comparisons; Restricted Enum subclassing; Planet
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Iota
    title: The Go Programming Language Specification
    locator: Iota
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
from enum import Enum, auto

class Color(Enum):
    RED = auto()
    GREEN = auto()
    BLUE = auto()

c = Color.GREEN
print(c, c.name, c.value)
print(Color(3), Color["RED"], list(Color))
print(c is Color.GREEN, c == 2)
```

```output for=basics
Color.GREEN GREEN 2
Color.BLUE Color.RED [<Color.RED: 1>, <Color.GREEN: 2>, <Color.BLUE: 3>]
True False
```

`Color(3)` finds a member by value and `Color['RED']` by name. A member is not equal to its plain value.

## Semantics

**r1.** An enumeration "is a set of symbolic names (members) bound to unique values", "can be iterated over to return its canonical (i.e. non-alias) members in definition order", "uses call syntax to return members by value" and "uses index syntax to return members by name".[^1]

**r2.** "Even though we can use class syntax to create Enums, Enums are not normal Python classes."[^1] The members "are functionally constants."[^1]

**r3.** "Enumeration members are compared by identity".[^2] "Ordered comparisons between enumeration values are not supported."[^2] "Equality comparisons against non-enumeration values will always return False".[^2] Example: [`order`](#order).

**r4.** "Given two entries A and B with the same value (and A defined first), B is an alias for the member A."[^2] The `@unique` decorator searches "an enumeration’s __members__, gathering any aliases it finds; if any are found ValueError is raised".[^1] Example: [`alias`](#alias).

**r5.** For auto, "For Enum and IntEnum that appropriate value will be the highest value seen plus one; for Flag and IntFlag it will be the first power-of-two greater than the highest value seen; for StrEnum it will be the lower-cased version of the member’s name."[^1]

**r6.** IntEnum "is the same as Enum, but its members are also integers and can be used anywhere that an integer can be used."[^1] Flag members "support the bitwise operators & (AND), | (OR), ^ (XOR), and ~ (INVERT)".[^1] Example: [`flag`](#flag).

**r7.** "subclassing an enumeration is allowed only if the enumeration does not define any members."[^2] Example: [`extend`](#extend).

## Examples

```python run id=methods
from enum import Enum

class Planet(Enum):
    MERCURY = (3.303e23, 2.4397e6)
    EARTH = (5.976e24, 6.37814e6)

    def __init__(self, mass, radius):
        self.mass = mass
        self.radius = radius

    def surface_gravity(self):
        return 6.673e-11 * self.mass / self.radius ** 2

print(round(Planet.EARTH.surface_gravity(), 2), Planet.EARTH.value[1])
```

```output for=methods
9.8 6378140.0
```

An enum can have methods. "If __new__() or __init__() is defined, the value of the enum member will be passed to those methods".[^2] Here the tuple fills `mass` and `radius`.

```python run id=flag
from enum import Flag, IntEnum, auto

class Perm(Flag):
    R = auto()
    W = auto()
    X = auto()

rw = Perm.R | Perm.W
print(Perm.W in rw, Perm.X in rw, rw.value, [p.name for p in rw])

class Level(IntEnum):
    LOW = 1
    HIGH = 3

print(Level.HIGH > Level.LOW, Level.HIGH + 1, Level.LOW == 1)
```

```output for=flag
True False 3 ['R', 'W']
True 4 True
```

Flag values are powers of two, so `|` combines them. IntEnum members act as ints (rules r5 and r6).

```python run id=alias
from enum import Enum

class Status(Enum):
    ACTIVE = 1
    ENABLED = 1
    CLOSED = 2

print(Status.ENABLED, Status(1), [s.name for s in Status])
print(list(Status.__members__))
```

```output for=alias
Status.ACTIVE Status.ACTIVE ['ACTIVE', 'CLOSED']
['ACTIVE', 'ENABLED', 'CLOSED']
```

ENABLED is an alias for ACTIVE. Iteration skips it, and `__members__` keeps it (rule r4). Put `@unique` on the class to refuse aliases.

## Pitfalls

Plain Enum members have no order:

```python fails id=order expect=exception:TypeError
from enum import Enum

class Size(Enum):
    S = 1
    M = 2

Size.S < Size.M
```

Compare `Size.S.value < Size.M.value`, or use IntEnum when the members are numbers (rules r3 and r6).

Comparing a member with its raw value is always False:

```python wrong id=raw-value
from enum import Enum

class Mode(Enum):
    READ = "r"

mode = "r"
print(mode == Mode.READ)
```

```output for=raw-value
False
```

Convert first with `Mode(mode)`, or use StrEnum (rule r3).

An enum with members cannot be extended:

```python fails id=extend expect=exception:TypeError
from enum import Enum

class Color(Enum):
    RED = 1

class MoreColor(Color):
    PINK = 2
```

Put shared methods in a base enum with no members (rule r7).

## Comparison

In Go, "the predeclared identifier iota represents successive untyped integer constants."[^3] "It can be used to construct a set of related constants".[^3] The spec shows `a = 1 << iota` for bit flags, like Python's Flag.[^3] The Go constants are untyped integers. A Python member is an object of its enum class, and it is not equal to its plain value (rule r3).
