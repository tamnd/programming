---
tag: WJ6ZDG
kind: feature
owner: python
slug: unpacking-argument-lists
name: Unpacking argument lists
aliases: [star in a call, double star in a call, splat operator, unpack a list into arguments, unpack a dict into keyword arguments, f(*args), PEP 448]
area: languages/python/functions
level: core
summary: >-
  In a call, *iterable gives the items of the iterable as positional arguments, and **mapping gives the items of the mapping as keyword arguments. A call can use several of each.
grammar: "positional_item: assignment_expression | '*' expression; keywords_arguments: (keyword_item | '**' expression) (',' keyword_item | ',' '**' expression)* (Language Reference, 6.3.3)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#calls", locator: "6.3.3. Calls"}
see_also: [python/arbitrary-argument-lists, python/keyword-arguments, python/iterable-unpacking, python/call-semantics]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#unpacking-argument-lists
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.5. Unpacking Argument Lists
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#calls
    title: The Python Language Reference, Python 3.15
    locator: 6.3.3. Calls
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0448/
    title: "PEP 448: Additional Unpacking Generalizations"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Passing_arguments_to_..._parameters
    title: The Go Programming Language Specification
    locator: Passing arguments to ... parameters
    accessed: 2026-11-16
---

## Synopsis

```python run id=range-args
args = [3, 6]
print(list(range(*args)))

def parrot(voltage, state="a stiff", action="voom"):
    return f"{action}: {voltage} volts, {state}"

d = {"voltage": "four million", "state": "demised", "action": "VOOM"}
print(parrot(**d))
```

```output for=range-args
[3, 4, 5]
VOOM: four million volts, demised
```

`*args` gives the list items as positional arguments. `**d` gives the dict items as keyword arguments.

## Semantics

**r1.** "If the syntax *expression appears in the function call, expression must evaluate to an iterable. Elements from these iterables are treated as if they were additional positional arguments."[^2] Example: [`range-args`](#range-args).

**r2.** "If the syntax **expression appears in the function call, expression must evaluate to a mapping, the contents of which are treated as additional keyword arguments."[^2] Example: [`range-args`](#range-args).

**r3.** "If a parameter matching a key has already been given a value (by an explicit keyword argument, or from another unpacking), a TypeError exception is raised."[^2] Example: [`duplicate`](#duplicate).

**r4.** "When **expression is used, each key in this mapping must be a string."[^2]

**r5.** "although the *expression syntax may appear after explicit keyword arguments, it is processed before the keyword arguments".[^2] Example: [`order`](#order).

**r6.** PEP 448 proposes "extended usages of the * iterable unpacking operator and ** dictionary unpacking operators to allow unpacking in more positions, an arbitrary number of times".[^3] Example: [`several`](#several).

## Examples

```python run id=several
def show(*args, **kwargs):
    print(args, kwargs)

show(*[1, 2], 3, *"ab", **{"x": 1}, y=2, **{"z": 3})
```

```output for=several
(1, 2, 3, 'a', 'b') {'x': 1, 'y': 2, 'z': 3}
```

A call can have several `*` and `**` items, and plain arguments can come between them (rule r6).

```python run id=order
def f(a, b):
    print(a, b)

f(b=1, *(2,))
f(1, *(2,))
```

```output for=order
2 1
1 2
```

This is the example of the reference. The `*(2,)` comes after `b=1`, but it fills `a` first (rule r5).

```python run id=zip-transpose
rows = [(1, 2, 3), (4, 5, 6)]
print(list(zip(*rows)))
```

```output for=zip-transpose
[(1, 4), (2, 5), (3, 6)]
```

`zip(*rows)` gives each row as one argument, so `zip` pairs the columns.

## Pitfalls

Two values for one parameter, from an unpacking and from a keyword, are a TypeError:

```python fails id=duplicate expect=exception:TypeError
def f(a, b):
    pass

f(**{"a": 1}, a=2)
```

A dict that has a key with no matching parameter is a TypeError, if the function has no `**` parameter:

```python fails id=extra-key expect=exception:TypeError
def point(x, y):
    return (x, y)

point(**{"x": 1, "y": 2, "z": 3})
```

## Comparison

Go can pass a slice to a variadic parameter with `...` after the argument: "If the final argument is assignable to a slice type []T and is followed by ..., it is passed unchanged as the value for a ...T parameter. In this case no new slice is created."[^4] This works only for the final variadic parameter. A Python `*` unpacking can fill any positional parameters, and a call can use it several times (rules r1 and r6).
