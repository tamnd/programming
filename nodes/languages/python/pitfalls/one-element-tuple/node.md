---
tag: WXKXSC
kind: feature
owner: python
slug: one-element-tuple
name: One-element tuples and stray commas
aliases: [singleton tuple, tuple with one item, trailing comma, the comma makes the tuple, "(1,)", COM818, R1707, trailing-comma-tuple]
area: languages/python/pitfalls
level: intro
summary: >-
  The comma makes a tuple, not the parentheses. So (1) is the integer 1, and (1,) is a tuple. A stray comma at the end of an assignment makes a tuple that you did not want.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/stdtypes.html#tuples", locator: "Tuples"}
proposals: []
see_also: [python/tuples-and-sequences, python/expression-lists, python/iterable-unpacking, python/sequence-types, python/atoms-and-primaries, python/comparing-sequences-and-other-types]
sources:
  - url: https://docs.python.org/3.15/library/stdtypes.html#tuples
    title: Built-in Types, Python 3.15 documentation
    locator: Tuples
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
    title: The Python Tutorial, Python 3.15 documentation
    locator: 5.3 Tuples and Sequences
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/expressions.html#expression-lists
    title: The Python Language Reference, Python 3.15 documentation
    locator: 6.16 Expression lists; 6.2.3 Parenthesized forms
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/trailing-comma-on-bare-tuple/
    title: trailing-comma-on-bare-tuple (COM818), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/refactor/trailing-comma-tuple.html
    title: trailing-comma-tuple / R1707, Pylint documentation
    locator: Description
    accessed: 2026-11-18
---

## Synopsis

```python run id=comma-makes-it
a = (1)
b = (1,)
c = 1,
print(type(a).__name__, type(b).__name__, type(c).__name__)
print(b == c)
```

```output for=comma-makes-it
int tuple tuple
True
```

The parentheses around `1` only group. The comma makes the tuple, with or without parentheses.

## Semantics

**r1.** "Note that it is actually the comma which makes a tuple, not the parentheses. The parentheses are optional, except in the empty tuple case, or when they are needed to avoid syntactic ambiguity."[^1] Example: [`comma-makes-it`](#comma-makes-it).

**r2.** The tutorial names the case: "A special problem is the construction of tuples containing 0 or 1 items: the syntax has some extra quirks to accommodate these. Empty tuples are constructed by an empty pair of parentheses; a tuple with one item is constructed by following a value with a comma (it is not sufficient to enclose a single value in parentheses). Ugly, but effective."[^2]

**r3.** The reference gives the rule for expression lists: "A trailing comma is required only to create a one-item tuple, such as 1,; it is optional in all other cases. A single expression without a trailing comma doesn’t create a tuple, but rather yields the value of that expression."[^3]

**r4.** So a stray comma at the end of a line makes a tuple. The line `total = price * count,` gives a tuple with one item, and a later calculation fails or gives a wrong result. Example: [`stray-comma`](#stray-comma).

**r5.** The same rule hits a loop over one item. `for name in ("ann"):` loops over the letters of a string, because `("ann")` is a string. Write `("ann",)` or a list `["ann"]`. Example: [`loop-over-string`](#loop-over-string).

**r6.** The linters:

- Ruff COM818 "Checks for the presence of trailing commas on bare (i.e., unparenthesized) tuples."[^4] Its reason: "The presence of a misplaced comma will cause Python to interpret the value as a tuple, which can lead to unexpected behaviour."[^4]
- Pylint R1707 `trailing-comma-tuple` explains: "In Python, a tuple is actually created by the comma symbol, not by the parentheses."[^5]

## Examples

```python run id=clear-forms
empty = ()
one = ("ann",)
also_one = tuple(["ann"])
print(len(empty), len(one), one == also_one)

(first,) = one
print(first)
```

```output for=clear-forms
0 1 True
ann
```

The empty tuple needs the parentheses (rule r1). For one item, `(x,)` with the parentheses is the clearest form. The same comma works in a target list to unpack one item.

## Versions

The rules are the same in all Python versions. They follow from rule r1.

## Pitfalls

```python fails id=stray-comma expect=exception:TypeError
price = 4
count = 3
total = price * count,
print(total)

with_tax = total * 1.1
```

The comma at the end of the third line made `total` the tuple `(12,)` (rule r4). A tuple times a float raises `TypeError`. Times an integer, it repeats the tuple and gives no error.

```python wrong id=loop-over-string
for name in ("ann"):
    print(name)

for name in ("ann",):
    print(name)
```

```output for=loop-over-string
a
n
n
ann
```

The first loop goes over the three letters of the string `"ann"` (rule r5). The second loop goes over a tuple with one string.

## Comparison

In many languages, parentheses make or mark a group of values. In Python, a comma does, and the parentheses only group (rule r1). So `(x)` and `x` are the same, and `x,` is a tuple. For tuples in general, see [[python/tuples-and-sequences]]. For the grammar, see [[python/expression-lists]].
