---
tag: NEW-floating-point-arithmetic
kind: concept
slug: floating-point-arithmetic
name: Floating-point arithmetic
aliases: [floating point, float, double, IEEE 754, binary64, rounding error, NaN]
area: foundations/numbers-in-machines
level: core
summary: >-
  A floating-point number is a binary fraction times a power of two. It covers a huge range with a fixed number of bits, but most decimal fractions, such as 0.1, have no exact form.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1985
  people: []
  note: IEEE 754-1985 fixed the binary formats and required that addition, subtraction, multiplication, division and square root be correctly rounded.
see_also: [integer-overflow, type-conversion, parse-an-integer, generate-random-numbers, undefined-behavior, measure-elapsed-time]
same_as:
  wikidata: Q117879
  wikipedia: Floating-point_arithmetic
sources:
  - url: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
    title: David Goldberg, What Every Computer Scientist Should Know About Floating-Point Arithmetic, ACM Computing Surveys 23(1), March 1991 (edited reprint by Sun Microsystems)
    locator: the sections Relative Error and Ulps, Cancellation, Exactly Rounded Operations, The IEEE Standard (Formats and Operations, Special Quantities, NaNs, Signed Zero), and Theorem 8 (Kahan Summation Formula) with the paragraph on optimizers after it
  - url: https://docs.python.org/3/tutorial/floatingpoint.html
    title: The Python Tutorial, 15. Floating-Point Arithmetic, Issues and Limitations
    locator: the opening section, and 15.1 Representation Error
  - url: https://tc39.es/ecma262/
    title: ECMAScript Language Specification (ECMA-262)
    locator: 6.1.6.1 The Number Type; 21.1.2.6 Number.MAX_SAFE_INTEGER
  - url: https://openjdk.org/jeps/306
    title: JEP 306, Restore Always-Strict Floating-Point Semantics (delivered in JDK 17)
    locator: Summary and Motivation
  - url: https://doc.rust-lang.org/std/primitive.f64.html#method.total_cmp
    title: Rust standard library, primitive type f64
    locator: method total_cmp
  - url: https://go.dev/ref/spec#Constant_expressions
    title: The Go Programming Language Specification
    locator: Constant expressions
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/floating-point-numeric-types
    title: C# language reference, Floating-point numeric types
    locator: Characteristics of the floating-point types (the table and the notes on decimal)
  - url: https://docs.python.org/3/whatsnew/3.12.html
    title: What's New In Python 3.12
    locator: Other Language Changes (sum uses Neumaier summation)
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899 working draft N3220 (C23)
    locator: 5.2.5.3.3 Characteristics of floating types <float.h>; 6.10.10.4 Conditional feature macros; Annex F, ISO/IEC 60559 floating-point arithmetic
  - url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
    title: GCC manual, Options That Control Optimization
    locator: -ffast-math and -fassociative-math
  - url: https://docs.python.org/3/library/math.html
    title: Python standard library, math
    locator: math.fsum, math.isclose, math.ulp
---

## Idea

A computer stores a number in a fixed number of bits. An integer type stores each whole number in its range exactly. Real numbers are a different problem. There are infinitely many of them in any interval, so a fixed number of bits can hold only a few. Goldberg puts it this way: "Squeezing infinitely many real numbers into a finite number of bits requires an approximate representation."[^1]

A floating-point number is a fraction with a fixed number of binary digits, times a power of two. The power of two lets the point "float": the same 64 bits can hold the distance between two atoms or between two galaxies, each with about 16 significant decimal digits. The cost is that most numbers have no exact form. The result of each operation is rounded to the nearest number that does.

The best-known example is `0.1 + 0.2`.

```python run id=idea
print(0.1 + 0.2)
print(0.1 + 0.2 == 0.3)
```

```output
0.30000000000000004
False
```

Nothing is broken here. The decimal fraction 0.1 has no exact binary form, as 1/3 has no exact decimal form. The program stores the nearest binary fraction, which is a little more than 0.1. The same is true of 0.2 and 0.3, and the rounding errors do not cancel.[^2]

