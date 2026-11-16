---
tag: 9MYFCM
kind: feature
owner: python
slug: class-objects
name: Class Objects
aliases: [class object, class attribute, instantiation, __init__, constructor, create an instance, class attribute reference]
area: languages/python/user-types
level: intro
summary: >-
  A class object supports two operations: attribute references and instantiation. A call to the class makes a new instance and runs __init__ with the arguments of the call.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#custom-classes", locator: "3.2.10. Custom classes"}
see_also: [python/classes, python/class-definition-syntax, python/instance-objects, python/class-and-instance-variables]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#class-objects
    title: The Python Tutorial, 9. Classes
    locator: 9.3.2. Class Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#custom-classes
    title: The Python Language Reference, 3. Data model
    locator: 3.2.10. Custom classes; object.__init__()
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#composite_literals
    title: Effective Go
    locator: Constructors and composite literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class MyClass:
    """A simple example class"""
    i = 12345

    def f(self):
        return 'hello world'

print(MyClass.i, type(MyClass.f).__name__)
print(MyClass.__doc__)
MyClass.i = 42
x = MyClass()
print(x.i, x.f())
```

```output for=basics
12345 function
A simple example class
42 hello world
```

`MyClass.i` reads a class attribute. `MyClass()` calls the class and gives a new instance.

## Semantics

**r1.** "Class objects support two kinds of operations: attribute references and instantiation."[^1]

**r2.** "Valid attribute names are all the names that were in the class’s namespace when the class object was created."[^1] "Class attributes can also be assigned to, so you can change the value of MyClass.i by assignment."[^1]

**r3.** "A class has a namespace implemented by a dictionary object."[^2] "Class attribute references are translated to lookups in this dictionary".[^2] "When the attribute name is not found there, the attribute search continues in the base classes."[^2]

**r4.** "Class attribute assignments update the class’s dictionary, never the dictionary of a base class."[^2] Example: [`dict`](#dict).

**r5.** "Class instantiation uses function notation. Just pretend that the class object is a parameterless function that returns a new instance of the class."[^1]

**r6.** "When a class defines an __init__() method, class instantiation automatically invokes __init__() for the newly created class instance."[^1] "arguments given to the class instantiation operator are passed on to __init__()."[^1] Example: [`complex`](#complex).

**r7.** "no non-None value may be returned by __init__(); doing so will cause a TypeError to be raised at runtime."[^2] Example: [`init-return`](#init-return).

## Examples

```python run id=complex
class Complex:
    def __init__(self, realpart, imagpart):
        self.r = realpart
        self.i = imagpart

x = Complex(3.0, -4.5)
print(x.r, x.i)
```

```output for=complex
3.0 -4.5
```

The arguments of `Complex(3.0, -4.5)` go to `__init__` after `self` (rule r6).

```python run id=dict
class Base:
    color = "red"

class Child(Base):
    pass

print("color" in Child.__dict__, Child.color)
Child.color = "blue"
print(Child.__dict__["color"], Base.color)
```

```output for=dict
False red
blue red
```

The lookup finds `color` in the base class (rule r3). The assignment puts a new `color` in the dictionary of Child, and Base does not change (rule r4).

## Pitfalls

`__init__` sets up the instance. It must not return a value:

```python fails id=init-return expect=exception:TypeError
class Account:
    def __init__(self, owner):
        self.owner = owner
        return self

Account("Ada")
```

The call to the class returns the new instance. `__init__` must return None (rule r7).

## Comparison

Go has no class objects and no `__init__`. A constructor is an ordinary function, and Effective Go says: "Sometimes the zero value isn't good enough and an initializing constructor is necessary".[^3] A composite literal is "an expression that creates a new instance each time it is evaluated."[^3] In Python the class object itself is the constructor, and a call to it runs `__init__` (rules r5 and r6).
