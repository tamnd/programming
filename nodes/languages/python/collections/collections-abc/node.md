---
tag: Q37152
kind: feature
owner: python
slug: collections-abc
name: collections.abc
aliases: [abstract base classes for containers, Sequence ABC, Mapping ABC, MutableMapping, Iterable ABC, Sized, Container, mixin methods, virtual subclass]
area: languages/python/collections
level: advanced
summary: >-
  collections.abc has abstract base classes such as Iterable, Sized, Sequence, Mapping and Set. They test if an object has a container interface, and give mixin methods to classes that inherit them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/collections.abc.html", locator: "collections.abc: Abstract Base Classes for Containers"}
see_also: [python/emulating-container-types, python/the-collections-module, python/sequence-types, python/dictionaries, python/sets]
sources:
  - url: https://docs.python.org/3.15/library/collections.abc.html
    title: "collections.abc: Abstract Base Classes for Containers, Python 3.15"
    locator: Introduction; Detailed Descriptions; Examples and Recipes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#emulating-container-types
    title: The Python Language Reference, 3. Data model
    locator: 3.3.7. Emulating container types
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Interface_types
    title: The Go Programming Language Specification
    locator: Interface types
    accessed: 2026-11-16
---

## Synopsis

```python run id=check
from collections.abc import Iterable, Sized, Sequence, Mapping, MutableSequence, Hashable

for value in ([1], (1,), "a", {"k": 1}, {1}, range(2)):
    print(type(value).__name__,
          isinstance(value, Sequence),
          isinstance(value, MutableSequence),
          isinstance(value, Mapping),
          isinstance(value, Hashable))
```

```output for=check
list True True False False
tuple True False False True
str True False False True
dict False False True False
set False False False False
range True False False True
```

The ABCs answer questions such as "is this a sequence" or "can it be hashed" without a test of the exact type.

## Semantics

**r1.** "This module provides abstract base classes that can be used to test whether a class provides a particular interface; for example, whether it is hashable or whether it is a mapping."[^1]

**r2.** "A newly written class can inherit directly from one of the abstract base classes. The class must supply the required abstract methods. The remaining mixin methods come from inheritance and can be overridden if desired."[^1] Examples: [`sequence-mixin`](#sequence-mixin), [`missing-abstract`](#missing-abstract).

**r3.** "Existing classes and built-in classes can be registered as “virtual subclasses” of the ABCs. Those classes should define the full API including all of the abstract methods and all of the mixin methods."[^1]

**r4.** "Some simple interfaces are directly recognizable by the presence of the required methods (unless those methods have been set to None)".[^1] "Complex interfaces do not support this last technique because an interface is more than just the presence of method names."[^1] Example: [`structural`](#structural).

**r5.** "Checking isinstance(obj, Iterable) detects classes that are registered as Iterable or that have an __iter__() method, but it does not detect classes that iterate with the __getitem__() method. The only reliable way to determine whether an object is iterable is to call iter(obj)."[^1] Example: [`iterable-check`](#iterable-check).

**r6.** "to write a class supporting the full Set API, it is only necessary to supply the three underlying abstract methods: __contains__(), __iter__(), and __len__(). The ABC supplies the remaining methods such as __and__() and isdisjoint()".[^1] Example: [`set-mixin`](#set-mixin).

**r7.** The data model reference says that MutableMapping helps "create those methods from a base set of __getitem__(), __setitem__(), __delitem__(), and keys()."[^2]

## Examples

```python run id=sequence-mixin
from collections.abc import Sequence

class Squares(Sequence):
    def __init__(self, n):
        self.n = n

    def __len__(self):
        return self.n

    def __getitem__(self, i):
        if not 0 <= i < self.n:
            raise IndexError(i)
        return i * i

sq = Squares(5)
print(list(sq), 9 in sq, sq.index(16), sq.count(4))
print(list(reversed(sq)))
```

```output for=sequence-mixin
[0, 1, 4, 9, 16] True 4 1
[16, 9, 4, 1, 0]
```

The class writes two methods. `Sequence` adds `__contains__`, `__iter__`, `__reversed__`, `index` and `count` (rule r2).

```python run id=set-mixin
from collections.abc import Set

class ListBasedSet(Set):
    def __init__(self, iterable):
        self.elements = lst = []
        for value in iterable:
            if value not in lst:
                lst.append(value)

    def __iter__(self):
        return iter(self.elements)

    def __contains__(self, value):
        return value in self.elements

    def __len__(self):
        return len(self.elements)

s1 = ListBasedSet("abcdef")
s2 = ListBasedSet("defghi")
print(list(s1 & s2), s1.isdisjoint("xyz"), s1 <= ListBasedSet("abcdefg"))
```

```output for=set-mixin
['d', 'e', 'f'] True True
```

This recipe is from the module documentation. `&`, `isdisjoint` and `<=` come from the mixin (rule r6).

```python run id=structural
from collections.abc import Iterable, Sized, Sequence

class Numbers:
    def __iter__(self):
        return iter([1, 2])

    def __len__(self):
        return 2

n = Numbers()
print(isinstance(n, Iterable), isinstance(n, Sized), isinstance(n, Sequence))
```

```output for=structural
True True False
```

`Iterable` and `Sized` check for one method. `Sequence` needs inheritance or registration (rule r4).

## Pitfalls

`isinstance(x, Iterable)` can be false for an object that `for` accepts:

```python wrong id=iterable-check
from collections.abc import Iterable

class OldStyle:
    def __getitem__(self, i):
        if i >= 3:
            raise IndexError
        return i

obj = OldStyle()
print(isinstance(obj, Iterable), list(iter(obj)))
```

```output for=iterable-check
False [0, 1, 2]
```

Call `iter(obj)` in a `try` statement to test if an object is iterable (rule r5).

A subclass that does not define the abstract methods cannot be instantiated:

```python fails id=missing-abstract expect=exception:TypeError
from collections.abc import Mapping

class Empty(Mapping):
    def __getitem__(self, key):
        raise KeyError(key)

Empty()
```

`Mapping` also needs `__iter__` and `__len__` (rule r2).

## Comparison

Go interfaces are satisfied without a declaration: "A variable of interface type can store a value of any type that is in the type set of the interface. Such a type is said to implement the interface."[^3] Python does this only for the simple ABCs such as `Iterable` and `Sized` (rule r4). The other ABCs need inheritance or `register`, and in exchange they supply mixin methods, which Go interfaces do not.
