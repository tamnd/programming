---
tag: W457XB
kind: feature
owner: python
slug: copying-collections
name: Copying collections
aliases: [copy, deepcopy, shallow copy, deep copy, copy a list, copy a dict, list.copy, __copy__, __deepcopy__, aliasing]
area: languages/python/collections
level: core
summary: >-
  Assignment never copies a collection. A shallow copy makes a new container that holds the same items, and copy.deepcopy() also copies the items inside, recursively.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/copy.html", locator: "copy: Shallow and deep copy operations"}
see_also: [python/lists, python/slicing, python/dictionaries, python/sequence-types, python/frozendict]
sources:
  - url: https://docs.python.org/3.15/library/copy.html
    title: "copy: Shallow and deep copy operations, Python 3.15"
    locator: Introduction; copy(); deepcopy(); __copy__(); __deepcopy__()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/introduction.html#lists
    title: The Python Tutorial, 3. An Informal Introduction to Python
    locator: 3.1.3. Lists
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Appending_and_copying_slices
    title: The Go Programming Language Specification
    locator: Appending to and copying slices
    accessed: 2026-11-16
---

## Synopsis

```python run id=levels
import copy

grid = [[1, 2], [3, 4]]
alias = grid
shallow = grid.copy()
deep = copy.deepcopy(grid)

grid[0].append(99)
grid.append([5])
print(alias)
print(shallow)
print(deep)
```

```output for=levels
[[1, 2, 99], [3, 4], [5]]
[[1, 2, 99], [3, 4]]
[[1, 2], [3, 4]]
```

The alias is the same list. The shallow copy is a new outer list with the same inner lists. The deep copy shares nothing.

## Semantics

**r1.** "Assignment statements in Python do not copy objects, they create bindings between a target and an object."[^1] Example: [`levels`](#levels).

**r2.** "A shallow copy constructs a new compound object and then (to the extent possible) inserts references into it to the objects found in the original."[^1]

**r3.** "A deep copy constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original."[^1]

**r4.** "Shallow copies of many collections can be made using the corresponding copy() method (such as list.copy(), dict.copy() or set.copy()), and of sequences (such as lists or bytearrays) by making a slice of the entire sequence (sequence[:])."[^1] "All slice operations return a new list containing the requested elements."[^2] Example: [`ways`](#ways).

**r5.** "The deepcopy() function avoids these problems by: keeping a memo dictionary of objects already copied during the current copying pass; and letting user-defined classes override the copying operation or the set of components copied."[^1] Example: [`recursive`](#recursive).

**r6.** "In order for a class to define its own copy implementation, it can define special methods __copy__() and __deepcopy__()."[^1] Example: [`custom`](#custom).

**r7.** The module "does “copy” functions and classes (shallow and deeply), by returning the original object unchanged".[^1]

## Examples

```python run id=ways
import copy

prices = {"apple": 3, "pear": 5}
for c in (prices.copy(), dict(prices), {**prices}, copy.copy(prices)):
    c["fig"] = 1
print(prices)

nums = [1, 2, 3]
print(nums[:] == nums, nums[:] is nums, list(nums) is nums)
```

```output for=ways
{'apple': 3, 'pear': 5}
True False False
```

Each shallow copy is a new dict, so an added key does not change `prices` (rule r4).

```python run id=recursive
import copy

node = {"name": "a"}
node["self"] = node
clone = copy.deepcopy(node)
print(clone["self"] is clone, clone is node)
```

```output for=recursive
True False
```

The memo dictionary maps the original to the copy, so the cycle is copied once (rule r5).

```python run id=custom
import copy

class Config:
    def __init__(self, settings, cache):
        self.settings = settings
        self.cache = cache

    def __deepcopy__(self, memo):
        return Config(copy.deepcopy(self.settings, memo), self.cache)

shared = {}
a = Config({"debug": False}, shared)
b = copy.deepcopy(a)
b.settings["debug"] = True
print(a.settings, b.cache is shared)
```

```output for=custom
{'debug': False} True
```

The class copies its settings but shares its cache (rule r6).

## Pitfalls

A shallow copy shares the inner items:

```python wrong id=shallow-nested
teams = {"red": ["ann"], "blue": ["bob"]}
backup = teams.copy()
teams["red"].append("cy")
print(backup)
```

```output for=shallow-nested
{'red': ['ann', 'cy'], 'blue': ['bob']}
```

Use `copy.deepcopy(teams)` when the values are mutable (rule r3).

## Comparison

Go has a built-in function for slices: "The function copy copies slice elements from a source src to a destination dst and returns the number of elements copied."[^3] "The number of elements copied is the minimum of len(src) and len(dst)."[^3] The program must make the destination slice first. Python has `list.copy()`, slicing and `copy.copy()` for shallow copies, and `copy.deepcopy()` for deep copies (rules r2 and r3).
