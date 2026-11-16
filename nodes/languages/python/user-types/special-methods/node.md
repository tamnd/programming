---
tag: GP7W2K
kind: feature
owner: python
slug: special-methods
name: Special methods
aliases: [dunder methods, magic methods, special method names, __repr__, __len__, __getitem__, __call__, __bool__, data model protocols]
area: languages/python/user-types
level: core
summary: >-
  A class takes part in len(), indexing, iteration, calls and other syntax when it defines methods with special names such as __len__. Python looks up these methods on the type, not on the instance.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#special-method-names", locator: "3.3. Special method names"}
see_also: [python/operator-overloading, python/rich-comparison-methods, python/object-creation, python/attribute-access-customization, python/repr-and-str]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#special-method-names
    title: The Python Language Reference, 3. Data model
    locator: 3.3. Special method names; 3.3.1; 3.3.6; 3.3.7; 3.3.13. Special method lookup
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#printing
    title: Effective Go
    locator: Printing
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#overloading
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not support overloading of methods and operators?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Playlist:
    def __init__(self, *songs):
        self.songs = list(songs)

    def __repr__(self):
        return f"Playlist({', '.join(map(repr, self.songs))})"

    def __len__(self):
        return len(self.songs)

    def __getitem__(self, index):
        return self.songs[index]

    def __iter__(self):
        return iter(self.songs)

    def __contains__(self, song):
        return song in self.songs

p = Playlist("Intro", "Theme", "Outro")
print(p, len(p), p[1], "Theme" in p, bool(Playlist()))
print([s.upper() for s in p], p[::2])
```

```output for=basics
Playlist('Intro', 'Theme', 'Outro') 3 Theme True False
['INTRO', 'THEME', 'OUTRO'] ['Intro', 'Outro']
```

Each built-in operation calls one special method: repr() calls `__repr__`, len() calls `__len__`, `p[1]` calls `__getitem__`, a for loop calls `__iter__`, and `in` calls `__contains__`.

## Semantics

**r1.** "A class can implement certain operations that are invoked by special syntax (such as arithmetic operations or subscripting and slicing) by defining methods with special names. This is Python’s approach to operator overloading, allowing classes to define their own behavior with respect to language operators."[^1]

**r2.** "For instance, if a class defines a method named __getitem__(), and x is an instance of this class, then x[i] is roughly equivalent to type(x).__getitem__(x, i)."[^1] "Except where mentioned, attempts to execute an operation raise an exception when no appropriate method is defined (typically AttributeError or TypeError)."[^1]

**r3.** "For custom classes, implicit invocations of special methods are only guaranteed to work correctly if defined on an object’s type, not in the object’s instance dictionary."[^1] Example: [`instance-len`](#instance-len).

**r4.** "Setting a special method to None indicates that the corresponding operation is not available."[^1] Example: [`none`](#none).

**r5.** `__repr__` gives the "official" representation: "If at all possible, this should look like a valid Python expression that could be used to recreate an object with the same value".[^1] "If a class defines __repr__() but not __str__(), then __repr__() is also used when an “informal” string representation of instances of that class is required."[^1]

**r6.** `__bool__` is "Called to implement truth value testing and the built-in operation bool()".[^1] "When this method is not defined, __len__() is called, if it is defined, and the object is considered true if its result is nonzero. If a class defines neither __len__() nor __bool__() (which is true of the object class itself), all its instances are considered true."[^1]

**r7.** `__call__` is "Called when the instance is “called” as a function".[^1] Example: [`callable`](#callable).

**r8.** The section groups the methods by topic: basic customization (3.3.1), attribute access (3.3.2), class creation (3.3.3), instance and subclass checks (3.3.4), callable objects (3.3.6), container types (3.3.7), numeric types (3.3.8) and context managers (3.3.9).[^1] "When implementing a class that emulates any built-in type, it is important that the emulation only be implemented to the degree that it makes sense for the object being modelled."[^1]

## Examples

```python run id=callable
class Multiplier:
    def __init__(self, factor):
        self.factor = factor

    def __call__(self, x):
        return x * self.factor

double = Multiplier(2)
print(double(21), list(map(double, [1, 2, 3])), callable(double))
```

```output for=callable
42 [2, 4, 6] True
```

An instance with `__call__` can go where code expects a function (rule r7).

```python run id=none
class Opaque:
    __iter__ = None

    def __getitem__(self, i):
        return i

try:
    iter(Opaque())
except TypeError as e:
    print("TypeError")
print(Opaque()[5])
```

```output for=none
TypeError
5
```

`__iter__ = None` turns iteration off, even though the class has `__getitem__` (rule r4).

## Pitfalls

A special method on the instance is not used by the built-in operation:

```python fails id=instance-len expect=exception:TypeError
class C:
    pass

c = C()
c.__len__ = lambda: 5
len(c)
```

len() looks for `__len__` on the type C, not in the instance dictionary (rule r3). Define the method in the class.

## Comparison

Go has one common hook of this kind for printing: "If you want to control the default format for a custom type, all that's required is to define a method with the signature String() string on the type."[^2] Go has no operator overloading: "Regarding operator overloading, it seems more a convenience than an absolute requirement. Again, things are simpler without it."[^3] In Python each operator and many built-in functions call a special method (rule r1).
