---
tag: JN99PQ
kind: feature
owner: python
slug: frozendict
name: frozendict
aliases: [frozen dict, immutable dict, immutable mapping, hashable dict, PEP 814, frozen dictionary]
area: languages/python/collections
level: advanced
summary: >-
  frozendict is an immutable mapping with the read API of dict. It keeps insertion order, it can be hashed when all its keys and values can be hashed, and the | operator makes a new frozendict.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#frozen-dictionaries", locator: "Frozen dictionaries"}
see_also: [python/dictionaries, python/hashability, python/sets, python/copying-collections]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#frozen-dictionaries
    title: Built-in Types, Python 3.15 documentation
    locator: Frozen dictionaries
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#func-frozendict
    title: Built-in Functions, Python 3.15 documentation
    locator: frozendict
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0814/
    title: "PEP 814: Add frozendict built-in type"
    locator: Abstract; Rationale; Specification
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Map_types
    title: The Go Programming Language Specification
    locator: Map types
    accessed: 2026-11-16
---

## Synopsis

```python run id=basic since=3.15
config = frozendict(host="localhost", port=8080)
print(config["port"], config.get("user", "guest"))
print(list(config), len(config))
print(config)
```

```output for=basic
8080 guest
['host', 'port'] 2
frozendict({'host': 'localhost', 'port': 8080})
```

A frozendict is read like a dict, but it cannot be changed after it is made.

## Semantics

**r1.** "A new public immutable type frozendict is added to the builtins module."[^3] It "is not a dict subclass but inherits directly from object."[^1] Example: [`not-dict`](#not-dict).

**r2.** "A frozendict has a similar API to the dict API".[^1] The methods that change a dict are not there: `__delitem__()`, `__setitem__()`, `clear()`, `pop()`, `popitem()`, `setdefault()` and `update()`.[^1] Example: [`set-item`](#set-item).

**r3.** "Keys must be hashable, but values can be non-hashable. Using hashable values creates a hashable frozendict."[^3] "A frozendict can be hashed with hash(frozendict) if all keys and values can be hashed."[^1] Examples: [`as-key`](#as-key), [`unhashable-value`](#unhashable-value).

**r4.** "The insertion order is preserved."[^3] "The hash value does not depend on the items’ order."[^3] "Equality test does not depend on the items’ order either."[^3] Example: [`equal`](#equal).

**r5.** "frozendict |= other does not modify the frozendict in-place but creates a new frozen dictionary."[^1] Example: [`merge`](#merge).

**r6.** The constructor has the same three forms as dict: keyword arguments, a mapping, or an iterable of pairs, each with optional keyword arguments.[^2] "Creating a frozendict from a dict, frozendict(dict), has a complexity of O(n): items are copied (shallow copy)."[^3]

**r7.** PEP 814 gives reasons for the type: "Immutable mappings are hashable which allows their use as dictionary keys or set elements." and "Using an immutable mapping as a function parameter’s default value avoids the problem of mutable default values."[^3]

## Examples

```python run id=as-key since=3.15
prices = {
    frozendict(size="S", color="red"): 10,
    frozendict(size="L", color="red"): 12,
}
print(prices[frozendict(color="red", size="L")])
```

```output for=as-key
12
```

The lookup key has the items in a different order, but it is equal and has the same hash (rules r3 and r4).

```python run id=equal since=3.15
a = frozendict(x=1, y=2)
b = frozendict(y=2, x=1)
print(a == b, hash(a) == hash(b))
print(list(a), list(b))
print(a == dict(x=1, y=2))
```

```output for=equal
True True
['x', 'y'] ['y', 'x']
True
```

```python run id=merge since=3.15
d = frozendict(x=1)
copy = d
d |= frozendict(y=2)
print(d)
print(copy)
print(frozendict(x=1, y=2) | {"y": 5})
```

```output for=merge
frozendict({'x': 1, 'y': 2})
frozendict({'x': 1})
frozendict({'x': 1, 'y': 5})
```

`|=` binds `d` to a new frozendict. The name `copy` still refers to the old one (rule r5).

```python run id=not-dict since=3.15
fd = frozendict(a=1)
print(isinstance(fd, dict))
print(dict(fd), frozendict({"b": 2}, c=3))
```

```output for=not-dict
False
{'a': 1} frozendict({'b': 2, 'c': 3})
```

## Pitfalls

A frozendict cannot be changed in place:

```python fails id=set-item expect=exception:TypeError since=3.15
fd = frozendict(a=1)
fd["b"] = 2
```

Make a new one with `fd | {"b": 2}` (rule r5).

A frozendict with a list value is not hashable:

```python fails id=unhashable-value expect=exception:TypeError since=3.15
key = frozendict(tags=["a", "b"])
lookup = {key: 1}
```

Use a tuple for the value, for example `frozendict(tags=("a", "b"))` (rule r3).

## Comparison

Go has no immutable map type. The Go spec says that map elements "may be added during execution using assignments and retrieved with index expressions; they may be removed with the delete and clear built-in function."[^4] A Go map also cannot be a map key, because its key type must support `==`. A Python frozendict can be a key when all its items are hashable (rule r3).
