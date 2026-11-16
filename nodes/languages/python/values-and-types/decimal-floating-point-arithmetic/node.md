---
tag: 3JCEQW
kind: feature
owner: python
slug: decimal-floating-point-arithmetic
name: Decimal floating-point arithmetic
aliases: [decimal module, decimal.Decimal, Python money arithmetic, getcontext, quantize, localcontext]
area: languages/python/values-and-types
level: core
summary: >-
  decimal.Decimal does floating-point arithmetic in base 10, so 0.1 is exact. A context sets the precision (28 digits by default), the rounding and the traps, and quantize() rounds to a fixed place.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/decimal.html", locator: "decimal, Decimal fixed-point and floating-point arithmetic"}
see_also: [python/fractions, python/floating-point-arithmetic, python/float-type, floating-point-arithmetic]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#decimal-floating-point-arithmetic
    title: The Python Tutorial, Python 3.15
    locator: 11.8. Decimal floating-point arithmetic
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/decimal.html
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/decimal.html#quick-start-tutorial
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: Quick-start tutorial
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/decimal.html#decimal.localcontext
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: localcontext()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Numeric_types
    title: The Go Programming Language Specification
    locator: Numeric types
    accessed: 2026-11-16
---

## Synopsis

```python run id=tax-example
from decimal import Decimal

print(round(Decimal("0.70") * Decimal("1.05"), 2), round(.70 * 1.05, 2))
print(Decimal("1.00") % Decimal(".10"), 1.00 % 0.10)
print(sum([Decimal("0.1")] * 10) == Decimal("1.0"))
```

```output for=tax-example
0.74 0.73
0.00 0.09999999999999995
True
```

A 5% tax on 70 cents rounds to 74 cents with `Decimal` and to 73 cents with `float`. The examples come from the tutorial.

## Semantics

**r1.** The `decimal` module offers a `Decimal` type for decimal floating-point arithmetic. The tutorial names its uses: financial applications and other uses that need "exact decimal representation", control over precision and rounding, tracking of significant decimal places, or results that match calculations done by hand.[^1] Example: [`tax-example`](#tax-example).

**r2.** "Decimal numbers can be represented exactly." In decimal floating point, `0.1 + 0.1 + 0.1 - 0.3` is exactly zero.[^2] Example: [`tax-example`](#tax-example).

**r3.** A `Decimal` keeps significant places, so `1.30 + 1.20` is `2.50`. Multiplication keeps all the figures: `1.3 * 1.2` gives `1.56`, and `1.30 * 1.20` gives `1.5600`.[^2] Example: [`significance`](#significance).

**r4.** A `Decimal` can be made from an integer, a string, a float or a tuple. Construction from an integer or a float "performs an exact conversion of the value". The precision of a new `Decimal` comes only from the digits of the input. The context precision and rounding apply only in arithmetic operations.[^3] Example: [`decimal-from-float`](#decimal-from-float).

**r5.** The context sets the precision, the rounding rule, the exponent limits, the flags and the traps. The default precision is 28 places.[^2] The default context uses `ROUND_HALF_EVEN` and traps `Overflow`, `DivisionByZero` and `InvalidOperation`.[^3] `getcontext()` gives the current context. `localcontext()` returns a context manager that uses a copy of a context in a `with` statement and restores the previous context at the end.[^4] Example: [`context-precision`](#context-precision).

**r6.** The signals are `Clamped`, `InvalidOperation`, `DivisionByZero`, `Inexact`, `Rounded`, `Subnormal`, `Overflow`, `Underflow` and `FloatOperation`. Each signal has a flag and a trap enabler. A signal sets its flag, and if its trap is enabled, an exception is raised.[^2] Example: [`division-by-zero`](#division-by-zero).

**r7.** `quantize()` "rounds a number to a fixed exponent", which is useful for monetary values that are rounded to a fixed number of places.[^3] Example: [`significance`](#significance).

## Examples

Trailing zeros show significance, and `quantize()` rounds to cents with a chosen rule (rules r3 and r7).

```python run id=significance
from decimal import ROUND_DOWN, ROUND_UP, Decimal

print(Decimal("1.30") + Decimal("1.20"), Decimal("1.3") * Decimal("1.2"))
print(Decimal("1.30") * Decimal("1.20"))
print(Decimal("7.325").quantize(Decimal(".01"), rounding=ROUND_DOWN))
print(Decimal("7.325").quantize(Decimal("1."), rounding=ROUND_UP))
```

```output for=significance
2.50 1.56
1.5600
7.32
8
```

The default precision is 28 digits. `localcontext()` changes it only inside the `with` block (rule r5).

```python run id=context-precision
from decimal import Decimal, getcontext, localcontext

print(getcontext().prec, getcontext().rounding)
with localcontext() as ctx:
    ctx.prec = 6
    print(Decimal(1) / Decimal(7))
print(Decimal(1) / Decimal(7))
```

```output for=context-precision
28 ROUND_HALF_EVEN
0.142857
0.1428571428571428571428571429
```

## Pitfalls

### `Decimal(3.14)` is not `Decimal("3.14")`

Construction from a float is exact (rule r4), so it keeps the binary error of the float. Make a `Decimal` from a string.

```python wrong id=decimal-from-float
from decimal import Decimal

print(Decimal(3.14))
print(Decimal("3.14"))
```

```output for=decimal-from-float
3.140000000000000124344978758017532527446746826171875
3.14
```

### Division by zero raises

`DivisionByZero` is trapped in the default context (rule r5), so `Decimal(1) / Decimal(0)` raises an exception and does not give `Infinity`.

```python fails id=division-by-zero expect=exception:DivisionByZero
from decimal import Decimal

print(Decimal(1) / Decimal(0))
```

## Comparison

Go's predeclared numeric types are integers, IEEE 754 binary floats and complex numbers built from them.[^5] Go has no decimal floating-point type in the language. Python has one in its standard library, with a context for precision and rounding (rule r5).
