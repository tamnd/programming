---
tag: F1A50N
kind: concept
slug: subnormal-numbers
name: Subnormal numbers
aliases: [subnormal number, denormal, denormalized number, denormal number, gradual underflow, flush to zero, FTZ, DAZ]
area: foundations/numbers-in-machines
level: advanced
summary: >-
  Subnormal numbers are the floats between zero and the smallest normal float. They make underflow gradual, so x - y is never 0 when x and y differ, but they have less precision and can be slow.
applies_to: core-languages
viz: optional
formal: none
see_also: [floating-point-arithmetic, machine-epsilon, nan-and-infinity, floating-point-rounding-error, overflow-semantics, binary-number]
same_as:
  wikidata: Q1070094
  wikipedia: Subnormal_number
sources:
  - url: https://en.wikipedia.org/wiki/Subnormal_number
    title: Subnormal number, Wikipedia
    accessed: 2026-11-16
    locator: the lead, Background, Performance issues, and Disabling subnormal floats at the code level
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Double.html
    title: Double (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: the fields MIN_NORMAL and MIN_VALUE
  - url: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
    title: David Goldberg, What Every Computer Scientist Should Know About Floating-Point Arithmetic, ACM Computing Surveys 23(1), March 1991 (edited reprint by Sun Microsystems)
    accessed: 2026-11-16
    locator: the section Denormalized Numbers, Figure D-2, and footnote 19
  - url: https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html
    title: GCC Manual, x86 Options, Free Software Foundation
    accessed: 2026-11-16
    locator: -mdaz-ftz
  - url: https://docs.python.org/3/library/sys.html
    title: sys, System-specific parameters and functions, Python Software Foundation
    accessed: 2026-11-16
    locator: sys.float_info, the attribute min
  - url: https://docs.python.org/3/library/math.html
    title: math, Mathematical functions, Python Software Foundation
    accessed: 2026-11-16
    locator: math.isnormal, math.issubnormal, and math.ulp
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: Math.getExponent(double)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MIN_VALUE
    title: Number.MIN_VALUE, MDN Web Docs
    accessed: 2026-11-16
    locator: Value, and Description
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: Primitive Type f64, The Rust Project
    accessed: 2026-11-16
    locator: the constant MIN_POSITIVE, and the method is_subnormal
  - url: https://pkg.go.dev/math
    title: math package, The Go Authors
    accessed: 2026-11-16
    locator: the constants MaxFloat64 and SmallestNonzeroFloat64
  - url: https://en.cppreference.com/c/types/limits
    title: Numeric limits (C), cppreference.com
    accessed: 2026-11-16
    locator: DBL_MIN, and DBL_TRUE_MIN
  - url: https://en.cppreference.com/c/numeric/math/fpclassify
    title: fpclassify (C), cppreference.com
    accessed: 2026-11-16
    locator: Return value
  - url: https://en.cppreference.com/cpp/types/numeric_limits/denorm_min
    title: std::numeric_limits<T>::denorm_min, cppreference.com
    accessed: 2026-11-16
    locator: the description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.double.epsilon
    title: Double.Epsilon Field, Microsoft Learn
    accessed: 2026-11-16
    locator: the field description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.double
    title: Double Struct, Microsoft Learn
    accessed: 2026-11-16
    locator: the method IsSubnormal
  - url: https://www.postgresql.org/docs/current/datatype-numeric.html
    title: PostgreSQL Documentation, 8.1. Numeric Types, The PostgreSQL Global Development Group
    accessed: 2026-11-16
    locator: 8.1.3 Floating-Point Types
---

## Idea

A normal float has no leading zeros in its significand. The exponent moves the point, so `0.0123` is kept as `1.23` times a power of ten.[^1] The exponent has a lower limit, so there is a smallest positive normal float. For a double it is `2^-1022`.[^2] Without more values, the next value below it is 0, and a result in that gap becomes 0. Goldberg: "If the result of a floating-point calculation falls into this gulf, it is flushed to zero."[^3]

Subnormal numbers fill the gap. They are the values with the smallest exponent and a leading digit of zero.[^1] The smallest positive double is then `2^-1074`, not `2^-1022`.[^2]

```python run id=idea
import math
import sys

tiny = sys.float_info.min
print("smallest normal:   ", tiny)
print("smallest subnormal:", math.ulp(0.0))
half = tiny / 2
print("tiny / 2:", half, "subnormal:", 0 < half < tiny)

x = 1.25 * tiny
y = tiny
print("x == y:", x == y)
print("x - y:", x - y, "is zero:", x - y == 0)

print("ulp(1e-300) / 1e-300:", math.ulp(1e-300) / 1e-300)
print("ulp(1e-320) / 1e-320:", math.ulp(1e-320) / 1e-320)
print("smallest subnormal / 2:", math.ulp(0.0) / 2)
```

```output
smallest normal:    2.2250738585072014e-308
smallest subnormal: 5e-324
tiny / 2: 1.1125369292536007e-308 subnormal: True
x == y: False
x - y: 5.562684646268003e-309 is zero: False
ulp(1e-300) / 1e-300: 1.6578092116916188e-16
ulp(1e-320) / 1e-320: 0.0004940711462450593
smallest subnormal / 2: 0.0
```

The output shows three facts.

- `x` and `y` differ, and `x - y` is not zero. The difference is a subnormal.
- Near `1e-300` the gap between neighbours is about `1e-16` of the value. Near `1e-320` it is about `5e-4` of the value, so a subnormal keeps far fewer digits.
- Below the smallest subnormal there is nothing but zero. Half of it rounds to 0.

## How it works

### The encoding

"In binary interchange formats, subnormal numbers are encoded with a biased exponent of 0, but are interpreted with the value of the smallest allowed exponent, which is one greater".[^1] A normal binary float has an implicit leading 1 that is not stored. A subnormal has a leading 0 in its place.[^1] So a subnormal double is `0.f` times `2^-1022`, where `f` is the stored fraction.

The subnormal range for binary32 and binary64 is: from `2^-149` to `2^-126 - 2^-149` for binary32, and from `2^-1074` to `2^-1022 - 2^-1074` for binary64.[^1] The values in this range are evenly spaced. Each step is the smallest subnormal, `2^-1074` for a double.

### Gradual underflow

Goldberg gives an example with three decimal digits and a smallest exponent of -98. The numbers `x = 6.87 × 10^-97` and `y = 6.81 × 10^-97` differ, but `x - y` is too small for a normal number and "must be flushed to zero".[^3] So the property `x = y` if and only if `x - y = 0` fails. Goldberg warns that code such as `if (x != y) then z = 1/(x-y)` can then fail "due to a spurious division by zero".[^3]

With subnormals, `x - y` is the denormalized number `0.6 × 10^-98`, not zero. "This behavior is called gradual underflow."[^3] The Wikipedia article gives the same guarantee for IEEE 754: "two nearby floating-point numbers always have a representable non-zero difference."[^1]

Underflow is still possible. A result below half the smallest subnormal rounds to zero, and a subnormal result has fewer significant bits. The precision goes down step by step: "significant digits are lost, but not as abruptly as when using the flush to zero on underflow approach".[^1]

### Speed, FTZ and DAZ

Some systems handle subnormal values in hardware, "in the same way as normal values". Others handle them in software, which is slower.[^1] On "many modern x86 processors", "instructions involving subnormal operands may take as many as 100 additional clock cycles, causing the fastest instructions to run as much as six times slower".[^1] Goldberg notes that "Programs that frequently underflow often run noticeably slower on hardware that uses software traps".[^3]

So x86 processors with SSE have two mode flags.[^1]

- **FTZ (flush to zero)** returns zero "instead of a subnormal float for operations that would result in a subnormal float".[^1]
- **DAZ (denormals are zero)** treats "subnormal input arguments to floating-point operations as zero".[^1]

These modes break IEEE 754: with them, `x - y` can again be 0 when `x != y`. A compiler option can turn them on for the whole program. In GCC for x86, "The -Ofast, -ffast-math, or -funsafe-math-optimizations options normally link in startup code that sets these flags except when building a shared library".[^4] Intel's C and Fortran compilers set DAZ and FTZ by default above `-O0`.[^1]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `sys.float_info.min`, `math.ulp(0.0)`, `math.issubnormal` (3.15) |
| JavaScript | native | `Number.MIN_VALUE` |
| TypeScript | native | as JavaScript |
| Java | native | `Double.MIN_NORMAL`, `Double.MIN_VALUE`, `Math.getExponent` |
| C | native | `DBL_MIN`, `DBL_TRUE_MIN`, `fpclassify` and `FP_SUBNORMAL` |
| C++ | native | `std::numeric_limits<double>::min()`, `denorm_min()`, `std::fpclassify` |
| C# | native | `double.Epsilon`, `double.IsSubnormal` |
| Go | native | `math.SmallestNonzeroFloat64` |
| Rust | native | `f64::MIN_POSITIVE`, `is_subnormal` |
| Kotlin | native | `Double.MIN_VALUE` (the JVM double) |
| PowerShell | native | `[double]::Epsilon`, `[double]::IsSubnormal` |
| Bash | absent | integer arithmetic only |
| SQL | native | PostgreSQL `double precision`, with an underflow error for values too close to zero |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | absent | no constant or test for subnormal values |

The values are the same in every language with IEEE 754 doubles. The names differ, and some names are traps.

- **Python** `sys.float_info.min` is "The minimum representable positive normalized float". The docs say to use `math.ulp(0.0)` "to get the smallest positive denormalized representable float".[^5] Python 3.15 adds `math.issubnormal`, which is true for "a finite nonzero number with a magnitude smaller than sys.float_info.min".[^6]
- **Java** has two constants: `MIN_NORMAL` is the smallest positive normal value, `2^-1022`, and `MIN_VALUE` is the smallest positive nonzero value, `2^-1074`.[^2] `Math.getExponent` gives `Double.MIN_EXPONENT - 1` when "the argument is zero or subnormal".[^7]
- **JavaScript** `Number.MIN_VALUE` is the smallest positive subnormal in the main engines. The specification allows a larger value: "implementations are not required to support this representation, in which case Number.MIN_VALUE may be larger."[^8]
- **Rust** `f64::MIN_POSITIVE` is the "Smallest positive normal f64 value", and `is_subnormal` tests a value.[^9]
- **Go** defines `SmallestNonzeroFloat64` as `0x1p-1022 * 0x1p-52`, that is, the smallest subnormal.[^10]
- **C** has `DBL_MIN`, the "minimum normalized positive value", and `DBL_TRUE_MIN`, the "minimum positive value".[^11] `fpclassify` returns `FP_SUBNORMAL` for a subnormal argument.[^12] C++ `denorm_min` "Returns the minimum positive subnormal value of the type".[^13]
- **C#** `Double.Epsilon` is not a machine epsilon. It "Represents the smallest positive Double value that is greater than zero", that is, the smallest subnormal ([[machine-epsilon]]).[^14] `double.IsSubnormal` "Determines whether the specified value is subnormal."[^15]
- **SQL** in PostgreSQL: "Numbers too close to zero that are not representable as distinct from zero will cause an underflow error."[^16]

## Variations and related ideas

- **Denormal and subnormal.** Older documents, such as the first release of IEEE 754 in 1985, use the word denormal. The 2008 revision uses subnormal, and "the terms subnormal and denormal are often used interchangeably".[^1] Goldberg's footnote: "They are called subnormal in 854, denormal in 754."[^3]
- **Decimal formats.** IEEE 754-2008 has subnormal numbers in its decimal formats too, and there "they require no special encoding".[^1]
- **Machine epsilon.** Machine epsilon is the gap above 1. The smallest subnormal is the gap above 0. The two are different constants ([[machine-epsilon]]).
- **Overflow.** At the other end of the range, a result too large for the format becomes infinity ([[nan-and-infinity]]).

## Pitfalls

### Taking the "smallest" constant to be the smallest normal value

`Double.MIN_VALUE` in Java and `Number.MIN_VALUE` in JavaScript are subnormal, about `4.9e-324`. `sys.float_info.min` in Python, `DBL_MIN` in C and `f64::MIN_POSITIVE` in Rust are normal, about `2.2e-308`.[^2][^8][^5][^11][^9] Code that uses one where it means the other is off by a factor of `2^52`.

### A program that is fast in tests and slow on real data

A loop whose values decay toward zero, such as a filter or a damped simulation, can enter the subnormal range and run several times slower on some processors.[^1] In audio code, "subnormal values usually represent a signal so quiet that it is out of the human hearing range", so such code often avoids them.[^1]

### Fast-math flags that change the results

A build with `-ffast-math` or `-Ofast` can turn on FTZ and DAZ, so the same code gives other results than a normal build.[^4] Then a nonzero `x - y` can become 0. A test that divides by `x - y` after `x != y` is no longer safe.

### A timing side channel

The speed difference can leak data. Researchers "showed that it provides a timing side channel that allows a malicious web site to extract page content from another site inside a web browser."[^1]

## Cost

A subnormal number costs no extra memory: it uses the same bits as other floats. The cost is precision and, on some processors, time. A subnormal double has fewer significant bits than a normal double, and the smallest subnormal has only 1. Operations on subnormals can take up to 100 extra clock cycles on some x86 processors.[^1] FTZ and DAZ remove the time cost and give up the guarantee that `x - y` is nonzero for `x != y`.[^1]

## History

Subnormal numbers "were implemented in the Intel 8087 while the IEEE 754 standard was being written." They were "by far the most controversial feature" of the proposal that was adopted, and the 8087 showed that they "could be supported in a practical implementation".[^1] Goldberg writes that denormals "are the most controversial part of the standard and probably accounted for the long delay in getting 754 approved."[^3]

## References
