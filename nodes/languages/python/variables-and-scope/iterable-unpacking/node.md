---
tag: 9AZ6Q5
kind: feature
owner: python
slug: iterable-unpacking
name: Iterable unpacking
aliases: [sequence unpacking, tuple unpacking, starred assignment, extended iterable unpacking, PEP 3132, destructuring in Python]
area: languages/python/variables-and-scope
level: intro
summary: >-
  An assignment with several targets takes the items of an iterable one by one. One starred target, such as *rest, gets a list of the items that are left. In a display, *x inserts the items of x.
grammar: "target: ... | '(' [target_list] ')' | '[' [target_list] ']' | '*' target (Language Reference, 7.2); starred_expression: '*' or_expr | expression (Language Reference, 6.15)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#assignment-statements", locator: "7.2. Assignment statements"}
see_also: [python/assignment-statements, python/augmented-assignment, python/a-word-about-names-and-objects]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#assignment-statements
    title: The Python Language Reference, Python 3.15
    locator: 7.2. Assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, 5. Data Structures
    locator: 5.3. Tuples and Sequences
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3132/#specification
    title: PEP 3132 - Extended Iterable Unpacking
    locator: Abstract; Specification
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#iterable-unpacking
    title: The Python Language Reference, Python 3.15
    locator: 6.15.1. Iterable unpacking
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Assignment_statements
    title: The Go Programming Language Specification
    locator: Assignment statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#dict
    title: Built-in Types, Python 3.15 documentation
    locator: Mapping Types - dict, iter(d)
    accessed: 2026-11-16
---

## Synopsis

```python run id=unpack
t = 12345, 54321, "hello!"
x, y, z = t
first, *rest = [1, 2, 3, 4]
a, *b, c = range(5)
print(x, z, first, rest, a, b, c)
```

```output for=unpack
12345 hello! 1 [2, 3, 4] 0 [1, 2, 3] 4
```

`t` packs three values in a tuple. The next lines take the items out again.

## Semantics

**r1.** `x, y, z = t` "is called, appropriately enough, sequence unpacking and works for any sequence on the right-hand side. Sequence unpacking requires that there are as many variables on the left side of the equals sign as there are elements in the sequence."[^2] The reference allows any iterable: "The object must be an iterable with the same number of items as there are targets in the target list".[^1] Examples: [`unpack`](#unpack), [`too-many`](#too-many).

**r2.** "Note that multiple assignment is really just a combination of tuple packing and sequence unpacking."[^2] So `a, b = b, a` makes a tuple and then unpacks it. Example: [`nested`](#nested).

**r3.** A target list can contain one starred target. "The object must be an iterable with at least as many items as there are targets in the target list, minus one. The first items of the iterable are assigned, from left to right, to the targets before the starred target. The final items of the iterable are assigned to the targets after the starred target. A list of the remaining items in the iterable is then assigned to the starred target (the list can be empty)."[^1] Examples: [`unpack`](#unpack), [`empty-rest`](#empty-rest).

**r4.** PEP 3132 added the starred target. It is "a “catch-all” name which will be assigned a list of all items not assigned to a “regular” name". A lone starred target is an error, "This, however, is valid syntax: *a, = range(5)". The rule also applies "in implicit assignment context, such as in a for statement".[^3] Example: [`for-target`](#for-target).

**r5.** A target can be a parenthesized or bracketed target list, so unpacking can be nested: "Assignment of an object to a target list, optionally enclosed in parentheses or square brackets, is recursively defined".[^1] Example: [`nested`](#nested).

**r6.** In an expression, a star does the reverse. "In an expression list or tuple, list or set display, any expression may be prefixed with an asterisk (*). This denotes iterable unpacking." "The iterable is expanded into a sequence of items, which are included in the new tuple, list, or set, at the site of the unpacking."[^4] Example: [`star-in-display`](#star-in-display).

## Examples

```python run id=nested
point = ("p", (3, 4))
name, (px, py) = point
a, b = 1, 2
a, b = b, a
print(name, px, py, a, b)
```

```output for=nested
p 3 4 2 1
```

```python run id=empty-rest
head, *tail = "x"
*init, last = "abc"
*everything, = range(3)
print(head, tail, init, last, everything)
```

```output for=empty-rest
x [] ['a', 'b'] c [0, 1, 2]
```

The starred target is always a list, even when the right side is a string or a range (rule r3).

```python run id=for-target
for a, *b in [(1, 2, 3), (4, 5, 6, 7)]:
    print(a, b)
```

```output for=for-target
1 [2, 3]
4 [5, 6, 7]
```

```python run id=star-in-display
numbers = (1, 2)
print([*numbers, "word", *numbers])
print((*"ab", *range(2)), {*"aab"} == {"a", "b"})
```

```output for=star-in-display
[1, 2, 'word', 1, 2]
('a', 'b', 0, 1) True
```

## Pitfalls

The number of items must match the number of plain targets. Too many items or too few items raise `ValueError`:

```python fails id=too-many expect=exception:ValueError
x, y = [1, 2, 3]
```

Without a star, use a starred target to take the extra items, for example `x, *others = [1, 2, 3]`.

For a dictionary, `iter(d)` returns "an iterator over the keys of the dictionary".[^6] So unpacking a dictionary gives the keys and not the values:

```python wrong id=dict-keys
first, second = {"a": 1, "b": 2}
print(first, second)
```

```output for=dict-keys
a b
```

Unpack `d.values()` or `d.items()` to get the values or the pairs.

## Comparison

Go has a tuple assignment with two forms. In the first, "the right hand operand is a single multi-valued expression such as a function call, a channel or map operation, or a type assertion", and "The number of operands on the left hand side must match the number of values". In the second, "the number of operands on the left must equal the number of expressions on the right".[^5] So a Go assignment does not take the elements of a slice, and it has no starred target. To ignore a value, Go uses the blank identifier, as in `x, _ = f()`.[^5] A Python target list takes the items of any iterable, and a starred target takes any number of items (rules r1 and r3).
