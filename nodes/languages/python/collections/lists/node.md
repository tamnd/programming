---
tag: JT8H9S
kind: feature
owner: python
slug: lists
name: Lists
aliases: [list, Python list, list methods, append, extend, insert, remove, pop, mutable sequence, dynamic array]
area: languages/python/collections
level: intro
summary: >-
  A list is a mutable sequence of items in square brackets. A program can index it, slice it, change items in place, and add or remove items with methods such as append, insert, remove and pop.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#lists", locator: "Lists"}
see_also: [python/sequence-types, python/slicing, python/list-comprehensions, python/lists-as-stacks, python/tuples-and-sequences, python/copying-collections]
sources:
  - url: https://docs.python.org/3.15/tutorial/introduction.html#lists
    title: The Python Tutorial, 3. An Informal Introduction to Python
    locator: 3.1.3. Lists
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#more-on-lists
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1. More on Lists
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#lists
    title: Built-in Types, Python 3.15 documentation
    locator: Lists; Mutable Sequence Types
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Slice_types
    title: The Go Programming Language Specification
    locator: Slice types
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
squares = [1, 4, 9, 16, 25]
print(squares[0], squares[-1], squares[-3:])

squares.append(36)
squares[0] = 0
print(squares, len(squares))
```

```output for=basics
1 25 [9, 16, 25]
[0, 4, 9, 16, 25, 36] 6
```

A list keeps its items in order. The program reads items by index, and it changes the list in place.

## Semantics

**r1.** "Lists are mutable sequences, typically used to store collections of homogeneous items (where the precise degree of similarity will vary by application)."[^3]

**r2.** A list can be made "Using a pair of square brackets to denote the empty list: []", "Using square brackets, separating items with commas: [a], [a, b, c]", "Using a list comprehension: [x for x in iterable]" or "Using the type constructor: list() or list(iterable)".[^3] Example: [`construct`](#construct).

**r3.** "If iterable is already a list, a copy is made and returned, similar to iterable[:]."[^3]

**r4.** "Simple assignment in Python never copies data. When you assign a list to a variable, the variable refers to the existing list."[^1] Example: [`alias`](#alias).

**r5.** The methods change the list in place. "You might have noticed that methods like insert, remove or sort that only modify the list have no return value printed – they return the default None."[^2] Example: [`append-result`](#append-result).

**r6.** remove() raises "a ValueError if there is no such item."[^2] pop() "raises an IndexError if the list is empty or the index is outside the list range."[^2] Example: [`remove-missing`](#remove-missing).

## Examples

```python run id=construct
print([], [1, "two", 3.0])
print(list("abc"), list((1, 2, 3)))
print([n * n for n in range(4)])
```

```output for=construct
[] [1, 'two', 3.0]
['a', 'b', 'c'] [1, 2, 3]
[0, 1, 4, 9]
```

```python run id=methods
fruits = ["orange", "apple", "pear", "banana", "apple"]
print(fruits.count("apple"), fruits.index("pear"))

fruits.insert(0, "kiwi")
fruits.remove("apple")
last = fruits.pop()
print(last, fruits)

fruits.extend(["fig", "lime"])
fruits.sort()
print(fruits)
```

```output for=methods
2 2
apple ['kiwi', 'orange', 'pear', 'banana']
['banana', 'fig', 'kiwi', 'lime', 'orange', 'pear']
```

`remove` takes out only the first `"apple"`, and `pop()` with no index takes the last item.

```python run id=alias
rgb = ["red", "green", "blue"]
rgba = rgb
rgba.append("alpha")
print(rgb)

copy = rgb[:]
copy.append("extra")
print(len(rgb), len(copy))
```

```output for=alias
['red', 'green', 'blue', 'alpha']
4 5
```

`rgba` and `rgb` are names for one list (rule r4). The slice `rgb[:]` makes a new list.

```python run id=nested
grid = [["a", "b"], [1, 2, 3]]
print(grid[0][1], len(grid), len(grid[1]))
```

```output for=nested
b 2 3
```

## Pitfalls

A method that changes the list returns None, so do not assign its result:

```python wrong id=append-result
items = [3, 1, 2]
items = items.sort()
print(items)
```

```output for=append-result
None
```

Use `items.sort()` alone, or `items = sorted(items)` (rule r5).

remove() fails when the value is not in the list:

```python fails id=remove-missing expect=exception:ValueError
colors = ["red", "blue"]
colors.remove("green")
```

Test with `in` first, or catch ValueError (rule r6).

## Comparison

The Go type that is closest to a Python list is a slice: "A slice is a descriptor for a contiguous segment of an underlying array and provides access to a numbered sequence of elements from that array."[^4] A Go slice holds items of one element type. A Python list can hold items of any type, and its methods change it in place (rule r5).
