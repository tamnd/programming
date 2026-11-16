---
tag: 0YKRY9
kind: feature
owner: python
slug: floating-point-arithmetic
name: "Floating-point arithmetic: issues and limitations"
aliases: [Python float precision, why 0.1 + 0.2 is not 0.3, math.isclose, math.fsum, float repr]
area: languages/python/values-and-types
level: core
summary: >-
  Python floats are binary fractions, so most decimal fractions such as 0.1 are stored as close approximations. repr() shows the shortest string that gives the same float. isclose() and fsum() help.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/floatingpoint.html", locator: "15. Floating-Point Arithmetic: Issues and Limitations"}
see_also: [python/float-type, python/representation-error, python/decimal-floating-point-arithmetic, python/fractions, floating-point-arithmetic]
sources:
  - url: https://docs.python.org/3.15/tutorial/floatingpoint.html#tut-fp-issues
    title: The Python Tutorial, Python 3.15
    locator: 15. Floating-Point Arithmetic, Issues and Limitations
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Numeric_types
    title: The Go Programming Language Specification
    locator: Numeric types
    accessed: 2026-11-16
---

## Synopsis

```python run id=tenth
print(0.1, 0.1 + 0.2, 0.1 + 0.1 + 0.1 == 0.3)
print(f"{0.1:.55f}")
print((0.1).as_integer_ratio())
```

```output for=tenth
0.1 0.30000000000000004 False
0.1000000000000000055511151231257827021181583404541015625
(3602879701896397, 36028797018963968)
```

`0.1` prints as `0.1`, but the stored value is a binary fraction slightly larger than one tenth.

## Semantics

**r1.** Floats are stored "as base 2 (binary) fractions", and "most decimal fractions cannot be represented exactly as binary fractions". So the decimal numbers in a program are only approximated by the stored binary numbers.[^1] Example: [`tenth`](#tenth).

**r2.** On most machines, a float is a binary fraction with a 53-bit numerator and a power of two as the denominator. For `0.1` this is `3602879701896397 / 2 ** 55`.[^1] Example: [`tenth`](#tenth).

**r3.** Python prints a rounded decimal value. Many decimal numbers share the same nearest binary fraction, and any of them keeps `eval(repr(x)) == x` true. Python, on most systems, chooses the shortest of them, so `0.1` prints as `0.1`.[^1] Example: [`tenth`](#tenth).

**r4.** This is "in the very nature of binary floating point" and not a bug in Python or in the program. All languages that use the hardware floating-point arithmetic show it, but some do not display the difference by default.[^1]

**r5.** Because `0.1` is not exactly 1/10, `0.1 + 0.1 + 0.1 == 0.3` is false, and rounding the operands first with `round()` does not help. `math.isclose()` compares inexact values, and `round()` compares rough approximations.[^1] Example: [`compare-floats`](#compare-floats).

**r6.** The error of a float operation is, on most machines, "no more than 1 part in 2**53 per operation", but every float operation "can suffer a new rounding error".[^1] For exact decimal values the tutorial points to the `decimal` module, and for exact rational values to the `fractions` module.[^1]

**r7.** `float.as_integer_ratio()` and `float.hex()` give the exact stored value, and `float.fromhex()` reconstructs the float from the hexadecimal form. `sum()` uses extended precision for intermediate rounding steps, and `math.fsum()` tracks all the lost digits, so the result has a single rounding.[^1] Example: [`sum-and-fsum`](#sum-and-fsum).

## Examples

`isclose()` accepts a small difference, and `round()` compares the first digits (rule r5).

```python run id=compare-floats
import math

print(round(0.1, 1) + round(0.1, 1) + round(0.1, 1) == round(0.3, 1))
print(math.isclose(0.1 + 0.1 + 0.1, 0.3))
print(round(math.pi, ndigits=2) == round(22 / 7, ndigits=2))
print(format(math.pi, ".12g"), format(math.pi, ".2f"), repr(math.pi))
```

```output for=compare-floats
False
True
True
3.14159265359 3.14 3.141592653589793
```

The six numbers of the tutorial almost cancel out. `math.fsum()` gives the correctly rounded sum, `sum()` is close, and a plain loop has no correct digits (rule r7).

```python run id=sum-and-fsum
import math

arr = [-0.10430216751806065, -266310978.67179024, 143401161448607.16,
       -143401161400469.7, 266262841.31058735, -0.003244936839808227]
print(math.fsum(arr))
print(sum(arr))
total = 0.0
for x in arr:
    total += x
print(total)
```

```output for=sum-and-fsum
8.042173697819788e-13
8.042178034628478e-13
-0.0051575902860057365
```

## Pitfalls

### `==` on computed floats

Ten additions of `0.1` do not give `1.0` in a loop of `+` operations (rule r5). Compare with `math.isclose()`, or add with `math.fsum()` (rule r7).

```python wrong id=equal-after-sum
import math

total = 0.0
for _ in range(10):
    total += 0.1
print(total, total == 1.0, math.isclose(total, 1.0), math.fsum([0.1] * 10))
```

```output for=equal-after-sum
0.9999999999999999 False True 1.0
```

### The display hides the error

`print()` shows the shortest string that gives the same float (rule r3), so `0.1` looks exact. Format with many digits, or use `as_integer_ratio()`, to see the stored value (see `tenth`).

## Comparison

Go's `float32` and `float64` are "the set of all IEEE 754 32-bit floating-point numbers" and the 64-bit set.[^2] Almost all platforms map Python floats to IEEE 754 binary64 values,[^1] so a Go `float64` has the same representation error as a Python float. The tutorial says that all languages that use the hardware floating-point arithmetic show it (rule r4).
