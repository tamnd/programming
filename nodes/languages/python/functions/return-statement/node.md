---
tag: AQ7989
kind: feature
owner: python
slug: return-statement
name: The return statement
aliases: [return, return value, return None, return several values, return a tuple, early return, return in finally]
area: languages/python/functions
level: intro
summary: >-
  return leaves the current function call and gives the value of its expression list to the caller. A bare return, or the end of the body, gives None. Several values separated by commas give a tuple.
grammar: "return_stmt: 'return' [expression_list] (Language Reference, 7.6)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-return-statement", locator: "7.6. The return statement"}
see_also: [python/defining-functions, python/expression-lists, python/generators, python/yield-expressions]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-return-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.6. The return statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#defining-functions
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.8. Defining Functions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#expression-lists
    title: The Python Language Reference, Python 3.15
    locator: 6.15. Expression lists
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Return_statements
    title: The Go Programming Language Specification
    locator: Return statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=min-max
def min_max(values):
    if not values:
        return None
    return min(values), max(values)

low, high = min_max([4, 1, 9])
print(low, high)
print(min_max([]))
```

```output for=min-max
1 9
None
```

The first `return` ends the call early. The second `return` gives a tuple of two values, and the caller unpacks it.

## Semantics

**r1.** "return may only occur syntactically nested in a function definition, not within a nested class definition."[^1] Example: [`outside-function`](#outside-function).

**r2.** "If an expression list is present, it is evaluated, else None is substituted."[^1] "return leaves the current function call with the expression list (or None) as return value."[^1]

**r3.** "return without an expression argument returns None. Falling off the end of a function also returns None."[^2] Example: [`none`](#none).

**r4.** "Except when part of a list or set display, an expression list containing at least one comma yields a tuple."[^3] So `return a, b` gives one tuple. Example: [`min-max`](#min-max).

**r5.** "When return passes control out of a try statement with a finally clause, that finally clause is executed before really leaving the function."[^1] Example: [`finally`](#finally).

**r6.** "In a generator function, the return statement indicates that the generator is done and will cause StopIteration to be raised. The returned value (if any) is used as an argument to construct StopIteration and becomes the StopIteration.value attribute."[^1] Example: [`generator-return`](#generator-return).

## Examples

```python run id=none
def nothing():
    pass

def bare():
    return

print(nothing(), bare())
```

```output for=none
None None
```

```python run id=finally
def read():
    try:
        return "data"
    finally:
        print("cleanup")

print(read())
```

```output for=finally
cleanup
data
```

The finally clause runs before the call ends, so `cleanup` comes first (rule r5).

```python run id=generator-return
def count():
    yield 1
    return "done"

g = count()
print(next(g))
try:
    next(g)
except StopIteration as stop:
    print("stopped with", stop.value)
```

```output for=generator-return
1
stopped with done
```

## Pitfalls

`return` outside a function is a syntax error (rule r1):

```python fails id=outside-function expect=exception:SyntaxError
for n in range(3):
    if n == 1:
        return n
```

A return statement in a loop ends the whole function, not only the loop:

```python wrong id=return-in-loop
def all_positive(values):
    for v in values:
        if v > 0:
            return True
        return False

print(all_positive([1, -2, 3]))
```

```output for=return-in-loop
True
```

The function returns after the first item. Put `return False` in the loop only for the failed test, and put `return True` after the loop.

## Comparison

In Go, a return statement "in a function F terminates the execution of F, and optionally provides one or more result values."[^4] A Go function can declare several results, and the return statement gives one value for each result. Python returns one object, and `return a, b` makes it a tuple (rule r4). Go also has named results: "The expression list may be empty if the function's result type specifies names for its result parameters."[^4] Python has no named results. A bare Python return gives None (rule r2).
