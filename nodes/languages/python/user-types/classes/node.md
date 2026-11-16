---
tag: SJEAW6
kind: feature
owner: python
slug: classes
name: Classes
aliases: [Python classes, class, object-oriented programming in Python, OOP in Python, user-defined type, instance, method]
area: languages/python/user-types
level: intro
summary: >-
  A class bundles data and functions into a new type. Calling the class makes an instance, and functions in the class body become methods. Classes are objects, made at run time, and can change later.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions", locator: "8.8. Class definitions"}
see_also: [python/class-definition-syntax, python/class-objects, python/instance-objects, python/method-objects, python/inheritance, python/objects-values-and-types]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#classes
    title: The Python Tutorial, 9. Classes
    locator: 9. Classes; 9.4. Random Remarks
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions
    title: The Python Language Reference, 8. Compound statements
    locator: 8.8. Class definitions
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#Is_Go_an_object-oriented_language
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Is Go an object-oriented language?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Counter:
    """Count events by name."""

    def __init__(self, name):
        self.name = name
        self.count = 0

    def add(self, n=1):
        self.count += n
        return self.count

clicks = Counter("clicks")
clicks.add()
clicks.add(4)
print(clicks.name, clicks.count)
print(type(clicks) is Counter, type(Counter))
```

```output for=basics
clicks 5
True <class 'type'>
```

The class statement makes a new type. `Counter("clicks")` makes an instance and runs `__init__`. A method gets the instance as its first argument, `self`.

## Semantics

**r1.** "Classes provide a means of bundling data and functionality together. Creating a new class creates a new type of object, allowing new instances of that type to be made."[^1]

**r2.** "A class definition is an executable statement."[^2] "Classes without an inheritance list inherit, by default, from the base class object".[^2]

**r3.** "As is true for modules, classes partake of the dynamic nature of Python: they are created at runtime, and can be modified further after creation."[^1] Example: [`dynamic`](#dynamic).

**r4.** "Python classes provide all the standard features of Object Oriented Programming: the class inheritance mechanism allows multiple base classes, a derived class can override any methods of its base class or classes, and a method can call the method of a base class with the same name."[^1]

**r5.** "In C++ terminology, normally class members (including the data members) are public (except see below Private Variables), and all member functions are virtual."[^1] "nothing in Python makes it possible to enforce data hiding".[^1]

**r6.** "the method function is declared with an explicit first argument representing the object, which is provided implicitly by the call."[^1] "Often, the first argument of a method is called self. This is nothing more than a convention".[^1]

**r7.** "As in Smalltalk, classes themselves are objects."[^1] "Unlike C++ and Modula-3, built-in types can be used as base classes for extension by the user."[^1] "Each value is an object, and therefore has a class (also called its type). It is stored as object.__class__."[^1] Example: [`builtin-base`](#builtin-base).

## Examples

```python run id=dynamic
class Point:
    pass

p = Point()
p.x, p.y = 3, 4

def norm(self):
    return (self.x ** 2 + self.y ** 2) ** 0.5

Point.norm = norm
print(p.norm(), Point.__bases__)
```

```output for=dynamic
5.0 (<class 'object'>,)
```

The method is added to the class after the instance exists, and the instance can call it at once (rule r3). The class has `object` as its base (rule r2).

```python run id=builtin-base
class Stack(list):
    def push(self, item):
        self.append(item)

    def peek(self):
        return self[-1]

s = Stack()
s.push(1)
s.push(2)
print(s, s.peek(), len(s), s.__class__.__name__)
print(isinstance(s, list))
```

```output for=builtin-base
[1, 2] 2 2 Stack
True
```

A class can extend a built-in type such as list (rule r7).

## Pitfalls

A method needs the `self` parameter. Without it, the call passes one argument too many:

```python fails id=no-self expect=exception:TypeError
class Greeter:
    def hello():
        return "hello"

Greeter().hello()
```

The instance goes in as the first argument, so the function must accept it (rule r6).

## Comparison

Go has methods but no classes: "Although Go has types and methods and allows an object-oriented style of programming, there is no type hierarchy."[^3] "methods in Go are more general than in C++ or Java: they can be defined for any sort of data, even built-in types such as plain, “unboxed” integers."[^3] In Python a method is a function in a class body (rule r6), and a new type that adds methods to int is a subclass of int (rule r7).
