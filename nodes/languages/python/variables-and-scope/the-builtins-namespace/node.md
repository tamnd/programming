---
tag: C3PKYC
kind: feature
owner: python
slug: the-builtins-namespace
name: The builtins namespace
aliases: [builtins module, __builtins__, built-in names in Python, shadowing a built-in, a variable named list breaks list(), override a built-in function]
area: languages/python/variables-and-scope
level: core
summary: >-
  Built-in names such as len, print and ValueError live in the builtins module. Python searches this namespace last, so a global or local name with the same name hides the built-in.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/builtins.html", locator: "builtins - Built-in objects"}
see_also: [python/python-scopes-and-namespaces, python/name-resolution, python/global-statement, python/scope-and-dynamic-features]
sources:
  - url: https://docs.python.org/3.15/library/builtins.html
    title: builtins - Built-in objects, Python 3.15 documentation
    locator: whole page
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#builtins-and-restricted-execution
    title: The Python Language Reference, Python 3.15
    locator: 4.2.2. Resolution of names; 4.2.5. Builtins and restricted execution
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#python-scopes-and-namespaces
    title: The Python Tutorial, 9. Classes
    locator: 9.2. Python Scopes and Namespaces
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Predeclared_identifiers
    title: The Go Programming Language Specification
    locator: Declarations and scope; Predeclared identifiers
    accessed: 2026-11-16
---

## Synopsis

```python run id=builtins-module
import builtins

print(builtins.len is len, builtins.ValueError is ValueError)
print("open" in dir(builtins), type(builtins.__dict__).__name__)
```

```output for=builtins-module
True True
True dict
```

`len` and `builtins.len` are the same object. The module `builtins` holds each built-in name.

## Semantics

**r1.** "This module provides direct access to all ‘built-in’ identifiers of Python; for example, builtins.open is the full name for the built-in function open()."[^1] Example: [`builtins-module`](#builtins-module).

**r2.** The builtins namespace is the last namespace that Python searches. The tutorial calls it "the outermost scope (searched last)".[^3] The reference says that the global namespace "is searched first. If the names are not found there, the builtins namespace is searched next."[^2] Example: [`shadow`](#shadow).

**r3.** "The namespace containing the built-in names is created when the Python interpreter starts up, and is never deleted." "The built-in names actually also live in a module; this is called builtins."[^3]

**r4.** The module "can be useful in modules that provide objects with the same name as a built-in value, but in which the built-in of that name is also needed."[^1] Example: [`wrap-builtin`](#wrap-builtin).

**r5.** "most modules have the name __builtins__ made available as part of their globals". This is "an implementation detail".[^1] The reference adds: "Users should not touch __builtins__; it is strictly an implementation detail. Users wanting to override values in the builtins namespace should import the builtins module and modify its attributes appropriately."[^2] Example: [`add-builtin`](#add-builtin).

## Examples

```python run id=shadow
def show():
    print = "not a function"
    return type(print).__name__

print(show())
print(len("abc"))
len = lambda s: -1
print(len("abc"))
del len
print(len("abc"))
```

```output for=shadow
str
3
-1
3
```

The local `print` in `show` and the global `len` hide the built-ins. After `del len`, the global name is gone and Python finds the built-in again (rule r2).

```python run id=wrap-builtin
import builtins

def print(*args):
    builtins.print("[log]", *args)

print("started")
```

```output for=wrap-builtin
[log] started
```

The module defines its own `print`, and it calls the built-in through `builtins.print` (rule r4).

```python run id=add-builtin
import builtins

builtins.APP_NAME = "demo"

def name():
    return APP_NAME

print(name())
del builtins.APP_NAME
```

```output for=add-builtin
demo
```

A new attribute of `builtins` is visible in each module. The reference names the builtins module as the way to change built-in values (rule r5).

## Pitfalls

A variable with the name of a built-in hides the built-in in the rest of its scope:

```python fails id=shadowed-list expect=exception:TypeError
list = [1, 2, 3]
copy = list("abc")
```

`list` is now a list object, and a list object cannot be called. Pick another name, such as `items`, or delete the variable with `del list`.

## Comparison

Go has a similar outermost block: "The scope of a predeclared identifier is the universe block." The predeclared identifiers include types such as `int` and `string`, constants such as `true` and `false`, and functions.[^4] As in Python, a declaration in an inner block can hide a predeclared identifier: "An identifier declared in a block may be redeclared in an inner block."[^4] In Python, the built-in names also live in a module, `builtins`, and a program can read and change its attributes (rules r1 and r5).
