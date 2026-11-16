---
tag: V5VE1M
kind: feature
owner: python
slug: default-argument-values
name: Default argument values
aliases: [default parameter value, optional argument, mutable default argument, default evaluated once, None as default, def f(x=1)]
area: languages/python/functions
level: intro
summary: >-
  A parameter written name=expression has a default value, so a call can omit its argument. Python evaluates the default once, when the def statement runs, and each call shares that same object.
grammar: "defparameter: parameter ['=' expression] (Language Reference, 8.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions", locator: "8.7. Function definitions"}
see_also: [python/defining-functions, python/keyword-arguments, python/keyword-only-arguments, python/call-semantics]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#default-argument-values
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.9.1. Default Argument Values
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#function-definitions
    title: The Python Language Reference, Python 3.15
    locator: 8.7. Function definitions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/programming.html#why-are-default-values-shared-between-objects
    title: Programming FAQ, Python 3.15
    locator: Why are default values shared between objects?
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_types
    title: The Go Programming Language Specification
    locator: Function types
    accessed: 2026-11-16
---

## Synopsis

```python run id=greet
def greet(name, greeting="Hello", mark="!"):
    return f"{greeting}, {name}{mark}"

print(greet("Ann"))
print(greet("Bob", "Hi"))
print(greet("Cy", mark="?"))
```

```output for=greet
Hello, Ann!
Hi, Bob!
Hello, Cy?
```

`greeting` and `mark` have default values. A call gives only the arguments that it needs.

## Semantics

**r1.** "For a parameter with a default value, the corresponding argument may be omitted from a call, in which case the parameter’s default value is substituted."[^2] Example: [`greet`](#greet).

**r2.** "If a parameter has a default value, all following parameters up until the “*” must also have a default value".[^2] Example: [`order`](#order).

**r3.** "Default parameter values are evaluated from left to right when the function definition is executed. This means that the expression is evaluated once, when the function is defined, and that the same “pre-computed” value is used for each call."[^2] Example: [`evaluated-at-def`](#evaluated-at-def).

**r4.** "The default values are evaluated at the point of function definition in the defining scope".[^1] Example: [`evaluated-at-def`](#evaluated-at-def).

**r5.** "The default value is evaluated only once. This makes a difference when the default is a mutable object such as a list, dictionary, or instances of most classes."[^1] Example: [`shared-list`](#shared-list).

**r6.** "A way around this is to use None as the default, and explicitly test for it in the body of the function".[^2] Example: [`none-default`](#none-default).

## Examples

```python run id=evaluated-at-def
i = 5

def f(arg=i):
    return arg

i = 6
print(f())
```

```output for=evaluated-at-def
5
```

This is the example of the tutorial. The default is the value of `i` when `def` runs (rules r3 and r4).

```python run id=none-default
def append_to(item, items=None):
    if items is None:
        items = []
    items.append(item)
    return items

print(append_to(1))
print(append_to(2))
```

```output for=none-default
[1]
[2]
```

Each call makes a new list (rule r6).

```python run id=cache
def fib(n, *, _memo={0: 0, 1: 1}):
    if n not in _memo:
        _memo[n] = fib(n - 1) + fib(n - 2)
    return _memo[n]

print(fib(30))
```

```output for=cache
832040
```

The FAQ says that this shared object can be useful: "This feature can be useful. When you have a function that’s time-consuming to compute, a common technique is to cache the parameters and the resulting value of each call to the function".[^3]

## Pitfalls

A mutable default keeps its changes from one call to the next:

```python wrong id=shared-list
def append_to(item, items=[]):
    items.append(item)
    return items

print(append_to(1))
print(append_to(2))
```

```output for=shared-list
[1]
[1, 2]
```

The FAQ says: "Default values are created exactly once, when the function is defined."[^3] So each call that omits `items` uses the same list. Use None as the default (rule r6).

A parameter with no default cannot follow one with a default:

```python fails id=order expect=exception:SyntaxError
def f(a=1, b):
    return a + b
```

A default such as `when=time.time()` is also evaluated once, so each call gets the same time (rule r3). Use None and compute the value in the body (rule r6).

## Comparison

Go has no default parameter values. In Go, "A function type denotes the set of all functions with the same parameter and result types."[^4] So each call gives a value for each parameter. The nearest Go form is a final variadic parameter: "A function with such a parameter is called variadic and may be invoked with zero or more arguments for that parameter."[^4] In Python, a default is part of the def statement, and Python evaluates it once (rule r3).