## How it works

### The format

IEEE 754 binary64, the `double` of C, Java and C# and the `float` of Python, has three fields in 64 bits.

| Field | Bits | Meaning |
|---|---|---|
| sign | 1 | 0 for positive, 1 for negative |
| exponent | 11 | the power of two, stored with a bias of 1023 |
| fraction | 52 | the digits after the binary point |

A normal number has the value `(-1)^sign * 1.fraction * 2^(exponent - 1023)`. The leading `1.` is not stored, so the significand has 53 bits of precision.[^1] Binary32, the `float` of C and Java, has 8 exponent bits and a 24-bit significand.

This program takes some doubles apart.

```python run id=fields
import struct

for x in [1.0, 0.1, -2.5, 0.0, -0.0]:
    bits = struct.unpack(">Q", struct.pack(">d", x))[0]
    sign = bits >> 63
    exponent = (bits >> 52) & 0x7FF
    fraction = bits & ((1 << 52) - 1)
    print(f"{x:>5}: sign {sign}, exponent field {exponent:4}, fraction {fraction:013x}")
```

```output
  1.0: sign 0, exponent field 1023, fraction 0000000000000
  0.1: sign 0, exponent field 1019, fraction 999999999999a
 -2.5: sign 1, exponent field 1024, fraction 4000000000000
  0.0: sign 0, exponent field    0, fraction 0000000000000
 -0.0: sign 1, exponent field    0, fraction 0000000000000
```

For 1.0 the exponent field is 1023, which means 2^0. For 0.1 it is 1019, which means 2^-4, and 0.1 is 1.6 times 2^-4. An exponent field of 0 marks zero and the subnormal numbers, and the two zeros differ only in the sign bit.

The fraction of 0.1 is the repeating binary pattern `1001 1001 ...`, cut off after 52 bits and rounded up in the last digit. That is why the stored value is a little more than 0.1. Its exact value is 0.1000000000000000055511151231257827021181583404541015625.[^2]

### Rounding and the ulp

The gap between a double and the next larger double is one ulp, a "unit in the last place".[^1] The gap is not fixed. It doubles with each power of two. Near 1.0 it is 2^-52, about 2.2e-16. Near 1e16 it is 2.0, so above 2^53 not every integer has a double. This is why JavaScript defines `Number.MAX_SAFE_INTEGER` as 2^53 - 1.[^3]

IEEE 754 requires that addition, subtraction, multiplication and division "be exactly rounded".[^1] The result is the exact result, rounded to a neighbouring double by the current rounding rule. The default rule is round to nearest, with ties to the value whose last bit is even. One operation therefore has an error of at most half an ulp. A sequence of operations can have a much larger error, because each step rounds again.

### Special values

IEEE 754 adds values that are not numbers in the usual sense.[^1]

- **Infinity.** A result too large for the format becomes infinity, with a sign. A nonzero number divided by zero also gives an infinity.
- **NaN.** "Not a number" is the result of an operation with no defined value, such as `0/0` or the square root of a negative number. Every operation with a NaN as input gives NaN, so the NaN reaches the end of the computation. A NaN is not equal to anything, not even to itself.
- **Signed zero.** There are two zeros, `+0` and `-0`. They compare equal, but `1/+0` is `+inf` and `1/-0` is `-inf`.
- **Subnormal numbers.** Below the smallest normal number, the leading bit is 0 instead of 1. These numbers fill the gap between the smallest normal number and zero. Goldberg shows that they guarantee that `x - y` is zero only when `x == y`.[^1]

