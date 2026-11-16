---
tag: ACHQHP
kind: task
slug: compare-floating-point-numbers
name: Compare floating-point numbers
aliases: [compare floats, float equality, approximately equal, isclose, almost equal, epsilon comparison, ULP distance, NaN comparison]
area: foundations/numbers-in-machines
level: core
summary: >-
  Tell whether two floats are close, with a relative and an absolute tolerance, and not with ==. Count the floats between them with the ULP distance, and treat NaN apart.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://docs.python.org/3/tutorial/floatingpoint.html
    title: Floating-Point Arithmetic, Issues and Limitations, Python 3 tutorial
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/math.html
    title: math, Mathematical functions, Python 3 documentation
    locator: math.isclose, math.nextafter, math.ulp
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0485/
    title: PEP 485, A Function for testing approximate equality
    locator: Handling of non-finite numbers; Behavior near zero; Symmetry
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/EPSILON
    title: Number.EPSILON, MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: ulp(double), nextUp(double)
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/types/limits
    title: Numeric limits, C reference, cppreference.com
    locator: Limits of floating-point types
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/numeric/math/nextafter
    title: nextafter, nexttoward, C reference, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.double.epsilon
    title: Double.Epsilon Field, .NET API reference, Microsoft Learn
    locator: the definition; Remarks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math
    title: math, Go standard library
    locator: func Float64bits, func Nextafter
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: f64, Rust standard library
    locator: EPSILON, to_bits
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual, version 5.3
    locator: 6.5 Shell Arithmetic
    accessed: 2026-11-16
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/awk.html
    title: awk, The Open Group Base Specifications Issue 8
    locator: Expressions in awk
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/NaN
    title: NaN, MDN Web Docs
    locator: Description
    accessed: 2026-11-16
---

## Task

Tell whether two floats `a` and `b` are close enough to be the same value.

A float is often not exact. The Python tutorial says that "summing three values of 0.1 may not yield exactly 0.3, either"[^1], so `0.1 + 0.2 == 0.3` is false in each language that uses binary floats. Thus a test with `==` is almost never the correct test for computed floats. Use a tolerance.

The function `is_close(a, b, rel_tol, abs_tol)` returns true when

```text
a == b  or  |a - b| <= max(rel_tol * max(|a|, |b|), abs_tol)
```

This is the rule of `math.isclose` in Python[^2]. It has two tolerances:

- **The relative tolerance** `rel_tol` scales with the size of the values. With `rel_tol = 1e-9`, the values must agree to about 9 decimal digits, both for 1.0 and for 1e10.
- **The absolute tolerance** `abs_tol` is a fixed distance. It is necessary near zero, because "no value is small relative to zero"[^3].

The test uses the larger of `|a|` and `|b|`, so `is_close(a, b)` is always the same as `is_close(b, a)`.

The special values follow rules of their own: NaN is close to no value, NaN included, and an infinity is close only to itself. JSON cannot hold these values, so the cases of the contract have finite values only, and the native tests check NaN and the infinities.

## Solutions

| Language | Tolerance test | Machine epsilon and ULP |
|---|---|---|
| Python | `math.isclose(a, b, rel_tol=..., abs_tol=...)` | `math.ulp(x)`, `math.nextafter(x, y)`, `sys.float_info.epsilon` |
| JavaScript, TypeScript | own function | `Number.EPSILON` |
| Java | own function | `Math.ulp(x)`, `Math.nextUp(x)` |
| C, C++ | own function | `DBL_EPSILON`, `nextafter(x, y)` |
| C# | own function | `double.Epsilon` is not the machine epsilon |
| Go | own function | `math.Nextafter(x, y)`, `math.Float64bits(x)` |
| Rust | own function | `f64::EPSILON`, `x.to_bits()` |
| Bash | `awk` | none |

