---
tag: 68PTKB
kind: feature
owner: python
slug: private-variables
name: Private Variables
aliases: [private attribute, name mangling, double underscore, leading underscore, _name convention, __name, non-public API]
area: languages/python/user-types
level: core
summary: >-
  Python has no private attributes. A leading underscore marks a name as non-public by convention. A name with two leading underscores in a class is mangled to _ClassName__name to stop name clashes.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#private-name-mangling", locator: "6.2.2.1. Private name mangling"}
see_also: [python/classes, python/inheritance, python/instance-objects]
sources:
  - url: https://docs.python.org/3.15/tutorial/classes.html#private-variables
    title: The Python Tutorial, 9. Classes
    locator: 9.6. Private Variables
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#private-name-mangling
    title: The Python Language Reference, 6. Expressions
    locator: 6.2.2.1. Private name mangling
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Exported_identifiers
    title: The Go Programming Language Specification
    locator: Exported identifiers
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Account:
    def __init__(self, owner):
        self.owner = owner
        self._audit = []        # non-public by convention
        self.__pin = "0000"     # the name is mangled

acct = Account("Ada")
print(sorted(vars(acct)))
print(acct._audit, acct._Account__pin)
```

```output for=basics
['_Account__pin', '_audit', 'owner']
[] 0000
```

`_audit` is an ordinary name. `__pin` is stored as `_Account__pin`, and code outside the class can still read it by that name.

## Semantics

**r1.** "“Private” instance variables that cannot be accessed except from inside an object don’t exist in Python."[^1]

**r2.** "a name prefixed with an underscore (e.g. _spam) should be treated as a non-public part of the API (whether it is a function, a method or a data member). It should be considered an implementation detail and subject to change without notice."[^1]

**r3.** "When an identifier that textually occurs in a class definition begins with two or more underscore characters and does not end in two or more underscores, it is considered a private name of that class."[^2] So `__init__` and other special names are not mangled.

**r4.** "The class name, with leading underscores removed and a single leading underscore inserted, is inserted in front of the identifier".[^2] "This mangling is done without regard to the syntactic position of the identifier, as long as it occurs within the definition of a class."[^1] Example: [`class-name`](#class-name).

**r5.** "Name mangling is helpful for letting subclasses override methods without breaking intraclass method calls."[^1] Example: [`mapping`](#mapping).

**r6.** "Note that the mangling rules are designed mostly to avoid accidents; it still is possible to access or modify a variable that is considered private."[^1]

**r7.** "Notice that code passed to exec() or eval() does not consider the classname of the invoking class to be the current class".[^1] "The same restriction applies to getattr(), setattr() and delattr(), as well as when referencing __dict__ directly."[^1] Example: [`getattr`](#getattr).

## Examples

```python run id=mapping
class Mapping:
    def __init__(self, iterable):
        self.items_list = []
        self.__update(iterable)

    def update(self, iterable):
        for item in iterable:
            self.items_list.append(item)

    __update = update   # private copy of original update() method

class MappingSubclass(Mapping):
    def update(self, keys, values):
        # provides new signature for update()
        # but does not break __init__()
        for item in zip(keys, values):
            self.items_list.append(item)

m = MappingSubclass("ab")
m.update(["x"], [1])
print(m.items_list)
print([n for n in vars(Mapping) if "update" in n])
```

```output for=mapping
['a', 'b', ('x', 1)]
['update', '_Mapping__update']
```

`Mapping.__init__` calls `self._Mapping__update`, so the override in the subclass does not change it (rule r5).

```python run id=class-name
class _Hidden:
    __secret = 1

class __Twice:
    __secret = 2

print([n for n in vars(_Hidden) if "secret" in n])
print([n for n in vars(__Twice) if "secret" in n])
```

```output for=class-name
['_Hidden__secret']
['_Twice__secret']
```

The leading underscores of the class name are removed, and then one underscore is added (rule r4).

## Pitfalls

Outside the class, the name is not mangled, so the short name is not found:

```python fails id=outside expect=exception:AttributeError
class Account:
    def __init__(self):
        self.__pin = "0000"

Account().__pin
```

A name in a string is not mangled, even inside the class:

```python fails id=getattr expect=exception:AttributeError
class Account:
    def __init__(self):
        self.__pin = "0000"

    def pin(self):
        return getattr(self, "__pin")

Account().pin()
```

getattr() gets the string as it is (rule r7). Use `self.__pin`, or the full name `'_Account__pin'`.

## Comparison

Go controls access by package and by the case of the first letter. "An identifier may be exported to permit access to it from another package."[^3] It is exported if "the first character of the identifier's name is a Unicode uppercase letter".[^3] "All other identifiers are not exported."[^3] Python has only a convention for non-public names (rule r2), and mangling is a guard against accidents, not a lock (rule r6).
