---
tag: A0WHSC
kind: feature
owner: python
slug: round-half-to-even
name: round() rounds half to even
aliases: [banker's rounding, round half to even, round(2.5) is 2, round(2.675, 2), ROUND_HALF_UP, ROUND_HALF_EVEN, rounding money]
area: languages/python/pitfalls
level: core
summary: >-
  round(2.5) is 2, because round() sends a tie to the even neighbor. round(2.675, 2) is 2.67, because the float is a little below 2.675. For decimal rules such as half up, use Decimal.quantize.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/functions.html#round", locator: "round()"}
proposals: []
see_also: [python/built-in-functions, python/float-type, python/floating-point-arithmetic, python/representation-error, python/decimal-floating-point-arithmetic, python/integer-division-and-modulo, python/fancier-output-formatting]
sources:
  - url: https://docs.python.org/3.15/library/functions.html#round
    title: Built-in Functions, Python 3.15 documentation
    locator: round()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/decimal.html#rounding-modes
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python 3.15 documentation
    locator: Rounding modes; Decimal objects; quantize()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.0.html#builtins
    title: What’s New In Python 3.0
    locator: Builtins
    accessed: 2026-11-18
---

## Synopsis

```python run id=ties
print(round(0.5), round(1.5), round(2.5), round(-0.5))
print(round(2.675, 2))
print(f"{2.5:.0f}")
```

```output for=ties
0 2 2 0
2.67
2
```

A value halfway between two integers goes to the even one. The value `2.675` is not a tie at all, because the float that the literal makes is a little smaller.

## Semantics

**r1.** The rule for `round`: "values are rounded to the closest multiple of 10 to the power minus ndigits; if two multiples are equally close, rounding is done toward the even choice (so, for example, both round(0.5) and round(-0.5) are 0, and round(1.5) is 2)."[^1] Example: [`ties`](#ties).

**r2.** Half to even does not move a sum of many rounded ties up or down, because half of the ties go up and half go down. Half up moves each tie the same way. Example: [`sum-of-ties`](#sum-of-ties).

**r3.** Most decimal ties are not ties as floats. "The behavior of round() for floats can be surprising: for example, round(2.675, 2) gives 2.67 instead of the expected 2.68. This is not a bug: it’s a result of the fact that most decimal fractions can’t be represented exactly as a float."[^1] A `Decimal` made from a float shows the exact value, because "Construction from an integer or a float performs an exact conversion of the value of that integer or float."[^2] Example: [`exact-value`](#exact-value).

**r4.** For a decimal rule, use the `decimal` module. "The quantize() method rounds a number to a fixed exponent. This method is useful for monetary applications".[^2] The modes include `ROUND_HALF_EVEN`, "Round to nearest with ties going to nearest even integer.", and `ROUND_HALF_UP`, "Round to nearest with ties going away from zero."[^2] Example: [`half-up`](#half-up).

**r5.** Make the `Decimal` from a string, such as `Decimal("2.675")`, not from a float. A `Decimal` made from the float `2.675` has the exact binary value, so it is below the tie (rule r3). Example: [`quantize-from-float`](#quantize-from-float).

**r6.** The default context of `decimal` also rounds half to even. Example: [`half-up`](#half-up).

## Examples

```python run id=exact-value
from decimal import Decimal

print(Decimal(2.675))
print(Decimal(2.5))
```

```output for=exact-value
2.67499999999999982236431605997495353221893310546875
2.5
```

The float for `2.675` is a little below the tie, so `round` goes down (rule r3). The float `2.5` is exact, so it is a real tie, and rule r1 applies.

```python run id=half-up
from decimal import Decimal, ROUND_HALF_UP, getcontext

cent = Decimal("0.01")
print(Decimal("2.675").quantize(cent, rounding=ROUND_HALF_UP))
print(Decimal("2.665").quantize(cent, rounding=ROUND_HALF_UP))
print(Decimal("-2.5").quantize(Decimal("1"), rounding=ROUND_HALF_UP))
print(Decimal("2.665").quantize(cent))
print(getcontext().rounding)
```

```output for=half-up
2.68
2.67
-3
2.66
ROUND_HALF_EVEN
```

`ROUND_HALF_UP` sends a tie away from zero, also for a negative value (rule r4). With no `rounding` argument, `quantize` uses the context, which is half to even (rule r6).

```python run id=sum-of-ties
from decimal import Decimal, ROUND_HALF_UP

ties = [0.5, 1.5, 2.5, 3.5]
print(sum(ties), sum(round(t) for t in ties))

one = Decimal("1")
up = [Decimal(str(t)).quantize(one, rounding=ROUND_HALF_UP) for t in ties]
print(sum(up))
```

```output for=sum-of-ties
8.0 8
10
```

Half to even keeps the sum at 8. Half up gives 10 (rule r2).

## Versions

> [!since v="3.0"]
> "Exact halfway cases are now rounded to the nearest even result instead of away from zero. (For example, round(2.5) now returns 2 rather than 3.)"[^3]

## Pitfalls

```python wrong id=quantize-from-float
from decimal import Decimal, ROUND_HALF_UP

price = 2.675
print(Decimal(price).quantize(Decimal("0.01"), rounding=ROUND_HALF_UP))
print(Decimal(str(price)).quantize(Decimal("0.01"), rounding=ROUND_HALF_UP))
```

```output for=quantize-from-float
2.67
2.68
```

The first `Decimal` has the exact value of the float, which is below the tie, so even `ROUND_HALF_UP` goes down (rule r5). `str(price)` gives the short text `2.675`, and the `Decimal` from it is a real tie.

```python wrong id=add-a-half
import math

for x in [2.5, -2.5]:
    print(x, math.floor(x + 0.5), int(x + 0.5))
```

```output for=add-a-half
2.5 3 3
-2.5 -2 -2
```

A hand-made rule such as `floor(x + 0.5)` sends a negative tie toward zero, so `-2.5` becomes `-2`, not `-3`. Use `quantize` with a named mode (rule r4).

## Comparison

The rule often taught in school sends a tie away from zero, so 2.5 becomes 3. Python’s `round` sends a tie to the even neighbor (rule r1), and the `decimal` module gives a choice of mode (rule r4). For the binary values of floats, see [[python/representation-error]]. For decimal numbers, see [[python/decimal-floating-point-arithmetic]].
