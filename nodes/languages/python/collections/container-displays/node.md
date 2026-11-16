---
tag: ET0442
kind: feature
owner: python
slug: container-displays
name: Container displays
aliases: [display, list literal, set literal, dict literal, tuple literal, trailing comma, empty set, star unpacking in a display, double star in a dict display]
area: languages/python/collections
level: core
summary: >-
  A display builds a list, set, tuple or dict from items in brackets, braces or parentheses. Items are evaluated left to right, a trailing comma is allowed, and * or ** unpacks an iterable or mapping.
grammar: "list: '[' [flexible_expression_list] ']'; set: '{' flexible_expression_list '}'; dict: '{' [double_starred_kvpairs] '}'"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#container-displays", locator: "6.2.5. Container displays"}
see_also: [python/lists, python/sets, python/dictionaries, python/tuples-and-sequences, python/set-and-dict-comprehensions, python/unpacking-in-lists-and-list-comprehensions]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#container-displays
    title: The Python Language Reference, Python 3.15
    locator: 6.2.4. Parenthesized groups; 6.2.5. Container displays
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0448/
    title: "PEP 448: Additional Unpacking Generalizations"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Composite_literals
    title: The Go Programming Language Specification
    locator: Composite literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=four-kinds
print(["one", "two", 1 + 2])
print(sorted({"b", "a", "b"}))
print((1, "x"), ("single",))
print({1: "one", 2: "two"})
```

```output for=four-kinds
['one', 'two', 3]
['a', 'b']
(1, 'x') ('single',)
{1: 'one', 2: 'two'}
```

Square brackets make a list, braces make a set or a dict, and parentheses with a comma make a tuple.

## Semantics

**r1.** "For constructing builtin containers (lists, sets, tuples or dictionaries), Python provides special syntax called displays."[^1] "All displays, however, consist of comma-separated items enclosed in paired delimiters."[^1]

**r2.** "In list, tuple and dictionary (but not set) displays, the series may be empty".[^1] "There is no special syntax for the empty set. The {} literal is a dictionary display that constructs an empty dictionary. Call set() with no arguments to get an empty set."[^1] Example: [`empty`](#empty).

**r3.** "If the series is not empty, the items may be followed by an additional comma, which has no effect".[^1] Example: [`trailing`](#trailing).

**r4.** "To avoid ambiguity, if a tuple display has exactly one element, it requires a trailing comma. Without it, you get a parenthesized group".[^1] Example: [`single`](#single).

**r5.** "At runtime, when a display is evaluated, the listed items are evaluated from left to right and placed into a new container of the appropriate type."[^1]

**r6.** "For tuple, list and set (but not dict) displays, any item in the display may be prefixed with an asterisk (*). This denotes iterable unpacking."[^1] In a dict display, `**` denotes dictionary unpacking, and "later values replace values already set by earlier items and unpackings."[^1] Example: [`unpack`](#unpack).

**r7.** In a dict display, "you can specify the same key multiple times in the comprehension, and the final dictionary’s value for a given key will be the last one given."[^1] Example: [`duplicate-key`](#duplicate-key).

**r8.** PEP 448 "proposes extended usages of the * iterable unpacking operator and ** dictionary unpacking operators to allow unpacking in more positions, an arbitrary number of times, and in function calls and displays."[^2]

## Examples

```python run id=empty
print([], (), {}, set())
print(type({}).__name__, type(set()).__name__)
```

```output for=empty
[] () {} set()
dict set
```

```python run id=trailing
colors = [
    "red",
    "green",
    "blue",
]
print(colors)
```

```output for=trailing
['red', 'green', 'blue']
```

A trailing comma lets a later edit add a line without a change to the line before it (rule r3).

```python run id=unpack
numbers = (1, 2)
print([*numbers, "word", *numbers])
print({*"aba", "c"} == {"a", "b", "c"})
defaults = {"color": "blue", "count": 8}
overrides = {"color": "yellow"}
print({**defaults, **overrides})
```

```output for=unpack
[1, 2, 'word', 1, 2]
True
{'color': 'yellow', 'count': 8}
```

The value from `overrides` replaces the default value (rule r6).

```python run id=duplicate-key
print({1: "this will be overridden", 2: "two", 1: "one"})
```

```output for=duplicate-key
{1: 'one', 2: 'two'}
```

## Pitfalls

Parentheses without a comma do not make a tuple:

```python wrong id=single
point = ("single")
print(point, len(point))
```

```output for=single
single 6
```

`("single")` is a string in a group, and its length is 6. Write `("single",)` for a one-item tuple (rule r4).

A single `*` in braces makes a set, not a dict:

```python fails id=star-in-dict expect=exception:TypeError
pairs = [("a", 1)]
d = {*pairs}
print(d["a"])
```

`{*pairs}` is a set display (rule r6), and a set has no indexing. Use `dict(pairs)` to make a dict from pairs.

## Comparison

Go uses composite literals: "Composite literals construct new values for structs, arrays, slices, and maps each time they are evaluated."[^3] A Go literal names its type, for example `[]int{1, 2}` or `map[string]int{"a": 1}`. A Python display gets its type from the delimiters, and a set or tuple display has no direct Go form.
