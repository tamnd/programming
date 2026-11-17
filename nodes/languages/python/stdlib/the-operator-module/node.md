---
tag: D4ETYW
kind: feature
owner: python
slug: the-operator-module
name: The operator module
aliases: [operator, itemgetter, attrgetter, methodcaller, operator.add, operator.iadd, operators as functions]
area: languages/python/stdlib
level: core
summary: >-
  The operator module gives each Python operator as a function, such as add for + and lt for <. Its itemgetter, attrgetter and methodcaller make small key functions for sorted, map and max.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/operator.html", locator: "operator, Standard operators as functions"}
proposals: []
see_also: [python/sorting, python/functools, python/lambda-expressions, python/operator-overloading, python/special-methods, python/mathematics]
sources:
  - url: https://docs.python.org/3.15/library/operator.html
    title: operator, Standard operators as functions, Python 3.15 documentation
    locator: Introduction; attrgetter(); itemgetter(); methodcaller(); Mapping Operators to Functions; In-place Operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import operator
from functools import reduce

print(operator.add(2, 3), operator.lt(2, 3), operator.neg(4))
print(list(map(operator.mul, [1, 2, 3], [4, 5, 6])))
print(reduce(operator.mul, [1, 2, 3, 4]))
```

```output for=basics
5 True -4
[4, 10, 18]
24
```

## Semantics

**r1.** "The operator module exports a set of efficient functions corresponding to the intrinsic operators of Python. For example, operator.add(x, y) is equivalent to the expression x+y."[^1] Example: [`basics`](#basics).

**r2.** "Many function names are those used for special methods, without the double underscores." "The variants without the double underscores are preferred for clarity."[^1] A name that is a keyword has a trailing underscore, as in `and_`, `or_`, `not_` and `is_`.

**r3.** The functions fall into four groups: "object comparisons, logical operations, mathematical operations and sequence operations."[^1] The comparison functions "can return any value, which may or may not be interpretable as a Boolean value."[^1] Example: [`logic`](#logic).

**r4.** `itemgetter`, `attrgetter` and `methodcaller` make callables. They "are useful for making fast field extractors as arguments for map(), sorted(), itertools.groupby(), or other functions that expect a function argument."[^1] Example: [`getters`](#getters).

**r5.** `itemgetter(item)` returns "a callable object that fetches item from its operand using the operand’s __getitem__() method. If multiple items are specified, returns a tuple of lookup values."[^1] `attrgetter` does the same for attributes, and "The attribute names can also contain dots."[^1]

**r6.** `methodcaller(name, ...)` returns "a callable object that calls the method name on its operand."[^1] The extra arguments go to the method.

**r7.** The in-place functions do only half of an augmented assignment: "the statement x += y is equivalent to x = operator.iadd(x, y)". "The in-place functions listed below only do the first step, calling the in-place method. The second step, assignment, is not handled."[^1] Example: [`inplace-immutable`](#inplace-immutable).

## Examples

```python run id=getters
from operator import itemgetter, attrgetter, methodcaller
from dataclasses import dataclass

rows = [("bob", 27, "Hanoi"), ("ann", 31, "Hue"), ("cy", 27, "Hue")]
print(sorted(rows, key=itemgetter(1, 0)))
print(list(map(itemgetter(0), rows)))

@dataclass
class User:
    name: str
    age: int

users = [User("bob", 27), User("ann", 31)]
print([u.name for u in sorted(users, key=attrgetter("age"), reverse=True)])
print(list(map(methodcaller("upper"), ["ann", "bob"])))
print(methodcaller("split", ",")("a,b,c"))
```

```output for=getters
[('bob', 27, 'Hanoi'), ('cy', 27, 'Hue'), ('ann', 31, 'Hue')]
['bob', 'ann', 'cy']
['ann', 'bob']
['ANN', 'BOB']
['a', 'b', 'c']
```

`itemgetter(1, 0)` returns a tuple, so the rows sort by age and then by name (rule r5). Each getter does the same work as a short `lambda`, such as `lambda row: (row[1], row[0])`.

```python run id=logic
import operator

values = [0, None, "", "x", 3]
print([operator.truth(v) for v in values])
print(list(filter(operator.is_not_none, values)))
print(operator.call(max, 3, 9, 4), operator.getitem({"k": 1}, "k"))
```

```output for=logic
[False, False, False, True, True]
[0, '', 'x', 3]
9 1
```

`truth(obj)` "is equivalent to using the bool constructor."[^1] `is_not_none` keeps the false values that are not `None`. `call(f, *args)` returns `f(*args)`.

## Versions

> [!since v="3.11"]
> `operator.call` is new.[^1]

> [!since v="3.14"]
> `operator.is_none` and `operator.is_not_none` are new.[^1]

## Pitfalls

```python wrong id=inplace-immutable
import operator

a = "hello"
print(operator.iadd(a, " world"), a)
s = ["h", "i"]
operator.iadd(s, ["!"])
print(s)
```

```output for=inplace-immutable
hello world hello
['h', 'i', '!']
```

For a string, `iadd` returns the new value but `a` does not change. "For immutable targets such as strings, numbers, and tuples, the updated value is computed, but not assigned back to the input variable".[^1] For a list, the list changes in place (rule r7). Assign the result: `a = operator.iadd(a, " world")`.

```python wrong id=and-is-bitwise
import operator

print(2 and 1, operator.and_(2, 1))
print(0 or 4, operator.or_(0, 4))
```

```output for=and-is-bitwise
1 0
4 4
```

`operator.and_(a, b)` returns `a & b`, the bitwise and, not the result of the `and` keyword.[^1] `2 & 1` is `0`. The keywords `and` and `or` do not evaluate the right operand when the left one decides the result, so no function can act the same way: the arguments of a call are always evaluated first.

```python fails id=missing-attribute expect=exception:AttributeError
from operator import attrgetter

class Point:
    def __init__(self, x):
        self.x = x

get_y = attrgetter("y")
print(get_y(Point(1)))
```

`attrgetter("y")` checks nothing when it is made. The error comes when the getter runs on an object that has no `y`.

## Comparison

A `lambda` can do each of these jobs, as in `key=lambda u: u.age` (see [[python/lambda-expressions]]). The operator functions are shorter for the common cases and have a readable name. For how `+` and `<` call the special methods of a class, see [[python/operator-overloading]]. For `reduce` and other higher-order functions, see [[python/functools]].