The C example prints each of these.

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `float` is binary64; `decimal.Decimal` and `fractions.Fraction` for exact work |
| JavaScript | native | every `number` is binary64; `BigInt` for large integers |
| TypeScript | native | `number` as in JavaScript; `bigint` |
| Java | native | `double` and `float`; `BigDecimal`; strict semantics since Java 17 |
| C | native | `float`, `double` and `long double`; IEC 60559 under Annex F |
| C++ | native | as C; `std::numeric_limits` describes each type |
| C# | native | `double` and `float`; `decimal` for money |
| Go | native | `float64` and `float32`; exact constant arithmetic |
| Rust | native | `f64` and `f32`; `PartialOrd` but not `Ord`; `total_cmp` |
| Kotlin | native | `Double` and `Float` of the JVM |
| PowerShell | native | `[double]` and `[decimal]` of .NET |
| Bash | absent | integer arithmetic only |
| SQL | native | `REAL` and `DOUBLE PRECISION` for floats; `NUMERIC` and `DECIMAL` for exact values |
| Regex | n/a | a pattern matches text, not numbers |
| HTML/CSS | n/a | CSS has numbers, but a page does not do arithmetic on them in the sense of this page |

All the tier 1 languages that have floats use the IEEE 754 binary formats on common hardware, so `0.1 + 0.2` gives `0.30000000000000004` in each of them. The differences are in the types around the floats, in the comparisons, and in the rules for constants.

- **JavaScript** has one number type. The Number type holds the binary64 values, except that the many NaN bit patterns of IEEE 754 are "represented in ECMAScript as a single special NaN".[^3]
- **Java** made all floating-point operations strict in Java 17. Java 1.2 had allowed a looser default because the x87 coprocessor could not match the strict rules at low cost. The SSE2 instructions, shipped "starting circa 2001", removed that reason.[^4]
- **Rust** gives `f64` the traits `PartialEq` and `PartialOrd`, but not `Eq` and `Ord`, because a NaN breaks the laws of a total order. `total_cmp` gives the `totalOrder` predicate of the 2008 revision of IEEE 754, where every value, NaN too, has a place.[^5] The Rust example shows that `v.sort()` does not compile for a `Vec<f64>`.
- **Go** evaluates constant expressions exactly: "Constant expressions are always evaluated exactly".[^6] The constant `0.1 + 0.2` becomes the double nearest to 0.3. The same sum of two variables gives `0.30000000000000004`.
- **C#** has a `decimal` type. It "is not an IEEE 754 floating-point format": it holds a 96-bit integer scaled by a power of 10, and it has no NaN and no infinity.[^7] It holds 0.1 exactly.
- **Python** prints the shortest string that reads back as the same double. Since Python 3.1, `repr(0.1)` gives `0.1`, though the stored value is not 0.1.[^2] Since Python 3.12, `sum` uses Neumaier summation for floats.[^8]
- **C** does not require IEEE 754. An implementation that defines `__STDC_IEC_60559_BFP__`, or the older macro `__STDC_IEC_559__`, conforms to IEC 60559 as Annex F specifies. C23 marks the older macro as obsolescent. The header `<float.h>` gives the properties of each type, such as `DBL_MANT_DIG` and `DBL_EPSILON`.[^9]
- **Bash** has no floats. The Bash example shows the error, and it keeps money as an integer number of cents.

## Variations and related ideas

- **Decimal floating point.** A decimal format stores digits in base 10, so 0.1 is exact. Python's `decimal`, Java's `BigDecimal` and C#'s `decimal` work this way. They still round: one third has no exact decimal form.
- **Fixed point and integers.** Money is often stored as an integer number of the smallest unit, such as cents. Addition is then exact, and rounding happens only where the business rules say. The TypeScript and Bash examples do this.
- **Rational numbers.** Python's `Fraction` keeps a numerator and a denominator, and every result of `+`, `-`, `*` and `/` is exact. The numbers can grow large.
- **Other widths.** Binary16 and bfloat16 have fewer bits and are common in machine learning. The x87 80-bit extended format has a 64-bit significand.[^1]
- **Interval arithmetic.** Each value is a pair of bounds that contain the true result. The rounding direction of each bound is chosen so that the interval stays correct.
- **Related pages.** Conversion between floats and integers can overflow or lose digits ([[type-conversion]], [[integer-overflow]]). A random float in `[0, 1)` has its own pitfalls ([[generate-random-numbers]]).

