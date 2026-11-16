---
tag: VXJVWQ
kind: feature
owner: python
slug: recursion-in-python
name: Recursion in Python
aliases: [recursive function, RecursionError, maximum recursion depth exceeded, recursion limit, sys.setrecursionlimit, memoized recursion, deep recursion]
area: languages/python/functions
level: core
summary: >-
  A function can call itself, and each call gets its own local variables. Python limits the depth of the interpreter stack and raises RecursionError when a chain of calls is too deep.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/sys.html#sys.setrecursionlimit", locator: "sys.getrecursionlimit(); sys.setrecursionlimit()"}
see_also: [python/defining-functions, python/call-semantics, python/functools, python/generators]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#defining-functions
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.8. Defining Functions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.setrecursionlimit
    title: sys, Python 3.15 documentation
    locator: sys.getrecursionlimit(); sys.setrecursionlimit()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#RecursionError
    title: Built-in Exceptions, Python 3.15 documentation
    locator: RecursionError
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/functools.html#functools.cache
    title: functools, Python 3.15 documentation
    locator: functools.cache
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#goroutines
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why goroutines instead of threads?
    accessed: 2026-11-16
---

## Synopsis

```python run id=tree-sum
def total(tree):
    if isinstance(tree, int):
        return tree
    return sum(total(branch) for branch in tree)

print(total([1, [2, 3], [[4], 5]]))
```

```output for=tree-sum
15
```

`total` calls itself for each branch. The base case, an `int`, ends the recursion.

## Semantics

**r1.** "When a function calls another function, or calls itself recursively, a new local symbol table is created for that call."[^1] Example: [`own-locals`](#own-locals).

**r2.** The recursion limit is "the maximum depth of the Python interpreter stack. This limit prevents infinite recursion from causing an overflow of the C stack and crashing Python."[^2]

**r3.** RecursionError "is derived from RuntimeError. It is raised when the interpreter detects that the maximum recursion depth (see sys.getrecursionlimit()) is exceeded."[^3] Example: [`no-base-case`](#no-base-case).

**r4.** "The highest possible limit is platform-dependent." A raise of the limit "should be done with care, because a too-high limit can lead to a crash."[^2]

**r5.** `functools.cache` is a "Simple lightweight unbounded function cache. Sometimes called “memoize”."[^4] The documentation shows it on a recursive factorial, where a second call makes "no new calls, just returns the cached result".[^4] Example: [`cached`](#cached).

## Examples

```python run id=own-locals
def countdown(n):
    if n == 0:
        print("liftoff")
        return
    print(n)
    countdown(n - 1)
    print("back in", n)

countdown(2)
```

```output for=own-locals
2
1
liftoff
back in 1
back in 2
```

Each call has its own `n`, so each call prints its own value after the inner call returns (rule r1).

```python run id=cached
from functools import cache

@cache
def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)

print(fib(80))
```

```output for=cached
23416728348467685
```

The cache keeps each `fib(k)` after the first call, so the other calls return the cached value (rule r5).

```python run id=iterative
def factorial(n):
    result = 1
    for k in range(2, n + 1):
        result *= k
    return result

print(len(str(factorial(1000))))
```

```output for=iterative
2568
```

A loop has no depth limit, so it is a good choice when the depth of the recursion can be large.

## Pitfalls

A recursion with no base case stops with RecursionError:

```python fails id=no-base-case expect=exception:RecursionError
def forever(n):
    return forever(n + 1)

forever(0)
```

A correct recursion can also be too deep:

```python fails id=too-deep expect=exception:RecursionError
def length(items):
    return 0 if not items else 1 + length(items[1:])

length(list(range(100000)))
```

A raise of the limit with `sys.setrecursionlimit()` can crash the process (rule r4). Write the function as a loop instead.

## Comparison

The Go FAQ says that "Go’s run-time uses resizable, bounded stacks." A new goroutine gets a small stack, and "When it isn’t, the run-time grows (and shrinks) the memory for storing the stack automatically".[^5] So a Go stack grows as a recursion gets deeper. A Python recursion stops at the limit of the interpreter stack (rule r2).
