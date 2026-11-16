---
tag: G7MRNR
kind: concept
slug: rounding-modes
name: Rounding modes
aliases: [rounding mode, rounding rule, tie-breaking rule, round half to even, bankers' rounding, round half up, round half away from zero, round toward zero, truncation, floor, ceiling, directed rounding]
area: foundations/numbers-in-machines
level: core
summary: >-
  A rounding mode says which nearby value replaces a number that has too many digits. Modes differ in their direction and in how they break ties, and the default differs between languages and functions.
applies_to: core-languages
viz: optional
formal: none
see_also: [floating-point-rounding-error, floating-point-arithmetic, integer-division-and-modulo, fixed-point-arithmetic, arbitrary-precision-arithmetic, binary-coded-decimal, machine-epsilon]
same_as:
  wikidata: Q663208
  wikipedia: Rounding
sources:
  - url: https://en.wikipedia.org/wiki/Rounding
    title: Rounding, Wikipedia
    accessed: 2026-11-16
    locator: the lead, Directed rounding to an integer, Rounding to the nearest integer, Double rounding, and History
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, Python Software Foundation
    accessed: 2026-11-16
    locator: round()
  - url: https://docs.python.org/3/library/decimal.html
    title: decimal, Decimal fixed point and floating point arithmetic, Python Software Foundation
    accessed: 2026-11-16
    locator: Quick-start tutorial, and Rounding modes
  - url: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
    title: David Goldberg, What Every Computer Scientist Should Know About Floating-Point Arithmetic, ACM Computing Surveys 23(1), March 1991 (edited reprint by Sun Microsystems)
    accessed: 2026-11-16
    locator: Theorem 5 and its example, and the section Rounding Modes
  - url: https://en.wikipedia.org/wiki/IEEE_754
    title: IEEE 754, Wikipedia
    accessed: 2026-11-16
    locator: Rounding rules
  - url: https://en.cppreference.com/c/numeric/fenv
    title: Floating-point environment (C), cppreference.com
    accessed: 2026-11-16
    locator: the description, fesetround, and the FE_ rounding macros
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/round
    title: Math.round(), MDN Web Docs
    accessed: 2026-11-16
    locator: Description
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: Math.round(double), and Math.rint
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/math/RoundingMode.html
    title: RoundingMode (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: the enum description and the constants
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.math.round
    title: Math.Round Method, Microsoft Learn
    accessed: 2026-11-16
    locator: Remarks, Midpoint values and rounding conventions
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.midpointrounding
    title: MidpointRounding Enum, Microsoft Learn
    accessed: 2026-11-16
    locator: Fields
  - url: https://pkg.go.dev/math
    title: math package, The Go Authors
    accessed: 2026-11-16
    locator: func Round, and func RoundToEven
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: Primitive Type f64, The Rust Project
    accessed: 2026-11-16
    locator: the methods round and round_ties_even
  - url: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
    title: GCC Manual, Options That Control Optimization, Free Software Foundation
    accessed: 2026-11-16
    locator: -frounding-math
  - url: https://www.postgresql.org/docs/current/functions-math.html
    title: PostgreSQL Documentation, 9.3. Mathematical Functions and Operators, The PostgreSQL Global Development Group
    accessed: 2026-11-16
    locator: round
  - url: https://developer.mozilla.org/en-US/docs/Web/CSS/round
    title: round(), MDN Web Docs
    accessed: 2026-11-16
    locator: Parameters
---

## Idea

A number often has more digits than the result can keep. A sum of money has more than two decimal places, an exact quotient has more bits than a double, and a measurement must become an integer. A rounding mode is the rule that picks the value to keep.

There are two kinds of rules.

- **Directed rounding** always moves the same way: down (floor, toward minus infinity), up (ceiling, toward plus infinity), toward zero (truncation), or away from zero.[^1]
- **Rounding to nearest** picks the closest value. It needs a second rule for a tie, a value exactly halfway between two candidates. The common tie rules are half up, half away from zero, and half to even.[^1]

```python run id=idea
import math
from decimal import ROUND_HALF_UP, Decimal


def half_away(x):
    return int(Decimal(x).quantize(Decimal(1), rounding=ROUND_HALF_UP))


print(f"{'x':>5} {'even':>5} {'away':>5} {'floor':>5} {'ceil':>5} {'trunc':>5}")
for x in [0.5, 1.5, 2.5, -0.5, -1.5, -2.5]:
    print(f"{x:5} {round(x):5} {half_away(x):5} {math.floor(x):5} {math.ceil(x):5} {math.trunc(x):5}")
```

```output
    x  even  away floor  ceil trunc
  0.5     0     1     0     1     0
  1.5     2     2     1     2     1
  2.5     2     3     2     3     2
 -0.5     0    -1    -1     0     0
 -1.5    -2    -2    -2    -1    -1
 -2.5    -2    -3    -3    -2    -2
```

Python's `round` uses half to even, so `round(0.5)` and `round(2.5)` go to the even neighbour.[^2] Python's `ROUND_HALF_UP` in the `decimal` module means "ties going away from zero".[^3] The names of the modes are not the same in every library.

## How it works

### The tie rules

Every round-to-nearest rule gives the same result except at a tie. The rules differ in where a tie goes.[^1]

- **Half up** (toward plus infinity): 2.5 becomes 3 and -2.5 becomes -2.
- **Half away from zero**: 2.5 becomes 3 and -2.5 becomes -3. Wikipedia calls this rule "commonly taught and used".[^1]
- **Half to even**: a tie goes to the even neighbour. "Thus, for example, 23.5 becomes 24, as does 24.5".[^1] It is also called convergent rounding, statistician's rounding, Gaussian rounding or bankers' rounding.[^1]
- **Half down** and **half to odd** are two more tie rules.[^1]

### The rounding modes of IEEE 754

Every IEEE 754 operation computes the exact result and then rounds it to the format.[^4] The rule for that step is the rounding mode. IEEE 754 "defines five rounding rules".[^5]

| Rule | +11.5 | +12.5 | -11.5 | -12.5 |
|---|---|---|---|---|
| to nearest, ties to even | +12 | +12 | -12 | -12 |
| to nearest, ties away from zero | +12 | +13 | -12 | -13 |
| toward 0 | +11 | +12 | -11 | -12 |
| toward +infinity | +12 | +13 | -11 | -12 |
| toward -infinity | +11 | +12 | -12 | -13 |

The table follows the IEEE 754 article.[^5] "Round to nearest, ties to even" is the default for binary floating point, and ties away from zero "is only required for decimal implementations".[^5] Goldberg: "By default, rounding means round toward nearest. The standard requires that three other rounding modes be provided".[^4]

Most programs never change the mode. C gives access to it through `<fenv.h>`: `fegetround` and `fesetround` with the macros `FE_TONEAREST`, `FE_UPWARD`, `FE_DOWNWARD` and `FE_TOWARDZERO`.[^6]

### Why half to even

A rule that always sends ties the same way adds a small bias. Goldberg shows it with three decimal digits, `x = 1.00` and `y = -0.555`. Under round half up, the steps `x - y` and then `+ y` give 1.56, 1.01, 1.57, 1.02, and so on: "each successive value of xn increases by .01". "Under round to even, xn is always 1.00."[^4] Goldberg's conclusion: "when using the round up rule, computations can gradually drift upward, whereas when using round to even the theorem says this cannot happen."[^4]

With half to even, the errors of many roundings tend to cancel. Wikipedia says the error of a sum then grows "in proportion to the square root of the number of operations rather than linearly".[^1]

### Directed rounding and interval arithmetic

The directed modes give bounds. Goldberg: "One application of rounding modes occurs in interval arithmetic". The sum of two intervals takes its lower end rounded toward minus infinity and its upper end rounded toward plus infinity, so the exact result is inside the interval.[^4] Round toward minus infinity also makes conversion to an integer the floor function, and round toward plus infinity makes it the ceiling.[^4]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `round` (half to even), `math.floor`, `math.ceil`, `math.trunc`, `decimal` modes `ROUND_HALF_EVEN`, `ROUND_HALF_UP`, `ROUND_DOWN` and others |
| JavaScript | native | `Math.round` (half toward plus infinity), `Math.floor`, `Math.ceil`, `Math.trunc` |
| TypeScript | native | as JavaScript |
| Java | native | `Math.round` (half toward plus infinity), `Math.rint` (half to even), `java.math.RoundingMode` |
| C | native | `round`, `rint`, `floor`, `ceil`, `trunc`, and `fesetround` |
| C++ | native | `std::round`, `std::rint`, `std::fesetround` |
| C# | native | `Math.Round` (half to even by default), `MidpointRounding` |
| Go | native | `math.Round` (half away from zero), `math.RoundToEven`, `math.Floor`, `math.Ceil`, `math.Trunc` |
| Rust | native | `round` (half away from zero), `round_ties_even`, `floor`, `ceil`, `trunc` |
| Kotlin | native | `round`, `roundToInt`, `java.math.RoundingMode` |
| PowerShell | native | `[math]::Round` with `MidpointRounding` |
| Bash | absent | integer arithmetic only |
| SQL | native | PostgreSQL `round`, `trunc`, `floor`, `ceil` |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | native | `round()` with `nearest`, `up`, `down` or `to-zero` |

The function called "round" has a different tie rule in each language. This is the main trap.

- **Python** `round` rounds to the closest multiple of the power of ten, and "if two multiples are equally close, rounding is done toward the even choice".[^2] The `decimal` module has eight modes and uses `ROUND_HALF_EVEN` in its default context.[^3]
- **JavaScript** `Math.round` rounds a fraction of exactly 0.5 "to the next integer in the direction of +∞". MDN notes that this "differs from many languages' round() functions, which often round half-increments away from zero".[^7]
- **Java** `Math.round` returns "the closest long to the argument, with ties rounding to positive infinity".[^8] `RoundingMode` has `UP`, `DOWN`, `CEILING`, `FLOOR`, `HALF_UP`, `HALF_DOWN`, `HALF_EVEN` and `UNNECESSARY`, for use with `BigDecimal`.[^9] Java's `HALF_UP` means ties away from zero: the RoundingMode table rounds -2.5 to -3 under `HALF_UP`.[^9]
- **C#** `Math.Round` "uses the round to nearest even convention" by default.[^10] `MidpointRounding` adds `AwayFromZero`, `ToZero`, `ToNegativeInfinity` and `ToPositiveInfinity`.[^11]
- **Go** `math.Round` "returns the nearest integer, rounding half away from zero", and `math.RoundToEven` rounds "ties to even".[^12]
- **Rust** `round` rounds "half-way between two integers" "away from 0.0", and `round_ties_even` rounds them "to the number with an even least significant digit".[^13]
- **C** has `fesetround` to change the IEEE rounding mode of arithmetic.[^6] GCC assumes round-to-nearest unless the program is built with `-frounding-math`, which "should be specified for programs that change the FP rounding mode dynamically".[^14]
- **SQL** in PostgreSQL: for `numeric`, "ties are broken by rounding away from zero". For `double precision`, "the tie-breaking behavior is platform dependent".[^15]
- **CSS** has `round()` with the strategies `nearest`, `up`, `down` and `to-zero`. With `nearest`, a value halfway between two targets "will be rounded up", as in JavaScript `Math.round()`.[^16]

## Variations and related ideas

- **Stochastic rounding.** The value is rounded up or down at random, with a probability that depends on its distance from each neighbour.[^1]
- **Round to odd.** A first rounding to an odd last digit avoids the error of rounding twice.[^1]
- **Integer division.** Integer division is rounding too: truncated division rounds toward zero and floored division rounds toward minus infinity ([[integer-division-and-modulo]]).
- **Decimal types.** Rounding a binary float to two decimals cannot be exact, because the float is not exact. A decimal type such as Python's `Decimal` or Java's `BigDecimal` rounds the decimal digits exactly ([[floating-point-rounding-error]], [[fixed-point-arithmetic]]).

## Pitfalls

### Assuming that round means half up

`round(2.5)` is 2 in Python and C#, 3 in Go, Rust and JavaScript. `round(-2.5)` is -2 in Python, C#, JavaScript and Java, and -3 in Go and Rust.[^2][^10][^12][^13][^7][^8] Code ported from one language to another can change its results at every tie.

### Rounding a binary float to decimal places

Python's docs: "round(2.675, 2) gives 2.67 instead of the expected 2.68". This is because 2.675 as a float is a little less than 2.675.[^2] The rounding mode is not the cause, and no mode fixes it. Use a decimal type for values that must round exactly in decimal ([[floating-point-rounding-error]]).

### Rounding twice

"Rounding a number twice in succession to different levels of precision, with the latter precision being coarser, is not guaranteed to give the same result as rounding once to the final precision".[^1] For example, 9.46 rounds to 9.5 at one decimal and then to 10, but 9.46 rounded once to an integer is 9.[^1]

### Truncation that adds up

The Vancouver Stock Exchange started a new index at 1000.000 in 1982. After 22 months it "had fallen to about 520, although the market appeared to be rising".[^1] The index was recalculated thousands of times a day and "always being truncated (rounded down) to 3 decimal places".[^1] With rounding to nearest, the value was 1098.892, not 524.811.[^1]

### Changing the mode in C without telling the compiler

Access to the floating-point environment "is only meaningful when #pragma STDC FENV_ACCESS is set to ON".[^6] Without it, the compiler may fold constants with the default mode. GCC needs `-frounding-math`, which "disables constant folding of floating-point expressions at compile time".[^14]

## Cost

The rounding of float arithmetic is done by the hardware in every operation, with no extra cost. Library rounding to an integer is a single function call. Rounding a `Decimal` or a `BigDecimal` costs more, because it works on the decimal digits ([[arbitrary-precision-arithmetic]]). The real cost is in the results: a biased rule can move a long sum far from the true value, as the Vancouver index shows.[^1]

## History

Half to even is old. In 1906 Robert Simpson Woodward called it "the computer's rule". This shows that the human computers who made mathematical tables used it then.[^1] The name bankers' rounding is less clear: "If this rounding method was ever a standard in banking, the evidence has proved extremely difficult to find."[^1] Reiser and Knuth gave a reason to prefer round to even in 1975.[^4] IEEE 754 made round to nearest, ties to even, the default for binary floating point.[^5]

## References
