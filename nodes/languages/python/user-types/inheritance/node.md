---
tag: Y1KAGW
kind: feature
owner: python
slug: inheritance
name: Inheritance
aliases: [Python inheritance, derived class, base class, subclass, superclass, override a method, isinstance, issubclass]
area: languages/python/user-types
level: intro
summary: >-
  A derived class names its base class in parentheses. Attribute lookup goes on to the base class when the derived class does not have the name, and a derived class can override any method.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions", locator: "8.8. Class definitions"}
see_also: [python/classes, python/multiple-inheritance, python/class-objects, python/method-objects]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#inheritance
    title: The Python Tutorial, 9. Classes
    locator: 9.5. Inheritance
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__init__
    title: The Python Language Reference, 3. Data model
    locator: object.__init__()
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#inheritance
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why is there no type inheritance?
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#embedding
    title: Effective Go
    locator: Embedding
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        return f"{self.name} makes a sound"

    def describe(self):
        return self.speak() + "."

class Dog(Animal):
    def speak(self):
        return f"{self.name} barks"

d = Dog("Rex")
print(d.describe())
print(isinstance(d, Animal), issubclass(Dog, Animal), [b.__name__ for b in Dog.__bases__])
```

```output for=basics
Rex barks.
True True ['Animal']
```

Dog gets `__init__` and `describe` from Animal. `describe` calls `self.speak()`, and that call goes to the method of Dog.

## Semantics

**r1.** A derived class lists its base class in parentheses after the class name.[^1] "In place of a base class name, other arbitrary expressions are also allowed."[^1]

**r2.** "When the class object is constructed, the base class is remembered. This is used for resolving attribute references: if a requested attribute is not found in the class, the search proceeds to look in the base class. This rule is applied recursively if the base class itself is derived from some other class."[^1]

**r3.** "Derived classes may override methods of their base classes. Because methods have no special privileges when calling other methods of the same object, a method of a base class that calls another method defined in the same base class may end up calling a method of a derived class that overrides it. (For C++ programmers: all methods in Python are effectively virtual.)"[^1]

**r4.** "An overriding method in a derived class may in fact want to extend rather than simply replace the base class method of the same name. There is a simple way to call the base class method directly: just call BaseClassName.methodname(self, arguments)."[^1] Example: [`extend`](#extend).

**r5.** "If a base class has an __init__() method, the derived class’s __init__() method, if any, must explicitly call it to ensure proper initialization of the base class part of the instance; for example: super().__init__([args...])."[^2] Example: [`no-super-init`](#no-super-init).

**r6.** "Use isinstance() to check an instance’s type: isinstance(obj, int) will be True only if obj.__class__ is int or some class derived from int."[^1] "Use issubclass() to check class inheritance: issubclass(bool, int) is True since bool is a subclass of int."[^1] Example: [`checks`](#checks).

## Examples

```python run id=extend
class Animal:
    def __init__(self, name):
        self.name = name

class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)
        self.breed = breed

    def __repr__(self):
        return f"Dog({self.name!r}, {self.breed!r})"

class LoudDog(Dog):
    def __repr__(self):
        return Dog.__repr__(self).upper()

print(Dog("Rex", "collie"), LoudDog("Max", "pug"))
```

```output for=extend
Dog('Rex', 'collie') DOG('MAX', 'PUG')
```

`Dog.__init__` calls the base `__init__` with super() (rule r5). `LoudDog.__repr__` calls the base method by the class name and changes its result (rule r4).

```python run id=checks
print(isinstance(True, int), issubclass(bool, int), issubclass(float, int))
print(type(True) is int, type(True).__name__)
```

```output for=checks
True True False
False bool
```

isinstance() accepts a derived class, but a test with `type(x) is` does not (rule r6).

## Pitfalls

An `__init__` in the derived class replaces the base `__init__`. If it does not call the base `__init__`, the base part of the instance is not set:

```python fails id=no-super-init expect=exception:AttributeError
class Animal:
    def __init__(self, name):
        self.name = name

class Dog(Animal):
    def __init__(self, name, breed):
        self.breed = breed

Dog("Rex", "collie").name
```

Add `super().__init__(name)` to `Dog.__init__` (rule r5).

## Comparison

Go has no type inheritance. "Rather than requiring the programmer to declare ahead of time that two types are related, in Go a type automatically satisfies any interface that specifies a subset of its methods."[^3] Go reuses code by embedding, and Effective Go gives the difference: "When we embed a type, the methods of that type become methods of the outer type, but when they are invoked the receiver of the method is the inner type, not the outer one."[^4] In Python the receiver stays the derived instance, so a base method can call an override (rule r3).
