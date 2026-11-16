---
tag: 077TMB
kind: definition
slug: machine-epsilon
name: Machine epsilon and ULP
aliases: [machine epsilon, macheps, unit in the last place, ulp, unit of least precision, DBL_EPSILON, Number.EPSILON]
area: foundations/numbers-in-machines
level: core
summary: >-
  Machine epsilon is the gap between 1.0 and the next float, 2^-52 for a double. An ulp is the gap between neighbouring floats at any size. Both measure rounding error.
see_also: [floating-point-arithmetic, floating-point-rounding-error, integer-representation, binary-number]
same_as:
  wikidata: Q1780355
  wikipedia: Machine_epsilon
sources:
  - url: https://en.wikipedia.org/wiki/Machine_epsilon
    title: Machine epsilon, Wikipedia
    accessed: 2026-11-16
    locator: the lead, the table of values for standard hardware arithmetics, and Formal definition
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions, Python Software Foundation
    accessed: 2026-11-16
    locator: sys.float_info, the attribute epsilon
  - url: https://en.cppreference.com/c/types/limits
    title: Numeric limits (C), cppreference.com
    accessed: 2026-11-16
    locator: the limits of floating-point types, FLT_EPSILON, DBL_EPSILON and LDBL_EPSILON
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/EPSILON
    title: Number.EPSILON, MDN Web Docs
    accessed: 2026-11-16
    locator: the first paragraph
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: Primitive Type f64, The Rust Project
    accessed: 2026-11-16
    locator: the constant EPSILON
  - url: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
    title: David Goldberg, What Every Computer Scientist Should Know About Floating-Point Arithmetic, ACM Computing Surveys 23(1), March 1991 (edited reprint by Sun Microsystems)
    accessed: 2026-11-16
    locator: the section Relative Error and Ulps, and the section on optimizers in Languages and Compilers
  - url: https://en.wikipedia.org/wiki/Unit_in_the_last_place
    title: Unit in the last place, Wikipedia
    accessed: 2026-11-16
    locator: the lead, Definition, and Example 1
  - url: https://docs.python.org/3/library/math.html
    title: math, Mathematical functions, Python Software Foundation
    accessed: 2026-11-16
    locator: math.ulp and math.nextafter
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: the class description (ulps), and Math.ulp(double)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.double.epsilon
    title: Double.Epsilon Field, Microsoft Learn
    accessed: 2026-11-16
    locator: the summary, and Remarks
---

Machine epsilon is the gap between 1.0 and the next larger float of a format. For binary64, the `double` of most languages, it is 2^-52, about 2.22e-16.[^1] A unit in the last place (ulp) is the same kind of gap at any value: the distance between a float and its neighbour. Both numbers tell how much a single rounding can change a value, so they are the units in which programmers measure floating-point error.

## Definition

**Machine epsilon.** For a binary format with a precision of p bits (counting the hidden bit), machine epsilon is 2^(1-p). This is the mainstream definition, where machine epsilon "is defined simply as the difference between 1 and the next larger floating point number".[^1] It is the definition of the constants in the languages:

- Python `sys.float_info.epsilon`: the "difference between 1.0 and the least value greater than 1.0 that is representable as a float".[^2]
- C `DBL_EPSILON` and its siblings: the "difference between 1.0 and the next representable value".[^3]
- JavaScript `Number.EPSILON`: "the difference between 1 and the smallest floating point number greater than 1".[^4]
- Rust `f64::EPSILON`: "the difference between 1.0 and the next larger representable number".[^5]

**Rounding machine epsilon.** The formal definition is the largest relative error of one rounding. With round to nearest it is 2^-p, half the value above.[^1] Goldberg uses this form: when a real number is rounded to the nearest float, "the relative error is always bounded by e, which is referred to as machine epsilon".[^6] The LAPACK linear algebra package uses it too, so check which definition a formula assumes.[^1]

