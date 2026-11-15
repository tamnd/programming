---
tag: F7535S
kind: feature
owner: python
slug: expression-lists
name: Expression lists
aliases: [Python comma operator, tuple packing, trailing comma, comma-separated expressions, iterable unpacking in expressions]
area: languages/python/syntax
level: core
summary: >-
  An expression list is expressions separated by commas. Outside a list or set display it yields a tuple: the comma makes the tuple, not the parentheses, and a starred item unpacks an iterable into it.
grammar: "expression_list: expression (',' expression)* [',']; starred_expression: '*' or_expr | expression (Language Reference, 6.15)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#expression-lists", locator: "6.15 Expression lists"}
see_also: [python/evaluation-order-in-python, python/operators-and-delimiters, python/atoms-and-primaries]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#expression-lists
    title: The Python Language Reference, Python 3.15
    locator: 6.15 Expression lists
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#tuple
    title: Built-in Types, Python 3.15 documentation
    locator: class tuple
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#iterable-unpacking
    title: The Python Language Reference, Python 3.15
    locator: 6.15.1 Iterable unpacking
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, Python 3.15
    locator: 5.3 Tuples and Sequences
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Assignment_statements
    title: The Go Programming Language Specification
    locator: Assignment statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=tuple-by-comma
point = 3, 4
single = 5,
plain = (5)
empty = ()
print(point, single, plain, empty)
print(type(single).__name__, type(plain).__name__)
```

```output for=tuple-by-comma
(3, 4) (5,) 5 ()
tuple int
```

## Semantics

**r1.** "Except when part of a list or set display, an expression list containing at least one comma yields a tuple." The tuple has one item per expression, and "The expressions are evaluated from left to right."[^1] Example: [`left-to-right-list`](#left-to-right-list).

**r2.** "A trailing comma is required only to create a one-item tuple", and it is optional elsewhere. A single expression without a trailing comma is just that expression's value, and the empty tuple is written `()`.[^1] Example: [`tuple-by-comma`](#tuple-by-comma).

**r3.** "Note that it is actually the comma which makes a tuple, not the parentheses." The parentheses are optional except for the empty tuple or to avoid ambiguity.[^2] Example: [`paren-not-tuple`](#paren-not-tuple).

**r4.** An item of an expression list, or of a tuple, list or set display, may be prefixed with `*`. The starred expression must evaluate to an iterable, whose items are spliced into the new tuple, list or set at that point.[^3] Example: [`unpack-items`](#unpack-items).

**r5.** Writing several values separated by commas is called tuple packing, and the reverse, `x, y, z = t`, is sequence unpacking.[^4] So `return a, b` returns one tuple, which the caller can unpack. Example: [`return-tuple`](#return-tuple).

## Examples

The items are evaluated in order (rule r1).

```python run id=left-to-right-list
order = []

def mark(n):
    order.append(n)
    return n

values = mark(1), mark(2), mark(3)
print(values, order)
```

```output for=left-to-right-list
(1, 2, 3) [1, 2, 3]
```

Starred items in expression lists and displays (rule r4).

```python run id=unpack-items
head = [1, 2]
tail = (3, 4)
combined = *head, *tail, 5
print(combined)
print([*head, *tail], {*head, *head})
```

```output for=unpack-items
(1, 2, 3, 4, 5)
[1, 2, 3, 4] {1, 2}
```

Packing on `return`, unpacking at the call site (rule r5).

```python run id=return-tuple
def min_max(values):
    return min(values), max(values)

result = min_max([4, 1, 9])
low, high = result
print(result, low, high)
```

```output for=return-tuple
(1, 9) 1 9
```

## Pitfalls

### A stray comma makes a tuple

A trailing comma at the end of an assignment turns the value into a one-item tuple (rule r2).

```python wrong id=stray-comma
total = 10,
print(total, total == 10)
```

```output for=stray-comma
(10,) False
```

### Parentheses alone do not make a tuple

`("solo")` is a string, so the loop walks its characters (rule r3). Write `("solo",)`.

```python wrong id=paren-not-tuple
for item in ("solo"):
    print(item)
```

```output for=paren-not-tuple
s
o
l
o
```

### A lone starred expression

A starred item needs a list to be spliced into; on its own it is a syntax error (rule r4). Add a comma: `x = *items,`.

```python fails id=lone-star expect=exception:SyntaxError
items = [1, 2]
x = *items
```

## Comparison

Go has no tuple type. Its assignment statement can assign "multiple values to a matching number of variables", and a tuple assignment takes the values of a multi-valued operation such as a function call.[^5] A Python function that returns `a, b` returns one tuple object (rule r5), which can be stored, passed around or unpacked later.
