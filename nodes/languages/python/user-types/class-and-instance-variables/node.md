---
tag: 1DBBWE
kind: feature
owner: python
slug: class-and-instance-variables
name: Class and Instance Variables
aliases: [class variable, instance variable, class attribute, shared attribute, static field in Python, mutable class attribute]
area: languages/python/user-types
level: intro
summary: >-
  A class variable is shared by all instances, and an instance variable belongs to one instance. A list in a class variable is shared, so a change through one instance shows in all of them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions", locator: "8.8. Class definitions, Programmer's note"}
see_also: [python/classes, python/class-objects, python/instance-objects, python/objects-values-and-types]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#class-and-instance-variables
    title: The Python Tutorial, 9. Classes
    locator: 9.3.5. Class and Instance Variables
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#class-definitions
    title: The Python Language Reference, 8. Compound statements
    locator: 8.8. Class definitions, Programmer's note
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#id4
    title: The Python Language Reference, 3. Data model
    locator: 3.2.11. Class instances
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Blocks
    title: The Go Programming Language Specification
    locator: Struct types; Blocks
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Dog:
    kind = 'canine'         # class variable shared by all instances

    def __init__(self, name):
        self.name = name    # instance variable unique to each instance

d = Dog('Fido')
e = Dog('Buddy')
print(d.kind, e.kind)
print(d.name, e.name)
```

```output for=basics
canine canine
Fido Buddy
```

`kind` is in the class, so all dogs share it. Each dog has its own `name`.

## Semantics

**r1.** "Generally speaking, instance variables are for data unique to each instance and class variables are for attributes and methods shared by all instances of the class".[^1]

**r2.** "Variables defined in the class definition are class attributes; they are shared by instances. Instance attributes can be set in a method with self.name = value."[^2]

**r3.** "Both class and instance attributes are accessible through the notation “self.name”, and an instance attribute hides a class attribute with the same name when accessed in this way."[^2] Example: [`default`](#default).

**r4.** "Attribute assignments and deletions update the instance’s dictionary, never a class’s dictionary."[^3] So `self.name = value` never changes a class variable. To change it, assign through the class. Examples: [`counter`](#counter), [`self-count`](#self-count).

**r5.** "Class attributes can be used as defaults for instance attributes, but using mutable values there can lead to unexpected results."[^2] The tutorial says that "shared data can have possibly surprising effects involving mutable objects such as lists and dictionaries."[^1] Example: [`tricks`](#tricks).

## Examples

```python run id=default
class Config:
    retries = 3

a = Config()
b = Config()
b.retries = 5
print(a.retries, b.retries, Config.retries)
Config.retries = 4
print(a.retries, b.retries)
```

```output for=default
3 5 3
4 5
```

`b.retries` hides the class value (rule r3). A change to the class value shows in `a`, but not in `b`.

```python run id=counter
class Ticket:
    issued = 0

    def __init__(self):
        Ticket.issued += 1
        self.number = Ticket.issued

t1, t2, t3 = Ticket(), Ticket(), Ticket()
print(t1.number, t3.number, Ticket.issued)
```

```output for=counter
1 3 3
```

The count goes through the class, so all instances share it (rule r4).

## Pitfalls

A mutable class variable is shared by all instances:

```python wrong id=tricks
class Dog:
    tricks = []             # mistaken use of a class variable

    def __init__(self, name):
        self.name = name

    def add_trick(self, trick):
        self.tricks.append(trick)

d = Dog('Fido')
e = Dog('Buddy')
d.add_trick('roll over')
e.add_trick('play dead')
print(d.tricks)
```

```output for=tricks
['roll over', 'play dead']
```

`self.tricks.append` does not assign to `self.tricks`. It changes the one list in the class (rule r5). Make the list in `__init__`:

```python run id=tricks-fix
class Dog:
    def __init__(self, name):
        self.name = name
        self.tricks = []    # creates a new empty list for each dog

    def add_trick(self, trick):
        self.tricks.append(trick)

d = Dog('Fido')
e = Dog('Buddy')
d.add_trick('roll over')
e.add_trick('play dead')
print(d.tricks, e.tricks)
```

```output for=tricks-fix
['roll over'] ['play dead']
```

An augmented assignment through `self` makes an instance variable. It does not change the class variable:

```python wrong id=self-count
class Ticket:
    issued = 0

    def __init__(self):
        self.issued += 1

t1, t2 = Ticket(), Ticket()
print(t1.issued, t2.issued, Ticket.issued)
```

```output for=self-count
1 1 0
```

`self.issued += 1` reads the class value 0 and assigns 1 to the instance (rules r3 and r4). Use `Ticket.issued += 1`.

## Comparison

A Go struct has only fields: "A struct is a sequence of named elements, called fields, each of which has a name and a type."[^4] Each struct value has its own fields. A variable that is not a field can be in the package block: "Each package has a package block containing all Go source text for that package."[^4] All code in the package shares such a variable. In Python a class variable is shared by the instances of one class (rule r2).
