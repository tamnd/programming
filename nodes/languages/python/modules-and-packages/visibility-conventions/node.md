---
tag: KG2T75
kind: feature
owner: python
slug: visibility-conventions
name: Visibility conventions
aliases: [private names in Python, leading underscore, _name, internal module, __all__ public API, public and internal interfaces, name mangling, __spam, non-public attribute, re-export]
area: languages/python/modules-and-packages
level: core
summary: >-
  Python does not enforce privacy. A leading underscore marks a name as internal, __all__ lists the public names of a module, and __name in a class is mangled. Other modules can still use every name.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0008/#public-and-internal-interfaces", locator: "Public and Internal Interfaces; Descriptive: Naming Styles"}
see_also: [python/modules, python/the-import-statement, python/importing-from-a-package, python/packages, python/module-objects]
sources:
- url: https://docs.python.org/3.15/tutorial/classes.html#private-variables
  title: The Python Tutorial, 9. Classes
  locator: 9.6. Private Variables
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0008/
  title: "PEP 8: Style Guide for Python Code"
  locator: "Descriptive: Naming Styles; Designing for Inheritance; Public and Internal Interfaces"
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/reference/lexical_analysis.html#reserved-classes-of-identifiers
  title: The Python Language Reference, 2. Lexical analysis
  locator: 2.3.3. Reserved classes of identifiers
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement
  title: The Python Language Reference, 7. Simple statements
  locator: 7.11. The import statement
  accessed: 2026-11-18
- url: https://go.dev/ref/spec#Exported_identifiers
  title: The Go Programming Language Specification
  locator: Exported identifiers
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-6.html#jls-6.6
  title: The Java Language Specification, Java SE 25 Edition, Chapter 6. Names
  locator: 6.6. Access Control; 6.6.1. Determining Accessibility
  accessed: 2026-11-18
---

## Synopsis

```text
# geometry.py
__all__ = ["area"]          # the public names of the module
import math as _math        # an import is internal; the alias makes this clear

def area(r): ...            # public
def _check(r): ...          # internal: one leading underscore

class Shape:
    def __init__(self):
        self.__id = 1       # mangled to self._Shape__id

shop/_pricing.py            # an internal module
shop/__init__.py            # from ._pricing import total; __all__ = ["total"]
```

## Semantics

