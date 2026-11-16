---
tag: GB6VM4
kind: feature
owner: python
slug: dict-order-and-views
name: Dict order and views
aliases: [insertion order, ordered dict, keys view, values view, items view, dict views, dictionary changed size during iteration, popitem LIFO, reversed dict]
area: languages/python/collections
level: core
summary: >-
  A dict keeps its keys in insertion order. keys(), values() and items() return views that show later changes to the dict. Keys views and items views also support set operations.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#dictionary-view-objects", locator: "Dictionary view objects"}
see_also: [python/dictionaries, python/sets, python/the-collections-module, python/looping-techniques]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#dictionary-view-objects
    title: Built-in Types, Python 3.15 documentation
    locator: Mapping types (dict order, popitem, reversed); Dictionary view objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#dictionaries
    title: The Python Tutorial, 5. Data Structures
    locator: 5.5. Dictionaries
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0468/
    title: "PEP 468: Preserving the order of **kwargs in a function"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=views
dishes = {"eggs": 2, "sausage": 1, "bacon": 1, "spam": 500}
keys = dishes.keys()
values = dishes.values()
print(list(keys), sum(values))

del dishes["eggs"]
del dishes["sausage"]
print(list(keys))
print(keys & {"eggs", "bacon", "salad"})
```

```output for=views
['eggs', 'sausage', 'bacon', 'spam'] 504
['bacon', 'spam']
{'bacon'}
```

The view `keys` was made before the deletions, and it still shows the current keys.

## Semantics

**r1.** "Dictionaries preserve insertion order. Note that updating a key does not affect the order. Keys added after deletion are inserted at the end."[^1] Example: [`order`](#order).

**r2.** "The objects returned by dict.keys(), dict.values() and dict.items() are view objects. They provide a dynamic view on the dictionary’s entries, which means that when the dictionary changes, the view reflects these changes."[^1]

**r3.** "Keys and values are iterated over in insertion order. This allows the creation of (value, key) pairs using zip(): pairs = zip(d.values(), d.keys())."[^1] Example: [`pairs`](#pairs).

**r4.** "Keys views are set-like since their entries are unique and hashable."[^1] "(Values views are not treated as set-like since the entries are generally not unique.)"[^1] "While using set operators, set-like views accept any iterable as the other operand, unlike sets which only accept sets as the input."[^1] Example: [`set-ops`](#set-ops).

**r5.** popitem() removes and returns a pair: "Pairs are returned in LIFO order."[^1] "Dictionaries and dictionary views are reversible."[^1] Example: [`lifo`](#lifo).

**r6.** "Iterating views while adding or deleting entries in the dictionary may raise a RuntimeError or fail to iterate over all entries."[^1] Example: [`change-while-iterating`](#change-while-iterating).

**r7.** "Performing list(d) on a dictionary returns a list of all the keys used in the dictionary, in insertion order (if you want it sorted, just use sorted(d) instead)."[^2]

**r8.** Dict equality ignores order: "Dictionaries compare equal if and only if they have the same (key, value) pairs (regardless of ordering)."[^1] PEP 468 "dictates that the collected keyword arguments be exposed in the function body as an ordered mapping."[^3] Example: [`kwargs`](#kwargs).

## Examples

```python run id=order
d = {"one": 1, "two": 2, "three": 3, "four": 4}
d["one"] = 42
del d["two"]
d["two"] = None
print(d)
```

```output for=order
{'one': 42, 'three': 3, 'four': 4, 'two': None}
```

The update of `"one"` keeps its place. The key `"two"` goes to the end after it is deleted and added again (rule r1).

```python run id=pairs
d = {"a": 1, "b": 2}
print(list(zip(d.values(), d.keys())))
print([(v, k) for (k, v) in d.items()])
print(("b", 2) in d.items(), 2 in d.values())
```

```output for=pairs
[(1, 'a'), (2, 'b')]
[(1, 'a'), (2, 'b')]
True True
```

```python run id=set-ops
a = {"x": 1, "y": 2, "z": 3}
b = {"y": 0, "w": 0}
print(sorted(a.keys() - b.keys()))
print(sorted(a.keys() | ["q"]))
print(a.keys() == {"z", "y", "x"})
```

```output for=set-ops
['x', 'z']
['q', 'x', 'y', 'z']
True
```

A keys view works with `-`, `|` and `==` like a set. The right operand of `|` can be a list (rule r4).

```python run id=lifo
d = {"one": 1, "two": 2, "three": 3}
print(list(reversed(d)), list(reversed(d.items()))[0])
print(d.popitem(), d)
```

```output for=lifo
['three', 'two', 'one'] ('three', 3)
('three', 3) {'one': 1, 'two': 2}
```

```python run id=kwargs
def show(**kwargs):
    print(list(kwargs))

show(b=1, a=2, c=3)
print({"a": 1, "b": 2} == {"b": 2, "a": 1})
```

```output for=kwargs
['b', 'a', 'c']
True
```

The keyword arguments keep the order of the call (rule r8). Two dicts with the same pairs in a different order are still equal.

## Pitfalls

Do not add keys to a dict while a loop goes over it:

```python fails id=change-while-iterating expect=exception:RuntimeError
counts = {"a": 1, "b": 2}
for key in counts:
    counts[key + key] = 0
```

Loop over a copy such as `list(counts)`, or collect the new pairs and add them after the loop (rule r6).

## Comparison

Go map iteration has no fixed order: "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next."[^4] A Go program that needs an order sorts the keys first. A Python dict always gives insertion order (rule r1), so the order is the same each time.
