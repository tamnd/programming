---
tag: Q8RWYQ
kind: feature
owner: python
slug: unpacking-in-lists-and-list-comprehensions
name: Unpacking in lists and list comprehensions
aliases: [star in a list display, iterable unpacking in a list, unpacking in comprehensions, flatten with star, PEP 798, Value after * must be an iterable]
area: languages/python/collections
level: core
summary: >-
  A * before an item of a list, tuple or set display adds the items of an iterable. A starred expression in a list or set comprehension, and ** in a dict comprehension, joins iterables or dicts.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#unpacking-in-comprehensions", locator: "6.2.5. Container displays; 6.2.6.3. Unpacking in comprehensions"}
see_also: [python/container-displays, python/list-comprehensions, python/unpacking-argument-lists, python/iterable-unpacking, python/set-and-dict-comprehensions]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#unpacking-in-lists-and-list-comprehensions
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1.5. Unpacking in Lists and List Comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#unpacking-in-comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.5. Container displays; 6.2.6.3. Unpacking in comprehensions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0798/
    title: "PEP 798: Unpacking in Comprehensions"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0448/
    title: "PEP 448: Additional Unpacking Generalizations"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Appending_and_copying_slices
    title: The Go Programming Language Specification
    locator: Appending to and copying slices
    accessed: 2026-11-16
---

## Synopsis

```python run id=display
x = [1, 2, 3]
print([0, *x, 4, 5, 6])
print((*"ab", *range(2)))
print({*x, *[3, 4]} == {1, 2, 3, 4})
```

```output for=display
[0, 1, 2, 3, 4, 5, 6]
('a', 'b', 0, 1)
True
```

Each starred item adds all the items of its iterable at that place in the new container.

## Semantics

**r1.** "For tuple, list and set (but not dict) displays, any item in the display may be prefixed with an asterisk (*). This denotes iterable unpacking. At runtime, the asterisk-prefixed expression must evaluate to an iterable, whose contents are inserted into the container at the location of the unpacking."[^2] Example: [`display`](#display).

**r2.** "This only works if the expression following the * evaluates to an iterable object; trying to unpack a non-iterable object will raise an exception".[^1] Example: [`not-iterable`](#not-iterable).

**r3.** "If the expression of a list or set comprehension is starred, the result will be unpacked to produce zero or more elements."[^2] The comprehension is roughly a loop that calls `result.extend(people)` for each item.[^2] Example: [`flatten`](#flatten).

**r4.** "In dict comprehensions, a double-starred expression will be evaluated and then unpacked using dictionary unpacking, inserting zero or more key/value pairs into the new dictionary."[^2] Example: [`merge-dicts`](#merge-dicts).

**r5.** PEP 798 extends "list, set, and dictionary comprehensions, as well as generator expressions, to allow unpacking notation (* and **) at the start of the expression".[^3] Unpacking in displays came earlier, from PEP 448.[^4]

## Examples

```python run id=flatten since=3.15
x = [[1, 2, 3], [4, 5, 6], [], [7], [8, 9]]
print([*element for element in x])

mixed = [[1, 2], "cat", {"spam": "eggs"}]
print([*element for element in mixed])
```

```output for=flatten
[1, 2, 3, 4, 5, 6, 7, 8, 9]
[1, 2, 'c', 'a', 't', 'spam']
```

Each item of `x` is unpacked into the new list (rule r3). A dict gives its keys.

```python run id=merge-dicts since=3.15
system_defaults = {"color": "blue", "count": 8}
user_defaults = {"color": "yellow"}
overrides = {"count": 5}
configuration_sets = [system_defaults, user_defaults, overrides]
print({**d for d in configuration_sets})
```

```output for=merge-dicts
{'color': 'yellow', 'count': 5}
```

A later dict replaces the value of a key that an earlier dict set (rule r4).

```python run id=set-union since=3.15
fruits = [{"apple", "avocado"}, {"banana"}, {"apple"}]
print(sorted({*group for group in fruits}))
```

```output for=set-union
['apple', 'avocado', 'banana']
```

```python run id=older-form
x = [[1, 2, 3], [4, 5, 6], [], [7], [8, 9]]
print([n for element in x for n in element])
```

```output for=older-form
[1, 2, 3, 4, 5, 6, 7, 8, 9]
```

Two `for` clauses give the same flat list, and this form also works before Python 3.15 (rule r5).

## Pitfalls

A starred item must be an iterable:

```python fails id=not-iterable expect=exception:TypeError
x = 1
items = [0, *x, 2, 3, 4]
```

Write `[0, x, 2, 3, 4]` to add the value itself (rule r2).

A dict display cannot use a single `*`. Use `**` for a mapping, and `*` only in tuple, list and set displays (rule r1).

## Comparison

Go has no unpacking in a slice literal. A Go program joins slices with `append` and the `...` form: "If the capacity of s is not large enough to fit the additional values, append allocates a new, sufficiently large underlying array that fits both the existing slice elements and the additional values."[^5] So `append(append([]int{0}, x...), 4, 5, 6)` gives the same values as the Python display `[0, *x, 4, 5, 6]` (rule r1).
