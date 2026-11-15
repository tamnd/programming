---
tag: 1D0XD6
kind: feature
owner: python
slug: truth-value-testing
name: Truth value testing
aliases: [Python truthiness, truthy and falsy, __bool__, __len__ truth value, falsy values]
area: languages/python/values-and-types
level: intro
summary: >-
  Any Python object can be tested for truth in if, while, and, or and not. None, False, numeric zeros and empty collections are false. Other objects are true unless __bool__ or __len__ says otherwise.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#truth-value-testing", locator: "Truth Value Testing"}
see_also: [python/bool-type, python/boolean-operators, python/none-notimplemented-and-ellipsis, boolean-type]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#truth-value-testing
    title: Built-in Types, Python 3.15 documentation
    locator: Truth Value Testing
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__bool__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.1. Basic customization, object.__bool__()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#If_statements
    title: The Go Programming Language Specification
    locator: If statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=falsy-values
from decimal import Decimal
from fractions import Fraction

falsy = [None, False, 0, 0.0, 0j, Decimal(0), Fraction(0, 1),
         "", (), [], {}, set(), range(0)]
print([bool(v) for v in falsy].count(False), "of", len(falsy), "are false")
print(bool(" "), bool([0]), bool(0.1), bool(float("nan")))
```

```output for=falsy-values
13 of 13 are false
True True True True
```

The first list is the list of false built-in objects in Built-in Types. A space, a list that holds a zero and NaN are all true.

## Semantics

**r1.** "Any object can be tested for truth value", for use in an `if` or `while` condition or as an operand of the Boolean operations.[^1]

**r2.** By default an object is true. It is false if its class defines a `__bool__()` method that returns `False`, or a `__len__()` method that returns zero.[^1] `__bool__()` is tried first. If it is not defined, `__len__()` is called, and a class with neither method has only true instances.[^2] Example: [`custom-truth`](#custom-truth).

**r3.** The false built-in objects are the constants `None` and `False`, the zero of any numeric type, and the empty sequences and collections.[^1] Example: [`falsy-values`](#falsy-values).

**r4.** If `__bool__()` or `__len__()` raises an exception, the exception propagates and the object has no truth value. `NotImplemented` is an example.[^1] `__bool__()` "should return False or True".[^2] Example: [`bool-must-be-bool`](#bool-must-be-bool).

**r5.** Operations and built-in functions with a Boolean result return `0` or `False` for false and `1` or `True` for true, unless stated otherwise. The exception is `or` and `and`, which "always return one of their operands".[^1] Example: [`loop-and-default`](#loop-and-default).

## Examples

`__bool__` wins over `__len__`, and a plain class is always true (rule r2).

```python run id=custom-truth
class Box:
    def __init__(self, items):
        self.items = items
    def __len__(self):
        return len(self.items)

class Flag:
    def __bool__(self):
        return False
    def __len__(self):
        return 5

class Plain:
    pass

print(bool(Box([])), bool(Box([1])), bool(Flag()), bool(Plain()))
```

```output for=custom-truth
False True False True
```

A list as a loop condition runs until the list is empty, and `or` picks the first true operand (rules r3 and r5).

```python run id=loop-and-default
queue = [3, 1, 2]
taken = []
while queue:
    taken.append(queue.pop())
print(taken)
name = "" or "guest"
print(name)
```

```output for=loop-and-default
[2, 1, 3]
guest
```

## Pitfalls

### Zero is not "missing"

`if not count` treats `0` the same as `None` (rule r3). When `0` is a valid value, test `is None`.

```python wrong id=zero-is-false
def describe(count=None):
    if not count:
        return "no count given"
    return f"count is {count}"

def describe_fixed(count=None):
    if count is None:
        return "no count given"
    return f"count is {count}"

print(describe(0), "|", describe_fixed(0))
```

```output for=zero-is-false
no count given | count is 0
```

### `__bool__` that returns an int

`bool()` checks the result type of `__bool__` (rule r4).

```python fails id=bool-must-be-bool expect=exception:TypeError
class Bad:
    def __bool__(self):
        return 1

print(bool(Bad()))
```

## Comparison

A Go `if` statement chooses a branch "according to the value of a boolean expression".[^3] A Go condition must have a Boolean type, so `if n {` with an integer `n` does not compile, and the test must be written `if n != 0 {`. Python accepts any object as a condition (rule r1).