**r1.** The tutorial says: "“Private” instance variables that cannot be accessed except from inside an object don’t exist in Python. However, there is a convention that is followed by most Python code: a name prefixed with an underscore (e.g. _spam) should be treated as a non-public part of the API (whether it is a function, a method or a data member)."[^1] PEP 8 calls this a "weak “internal use” indicator".[^2] Example: [`underscore`](#underscore).

**r2.** The underscore has one effect in the language. The reference lists `_*` as "Not imported by from module import *."[^3] "If __all__ is not defined, the set of public names includes all names found in the module’s namespace which do not begin with an underscore character ('_')."[^4] Examples: [`underscore`](#underscore), [`imports-leak`](#imports-leak).

**r3.** `__all__` lists the public names. "The names given in __all__ are all considered public and are required to exist."[^4] "It is intended to avoid accidentally exporting items that are not part of the API (such as library modules which were imported and used within the module)."[^4] PEP 8 says: "modules should explicitly declare the names in their public API using the __all__ attribute. Setting __all__ to an empty list indicates that the module has no public API."[^2] Example: [`all-list`](#all-list).

**r4.** PEP 8 asks for both marks: "Even with __all__ set appropriately, internal interfaces (packages, modules, classes, functions, attributes or other names) should still be prefixed with a single leading underscore. An interface is also considered internal if any containing namespace (package, module or class) is considered internal."[^2] Example: [`private-module`](#private-module).

**r5.** "Imported names should always be considered an implementation detail. Other modules must not rely on indirect access to such imported names unless they are an explicitly documented part of the containing module’s API, such as os.path or a package’s __init__ module that exposes functionality from submodules."[^2] Examples: [`imports-leak`](#imports-leak), [`private-module`](#private-module).

**r6.** In a class, two leading underscores change the name. "Any identifier of the form __spam (at least two leading underscores, at most one trailing underscore) is textually replaced with _classname__spam, where classname is the current class name with leading underscore(s) stripped."[^1] The tutorial gives the purpose: "to avoid name clashes of names with names defined by subclasses".[^1] Example: [`mangling`](#mangling).

**r7.** Names with two underscores on both sides belong to Python. PEP 8 calls them "“magic” objects or attributes that live in user-controlled namespaces. E.g. __init__, __import__ or __file__. Never invent such names; only use them as documented."[^2]

## Examples

An underscore name is left out of `from geometry import *` (rule r2). A plain import can still use it, because the underscore is only a convention (rule r1).

```python norun reason=fragment file=geometry.py group=underscore
def area(r):
    _check(r)
    return 3 * r * r


def _check(r):
    if r < 0:
        raise ValueError("negative radius")
```

```python run file=main.py group=underscore id=underscore
from geometry import *

print(area(2))
print("_check" in dir())

import geometry

geometry._check(1)
print("geometry._check still works")
```

```output for=underscore
12
False
geometry._check still works
```

Without `__all__`, a star import also copies the modules and names that the module imported (rules r2 and r5).

```python norun reason=fragment file=report.py group=imports-leak
import json
from os import path


def save(data, name):
    return path.join("out", name), json.dumps(data)
```

```python run file=main.py group=imports-leak id=imports-leak
from report import *

print(sorted(name for name in dir() if not name.startswith("__")))
```

```output for=imports-leak
['json', 'path', 'save']
```

With `__all__`, the star import gives only the listed names (rule r3). An explicit import of another name still works.

```python norun reason=fragment file=report.py group=all-list
import json
from os import path

__all__ = ["save"]


def save(data, name):
    return path.join("out", name), json.dumps(data)


def preview(data):
    return json.dumps(data)[:20]
```

```python run file=main.py group=all-list id=all-list
from report import *

print(sorted(name for name in dir() if not name.startswith("__")))

from report import preview

print(preview({"a": 1}))
```

```output for=all-list
['save']
{"a": 1}
```

A package keeps its code in an internal module and exposes the public names in `__init__.py` (rules r4 and r5). Users import `shop.total`, and the module `shop._pricing` can change.

```python norun reason=fragment file=shop/_pricing.py group=private-module
TAX = 0.25


def total(prices):
    return round(sum(prices) * (1 + TAX), 2)
```

```python norun reason=fragment file=shop/__init__.py group=private-module
from ._pricing import total

__all__ = ["total"]
```

```python run file=main.py group=private-module id=private-module
import shop

print(shop.total([2.0, 6.0]))
print(shop.total.__module__)
print(shop.__all__)
```

```output for=private-module
10.0
shop._pricing
['total']
```

A `__name` in a class is stored under a mangled name (rule r6). The mangled name can still be used from outside.

```python run id=mangling
class Account:
    def __init__(self, balance):
        self.__balance = balance

    def balance(self):
        return self.__balance


a = Account(100)
print(a.balance())
print(vars(a))
print(a._Account__balance)
try:
    a.__balance
except AttributeError as e:
    print("AttributeError:", e.name)
```

```output for=mangling
100
{'_Account__balance': 100}
100
AttributeError: __balance
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Pitfalls

### A name in `__all__` that does not exist

The names in `__all__` "are required to exist" (rule r3). A misspelled name breaks every star import of the module.

```python norun reason=fragment file=units.py group=all-missing
__all__ = ["metre", "kilogram"]

metre = 1.0
kilo_gram = 1.0
```

```python fails file=main.py group=all-missing id=all-missing expect=exception:AttributeError
from units import *

print(metre)
```

A plain `import units` works, so the error shows only at a star import. Check `__all__` with a test.

### Two underscores for an internal name

`__name` in a class is not a stronger form of `_name`. It changes the attribute name (rule r6), and a subclass cannot use it under its written name. PEP 8 says: "We don’t use the term “private” here, since no attribute is really private in Python (without a generally unnecessary amount of work)."[^2] Use one underscore for an internal name, and two only to avoid a clash with a subclass.

```python fails id=subclass-mangling expect=exception:AttributeError
class Base:
    def __init__(self):
        self.__size = 1


class Child(Base):
    def grow(self):
        self.__size += 1


Child().grow()
```

## Comparison

**Go** makes visibility part of the name, and the compiler checks it: "An identifier is exported if both: the first character of the identifier's name is a Unicode uppercase letter (Unicode character category Lu); and the identifier is declared in the package block or it is a field name or method name. All other identifiers are not exported."[^5]

**Java** has access modifiers that the compiler checks: "The Java programming language provides mechanisms for access control, to prevent the users of a package or class from depending on unnecessary details of the implementation of that package or class."[^6] In Python the underscore and `__all__` are signals to people and tools. Only the star import reads them (rule r2).

## Implementation notes

Mangling happens when the class body is compiled. The tutorial says: "This mangling is done without regard to the syntactic position of the identifier, as long as it occurs within the definition of a class."[^1] So `vars(a)` in [`mangling`](#mangling) shows `_Account__balance`. The page [[python/module-objects]] shows a module `__dir__`, which controls the names that `dir()` lists.

## References
