---
tag: SXWBDY
kind: feature
owner: python
slug: float-type
name: The float type
aliases: [Python float, double precision in Python, float(), sys.float_info, float.hex, NaN and infinity in Python]
area: languages/python/values-and-types
level: intro
summary: >-
  float is a binary floating-point number, usually a C double. float() reads decimal strings, "inf" and "nan", and hex() and fromhex() give exact text forms. NaN is not equal to itself.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex", locator: "Numeric Types - int, float, complex"}
see_also: [python/numeric-literals, python/int-type, python/numbers, floating-point-arithmetic]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types - int, float, complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#additional-methods-on-float
    title: Built-in Types, Python 3.15 documentation
    locator: Additional Methods on Float
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#float
    title: Built-in Functions, Python 3.15 documentation
    locator: float()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.float_info
    title: sys, System-specific parameters and functions, Python 3.15 documentation
    locator: sys.float_info
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#numbers-real-float
    title: The Python Language Reference, Python 3.15
    locator: 3.2.4.2. numbers.Real (float)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10.1. Value comparisons
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#round
    title: Built-in Functions, Python 3.15 documentation
    locator: round()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Numeric_types
    title: The Go Programming Language Specification
    locator: Numeric types
    accessed: 2026-11-16
---

## Synopsis

```python run id=float-basics
import sys

print(sys.float_info.mant_dig, sys.float_info.dig, sys.float_info.epsilon)
print(sys.float_info.max)
print(0.1 + 0.2, 1e308 * 10, float("-inf"), float(" 1_000.5 "))
```

```output for=float-basics
53 15 2.220446049250313e-16
1.7976931348623157e+308
0.30000000000000004 inf -inf 1000.5
```

A float has a 53-bit significand. `0.1 + 0.2` is not exactly `0.3`, and a result that is too large becomes `inf`.

## Semantics

**r1.** Floating-point numbers "are usually implemented using double in C", and `sys.float_info` gives the precision and the internal representation on the current machine.[^1] Its fields come from the C header `float.h`. For example, `epsilon` is the difference between `1.0` and the next larger float, and `max` is the largest finite float.[^4] Example: [`float-basics`](#float-basics).

**r2.** A float represents a "machine-level double precision" number, and the range and the handling of overflow depend on the machine. Python has no single-precision float type.[^5]

**r3.** A numeric literal with a decimal point or an exponent gives a float.[^1] `float()` reads a decimal number with an optional sign and whitespace around it, and also the strings for NaN and positive or negative infinity, in any case, such as `inf`, `Infinity` and `nan`. With an integer or a float, it returns a float with the same value "within Python's floating-point precision", and a value outside the float range raises `OverflowError`.[^3] Example: [`float-basics`](#float-basics).

**r4.** `as_integer_ratio()` returns a pair of integers whose ratio is exactly the float, and `is_integer()` tests for a finite integral value. A decimal string usually has a small rounding error, but hexadecimal strings "allow exact representation and specification of floating-point numbers": `hex()` writes one and the class method `float.fromhex()` reads one.[^2] Example: [`exact-forms`](#exact-forms).

**r5.** "Any ordered comparison of a number to a not-a-number value is false", and a NaN is not equal to itself, as IEEE 754 requires.[^6] Example: [`nan-compare`](#nan-compare).

**r6.** Conversion from float to int "truncates, discarding the fractional part".[^1] `round()` rounds to the closer multiple of the power of ten and, on a tie, "toward the even choice".[^7] Example: [`round-half-even`](#round-half-even).

## Examples

The exact value of `0.1` is a ratio with a power of two in the denominator. The hexadecimal forms are exact (rule r4).

```python run id=exact-forms
print((0.25).as_integer_ratio(), (0.1).as_integer_ratio())
print((-2.0).is_integer(), (3.2).is_integer())
print(float.fromhex("0x3.a7p10"), (3740.0).hex(), (0.1).hex())
```

```output for=exact-forms
(1, 4) (3602879701896397, 36028797018963968)
True False
3740.0 0x1.d380000000000p+11 0x1.999999999999ap-4
```

## Pitfalls

### NaN is not equal to itself

`x == x` is false for a NaN, and every ordered comparison with it is false (rule r5). `x != x` is true only for a NaN.

```python wrong id=nan-compare
x = float("nan")
print(x == x, x != x, 3 < x, x < 3, x > 3)
```

```output for=nan-compare
False True False False False
```

### `round()` and halves

`round(2.5)` is `2`, because a tie goes to the even neighbor (rule r6). `round(2.675, 2)` is `2.67` and not `2.68`. Built-in Functions says that this is not a bug: most decimal fractions cannot be represented exactly as a float.[^7]

```python wrong id=round-half-even
print(round(0.5), round(1.5), round(2.5), round(3.5))
print(round(2.675, 2))
```

```output for=round-half-even
0 2 2 4
2.67
```

### Comparing results of float arithmetic with `==`

`0.1 + 0.2 == 0.3` is false (see `float-basics`). Compare with a tolerance, for example with `math.isclose()`, or use `decimal.Decimal` or `fractions.Fraction` when exact decimal or rational values matter.

## Comparison

Go has two floating-point types, `float32` and `float64`, "the set of all IEEE 754 32-bit floating-point numbers" and the 64-bit set.[^8] Python has one float type, usually a C `double`, and no single-precision type like `float32` (rules r1 and r2).
