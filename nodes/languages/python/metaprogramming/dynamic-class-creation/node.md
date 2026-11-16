---
tag: QKN6WB
kind: feature
owner: python
slug: dynamic-class-creation
name: Dynamic class creation
aliases: [type with three arguments, "type(name, bases, dict)", types.new_class, __mro_entries__, resolve_bases, get_original_bases, SimpleNamespace, PEP 560]
area: languages/python/metaprogramming
level: advanced
summary: >-
  type(name, bases, dict) makes a class at run time, like a class statement. types.new_class also runs the metaclass steps and __mro_entries__. SimpleNamespace gives an object for free attributes.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#type", locator: "Built-in Functions: type()"}
proposals: [pep-560]
see_also: [python/metaclasses, python/class-objects, python/init-subclass-and-set-name, python/named-tuples, python/dataclasses]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#type
    title: Built-in Functions, Python 3.15 documentation
    locator: type()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/types.html#dynamic-type-creation
    title: types — Dynamic type creation and names for built-in types, Python 3.15 documentation
    locator: Dynamic Type Creation; SimpleNamespace
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#resolving-mro-entries
    title: The Python Language Reference, Python 3.15
    locator: 3.3.3.2 Resolving MRO entries
    accessed: 2026-11-16
---

## Synopsis

```python run id=three-args
def describe(self):
    return f"{type(self).__name__} with {self.sides} sides"

Triangle = type("Triangle", (), {"sides": 3, "describe": describe})
print(Triangle().describe())
print([b.__name__ for b in Triangle.__bases__])

shapes = [type(name, (Triangle,), {"sides": n}) for name, n in [("Square", 4), ("Pentagon", 5)]]
print([s().describe() for s in shapes])
```

```output for=three-args
Triangle with 3 sides
['object']
['Square with 4 sides', 'Pentagon with 5 sides']
```

## Semantics

**r1.** "With three arguments, return a new type object. This is essentially a dynamic form of the class statement."[^1] Example: [`three-args`](#three-args).

**r2.** The name "becomes the __name__ attribute". The bases tuple "becomes the __bases__ attribute; if empty, object, the ultimate base of all classes, is added." The dictionary "contains attribute and method definitions for the class body".[^1]

**r3.** Other keyword arguments of `type()` "are passed to the appropriate metaclass machinery (usually __init_subclass__()) in the same way that keywords in a class definition (besides metaclass) would."[^1] Example: [`keywords`](#keywords).

**r4.** `types.new_class(name, bases, kwds, exec_body)` "Creates a class object dynamically using the appropriate metaclass." `kwds` can hold `metaclass`. `exec_body` is a function that fills the class namespace.[^2] Example: [`new-class`](#new-class).

**r5.** "If a base that appears in a class definition is not an instance of type, then an __mro_entries__() method is searched on the base." The result of the call replaces the base.[^3] The class statement and `types.new_class` do this; `type()` does not. Example: [`mro-entries`](#mro-entries).

**r6.** `types.get_original_bases(cls)` returns the bases "before the __mro_entries__() method has been called on any bases".[^2]

**r7.** `types.SimpleNamespace` is "A simple object subclass that provides attribute access to its namespace, as well as a meaningful repr." It "may be useful as a replacement for class NS: pass."[^2] Example: [`namespace`](#namespace).

## Examples

```python run id=keywords
class Base:
    def __init_subclass__(cls, flavor=None, **kwargs):
        super().__init_subclass__(**kwargs)
        print("subclass", cls.__name__, flavor)

Child = type("Child", (Base,), {}, flavor="mint")
```

```output for=keywords
subclass Child mint
```

```python run id=new-class
import types

class Meta(type):
    def __new__(mcls, name, bases, ns, **kwargs):
        print("meta", name, kwargs)
        return super().__new__(mcls, name, bases, ns)

def body(ns):
    ns["x"] = 1

C = types.new_class("C", (), {"metaclass": Meta, "tag": "a"}, body)
print(type(C).__name__, C.x)
```

```output for=new-class
meta C {'tag': 'a'}
Meta 1
```

```python run id=mro-entries
import types

class Base:
    pass

class Proxy:
    def __mro_entries__(self, bases):
        return (Base,)

p = Proxy()

class D(p):
    pass

print([b.__name__ for b in D.__bases__])
print(types.get_original_bases(D)[0] is p)
E = types.new_class("E", (p,))
print([b.__name__ for b in E.__bases__])
```

```output for=mro-entries
['Base']
True
['Base']
```

`p` is not a class, so Python asked it for the real bases (rule r5). The reference links this hook to PEP 560, "Core support for typing module and generic types."[^3] `Generic[T]` in `class Box(Generic[T])` is not a class, and it uses this hook.

```python run id=namespace
from types import SimpleNamespace

ns = SimpleNamespace(x=1, y=2)
ns.z = 3
del ns.x
print(ns)
print(ns == SimpleNamespace(y=2, z=3))
```

```output for=namespace
namespace(y=2, z=3)
True
```

## Versions

> [!since v="3.3"]
> `types.new_class` and `types.SimpleNamespace` were added.[^2]

> [!since v="3.7"]
> PEP 560 added `__mro_entries__` and `types.resolve_bases`.[^2][^3]

## Pitfalls

```python fails id=type-no-mro-entries expect=exception:TypeError
class Base:
    pass

class Proxy:
    def __mro_entries__(self, bases):
        return (Base,)

E = type("E", (Proxy(),), {})
```

`type()` does not resolve MRO entries (rule r5). Use `types.new_class`, or call `types.resolve_bases()` on the bases first.

```python wrong id=shared-dict
defaults = {"items": []}
A = type("A", (), defaults)
B = type("B", (), defaults)
A.items.append(1)
print(B.items)
```

```output for=shared-dict
[1]
```

The class dictionary is copied, but the list in it is not. Both classes hold the same list object. Make a new list for each class.

## Comparison

A class statement is fixed in the source code. `type()` and `types.new_class` take the name, the bases and the body as data, so a program can make many classes in a loop (rule r1). See [[python/named-tuples]] and [[python/metaclasses]].
