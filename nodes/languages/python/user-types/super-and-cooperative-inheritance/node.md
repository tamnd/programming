---
tag: V3Q1V0
kind: feature
owner: python
slug: super-and-cooperative-inheritance
name: super() and cooperative inheritance
aliases: [super, zero-argument super, cooperative multiple inheritance, next in MRO, calling the parent method]
area: languages/python/user-types
level: advanced
summary: >-
  super() returns a proxy that finds a method in the class after the current one in the MRO. When every class in a hierarchy calls super(), each method in a diamond runs once, in MRO order.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#super", locator: "Built-in Functions, class super"}
see_also: [python/inheritance, python/multiple-inheritance, python/object-creation, python/static-and-class-methods]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#super
    title: Built-in Functions
    locator: class super
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#multiple-inheritance
    title: The Python Tutorial, 9. Classes
    locator: 9.5.1. Multiple Inheritance
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#embedding
    title: Effective Go
    locator: Embedding
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Account:
    def __init__(self, owner):
        self.owner = owner

    def describe(self):
        return f"account of {self.owner}"

class Savings(Account):
    def __init__(self, owner, rate):
        super().__init__(owner)
        self.rate = rate

    def describe(self):
        return super().describe() + f" at {self.rate}%"

print(Savings("Ann", 2).describe())
```

```output for=basics
account of Ann at 2%
```

`super().__init__(owner)` runs the Account initializer on the same object. `super().describe()` extends the parent method instead of copying it.

## Semantics

**r1.** super returns "a proxy object that delegates method calls to a parent or sibling class of type."[^1] "The object_or_type determines the method resolution order to be searched. The search starts from the class right after the type."[^1] Example: [`two-arg`](#two-arg).

**r2.** "When called directly within an ordinary method of a class, both arguments may be omitted (“zero-argument super()”). In this case, type will be the enclosing class, and object_or_type will be the first argument of the immediately enclosing function (typically self)."[^1]

**r3.** "In a class hierarchy with single inheritance, super can be used to refer to parent classes without naming them explicitly, thus making the code more maintainable."[^1]

**r4.** "The second use case is to support cooperative multiple inheritance in a dynamic execution environment."[^1] "Good design dictates that such implementations have the same calling signature in every case".[^1] Example: [`diamond`](#diamond), [`kwargs`](#kwargs).

**r5.** The tutorial says that the method resolution order "calls each parent only once".[^2] Code that calls a parent by name does not follow this order, so a shared base can run twice. Example: [`twice`](#twice).

**r6.** "super() is undefined for implicit lookups using statements or operators such as super()[name]."[^1] Example: [`subscript`](#subscript).

## Examples

```python run id=diamond
class Base:
    def save(self):
        print("Base")

class Logged(Base):
    def save(self):
        print("Logged")
        super().save()

class Cached(Base):
    def save(self):
        print("Cached")
        super().save()

class Record(Logged, Cached):
    def save(self):
        print("Record")
        super().save()

Record().save()
print([c.__name__ for c in Record.__mro__])
```

```output for=diamond
Record
Logged
Cached
Base
['Record', 'Logged', 'Cached', 'Base', 'object']
```

In Logged, `super()` goes to Cached, a sibling that Logged does not name. Base runs once.

```python run id=kwargs
class Named:
    def __init__(self, *, name, **kwargs):
        super().__init__(**kwargs)
        self.name = name

class Sized:
    def __init__(self, *, size, **kwargs):
        super().__init__(**kwargs)
        self.size = size

class Item(Named, Sized):
    pass

it = Item(name="box", size=3)
print(it.name, it.size)
```

```output for=kwargs
box 3
```

Each class takes its own keyword argument and passes the rest on. The last call reaches `object.__init__` with no arguments left (rule r4).

```python run id=two-arg
class A:
    def who(self):
        return "A"

class B(A):
    def who(self):
        return "B"

class C(B):
    def who(self):
        return "C"

c = C()
print(super(C, c).who(), super(B, c).who())
```

```output for=two-arg
B A
```

`super(B, c)` starts the search after B in the MRO of C (rule r1).

## Pitfalls

Calling the parents by name in a diamond runs the shared base twice:

```python wrong id=twice
class Base:
    def __init__(self):
        print("Base init")

class Left(Base):
    def __init__(self):
        Base.__init__(self)

class Right(Base):
    def __init__(self):
        Base.__init__(self)

class Both(Left, Right):
    def __init__(self):
        Left.__init__(self)
        Right.__init__(self)

Both()
```

```output for=twice
Base init
Base init
```

Use `super().__init__()` in every class, as in the diamond example (rule r5).

The proxy does not support operators:

```python fails id=subscript expect=exception:TypeError
class Table(dict):
    def __getitem__(self, key):
        return super()[key.lower()]

Table(a=1)["A"]
```

Write `super().__getitem__(key.lower())` (rule r6).

## Comparison

Go reuses methods by embedding. Effective Go says: "When we embed a type, the methods of that type become methods of the outer type, but when they are invoked the receiver of the method is the inner type, not the outer one."[^3] To call the inner method from an outer method with the same name, Go code names the embedded field, as in `job.Logger.Printf(...)`.[^3] That is like calling the parent by name in Python, as in the twice example, and not like `super()`.
