---
tag: 77SRT9
kind: feature
owner: python
slug: objects-values-and-types
name: Objects, values and types
aliases: [Python object model, identity type and value, id(), type(), mutable and immutable objects, is operator]
area: languages/python/values-and-types
level: core
summary: >-
  All data in Python is an object. Each object has an identity, a type and a value. The identity and the type never change; the value of a mutable object can change.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types", locator: "3.1. Objects, values and types"}
see_also: [python/the-standard-type-hierarchy, python/comparison-operators, immutability]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Python 3.15
    locator: 3.1. Objects, values and types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#id
    title: Built-in Functions, Python 3.15 documentation
    locator: id(), type()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Variables
    title: The Go Programming Language Specification
    locator: Variables
    accessed: 2026-11-16
---

## Synopsis

```python run id=three-parts
a = [1, 2]
b = a
c = [1, 2]
print(a is b, a is c, a == c)
print(id(a) == id(b), type(a).__name__, type(type(a)).__name__)
b.append(3)
print(a)
```

```output for=three-parts
True False True
True list type
[1, 2, 3]
```

`a` and `b` are two names for one object, and `c` is another object with an equal value. A change through `b` is seen through `a`.

## Semantics

**r1.** "Objects are Python's abstraction for data." All data in a program is represented by objects or by relations between objects, and code is an object too.[^1]

**r2.** "Every object has an identity, a type and a value." The identity never changes after the object is created. The `is` operator compares identities, and `id()` returns an integer for the identity.[^1] The `id()` value is unique and constant for the object during its lifetime, and two objects whose lifetimes do not overlap can have the same `id()` value.[^2] In CPython, `id(x)` is the memory address of `x`.[^1] Example: [`three-parts`](#three-parts).

**r3.** The type of an object decides the operations that it supports and its possible values. `type()` returns the type, which is an object too. The type of an object cannot change.[^1]

**r4.** Objects whose value can change are mutable, and the others are immutable. Mutability comes from the type: numbers, strings and tuples are immutable, and dictionaries and lists are mutable.[^1] Example: [`rebind-or-mutate`](#rebind-or-mutate).

**r5.** An immutable container can hold a reference to a mutable object. When that object changes, the value of the container changes, but the container stays immutable, "because the collection of objects it contains cannot be changed".[^1] Example: [`tuple-with-list`](#tuple-with-list).

**r6.** For immutable types, an operation may return any existing object with the same type and value. After `a = 1; b = 1`, `a` and `b` may or may not be the same object. After `c = []; d = []`, `c` and `d` are always two different new lists, and `e = f = []` gives both names the same list.[^1] Example: [`new-objects`](#new-objects).

**r7.** Objects are never destroyed explicitly. An object that cannot be reached may be garbage-collected, and the implementation can postpone collection or omit it. Objects that hold external resources such as open files give an explicit way to release them, usually a `close()` method.[^1]

## Examples

`+=` on a string makes a new object, because strings are immutable. `+=` on a list changes the same object (rule r4).

```python run id=rebind-or-mutate
s = "abc"
before = id(s)
s += "d"
print(s, id(s) == before)
lst = [1]
before = id(lst)
lst += [2]
print(lst, id(lst) == before)
```

```output for=rebind-or-mutate
abcd False
[1, 2] True
```

A tuple cannot get a new item, but the list inside it can change (rule r5).

```python run id=tuple-with-list
t = ([1], 2)
t[0].append(9)
print(t)
try:
    t[1] = 3
except TypeError as e:
    print("TypeError:", e)
```

```output for=tuple-with-list
([1, 9], 2)
TypeError: 'tuple' object does not support item assignment
```

## Pitfalls

### `e = f = []` is one list

Two empty list displays make two lists, but a chained assignment binds both names to one list (rule r6).

```python wrong id=new-objects
c = []; d = []
e = f = []
e.append("x")
print(c is d, e is f, f)
```

```output for=new-objects
False True ['x']
```

### `is` for values

Use `==` to compare values. Whether two equal immutable values are one object depends on the implementation (rule r6), so `is` with numbers or strings gives results that can change between implementations and versions.

## Comparison

In Go, a variable "is a storage location for holding a value". The spec adds: "The set of permissible values is determined by the variable's type."[^3] The type belongs to the variable. In Python, a name has no type: it refers to an object, and the object carries the type (rule r3). Assignment binds the name to the object, so two names can share one object (see `three-parts`).
