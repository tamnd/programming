---
tag: GYWPG8
kind: feature
owner: python
slug: the-numeric-tower
name: The numeric tower
aliases: [numbers module, numeric abstract base classes, numbers.Number, numbers.Real, numbers.Integral, PEP 3141]
area: languages/python/values-and-types
level: advanced
summary: >-
  The numbers module defines the abstract base classes Number, Complex, Real, Rational and Integral. Each one adds operations to the one above it, and isinstance() with them tests what a number can do.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/numbers.html", locator: "numbers, Numeric abstract base classes"}
see_also: [python/int-type, python/float-type, python/complex-type, python/arithmetic-conversions, python/none-notimplemented-and-ellipsis, subtyping]
sources:
  - url: https://docs.python.org/3.15/library/numbers.html
    title: numbers, Numeric abstract base classes, Python 3.15 documentation
    locator: The numeric tower; Notes for type implementers
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#additional-methods-on-integer-types
    title: Built-in Types, Python 3.15 documentation
    locator: Additional Methods on Integer Types, on Float, on Complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/fractions.html#fractions.Fraction
    title: fractions, Rational numbers, Python 3.15 documentation
    locator: class fractions.Fraction
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Operators
    title: The Go Programming Language Specification
    locator: Operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=tower-membership
from decimal import Decimal
from fractions import Fraction
from numbers import Complex, Integral, Number, Rational, Real

abcs = (Number, Complex, Real, Rational, Integral)
for v in [7, True, 1.5, 2j, Fraction(1, 3), Decimal("1.1")]:
    print(type(v).__name__, [c.__name__ for c in abcs if isinstance(v, c)])
```

```output for=tower-membership
int ['Number', 'Complex', 'Real', 'Rational', 'Integral']
bool ['Number', 'Complex', 'Real', 'Rational', 'Integral']
float ['Number', 'Complex', 'Real']
complex ['Number', 'Complex']
Fraction ['Number', 'Complex', 'Real', 'Rational']
Decimal ['Number']
```

Each built-in number type is an instance of the classes above its own level. The output shows that `Decimal` is a `Number` but not a `Real`.

## Semantics

**r1.** The `numbers` module "defines a hierarchy of numeric abstract base classes which progressively define more operations". None of its types is intended to be instantiated.[^1] Example: [`no-instances`](#no-instances).

**r2.** `Number` is the root of the hierarchy. To check that `x` is a number of any kind, use `isinstance(x, Number)`.[^1] Example: [`tower-membership`](#tower-membership).

**r3.** `Complex` has the operations of the built-in `complex` type: conversions to `complex` and `bool`, `real`, `imag`, `+`, `-`, `*`, `/`, `**`, `abs()`, `conjugate()`, `==` and `!=`. `Real` adds the operations on real numbers: conversion to `float`, `math.trunc()`, `round()`, `math.floor()`, `math.ceil()`, `divmod()`, `//`, `%` and the order comparisons.[^1]

**r4.** `Rational` adds the `numerator` and `denominator` properties, which should be in lowest terms with a positive denominator. `Integral` adds conversion to `int`, `pow()` with a modulus, and the bit-string operations `<<`, `>>`, `&`, `^`, `|` and `~`.[^1] Example: [`fractions-of-numbers`](#fractions-of-numbers).

**r5.** `int` implements `numbers.Integral`, `float` implements `numbers.Real`, and `complex` implements `numbers.Complex`.[^2] `fractions.Fraction` inherits from `numbers.Rational`.[^3] Example: [`tower-membership`](#tower-membership).

**r6.** More classes can be added to the hierarchy. For example, a class `MyFoo` that subclasses `Complex` is put between `Complex` and `Real` with `MyFoo.register(Real)`.[^1] Example: [`insert-a-level`](#insert-a-level).

**r7.** Implementers should "make equal numbers equal and hash them to the same values". A mixed-type operation should either call an implementation that knows both types, or convert both operands to the nearest built-in type. So `__add__()` returns `NotImplemented` for an operand that it does not know, and `__radd__()` holds the fallback to `int`, `float` or `complex`.[^1] Example: [`mixed-mode`](#mixed-mode).

## Examples

Every `Rational` has a numerator and a denominator, so an `int` has them too, and every `Real` has `real` and `imag` (rules r3 and r4).

```python run id=fractions-of-numbers
from fractions import Fraction

print(Fraction(6, 8).numerator, Fraction(6, 8).denominator)
print((7).numerator, (7).denominator, (2.5).real, (2.5).imag)
```

```output for=fractions-of-numbers
3 4
7 1 2.5 0.0
```

`MyFoo.register(Real)` makes `Real`, and so `float` and `int`, virtual subclasses of `MyFoo`. `complex` is not one (rule r6).

```python run id=insert-a-level
from numbers import Complex, Real

class MyFoo(Complex):
    pass

MyFoo.register(Real)
print(issubclass(Real, MyFoo), isinstance(1.5, MyFoo), isinstance(7, MyFoo))
print(isinstance(1j, MyFoo))
```

```output for=insert-a-level
True True True
False
```

`Tally.__add__()` knows only `Tally`, and `Tally.__radd__()` falls back to the built-in types (rule r7). For `1 + Tally(2)`, `int.__add__()` gives `NotImplemented`, and Python calls `Tally.__radd__()`.

```python run id=mixed-mode
from numbers import Integral, Real

class Tally:
    def __init__(self, n):
        self.n = n
    def __add__(self, other):
        if isinstance(other, Tally):
            return Tally(self.n + other.n)
        return NotImplemented
    def __radd__(self, other):
        if isinstance(other, Integral):
            return int(other) + self.n
        if isinstance(other, Real):
            return float(other) + self.n
        return NotImplemented

print((Tally(2) + Tally(3)).n, 1 + Tally(2), 1.5 + Tally(2))
```

```output for=mixed-mode
5 3 3.5
```

## Pitfalls

### `isinstance(x, float)` to test for a real number

An `int` or a `Fraction` is a real number, but it is not a `float`. Test with `numbers.Real` (rule r5).

```python wrong id=float-check
from fractions import Fraction
from numbers import Real

for v in [2, 2.0, Fraction(1, 2)]:
    print(isinstance(v, float), isinstance(v, Real))
```

```output for=float-check
False True
True True
False True
```

### Instantiating an abstract class

The classes of the tower are abstract (rule r1). `numbers.Real()` raises `TypeError`, because `Real` has abstract methods.

```python fails id=no-instances expect=exception:TypeError
import numbers

print(numbers.Real())
```

### `bool` is `Integral`

`True` passes an `isinstance(x, Integral)` test (see `tower-membership`). Exclude `bool` explicitly when a function must reject it.

## Comparison

Go has no hierarchy of number types. For a binary operator other than a shift or a comparison, "the operand types must be identical" unless an untyped constant is involved.[^4] So an `int` and a `float64` cannot be added without a conversion. Python's tower and the `NotImplemented` protocol (rule r7) let operands of different number types mix.
