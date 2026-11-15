---
tag: 1DJRWV
kind: feature
owner: python
slug: comparison-operators
name: Comparison operators
aliases: [Python comparison operators, chained comparisons, relational operators, in operator, is operator, not in, is not]
area: languages/python/syntax
level: intro
summary: >-
  Python has eight comparison operators: <, >, ==, >=, <=, !=, is and in, plus is not and not in. They share one priority, and a chain like a < b < c means a < b and b < c.
grammar: "comparison: or_expr (comp_operator or_expr)*; comp_operator: '<' | '>' | '==' | '>=' | '<=' | '!=' | 'is' ['not'] | ['not'] 'in' (Language Reference, 6.10)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#comparisons", locator: "6.10 Comparisons"}
see_also: [python/arithmetic-operators, python/bitwise-and-shift-operators, python/operators-and-delimiters]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10 Comparisons
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10.1 Value comparisons
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#membership-test-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.10.2 Membership test operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#is-not
    title: The Python Language Reference, Python 3.15
    locator: 6.10.3 Identity comparisons
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0008/#programming-recommendations
    title: "PEP 8: Style Guide for Python Code"
    locator: Programming Recommendations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#boolean-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.11 Boolean operations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=compare-basics
x = 5
print(x == 5, x != 5, x < 10, x >= 10)
print(1 < x < 10, 3 in [1, 2, 3], "py" in "python", None is None)
```

```output for=compare-basics
True False True False
True True True True
```

## Semantics

**r1.** All comparison operators have the same priority, "which is lower than that of any arithmetic, shifting or bitwise operation". Comparisons yield `True` or `False`, though a custom rich comparison method may return another value.[^1] Example: [`compare-basics`](#compare-basics).

**r2.** Comparisons can be chained: `a op1 b op2 c` is equivalent to `a op1 b and b op2 c`, "except that each expression is evaluated at most once". A false link stops the chain, so the later operands are not evaluated. A chain does not compare `a` with `c`.[^1] Example: [`chain-once`](#chain-once).

**r3.** `<`, `>`, `==`, `>=`, `<=` and `!=` compare values, and the objects need not have the same type. By default `==` compares identity, and the order operators raise `TypeError`; a class changes this with methods like `__lt__()`.[^2] Example: [`mixed-order`](#mixed-order).

**r4.** Numbers of the built-in numeric types compare mathematically within and across types, but complex numbers have no order. A NaN is not equal to itself: with `x = float('NaN')`, `x == x` is false and `x != x` is true.[^2] Example: [`nan`](#nan).

**r5.** Strings compare by the Unicode code points of their characters. Lists, tuples and ranges compare only within their own type, element by element: the first unequal elements decide, and a shorter collection that matches a longer one is ordered first. Sets use the order operators for subset and superset tests.[^2] Example: [`sequence-order`](#sequence-order).

**r6.** `x in s` is true if `x` is a member of `s`; for a dict it tests the keys, and for strings and bytes it tests for a substring. `x not in s` is the negation.[^3] Example: [`membership`](#membership).

**r7.** `x is y` is true if and only if `x` and `y` are the same object, and `is not` gives the inverse.[^4] PEP 8 says comparisons to singletons like `None` "should always be done with is or is not, never the equality operators", and prefers `is not` to `not ... is`.[^5] Example: [`is-vs-eq`](#is-vs-eq).

**r8.** `not` sits below the comparisons in the grammar (`not_test: comparison | "not" not_test`), so `not a in b` means `not (a in b)`, and a comparison operand cannot start with `not`.[^6] Example: [`membership`](#membership).

## Examples

Each operand of a chain is evaluated once, and the chain stops at the first false link (rule r2).

```python run id=chain-once
def value(n):
    print("eval", n)
    return n

print(value(1) < value(5) < value(10))
print(value(10) < value(5) < value(1))
```

```output for=chain-once
eval 1
eval 5
eval 10
True
eval 10
eval 5
False
```

Numbers compare across types, and NaN is unequal to itself (rule r4).

```python run id=nan
x = float("nan")
print(x == x, x != x, x < 3, 3 < x)
print(1 == 1.0, 2 > 1.5, 1 == 1 + 0j)
```

```output for=nan
False True False False
True True True
```

Sequences, strings and sets (rule r5). Uppercase letters have smaller code points than lowercase ones.

```python run id=sequence-order
print([1, 2, 3] < [1, 2, 4], [1, 2] < [1, 2, 3], [1, 2] == (1, 2))
print("apple" < "banana", "Zebra" < "apple")
print({1, 2} <= {1, 2, 3}, {1, 2} < {2, 3}, {2, 3} < {1, 2})
```

```output for=sequence-order
True True False
True True
True False False
```

Membership in dicts and strings (rules r6 and r8). The empty string is a substring of every string.

```python run id=membership
ages = {"ana": 31, "bo": 27}
print("ana" in ages, 31 in ages, 31 in ages.values())
print("" in "abc", "ab" not in "abc", not "ab" in "abc")
```

```output for=membership
True False True
True False False
```

## Pitfalls

### `is` is not `==`

Two equal lists are still two objects (rule r7). Use `is` for `None` and other singletons, and `==` for values.

```python wrong id=is-vs-eq
a = [1, 2]
b = [1, 2]
print(a == b, a is b, a is not b)
```

```output for=is-vs-eq
True False True
```

### A chain with `== True`

`x > 3 == True` is a chain (rule r2): it means `x > 3 and 3 == True`, which is false.

```python wrong id=chain-surprise
x = 5
print(x > 3 == True)
print((x > 3) == True, x > 3)
```

```output for=chain-surprise
False
True True
```

Drop the `== True`; the comparison is already a bool.

### `== not` is a syntax error

An operand of a comparison cannot start with `not` (rule r8). Write `ready != True`, or `ready == (not True)`.

```python fails id=eq-not expect=exception:SyntaxError
ready = False
print(ready == not True)
```

### Ordering mixed types

Order comparisons between unrelated types raise `TypeError` (rule r3), so a list of mixed numbers and strings cannot be sorted.

```python fails id=mixed-order expect=exception:TypeError
print(sorted([3, "2", 1]))
```

## Comparison

The Language Reference contrasts Python with C twice: unlike C, all comparisons have the same priority, and `a < b < c` has its conventional mathematical meaning.[^1] In Go, the operands of a comparison must be assignable to each other's type, and the ordering operators apply only to ordered types, while booleans are only comparable.[^7] So Go rejects `1 < x < 3`, because `1 < x` is a bool; Python reads it as a chain (rule r2).
