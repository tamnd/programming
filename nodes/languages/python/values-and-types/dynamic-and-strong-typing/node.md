---
tag: WJS62T
kind: feature
owner: python
slug: dynamic-and-strong-typing
name: Dynamic and strong typing
aliases: [is Python strongly typed, is Python dynamically typed, duck typing, Python type hints not enforced, Python runtime types]
area: languages/python/values-and-types
level: core
summary: >-
  In Python, types belong to objects, not to names. A name can refer to an object of any type, and type hints are not enforced. Operations do not mix unrelated types, so "1" + 1 is a TypeError.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types", locator: "3.1. Objects, values and types"}
see_also: [python/type-conversions, python/arithmetic-conversions, static-and-dynamic-typing, type-system, subtyping]
sources:
  - url: https://docs.python.org/3.15/faq/design.html#why-doesn-t-python-have-a-with-statement-for-attribute-assignments
    title: Design and History FAQ, Python 3.15 documentation
    locator: Why doesn't Python have a "with" statement for attribute assignments?
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Python 3.15
    locator: 3.1. Objects, values and types; footnote 1
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-duck-typing
    title: Glossary, Python 3.15 documentation
    locator: duck-typing; type hint
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/typing.html
    title: typing - Support for type hints, Python 3.15 documentation
    locator: Note at the top of the page
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.7. Binary arithmetic operations, the + operator
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Variables
    title: The Go Programming Language Specification
    locator: Introduction; Variables
    accessed: 2026-11-16
---

## Synopsis

```python run id=rebind
x = 1
print(type(x).__name__)
x = "one"
print(type(x).__name__)
```

```output for=rebind
int
str
```

```python fails id=no-mixing expect=exception:TypeError
print("1" + 1)
```

The name `x` refers to an `int` and then to a `str`. The object carries the type, and the name does not. But `+` does not join a string and a number.

## Semantics

**r1.** "Python uses dynamic types." The FAQ adds: "It is impossible to know in advance which attribute will be referenced at runtime."[^1] Example: [`rebind`](#rebind).

**r2.** Every object has a type. "The type() function returns an object's type (which is an object itself). Like its identity, an object's type is also unchangeable."[^2] A footnote adds: "It is possible in some cases to change an object's type, under certain controlled conditions. It generally isn't a good idea though".[^2] Examples: [`class-change`](#class-change), [`int-class-fixed`](#int-class-fixed).

**r3.** "Member attributes may be added or removed from objects on the fly."[^1] Example: [`attrs-on-the-fly`](#attrs-on-the-fly).

**r4.** Duck typing is "A programming style which does not look at an object's type to determine if it has the right interface; instead, the method or attribute is simply called or used". It "avoids tests using type() or isinstance()" and "typically employs hasattr() tests or EAFP programming".[^3] Example: [`duck`](#duck).

**r5.** "Type hints are optional and are not enforced by Python but they are useful to static type checkers."[^3] The typing module says the same: "The Python runtime does not enforce function and variable type annotations. They can be used by third party tools such as type checkers, IDEs, linters, etc."[^4] Examples: [`hints-not-enforced`](#hints-not-enforced), [`hint-is-not-a-check`](#hint-is-not-a-check).

**r6.** Operators do not convert between unrelated types. For `+`, "The arguments must either both be numbers or both be sequences of the same type."[^5] Numbers of different types are converted to a common type (see the arithmetic conversions page). A string and a number are not, so the program must convert one of them (see the type conversions page). Example: [`no-mixing`](#no-mixing).

## Examples

```python run id=duck
def double(v):
    return v * 2

print(double(3), double("ab"), double([0]))
```

```output for=duck
6 abab [0, 0]
```

`double` does not test the type of `v`. It works for each object that supports `* 2` (rule r4).

```python run id=attrs-on-the-fly
class Point:
    pass

p = Point()
p.x = 1
print(p.x)
del p.x
print(hasattr(p, "x"))
```

```output for=attrs-on-the-fly
1
False
```

```python run id=hints-not-enforced
def area(edge: float) -> str:
    return edge * edge * 6

result = area(2)
print(result, type(result).__name__)
```

```output for=hints-not-enforced
24 int
```

The hints say `float` and `str`. The call passes an `int` and gets an `int` back, and Python reports no error (rule r5).

```python run id=class-change
class A:
    pass

class B:
    pass

a = A()
a.__class__ = B
print(type(a).__name__)
```

```output for=class-change
B
```

```python fails id=int-class-fixed expect=exception:TypeError
(1).__class__ = float
```

Assignment to `__class__` works for some objects of plain classes. For built-in objects such as `1` it raises `TypeError` (rule r2).

## Pitfalls

Type hints do not check arguments at run time. A wrong argument can give a wrong result with no error:

```python wrong id=hint-is-not-a-check
def twice(n: int) -> int:
    return n + n

print(twice("5"))
```

```output for=hint-is-not-a-check
55
```

The call passes a string, so `+` joins two strings and the result is `55`, not `10`. A static type checker can find this mistake before the program runs (rule r5).

A test with `type()` or `isinstance()` rejects objects that have the right methods but a different type. The glossary recommends `hasattr()` tests or EAFP instead (rule r4).

## Comparison

The Go specification says that Go "is strongly typed and garbage-collected".[^6] In Go, "The static type (or just type) of a variable is the type given in its declaration". Variables of interface type also have a dynamic type, and "The dynamic type may vary during execution but values stored in interface variables are always assignable to the static type of the variable".[^6] So a Go variable always has a static type that the compiler checks. A Python name has no declared type, and Python does not check hints (rules r1 and r5).
