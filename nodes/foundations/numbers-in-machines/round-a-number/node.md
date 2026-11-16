---
tag: BAZ4PH
kind: task
slug: round-a-number
name: Round a number
aliases: [round a float, round half away from zero, round half to even, banker's rounding, round to n decimal places, floor and ceiling, truncate a number]
area: foundations/numbers-in-machines
level: intro
summary: >-
  Round a float to an integer or to n decimal places. Each library has its own rule for a half, and a decimal such as 2.675 is not exact in binary.
contract: contract.yaml
category: numbers
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.math.round
    title: Math.Round Method, .NET API reference, Microsoft Learn
    locator: Overloads
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/functions.html
    title: Built-in Functions, Python 3 documentation
    locator: round()
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/math.html
    title: math, Mathematical functions, Python 3 documentation
    locator: math.ceil, math.floor, math.trunc
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/round
    title: Math.round(), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/trunc
    title: Math.trunc(), MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/toFixed
    title: Number.prototype.toFixed(), MDN Web Docs
    locator: the introduction; Examples
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: round(double), rint(double)
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/math/RoundingMode.html
    title: RoundingMode, Java SE 27 API
    locator: Enum Constants
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/numeric/math/round
    title: round, lround, llround, C reference, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/numeric/math/rint
    title: rint, lrint, llrint, C reference, cppreference.com
    locator: Notes
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/io/fprintf
    title: printf, fprintf, sprintf, snprintf, C reference, cppreference.com
    locator: the conversion specifier f
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/numeric/math/round
    title: std::round, std::lround, std::llround, C++ reference, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math
    title: math, Go standard library
    locator: func Round, func RoundToEven
    accessed: 2026-11-16
  - url: https://pkg.go.dev/strconv
    title: strconv, Go standard library
    locator: func FormatFloat
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: f64, Rust standard library
    locator: round, round_ties_even
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/fmt/index.html
    title: std::fmt, Rust standard library
    locator: Precision
    accessed: 2026-11-16
  - url: https://tiswww.case.edu/php/chet/bash/bashref.html
    title: Bash Reference Manual, version 5.3
    locator: 4.2 Bash Builtin Commands, printf
    accessed: 2026-11-16
---

## Task

Round a float to the nearest integer. When the float is exactly halfway between two integers, choose the integer that is farther from zero.

The function `round_half_away(x)` returns an integer. For example, `round_half_away(2.4)` is 2, `round_half_away(2.5)` is 3, and `round_half_away(-2.5)` is -3.

Each rounding function answers two questions. The first is the direction: to the nearest integer, down, up, or toward zero. The second is the rule for a half, when the two nearest integers are equally close:

| Rule | 2.5 | -2.5 | 3.5 | Functions |
|---|---|---|---|---|
| floor, toward minus infinity | 2 | -3 | 3 | `math.floor`, `Math.floor`, `floor` |
| ceiling, toward plus infinity | 3 | -2 | 4 | `math.ceil`, `Math.ceil`, `ceil` |
| truncation, toward zero | 2 | -2 | 3 | `math.trunc`, `Math.trunc`, `trunc` |
| nearest, half away from zero | 3 | -3 | 4 | C `round`, Go `math.Round`, Rust `f64::round` |
| nearest, half to even | 2 | -2 | 4 | Python `round`, C# `Math.Round`, Go `math.RoundToEven` |
| nearest, half toward plus infinity | 3 | -2 | 4 | JavaScript `Math.round`, Java `Math.round` |

The rule for a half is the one that differs most between libraries. The same call `round(2.5)` gives 2 in Python and 3 in C. The .NET documentation gives another name for a half to even: "Rounding to nearest even, or banker's rounding"[^1].

Rounding to n decimal places has one more problem. Most decimal fractions are not exact in binary, so the float nearest to 2.675 is a little below 2.675, and `round(2.675, 2)` in Python gives 2.67. See [[floating-point-arithmetic]] and [[decimal-arithmetic]].

## Solutions

| Language | Half away from zero | Half to even | To n decimal places |
|---|---|---|---|
| Python | the floor of the magnitude, then a test | `round(x)` | `round(x, n)` |
| JavaScript, TypeScript | `Math.sign(x) * Math.round(Math.abs(x))` | no built-in function | `x.toFixed(n)` gives a string |
| Java | `Math.round` on the magnitude | `Math.rint(x)` | `BigDecimal.setScale(n, RoundingMode.HALF_UP)` |
| C, C++ | `llround(x)` | `rint(x)` in the default rounding mode | `printf("%.2f", x)` |
| C# | `Math.Round(x, MidpointRounding.AwayFromZero)` | `Math.Round(x)` | `Math.Round(x, n, mode)` |
| Go | `math.Round(x)` | `math.RoundToEven(x)` | `strconv.FormatFloat(x, 'f', n, 64)` |
| Rust | `x.round()` | `x.round_ties_even()` | `format!("{:.2}", x)` |
| Bash | the first digit after the point | `printf '%.0f'` | `printf '%.2f'` |

- **Python.** `round(x)` rounds to the nearest integer, and "if two multiples are equally close, rounding is done toward the even choice"[^2]. So `round(0.5)` is 0 and `round(2.5)` is 2. `math.floor(x)` is "the largest integer less than or equal to x"[^3], `math.ceil(x)` is "the smallest integer greater than or equal to x"[^3], and `math.trunc(x)` "rounds toward 0"[^3]. The solution rounds the magnitude: it takes the floor, then adds 1 when the fraction is 0.5 or more.
- **JavaScript.** When "the fractional portion is exactly 0.5, the argument is rounded to the next integer in the direction of +∞"[^4], so `Math.round(-2.5)` is -2. The solution rounds the magnitude and sets the sign after. `Math.trunc` "returns the integer part of a number by removing any fractional digits"[^5]. `toFixed` "returns a string representing this number using fixed-point notation with the specified number of decimal places"[^6].
- **TypeScript.** The same code as JavaScript.
- **Java.** `Math.round` "Returns the closest long to the argument, with ties rounding to positive infinity"[^7]. `Math.rint` gives a `double`, and "If two double values that are mathematical integers are equally close, the result is the integer value that is even"[^7]. For decimal places, `RoundingMode` "Specifies a rounding policy for numerical operations capable of discarding precision"[^8]: `HALF_UP` and `HALF_EVEN` are two of its constants.
- **C.** `round` and `llround` round to the nearest integer, "rounding halfway cases away from zero, regardless of the current rounding mode"[^9]. In the default rounding mode, `rint` is different: "halfway cases are rounded to even rather than away from zero"[^10]. For `printf` with `%f`, "The precision specifies the number of decimal places"[^11].
- **C++.** `std::round` and `std::llround` in `<cmath>` have the same rule, "rounding halfway cases away from zero, regardless of the current rounding mode"[^12].
- **C#.** `Math.Round(Double)` "Rounds a double-precision floating-point value to the nearest integral value, and rounds midpoint values to the nearest even number"[^1]. For a half away from zero, give the mode `MidpointRounding.AwayFromZero`. The overload with an `Int32` "Rounds a double-precision floating-point value to a specified number of fractional digits using the specified rounding convention"[^1].
- **Go.** "Round returns the nearest integer, rounding half away from zero"[^13], and "RoundToEven returns the nearest integer, rounding ties to even"[^13]. For decimal places, the precision of `strconv.FormatFloat` with the format `'f'` "is the number of digits after the decimal point"[^14].
- **Rust.** `f64::round` returns the nearest integer: "If a value is half-way between two integers, round away from 0.0"[^15]. `f64::round_ties_even` "Rounds half-way cases to the number with an even least significant digit"[^15]. In a format string, the precision of a float "indicates how many digits after the decimal point should be printed"[^16].
- **Bash.** Bash arithmetic has integers only, so the solution reads the decimal text: the integer part, and the first digit after the point. The `printf` builtin knows "the standard printf(3) format characters"[^17], so `printf '%.2f'` rounds a number to two decimal places.

The rules of Python, with `round` and the functions of `math`:

```python run id=python-round
import math

for x in [2.5, -2.5, 3.5, 0.5]:
    print(x, round(x), math.floor(x), math.ceil(x), math.trunc(x))
print(round(2.675, 2), round(3.14159, 2))
```

```output for=python-round
2.5 2 2 3 2
-2.5 -2 -3 -2 -2
3.5 4 3 4 3
0.5 0 0 1 0
2.67 3.14
```

JavaScript rounds a half toward plus infinity, and `-0.5` rounds to negative zero:

```javascript run id=javascript-round
for (const x of [2.5, -2.5, 3.5, -0.5]) {
  console.log(x, Math.round(x), Math.floor(x), Math.ceil(x), Math.trunc(x));
}
console.log((2.55).toFixed(1), (2.35).toFixed(1), (1.005).toFixed(2));
```

```output for=javascript-round
2.5 3 2 3 2
-2.5 -2 -3 -2 -2
3.5 4 3 4 3
-0.5 -0 -1 -0 -0
2.5 2.4 1.00
```

C has both rules, and `printf` rounds the binary value of the float:

```c run id=c-round
#include <math.h>
#include <stdio.h>

int main(void) {
    double xs[] = {2.5, -2.5, 3.5};
    for (int i = 0; i < 3; i++) {
        double x = xs[i];
        printf("%4.1f: round %2.0f, rint %2.0f, floor %2.0f, ceil %2.0f, trunc %2.0f\n",
               x, round(x), rint(x), floor(x), ceil(x), trunc(x));
    }
    printf("%.2f %.2f\n", 2.675, 1.005);
}
```

```output for=c-round
 2.5: round  3, rint  2, floor  2, ceil  3, trunc  2
-2.5: round -3, rint -2, floor -3, ceil -2, trunc -2
 3.5: round  4, rint  4, floor  3, ceil  4, trunc  3
2.67 1.00
```

In C#, `Math.Round` without a mode rounds a half to even:

```csharp run id=csharp-round
using System;
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
Console.WriteLine($"{Math.Round(2.5)} {Math.Round(3.5)} {Math.Round(-2.5)}");
Console.WriteLine($"{Math.Round(2.5, MidpointRounding.AwayFromZero)} {Math.Round(-2.5, MidpointRounding.AwayFromZero)}");
Console.WriteLine($"{Math.Round(2.675, 2)} {Math.Round(2.675m, 2, MidpointRounding.AwayFromZero)}");
```

```output for=csharp-round
2 4 -2
3 -3
2.68 2.68
```

The last line rounds 2.675 to two decimal places. For the `double` value, `Math.Round` gives 2.68 on this run, but Python and C give 2.67 for the same float, as the blocks above show. A `decimal` value, such as `2.675m`, holds 2.675 exactly, so the result follows only the mode.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `half-even-up`, `half-zero`, `half-negative`, `half-negative-small`: halves where the rule matters. Half to even gives 2, 0, -2 and 0; half away from zero gives 3, 1, -3 and -1.
- `half-odd-up`: 1.5 rounds to 2 with both rules.
- `negative-below-half`: -0.4 rounds to 0, not to negative zero. A float function such as `Math.round` can give -0.
- `near-half`: 0.49999999999999994 is the largest float below 0.5. It rounds to 0.
- `large-odd`: 4503599627370497 is 2 to the power 52, plus 1. It is already an integer.
- `large-half-negative`: -1000000000000000.5 is exact in a float, and it rounds to -1000000000000001.
- `zero`: 0.0 rounds to 0.

## Pitfalls

- **`floor(x + 0.5)` is not a correct rounding.** The sum `x + 0.5` is a float, and it can round up. For 0.49999999999999994, the sum rounds to 1.0. For 4503599627370497, the sum is halfway between two floats and rounds to 4503599627370498. Both results are wrong:

```python wrong id=python-floor-half
import math

for x in [0.49999999999999994, 4503599627370497.0]:
    print(x, math.floor(x + 0.5), round(x))
```

```output for=python-floor-half
0.49999999999999994 1 0
4503599627370497.0 4503599627370498 4503599627370497
```

- **The decimal text is not the value.** The Python manual says that `round(2.675, 2)` "gives 2.67 instead of the expected 2.68"[^2], and "This is not a bug"[^2]. The float nearest to 2.675 is a little smaller than 2.675. MDN gives the same example for `toFixed`: `(2.55).toFixed(1)` is `'2.5'`[^6]. For exact decimal rounding, use a decimal type, such as `decimal` in C#, `BigDecimal` in Java, or the `decimal` module in Python.

- **A rounded float is still a float.** `Math.round` in JavaScript, `round` in C and `math.Round` in Go return a float. Convert the result to an integer type only when it fits; a float can be much larger than the largest 64-bit integer. See [[convert-between-numeric-types]].

- **Negative zero.** In JavaScript, `Math.round(-0.4)` is `-0`, and a float function in other languages can also give -0. It is equal to 0, but it prints as `-0` in some languages. See [[ieee-754]].

- **Rounding money.** A rule for money can ask for a specific rule for a half. Do not depend on the default of the library; give the mode in the call. See [[represent-money]].

## Related tasks

- [[do-integer-arithmetic]]: integer division, which rounds toward zero in most languages.
- [[compare-floating-point-numbers]]: compare floats with a tolerance.
- [[format-a-number]]: print a number with a fixed number of decimal places.
- [[convert-between-numeric-types]]: convert a float to an integer type.

See also the concepts [[rounding-modes]], [[floating-point-arithmetic]], [[ieee-754]] and [[decimal-arithmetic]].

## References
