---
tag: CYCBVZ
kind: feature
owner: python
slug: representation-error
name: Representation error
aliases: [why 0.1 is not exact, 0.1 as a binary fraction, IEEE 754 binary64 in Python, Decimal.from_float, Fraction.from_float]
area: languages/python/values-and-types
level: advanced
summary: >-
  Most decimal fractions have no exact binary form. The tutorial computes the double nearest to 0.1, 3602879701896397 / 2**55, which is a little larger than 1/10, and shows it with Fraction and Decimal.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/floatingpoint.html#representation-error", locator: "15.1. Representation Error"}
see_also: [python/floating-point-arithmetic, python/float-type, python/fractions, python/decimal-floating-point-arithmetic, floating-point-arithmetic]
sources:
  - url: https://docs.python.org/3.15/tutorial/floatingpoint.html#representation-error
    title: The Python Tutorial, Python 3.15
    locator: 15.1. Representation Error
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/decimal.html#quick-start-tutorial
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: Quick-start tutorial
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Constants
    title: The Go Programming Language Specification
    locator: Constants; Numeric types
    accessed: 2026-11-16
---

## Synopsis

```python run id=exact-tenth
from decimal import Decimal
from fractions import Fraction

print(Fraction.from_float(0.1))
print(Decimal.from_float(0.1))
print(format(0.1, ".17f"))
```

```output for=exact-tenth
3602879701896397/36028797018963968
0.1000000000000000055511151231257827021181583404541015625
0.10000000000000001
```

The float `0.1` is exactly the fraction on the first line, and its exact decimal value is on the second line.

## Semantics

**r1.** "Representation error refers to the fact that some (most, actually) decimal fractions cannot be represented exactly as binary (base 2) fractions." It is the main reason why Python and many other languages often do not display the exact decimal number that the user expects.[^1]

**r2.** Almost all machines use IEEE 754 binary floating-point arithmetic, and almost all platforms map Python floats to IEEE 754 binary64 "double precision" values, which have 53 bits of precision. So on input, `0.1` becomes the closest fraction of the form `J/2**N`, where `J` is an integer with exactly 53 bits.[^1]

**r3.** `J` must be at least `2**52` and less than `2**53`, so the only possible `N` is 56. `J` is the quotient `2**56 // 10`, rounded. The remainder is 6, which is more than half of 10, so the quotient is rounded up to `7205759403792794`.[^1] Example: [`find-j`](#find-j).

**r4.** `7205759403792794 / 2**56` reduces to `3602879701896397 / 2**55`. Because the quotient was rounded up, this value is a little larger than 1/10. The exact stored value is `0.1000000000000000055511151231257827021181583404541015625`.[^1] Example: [`find-j`](#find-j).

**r5.** Instead of the full decimal value, many languages, including older versions of Python, round the display to 17 significant digits, which gives `0.10000000000000001`.[^1] Example: [`exact-tenth`](#exact-tenth).

**r6.** The `fractions` and `decimal` modules give the exact value: `Fraction.from_float(0.1)`, `(0.1).as_integer_ratio()` and `Decimal.from_float(0.1)`.[^1] A `Decimal` made from a float is an exact conversion of the float's value.[^2] Example: [`exact-tenth`](#exact-tenth).

## Examples

The steps of the tutorial, in code (rules r3 and r4).

```python run id=find-j
print(2**52 <= 2**56 // 10 < 2**53)
q, r = divmod(2**56, 10)
print(r, q + 1)
print(0.1 * 2 ** 55)
print(3602879701896397 * 10 ** 55 // 2 ** 55)
```

```output for=find-j
True
6 7205759403792794
3602879701896397.0
1000000000000000055511151231257827021181583404541015625
```

## Pitfalls

### `0.1` is larger than one tenth, but `0.3` is smaller

The rounding direction depends on the number. The stored `0.1` is above 1/10 (rule r4), and the exact value of the stored `0.3` is below 3/10, as the output shows.

```python wrong id=directions
from decimal import Decimal
from fractions import Fraction

print(Fraction(0.1) > Fraction(1, 10), Fraction(0.3) < Fraction(3, 10))
print(Decimal(0.3))
print(0.1 + 0.1 + 0.1 == 0.3)
```

```output for=directions
True True
0.299999999999999988897769753748434595763683319091796875
False
```

### `Decimal(0.1)` does not fix the error

A `Decimal` made from the float `0.1` has the 55 digits of the binary value (rule r6). Write `Decimal("0.1")` to get one tenth.

## Comparison

Go constants avoid representation error at compile time: "Numeric constants represent exact values of arbitrary precision and do not overflow."[^3] A Go `float64` is an IEEE 754 64-bit number,[^3] so a `float64` variable stores `0.1` with the same error as a Python float (rule r2).
