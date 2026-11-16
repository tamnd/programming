---
tag: R6CDME
kind: feature
owner: python
slug: functools
name: The functools module
aliases: [functools, functools.partial, functools.reduce, functools.wraps, lru_cache, functools.cache, higher-order functions, partial application]
area: languages/python/functions
level: core
summary: >-
  functools has tools that take or return functions: partial fixes some arguments, cache and lru_cache save results, reduce folds an iterable, and wraps keeps the name of a wrapped function.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/functools.html", locator: "functools: Higher-order functions and operations on callable objects"}
see_also: [python/function-objects, python/callable-objects, python/recursion-in-python, python/function-overloading-alternatives, python/lambda-expressions]
sources:
  - url: https://docs.python.org/3.15/library/functools.html
    title: functools, Python 3.15 documentation
    locator: functools.partial; functools.cache; functools.lru_cache; functools.reduce; functools.wraps
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/howto/functional.html#small-functions-and-the-lambda-expression
    title: Functional Programming HOWTO, Python 3.15
    locator: Small functions and the lambda expression
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Function_literals
    title: The Go Programming Language Specification
    locator: Function literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=partial
from functools import partial, reduce

basetwo = partial(int, base=2)
print(basetwo("10010"))

print(reduce(lambda x, y: x + y, [1, 2, 3, 4, 5]))
```

```output for=partial
18
15
```

`basetwo` is `int` with the argument `base=2` fixed. `reduce` adds the items from left to right.

## Semantics

**r1.** "The functools module is for higher-order functions: functions that act on or return other functions. In general, any callable object can be treated as a function for the purposes of this module."[^1]

**r2.** partial returns "a new partial object which when called will behave like func called with the positional arguments args and keyword arguments keywords. If more arguments are supplied to the call, they are appended to args. If additional keyword arguments are supplied, they extend and override keywords."[^1] Example: [`partial-args`](#partial-args).

**r3.** lru_cache is a "Decorator to wrap a function with a memoizing callable that saves up to the maxsize most recent calls."[^1] "Since a dictionary is used to cache results, the positional and keyword arguments to the function must be hashable."[^1] Examples: [`cache-info`](#cache-info), [`unhashable`](#unhashable).

**r4.** cache is a "Simple lightweight unbounded function cache." It "Returns the same as lru_cache(maxsize=None)".[^1]

**r5.** reduce does this: "Apply function of two arguments cumulatively to the items of iterable, from left to right, so as to reduce the iterable to a single value."[^1] "If the optional initial is present, it is placed before the items of the iterable in the calculation, and serves as a default when the iterable is empty."[^1] Example: [`reduce-initial`](#reduce-initial).

**r6.** wraps is "a convenience function for invoking update_wrapper() as a function decorator when defining a wrapper function."[^1] "Without the use of this decorator factory, the name of the example function would have been 'wrapper', and the docstring of the original example() would have been lost."[^1] Example: [`wraps`](#wraps).

## Examples

```python run id=partial-args
from functools import partial

def power(base, exp):
    return base ** exp

square = partial(power, exp=2)
cube = partial(power, exp=3)
print(square(5), cube(2), square(5, exp=4))
print(square.func.__name__, square.keywords)
```

```output for=partial-args
25 8 625
power {'exp': 2}
```

The call `square(5, exp=4)` overrides the fixed keyword (rule r2).

```python run id=cache-info
from functools import lru_cache

@lru_cache(maxsize=2)
def slow_square(n):
    print("compute", n)
    return n * n

slow_square(3)
slow_square(3)
slow_square(4)
print(slow_square.cache_info())
```

```output for=cache-info
compute 3
compute 4
CacheInfo(hits=1, misses=2, maxsize=2, currsize=2)
```

The second call with 3 uses the saved result (rule r3).

```python run id=reduce-initial
from functools import reduce

print(reduce(lambda acc, n: acc * n, [], 1))
print(reduce(lambda acc, word: acc + len(word), ["ab", "cde"], 0))
```

```output for=reduce-initial
1
5
```

```python run id=wraps
from functools import wraps

def logged(f):
    @wraps(f)
    def wrapper(*args, **kwargs):
        print("calling", f.__name__)
        return f(*args, **kwargs)
    return wrapper

@logged
def example():
    """Docstring"""
    return 42

print(example(), example.__name__, example.__doc__)
```

```output for=wraps
calling example
42 example Docstring
```

## Pitfalls

lru_cache keeps arguments in a dict, so a list argument is a TypeError:

```python fails id=unhashable expect=exception:TypeError
from functools import cache

@cache
def total(items):
    return sum(items)

total([1, 2, 3])
```

Pass a tuple instead of a list (rule r3).

reduce with an empty iterable and no initial value has nothing to return:

```python fails id=reduce-empty expect=exception:TypeError
from functools import reduce

reduce(lambda x, y: x + y, [])
```

The Functional Programming HOWTO says: "Many uses of functools.reduce() are clearer when written as for loops."[^2]

## Comparison

In Go, a program can write a function literal for a partial call, as in `square := func(b int) int { return power(b, 2) }`, because "Function literals are closures: they may refer to variables declared in a surrounding function."[^3] A Python partial object does the same with no new function definition, and it shows the fixed arguments in its attributes (rule r2).
