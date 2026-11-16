---
tag: VJP63D
kind: feature
owner: python
slug: dictionaries
name: Dictionaries
aliases: [dict, dictionary, associative array, hash map, key value pairs, dict get, setdefault, KeyError, dict merge]
area: languages/python/collections
level: intro
summary: >-
  A dictionary maps unique hashable keys to values. A program stores a value with d[key] = value, reads it with d[key] or get(), removes it with del or pop(), and tests a key with in.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#mapping-types-dict-frozendict", locator: "Mapping types: dict, frozendict"}
see_also: [python/dict-order-and-views, python/set-and-dict-comprehensions, python/hashability, python/sets, python/the-collections-module, python/frozendict]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#dictionaries
    title: The Python Tutorial, 5. Data Structures
    locator: 5.5. Dictionaries
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#mapping-types-dict-frozendict
    title: Built-in Types, Python 3.15 documentation
    locator: Mapping types; dict
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0584/
    title: "PEP 584: Add Union Operators To dict"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Index_expressions
    title: The Go Programming Language Specification
    locator: Index expressions
    accessed: 2026-11-16
---

## Synopsis

```python run id=tel
tel = {"jack": 4098, "sape": 4139}
tel["guido"] = 4127
print(tel["jack"], tel.get("irv"))
del tel["sape"]
tel["irv"] = 4127
print(tel)
print("guido" in tel, sorted(tel))
```

```output for=tel
4098 None
{'jack': 4098, 'guido': 4127, 'irv': 4127}
True ['guido', 'irv', 'jack']
```

A dict stores values by key. `get` returns None for a missing key, and `in` tests for a key.

## Semantics

**r1.** "It is best to think of a dictionary as a set of key: value pairs, with the requirement that the keys are unique (within one dictionary)."[^1] "If you store using a key that is already in use, the old value associated with that key is forgotten."[^1]

**r2.** "A mapping object maps hashable values to arbitrary objects."[^2] "Values that are not hashable, that is, values containing lists, dictionaries or other mutable types (that are compared by value rather than by object identity) may not be used as keys."[^2] Example: [`list-key`](#list-key).

**r3.** "Values that compare equal (such as 1, 1.0, and True) can be used interchangeably to index the same dictionary entry."[^2] Example: [`equal-keys`](#equal-keys).

**r4.** "Extracting a value for a non-existent key by subscripting (d[key]) raises a KeyError. To avoid getting this error when trying to access a possibly non-existent key, use the get() method instead, which returns None (or a specified default value) if the key is not in the dictionary."[^1] Example: [`missing`](#missing).

**r5.** A dict can be made with braces, a dict comprehension, or the constructor: "dict(), dict([('foo', 100), ('bar', 200)]), dict(foo=100, bar=200)".[^2] Example: [`construct`](#construct).

**r6.** "Dictionaries compare equal if and only if they have the same (key, value) pairs (regardless of ordering). Order comparisons (‘<’, ‘<=’, ‘>=’, ‘>’) raise TypeError."[^2]

**r7.** setdefault(): "If key is in the dictionary, return its value. If not, insert key with a value of default and return default."[^2] Example: [`group`](#group).

**r8.** `d | other` creates "a new dictionary with the merged keys and values of d and other, which must both be dictionaries. The values of other take priority when d and other share keys."[^2] PEP 584 "proposes adding merge (|) and update (|=) operators to the built-in dict class."[^3] Example: [`merge`](#merge).

## Examples

```python run id=construct
a = dict(one=1, two=2, three=3)
b = {"one": 1, "two": 2, "three": 3}
c = dict(zip(["one", "two", "three"], [1, 2, 3]))
d = dict([("two", 2), ("one", 1), ("three", 3)])
print(a == b == c == d)
```

```output for=construct
True
```

All four dicts have the same pairs, so they are equal (rule r6).

```python run id=group
words = ["apple", "bob", "avocado", "cat", "bean"]
by_letter = {}
for w in words:
    by_letter.setdefault(w[0], []).append(w)
print(by_letter)
print(by_letter.pop("c"), len(by_letter))
```

```output for=group
{'a': ['apple', 'avocado'], 'b': ['bob', 'bean'], 'c': ['cat']}
['cat'] 2
```

```python run id=merge
defaults = {"color": "blue", "size": 1}
user = {"size": 3}
print(defaults | user)
defaults |= {"color": "red"}
print(defaults)
```

```output for=merge
{'color': 'blue', 'size': 3}
{'color': 'red', 'size': 1}
```

```python run id=equal-keys
d = {1: "int"}
d[1.0] = "float"
d[True] = "bool"
print(d)
```

```output for=equal-keys
{1: 'bool'}
```

`1`, `1.0` and `True` are equal, so they are one key. The dict keeps the first key object and the last value (rule r3).

## Pitfalls

Subscripting a missing key is an error:

```python fails id=missing expect=exception:KeyError
tel = {"jack": 4098}
print(tel["irv"])
```

Use `tel.get("irv")`, `tel.get("irv", 0)`, or test with `in` first (rule r4).

A list cannot be a key:

```python fails id=list-key expect=exception:TypeError
grid = {}
grid[[0, 0]] = "origin"
```

Use a tuple such as `(0, 0)` (rule r2).

## Comparison

A Go map index expression does not fail for a missing key. The Go spec says that "if the map is nil or does not contain such an entry, a[x] is the zero value for the element type of M".[^4] The form `v, ok := m[k]` tells the program whether the key was there. A Python dict raises KeyError (rule r4), and `get` returns None or a default value.
