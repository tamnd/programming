---
tag: J34F9H
kind: feature
owner: python
slug: sequence-types
name: Sequence types
aliases: [sequence, common sequence operations, list tuple range, mutable sequence, immutable sequence, in operator on sequences, sequence repetition]
area: languages/python/collections
level: core
summary: >-
  list, tuple and range are the basic sequence types. They share operations such as in, +, *, indexing, slicing, len, min, max, count and index. Mutable sequences also support item and slice assignment.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#sequence-types-list-tuple-range", locator: "Sequence Types: list, tuple, range"}
see_also: [python/lists, python/tuples-and-sequences, python/the-range-function, python/slicing, python/comparing-sequences-and-other-types]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#sequence-types-list-tuple-range
    title: Built-in Types, Python 3.15 documentation
    locator: Sequence Types; Common Sequence Operations; Immutable Sequence Types; Mutable Sequence Types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, 5. Data Structures
    locator: 5.3. Tuples and Sequences
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Array_types
    title: The Go Programming Language Specification
    locator: Array types; Slice types
    accessed: 2026-11-16
---

## Synopsis

```python run id=common
for s in ([3, 1, 2], (3, 1, 2), range(1, 4), "312"):
    print(type(s).__name__, len(s), min(s), max(s), s[0], s.index(s[-1]))
```

```output for=common
list 3 1 3 3 2
tuple 3 1 3 3 2
range 3 1 3 1 2
str 3 1 3 3 2
```

The same operations work on a list, a tuple, a range and a string.

## Semantics

**r1.** "There are three basic sequence types: lists, tuples, and range objects. Additional sequence types tailored for processing of binary data and text strings are described in dedicated sections."[^1]

**r2.** "The operations in the following table are supported by most sequence types, both mutable and immutable."[^1] They are `x in s`, `x not in s`, `s + t`, `s * n`, `s[i]`, `s[i:j]`, `s[i:j:k]`, `len(s)`, `min(s)`, `max(s)`, and the methods `count` and `index`. Example: [`operations`](#operations).

**r3.** "Values of n less than 0 are treated as 0 (which yields an empty sequence of the same type as s). Note that items in the sequence s are not copied; they are referenced multiple times."[^1] Example: [`repeat-lists`](#repeat-lists).

**r4.** "If i or j is negative, the index is relative to the end of sequence s: len(s) + i or len(s) + j is substituted. But note that -0 is still 0."[^1] "An IndexError is raised if i is outside the sequence range."[^1] Example: [`out-of-range`](#out-of-range).

**r5.** "The only operation that immutable sequence types generally implement that is not also implemented by mutable sequence types is support for the hash() built-in."[^1] "This support allows immutable sequences, such as tuple instances, to be used as dict keys and stored in set and frozenset instances."[^1]

**r6.** Mutable sequence types also support `s[i] = x`, `del s[i]`, `s[i:j] = t`, `del s[i:j]`, `s += t` and `s *= n`.[^1] Example: [`mutable`](#mutable).

**r7.** "Concatenating immutable sequences always results in a new object. This means that building up a sequence by repeated concatenation will have a quadratic runtime cost in the total sequence length."[^1]

## Examples

```python run id=operations
s = [10, 20, 30, 20]
t = (1, 2)
print(s + [40], t * 2)
print(s[-1], s[1:3], s[::2])
print(s.count(20), s.index(20), s.index(20, 2))
print(30 not in s, "gg" in "eggs")
```

```output for=operations
[10, 20, 30, 20, 40] (1, 2, 1, 2)
20 [20, 30] [10, 30]
2 1 3
False True
```

For `str`, `in` also tests for a substring, so `"gg" in "eggs"` is true.

```python run id=mutable
nums = [1, 2, 3, 4, 5]
nums[0] = 100
del nums[1]
nums[1:3] = ["a", "b", "c"]
nums += [6]
print(nums)
```

```output for=mutable
[100, 'a', 'b', 'c', 5, 6]
```

A slice assignment can change the length of the list (rule r6).

```python run id=hashable
point = (2, 3)
seen = {point: "start"}
print(seen[(2, 3)], hash((1, 2)) == hash((1, 2)))
```

```output for=hashable
start True
```

A tuple of hashable items can be a dict key (rule r5).

## Pitfalls

Repetition copies references, not items:

```python wrong id=repeat-lists
rows = [[]] * 3
rows[0].append(3)
print(rows)
```

```output for=repeat-lists
[[3], [3], [3]]
```

The three items are one list (rule r3). Use `[[] for i in range(3)]` to get three lists.

An index past the end is an error:

```python fails id=out-of-range expect=exception:IndexError
letters = ["a", "b", "c"]
letters[3]
```

## Comparison

Go has arrays and slices. "An array is a numbered sequence of elements of a single type, called the element type. The number of elements is called the length of the array and is never negative."[^3] A Go array has a fixed length that is part of its type. A Python list, tuple or range can have any length, and all three share the operations of rule r2.
