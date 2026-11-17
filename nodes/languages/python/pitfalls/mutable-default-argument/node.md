---
tag: G8F8W7
kind: feature
owner: python
slug: mutable-default-argument
name: Mutable default argument
aliases: [mutable default, shared default list, default argument shared between calls, B006, mutable-argument-default, dangerous-default-value]
area: languages/python/pitfalls
level: core
summary: >-
  Python evaluates a default value once, when the def statement runs. A list or dict default is one object that all calls share, so a change in one call shows in the next. Use None and make a new one.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7 Function definitions"}
proposals: []
see_also: [python/default-argument-values, python/defining-functions, python/function-objects, python/dataclasses, python/shared-mutable-class-attributes, python/none-notimplemented-and-ellipsis, python/functools]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15 documentation
    locator: 8.7 Function definitions
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/faq/programming.html#why-are-default-values-shared-between-objects
    title: Programming FAQ, Python 3.15 documentation
    locator: Why are default values shared between objects?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#default-argument-values
    title: The Python Tutorial, Python 3.15 documentation
    locator: 4.9.1 Default Argument Values
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/datamodel.html#function.__defaults__
    title: The Python Language Reference, Python 3.15 documentation
    locator: 3.2.8.1 User-defined functions, function.__defaults__
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/mutable-argument-default/
    title: mutable-argument-default (B006), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/warning/dangerous-default-value.html
    title: dangerous-default-value / W0102, Pylint documentation
    locator: Description
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/dataclasses.html#mutable-default-values
    title: dataclasses, Data Classes, Python 3.15 documentation
    locator: Mutable default values
    accessed: 2026-11-18
---

## Synopsis

```python run id=shared
def add(item, bag=[]):
    bag.append(item)
    return bag


print(add(1))
print(add(2))
print(add(3, []))
print(add(4))
```

```output for=shared
[1]
[1, 2]
[3]
[1, 2, 4]
```

The second call did not get a new empty list. It got the list that the first call changed.

## Semantics

**r1.** "Default parameter values are evaluated from left to right when the function definition is executed. This means that the expression is evaluated once, when the function is defined, and that the same “pre-computed” value is used for each call."[^1] The `def` statement makes the default object, not the call.

**r2.** So a mutable default is shared. "if the function modifies the object (e.g. by appending an item to a list), the default parameter value is in effect modified. This is generally not what was intended."[^1] The FAQ says: "Default values are created exactly once, when the function is defined. If that object is changed, like the dictionary in this example, subsequent calls to the function will refer to this changed object."[^2] Example: [`shared`](#shared).

**r3.** The tutorial gives the same warning: "The default value is evaluated only once. This makes a difference when the default is a mutable object such as a list, dictionary, or instances of most classes."[^3]

**r4.** The default objects are stored on the function. `__defaults__` is "A tuple containing default parameter values for those parameters that have defaults, or None if no parameters have a default value."[^4] Example: [`defaults-tuple`](#defaults-tuple).

**r5.** Immutable defaults are safe: "By definition, immutable objects such as numbers, strings, tuples, and None, are safe from change."[^2] A tuple that holds a list is not safe, because the list can still change.

**r6.** The fix: "use None as the default value and inside the function, check if the parameter is None and create a new list/dictionary/whatever if it is."[^2] When `None` is a valid argument, use a private sentinel object instead. Example: [`none-sentinel`](#none-sentinel).

**r7.** A default expression that is not mutable can still surprise. A call such as `next(counter)` or `time.time()` in a default runs once, so each call sees the same value. Example: [`evaluated-once`](#evaluated-once).

**r8.** Linters find the trap. Ruff rule B006 "Checks for uses of mutable objects as function argument defaults."[^5] Its reason: "The same mutable object is then shared across all calls to the function. If the object is modified, those modifications will persist across calls, which can lead to unexpected behavior."[^5] Pylint reports the same case as W0102 `dangerous-default-value`, "Used when a mutable value as list or dictionary is detected in a default value for an argument."[^6]

**r9.** The behavior can be useful. The FAQ shows a cache in a default: "This is called “memoizing”".[^2] Make such a parameter keyword-only and give it a leading underscore, so that callers do not pass it. Example: [`memoize`](#memoize).

## Examples

```python run id=defaults-tuple
def add(item, bag=[]):
    bag.append(item)
    return bag


print(add.__defaults__)
add("x")
add("y")
print(add.__defaults__)
print(add.__defaults__[0] is add("z"))
```

```output for=defaults-tuple
([],)
(['x', 'y'],)
True
```

The list in `__defaults__` is the same object that each call returns (rule r4).

```python run id=none-sentinel
def add(item, bag=None):
    if bag is None:
        bag = []
    bag.append(item)
    return bag


print(add(1), add(2))

_MISSING = object()


def lookup(table, key, default=_MISSING):
    if key in table:
        return table[key]
    if default is _MISSING:
        raise KeyError(key)
    return default


print(lookup({"a": 1}, "b", None))
```

```output for=none-sentinel
[1] [2]
None
```

Each call of `add` makes a new list (rule r6). `lookup` lets the caller pass `None` as a real default, so it uses its own object `_MISSING` to mean that no argument was given.

```python run id=memoize
calls = 0


def square(n, *, _cache={}):
    global calls
    if n not in _cache:
        calls += 1
        _cache[n] = n * n
    return _cache[n]


print(square(4), square(4), square(5), calls)
```

```output for=memoize
16 16 25 2
```

The shared dictionary is the point here, so the default is a cache (rule r9). The second call with 4 does not compute again. For real code, `functools.cache` does the same job.

## Versions

> [!since v="3.11"]
> In a data class, a field default that is not hashable raises `ValueError`. "Instead of looking for and disallowing objects of type list, dict, or set, unhashable objects are now not allowed as default values. Unhashability is used to approximate mutability."[^7]

## Pitfalls

```python wrong id=evaluated-once
import itertools

counter = itertools.count(1)


def new_order(number=next(counter)):
    return number


print(new_order(), new_order(), next(counter))
```

```output for=evaluated-once
1 1 2
```

`next(counter)` ran once, when `def` ran. Each call gets 1 (rule r7). Use `number=None` and call `next(counter)` in the body.

```python fails id=dataclass-list expect=exception:ValueError
from dataclasses import dataclass


@dataclass
class Order:
    items: list = []
```

A data class would share the list in the same way, so `@dataclass` rejects it: it "will raise a ValueError if it detects an unhashable default parameter."[^7] Use `items: list = field(default_factory=list)`. For class attributes, see [[python/shared-mutable-class-attributes]].

## Comparison

In some other languages, a default expression is evaluated at each call, so `[]` is a new list each time. In Python, it is evaluated once at definition time (rule r1). The behavior follows from the fact that a default is a value stored on the function object, not code that runs in the call. For the defaults themselves, see [[python/default-argument-values]].
