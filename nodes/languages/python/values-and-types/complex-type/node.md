---
tag: ZVW4PM
kind: feature
owner: python
slug: complex-type
name: The complex type
aliases: [Python complex numbers, imaginary literal, complex(), cmath, z.real and z.imag, conjugate]
area: languages/python/values-and-types
level: core
summary: >-
  complex holds a complex number as two floats, z.real and z.imag. A literal with a j suffix is imaginary. Complex numbers have no order, //, % or divmod, and the cmath module has their math functions.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex", locator: "Numeric Types - int, float, complex"}
see_also: [python/float-type, python/numeric-literals, python/the-numeric-tower, python/arithmetic-conversions]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types - int, float, complex
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#complex
    title: Built-in Functions, Python 3.15 documentation
    locator: complex()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#numbers-complex-complex
    title: The Python Language Reference, Python 3.15
    locator: 3.2.4.3. numbers.Complex (complex)
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/cmath.html
    title: cmath, Mathematical functions for complex numbers, Python 3.15 documentation
    locator: introduction; cmath.phase(), cmath.polar()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/math.html
    title: math, Mathematical functions, Python 3.15 documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10.1. Value comparisons
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Complex_numbers
    title: The Go Programming Language Specification
    locator: Numeric types; Manipulating complex numbers
    accessed: 2026-11-16
---

## Synopsis

```python run id=complex-basics
z = 3 + 4j
print(z.real, z.imag, abs(z), z.conjugate())
print(1j * 1j, complex(1, 2), complex("-1.23+4.5j"))
```

```output for=complex-basics
3.0 4.0 5.0 (3-4j)
(-1+0j) (1+2j) (-1.23+4.5j)
```

`3 + 4j` adds an integer and an imaginary number. The parts are floats, and `abs()` gives the magnitude.

## Semantics

**r1.** A complex number is "a pair of machine-level double precision floating-point numbers". The read-only attributes `z.real` and `z.imag` give the two parts.[^3] Example: [`complex-basics`](#complex-basics).

**r2.** A numeric literal with the suffix `j` or `J` gives an imaginary number, a complex number with a zero real part. Add an integer or a float to it to get a complex number with both parts.[^1]

**r3.** `complex()` converts a string or a number, or makes a complex number from a real and an imaginary part. A string may have whitespace and parentheses around it, but no whitespace between the sign, the number and the `j` suffix: `complex('1+2j')` is valid and `complex('1 + 2j')` raises `ValueError`.[^2] Example: [`complex-string`](#complex-string).

**r4.** Floor division, `%` and `divmod()` are "Not for complex numbers". Built-in Types says to convert to floats with `abs()` if appropriate.[^1] Complex numbers "do not support order comparison".[^6] Example: [`no-floor-division`](#no-floor-division).

**r5.** The functions of the `math` module "cannot be used with complex numbers". The `cmath` module has functions of the same names for complex numbers.[^5] They accept integers, floats and complex numbers.[^4] Example: [`math-and-cmath`](#math-and-cmath).

**r6.** `cmath.phase(z)` returns the argument of `z` as a float in the range from -π to π, and `cmath.polar(z)` returns the pair `(abs(z), phase(z))`. The module has no function for the modulus, because the built-in `abs()` computes it.[^4] Example: [`math-and-cmath`](#math-and-cmath).

## Examples

`math.sqrt()` rejects a negative number. `cmath.sqrt()` returns a complex result (rules r5 and r6).

```python run id=math-and-cmath
import cmath
import math

print(cmath.sqrt(-1), cmath.polar(1j))
try:
    math.sqrt(-1)
except ValueError:
    print("math.sqrt(-1): ValueError")
```

```output for=math-and-cmath
1j (1.0, 1.5707963267948966)
math.sqrt(-1): ValueError
```

`//` fails for a complex number. Use the real part or the magnitude, which are floats (rule r4).

```python run id=no-floor-division
z = 7 + 0j
try:
    z // 2
except TypeError:
    print("z // 2: TypeError")
print(z.real // 2, abs(z) // 2)
```

```output for=no-floor-division
z // 2: TypeError
3.0 3.0
```

## Pitfalls

### Spaces in a complex string

Code can write `1 + 2j` with spaces, but `complex()` rejects a string with spaces around the sign (rule r3).

```python wrong id=complex-string
for s in ["1+2j", "(1+2j)", "1 + 2j"]:
    try:
        print(complex(s))
    except ValueError:
        print("ValueError for", repr(s))
```

```output for=complex-string
(1+2j)
(1+2j)
ValueError for '1 + 2j'
```

### Sorting complex numbers

`<` is not defined for complex numbers (rule r4), so `sorted()` of complex values fails. Sort with a key such as `abs`.

```python fails id=no-order expect=exception:TypeError
print(sorted([2j, 1 + 1j]))
```

## Comparison

Go has two complex types, `complex64` and `complex128`, with `float32` or `float64` parts. The built-in function `complex` builds a value from a real and an imaginary part, and `real` and `imag` extract the parts.[^7] Python has one `complex` type with two float parts, and gets the parts with the attributes `z.real` and `z.imag` (rule r1).
