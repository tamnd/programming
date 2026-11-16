---
tag: E82DES
kind: feature
owner: python
slug: constants-by-convention
name: Constants by convention
aliases: [Python constants, UPPER_CASE names, typing.Final, const in Python, how to declare a constant in Python, PEP 8 constants]
area: languages/python/variables-and-scope
level: intro
summary: >-
  Python has no constant declaration. By convention, a module-level name in capital letters, such as MAX_SIZE, is a constant. typing.Final informs type checkers, but Python allows a new assignment.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0008/#constants", locator: "PEP 8, Constants"}
see_also: [python/annotated-assignment, python/global-statement, python/a-word-about-names-and-objects, python/the-builtins-namespace]
sources:
  - url: https://peps.python.org/pep-0008/#constants
    title: PEP 8 - Style Guide for Python Code
    locator: Naming Conventions, Constants
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/typing.html#typing.Final
    title: typing - Support for type hints, Python 3.15 documentation
    locator: typing.Final
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/constants.html
    title: Built-in Constants, Python 3.15 documentation
    locator: Note after __debug__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Python 3.15
    locator: 3.1. Objects, values and types
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Constant_declarations
    title: The Go Programming Language Specification
    locator: Constant declarations; Assignment statements; Address operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=convention
MAX_OVERFLOW = 100
TOTAL = 0

def overflow(n):
    return n > MAX_OVERFLOW

print(overflow(150), MAX_OVERFLOW)
```

```output for=convention
True 100
```

The capital letters tell the reader that `MAX_OVERFLOW` must not change. Python treats it as an ordinary global variable.

## Semantics

**r1.** PEP 8 says: "Constants are usually defined on a module level and written in all capital letters with underscores separating words. Examples include MAX_OVERFLOW and TOTAL."[^1] Example: [`convention`](#convention).

**r2.** The name is only a convention. A constant name is a normal name, and an assignment can rebind it. Example: [`rebind`](#rebind).

**r3.** `typing.Final` is a "Special typing construct to indicate final names to type checkers. Final names cannot be reassigned in any scope." But "There is no runtime checking of these properties."[^2] Example: [`final-not-checked`](#final-not-checked).

**r4.** A few built-in names are real constants: "The names None, False, True and __debug__ cannot be reassigned (assignments to them, even as an attribute name, raise SyntaxError), so they can be considered “true” constants."[^3] Example: [`true-constant`](#true-constant).

**r5.** A constant name does not make its object immutable. "Objects whose value can change are said to be mutable; objects whose value is unchangeable once they are created are called immutable."[^4] Use an immutable object, such as a tuple or a frozenset, for a constant collection. Example: [`mutable-constant`](#mutable-constant).

## Examples

```python run id=rebind
MAX_SIZE = 10
MAX_SIZE = 20
print(MAX_SIZE)
```

```output for=rebind
20
```

```python run id=final-not-checked
from typing import Final

MAX_SIZE: Final = 9000
MAX_SIZE += 1
print(MAX_SIZE)
```

```output for=final-not-checked
9001
```

A type checker reports an error for `MAX_SIZE += 1`, but Python runs it (rule r3).

```python fails id=true-constant expect=exception:SyntaxError
None = 0
```

## Pitfalls

A list in a constant name can still change:

```python wrong id=mutable-constant
COLORS = ["red", "green"]
COLORS.append("blue")
print(COLORS)
FROZEN = ("red", "green")
print(hasattr(FROZEN, "append"))
```

```output for=mutable-constant
['red', 'green', 'blue']
False
```

The name `COLORS` still refers to the same list, but the list has a new item. A tuple has no `append` method (rule r5).

A class attribute in capital letters is also only a convention. Mark it with `Final` if you use a type checker. The typing documentation says that "Final names declared in class scopes cannot be overridden in subclasses", as a rule for type checkers.[^2]

## Comparison

Go has constant declarations: "A constant declaration binds a list of identifiers (the names of the constants) to the values of a list of constant expressions."[^5] An assignment needs an addressable left operand: "Each left-hand side operand must be addressable, a map index expression, or (for = assignments only) the blank identifier." "The operand must be addressable, that is, either a variable, pointer indirection, or slice indexing operation", or a field or element of an addressable value.[^5] A constant is none of these, so a Go compiler rejects an assignment to a constant. Python has no such check. The capital letters are only a message to the reader (rules r1 and r2).