- **Python.** `math.isclose` "Return True if the values a and b are close to each other and False otherwise"[^2]. Its result is `abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)`[^2], and the defaults are `rel_tol=1e-09` and `abs_tol=0.0`[^2]. The tutorial says that "the math.isclose() function can be useful for comparing inexact values"[^1]. The design is in PEP 485: "NaN is not considered close to any other value, including NaN"[^3], and "inf and -inf are only considered close to themselves"[^3].
- **JavaScript.** There is no built-in function. The solution tests `a === b` first, which is true for two equal infinities. Then `Number.isFinite` sends NaN and the infinities to false. `Number.EPSILON` is "the difference between 1 and the smallest floating point number greater than 1"[^4]; it is a good tolerance only for values near 1.
- **TypeScript.** The same code as JavaScript.
- **Java.** The same function, with `Double.isFinite`. `Math.ulp` "Returns the size of an ulp of the argument"[^5], and `Math.nextUp` "Returns the floating-point value adjacent to d in the direction of positive infinity"[^5].
- **C, C++.** The same function, with `isfinite`, `fabs` and `fmax`. `DBL_EPSILON` is the "difference between 1.0 and the next representable value"[^6] for `double`. `nextafter(from, to)` "returns the next representable value of from in the direction of to"[^7].
- **C#.** The same function, with `double.IsFinite`. Do not use `double.Epsilon` as a tolerance: it "Represents the smallest positive Double value that is greater than zero"[^8].
- **Go.** The same function, with `math.IsInf` and `math.IsNaN`. "Nextafter returns the next representable float64 value after x towards y"[^9], and `math.Float64bits` "returns the IEEE 754 binary representation of f"[^9].
- **Rust.** The same function, with `f64::is_finite`. `f64::EPSILON` is the "Machine epsilon value for f64"[^10], and `to_bits` is a "Raw transmutation to u64"[^10].
- **Bash.** Shell arithmetic "is done in the largest fixed-width integers available"[^11], so there are no floats. The solution gives the numbers to `awk`, where "All arithmetic shall follow the semantics of floating-point arithmetic as specified by the ISO C standard"[^12].

The problem and the fix in Python:

```python run id=python-isclose
import math

x = 0.1 + 0.2
print(x, x == 0.3, math.isclose(x, 0.3))
print(math.isclose(1e-12, 0.0), math.isclose(1e-12, 0.0, abs_tol=1e-9))
print(math.ulp(1.0), math.ulp(1e16), math.nextafter(1.0, 2.0))
```

```output for=python-isclose
0.30000000000000004 False True
False True
2.220446049250313e-16 2.0 1.0000000000000002
```

The space between two floats grows with their size: `math.ulp(1e16)` is 2.0. So a fixed tolerance that is right near 1 is too small for large values, and too large for small values.

### The ULP distance

Another test counts the floats between `a` and `b`. This count is the distance in units in the last place (ULP). For two finite floats of the same sign, the bit patterns, read as integers, are in the same order as the floats. The distance is the difference of the integers. A negative float needs a mapping first, so that -0.0 and 0.0 give the same integer:

```go run id=go-ulp-distance
package main

import (
	"fmt"
	"math"
)

// ordered maps the bits of a finite float to an integer in the order of the floats.
func ordered(x float64) int64 {
	b := int64(math.Float64bits(x))
	if b < 0 {
		return -(b & math.MaxInt64)
	}
	return b
}

func ulpDistance(a, b float64) int64 {
	d := ordered(a) - ordered(b)
	if d < 0 {
		return -d
	}
	return d
}

func main() {
	x, y := 0.1, 0.2
	fmt.Println(ulpDistance(x+y, 0.3))
	fmt.Println(ulpDistance(1, math.Nextafter(1, 2)))
	fmt.Println(ulpDistance(math.Copysign(0, -1), 0))
	fmt.Println(ulpDistance(-math.SmallestNonzeroFloat64, math.SmallestNonzeroFloat64))
}
```

```output for=go-ulp-distance
1
1
0
2
```