**Unit in the last place.** The ulp of x is "the spacing between two consecutive floating-point numbers".[^7] In a format with base b and precision p, if b^e <= |x| < b^(e+1), then ulp(x) = b^(max(e, emin) - p + 1), where emin is the smallest normal exponent.[^7] For binary64, ulp(1.0) is machine epsilon, ulp(2.0) is twice that, and the ulp doubles at each power of two. Python's `math.ulp(x)` returns "the value of the least significant bit of the float x".[^8] Java measures the accuracy of its `Math` methods in ulps.[^9]

```python run id=definition
import math
import sys

eps = sys.float_info.epsilon
print("epsilon:", eps, "== 2**-52:", "yes" if eps == 2.0 ** -52 else "no")
print("next after 1.0:", math.nextafter(1.0, 2.0))
print("1.0 + eps/2 == 1.0:", "yes" if 1.0 + eps / 2 == 1.0 else "no")
for x in [1.0, 2.0, 1000.0, 1e16, 2.0 ** 53]:
    print(f"ulp({x:g}) = {math.ulp(x):g}")
print("ulp(0.0) =", math.ulp(0.0))
```

```output
epsilon: 2.220446049250313e-16 == 2**-52: yes
next after 1.0: 1.0000000000000002
1.0 + eps/2 == 1.0: yes
ulp(1) = 2.22045e-16
ulp(2) = 4.44089e-16
ulp(1000) = 1.13687e-13
ulp(1e+16) = 2
ulp(9.0072e+15) = 2
ulp(0.0) = 5e-324
```

`1.0 + eps/2` is a tie between 1.0 and the next float, and round to nearest even gives 1.0. Above 2^53 the ulp is 2, so not every integer has a double. At zero the ulp is the smallest subnormal number.

## Examples and non-examples

| Format | Precision p | Machine epsilon 2^(1-p) | Rounding epsilon 2^-p |
|---|---|---|---|
| binary16 | 11 | 2^-10, about 9.77e-04 | 2^-11 |
| binary32 (`float`) | 24 | 2^-23, about 1.19e-07 | 2^-24, about 5.96e-08 |
| binary64 (`double`) | 53 | 2^-52, about 2.22e-16 | 2^-53, about 1.11e-16 |
| x87 extended | 64 | 2^-63, about 1.08e-19 | 2^-64, about 5.42e-20 |
| binary128 | 113 | 2^-112 | 2^-113, about 9.63e-35 |

The values come from the table in the Machine epsilon article.[^1]

- **Non-example: C# `Double.Epsilon`.** The name is a trap. The field "Represents the smallest positive Double value that is greater than zero", about 4.94e-324.[^10] Microsoft adds that it "is not equivalent to machine epsilon".[^10]
- **Non-example: a tolerance for all values.** `Math.abs(a - b) < Number.EPSILON` is a test for values near 1. Near 1e16 the ulp is 2, so the test is the same as `a == b`. Near 1e-20 it accepts values that differ by many orders of magnitude.
- **Non-example: the loop that halves eps.** Goldberg shows the loop `eps = 1; do eps = 0.5*eps; while (eps + 1 > 1);`. An optimizing compiler that rewrites `eps + 1 > 1` as `eps > 0` changes it completely, and it computes a different number.[^6] The library constant is safer.
- **Example: the ulp near 2^24 in binary32.** The first integer that a `float` cannot hold is 2^24 + 1, so a loop that adds 1 to a `float` stops at 16777216.[^7]

## Related definitions

- **Relative error.** The error divided by the true value. One rounding to nearest has a relative error of at most the rounding epsilon.[^6]
- **Error in ulps.** The error divided by the ulp of the result. A correctly rounded operation has an error of at most half an ulp.[^6]
- **Floating-point rounding error.** How the errors of single operations add up and when a subtraction exposes them: [[floating-point-rounding-error]].
- **The format.** The sign, exponent and fraction fields that fix the precision p: [[floating-point-arithmetic]].

## References
