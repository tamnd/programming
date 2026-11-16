---
tag: JX2SJH
kind: feature
owner: python
slug: metaclasses
name: Metaclasses
aliases: [metaclass, type, "metaclass=", __prepare__, class creation, most derived metaclass, metaclass conflict, PEP 3115]
area: languages/python/metaprogramming
level: advanced
summary: >-
  A metaclass is the class of a class. type is the default. The metaclass keyword picks another one, which can run code when a class statement runs and can change how its classes behave.
grammar: |
  classdef: [decorators] "class" classname [type_params] [inheritance] ":" suite
  inheritance: "(" [argument_list] ")"  # may contain metaclass=expression
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#metaclasses", locator: "3.3.3.1 Metaclasses to 3.3.3.7 Uses for metaclasses"}
proposals: [pep-3115]
see_also: [python/init-subclass-and-set-name, python/dynamic-class-creation, python/class-objects, python/decorators, python/abstract-base-classes, python/enums]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#metaclasses
    title: The Python Language Reference, Python 3.15
    locator: 3.3.3.1 Metaclasses; 3.3.3.3 Determining the appropriate metaclass; 3.3.3.4 Preparing the class namespace; 3.3.3.5 Executing the class body; 3.3.3.6 Creating the class object; 3.3.3.7 Uses for metaclasses
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3115/
    title: "PEP 3115: Metaclasses in Python 3000"
    locator: Abstract; Specification
    accessed: 2026-11-16
---

## Synopsis

```python run id=meta-new
class Meta(type):
    def __new__(mcls, name, bases, ns, **kwargs):
        print("Meta.__new__", name, sorted(k for k in ns if not k.startswith("__")))
        return super().__new__(mcls, name, bases, ns)

class Base(metaclass=Meta):
    x = 1

class Child(Base):
    def f(self):
        pass

print(type(Base).__name__, type(Child).__name__, type(Meta).__name__)
```

```output for=meta-new
Meta.__new__ Base ['x']
Meta.__new__ Child ['f']
Meta Meta type
```

## Semantics

**r1.** "By default, classes are constructed using type(). The class body is executed in a new namespace and the class name is bound locally to the result of type(name, bases, namespace)."[^1]

**r2.** The `metaclass` keyword in the class line picks the metaclass. A subclass gets the metaclass of its base: "both MyClass and MySubclass are instances of Meta" in the reference example.[^1] Example: [`meta-new`](#meta-new).

**r3.** When a class definition runs, these steps occur in order: "MRO entries are resolved; the appropriate metaclass is determined; the class namespace is prepared; the class body is executed; the class object is created."[^1] Example: [`steps`](#steps).

**r4.** "If the metaclass has a __prepare__ attribute, it is called as namespace = metaclass.__prepare__(name, bases, **kwds)". The class body then runs in that namespace. Without `__prepare__`, the namespace starts as "an empty ordered mapping."[^1]

**r5.** The class object is made "by calling metaclass(name, bases, namespace, **kwds)". Other keywords in the class line go to `__prepare__` and to that call.[^1] Example: [`steps`](#steps).

**r6.** The metaclass that Python uses is the most derived one of the explicit metaclass and the metaclasses of all the bases. "If none of the candidate metaclasses meets that criterion, then the class definition will fail with TypeError."[^1] Example: [`conflict`](#conflict).

**r7.** A method defined on the metaclass applies to the class, not to its instances. Special methods are looked up on the type; the reference notes this for `__instancecheck__`, "only in this case the instance is itself a class."[^1] So `__call__` on the metaclass controls what `Cls()` does, and `__len__` controls `len(Cls)`. Example: [`singleton`](#singleton).

**r8.** "The potential uses for metaclasses are boundless. Some ideas that have been explored include enum, logging, interface checking, automatic delegation, automatic property creation, proxies, frameworks, and automatic resource locking/synchronization."[^1]

## Examples

```python run id=steps
class Recorder(type):
    @classmethod
    def __prepare__(mcls, name, bases, **kwargs):
        print("prepare", name, kwargs)
        return {"defined_by": "prepare"}

    def __new__(mcls, name, bases, ns, **kwargs):
        print("new", name)
        return super().__new__(mcls, name, bases, ns)

    def __init__(cls, name, bases, ns, **kwargs):
        print("init", name)
        super().__init__(name, bases, ns)

class Thing(metaclass=Recorder, flavor="x"):
    print("body runs")

print(Thing.defined_by)
```

```output for=steps
prepare Thing {'flavor': 'x'}
body runs
new Thing
init Thing
prepare
```

The output shows the steps of rule r3. The keyword `flavor` went to `__prepare__`, and the class body saw the namespace that `__prepare__` returned (rules r4 and r5).

```python run id=singleton
class Singleton(type):
    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__call__(*args, **kwargs)
        return cls._instances[cls]

class Settings(metaclass=Singleton):
    def __init__(self):
        print("init once")

a = Settings()
b = Settings()
print(a is b)
```

```output for=singleton
init once
True
```

`Settings()` calls `Singleton.__call__`, because `Settings` is an instance of `Singleton` (rule r7).

```python run id=enum-meta
from enum import Enum

class Color(Enum):
    RED = 1
    GREEN = 2

print(type(Color).__name__)
print(len(Color), list(Color.__members__))
```

```output for=enum-meta
EnumType
2 ['RED', 'GREEN']
```

`len(Color)` works because the metaclass `EnumType` defines `__len__` (rules r7 and r8).

## Versions

> [!since v="3.0"]
> PEP 3115 added the `metaclass=` keyword and `__prepare__`. In its words, "the syntax for specifying a metaclass is via a keyword argument in the list of base classes".[^2]

## Pitfalls

```python fails id=conflict expect=exception:TypeError
class MA(type):
    pass

class MB(type):
    pass

class A(metaclass=MA):
    pass

class B(metaclass=MB):
    pass

class C(A, B):
    pass
```

`MA` and `MB` are not subclasses of each other, so no metaclass is the most derived (rule r6). Make one metaclass inherit from the other, or use one metaclass for both.

```python wrong id=new-instead-of-hook
class Registry(type):
    classes = []

    def __new__(mcls, name, bases, ns):
        cls = super().__new__(mcls, name, bases, ns)
        mcls.classes.append(name)
        return cls

class Plugin(metaclass=Registry):
    pass

class CsvPlugin(Plugin):
    pass

print(Registry.classes)
```

```output for=new-instead-of-hook
['Plugin', 'CsvPlugin']
```

This works, but a metaclass is more than this task needs, and it also records the base class. `__init_subclass__` does the same work with no metaclass and with no conflict risk (rule r6). See [[python/init-subclass-and-set-name]].

## Comparison

A class decorator changes one class after it is made. `__init_subclass__` runs for each subclass. A metaclass controls every step of class creation and the behavior of the class object itself (rules r3 and r7). Use the simplest tool that does the job. See [[python/decorators]] and [[python/init-subclass-and-set-name]].
