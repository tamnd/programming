---
tag: 15619D
kind: feature
owner: python
slug: dataclasses
name: Data classes
aliases: [dataclass, dataclass decorator, data class, field, default_factory, __post_init__, frozen dataclass, record type]
area: languages/python/user-types
level: core
summary: >-
  The @dataclass decorator reads the annotated class variables of a class as fields. It then adds __init__, __repr__ and __eq__ for those fields, and on request ordering, hashing and frozen instances.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/dataclasses.html", locator: "dataclasses, Data Classes"}
see_also: [python/slots, python/named-tuples, python/rich-comparison-methods, python/class-and-instance-variables]
sources:
  - url: https://docs.python.org/3.15/library/dataclasses.html#dataclasses.dataclass
    title: dataclasses, Data Classes
    locator: "@dataclasses.dataclass; dataclasses.field; Post-init processing; Frozen instances; Inheritance; Mutable default values"
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#odds-and-ends
    title: The Python Tutorial, 9. Classes
    locator: 9.7. Odds and Ends
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Struct types; Comparison operators
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#composite_literals
    title: Effective Go
    locator: Constructors and composite literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
from dataclasses import dataclass, field

@dataclass
class Item:
    name: str
    price: float
    tags: list[str] = field(default_factory=list)
    quantity: int = 1

    def total(self):
        return self.price * self.quantity

a = Item("pen", 1.5, quantity=4)
b = Item("pen", 1.5, [], 4)
print(a)
print(a == b, a.total())
```

```output for=basics
Item(name='pen', price=1.5, tags=[], quantity=4)
True 6.0
```

The four annotated lines are the fields. The decorator writes `__init__`, `__repr__` and `__eq__` from them, and ordinary methods such as `total` stay as they are.

## Semantics

**r1.** "A field is defined as a class variable that has a type annotation."[^1] "The order of the fields in all of the generated methods is the order in which they appear in the class definition."[^1] Example: [`no-annotation`](#no-annotation).

**r2.** The tutorial says that for "a data type similar to the Pascal “record” or C “struct”", "The idiomatic approach is to use dataclasses for this purpose".[^2]

**r3.** By default `__init__`, `__repr__` and `__eq__` are generated. The generated `__eq__` "compares the class by comparing each field in order. Both instances in the comparison must be of the identical type."[^1] With `order=True`, the order methods "compare the class as if it were a tuple of its fields, in order."[^1] Example: [`order`](#order).

**r4.** "TypeError will be raised if a field without a default value follows a field with a default value."[^1]

**r5.** "the @dataclass decorator will raise a ValueError if it detects an unhashable default parameter."[^1] `default_factory` "must be a zero-argument callable that will be called when a default value is needed for this field."[^1] Example: [`mutable-default`](#mutable-default).

**r6.** `__post_init__` "will be called by the generated __init__(), normally as self.__post_init__()."[^1] "this allows for initializing field values that depend on one or more other fields."[^1] Example: [`post-init`](#post-init).

**r7.** With `frozen=True`, "assigning to fields will generate an exception. This emulates read-only frozen instances."[^1] FrozenInstanceError "is a subclass of AttributeError."[^1] Example: [`frozen`](#frozen).

**r8.** "If eq and frozen are both true, by default @dataclass will generate a __hash__() method for you. If eq is true and frozen is false, __hash__() will be set to None, marking it unhashable (which it is, since it is mutable)."[^1] Example: [`hash`](#hash).

## Examples

```python run id=order
from dataclasses import dataclass

@dataclass(order=True)
class Version:
    major: int
    minor: int

vs = [Version(1, 10), Version(1, 2), Version(0, 9)]
print(sorted(vs)[0], max(vs))
```

```output for=order
Version(major=0, minor=9) Version(major=1, minor=10)
```

The versions compare like the tuples `(major, minor)` (rule r3).

```python run id=post-init
from dataclasses import dataclass, field

@dataclass
class Rect:
    w: float
    h: float
    area: float = field(init=False)

    def __post_init__(self):
        self.area = self.w * self.h

print(Rect(2, 3))
```

```output for=post-init
Rect(w=2, h=3, area=6)
```

`area` is a field but not an `__init__` parameter. `__post_init__` computes it (rule r6).

```python run id=frozen
from dataclasses import dataclass, replace, FrozenInstanceError

@dataclass(frozen=True)
class Point:
    x: int
    y: int

p = Point(1, 2)
try:
    p.x = 5
except FrozenInstanceError:
    print("frozen")
q = replace(p, x=5)
print(q, {p, q} == {Point(1, 2), Point(5, 2)})
```

```output for=frozen
frozen
Point(x=5, y=2) True
```

`replace` makes a new object with some fields changed. A frozen dataclass with `eq` gets a `__hash__`, so the points go in a set (rules r7 and r8).

## Pitfalls

A list as a default value is rejected:

```python fails id=mutable-default expect=exception:ValueError
from dataclasses import dataclass

@dataclass
class Cart:
    items: list = []
```

Write `items: list = field(default_factory=list)`, so each Cart gets a new list (rule r5).

A class variable without an annotation is not a field:

```python wrong id=no-annotation
from dataclasses import dataclass

@dataclass
class Config:
    host: str
    port = 8080

c = Config("example.org")
print(c)
```

```output for=no-annotation
Config(host='example.org')
```

`port` is an ordinary class attribute. It is not in `__init__`, `__repr__` or `__eq__`. Write `port: int = 8080` (rule r1).

A mutable dataclass is unhashable:

```python fails id=hash expect=exception:TypeError
from dataclasses import dataclass

@dataclass
class Tag:
    name: str

{Tag("a")}
```

Use `frozen=True` when the instances must go in a set or be dict keys (rule r8).

## Comparison

In Go, "Two struct values are equal if their corresponding non-blank field values are equal."[^3] So `==` on structs works like the generated `__eq__` (rule r3). "Struct types are comparable if all their field types are comparable."[^3] Effective Go says: "Sometimes the zero value isn't good enough and an initializing constructor is necessary".[^4] Such a function, like `NewFile` there, does the work that `__init__` and `__post_init__` do in a dataclass (rule r6).
