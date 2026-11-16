---
tag: HQ9SVE
kind: feature
owner: python
slug: instance-objects
name: Instance Objects
aliases: [instance, object instance, data attribute, instance attribute, instance dictionary, __dict__, vars]
area: languages/python/user-types
level: intro
summary: >-
  An instance has two kinds of attributes: data attributes and methods. A data attribute comes into existence when code first assigns it. The lookup tries the instance dictionary before the class.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#id4", locator: "3.2.11. Class instances"}
see_also: [python/classes, python/class-objects, python/method-objects, python/class-and-instance-variables]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#instance-objects
    title: The Python Tutorial, 9. Classes
    locator: 9.3.3. Instance Objects; 9.4. Random Remarks
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#id4
    title: The Python Language Reference, 3. Data model
    locator: 3.2.11. Class instances
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Struct_types
    title: The Go Programming Language Specification
    locator: Struct types
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class MyClass:
    i = 12345

    def f(self):
        return 'hello world'

x = MyClass()
x.counter = 1
while x.counter < 10:
    x.counter = x.counter * 2
print(x.counter)
del x.counter
print(vars(x), hasattr(x, "counter"), x.i, x.f())
```

```output for=basics
16
{} False 12345 hello world
```

The code makes the data attribute `counter` with an assignment and removes it with `del`. The class gives `i` and `f`.

## Semantics

**r1.** "The only operations understood by instance objects are attribute references. There are two kinds of valid attribute names: data attributes and methods."[^1]

**r2.** "Data attributes need not be declared; like local variables, they spring into existence when they are first assigned to."[^1] Example: [`typo`](#typo).

**r3.** "A class instance has a namespace implemented as a dictionary which is the first place in which attribute references are searched. When an attribute is not found there, and the instance’s class has an attribute by that name, the search continues with the class attributes."[^2] "If the same attribute name occurs in both an instance and in a class, then attribute lookup prioritizes the instance".[^1] Example: [`lookup`](#lookup).

**r4.** "Attribute assignments and deletions update the instance’s dictionary, never a class’s dictionary."[^2] Example: [`delete`](#delete).

**r5.** "By definition, all attributes of a class that are function objects define corresponding methods of its instances."[^1] "But x.f is not the same thing as MyClass.f".[^1] It is a method object. Example: [`method`](#method).

**r6.** `object.__dict__` is "A dictionary or other mapping object used to store an object’s (writable) attributes. Not all instances have a __dict__ attribute".[^2] `object.__class__` is "The class to which a class instance belongs."[^2]

## Examples

```python run id=lookup
class Warehouse:
    purpose = 'storage'
    region = 'west'

w1 = Warehouse()
print(w1.purpose, w1.region)
w2 = Warehouse()
w2.region = 'east'
print(w2.purpose, w2.region)
print(vars(w1), vars(w2))
```

```output for=lookup
storage west
storage east
{} {'region': 'east'}
```

`w2.region` is in the instance dictionary, so the lookup does not go to the class (rule r3).

```python run id=delete
class Warehouse:
    region = 'west'

w = Warehouse()
w.region = 'east'
del w.region
print(w.region, Warehouse.region)
```

```output for=delete
west west
```

The `del` removes the name from the instance dictionary only. After it, the lookup finds the class attribute again (rule r4).

```python run id=method
class MyClass:
    def f(self):
        return 'hello world'

x = MyClass()
print(type(MyClass.f).__name__, type(x.f).__name__)
print(x.__class__ is MyClass, x.f.__func__ is MyClass.f)
```

```output for=method
function method
True True
```

## Pitfalls

Python does not check the name of a new data attribute. A misspelt name makes a second attribute:

```python wrong id=typo
class Account:
    def __init__(self):
        self.balance = 0

acct = Account()
acct.balence = 100
print(acct.balance, vars(acct))
```

```output for=typo
0 {'balance': 0, 'balence': 100}
```

The assignment is valid, so there is no error (rule r2). Look at `vars(acct)` when a value does not change as you expect.

## Comparison

A Go struct has a fixed list of fields: "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^3] The field list is part of the struct type. A Python instance keeps its data attributes in a dictionary, and an assignment can add a new one at any time (rules r2 and r3).
