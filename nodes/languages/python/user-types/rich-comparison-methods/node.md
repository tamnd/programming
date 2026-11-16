---
tag: 2VJSVH
kind: feature
owner: python
slug: rich-comparison-methods
name: Rich comparison methods
aliases: [__eq__, __lt__, __le__, __gt__, __ge__, __ne__, __hash__, NotImplemented, functools.total_ordering, compare objects]
area: languages/python/user-types
level: core
summary: >-
  The six methods __lt__, __le__, __eq__, __ne__, __gt__ and __ge__ implement the comparison operators. A class that defines __eq__ but not __hash__ becomes unhashable.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#object.__lt__", locator: "3.3.1. Basic customization, rich comparison methods and object.__hash__()"}
see_also: [python/special-methods, python/operator-overloading, python/classes]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__lt__
    title: The Python Language Reference, 3. Data model
    locator: 3.3.1. Basic customization, object.__lt__() to object.__ge__(); object.__hash__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/functools.html#functools.total_ordering
    title: functools, Higher-order functions and operations on callable objects
    locator: functools.total_ordering
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
from functools import total_ordering

@total_ordering
class Version:
    def __init__(self, major, minor):
        self.major, self.minor = major, minor

    def _key(self):
        return (self.major, self.minor)

    def __eq__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._key() == other._key()

    def __lt__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._key() < other._key()

    def __hash__(self):
        return hash(self._key())

    def __repr__(self):
        return f"Version({self.major}, {self.minor})"

vs = [Version(3, 12), Version(2, 7), Version(3, 9)]
print(sorted(vs))
print(Version(3, 9) <= Version(3, 12), Version(3, 9) == Version(3, 9), Version(1, 0) == "1.0")
print(len({Version(3, 9), Version(3, 9)}))
```

```output for=basics
[Version(2, 7), Version(3, 9), Version(3, 12)]
True True False
1
```

The class defines `__eq__` and `__lt__`, and total_ordering adds `<=`, `>` and `>=`. `__hash__` uses the same key as `__eq__`, so equal versions are one item in a set.

## Semantics

**r1.** "These are the so-called “rich comparison” methods. The correspondence between operator symbols and method names is as follows: x<y calls x.__lt__(y), x<=y calls x.__le__(y), x==y calls x.__eq__(y), x!=y calls x.__ne__(y), x>y calls x.__gt__(y), and x>=y calls x.__ge__(y)."[^1]

**r2.** "A rich comparison method may return the singleton NotImplemented if it does not implement the operation for a given pair of arguments."[^1] "When no appropriate method returns any value other than NotImplemented, the == and != operators will fall back to is and is not, respectively."[^1]

**r3.** "By default, the object class provides implementations consistent with Value comparisons: equality compares according to object identity, and order comparisons raise TypeError."[^1] "For __ne__(), by default it delegates to __eq__() and inverts the result unless it is NotImplemented. There are no other implied relationships among the comparison operators or default implementations".[^1] Examples: [`default`](#default), [`le-missing`](#le-missing).

**r4.** "There are no swapped-argument versions of these methods".[^1] Instead, "__lt__() and __gt__() are each other’s reflection, __le__() and __ge__() are each other’s reflection, and __eq__() and __ne__() are their own reflection."[^1] Example: [`reflected`](#reflected).

**r5.** "Given a class defining one or more rich comparison ordering methods, this class decorator supplies the rest."[^2] "The class must define one of __lt__(), __le__(), __gt__(), or __ge__(). In addition, the class should supply an __eq__() method."[^2]

**r6.** "The only required property is that objects which compare equal have the same hash value".[^1] "A class that overrides __eq__() and does not define __hash__() will have its __hash__() implicitly set to None."[^1] Then its instances "raise an appropriate TypeError when a program attempts to retrieve their hash value".[^1] Examples: [`hash-none`](#hash-none), [`unhashable`](#unhashable).

**r7.** "If a class defines mutable objects and implements an __eq__() method, it should not implement __hash__(), since the implementation of hashable collections requires that a key’s hash value is immutable".[^1]

## Examples

```python run id=default
class Plain:
    pass

a, b = Plain(), Plain()
print(a == a, a == b, a != b)
```

```output for=default
True False True
```

Without `__eq__`, two objects are equal only when they are the same object (rule r3).

```python run id=reflected
class Meters:
    def __init__(self, n):
        self.n = n

    def __lt__(self, other):
        print("Meters.__lt__")
        return self.n < other if isinstance(other, int) else NotImplemented

    def __gt__(self, other):
        print("Meters.__gt__")
        return self.n > other if isinstance(other, int) else NotImplemented

print(5 < Meters(3))
```

```output for=reflected
Meters.__gt__
False
```

int does not know Meters, so `5 < m` becomes `m > 5`, the reflection of `<` (rule r4).

```python run id=hash-none
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)

print(Point(1, 2) == Point(1, 2), Point.__hash__)
```

```output for=hash-none
True None
```

## Pitfalls

`__eq__` without `__hash__` makes the class unhashable, so its instances cannot go in a set or be dict keys:

```python fails id=unhashable expect=exception:TypeError
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)

{Point(1, 2)}
```

Add a `__hash__` that hashes the same fields as `__eq__`, if the objects do not change (rules r6 and r7).

`__lt__` and `__eq__` do not give `<=`:

```python fails id=le-missing expect=exception:TypeError
class Score:
    def __init__(self, n):
        self.n = n

    def __lt__(self, other):
        return self.n < other.n

    def __eq__(self, other):
        return self.n == other.n

Score(1) <= Score(2)
```

Define `__le__` too, or use total_ordering (rules r3 and r5).

## Comparison

Go compares structs field by field, with no methods: "Struct types are comparable if all their field types are comparable. Two struct values are equal if their corresponding non-blank field values are equal."[^3] "The ordering operators <, <=, >, and >= apply to operands of ordered types."[^3] A struct is not in the list of ordered types. In Python a class chooses what `==` and `<` mean (rule r1).
