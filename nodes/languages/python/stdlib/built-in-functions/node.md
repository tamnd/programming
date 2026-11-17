---
tag: PQ77MS
kind: feature
owner: python
slug: built-in-functions
name: Built-in functions
aliases: [builtins, built-in function, len, print, sorted, enumerate, zip, map, filter, any, all, sum, round, isinstance, getattr, builtins module]
area: languages/python/stdlib
level: intro
summary: >-
  The built-in functions, from abs to zip, are always available with no import. They cover numbers, iteration, objects, input and output, and running code. They live in the builtins module.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html", locator: "Built-in functions"}
proposals: []
see_also: [python/built-in-types-reference, python/the-builtins-namespace, python/standard-library-map, python/sorting, python/looping-techniques, python/the-dir-function]
sources:
  - url: https://docs.python.org/3.15/builtins/functions.html
    title: Built-in functions, Python 3.15 documentation
    locator: Built-in functions; round(); sum(); zip(); all(); any(); sorted()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/index.html
    title: Python built-ins reference, Python 3.15 documentation
    locator: Python built-ins reference
    accessed: 2026-11-16
---

## Synopsis

```python run id=everyday
scores = {"ann": 91, "bob": 78, "cy": 85}
print(len(scores), sum(scores.values()), max(scores, key=scores.get))
ranked = sorted(scores.items(), key=lambda pair: pair[1], reverse=True)
for rank, (name, score) in enumerate(ranked, start=1):
    print(rank, name, score)
```

```output for=everyday
3 254 ann
1 ann 91
2 cy 85
3 bob 78
```

## Semantics

**r1.** "The Python interpreter has a number of functions and types built into it that are always available."[^1] They are "objects that can be used by all Python code without the need of an import statement."[^2] Example: [`objects`](#objects).

**r2.** This page groups them by task:

- Numbers: `abs`, `divmod`, `pow`, `round`, `sum`, `min`, `max`, `bin`, `hex`, `oct`.
- Iteration: `iter`, `next`, `aiter`, `anext`, `len`, `range`, `enumerate`, `zip`, `map`, `filter`, `reversed`, `sorted`, `all`, `any`.
- Types: `bool`, `int`, `float`, `complex`, `str`, `bytes`, `bytearray`, `memoryview`, `list`, `tuple`, `dict`, `set`, `frozenset`, `frozendict`, `object`, `slice`, `type`.
- Objects and attributes: `getattr`, `setattr`, `delattr`, `hasattr`, `isinstance`, `issubclass`, `callable`, `id`, `hash`, `dir`, `vars`, `repr`, `ascii`, `format`, `chr`, `ord`.
- Classes: `classmethod`, `staticmethod`, `property`, `super`.
- Input and output: `print`, `input`, `open`.
- Code and the interpreter: `compile`, `eval`, `exec`, `globals`, `locals`, `__import__`, `breakpoint`, `help`, `sentinel`.

**r3.** `all` returns "True if all elements of the iterable are true (or if the iterable is empty)". `any` returns "True if any element of the iterable is true. If the iterable is empty, return False."[^1] Example: [`numbers`](#numbers).

**r4.** "The built-in sorted() function is guaranteed to be stable."[^1] Its `key` and `reverse` arguments "must be specified as keyword arguments."[^1] Example: [`everyday`](#everyday).

**r5.** "zip() is lazy: The elements won’t be processed until the iterable is iterated on".[^1] "By default, zip() stops when the shortest iterable is exhausted."[^1] Example: [`iterate`](#iterate).

**r6.** For `round`, "if two multiples are equally close, rounding is done toward the even choice".[^1] Example: [`round-half-even`](#round-half-even).

## Examples

```python run id=iterate
names = ["ann", "bob", "cy"]
ages = [31, 27, 45]
print(list(zip(names, ages)))
print(list(map(str.upper, names)))
print(list(filter(lambda a: a > 30, ages)))
print(list(reversed(names)), any(a > 40 for a in ages), all(a > 40 for a in ages))
```

```output for=iterate
[('ann', 31), ('bob', 27), ('cy', 45)]
['ANN', 'BOB', 'CY']
[31, 45]
['cy', 'bob', 'ann'] True False
```

`zip`, `map`, `filter` and `reversed` return iterators, so `list` collects the items (rule r5).

```python run id=numbers
print(any([]), all([]))
print(divmod(17, 5), divmod(-7, 2))
print(pow(3, 4), pow(3, 4, 5), abs(-2.5))
print(min([], default=None), max(3, 9, 4))
```

```output for=numbers
False True
(3, 2) (-4, 1)
81 1 2.5
None 9
```

`all([])` is `True` and `any([])` is `False` (rule r3). For integers, `divmod(a, b)` "is the same as (a // b, a % b)", so the remainder has the sign of `b`.[^1] `pow(3, 4, 5)` is the power modulo 5, and `default` gives `min` a value for an empty input.

```python run id=objects
import builtins

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

p = Point(1, 2)
print(isinstance(p, Point), callable(Point), hasattr(p, "z"))
setattr(p, "z", 3)
print(getattr(p, "z"), vars(p))
print(builtins.len is len, type(len).__name__)
```

```output for=objects
True True False
3 {'x': 1, 'y': 2, 'z': 3}
True builtin_function_or_method
```

The attribute functions take the name as a string. Each built-in is an attribute of the `builtins` module (rule r1).

## Versions

> [!since v="3.10"]
> `zip` has the `strict` argument.[^1]

> [!since v="3.15"]
> `frozendict` and `sentinel` are new built-ins.[^1]

## Pitfalls

```python wrong id=round-half-even
print(round(0.5), round(1.5), round(2.5))
print(round(2.675, 2))
```

```output for=round-half-even
0 2 2
2.67
```

`round(2.5)` is `2`, not `3`, because ties go to the even number (rule r6). "round(2.675, 2) gives 2.67 instead of the expected 2.68. This is not a bug: it’s a result of the fact that most decimal fractions can’t be represented exactly as a float."[^1] Use `decimal` for money.

```python fails id=zip-strict expect=exception:ValueError
names = ["ann", "bob", "cy"]
ages = [31, 27]
print(list(zip(names, ages)))
print(list(zip(names, ages, strict=True)))
```

The plain `zip` drops the last name with no error. "Without the strict=True argument, any bug that results in iterables of different lengths will be silenced".[^1] With `strict=True`, the short input raises `ValueError`.

```python fails id=sum-strings expect=exception:TypeError
print(sum([[1], [2, 3]], start=[]))
print(sum(["a", "b"], ""))
```

`sum` adds lists with `start=[]`, but "the start value is not allowed to be a string."[^1] "The preferred, fast way to concatenate a sequence of strings is by calling ''.join(sequence)."[^1]

## Comparison

Other languages put many of these operations on the types as methods. In Python, `len(x)` calls `x.__len__()` and `sorted` works on any iterable, so one function serves every type. See [[python/the-builtins-namespace]] for how the names are found, and [[python/built-in-types-reference]] for the methods of each type.
