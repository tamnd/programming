---
tag: X0SY5P
kind: feature
owner: python
slug: dynamic-attribute-access
name: Dynamic attribute access
aliases: [getattr, setattr, hasattr, delattr, vars, __dict__, attribute by name, reflection]
area: languages/python/metaprogramming
level: core
summary: >-
  getattr, setattr, hasattr and delattr read, write, test and delete an attribute whose name is a string made at run time. vars(obj) returns the __dict__ that holds the attributes of the object.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#getattr", locator: "Built-in Functions: delattr(), getattr(), hasattr(), setattr(), vars()"}
proposals: []
see_also: [runtime-reflection, python/the-dir-function, python/attribute-access-customization, python/slots, python/private-variables]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html#getattr
    title: Built-in Functions, Python 3.15 documentation
    locator: delattr(); getattr(); hasattr(); setattr(); vars()
    accessed: 2026-11-16
---

## Synopsis

```python run id=config
class Config:
    pass

c = Config()
for key, value in [("host", "localhost"), ("port", 8080)]:
    setattr(c, key, value)

print(c.host, getattr(c, "port"))
print(getattr(c, "user", "guest"))
print(hasattr(c, "port"), hasattr(c, "user"))
delattr(c, "port")
print(vars(c))
```

```output for=config
localhost 8080
guest
True False
{'host': 'localhost'}
```

## Semantics

**r1.** `getattr(x, 'foobar')` "is equivalent to x.foobar. If the named attribute does not exist, default is returned if provided, otherwise AttributeError is raised."[^1]

**r2.** `setattr(x, 'foobar', 123)` "is equivalent to x.foobar = 123." `delattr(x, 'foobar')` "is equivalent to del x.foobar."[^1]

**r3.** `hasattr()` "is implemented by calling getattr(object, name) and seeing whether it raises an AttributeError or not."[^1] So it runs the same code as a normal read, for example a property. Example: [`hidden-bug`](#hidden-bug).

**r4.** The name does not have to be an identifier. "An attribute whose name is not an identifier will not be accessible using the dot notation, but is accessible through getattr() etc.."[^1] Example: [`odd-name`](#odd-name).

**r5.** Name mangling does not apply to the string. "Since private name mangling happens at compilation time, one must manually mangle a private attribute’s (attributes with two leading underscores) name in order to retrieve it with getattr()."[^1] Example: [`mangling`](#mangling).

**r6.** `vars(obj)` returns the `__dict__` of the object. "Objects such as modules and instances have an updateable __dict__ attribute; however, other objects may have write restrictions on their __dict__ attributes (for example, classes use a types.MappingProxyType to prevent direct dictionary updates)."[^1] Example: [`dict-view`](#dict-view).

**r7.** "A TypeError exception is raised if an object is specified but it doesn’t have a __dict__ attribute (for example, if its class defines the __slots__ attribute)."[^1] Example: [`no-dict`](#no-dict).

## Examples

```python run id=dispatch
class Square:
    def __init__(self, side):
        self.side = side

    def area(self):
        return self.side ** 2

for op in ["area", "perimeter"]:
    method = getattr(Square(3), op, None)
    print(op, method() if method else "not supported")
```

```output for=dispatch
area 9
perimeter not supported
```

The name of the method comes from data. The default `None` stops the `AttributeError` (rule r1).

```python run id=odd-name
class Box:
    pass

b = Box()
setattr(b, "not a name", 1)
print(getattr(b, "not a name"))
print(vars(b))
```

```output for=odd-name
1
{'not a name': 1}
```

```python run id=mangling
class Account:
    def __init__(self):
        self.__balance = 10

a = Account()
print(hasattr(a, "__balance"))
print(getattr(a, "_Account__balance"))
```

```output for=mangling
False
10
```

```python run id=dict-view
class Config:
    pass

c = Config()
c.__dict__["color"] = "red"
print(c.color)
try:
    Config.__dict__["size"] = 1
except TypeError as e:
    print(e)
```

```output for=dict-view
red
'mappingproxy' object does not support item assignment
```

The `__dict__` of an instance is a normal dictionary. The `__dict__` of a class is a read-only view; use `setattr(Config, "size", 1)` instead (rule r6).

## Pitfalls

```python wrong id=hidden-bug
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    @property
    def fahrenheit(self):
        return self.celcius * 9 / 5 + 32

t = Temperature(20)
print(hasattr(t, "fahrenheit"))
```

```output for=hidden-bug
False
```

The property has a typo, `celcius`, so it raises `AttributeError`. `hasattr()` takes that as "no such attribute" and hides the bug (rule r3).

```python fails id=no-dict expect=exception:TypeError
class Point:
    __slots__ = ("x", "y")

print(vars(Point()))
```

An instance of a class with `__slots__` has no `__dict__`, so `vars()` fails (rule r7). `getattr()` still works for each slot.

## Comparison

In Python, an attribute name is a string at run time, and four built-in functions read and change attributes by that string (rules r1 and r2). Many statically compiled languages need a separate reflection API for the same work. See [[runtime-reflection]] for other languages, and [[python/attribute-access-customization]] for `__getattr__` and `__setattr__`, which these functions call.
