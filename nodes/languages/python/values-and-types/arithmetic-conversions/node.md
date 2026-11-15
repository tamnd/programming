---
tag: JDR75M
kind: feature
owner: python
slug: arithmetic-conversions
name: Arithmetic conversions
aliases: [mixed arithmetic in Python, numeric widening, common real type, int plus float, mixed-type comparison]
area: languages/python/values-and-types
level: core
summary: >-
  When an arithmetic operator gets two numbers of different built-in types, Python widens the narrower one: int to float, and int or float to complex. Comparisons use the exact values instead.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#arithmetic-conversions", locator: "6.1. Arithmetic conversions"}
see_also: [python/arithmetic-operators, python/the-numeric-tower, python/int-type, python/float-type, type-conversion]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#arithmetic-conversions
    title: The Python Language Reference, Python 3.15
    locator: 6.1. Arithmetic conversions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types - int, float, complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.7. Binary arithmetic operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10.1. Value comparisons
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/numbers.html#implementing-the-arithmetic-operations
    title: numbers, Numeric abstract base classes, Python 3.15 documentation
    locator: Implementing the arithmetic operations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/decimal.html#decimal-objects
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: Decimal objects
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Operators
    title: The Go Programming Language Specification
    locator: Operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=mixed-results
print(type(1 + 2.0).__name__, type(1 + 2j).__name__, type(1.5 + 2j).__name__)
print(type(True + 1).__name__, 10 / 2, type(10 / 2).__name__)
print(1 + 2.0, 2 * (1 + 1j), 1 == 1.0 == 1 + 0j)
```

```output for=mixed-results
float complex complex
int 5.0 float
3.0 (2+2j) True
```

An `int` mixed with a `float` gives a `float`. A `complex` operand gives a `complex` result. `/` gives a `float` even for two integers.

## Semantics

**r1.** When the Language Reference says that "the numeric arguments are converted to a common real type", the built-in numeric types follow the rules in the Numeric Types section of Built-in Types. "Extensions must define their own conversion behavior."[^1]

**r2.** When a binary arithmetic operator gets operands of different built-in numeric types, "the operand with the "narrower" type is widened to that of the other". If both are complex numbers, no conversion happens. If one is a complex number or a float, the other is converted to a float. If neither is, both are integers and no conversion is needed.[^2] Example: [`mixed-results`](#mixed-results).

**r3.** A complex operand and a real operand combine with the usual mathematical formula, for example `x + complex(u, v) = complex(x + u, v)`.[^2] Python 3.14 made this the rule for the operators: "If only one operand is a complex number, the other operand is converted to a floating-point number."[^3]

**r4.** `/` on two integers gives a float, and `//` on two integers gives an integer.[^3] Example: [`mixed-results`](#mixed-results).

**r5.** "A comparison between numbers of different types behaves as though the exact values of those numbers were being compared."[^2] Numbers of the built-in types, `fractions.Fraction` and `decimal.Decimal` compare "mathematically (algorithmically) correct without loss of precision", but complex numbers have no order.[^4] Example: [`exact-compare`](#exact-compare).

**r6.** Other number types define their own conversions with `__add__()`, `__radd__()` and the other operator methods. `fractions.Fraction` combines with an `int` exactly, and converts itself to a `float` or a `complex` for an operand of that type.[^5] `Decimal` objects "cannot generally be combined with floats or instances of fractions.Fraction in arithmetic operations", and an attempt raises `TypeError`, but comparisons between them work.[^6] Example: [`library-types`](#library-types).

## Examples

`Fraction` stays exact with an `int`, becomes a `float` with a `float`, and `Decimal` refuses a `float` (rule r6).

```python run id=library-types
from decimal import Decimal
from fractions import Fraction

print(Fraction(1, 3) + 1, Fraction(1, 2) + 0.5, Decimal("1.1") + 1)
try:
    Decimal("1.1") + 1.0
except TypeError:
    print("Decimal + float: TypeError")
print(Decimal("0.5") == 0.5, Decimal("1.1") < 1.2)
```

```output for=library-types
4/3 1.0 2.1
Decimal + float: TypeError
True True
```

## Pitfalls

### Arithmetic converts, comparison does not

`2**53 + 1` has no exact float. In `n + 0.0` the integer is converted to the nearest float (rule r2), but `==` compares the exact values (rule r5), so the two sides differ.

```python wrong id=exact-compare
n = 2 ** 53 + 1
print(n == n + 0.0, n == float(n), float(n) == n - 1)
```

```output for=exact-compare
False False True
```

### Large integers in float arithmetic

An `int` has no size limit, but a float does. When the integer operand is converted, a value out of the float range raises `OverflowError`.

```python fails id=too-large-for-float expect=exception:OverflowError
print(10 ** 400 + 0.5)
```

## Comparison

Go does not convert between numeric types for an operator. The operand types "must be identical unless the operation involves shifts or untyped constants", and an untyped constant is converted to the type of the other operand.[^7] So `x + 0.5` with an `int` variable `x` does not compile in Go. In Python it gives a float (rule r2).
