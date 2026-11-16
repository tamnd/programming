---
tag: JXYMN7
kind: feature
owner: python
slug: the-dir-function
name: The dir() Function
aliases: [dir, dir(), __dir__, list attributes, module __dir__]
area: languages/python/metaprogramming
level: intro
summary: >-
  dir() with no argument lists the names in the current local scope. dir(obj) lists the attributes of an object, its class and the base classes, sorted. An object can change the list with __dir__.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#dir", locator: "Built-in Functions: dir()"}
proposals: [pep-562]
see_also: [runtime-reflection, python/dynamic-attribute-access, python/the-builtins-namespace, python/attribute-access-customization]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#dir
    title: Built-in Functions, Python 3.15 documentation
    locator: dir()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__dir__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.2 Customizing attribute access; 3.3.2.1 Customizing module attribute access
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/modules.html#the-dir-function
    title: The Python Tutorial, Python 3.15
    locator: 6.3 The dir() Function
    accessed: 2026-11-16
---

## Synopsis

```python run id=basic
def f():
    x = 1
    y = 2
    print(dir())

f()

class Point:
    dims = 2

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def norm(self):
        return abs(self.x) + abs(self.y)

p = Point(3, 4)
print([n for n in dir(p) if not n.startswith("_")])
print([n for n in dir(Point) if not n.startswith("_")])
```

```output for=basic
['x', 'y']
['dims', 'norm', 'x', 'y']
['dims', 'norm']
```

## Semantics

**r1.** "Without arguments, return the list of names in the current local scope. With an argument, attempt to return a list of valid attributes for that object."[^1] Example: [`basic`](#basic).

**r2.** "If the object has a method named __dir__(), this method will be called and must return the list of attributes."[^1] `dir()` "converts the returned iterable to a list and sorts it."[^2] Example: [`custom-dir`](#custom-dir).

**r3.** With no `__dir__()`, `dir()` uses the `__dict__` of the object and its type. For a module, the list has the attributes of the module. For a class, it has the attributes of the class "and recursively of the attributes of its bases." For any other object, it has the attributes of the object, of its class and of the base classes of the class.[^1]

**r4.** "The resulting list is sorted alphabetically."[^1]

**r5.** A module can define a function `__dir__` with no arguments. "If present, this function overrides the standard dir() search on a module."[^2] Example: [`module-dir`](#module-dir).

**r6.** "dir() does not list the names of built-in functions and variables. If you want a list of those, they are defined in the standard module builtins."[^3] Example: [`builtins`](#builtins).

**r7.** `dir()` is a tool for the interactive prompt. Its "detailed behavior may change across releases."[^1] Do not use its result as a full list of attributes; use `hasattr()` or `getattr()` to test for one attribute.

## Examples

```python run id=custom-dir
class Shape:
    def __dir__(self):
        return ["perimeter", "area"]

print(dir(Shape()))
```

```output for=custom-dir
['area', 'perimeter']
```

`__dir__` returned the names in any order, and `dir()` sorted them (rule r2).

```python run id=module-dir
import types

m = types.ModuleType("m")
m.a = 1
m.__dir__ = lambda: ["a", "b"]
print(dir(m))
```

```output for=module-dir
['a', 'b']
```

A module file does the same thing with `def __dir__(): ...` at the top level (rule r5).

```python run id=builtins
import builtins

print("len" in dir())
print("len" in dir(builtins))
```

```output for=builtins
False
True
```

## Pitfalls

```python wrong id=dynamic-attr
class Lazy:
    def __getattr__(self, name):
        if name == "answer":
            return 42
        raise AttributeError(name)

z = Lazy()
print(z.answer)
print("answer" in dir(z))
```

```output for=dynamic-attr
42
False
```

The attribute exists, but `dir()` cannot see it. "The resulting list is not necessarily complete and may be inaccurate when the object has a custom __getattr__()."[^1] Define `__dir__` too, or test with `hasattr()`.

```python wrong id=metaclass-attr
print(hasattr(int, "mro"))
print("mro" in dir(int))
```

```output for=metaclass-attr
True
False
```

`mro` is an attribute of the metaclass `type`, and "metaclass attributes are not in the result list when the argument is a class."[^1]

## Comparison

`vars(obj)` returns the `__dict__` of one object, with no inherited names. `dir(obj)` merges the names of the object, its class and the base classes into one sorted list (rule r3). See [[python/dynamic-attribute-access]]. For a general view of reflection in many languages, see [[runtime-reflection]].
