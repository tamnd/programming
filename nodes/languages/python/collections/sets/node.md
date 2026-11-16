---
tag: BQG1A0
kind: feature
owner: python
slug: sets
name: Sets
aliases: [set, frozenset, set operations, union, intersection, difference, symmetric difference, membership test, remove duplicates]
area: languages/python/collections
level: intro
summary: >-
  A set is an unordered collection of distinct hashable objects. It has fast membership tests and the operations union, intersection, difference and symmetric difference. frozenset is immutable.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#set-types-set-frozenset", locator: "Set Types: set, frozenset"}
see_also: [python/set-and-dict-comprehensions, python/dictionaries, python/hashability, python/container-displays, python/collections-abc]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#sets
    title: The Python Tutorial, 5. Data Structures
    locator: 5.4. Sets
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#set-types-set-frozenset
    title: Built-in Types, Python 3.15 documentation
    locator: Set Types; set; frozenset
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Map_types
    title: The Go Programming Language Specification
    locator: Map types
    accessed: 2026-11-16
---

## Synopsis

```python run id=basket
basket = {"apple", "orange", "apple", "pear", "orange", "banana"}
print(len(basket), sorted(basket))
print("orange" in basket, "crabgrass" in basket)

a = set("abracadabra")
b = set("alacazam")
print(sorted(a - b), sorted(a | b))
print(sorted(a & b), sorted(a ^ b))
```

```output for=basket
4 ['apple', 'banana', 'orange', 'pear']
True False
['b', 'd', 'r'] ['a', 'b', 'c', 'd', 'l', 'm', 'r', 'z']
['a', 'c'] ['b', 'd', 'l', 'm', 'r', 'z']
```

The set keeps one copy of each item. The examples print `sorted()` results, because the order of a set is not fixed.

## Semantics

**r1.** "A set object is an unordered collection of distinct hashable objects. Common uses include membership testing, removing duplicates from a sequence, and computing mathematical operations such as intersection, union, difference, and symmetric difference."[^2]

**r2.** "Being an unordered collection, sets do not record element position or order of insertion. Accordingly, sets do not support indexing, slicing, or other sequence-like behavior."[^2] "Because sets are unordered, iterating over them or printing them can produce the elements in a different order than you expect."[^1] Example: [`index`](#index).

**r3.** "Curly braces or the set() function can be used to create sets. Note: to create an empty set you have to use set(), not {}; the latter creates an empty dictionary".[^1] Example: [`empty-braces`](#empty-braces).

**r4.** The contents of a set "can be changed using methods like add() and remove(). Since it is mutable, it has no hash value and cannot be used as either a dictionary key or as an element of another set."[^2] "The frozenset type is immutable and hashable".[^2] Example: [`frozen`](#frozen).

**r5.** "The elements of a set must be hashable. To represent sets of sets, the inner sets must be frozenset objects."[^2] Example: [`list-element`](#list-element).

**r6.** "the non-operator versions of union(), intersection(), difference(), symmetric_difference(), issubset(), and issuperset() methods will accept any iterable as an argument. In contrast, their operator based counterparts require their arguments to be sets."[^2] Example: [`methods`](#methods).

**r7.** remove() "Raises KeyError if elem is not contained in the set."[^2] discard() removes the element "if it is present."[^2]

**r8.** "Two sets are equal if and only if every element of each set is contained in the other (each is a subset of the other)."[^2] `<` and `>` test for a proper subset and a proper superset.

## Examples

```python run id=methods
tags = {"python"}
tags.add("go")
tags.update(["rust", "go"])
tags.discard("java")
tags.remove("rust")
print(sorted(tags))
print(set("abc").intersection("cbs") == {"b", "c"})
print({1, 2} <= {1, 2, 3}, {1, 2} < {1, 2}, {1, 2}.isdisjoint([3]))
```

```output for=methods
['go', 'python']
True
True False True
```

`update` and `intersection` take any iterable (rule r6). `discard` does nothing when the item is not there (rule r7).

```python run id=dedupe
words = ["b", "a", "b", "c", "a"]
print(sorted(set(words)))
print(list(dict.fromkeys(words)))
```

```output for=dedupe
['a', 'b', 'c']
['b', 'a', 'c']
```

A set removes duplicates but loses the order. `dict.fromkeys` keeps the first position of each item.

```python run id=frozen
pair = frozenset({"x", "y"})
groups = {pair: "edge", frozenset(): "none"}
print(groups[frozenset(["y", "x"])])
print(set("abc") == frozenset("abc"))
```

```output for=frozen
edge
True
```

A frozenset can be a dict key (rule r4). A set and a frozenset with the same members are equal.

## Pitfalls

`{}` is an empty dict, not an empty set:

```python wrong id=empty-braces
seen = {}
print(type(seen).__name__)
```

```output for=empty-braces
dict
```

Use `set()` (rule r3).

A set has no positions:

```python fails id=index expect=exception:TypeError
colors = {"red", "green"}
colors[0]
```

Make a sorted list first with `sorted(colors)` if the program needs an order (rule r2).

A list cannot be a set element:

```python fails id=list-element expect=exception:TypeError
points = {[0, 0], [1, 2]}
```

Use tuples, for example `{(0, 0), (1, 2)}` (rule r5).

## Comparison

Go has no set type. A Go program uses a map with the set elements as keys, for example `map[string]bool`. The Go spec requires that "The comparison operators == and != must be fully defined for operands of the key type; thus the key type must not be a function, map, or slice."[^3] This is like the Python rule that set elements must be hashable (rule r5). Go has no built-in union or intersection.
