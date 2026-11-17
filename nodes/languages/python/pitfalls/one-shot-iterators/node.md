---
tag: WXX0N5
kind: feature
owner: python
slug: one-shot-iterators
name: One-shot iterators
aliases: [exhausted iterator, iterator used twice, generator used twice, empty second loop, map object, zip object, StopIteration, itertools.tee]
area: languages/python/pitfalls
level: core
summary: >-
  An iterator such as a generator, a map or a zip gives its items once. A second loop over it sees nothing, and an in test uses up items. Make a list, or call the function that makes the iterator again.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/glossary.html#term-iterator", locator: "Glossary, iterator"}
proposals: []
see_also: [python/generators, python/generator-expressions, python/itertools, python/for-statements, python/built-in-functions, python/comparison-operators, python/special-methods]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-iterator
    title: Glossary, Python 3.15 documentation
    locator: iterator
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/functions.html#map
    title: Built-in Functions, Python 3.15 documentation
    locator: map(); zip()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/expressions.html#membership-test-operations
    title: The Python Language Reference, Python 3.15 documentation
    locator: 6.10.2 Membership test operations
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/itertools.html#itertools.tee
    title: itertools, Functions creating iterators for efficient looping, Python 3.15 documentation
    locator: itertools.tee()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.0.html#views-and-iterators-instead-of-lists
    title: What’s New In Python 3.0
    locator: Views And Iterators Instead Of Lists
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=second-loop
squares = map(lambda n: n * n, [1, 2, 3])
print(list(squares))
print(list(squares))

squares = [n * n for n in [1, 2, 3]]
print(list(squares))
print(list(squares))
```

```output for=second-loop
[1, 4, 9]
[]
[1, 4, 9]
[1, 4, 9]
```

The `map` object is an iterator, so the second `list` call gets no items. The list can be read many times.

## Semantics

**r1.** An iterator is "An object representing a stream of data."[^1] When it has no more items, "the iterator object is exhausted and any further calls to its __next__() method just raise StopIteration again."[^1] A loop or a `list` call ends at the first `StopIteration`, so an exhausted iterator looks empty and gives no error.

**r2.** A container is different: "A container object (such as a list) produces a fresh new iterator each time you pass it to the iter() function or use it in a for loop. Attempting this with an iterator will just return the same exhausted iterator object used in the previous iteration pass, making it appear like an empty container."[^1] Example: [`second-loop`](#second-loop).

**r3.** Many functions return iterators, not lists. `map` will "Return an iterator that applies function to every item of iterable".[^2] `zip`, `filter`, `enumerate`, `reversed`, generator functions, generator expressions, file objects and the functions in `itertools` also give iterators.

**r4.** A function that reads its argument twice gives a wrong result for an iterator. The first pass uses up all the items. Example: [`two-passes`](#two-passes).

**r5.** The `in` test reads an iterator until it finds the item: "x in y is True if some value z, for which the expression x is z or x == z is true, is produced while iterating over y."[^3] The items up to the match are gone. Example: [`in-consumes`](#in-consumes).

**r6.** The fixes:

- Store the items in a list or a tuple when the data is small enough.
- Call the function that makes the iterator again, for each pass.
- Make the function accept an iterable and call `iter` on it once, or accept a container and document that it reads the data twice.
- Use `itertools.tee`, which will "Return n independent iterators from a single iterable."[^4] It stores the items that one copy has read and the other has not, so it "may require significant auxiliary storage".[^4] Example: [`tee-copies`](#tee-copies).

**r7.** A test `iter(x) is x` is true for an iterator, because "Iterators are required to have an __iter__() method that returns the iterator object itself".[^1] A function can use this test to reject an iterator when it needs two passes. Example: [`two-passes`](#two-passes).

## Examples

```python run id=tee-copies
import itertools

readings = (t for t in [21, 23, 22])
a, b = itertools.tee(readings)
print(max(a), min(b))
```

```output for=tee-copies
23 21
```

Each copy gives all three items (rule r6).

```python run id=fresh-each-time
def readings():
    return (t for t in [21, 23, 22])


print(max(readings()), min(readings()))
```

```output for=fresh-each-time
23 21
```

Each call makes a new generator, so each pass starts at the first item (rule r6).

## Versions

The rules are the same in all Python 3 versions. Python 3.0 made more functions return iterators: "map() and filter() return iterators." and "zip() now returns an iterator."[^5] In Python 2, they returned lists, which could be read many times.

## Pitfalls

```python wrong id=two-passes
def shares(values):
    total = sum(values)
    return [v / total for v in values]


print(shares([1, 3]))
print(shares(v for v in [1, 3]))

gen = (v for v in [1, 3])
print(iter(gen) is gen, iter([1, 3]) is [1, 3])
```

```output for=two-passes
[0.25, 0.75]
[]
True False
```

`sum` used up the generator, so the comprehension saw no items (rule r4). The last line shows the test of rule r7.

```python wrong id=in-consumes
items = iter([1, 2, 3, 4, 5])
print(3 in items)
print(list(items))
```

```output for=in-consumes
True
[4, 5]
```

The `in` test read 1, 2 and 3 to find the match, so only 4 and 5 are left (rule r5).

```python wrong id=zip-twice
pairs = zip("ab", [1, 2])
print(dict(pairs))
print(dict(pairs))
```

```output for=zip-twice
{'a': 1, 'b': 2}
{}
```

A `zip` object is also an iterator (rule r3).

## Comparison

In Python, a for loop calls `iter` on the object, and a container gives a new iterator each time, while an iterator gives itself (rules r2 and r7). So the same loop code works twice for a list and once for a generator. For generators, see [[python/generators]]. For lazy pipelines, see [[python/generator-expressions]] and [[python/itertools]].
