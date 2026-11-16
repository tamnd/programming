---
tag: 7TK3MK
kind: feature
owner: python
slug: tuples-and-sequences
name: Tuples and sequences
aliases: [tuple, tuples, tuple packing, sequence unpacking, singleton tuple, empty tuple, immutable sequence, multiple assignment]
area: languages/python/collections
level: intro
summary: >-
  A tuple is an immutable sequence of values separated by commas. Tuple packing puts values into a tuple, and sequence unpacking takes them out into names. A one-item tuple needs a trailing comma.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#tuples", locator: "Tuples"}
see_also: [python/sequence-types, python/lists, python/container-displays, python/hashability, python/the-collections-module]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, 5. Data Structures
    locator: 5.3. Tuples and Sequences
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#tuples
    title: Built-in Types, Python 3.15 documentation
    locator: Tuples
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Assignment_statements
    title: The Go Programming Language Specification
    locator: Assignment statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=pack
t = 12345, 54321, "hello!"
print(t[0], t)
x, y, z = t
print(z)
u = t, (1, 2, 3)
print(u)
```

```output for=pack
12345 (12345, 54321, 'hello!')
hello!
((12345, 54321, 'hello!'), (1, 2, 3))
```

The commas make the tuple. Unpacking puts each item into its own name.

## Semantics

**r1.** "A tuple consists of a number of values separated by commas".[^1] "Note that it is actually the comma which makes a tuple, not the parentheses. The parentheses are optional, except in the empty tuple case, or when they are needed to avoid syntactic ambiguity."[^2]

**r2.** "Tuples are immutable sequences, typically used to store collections of heterogeneous data (such as the 2-tuples produced by the enumerate() built-in)."[^2] "It is not possible to assign to the individual items of a tuple, however it is possible to create tuples which contain mutable objects, such as lists."[^1] Examples: [`assign-item`](#assign-item), [`mutable-inside`](#mutable-inside).

**r3.** "Empty tuples are constructed by an empty pair of parentheses; a tuple with one item is constructed by following a value with a comma (it is not sufficient to enclose a single value in parentheses)."[^1] Example: [`singleton`](#singleton).

**r4.** "Sequence unpacking requires that there are as many variables on the left side of the equals sign as there are elements in the sequence. Note that multiple assignment is really just a combination of tuple packing and sequence unpacking."[^1] Examples: [`swap`](#swap), [`too-many`](#too-many).

**r5.** "Tuples implement all of the common sequence operations."[^2] The constructor `tuple(iterable)` "builds a tuple whose items are the same and in the same order as iterable’s items."[^2]

**r6.** "Tuples are immutable, and usually contain a heterogeneous sequence of elements that are accessed via unpacking (see later in this section) or indexing (or even by attribute in the case of namedtuples). Lists are mutable, and their elements are usually homogeneous and are accessed by iterating over the list."[^1]

## Examples

```python run id=singleton
empty = ()
singleton = "hello",
print(len(empty), len(singleton), singleton)
print(tuple("abc"), tuple([1, 2, 3]))
```

```output for=singleton
0 1 ('hello',)
('a', 'b', 'c') (1, 2, 3)
```

```python run id=swap
a, b = 1, 2
a, b = b, a
print(a, b)
for index, name in enumerate(["ann", "bob"]):
    print(index, name)
```

```output for=swap
2 1
0 ann
1 bob
```

The right side is packed into a tuple first, and then unpacked into `a` and `b` (rule r4).

```python run id=mutable-inside
v = ([1, 2, 3], [3, 2, 1])
v[0].append(4)
print(v)
```

```output for=mutable-inside
([1, 2, 3, 4], [3, 2, 1])
```

The tuple still holds the same two lists. One of the lists changed (rule r2).

## Pitfalls

A tuple item cannot be replaced:

```python fails id=assign-item expect=exception:TypeError
t = (1, 2, 3)
t[0] = 88888
```

Make a new tuple instead, for example `(88888,) + t[1:]`.

The number of names must match the number of items:

```python fails id=too-many expect=exception:ValueError
x, y = (1, 2, 3)
```

Use a starred name such as `x, *rest = (1, 2, 3)` to collect the extra items.

## Comparison

Go has no tuple type. A Go function can return many values, and a Go assignment can unpack them: "A tuple assignment assigns the individual elements of a multi-valued operation to a list of variables."[^3] The values cannot be stored as one value. In Python a tuple is an object that a program can store, pass and index (rule r5).