A test such as "at most 4 ULP apart" works at each size, but not near zero: the two smallest floats of opposite signs are 2 ULP apart, while 1e-300 and 0.0 are far apart in ULP. So the ULP test also needs an absolute tolerance near zero.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Each native test also checks each case with `a` and `b` swapped.

- `zero-and-zero`: equal values are close, also with both tolerances 0.
- `one-ulp-apart`: with both tolerances 0, only equal values are close.
- `near-zero-relative`, `near-zero-absolute`: 1e-12 is not close to 0.0 with a relative tolerance only. It is close with `abs_tol = 1e-9`.
- `opposite-signs`, `opposite-signs-absolute`: 1e-10 and -1e-10 are close only with an absolute tolerance.
- `on-the-boundary`, `swapped-boundary`, `past-the-boundary`: with `rel_tol = 0.1`, 9.0 is close to 10.0 in both orders, and 8.875 is not.
- `negative-values`: the absolute values set the tolerance.
- NaN and the infinities, in the native tests only: `is_close(NaN, NaN)` is false, `is_close(inf, inf)` is true, and `is_close(inf, 1e308)` is false.

## Pitfalls

- **`==` on computed floats.** `0.1 + 0.2 == 0.3` is false (see the Python block). Compare computed floats with a tolerance. Use `==` only when the value is exact, for example a value that the program stored and reads back.

- **A fixed epsilon.** `Math.abs(a - b) < Number.EPSILON` works for values near 1, but not for large values. The sum `1000.1 + 1000.2` is a few ULP from `2000.3`, and the difference is larger than `Number.EPSILON`:

```javascript wrong id=javascript-fixed-epsilon
const closeFixed = (a, b) => Math.abs(a - b) < Number.EPSILON;
console.log(closeFixed(0.1 + 0.2, 0.3), closeFixed(1000.1 + 1000.2, 2000.3));
console.log(1000.1 + 1000.2, Math.abs(1000.1 + 1000.2 - 2000.3));
```

```output for=javascript-fixed-epsilon
true false
2000.3000000000002 2.2737367544323206e-13
```

- **`double.Epsilon` in C#.** The .NET documentation says that "The value of the Epsilon property is not equivalent to machine epsilon"[^8]. It is about 4.9e-324, so a test `Math.Abs(a - b) < double.Epsilon` is the same as `a == b`.

- **A relative tolerance near zero.** The Python manual says that `isclose(x, 0)` "is False for any nonzero x and rel_tol less than 1.0"[^2]. When one of the values can be 0, give an absolute tolerance too.

- **An asymmetric test.** A test such as `|a - b| <= rel_tol * |b|` uses only one of the values. PEP 485 calls it asymmetric: then "isclose(a,b) is not necessarily the same as isclose(b,a)"[^3]. With `rel_tol = 0.1`, the asymmetric test says that 10.0 is close to 9.0, and that 9.0 is not close to 10.0.

- **NaN.** "When NaN is one of the operands of any relational comparison"[^13], the result is false, and NaN is unequal to each value, also to NaN[^13]. So `x == x` is false when `x` is NaN, and a sort or a search with `<` gives wrong results when the data has a NaN. Test for NaN with `math.isnan`, `Number.isNaN`, `Double.isNaN` or `isnan`.

- **An infinity.** Without the first test `a == b`, the difference `inf - inf` is NaN, so two equal infinities are not close. Without the test of `isfinite`, `|inf - 1e308|` is infinite, and the relative tolerance `rel_tol * inf` is infinite too, so infinity is close to 1e308.

## Related tasks

- [[round-a-number]]: round a float before you print it.
- [[detect-division-by-zero]]: a float division by zero gives an infinity or NaN.
- [[do-integer-arithmetic]]: integers are exact, so `==` is correct for them.
- [[write-a-unit-test]]: test frameworks have their own float comparisons.

See also the concepts [[floating-point-arithmetic]], [[floating-point-rounding-error]], [[machine-epsilon]], [[nan-and-infinity]] and [[ieee-754]].

## References
