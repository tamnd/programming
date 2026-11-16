---
tag: S3E9P1
kind: feature
owner: python
slug: init-subclass-and-set-name
name: __init_subclass__ and __set_name__
aliases: [__init_subclass__, __set_name__, subclass hook, class keyword arguments, plugin registry, PEP 487]
area: languages/python/metaprogramming
level: advanced
summary: >-
  __init_subclass__ on a base class runs each time a subclass is made, with the keywords of the class line. __set_name__ on a class attribute runs when the owner class is made, with the attribute name.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#customizing-class-creation", locator: "3.3.3 Customizing class creation"}
proposals: [pep-487]
see_also: [python/metaclasses, python/descriptors, python/decorators, python/inheritance, python/super-and-cooperative-inheritance]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#customizing-class-creation
    title: The Python Language Reference, Python 3.15
    locator: 3.3.3 Customizing class creation; 3.3.3.6 Creating the class object
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0487/
    title: "PEP 487: Simpler customisation of class creation"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/descriptor.html
    title: Descriptor Guide, Python 3.15 documentation
    locator: Customized names
    accessed: 2026-11-16
---

## Synopsis

```python run id=registry
class Plugin:
    registry = {}

    def __init_subclass__(cls, /, name=None, **kwargs):
        super().__init_subclass__(**kwargs)
        Plugin.registry[name or cls.__name__.lower()] = cls.__name__

class Csv(Plugin):
    pass

class Json(Plugin, name="json-v2"):
    pass

print(Plugin.registry)
```

```output for=registry
{'csv': 'Csv', 'json-v2': 'Json'}
```

## Semantics

**r1.** "Whenever a class inherits from another class, __init_subclass__() is called on the parent class."[^1] The base class itself does not trigger its own hook. Example: [`registry`](#registry).

**r2.** "If defined as a normal instance method, this method is implicitly converted to a class method." Its `cls` argument is the new subclass.[^1]

**r3.** "Keyword arguments which are given to a new class are passed to the parent class’s __init_subclass__." A hook should take out its own keywords and pass the rest to `super().__init_subclass__(**kwargs)`. "The default implementation object.__init_subclass__ does nothing, but raises an error if it is called with any arguments."[^1] Example: [`unknown-keyword`](#unknown-keyword).

**r4.** The `metaclass` keyword "is never passed to __init_subclass__ implementations."[^1]

**r5.** "When a class is created, type.__new__() scans the class variables and makes callbacks to those with a __set_name__() hook." The hook gets the owner class and the name of the attribute.[^1] Example: [`set-name`](#set-name).

**r6.** "If the class variable is assigned after the class is created, __set_name__() will not be called automatically."[^1] Example: [`late-assign`](#late-assign).

**r7.** The two hooks replace many metaclasses. PEP 487 says that a custom metaclass "persists for the entire lifecycle of the class, creating the potential for spurious metaclass conflicts."[^2]

## Examples

```python run id=set-name
class Positive:
    def __set_name__(self, owner, name):
        print("set_name", owner.__name__, name)
        self.private = "_" + name

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self
        return getattr(obj, self.private)

    def __set__(self, obj, value):
        if value <= 0:
            raise ValueError(f"{self.private[1:]} must be positive")
        setattr(obj, self.private, value)

class Order:
    qty = Positive()
    price = Positive()

    def __init__(self, qty, price):
        self.qty = qty
        self.price = price

print(Order(2, 5).qty)
try:
    Order(0, 5)
except ValueError as e:
    print(e)
```

```output for=set-name
set_name Order qty
set_name Order price
2
qty must be positive
```

Each descriptor learns its own name, so the class does not have to pass the name as an argument. The Descriptor Guide uses the hook "so that the field names can be recorded, giving each descriptor its own public_name and private_name" (rule r5).[^3]

```python run id=check-subclass
class Shape:
    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        if "area" not in cls.__dict__:
            raise TypeError(f"{cls.__name__} must define area()")

class Square(Shape):
    def area(self):
        return 1

try:
    class Blob(Shape):
        pass
except TypeError as e:
    print(e)
```

```output for=check-subclass
Blob must define area()
```

The check runs when the `class` statement runs, not when an instance is made.

```python run id=late-assign
class Hook:
    def __set_name__(self, owner, name):
        print("hook", name)

class A:
    pass

A.x = Hook()
print("no hook for x")
h = Hook()
h.__set_name__(A, "y")
```

```output for=late-assign
no hook for x
hook y
```

## Versions

> [!since v="3.6"]
> PEP 487 added `__init_subclass__` and `__set_name__`.[^1][^2]

## Pitfalls

```python fails id=unknown-keyword expect=exception:TypeError
class Base:
    pass

class Child(Base, color="red"):
    pass
```

No class in the chain takes the keyword `color`, so it reaches `object.__init_subclass__`, which "raises an error if it is called with any arguments" (rule r3).[^1]

```python wrong id=no-super
class Base:
    def __init_subclass__(cls, flavor, **kwargs):
        cls.flavor = flavor

class Mixin:
    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        print("Mixin hook", cls.__name__)

class Both(Base, Mixin, flavor="x"):
    pass

print(Both.flavor)
```

```output for=no-super
x
```

`Base.__init_subclass__` does not call `super()`, so the hook of `Mixin` never runs for `Both`. Always pass the other keywords on with `super().__init_subclass__(**kwargs)` (rule r3).

## Comparison

A class decorator changes the one class that it decorates. "__init_subclass__ solely applies to future subclasses of the class defining the method."[^1] A metaclass can do both jobs, but it adds a new type to the class, with the conflict risk of rule r7. See [[python/metaclasses]] and [[python/decorators]].
