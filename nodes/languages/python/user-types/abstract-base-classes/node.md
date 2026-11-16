---
tag: 7H3JHD
kind: feature
owner: python
slug: abstract-base-classes
name: Abstract base classes
aliases: [ABC, ABCMeta, abstractmethod, abstract method, abstract class, interface class, __subclasshook__]
area: languages/python/user-types
level: core
summary: >-
  An abstract base class (ABC) derives from abc.ABC and marks methods with @abstractmethod. Python refuses to make an instance until a subclass overrides every abstract method.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/abc.html", locator: "abc, Abstract Base Classes"}
see_also: [python/instance-and-subclass-checks, python/duck-typing, python/inheritance, python/properties]
sources:
  - url: https://docs.python.org/3.15/library/abc.html#abc.abstractmethod
    title: abc, Abstract Base Classes
    locator: class abc.ABC; class abc.ABCMeta; __subclasshook__; @abc.abstractmethod
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-abstract-base-class
    title: Glossary
    locator: abstract base class
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#implements_interface
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why doesn’t Go have “implements” declarations?; How can I guarantee my type satisfies an interface?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
from abc import ABC, abstractmethod

class Shape(ABC):
    @abstractmethod
    def area(self):
        ...

    def describe(self):
        return f"{type(self).__name__} with area {self.area()}"

class Square(Shape):
    def __init__(self, side):
        self.side = side

    def area(self):
        return self.side ** 2

print(Square(3).describe())
try:
    Shape()
except TypeError:
    print("Shape is abstract")
```

```output for=basics
Square with area 9
Shape is abstract
```

Shape declares `area` and gives `describe` to every subclass. Square overrides `area`, so it can make instances.

## Semantics

**r1.** "Abstract base classes complement duck-typing by providing a way to define interfaces when other techniques like hasattr() would be clumsy or subtly wrong".[^2] abc.ABC is "A helper class that has ABCMeta as its metaclass."[^1]

**r2.** "A class that has a metaclass derived from ABCMeta cannot be instantiated unless all of its abstract methods and properties are overridden."[^1] Example: [`partial`](#partial).

**r3.** "Using this decorator requires that the class’s metaclass is ABCMeta or is derived from it."[^1] Example: [`no-abc`](#no-abc).

**r4.** "The abstract methods can be called using any of the normal ‘super’ call mechanisms."[^1] Example: [`super-call`](#super-call).

**r5.** "When abstractmethod is applied in combination with other method descriptors, it should be applied as the innermost decorator".[^1] Example: [`property`](#property).

**r6.** "The abstractmethod only affects subclasses derived using regular inheritance; “virtual subclasses” registered with the ABC’s register() method are not affected."[^1] Example: [`register`](#register).

**r7.** `__subclasshook__` lets you "customize the behavior of issubclass() further without the need to call register() on every class you want to consider a subclass of the ABC."[^1] "If it returns NotImplemented, the subclass check is continued with the usual mechanism."[^1] Example: [`hook`](#hook).

## Examples

```python run id=super-call
from abc import ABC, abstractmethod

class Exporter(ABC):
    @abstractmethod
    def export(self, rows):
        return [str(r) for r in rows]

class CsvExporter(Exporter):
    def export(self, rows):
        return ",".join(super().export(rows))

print(CsvExporter().export([1, 2, 3]))
```

```output for=super-call
1,2,3
```

The abstract method has a body, and the subclass calls it with `super()` (rule r4).

```python run id=property
from abc import ABC, abstractmethod

class Named(ABC):
    @property
    @abstractmethod
    def name(self):
        ...

class User(Named):
    @property
    def name(self):
        return "ann"

print(User().name)
```

```output for=property
ann
```

`@abstractmethod` is the innermost decorator, under `@property` (rule r5).

```python run id=hook
from abc import ABC

class Closeable(ABC):
    @classmethod
    def __subclasshook__(cls, C):
        if cls is Closeable:
            if any("close" in B.__dict__ for B in C.__mro__):
                return True
        return NotImplemented

class File:
    def close(self):
        pass

print(issubclass(File, Closeable), isinstance(File(), Closeable), isinstance(3, Closeable))
```

```output for=hook
True True False
```

File does not name Closeable, but the hook accepts any class with a `close` method (rule r7).

## Pitfalls

A subclass that overrides only some abstract methods is still abstract:

```python fails id=partial expect=exception:TypeError
from abc import ABC, abstractmethod

class Store(ABC):
    @abstractmethod
    def get(self, key): ...

    @abstractmethod
    def put(self, key, value): ...

class ReadOnly(Store):
    def get(self, key):
        return None

ReadOnly()
```

Override `put` too, or keep ReadOnly as an abstract class and override it in a subclass (rule r2).

`@abstractmethod` without the ABC base checks nothing:

```python wrong id=no-abc
from abc import abstractmethod

class Shape:
    @abstractmethod
    def area(self): ...

print(type(Shape()).__name__)
```

```output for=no-abc
Shape
```

Derive from `ABC` (rule r3).

`register` does not check the methods:

```python wrong id=register
from abc import ABC, abstractmethod

class Shape(ABC):
    @abstractmethod
    def area(self): ...

class Blob:
    pass

Shape.register(Blob)
print(isinstance(Blob(), Shape), hasattr(Blob(), "area"))
```

```output for=register
True False
```

Blob passes the isinstance check but has no `area`. Use register only for a class that has the methods (rule r6).

## Comparison

"A Go type implements an interface by implementing the methods of that interface, nothing more."[^3] A Go type does not name its interfaces, like a class accepted by `__subclasshook__` (rule r7). To make the compiler check a type, the Go FAQ shows the line `var _ I = T{} // Verify that T implements I.`[^3] Python checks an ABC subclass when the program makes an instance, and the check covers only abstract methods (rule r2).