## Pitfalls

### Comparing with ==

Two computations of the same real number often give different doubles. Compare with a tolerance. Python's `math.isclose` uses a relative tolerance, by default `1e-09`.[^11] A fixed absolute tolerance such as `Number.EPSILON` works only for values near 1. For values near 1e16, it is smaller than the gap between two neighbouring doubles.

### Money in binary floats

Ten additions of 0.1 do not give exactly 1.0. The Java example `loop` prints `0.9999999999999999`. A bill that adds many prices drifts by a cent now and then. Use a decimal type or integer cents.

### Rounding a decimal that was never exact

`round(2.675, 2)` in Python gives `2.67`, and `(1.005).toFixed(2)` in JavaScript gives `"1.00"`. The functions are correct. The stored values are a little below 2.675 and 1.005, so the nearest result is the lower one.

### Catastrophic cancellation

Subtraction of two close values removes the digits they share. Goldberg: the subtraction "did not introduce any error, but rather exposed the error introduced in the earlier multiplications".[^1] The C example computes `(1 + 1e-15) - 1` with a relative error of 11%. Rewrite the formula where you can. `log1p(x)` computes `log(1 + x)` without that loss.

### Summation order

Floating-point addition is not associative. `(1e16 + 1.0) - 1e16` is 0, but `(1e16 - 1e16) + 1.0` is 1. A long sum loses the small terms. The C++ example adds 0.1 ten million times. A plain loop gives 999999.99983897537, and Kahan summation gives 1000000. Python's `math.fsum` gives a correctly rounded sum.[^11]

### NaN in comparisons and collections

`NaN == NaN` is false. A sort with a comparison that does not handle NaN can give an order that is not sorted. In Go, each NaN key in a map is a separate entry that no lookup finds. Java's `Double.equals` and `Double.compare` treat NaN as equal to itself, so collections of `Double` work, but they differ from `==`.

### Optimizations that assume real arithmetic

Kahan summation computes `c = (t - sum) - y`. In real arithmetic this is always 0. Goldberg warns that an optimizer "that believed floating-point arithmetic obeyed the laws of algebra would conclude that C = [T-S] - Y = [(S+Y)-S] - Y = 0, rendering the algorithm completely useless".[^1] GCC's `-ffast-math` sets `-funsafe-math-optimizations`, which enables `-fassociative-math`, and that option allows such changes.[^10]

### Large integers in doubles

A double holds every integer up to 2^53 exactly, and not all of them above it. In JavaScript, `2 ** 53 + 1` gives `2 ** 53`. IDs and counters from a database can pass this limit. Keep them as strings or `BigInt`.

## Cost

- **Speed.** Addition and multiplication of doubles are single instructions on modern processors, close to integer speed. Division and square root are slower.
- **Decimal types.** `decimal` and `BigDecimal` work in software, and they are much slower than doubles. The difference matters for numerical work, not for a bill.
- **Subnormals.** Some hardware handles subnormal numbers with software traps. Goldberg notes that programs that often underflow then run "noticeably slower".[^1] Options such as flush-to-zero trade gradual underflow for speed.
- **Compensated sums.** Kahan summation does four operations for each term instead of one.

## History

- **1985.** IEEE 754-1985 defined the binary32 and binary64 formats, the special values, and correct rounding of the basic operations.[^1]
- **1991.** David Goldberg's survey "What Every Computer Scientist Should Know About Floating-Point Arithmetic" explained rounding error, the standard and its effect on compilers.[^1]
- **Late 1990s.** Java SE 1.2 added a looser default floating-point mode next to `strictfp`, because of the x87 coprocessor.[^4]
- **2008.** The revision of IEEE 754 defined the `totalOrder` predicate that Rust's `total_cmp` follows.[^5]
- **2021.** Java 17 made all floating-point operations strict again (JEP 306).[^4]
- **2023.** Python 3.12 changed `sum` to use Neumaier summation for floats.[^8]

## References
