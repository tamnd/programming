---
tag: 0NJ054
kind: feature
owner: python
slug: attribute-access-customization
name: Attribute access customization
aliases: [__getattr__, __getattribute__, __setattr__, __delattr__, __dir__, module __getattr__, attribute hooks, delegation, proxy object]
area: languages/python/user-types
level: advanced
summary: >-
  __getattr__ runs only when the normal lookup fails. __getattribute__ runs for every read, and __setattr__ and __delattr__ run for every assignment and del. They are used for proxies and checks.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#customizing-attribute-access", locator: "3.3.2. Customizing attribute access"}
see_also: [python/special-methods, python/instance-objects, python/descriptors, python/properties]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#customizing-attribute-access
    title: The Python Language Reference, 3. Data model
    locator: 3.3.2. Customizing attribute access; 3.3.2.1. Customizing module attribute access
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#hasattr
    title: Built-in Functions, Python 3.15 documentation
    locator: hasattr
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Selectors
    title: The Go Programming Language Specification
    locator: Selectors
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Settings:
    def __init__(self, **values):
        object.__setattr__(self, "_values", dict(values))

    def __getattr__(self, name):
        try:
            return self._values[name]
        except KeyError:
            raise AttributeError(name) from None

    def __setattr__(self, name, value):
        if name not in self._values:
            raise AttributeError(f"unknown setting {name!r}")
        self._values[name] = value

    def __dir__(self):
        return list(self._values)

s = Settings(debug=False, level=1)
s.level = 3
print(s.debug, s.level, dir(s))
try:
    s.levle = 4
except AttributeError as e:
    print(e)
print(hasattr(s, "color"))
```

```output for=basics
False 3 ['debug', 'level']
unknown setting 'levle'
False
```

`s.debug` is not a normal attribute, so `__getattr__` gives it. Each assignment goes through `__setattr__`, which rejects an unknown name.

## Semantics

**r1.** These methods "customize the meaning of attribute access (use of, assignment to, or deletion of x.name) for class instances."[^1]

**r2.** `__getattr__` is "Called when the default attribute access fails with an AttributeError".[^1] "This method should either return the (computed) attribute value or raise an AttributeError exception."[^1] "Note that if the attribute is found through the normal mechanism, __getattr__() is not called. (This is an intentional asymmetry between __getattr__() and __setattr__().)"[^1] Example: [`delegate`](#delegate).

**r3.** `__getattribute__` is "Called unconditionally to implement attribute accesses for instances of the class."[^1] "In order to avoid infinite recursion in this method, its implementation should always call the base class method with the same name to access any attributes it needs, for example, object.__getattribute__(self, name)."[^1] Example: [`every-read`](#every-read).

**r4.** `__setattr__` is "Called when an attribute assignment is attempted. This is called instead of the normal mechanism (i.e. store the value in the instance dictionary)."[^1] "If __setattr__() wants to assign to an instance attribute, it should call the base class method with the same name, for example, object.__setattr__(self, name, value)."[^1] Example: [`recursion`](#recursion).

**r5.** `__delattr__` is "Like __setattr__() but for attribute deletion instead of assignment."[^1] `__dir__` is "Called when dir() is called on the object. An iterable must be returned. dir() converts the returned iterable to a list and sorts it."[^1]

**r6.** A module can define `__getattr__` too: "If an attribute is not found on a module object through the normal lookup, i.e. object.__getattribute__(), then __getattr__ is searched in the module __dict__ before raising an AttributeError. If found, it is called with the attribute name and the result is returned."[^1] Example: [`module`](#module).

**r7.** hasattr() "is implemented by calling getattr(object, name) and seeing whether it raises an AttributeError or not."[^2] Example: [`lenient`](#lenient).

## Examples

```python run id=delegate
class Logged:
    def __init__(self, target):
        self._target = target
        self.calls = []

    def __getattr__(self, name):
        self.calls.append(name)
        return getattr(self._target, name)

items = Logged([3, 1, 2])
items.append(0)
items.sort()
print(items._target, items.calls)
```

```output for=delegate
[0, 1, 2, 3] ['append', 'sort']
```

`append` and `sort` are not attributes of Logged, so `__getattr__` passes them on to the list. `calls` and `_target` are found the normal way, so they are not logged (rule r2).

```python run id=every-read
class Traced:
    def __init__(self):
        self.x = 1

    def __getattribute__(self, name):
        print("get", name)
        return object.__getattribute__(self, name)

t = Traced()
print(t.x + t.x)
```

```output for=every-read
get x
get x
2
```

```python run id=module
import types

mod = types.ModuleType("config")
mod.known = 1

def __getattr__(name):
    if name.startswith("legacy_"):
        return f"<{name} is deprecated>"
    raise AttributeError(name)

mod.__getattr__ = __getattr__
print(mod.known, mod.legacy_mode)
```

```output for=module
1 <legacy_mode is deprecated>
```

The function is in the module dictionary, so Python calls it for a name that the module does not have (rule r6). In a file, define `__getattr__` at the top level of the module.

## Pitfalls

An assignment to `self.name` inside `__setattr__` calls `__setattr__` again, without end:

```python fails id=recursion expect=exception:RecursionError
class Loud:
    def __setattr__(self, name, value):
        self.name = value

Loud().x = 1
```

Call `object.__setattr__(self, name, value)` instead (rule r4).

A `__getattr__` that never raises AttributeError makes every name look present:

```python wrong id=lenient
class Lenient:
    def __getattr__(self, name):
        return None

obj = Lenient()
print(hasattr(obj, "anything"), obj.colour)
```

```output for=lenient
True None
```

hasattr() depends on the AttributeError (rule r7), and a misspelt name gives None, not an error. Raise AttributeError for a name that the class does not know (rule r2).

## Comparison

Go has no attribute hooks. The type of x decides each selector: "x.f denotes the field or method at the shallowest depth in T where there is such an f."[^3] "In all other cases, x.f is illegal."[^3] In Python the lookup happens at run time, and a class can change it (rule r1).
