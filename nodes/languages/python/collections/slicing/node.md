---
tag: S9FTD7
kind: feature
owner: python
slug: slicing
name: Slicing
aliases: [slice, slice notation, start stop step, negative index, reverse a list, slice assignment, slice object, reversed slice]
area: languages/python/collections
level: core
summary: >-
  s[i:j:k] takes the items from index i up to but not including j, with step k. Any of the three can be left out, and negative values count from the end. A slice of a list is a new list.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#slicings", locator: "6.3.2.1. Slicings"}
see_also: [python/sequence-types, python/lists, python/the-range-function, python/copying-collections]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#slicings
    title: The Python Language Reference, Python 3.15
    locator: 6.3.2.1. Slicings
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#common-sequence-operations
    title: Built-in Types, Python 3.15 documentation
    locator: Common Sequence Operations, notes 3 to 5
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/introduction.html#lists
    title: The Python Tutorial, 3. An Informal Introduction to Python
    locator: 3.1.3. Lists
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#slice
    title: Built-in Functions, Python 3.15 documentation
    locator: slice()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Slice_expressions
    title: The Go Programming Language Specification
    locator: Slice expressions
    accessed: 2026-11-16
---

## Synopsis

```python run id=forms
names = ["zero", "one", "two", "three", "four", "five"]
print(names[1:3])
print(names[:3], names[3:])
print(names[::2])
print(names[-2:], names[::-1][:2])
```

```output for=forms
['one', 'two']
['zero', 'one', 'two'] ['three', 'four', 'five']
['zero', 'two', 'four']
['four', 'five'] ['five', 'four']
```

The first number is the start, the second is the stop, and the third is the step. The stop item is not in the result.

## Semantics

**r1.** "In this form, the subscript is a slice: up to three expressions separated by colons. Any of the expressions may be omitted, but a slice must contain at least one colon".[^1]

**r2.** "The slice of s from i to j is defined as the sequence of items with index k such that i <= k < j."[^2] "If i is omitted or None, use 0. If j is omitted or None, use len(s)."[^2]

**r3.** "If i or j is less than -len(s), use 0. If i or j is greater than len(s), use len(s)."[^2] So a slice never raises IndexError. Example: [`clamp`](#clamp).

**r4.** "If i or j are omitted or None, they become “end” values (which end depends on the sign of k). Note, k cannot be zero."[^2] Example: [`zero-step`](#zero-step).

**r5.** "When a slice is evaluated, the interpreter constructs a slice object whose start, stop and step attributes, respectively, are the results of the expressions between the colons. Any missing expression evaluates to None."[^1] Example: [`slice-object`](#slice-object).

**r6.** "All slice operations return a new list containing the requested elements. This means that the following slice returns a shallow copy of the list".[^3] Example: [`copy`](#copy).

**r7.** "Assignment to slices is also possible, and this can even change the size of the list or clear it entirely".[^3] For an extended slice, "If k is not equal to 1, t must have the same length as the slice it is replacing."[^2] Examples: [`assign`](#assign), [`extended-length`](#extended-length).

## Examples

```python run id=clamp
s = [1, 2, 3]
print(s[1:100], s[-100:2], s[5:])
```

```output for=clamp
[2, 3] [1, 2] []
```

Bounds outside the list are moved to the ends (rule r3). The index `s[5]` would raise IndexError.

```python run id=assign
letters = ["a", "b", "c", "d", "e", "f", "g"]
letters[2:5] = ["C", "D", "E"]
print(letters)
letters[2:5] = []
print(letters)
letters[1:1] = ["x", "y"]
print(letters)
letters[::2] = [0, 0, 0]
print(letters)
```

```output for=assign
['a', 'b', 'C', 'D', 'E', 'f', 'g']
['a', 'b', 'f', 'g']
['a', 'x', 'y', 'b', 'f', 'g']
[0, 'x', 0, 'b', 0, 'g']
```

The empty slice `[1:1]` inserts items. The extended slice `[::2]` replaces every second item (rule r7).

```python run id=slice-object
middle = slice(1, -1)
word = "python"
nums = [1, 2, 3, 4]
print(word[middle], nums[middle])
print(middle.start, middle.stop, middle.step)
print(slice(None, None, -1).indices(4))
```

```output for=slice-object
ytho [2, 3]
1 -1 None
(3, -1, -1)
```

A program can store a slice object and use it on many sequences (rule r5).

```python run id=copy
original = [[1], [2]]
copy = original[:]
copy.append([3])
copy[0].append(99)
print(original)
print(copy)
```

```output for=copy
[[1, 99], [2]]
[[1, 99], [2], [3]]
```

The slice is a new list, but it holds the same inner lists (rule r6).

## Pitfalls

A step of zero is an error:

```python fails id=zero-step expect=exception:ValueError
"abc"[::0]
```

An extended slice assignment needs the same number of items:

```python fails id=extended-length expect=exception:ValueError
nums = [1, 2, 3, 4]
nums[::2] = [9]
```

## Comparison

A Go slice expression `a[low:high]` has no step and no negative indices, and the result shares memory with the operand: "if the result is a slice, it shares its underlying array with the operand."[^5] A Python slice of a list is a new list (rule r6), so a change to the slice does not change the original list.
