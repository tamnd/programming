---
tag: Z584G7
kind: concept
slug: nan-and-infinity
name: NaN and infinity
aliases: [NaN, not a number, infinity, inf, quiet NaN, signaling NaN, signalling NaN, negative zero, signed zero]
area: foundations/numbers-in-machines
level: core
summary: >-
  IEEE 754 floats have values that are not ordinary numbers: two infinities, two zeros and many NaNs. A NaN is not equal to itself, which breaks equality tests, sorting and lookups.
applies_to: core-languages
viz: optional
formal: none
see_also: [floating-point-arithmetic, floating-point-rounding-error, machine-epsilon, ones-complement-and-sign-magnitude, integer-division-and-modulo, type-conversion]
same_as:
  wikidata: Q858684
  wikipedia: NaN
sources:
  - url: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
    title: David Goldberg, What Every Computer Scientist Should Know About Floating-Point Arithmetic, ACM Computing Surveys 23(1), March 1991 (edited reprint by Sun Microsystems)
    accessed: 2026-11-16
    locator: the section Special Quantities, and Infinity
  - url: https://docs.python.org/3/reference/expressions.html
    title: The Python Language Reference, 6. Expressions, Python Software Foundation
    accessed: 2026-11-16
    locator: 6.10.1 Value comparisons, and 6.10.2 Membership test operations
  - url: https://en.wikipedia.org/wiki/Signed_zero
    title: Signed zero, Wikipedia
    accessed: 2026-11-16
    locator: the lead, and Representations
  - url: https://en.wikipedia.org/wiki/NaN
    title: NaN, Wikipedia
    accessed: 2026-11-16
    locator: the lead, Floating point, Comparison with NaN, Operations generating NaN, Signaling NaN, Encoding, and Canonical NaN
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/NaN
    title: NaN, MDN Web Docs
    accessed: 2026-11-16
    locator: Description, and Examples (Testing against NaN)
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/is
    title: Object.is(), MDN Web Docs
    accessed: 2026-11-16
    locator: Description
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Double.html
    title: Double (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: the class description (Floating-point Equality, Equivalence, and Comparison), and Double.compare
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: Primitive Type f64, The Rust Project
    accessed: 2026-11-16
    locator: the method total_cmp
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification, The Go Authors
    accessed: 2026-11-16
    locator: Min and max
  - url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
    title: GCC Manual, Options That Control Optimization, Free Software Foundation
    accessed: 2026-11-16
    locator: -ffinite-math-only
  - url: https://developer.mozilla.org/en-US/docs/Web/CSS/calc-keyword
    title: "<calc-keyword>, MDN Web Docs"
    accessed: 2026-11-16
    locator: Values, and Notes
  - url: https://www.postgresql.org/docs/current/datatype-numeric.html
    title: PostgreSQL Documentation, 8.1. Numeric Types, The PostgreSQL Global Development Group
    accessed: 2026-11-16
    locator: 8.1.2 Arbitrary Precision Numbers, and 8.1.3 Floating-Point Types
  - url: https://www.rfc-editor.org/rfc/rfc8259.txt
    title: RFC 8259, The JavaScript Object Notation (JSON) Data Interchange Format, IETF
    accessed: 2026-11-16
    locator: section 6, Numbers
  - url: https://docs.python.org/3/library/json.html
    title: json, JSON encoder and decoder, Python Software Foundation
    accessed: 2026-11-16
    locator: json.dump, the parameter allow_nan
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
    title: JSON.stringify(), MDN Web Docs
    accessed: 2026-11-16
    locator: Description
---

## Idea

Some float operations have no number as their result. `0/0` has no value, and `1e308 * 10` is too large for a double. A program could stop at such a step. IEEE 754 instead gives the result a special value, and the computation goes on.[^1]

```python run id=idea
import math

big = 1e308
print("1e308 * 10:", big * 10)
print("inf - inf:", math.inf - math.inf)
print("inf * 0:", math.inf * 0)

nan = float("nan")
print("nan == nan:", nan == nan)
print("nan != nan:", nan != nan)
print("3 < nan or nan < 3:", 3 < nan or nan < 3)
print("-0.0 == 0.0:", -0.0 == 0.0)
print("copysign(1, -0.0):", math.copysign(1, -0.0))
```

```output
1e308 * 10: inf
inf - inf: nan
inf * 0: nan
nan == nan: False
nan != nan: True
3 < nan or nan < 3: False
-0.0 == 0.0: True
copysign(1, -0.0): -1.0
```

There are three kinds of special values.

- **Infinity.** A result too large for the format becomes plus or minus infinity. Infinity takes part in arithmetic: `1/inf` is 0.[^1]
- **NaN.** NaN, short for not a number, is the result of an operation with no defined value. Every comparison with a NaN is false, except `!=`.[^2]
- **Signed zero.** There are two zeros, `+0` and `-0`. They compare equal, but they can give different results in some operations.[^3]

## How it works

### The bits

In a binary format, an exponent field of all ones marks the special values. A fraction of zero means infinity, and any other fraction means NaN.[^4] So there is one positive infinity, one negative infinity, and many NaNs. In most processors, the top bit of the fraction tells a quiet NaN from a signaling NaN, and "The remaining bits encode a payload".[^4] Zero has an exponent field and a fraction of all zeros, and "Negative zero has the sign bit set to one."[^3]

### Operations that give NaN

The NaN article lists three kinds of operations that return NaN: most operations with a NaN operand, the indeterminate forms, and real operations with complex results.[^4] The indeterminate forms include `0/0`, `inf/inf`, `0 * inf`, `inf - inf`, and the remainder `x mod y` when x is infinite or y is zero.[^4] A quiet NaN goes through each later step, which "allows errors to be detected at the end of a sequence of operations without extensive testing during intermediate stages".[^4]

### Quiet and signaling NaN

"Quiet NaNs are used to propagate errors resulting from invalid operations or values."[^4] A signaling NaN raises the invalid operation exception when most operations use it, and then becomes a quiet NaN.[^4] One proposed use is to fill memory that is not yet initialized with signaling NaNs, so that a read of it raises the exception.[^4]

### Comparisons

The relations `<`, `<=`, `==`, `>=` and `>` "return false on an unordered relation", that is, when one operand is a NaN.[^4] So `NOT(x < y)` and `x >= y` differ when x or y is a NaN.[^4] A NaN is not equal to itself. Python's reference: "A counter-intuitive implication is that not-a-number values are not equal to themselves."[^2] The usual test for NaN is therefore `x != x`, though each language also has a named function such as `isnan`.

### Signed zero and infinity

IEEE 754 "requires both +0 and −0".[^3] The two zeros compare equal, but `1/+0` is `+inf` and `1/-0` is `-inf`.[^3] The sign of zero keeps the direction from which a value reached zero, for example when a negative number underflows.[^3] Goldberg shows that infinity arithmetic can remove a test: `1/(x + 1/x)` gives the correct value 0 at `x = 0` because `1/0` is infinity.[^1]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `math.inf`, `math.nan`, `math.isnan`, `math.isinf`, `math.copysign` |
| JavaScript | native | `NaN`, `Infinity`, `Number.isNaN`, `Object.is` |
| TypeScript | native | as JavaScript |
| Java | native | `Double.NaN`, `Double.POSITIVE_INFINITY`, `Double.isNaN`, `Double.compare` |
| C | native | `NAN`, `INFINITY`, `isnan`, `isinf`, `signbit` in `<math.h>` |
| C++ | native | `std::numeric_limits<double>::quiet_NaN()`, `infinity()`, `std::isnan` |
| C# | native | `double.NaN`, `double.PositiveInfinity`, `double.IsNaN` |
| Go | native | `math.NaN()`, `math.Inf(1)`, `math.IsNaN`, `math.Signbit` |
| Rust | native | `f64::NAN`, `f64::INFINITY`, `is_nan`, `total_cmp` |
| Kotlin | native | `Double.NaN`, `Double.POSITIVE_INFINITY`, `isNaN()` |
| PowerShell | native | `[double]::NaN`, `[double]::PositiveInfinity` |
| Bash | absent | integer arithmetic only |
| SQL | native | PostgreSQL `real`, `double precision` and `numeric` accept `'NaN'` and `'Infinity'`; NULL marks a missing value |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | native | the constants `infinity`, `-infinity` and `NaN` inside `calc()` |

The IEEE rules are the same in every language with floats. The differences are in the equality of the containers and the sort functions.

- **JavaScript** has one NaN value in the language. With `==`, `!=`, `===` and `!==`, `NaN` "compares unequal" to every value, "including to another NaN value".[^5] `indexOf` "cannot find NaN", but `includes` can.[^5] `Object.is` differs from `===` only "in their treatment of signed zeros and NaN values".[^6]
- **Java** keeps the IEEE rules for `==` and `<`. `Double.equals` and `Double.compare` define `Double.NaN` "to be equal to itself and greater than all other double values", and they put `-0.0` below `0.0`.[^7] This "allows NaN to be used as an element of a HashSet or as the key of a HashMap".[^7]
- **Python** tests membership in a list, a set or a dict as `any(x is e or x == e for e in y)`.[^2] The same NaN object is found, because `x is e` is true. A different NaN object is not.
- **Rust** gives `f64` only `PartialOrd`. `total_cmp` gives "an ordering in accordance to the totalOrder predicate" of IEEE 754-2008.[^8]
- **Go** defines `min` and `max` on floats so that "if any argument is a NaN, the result is a NaN", and negative zero is smaller than zero.[^9]
- **C and C++** give `isnan`, `isinf` and `signbit`. GCC's `-ffinite-math-only` allows optimizations that "assume that arguments and results are not NaNs or +-Infs", so a test for NaN may not work under it.[^10]
- **CSS** has the constants `infinity`, `-infinity` and `NaN` in `calc()`, and MDN says that the arguments follow "the IEEE-754 standard for floating point math". In CSS too, `0 / 0` and `infinity - infinity` give NaN.[^11]
- **SQL** in PostgreSQL has `NaN` and `Infinity` in its float types and in `numeric`. "In order to allow floating-point values to be sorted and used in tree-based indexes, PostgreSQL treats NaN values as equal, and greater than all non-NaN values."[^12]

## Variations and related ideas

- **NaN-boxing.** A NaN has many unused payload bits. Some systems keep other data in them, a trick called NaN-boxing.[^4]
- **Canonical NaN.** Some systems produce one NaN value only. JavaScript treats all NaNs as the same value.[^4]
- **Integer division by zero.** Integers have no infinity and no NaN. An integer `1/0` is an error, an exception or undefined behaviour ([[integer-division-and-modulo]]).
- **Signed zero in integers.** Ones' complement and sign-magnitude integers also have a negative zero ([[ones-complement-and-sign-magnitude]]).[^3]

## Pitfalls

### A test with == for NaN

`x == float("nan")` is always false. Use the named test, such as `math.isnan(x)` or `Number.isNaN(x)`.

### Sorting a list with NaN

A sort assumes that the comparison is a total order. With a NaN, `a < b` and `b < a` are both false for every b, so the sort can stop with the list out of order. Remove the NaNs first, or sort with a total order such as Java's `Double.compare` or Rust's `total_cmp`. The Python example shows a sorted list that is not in order.

### NaN keys in maps and sets

A hash table finds a key with `==`. A NaN key is not equal to itself, so a lookup with a new NaN finds nothing, and each NaN can be a new key. Python finds the same NaN object through its identity test, but not an equal-looking new one.[^2]

### min and max with NaN

The result depends on the function. Go's `min` gives NaN.[^9] The Python example shows that `max` can give a different result when the NaN moves. The IEEE 754-2008 functions `minNum` and `maxNum` return the number when only one operand is NaN, and IEEE 754-2019 replaced them because they "are not associative" when a signaling NaN appears.[^4]

### NaN and infinity in JSON

JSON has no NaN and no infinity. RFC 8259: "Numeric values that cannot be represented in the grammar below (such as Infinity and NaN) are not permitted."[^13] Python's `json.dump` writes `NaN` and `Infinity` by default, unless `allow_nan` is false, and then it raises `ValueError` "in strict compliance with the JSON specification".[^14] JavaScript's `JSON.stringify` writes `null` for them.[^15] A NaN can therefore leave one program and fail in the next one.

### A lost minus sign

A negative number that underflows or rounds to zero can become `-0.0`, and a report can then show `-0.00`. `x == 0` is true for both zeros, so a test for the sign needs `signbit` or `copysign`.[^3]

## Cost

- **Checks.** A program that must not output NaN needs a check at its boundary, because a NaN does not stop the computation.
- **Fast-math.** GCC does not turn on `-ffinite-math-only` at any `-O` level except `-Ofast`, because it "can result in incorrect output for programs that depend on an exact implementation of IEEE or ISO rules".[^10]

## History

- **Before 1985.** The CDC 6600 had bit patterns for the special quantities INDEFINITE and INFINITY. Goldberg traces the idea of the IEEE special values to it.[^1]
- **1985.** "Systematic use of NaNs was introduced by the IEEE 754 floating-point standard in 1985", together with the infinities.[^4]
- **2008.** IEEE 754-2008 defined the `totalOrder` predicate, which Rust's `total_cmp` follows.[^8]
- **2019.** IEEE 754-2019 replaced `minNum` and `maxNum`, and it reserved the top bit of the fraction to tell quiet NaN from signaling NaN.[^4]

## References
