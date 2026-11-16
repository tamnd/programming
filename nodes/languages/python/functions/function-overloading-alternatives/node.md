---
tag: EAK62T
kind: feature
owner: python
slug: function-overloading-alternatives
name: Function overloading and its alternatives
aliases: [function overloading, overload by type, singledispatch, typing.overload, generic function, same function name different arguments, method overloading in Python]
area: languages/python/functions
level: core
summary: >-
  Python has no overloading by signature. A second def with the same name replaces the first. Use default values, *args, functools.singledispatch, or typing.overload for type checkers only.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/functools.html#functools.singledispatch", locator: "functools.singledispatch"}
see_also: [python/functools, python/default-argument-values, python/arbitrary-argument-lists, python/defining-functions, python/class-patterns]
sources:
  - url: https://docs.python.org/3.15/library/functools.html#functools.singledispatch
    title: functools, Python 3.15 documentation
    locator: functools.singledispatch
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0443/
    title: "PEP 443: Single-dispatch generic functions"
    locator: Abstract; Rationale and Goals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/typing.html#typing.overload
    title: typing, Python 3.15 documentation
    locator: typing.overload
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#overloading
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not support overloading of methods and operators?
    accessed: 2026-11-16
---

## Synopsis

```python run id=singledispatch
from functools import singledispatch

@singledispatch
def describe(arg):
    return f"something: {arg!r}"

@describe.register
def _(arg: int):
    return f"an int: {arg}"

@describe.register
def _(arg: list):
    return f"a list of {len(arg)} items"

print(describe(7))
print(describe([1, 2]))
print(describe("x"))
```

```output for=singledispatch
an int: 7
a list of 2 items
something: 'x'
```

`describe` picks an implementation from the type of its first argument. The first function is the fallback.

## Semantics

**r1.** A def binds the function name: "Its execution binds the function name in the current local namespace to a function object".[^4] So a second def with the same name binds the name again, and the first function is lost. Example: [`replaced`](#replaced).

**r2.** "A generic function is composed of multiple functions implementing the same operation for different types. Which implementation should be used during a call is determined by the dispatch algorithm. When the implementation is chosen based on the type of a single argument, this is known as single dispatch."[^2]

**r3.** "When defining a function using @singledispatch, note that the dispatch happens on the type of the first argument".[^1] "To add overloaded implementations to the function, use the register() attribute of the generic function, which can be used as a decorator."[^1] Example: [`singledispatch`](#singledispatch).

**r4.** "For code which doesn’t use type annotations, the appropriate type argument can be passed explicitly to the decorator itself".[^1] Example: [`explicit-type`](#explicit-type).

**r5.** "@overload-decorated definitions are for the benefit of the type checker only, since they will be overwritten by the non-@overload-decorated definition."[^3] Example: [`typing-overload`](#typing-overload).

**r6.** PEP 443 calls type tests a problem: "it is currently a common anti-pattern for Python code to inspect the types of received arguments, in order to decide what to do with the objects."[^2]

## Examples

```python run id=explicit-type
from functools import singledispatch

@singledispatch
def area(shape):
    raise TypeError("unknown shape")

@area.register(tuple)
def _(shape):
    width, height = shape
    return width * height

@area.register(float)
@area.register(int)
def _(shape):
    return shape * shape

print(area((2, 3)), area(4), area(1.5))
```

```output for=explicit-type
6 16 2.25
```

The type goes to `register()`, so the functions need no annotations (rule r4).

```python run id=typing-overload
from typing import overload

@overload
def double(x: int) -> int: ...
@overload
def double(x: str) -> str: ...
def double(x):
    return x * 2

print(double(4), double("ab"))
```

```output for=typing-overload
8 abab
```

Only the last def runs. The `@overload` lines tell a type checker which argument types go with which result types (rule r5).

```python run id=defaults
def greet(name, times=1, *extra):
    line = " ".join([f"hi {name}"] * times)
    return line + "".join(f" and {e}" for e in extra)

print(greet("ann"))
print(greet("ann", 2))
print(greet("ann", 1, "bob", "cy"))
```

```output for=defaults
hi ann
hi ann hi ann
hi ann and bob and cy
```

Default values and `*args` let one def take several argument lists.

## Pitfalls

A second def with the same name replaces the first, so the old call form fails:

```python fails id=replaced expect=exception:TypeError
def area(radius):
    return 3.14159 * radius * radius

def area(width, height):
    return width * height

area(2)
```

A singledispatch function uses only the first argument for the choice. A second argument of a different type does not select another implementation (rule r3).

## Comparison

Go also has no overloading. The Go FAQ says: "Experience with other languages told us that having a variety of methods with the same name but different signatures was occasionally useful but that it could also be confusing and fragile in practice."[^5] A Go program gives each function its own name, or it uses a type switch on an interface value. A Python program can use singledispatch to choose a function by type (rule r3).
