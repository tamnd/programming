---
tag: WT5YEB
kind: feature
owner: python
slug: fractions
name: Fractions
aliases: [fractions module, fractions.Fraction, Python rational numbers, exact rational arithmetic, limit_denominator]
area: languages/python/values-and-types
level: core
summary: >-
  fractions.Fraction is an exact rational number in lowest terms, with a positive denominator. Arithmetic on fractions and ints stays exact, and limit_denominator() finds a simple fraction near a float.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/fractions.html", locator: "fractions, Rational numbers"}
see_also: [python/the-numeric-tower, python/decimal-floating-point-arithmetic, python/floating-point-arithmetic, python/arithmetic-conversions]
sources:
  - url: https://docs.python.org/3.15/library/fractions.html#fractions.Fraction
    title: fractions, Rational numbers, Python 3.15 documentation
    locator: class fractions.Fraction, numerator, denominator
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/fractions.html#fractions.Fraction.limit_denominator
    title: fractions, Rational numbers, Python 3.15 documentation
    locator: limit_denominator()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/floatingpoint.html#tut-fp-issues
    title: The Python Tutorial, Python 3.15
    locator: 15. Floating-Point Arithmetic, Issues and Limitations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Constant_expressions
    title: The Go Programming Language Specification
    locator: Numeric types; Constants; Constant expressions
    accessed: 2026-11-16
---

## Synopsis

```python run id=fraction-basics
from fractions import Fraction

x = Fraction(1, 3) + Fraction(1, 6)
print(x, x.numerator, x.denominator, float(x))
print(Fraction(16, -10), Fraction("3/7"), Fraction("-.125"), Fraction(2.25))
print(Fraction(1, 10) * 3 == Fraction(3, 10), 0.1 * 3 == 0.3)
```

```output for=fraction-basics
1/2 1 2 0.5
-8/5 3/7 -1/8 9/4
True False
```

Three tenths is exact with fractions, but not with floats. A fraction is stored in lowest terms, and the sign moves to the numerator.

## Semantics

**r1.** The `fractions` module "provides support for rational number arithmetic". A `Fraction` is made from a pair of rational numbers, from a single number or from a string.[^1] Example: [`fraction-basics`](#fraction-basics).

**r2.** `Fraction(numerator, denominator)` needs two instances of `numbers.Rational` and gives a fraction with the value `numerator/denominator`. A zero denominator raises `ZeroDivisionError`.[^1] Example: [`zero-denominator`](#zero-denominator).

**r3.** `Fraction(number)` accepts a `numbers.Rational` or an object with an `as_integer_ratio()` method, such as a `float` or a `decimal.Decimal`, and returns a fraction with "exactly the same value". So `Fraction(1.1)` is not `Fraction(11, 10)`.[^1] Example: [`from-float`](#from-float).

**r4.** `Fraction(string)` reads a string of the form `[sign] numerator ['/' denominator]`, or any string for a finite value that `float()` accepts. Whitespace around the string is allowed.[^1] Example: [`fraction-basics`](#fraction-basics).

**r5.** `Fraction` inherits from `numbers.Rational` and implements all its methods and operations. Fractions are hashable and should be treated as immutable. `numerator` and `denominator` are in lowest terms, and the denominator is always positive.[^1] Example: [`fraction-basics`](#fraction-basics).

**r6.** `limit_denominator(max_denominator=1000000)` "Finds and returns the closest Fraction to self that has denominator at most max_denominator". It finds rational approximations to a float, and it recovers a rational number that is stored as a float.[^2] Example: [`from-float`](#from-float).

**r7.** The tutorial names the `fractions` module as one form of exact arithmetic, "so the numbers like 1/3 can be represented exactly".[^3]

## Examples

`Fraction(1.1)` keeps the exact value of the float. `limit_denominator()` gives back `11/10`, and finds `355/113` as an approximation of π (rules r3 and r6).

```python run id=from-float
from decimal import Decimal
from fractions import Fraction

print(Fraction(1.1))
print(Fraction(1.1).limit_denominator(), Fraction(Decimal("1.1")))
print(Fraction("3.1415926535897932").limit_denominator(1000))
```

```output for=from-float
2476979795053773/2251799813685248
11/10 11/10
355/113
```

## Pitfalls

### A float operand ends exact arithmetic

A `Fraction` combined with a `float` gives a `float` (see `python/arithmetic-conversions`), and a power with a fractional exponent gives a `float` too. Keep all the operands as fractions or integers to keep the result exact.

```python wrong id=float-operand
from fractions import Fraction

third = Fraction(1, 3)
print(third + 1, type(third + 1).__name__)
print(third + 0.5, type(third + 0.5).__name__)
print(Fraction(1, 4) ** 0.5, Fraction(1, 3) ** 2)
```

```output for=float-operand
4/3 Fraction
0.8333333333333333 float
0.5 1/9
```

### A zero denominator

`Fraction(1, 0)` fails at once (rule r2).

```python fails id=zero-denominator expect=exception:ZeroDivisionError
from fractions import Fraction

print(Fraction(1, 0))
```

## Comparison

Go's predeclared numeric types are integer, floating-point and complex types. Go constants come close to fractions: "Numeric constants represent exact values of arbitrary precision and do not overflow", and "Constant expressions are always evaluated exactly". But constant expressions "may contain only constant operands and are evaluated at compile time".[^4] A Python `Fraction` is exact at run time, for any value that the program computes (rule r5).
