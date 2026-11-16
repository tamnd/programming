---
tag: B54G2A
kind: feature
owner: python
slug: hashability
name: Hashability
aliases: [hashable, unhashable type, __hash__, hash(), dict key requirements, set member requirements, __hash__ = None, hash and eq]
area: languages/python/collections
level: core
summary: >-
  An object is hashable if its hash value never changes and it can be compared with ==. Only hashable objects can be dict keys or set members. Objects that compare equal must have the same hash.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#object.__hash__", locator: "3.3.1. Basic customization, object.__hash__"}
see_also: [python/dictionaries, python/sets, python/frozendict, python/tuples-and-sequences, python/emulating-container-types]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-hashable
    title: Glossary, Python 3.15 documentation
    locator: hashable
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__hash__
    title: The Python Language Reference, 3. Data model
    locator: object.__hash__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#hash
    title: Built-in Functions, Python 3.15 documentation
    locator: hash()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=which
for value in (42, "text", (1, 2), frozenset({3}), [1, 2], {"a": 1}, {3}):
    try:
        hash(value)
        print(type(value).__name__, "hashable")
    except TypeError:
        print(type(value).__name__, "unhashable")
```

```output for=which
int hashable
str hashable
tuple hashable
frozenset hashable
list unhashable
dict unhashable
set unhashable
```

Immutable built-in values are hashable. Mutable containers are not.

## Semantics

**r1.** "An object is hashable if it has a hash value which never changes during its lifetime (it needs a __hash__() method), and can be compared to other objects (it needs an __eq__() method). Hashable objects which compare equal must have the same hash value."[^1]

**r2.** "Hashability makes an object usable as a dictionary key and a set member, because these data structures use the hash value internally."[^1]

**r3.** "Most of Python’s immutable built-in objects are hashable; mutable containers (such as lists or dictionaries) are not; immutable containers (such as tuples and frozensets) are only hashable if their elements are hashable."[^1] Example: [`tuple-with-list`](#tuple-with-list).

**r4.** "Objects which are instances of user-defined classes are hashable by default. They all compare unequal (except with themselves), and their hash value is derived from their id()."[^1] Example: [`default-identity`](#default-identity).

**r5.** "A class that overrides __eq__() and does not define __hash__() will have its __hash__() implicitly set to None."[^2] Then the instances "will raise an appropriate TypeError when a program attempts to retrieve their hash value".[^2] Example: [`eq-only`](#eq-only).

**r6.** "The only required property is that objects which compare equal have the same hash value; it is advised to mix together the hash values of the components of the object that also play a part in comparison of objects by packing them into a tuple and hashing the tuple."[^2] Example: [`custom`](#custom).

**r7.** "If a class defines mutable objects and implements an __eq__() method, it should not implement __hash__(), since the implementation of hashable collections requires that a key’s hash value is immutable (if the object’s hash value changes, it will be in the wrong hash bucket)."[^2] Example: [`mutated-key`](#mutated-key).

**r8.** "Numeric values that compare equal have the same hash value (even if they are of different types, as is the case for 1 and 1.0)."[^3] "By default, the __hash__() values of str and bytes objects are “salted” with an unpredictable random value."[^2] So a program must not depend on the hash value of a string from one run to the next.

## Examples

```python run id=custom
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)

    def __hash__(self):
        return hash((self.x, self.y))

seen = {Point(1, 2)}
print(Point(1, 2) in seen, Point(2, 1) in seen)
print(hash(1) == hash(1.0) == hash(True))
```

```output for=custom
True False
True
```

`__hash__` hashes the same fields that `__eq__` compares (rule r6).

```python run id=default-identity
class Tag:
    def __init__(self, name):
        self.name = name

a, b = Tag("x"), Tag("x")
print(a == b, len({a, b}), a in {a})
```

```output for=default-identity
False 2 True
```

Without `__eq__`, two objects with the same data are different keys (rule r4).

## Pitfalls

A tuple that holds a list is not hashable:

```python fails id=tuple-with-list expect=exception:TypeError
key = (1, [2, 3])
cache = {key: "value"}
```

A class with `__eq__` and no `__hash__` cannot be used in a set:

```python fails id=eq-only expect=exception:TypeError
class Money:
    def __init__(self, cents):
        self.cents = cents

    def __eq__(self, other):
        return self.cents == other.cents

prices = {Money(100)}
```

Add a `__hash__` that hashes `self.cents`, but only if the objects do not change (rules r5 and r7).

A key that changes after it is stored cannot be found:

```python wrong id=mutated-key
class Box:
    def __init__(self, v):
        self.v = v

    def __eq__(self, other):
        return self.v == other.v

    def __hash__(self):
        return hash(self.v)

box = Box(1)
table = {box: "first"}
box.v = 2
print(Box(2) in table, Box(1) in table, len(table))
```

```output for=mutated-key
False False 1
```

The dict stored the key under the old hash. The lookup for `Box(2)` uses the new hash, and the lookup for `Box(1)` finds an object that is no longer equal (rule r7).

## Comparison

Go uses comparability, not hashing, for map keys. The Go spec says: "Slice, map, and function types are not comparable."[^4] and "Struct types are comparable if all their field types are comparable."[^4] A Go struct of comparable fields is a valid map key without extra code. A Python class needs `__eq__` and `__hash__` that agree (rule r6) to be a key by value.
