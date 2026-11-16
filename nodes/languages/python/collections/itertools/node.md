---
tag: 62E3PF
kind: feature
owner: python
slug: itertools
name: itertools
aliases: [itertools module, iterator algebra, chain, islice, groupby, product, permutations, combinations, accumulate, batched, pairwise, count, cycle, zip_longest, tee]
area: languages/python/collections
level: core
summary: >-
  The itertools module gives fast, memory efficient iterator building blocks. They chain, slice, group, combine and accumulate items from any iterable, and some of them produce endless streams.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/itertools.html", locator: "itertools: Functions creating iterators for efficient looping"}
see_also: [python/generator-expressions, python/the-collections-module, python/sorting, python/slicing]
sources:
  - url: https://docs.python.org/3.15/library/itertools.html
    title: "itertools: Functions creating iterators for efficient looping, Python 3.15"
    locator: Introduction; Itertool Functions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=tour
from itertools import accumulate, chain, islice, count, product, combinations, pairwise, batched

print(list(accumulate([1, 2, 3, 4, 5])))
print(list(chain("ABC", "DEF")))
print(list(islice(count(10, 5), 4)))
print(["".join(p) for p in product("AB", repeat=2)])
print(["".join(c) for c in combinations("ABCD", 2)])
print(list(pairwise([1, 4, 9])), list(batched("ABCDEFG", 3)))
```

```output for=tour
[1, 3, 6, 10, 15]
['A', 'B', 'C', 'D', 'E', 'F']
[10, 15, 20, 25]
['AA', 'AB', 'BA', 'BB']
['AB', 'AC', 'AD', 'BC', 'BD', 'CD']
[(1, 4), (4, 9)] [('A', 'B', 'C'), ('D', 'E', 'F'), ('G',)]
```

Each function returns an iterator. `list()` collects the items so the program can print them.

## Semantics

**r1.** "This module implements a number of iterator building blocks inspired by constructs from APL, Haskell, and SML."[^1] "The module standardizes a core set of fast, memory efficient tools that are useful by themselves or in combination."[^1]

**r2.** "The following functions all construct and return iterators. Some provide streams of infinite length, so they should only be accessed by functions or loops that truncate the stream."[^1] Example: [`truncate`](#truncate).

**r3.** islice() "Works like sequence slicing but does not support negative values for start, stop, or step."[^1] Example: [`negative-islice`](#negative-islice).

**r4.** groupby() makes "an iterator that returns consecutive keys and groups from the iterable."[^1] "Generally, the iterable needs to already be sorted on the same key function."[^1] Example: [`unsorted-groupby`](#unsorted-groupby).

**r5.** "The returned group is itself an iterator that shares the underlying iterable with groupby(). Because the source is shared, when the groupby() object is advanced, the previous group is no longer visible. So, if that data is needed later, it should be stored as a list".[^1] Example: [`groups`](#groups).

**r6.** product() is "Roughly equivalent to nested for-loops in a generator expression. For example, product(A, B) returns the same as ((x,y) for x in A for y in B)."[^1]

**r7.** tee() returns "n independent iterators from a single iterable."[^1] Example: [`tee`](#tee).

## Examples

```python run id=truncate
from itertools import count, cycle, repeat, islice

print(list(zip(count(1), "abc")))
print(list(islice(cycle("RG"), 5)))
print(list(map(pow, range(4), repeat(2))))
```

```output for=truncate
[(1, 'a'), (2, 'b'), (3, 'c')]
['R', 'G', 'R', 'G', 'R']
[0, 1, 4, 9]
```

`count`, `cycle` and `repeat` never stop. `zip`, `islice` and the shorter `range` end the loop (rule r2).

```python run id=groups
from itertools import groupby

words = sorted(["apple", "bob", "avocado", "cat", "bean"], key=lambda w: w[0])
for letter, group in groupby(words, key=lambda w: w[0]):
    print(letter, list(group))
```

```output for=groups
a ['apple', 'avocado']
b ['bob', 'bean']
c ['cat']
```

`list(group)` saves each group before the loop moves on (rule r5).

```python run id=tee
from itertools import tee, zip_longest, chain

a, b = tee(iter([1, 2, 3]))
print(list(a), list(b))
print(list(zip_longest("ab", "xyz", fillvalue="-")))
print(list(chain.from_iterable([[1, 2], [3]])))
```

```output for=tee
[1, 2, 3] [1, 2, 3]
[('a', 'x'), ('b', 'y'), ('-', 'z')]
[1, 2, 3]
```

## Pitfalls

groupby only groups items that are next to each other:

```python wrong id=unsorted-groupby
from itertools import groupby

data = ["a", "b", "a"]
print([(k, len(list(g))) for k, g in groupby(data)])
```

```output for=unsorted-groupby
[('a', 1), ('b', 1), ('a', 1)]
```

Sort with the same key first, or use `collections.Counter` to count all items (rule r4).

islice does not accept a negative index:

```python fails id=negative-islice expect=exception:ValueError
from itertools import islice
list(islice([1, 2, 3], -1))
```

To get the last items of an iterator, use `collections.deque(it, maxlen=n)` (rule r3).

## Comparison

Go has no standard iterator algebra in the language spec. A Go `for` loop can range over a function: "For a function f, the iteration proceeds by calling f with a new, synthesized yield function as its argument."[^2] A Go program writes such functions to chain or filter values. In Python, itertools gives ready-made functions that work on any iterable (rule r1).
