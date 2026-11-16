---
tag: XJN0DJ
kind: feature
owner: python
slug: set-and-dict-comprehensions
name: Set and dict comprehensions
aliases: [set comprehension, dict comprehension, dictionary comprehension, setcomp, dictcomp, invert a dict, no tuple comprehension]
area: languages/python/collections
level: core
summary: >-
  A set comprehension is a comprehension in curly braces, and a dict comprehension has a key: value pair before its for clause. They build a set or dict the same way a list comprehension builds a list.
grammar: "setcomp: '{' comprehension '}'; dictcomp: '{' kvpair for_if_clause+ '}'"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#comprehensions", locator: "6.2.6. Comprehensions"}
see_also: [python/list-comprehensions, python/sets, python/dictionaries, python/generator-expressions, python/comprehension-scope]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6. Comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#dictionaries
    title: The Python Tutorial, 5. Data Structures
    locator: 5.4. Sets; 5.5. Dictionaries
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0274/
    title: "PEP 274: Dict Comprehensions"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Map_types
    title: The Go Programming Language Specification
    locator: Map types
    accessed: 2026-11-16
---

## Synopsis

```python run id=both
letters = {x for x in "abracadabra" if x not in "abc"}
print(sorted(letters))

squares = {x: x ** 2 for x in (2, 4, 6)}
print(squares)
```

```output for=both
['d', 'r']
{2: 4, 4: 16, 6: 36}
```

The set comprehension keeps one copy of each letter. The dict comprehension makes a key and a value for each item.

## Semantics

**r1.** "List, set and dictionary comprehensions are a form of container displays where items are computed via a set of looping and filtering instructions rather than listed explicitly."[^1]

**r2.** "Set comprehensions work similarly." A set comprehension roughly corresponds to a loop that calls `result.add()` on a new set.[^1] Example: [`lower`](#lower).

**r3.** "Dictionary comprehensions start with a colon-separated key-value pair instead of an expression."[^1] "As in other kinds of dictionary displays, the same key may be specified multiple times. Earlier values are overwritten by ones that are evaluated later."[^1] Example: [`invert`](#invert).

**r4.** "There are no tuple comprehensions. A similar syntax is instead used for generator expressions, from which you can construct a tuple like this".[^1] Example: [`tuple`](#tuple).

**r5.** PEP 274 says: "You can use dict comprehensions in ways very similar to list comprehensions, except that they produce Python dictionary objects instead of list objects."[^3]

**r6.** The key of each dict entry must be hashable. "You can’t use lists as keys, since lists can be modified in place using index assignments, slice assignments, or methods like append() and extend()."[^2] Example: [`list-key`](#list-key).

## Examples

```python run id=lower
print(sorted({x.lower() for x in ["a", "A", "b", "C"]}))
```

```output for=lower
['a', 'b', 'c']
```

```python run id=from-pairs
prices = [("apple", 3), ("pear", 5), ("fig", 2)]
cheap = {name: p for name, p in prices if p < 5}
print(cheap)
print({name: len(name) for name, _ in prices})
```

```output for=from-pairs
{'apple': 3, 'fig': 2}
{'apple': 5, 'pear': 4, 'fig': 3}
```

The `for` clause can unpack each pair, and an `if` clause drops entries.

```python run id=tuple
print(tuple(x ** 2 for x in range(5)))
```

```output for=tuple
(0, 1, 4, 9, 16)
```

## Pitfalls

When two items give the same key, the last one wins:

```python wrong id=invert
ages = {"ann": 30, "bob": 25, "cy": 30}
by_age = {age: name for name, age in ages.items()}
print(by_age)
```

```output for=invert
{30: 'cy', 25: 'bob'}
```

`"ann"` is lost (rule r3). To keep all names, collect them in lists, for example with `collections.defaultdict(list)`.

A key that is a list is a TypeError:

```python fails id=list-key expect=exception:TypeError
groups = {[n, n + 1]: n for n in range(3)}
```

Use a tuple as the key (rule r6).

## Comparison

Go has no comprehension syntax. A Go program makes a map with `make` and fills it in a loop. The Go spec says: "A map is an unordered group of elements of one type, called the element type, indexed by a set of unique keys of another type, called the key type."[^4] A Python dict comprehension builds the dict in one expression (rule r3).
