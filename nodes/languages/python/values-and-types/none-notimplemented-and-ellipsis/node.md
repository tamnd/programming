---
tag: 2EEANS
kind: feature
owner: python
slug: none-notimplemented-and-ellipsis
name: None, NotImplemented and Ellipsis
aliases: [Python None, NoneType, NotImplemented constant, Ellipsis, three dots literal, Python singletons]
area: languages/python/values-and-types
level: intro
summary: >-
  None, NotImplemented and Ellipsis are the only values of their types. None marks an absent value, NotImplemented is returned by operator methods that give up, and ... marks an omission.
grammar: "atom: ... | 'None' | ... | '...' (Language Reference, 6.2)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#none", locator: "3.2.1 None, 3.2.2 NotImplemented, 3.2.3 Ellipsis"}
see_also: [python/the-standard-type-hierarchy, python/truth-value-testing, null-references, python/keywords]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#none
    title: The Python Language Reference, Python 3.15
    locator: 3.2.1. None, 3.2.2. NotImplemented, 3.2.3. Ellipsis
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/constants.html
    title: Built-in Constants, Python 3.15 documentation
    locator: None, NotImplemented, Ellipsis
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__eq__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.1. Basic customization, object.__eq__()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#The_zero_value
    title: The Go Programming Language Specification
    locator: The zero value
    accessed: 2026-11-16
---

## Synopsis

```python run id=three-singletons
def nothing():
    pass

print(nothing() is None, type(None).__name__)
print(None is type(None)(), Ellipsis is ..., type(...).__name__)
print(bool(None), bool(...))
```

```output for=three-singletons
True NoneType
True True ellipsis
False True
```

A function without a `return` gives `None`. Each of the three types has one object, and calling the type gives that object again.

## Semantics

**r1.** Each of the three types "has a single value. There is a single object with this value."[^1] So `x is None` is the test for `None`. Example: [`three-singletons`](#three-singletons).

**r2.** `None` signifies "the absence of a value in many situations", for example the result of a function that does not return anything explicitly. Its truth value is false.[^1] It is often used for a default argument that the caller did not pass, and an assignment to `None` raises a `SyntaxError`.[^2]

**r3.** Numeric methods and rich comparison methods should return `NotImplemented` when they do not implement the operation for the given operands. The interpreter then tries the reflected operation, or another fallback that depends on the operator.[^1] If all attempts return `NotImplemented`, the interpreter raises an exception.[^2] Example: [`not-implemented`](#not-implemented).

**r4.** `NotImplemented` should not be used in a Boolean context. Such a use was deprecated first, and Python 3.14 made it raise a `TypeError`.[^1]

**r5.** `Ellipsis` is "accessed through the literal ... or the built-in name Ellipsis", and its truth value is true.[^1] It is "frequently used to indicate that something is omitted". The name `Ellipsis` can be assigned to, but an assignment to `...` raises a `SyntaxError`.[^2] Example: [`placeholder-body`](#placeholder-body).

## Examples

`__add__` returns `NotImplemented` for an operand that it does not know. For `0 + Meters(1)`, the first call made by `sum()`, `int.__add__` gives `NotImplemented` and Python calls `Meters.__radd__` (rule r3). For `Meters(1) + "x"`, both sides give up, so Python raises `TypeError`.

```python run id=not-implemented
class Meters:
    def __init__(self, v):
        self.v = v
    def __add__(self, other):
        if not isinstance(other, Meters):
            return NotImplemented
        return Meters(self.v + other.v)
    def __radd__(self, other):
        if other == 0:
            return self
        return NotImplemented

print(sum([Meters(1), Meters(2)]).v)
try:
    Meters(1) + "x"
except TypeError as e:
    print("TypeError:", e)
```

```output for=not-implemented
3
TypeError: unsupported operand type(s) for +: 'Meters' and 'str'
```

`...` is a valid body for a function that is not written yet (rule r5). The function returns `None`.

```python run id=placeholder-body
def todo():
    ...

print(todo(), ... is Ellipsis)
```

```output for=placeholder-body
None True
```

## Pitfalls

### `if not x` for `None`

`0`, `""` and `[]` are false too, so `not x` does not tell `None` apart from them (rule r2). Use `x is None`.

```python wrong id=falsy-not-none
v = 0
print(v is None, not v)
```

```output for=falsy-not-none
False True
```

### Raising `NotImplemented`

`NotImplemented` and the exception `NotImplementedError` "are not interchangeable".[^2] Return `NotImplemented` from an operator method. Raise `NotImplementedError` from a method that a subclass must override.

### The fallback of `==`

If `__eq__` returns `NotImplemented` on both sides, `==` does not fail. It falls back to identity. By default, `object` implements `__eq__()` "by using is".[^3]

```python wrong id=eq-fallback
class C:
    def __eq__(self, other):
        return NotImplemented

c = C()
print(C() == C(), c == c)
```

```output for=eq-fallback
False True
```

## Comparison

Go has no single object for a missing value. A variable without an initial value gets the zero value of its type: "false for booleans, 0 for numeric types, "" for strings, and nil for pointers, functions, interfaces, slices, channels, and maps".[^4] An `int` in Go cannot be `nil`. In Python, any name can refer to `None` (rule r2).
