---
tag: VBTZ7S
kind: feature
owner: python
slug: boolean-operators
name: Boolean operators
aliases: [Python and or not, logical operators, short-circuit evaluation, truthiness, truth value testing]
area: languages/python/syntax
level: intro
summary: >-
  Python's Boolean operators are the keywords and, or and not. and and or short-circuit and return one of their operands, not always a bool; not always returns True or False.
grammar: "or_test: and_test | or_test 'or' and_test; and_test: not_test | and_test 'and' not_test; not_test: comparison | 'not' not_test (Language Reference, 6.11)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#boolean-operations", locator: "6.11 Boolean operations"}
see_also: [python/comparison-operators, python/keywords, python/operators-and-delimiters]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#boolean-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.11 Boolean operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#truth-value-testing
    title: Built-in Types, Python 3.15 documentation
    locator: Truth Value Testing
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#more-on-conditions
    title: The Python Tutorial, Python 3.15
    locator: 5.7 More on Conditions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#boolean-operations-and-or-not
    title: Built-in Types, Python 3.15 documentation
    locator: Boolean Operations, and, or, not
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: "PEP 8: Style Guide for Python Code"
    locator: Programming Recommendations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Logical_operators
    title: The Go Programming Language Specification
    locator: Logical operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=bool-ops
age, member = 25, False
print(age >= 18 and not member, age < 13 or member, not age)
print(0 or "default", "x" and "y", [] and 1 / 0)
```

```output for=bool-ops
True False False
default y []
```

## Semantics

**r1.** In a Boolean operation or a control flow condition, `False`, `None`, numeric zero of all types, and empty strings and containers are false, and all other values are true. A class can set the truth value of its objects with `__bool__()`.[^1] Without `__bool__()`, a `__len__()` that returns zero also makes an object false.[^2] Example: [`truthiness`](#truthiness).

**r2.** `x and y` first evaluates `x`; if `x` is false, its value is returned; otherwise `y` is evaluated and its value is returned. `x or y` returns `x` if it is true, and otherwise evaluates and returns `y`.[^1] Example: [`short-circuit`](#short-circuit).

**r3.** "Note that neither" `and` nor `or` restricts the result to `False` and `True`: each returns the last evaluated argument. "Because" `not` "has to create a new value, it returns a boolean value regardless of the type of its argument".[^1] Example: [`bool-ops`](#bool-ops).

**r4.** The arguments are evaluated from left to right, "and evaluation stops as soon as the outcome is determined". Among the three, `not` has the highest priority and `or` the lowest, so `A and not B or C` is `(A and (not B)) or C`. All three bind looser than the comparisons.[^3] Example: [`bool-precedence`](#bool-precedence).

**r5.** Because `not` has a lower priority than the non-Boolean operators, `not a == b` is read as `not (a == b)`, and `a == not b` is a syntax error.[^4] Example: [`bool-precedence`](#bool-precedence).

## Examples

Short-circuit evaluation (rules r2 and r4): the calls stop as soon as the result is known.

```python run id=short-circuit
def check(name, result):
    print("check", name)
    return result

print(check("a", True) and check("b", False) and check("c", True))
print(check("a", False) or check("b", 0) or check("c", "found"))
```

```output for=short-circuit
check a
check b
False
check a
check b
check c
found
```

The false values of rule r1. A non-empty string or list is true even when it holds a zero.

```python run id=truthiness
values = [None, False, 0, 0.0, 0j, "", (), [], {}, set(), range(0), "0", [0], " "]
print([bool(v) for v in values])
```

```output for=truthiness
[False, False, False, False, False, False, False, False, False, False, False, True, True, True]
```

A class with `__len__()` gets its truth value from its length (rule r1).

```python run id=custom-truth
class Box:
    def __init__(self, items):
        self.items = items

    def __len__(self):
        return len(self.items)

print(bool(Box([])), bool(Box([1])), Box([]) or "empty")
```

```output for=custom-truth
False True empty
```

Priorities (rules r4 and r5).

```python run id=bool-precedence
A, B, C = False, False, True
print(A and not B or C, (A and (not B)) or C, A and (not B or C))
print(not 1 == 2, not (1 == 2))
```

```output for=bool-precedence
True True False
True True
```

## Pitfalls

### `x == a or b` is always true

`or` binds looser than `==` (rule r4), so `color == "red" or "green"` is `(color == "red") or "green"`, and a non-empty string is true.

```python wrong id=or-always-true
color = "blue"
if color == "red" or "green":
    print("warm")
print(color == "red" or "green")
print(color in ("red", "green"))
```

```output for=or-always-true
warm
green
False
```

Use `in` with a tuple, or repeat the comparison.

### `or` replaces every false value

`value or default` also replaces `0`, an empty string and `[]`, not only `None` (rules r1 and r2). PEP 8 warns: "beware of writing if x when you really mean if x is not None".[^5]

```python wrong id=or-default-zero
def scale(value, factor=None):
    factor = factor or 10
    return value * factor

print(scale(3), scale(3, 2), scale(3, 0))
```

```output for=or-default-zero
30 6 30
```

Write `factor = 10 if factor is None else factor` to keep a `0`.

### `&&` and `||` are not Python

The Boolean operators are keywords. `&&` is a syntax error, and `&` and `|` are bitwise operators that do not short-circuit.

```python fails id=double-amp expect=exception:SyntaxError
ready, valid = True, False
print(ready && valid)
```

## Comparison

Go's `&&`, `||` and `!` "apply to boolean values and yield a result of the same type as the operands", and the right operand is evaluated only if needed.[^6] Python's `and` and `or` short-circuit the same way, but accept operands of any type and return one of them (rule r3), so `"" or "guest"` is `"guest"`; in Go that expression does not compile.
