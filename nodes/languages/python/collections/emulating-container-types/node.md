---
tag: YC64W3
kind: feature
owner: python
slug: emulating-container-types
name: Emulating container types
aliases: [__len__, __getitem__, __setitem__, __delitem__, __contains__, __iter__, __reversed__, __missing__, custom container, sequence protocol, mapping protocol]
area: languages/python/collections
level: advanced
summary: >-
  A class becomes a container when it defines special methods such as __len__, __getitem__, __setitem__, __delitem__, __iter__ and __contains__. Then len(), indexing, slicing, in and for work on it.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#emulating-container-types", locator: "3.3.7. Emulating container types"}
see_also: [python/collections-abc, python/sequence-types, python/dictionaries, python/hashability, python/slicing]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#emulating-container-types
    title: The Python Language Reference, 3. Data model
    locator: 3.3.7. Emulating container types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#mapping-types-dict-frozendict
    title: Built-in Types, Python 3.15 documentation
    locator: dict, d[key] and __missing__()
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#overloading
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not support overloading of methods and operators?
    accessed: 2026-11-16
---

## Synopsis

```python run id=deck
class Deck:
    def __init__(self):
        self._cards = [r + s for s in "SH" for r in "AKQ"]

    def __len__(self):
        return len(self._cards)

    def __getitem__(self, index):
        return self._cards[index]

deck = Deck()
print(len(deck), deck[0], deck[-1], deck[:2])
print("QH" in deck, list(reversed(deck))[:2])
for card in deck:
    print(card, end=" ")
print()
```

```output for=deck
6 AS QH ['AS', 'KS']
True ['QH', 'KH']
AS KS QS AH KH QH
```

Two methods are enough for `len`, indexing, slicing, `in`, `reversed` and `for`.

## Semantics

**r1.** "The following methods can be defined to implement container objects. None of them are provided by the object class itself."[^1]

**r2.** `__len__` is "Called to implement the built-in function len(). Should return the length of the object, an integer >= 0. Also, an object that doesn’t define a __bool__() method and whose __len__() method returns zero is considered to be false in a Boolean context."[^1] Example: [`empty-false`](#empty-false).

**r3.** For `__getitem__`: "If subscript is of an inappropriate type, __getitem__() should raise TypeError. If subscript has an inappropriate value, __getitem__() should raise an LookupError or one of its subclasses (IndexError for sequences; KeyError for mappings)."[^1]

**r4.** "Slicing is handled by __getitem__(), __setitem__(), and __delitem__()."[^1] A call like `a[1:2] = b` "is translated to a[slice(1, 2, None)] = b".[^1] Example: [`slice-arg`](#slice-arg).

**r5.** "The sequence iteration protocol (used, for example, in for loops), expects that an IndexError will be raised for illegal indexes to allow proper detection of the end of a sequence."[^1] Example: [`deck`](#deck).

**r6.** "For objects that don’t define __contains__(), the membership test first tries iteration via __iter__(), then the old sequence iteration protocol via __getitem__()".[^1] For `__reversed__`: "If the __reversed__() method is not provided, the reversed() built-in will fall back to using the sequence protocol (__len__() and __getitem__())."[^1]

**r7.** "If a subclass of dict defines a method __missing__() and key is not present, the d[key] operation calls that method with the key key as argument."[^2] Example: [`missing`](#missing).

**r8.** "The collections.abc module provides a MutableMapping abstract base class to help create those methods from a base set of __getitem__(), __setitem__(), __delitem__(), and keys()."[^1]

## Examples

```python run id=slice-arg
class Show:
    def __getitem__(self, key):
        return key

s = Show()
print(s[3], s[1:2], s[::2], s[1, 2])
```

```output for=slice-arg
3 slice(1, 2, None) slice(None, None, 2) (1, 2)
```

A slice reaches `__getitem__` as a slice object, and a comma gives a tuple (rule r4).

```python run id=registry
class Registry:
    def __init__(self):
        self._items = {}

    def __setitem__(self, key, value):
        self._items[key.lower()] = value

    def __getitem__(self, key):
        return self._items[key.lower()]

    def __delitem__(self, key):
        del self._items[key.lower()]

    def __contains__(self, key):
        return key.lower() in self._items

    def __iter__(self):
        return iter(self._items)

    def __len__(self):
        return len(self._items)

r = Registry()
r["Host"] = "example.org"
r["PORT"] = 80
print(r["host"], "port" in r, list(r), len(r))
del r["HOST"]
print(list(r))
```

```output for=registry
example.org True ['host', 'port'] 2
['port']
```

This mapping ignores the case of its keys. `__contains__` searches keys, as a mapping should (rule r6).

```python run id=missing
class Counter(dict):
    def __missing__(self, key):
        return 0

c = Counter()
c["red"] += 1
c["red"] += 1
print(c["red"], c["blue"], dict(c))
```

```output for=missing
2 0 {'red': 2}
```

`c["blue"]` returns 0 but does not add the key (rule r7).

```python run id=empty-false
class Bag:
    def __init__(self, *items):
        self.items = list(items)

    def __len__(self):
        return len(self.items)

print(bool(Bag()), bool(Bag(1)))
```

```output for=empty-false
False True
```

## Pitfalls

`__len__` must return an int that is not negative:

```python fails id=negative-len expect=exception:ValueError
class Broken:
    def __len__(self):
        return -1

len(Broken())
```

A `__getitem__` that never raises IndexError makes an endless sequence:

```python wrong id=no-index-error
class Forever:
    def __getitem__(self, i):
        return i * i

for n in Forever():
    if n > 10:
        break
    print(n, end=" ")
print()
```

```output for=no-index-error
0 1 4 9
```

Without the `break`, the loop never stops (rule r5). Raise IndexError when the index is past the end.

## Comparison

Go has no operator overloading, so a Go type cannot support `x[i]` or `len(x)`. The Go FAQ says: "Regarding operator overloading, it seems more a convenience than an absolute requirement. Again, things are simpler without it."[^3] A Go container type uses ordinary methods instead. A Python class gets the built-in syntax from its special methods (rule r1).
