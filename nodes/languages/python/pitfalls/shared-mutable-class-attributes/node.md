---
tag: J50A89
kind: feature
owner: python
slug: shared-mutable-class-attributes
name: Shared mutable class attributes
aliases: [mutable class attribute, class variable list, shared class list, RUF012, mutable-class-default, ClassVar]
area: languages/python/pitfalls
level: core
summary: >-
  A list or dict assigned in the class body is one object that all instances share. A change through one instance shows in all of them. Make the object in __init__, or use a dataclass field factory.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/classes.html#class-and-instance-variables", locator: "9.3.5 Class and Instance Variables"}
proposals: []
see_also: [python/class-and-instance-variables, python/dataclasses, python/instance-objects, python/mutable-default-argument, python/attribute-access-customization, python/slots]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#class-and-instance-variables
    title: The Python Tutorial, Python 3.15 documentation
    locator: 9.3.5 Class and Instance Variables
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/dataclasses.html#mutable-default-values
    title: dataclasses, Data Classes, Python 3.15 documentation
    locator: Mutable default values; dataclasses.field()
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/mutable-class-default/
    title: mutable-class-default (RUF012), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
---

## Synopsis

```python run id=tricks
class Dog:
    tricks = []

    def __init__(self, name):
        self.name = name

    def add_trick(self, trick):
        self.tricks.append(trick)


d = Dog("Fido")
e = Dog("Buddy")
d.add_trick("roll over")
e.add_trick("play dead")
print(d.tricks)
print(d.tricks is Dog.tricks)
```

```output for=tricks
['roll over', 'play dead']
True
```

Buddy's trick shows in the list of Fido, because both dogs use the one list on the class.

## Semantics

**r1.** "Generally speaking, instance variables are for data unique to each instance and class variables are for attributes and methods shared by all instances of the class".[^1]

**r2.** A mutable class variable is shared: "shared data can have possibly surprising effects involving mutable objects such as lists and dictionaries. For example, the tricks list in the following code should not be used as a class variable because just a single list would be shared by all Dog instances".[^1] Example: [`tricks`](#tricks).

**r3.** The lookup `self.tricks` finds the class attribute when the instance has no attribute of that name. `self.tricks.append(x)` changes the object that it finds. An assignment `self.tricks = [...]` is different: it makes a new attribute on the instance, which hides the class attribute. Example: [`assign-hides`](#assign-hides).

**r4.** The fix: "Correct design of the class should use an instance variable instead".[^1] Make the list in `__init__`, so that each instance gets its own. Example: [`fixed`](#fixed).

**r5.** Data classes reject a mutable default, because "Python stores default member variable values in class attributes."[^2] For a field that needs a new list for each instance, use `field(default_factory=list)`.[^2] Example: [`dataclass-factory`](#dataclass-factory).

**r6.** Ruff rule RUF012 "Checks for mutable default values in class attributes."[^3] Its reason: "Mutable default values share state across all instances of the class, while not being obvious."[^3] When sharing is the intent, it suggests "Explicitly annotating the variable with typing.ClassVar to indicate that it is intended to be shared across all instances."[^3]

## Examples

```python run id=fixed
class Dog:
    def __init__(self, name):
        self.name = name
        self.tricks = []

    def add_trick(self, trick):
        self.tricks.append(trick)


d = Dog("Fido")
e = Dog("Buddy")
d.add_trick("roll over")
e.add_trick("play dead")
print(d.tricks, e.tricks)
```

```output for=fixed
['roll over'] ['play dead']
```

Each call of `__init__` makes a new list (rule r4).

```python run id=dataclass-factory
from dataclasses import dataclass, field
from typing import ClassVar


@dataclass
class Cart:
    owner: str
    items: list[str] = field(default_factory=list)
    registry: ClassVar[list[str]] = []

    def __post_init__(self):
        Cart.registry.append(self.owner)


a = Cart("ann")
b = Cart("bob")
a.items.append("tea")
print(a.items, b.items, Cart.registry)
```

```output for=dataclass-factory
['tea'] [] ['ann', 'bob']
```

`items` is a new list for each cart (rule r5). `registry` is shared on purpose, and `ClassVar` says so, so the data class does not make it a field (rule r6).

## Versions

> [!since v="3.11"]
> A data class rejects any default that is not hashable, not only a `list`, `dict` or `set`: "unhashable objects are now not allowed as default values."[^2]

## Pitfalls

```python wrong id=assign-hides
class Counter:
    hits = []
    total = 0

    def hit(self, page):
        self.hits.append(page)
        self.total += 1


a = Counter()
b = Counter()
a.hit("home")
b.hit("about")
print(a.hits, a.total, b.total, Counter.total)
```

```output for=assign-hides
['home', 'about'] 1 1 0
```

The two attributes act in different ways (rule r3). `append` changed the shared list. `self.total += 1` read 0 from the class and then assigned 1 to a new instance attribute, so the class value stays 0. Both results are often wrong.

```python fails id=dataclass-list expect=exception:ValueError
from dataclasses import dataclass


@dataclass
class Cart:
    items: list = []
```

`@dataclass` sees a list as the default and raises `ValueError` before the class can share it (rule r5).

## Comparison

In many languages, a field declared in the class body is a per-instance field, and a shared field needs a keyword such as `static`. In Python, a name assigned in the class body is always a class attribute, and per-instance data is assigned through `self`. For the full rules, see [[python/class-and-instance-variables]]. The same trap for function defaults is in [[python/mutable-default-argument]].
