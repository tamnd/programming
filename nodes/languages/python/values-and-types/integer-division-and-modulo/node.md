---
tag: GF46WX
kind: feature
owner: python
slug: integer-division-and-modulo
name: Integer division and modulo
aliases: [floor division, Python // operator, Python % operator, divmod, negative modulo in Python, math.fmod]
area: languages/python/values-and-types
level: core
summary: >-
  Python's // rounds the quotient toward minus infinity, and % takes the sign of the divisor, so x == (x//y)*y + x%y. Go truncates instead. math.fmod() follows C for floats.
grammar: "m_expr: ... | m_expr '//' u_expr | m_expr '%' u_expr (Language Reference, 6.7)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations", locator: "6.7. Binary arithmetic operations"}
see_also: [python/arithmetic-operators, python/numbers, python/int-type, python/arithmetic-conversions]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#binary-arithmetic-operations
    title: The Python Language Reference, Python 3.15
    locator: 6.7. Binary arithmetic operations, and footnotes 1 and 2
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types - int, float, complex, notes 1 and 2
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#divmod
    title: Built-in Functions, Python 3.15 documentation
    locator: divmod()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/math.html#math.fmod
    title: math, Mathematical functions, Python 3.15 documentation
    locator: math.fmod()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/decimal.html#decimal-objects
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: Decimal objects
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Integer_operators
    title: The Go Programming Language Specification
    locator: Integer operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=floor-and-mod
print(7 // 2, -7 // 2, 7 // -2, -7 // -2)
print(7 % 3, -7 % 3, 7 % -3, -7 % -3)
print(divmod(-7, 3), divmod(7.5, 2))
```

```output for=floor-and-mod
3 -4 -4 3
1 2 -2 -1
(-3, 2) (3.0, 1.5)
```

`-7 // 2` is `-4`, not `-3`, and `-7 % 3` is `2`, not `-1`. The remainder has the sign of the divisor.

## Semantics

**r1.** `//` gives the result of mathematical division "with the 'floor' function applied to the result". For integers the result is an integer.[^1] The result "is always rounded towards minus infinity": `1//2` is `0`, `(-1)//2` is `-1`, `1//(-2)` is `-1` and `(-1)//(-2)` is `0`.[^2] Example: [`floor-and-mod`](#floor-and-mod).

**r2.** `%` gives the remainder of the division of the first argument by the second. The result has "the same sign as its second operand (or zero)", and its absolute value is smaller than the absolute value of the second operand.[^1] Example: [`floor-and-mod`](#floor-and-mod).

**r3.** The two operators are connected by the identity `x == (x//y)*y + (x%y)`, and `divmod(x, y) == (x//y, x%y)`.[^1] For floats, `divmod()` returns `(q, a % b)` where `q` is usually `math.floor(a / b)` but may be 1 less, and `q * b + a % b` is very close to `a`.[^3] Example: [`identity`](#identity).

**r4.** The numeric arguments are converted to a common type first. A zero right operand raises `ZeroDivisionError`. The arguments can be floats: `3.14 % 0.7` equals `0.34`.[^1] `//`, `%` and `divmod()` are not defined for complex numbers.[^1]

**r5.** For floats, `abs(x % y) < abs(y)` can fail because of rounding. `-1e-100 % 1e100` must have the sign of `1e100`, so it is `1e100 - 1e-100`, which rounds to `1e100`. `math.fmod()` gives a result with the sign of the first argument, so `math.fmod(-1e-100, 1e100)` is `-1e-100`.[^1] The `math` documentation prefers `fmod()` for floats and `%` for integers.[^4] Example: [`fmod-vs-mod`](#fmod-vs-mod).

**r6.** `decimal.Decimal` follows a different rule. Its `%` gives a result with the sign of the dividend, and its `//` truncates toward zero, which keeps the identity `x == (x // y) * y + x % y`.[^5] Example: [`decimal-truncates`](#decimal-truncates).

## Examples

The identity of rule r3 holds for every sign combination.

```python run id=identity
pairs = [(17, 5), (-17, 5), (17, -5), (-17, -5)]
print(all(x == (x // y) * y + x % y for x, y in pairs))
print([divmod(x, y) for x, y in pairs])
```

```output for=identity
True
[(3, 2), (-4, 3), (-4, -3), (3, -2)]
```

A floored `%` is useful for wrap-around arithmetic, such as hours on a clock. The result is never negative for a positive divisor (rule r2).

```python run id=clock
hour = 1
print((hour - 3) % 12, (hour - 3) % 24, (hour + 25) % 24)
```

```output for=clock
10 22 2
```

`%` and `math.fmod()` differ for operands with different signs (rule r5).

```python run id=fmod-vs-mod
import math

print(-7 % 3, math.fmod(-7, 3))
print(-1e-100 % 1e100, math.fmod(-1e-100, 1e100))
```

```output for=fmod-vs-mod
2 -1.0
1e+100 -1e-100
```

## Pitfalls

### `int(a / b)` is not `a // b`

`int()` truncates toward zero, but `//` rounds toward minus infinity (rule r1). For a negative quotient the two differ.

```python wrong id=int-of-quotient
a, b = -7, 2
print(int(a / b), a // b)
```

```output for=int-of-quotient
-3 -4
```

### `Decimal` does not floor

Code that works with `int` can change its result when the values become `Decimal` (rule r6).

```python wrong id=decimal-truncates
from decimal import Decimal

print(-7 % 4, Decimal(-7) % Decimal(4))
print(-7 // 4, Decimal(-7) // Decimal(4))
```

```output for=decimal-truncates
1 -3
-2 -1
```

### Division by zero

`//` and `%` with a zero right operand raise `ZeroDivisionError`, for floats too (rule r4).

```python fails id=mod-by-zero expect=exception:ZeroDivisionError
print(5.0 % 0)
```

## Comparison

Go truncates: for integers, `x / y` is "truncated towards zero", and `x % y` has the sign of `x`. In Go `-5 / 3` is `-1` and `-5 % 3` is `-2`. In Python `-5 // 3` is `-2` and `-5 % 3` is `1` (rules r1 and r2). Both languages keep an identity of the form `x = q*y + r`.[^6]
