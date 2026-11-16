---
tag: 39GRSD
kind: feature
owner: python
slug: generator-expressions
name: Generator expressions
aliases: [generator expression, genexpr, generator comprehension, lazy comprehension, sum of a generator, Generator expression must be parenthesized]
area: languages/python/functions
level: core
summary: >-
  A generator expression has the syntax of a list comprehension in parentheses. It gives a generator iterator that makes each value only when it is asked for, so it uses less memory than a list.
grammar: 'generator_expression: "(" comprehension ")"'
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#generator-expressions", locator: "6.2.7. Generator expressions"}
see_also: [python/generators, python/yield-expressions, python/lambda-expressions, python/functools]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#generator-expressions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.7. Generator expressions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#generator-expressions
    title: The Python Tutorial, 9. Classes
    locator: 9.10. Generator Expressions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0289/
    title: "PEP 289: Generator Expressions"
    locator: Abstract; Rationale; The Details
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=sum-squares
print(sum(x * x for x in range(10)))

words = ["alpha", "be", "gamma"]
print(max(len(w) for w in words))
print(list(w.upper() for w in words if len(w) > 2))
```

```output for=sum-squares
285
5
['ALPHA', 'GAMMA']
```

Each generator expression gives its values to the function one at a time. No list of all the values is made first.

## Semantics

**r1.** "The syntax for generator expressions is the same as for list comprehensions, except that they are enclosed in parentheses instead of brackets."[^1]

**r2.** "At runtime, a generator expression evaluates to a generator iterator which yields the same values as the corresponding list comprehension".[^1] Example: [`once`](#once).

**r3.** "The enclosing parentheses can be omitted in calls when the generator expression is the only positional argument and there are no keyword arguments."[^1] Examples: [`with-start`](#with-start), [`second-argument`](#second-argument).

**r4.** "The iterable expression in the leftmost for clause is evaluated immediately, so that an error raised by this expression will be emitted at the point where the generator expression is defined, rather than at the point where the first value is retrieved".[^1] "All other expressions are evaluated lazily, in the same fashion as normal generators (that is, when the iterator is asked to yield a value)".[^1] Example: [`late-condition`](#late-condition).

**r5.** "Generator expressions are more compact but less versatile than full generator definitions and tend to be more memory friendly than equivalent list comprehensions."[^2] PEP 289 says: "many of the use cases do not need to have a full list created in memory. Instead, they only need to iterate over the elements one at a time."[^3]

**r6.** "To avoid interfering with the expected operation of the generator expression itself, yield and yield from expressions are prohibited inside the implicitly nested scope."[^1]

## Examples

```python run id=with-start
print(sum((x ** 2 for x in range(10)), start=1000))

pairs = dict((k, len(k)) for k in ["a", "bb"])
print(pairs)
```

```output for=with-start
1285
{'a': 1, 'bb': 2}
```

The first call has a keyword argument, so the generator expression needs its own parentheses (rule r3).

```python run id=lazy-values
def noisy(n):
    print("make", n)
    return n

g = (noisy(n) for n in range(3))
print("created")
print(next(g))
print(next(g))
```

```output for=lazy-values
created
make 0
0
make 1
1
```

The expression `noisy(n)` runs only when `next()` asks for a value (rule r4).

```python run id=early-error
try:
    g = (x for x in None)
except TypeError as e:
    print("at definition:", e)
```

```output for=early-error
at definition: 'NoneType' object is not iterable
```

The leftmost iterable is checked when the expression is made, not at the first value (rule r4).

## Pitfalls

A generator expression gives its values only once:

```python wrong id=once
squares = (n * n for n in range(4))
print(list(squares))
print(sum(squares))
```

```output for=once
[0, 1, 4, 9]
0
```

Use a list comprehension when the program reads the values more than once (rule r2).

Only the leftmost iterable is fixed when the expression is made. A name in the condition is read later:

```python wrong id=late-condition
limit = 2
small = (n for n in range(5) if n < limit)
limit = 4
print(list(small))
```

```output for=late-condition
[0, 1, 2, 3]
```

The condition used the new value of `limit` (rule r4).

A generator expression that is not the only argument must have its own parentheses:

```python fails id=second-argument expect=exception:SyntaxError
total = sum(x for x in range(3), 10)
```

## Comparison

Go has no generator expression. A Go program writes a `for` loop with a range clause, or it ranges over a function: "For a function f, the iteration proceeds by calling f with a new, synthesized yield function as its argument."[^4] A Python generator expression is a single expression that a program can pass straight to `sum`, `max` or `list` (rule r3).
