---
tag: 0YSJMR
kind: feature
owner: python
slug: instance-and-subclass-checks
name: Instance and subclass checks
aliases: [isinstance, issubclass, type check, virtual subclass, __instancecheck__, __subclasscheck__]
area: languages/python/user-types
level: advanced
summary: >-
  isinstance(obj, C) asks if obj is an instance of C or of a subclass of C. issubclass(D, C) asks the same about a class. Both accept a tuple or union of classes, and an ABC can add virtual subclasses.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#customizing-instance-and-subclass-checks", locator: "3.3.4. Customizing instance and subclass checks"}
see_also: [python/inheritance, python/abstract-base-classes, python/duck-typing, python/class-objects]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#isinstance
    title: Built-in Functions
    locator: isinstance; issubclass
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#inheritance
    title: The Python Tutorial, 9. Classes
    locator: 9.5. Inheritance
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/abc.html#abc.ABCMeta.register
    title: abc, Abstract Base Classes
    locator: class abc.ABCMeta; register(subclass)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#customizing-instance-and-subclass-checks
    title: The Python Language Reference, 3. Data model
    locator: 3.3.4. Customizing instance and subclass checks
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Type_assertions
    title: The Go Programming Language Specification
    locator: Type assertions; Type switches
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Animal: pass
class Dog(Animal): pass

d = Dog()
print(isinstance(d, Dog), isinstance(d, Animal), type(d) is Animal)
print(issubclass(Dog, Animal), issubclass(Dog, Dog), issubclass(Animal, Dog))
print(isinstance(3.0, (int, float)), isinstance("x", int | str))
```

```output for=basics
True True False
True True False
True True
```

`isinstance` counts subclasses, and `type(d) is Animal` does not. A class is a subclass of itself.

## Semantics

**r1.** isinstance returns "True if the object argument is an instance of the classinfo argument, or of a (direct, indirect, or virtual) subclass thereof."[^1] The tutorial says `isinstance(obj, int)` "will be True only if obj.__class__ is int or some class derived from int."[^2]

**r2.** issubclass returns "True if cls is a subclass (direct, indirect, or virtual) of classinfo. A class is considered a subclass of itself."[^1] The tutorial says "issubclass(bool, int) is True since bool is a subclass of int."[^2] Example: [`bool`](#bool).

**r3.** "If classinfo is a tuple of type objects (or recursively, other such tuples) or a Union Type of multiple types, return True if object is an instance of any of the types."[^1] "If classinfo is not a type or tuple of types and such tuples, a TypeError exception is raised."[^1] Example: [`not-a-type`](#not-a-type).

**r4.** An ABC can "register unrelated concrete classes (even built-in classes) and unrelated ABCs as “virtual subclasses”".[^3] The registering ABC "won’t show up in their MRO (Method Resolution Order) nor will method implementations defined by the registering ABC be callable".[^3] Example: [`virtual`](#virtual).

**r5.** `__instancecheck__` and `__subclasscheck__` "are used to override the default behavior of the isinstance() and issubclass() built-in functions."[^4] "Note that these methods are looked up on the type (metaclass) of a class. They cannot be defined as class methods in the actual class."[^4] Examples: [`metaclass`](#metaclass), [`on-class`](#on-class).

## Examples

```python run id=virtual
from abc import ABC

class Sequenceish(ABC):
    def first(self):
        return self[0]

Sequenceish.register(tuple)
print(isinstance((1, 2), Sequenceish), issubclass(tuple, Sequenceish))
print(Sequenceish in tuple.__mro__, hasattr((1, 2), "first"))
```

```output for=virtual
True True
False False
```

tuple passes both checks, but it does not inherit `first` (rule r4).

```python run id=metaclass
class EvenMeta(type):
    def __instancecheck__(cls, obj):
        return isinstance(obj, int) and obj % 2 == 0

class Even(metaclass=EvenMeta):
    pass

print(isinstance(4, Even), isinstance(5, Even), isinstance("4", Even))
```

```output for=metaclass
True False False
```

The metaclass defines `__instancecheck__`, so `isinstance(x, Even)` calls it (rule r5).

## Pitfalls

bool is a subclass of int, so an int check accepts True:

```python wrong id=bool
def set_port(port):
    if not isinstance(port, int):
        raise TypeError("port must be an int")
    return port

print(set_port(True))
```

```output for=bool
True
```

Add `or isinstance(port, bool)` to the rejection test when a bool is not a valid value (rule r2).

`__instancecheck__` in the class itself has no effect:

```python wrong id=on-class
class Even:
    @classmethod
    def __instancecheck__(cls, obj):
        return isinstance(obj, int) and obj % 2 == 0

print(isinstance(4, Even))
```

```output for=on-class
False
```

Define it in a metaclass, as in the metaclass example (rule r5).

The second argument must be a class, a tuple of classes or a union:

```python fails id=not-a-type expect=exception:TypeError
isinstance(3, "int")
```

Pass `int`, not the string `'int'` (rule r3).

## Comparison

Go checks the type of an interface value with a type assertion. `x.(T)` "asserts that x is not nil and that the value stored in x is of type T."[^5] "If the type assertion is false, a run-time panic occurs."[^5] In the form `v, ok := x.(T)`, ok is false when the assertion fails, and "No run-time panic occurs in this case."[^5] "A type switch compares types rather than values."[^5] For a non-interface type T, the assertion checks that "the dynamic type of x is identical to the type T".[^5] Python's `isinstance` also accepts instances of subclasses (rule r1).
