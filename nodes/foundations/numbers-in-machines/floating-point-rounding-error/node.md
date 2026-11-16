---
tag: JAXVK5
kind: concept
slug: floating-point-rounding-error
name: Floating-point rounding error
aliases: [rounding error, round-off error, roundoff error, representation error, why 0.1 + 0.2 is not 0.3]
area: foundations/numbers-in-machines
level: core
summary: >-
  A float holds the nearest binary fraction, not the decimal you wrote, and each operation rounds again. The errors are small, but they can add up or be exposed by a subtraction.
applies_to: core-languages
viz: optional
formal: none
see_also: [floating-point-arithmetic, fixed-point-arithmetic, rational-numbers-in-code, arbitrary-precision-arithmetic, integer-representation, type-conversion]
same_as:
  wikidata: Q2145211
  wikipedia: Round-off_error
sources:
  - url: https://docs.python.org/3/tutorial/floatingpoint.html
    title: The Python Tutorial, 15. Floating-Point Arithmetic, Issues and Limitations, Python Software Foundation
    accessed: 2026-11-16
    locator: the opening section, and 15.1 Representation Error
  - url: https://en.wikipedia.org/wiki/Round-off_error
    title: Round-off error, Wikipedia
    accessed: 2026-11-16
    locator: the lead, Representation error, Accumulation of roundoff error, and Ill-conditioned problems
  - url: https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
    title: David Goldberg, What Every Computer Scientist Should Know About Floating-Point Arithmetic, ACM Computing Surveys 23(1), March 1991 (edited reprint by Sun Microsystems)
    accessed: 2026-11-16
    locator: the sections Rounding Error, Relative Error and Ulps, and Cancellation
  - url: https://en.wikipedia.org/wiki/Kahan_summation_algorithm
    title: Kahan summation algorithm, Wikipedia
    accessed: 2026-11-16
    locator: the lead, and The algorithm
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/math/BigDecimal.html
    title: BigDecimal (Java SE 27 and JDK 27), Oracle
    accessed: 2026-11-16
    locator: the constructor BigDecimal(double), API Note
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.decimal
    title: Decimal Struct, Microsoft Learn
    accessed: 2026-11-16
    locator: Remarks
  - url: https://docs.python.org/3/library/decimal.html
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python Software Foundation
    accessed: 2026-11-16
    locator: the list of advantages at the top of the page
  - url: https://en.wikipedia.org/wiki/MIM-104_Patriot
    title: MIM-104 Patriot, Wikipedia
    accessed: 2026-11-16
    locator: Failure at Dhahran
---

## Idea

You type `0.1`. The program does not store one tenth. It stores the binary fraction that is nearest to one tenth, because "most decimal fractions cannot be represented exactly as binary fractions".[^1] For a binary64 float, that fraction is 3602879701896397 / 2^55.[^1]

```python run id=idea
from decimal import Decimal
from fractions import Fraction

print(Fraction(0.1))
print(Decimal(0.1))
print(Decimal(0.1 + 0.2))
print(Decimal(0.3))
print(0.1 + 0.2 == 0.3)
```

```output
3602879701896397/36028797018963968
0.1000000000000000055511151231257827021181583404541015625
0.3000000000000000444089209850062616169452667236328125
0.299999999999999988897769753748434595763683319091796875
False
```

`Fraction` and `Decimal` show the exact value of each float. The stored 0.1 is a little more than one tenth, and the sum of 0.1 and 0.2 is a different float from the one nearest to 0.3. Neither number is wrong. Each one is the nearest float to its exact value.

A rounding error is the difference between the result of exact arithmetic and the result of the same steps with rounded arithmetic.[^2] It has two causes. The input can have no exact form (representation error), and each operation rounds its result. One error is very small. The problems come when errors add up, or when a subtraction removes the correct digits and leaves only the error.

## How it works

### Representation error

A binary64 float is an integer of 53 bits times a power of two.[^1] A decimal fraction such as 0.1 has an infinite binary expansion, `0.000110011001100...`, as one third has an infinite decimal expansion. The machine cuts it off and rounds.[^1] The Round-off error article calls this "a form of roundoff error called representation error".[^2]

Representation error happens once, when the value enters the program: in a literal, in a parse of text, or in a conversion from a decimal type. The program cannot get it back later, and rounding the input again does not help. The Python tutorial shows that `round(0.1, 1) + round(0.1, 1) + round(0.1, 1) == round(0.3, 1)` is false.[^1]

### The error of one operation

IEEE 754 requires that the basic operations round the exact result to a float. With the default rounding, the error is at most half a unit in the last place (ulp).[^3] The Python tutorial gives the same bound in other words: on most machines the error of one operation is no more than 1 part in 2^53.[^1] Goldberg measures error in two ways: in ulps, and as relative error, "the difference between the two numbers divided by the real number".[^3]

### Accumulated error

Each step of a long computation adds its own small error. "When a sequence of calculations with an input involving any roundoff error are made, errors may accumulate, sometimes dominating the calculation."[^2] A loop that adds 0.1 ten times does not give 1.0.

```python run id=accumulate
import math
from fractions import Fraction

total = 0.0
for _ in range(10):
    total += 0.1
print("loop:", total)
print("math.fsum:", math.fsum([0.1] * 10))

values = [1e16, 1.0, -1e16]
print("loop over", values, "->", values[0] + values[1] + values[2])
print("math.fsum:", math.fsum(values))
print("exact:", float(sum(map(Fraction, values))))
```

```output
loop: 0.9999999999999999
math.fsum: 1.0
loop over [1e+16, 1.0, -1e+16] -> 0.0
math.fsum: 1.0
exact: 1.0
```

`math.fsum` tracks the digits that each addition loses, so the result has only one rounding.[^1] Kahan summation does a similar job with one extra variable. For plain summation the worst-case error "grows proportional to n", while with a good compensation the bound is "effectively independent of n".[^4]

### Cancellation

A subtraction of two close numbers is exact, but the digits that the numbers share cancel. If those numbers already had rounding errors, the result shows mostly the errors. Goldberg calls this catastrophic cancellation, and for one example he writes: "The subtraction did not introduce any error, but rather exposed the error introduced in the earlier multiplications."[^3] A subtraction of exact values is benign cancellation.[^3]

```python run id=cancel
import math

x = 1e-10
print("(1 + x) - 1:", (1 + x) - 1)
print("exact:      ", x)
print("log(1 + x): ", math.log(1 + x))
print("log1p(x):   ", math.log1p(x))
```

```output
(1 + x) - 1: 1.000000082740371e-10
exact:       1e-10
log(1 + x):  1.000000082690371e-10
log1p(x):    9.999999999500001e-11
```

`1 + x` rounds away most of the digits of `x`, and the subtraction shows what is left. Only 7 or 8 digits of the result are correct. `log1p` computes `log(1 + x)` without the cancellation.

Goldberg's rule is to change the formula: `x*x - y*y` has a catastrophic cancellation, and `(x - y) * (x + y)` has only a benign one.[^3]

### Ill-conditioned problems

Some problems magnify any small error in the input, whatever the algorithm. The condition number is "the ratio of the relative change in the solution to the relative change in the input".[^2] When it is large, a correct algorithm still gives a result with few correct digits.[^2]

## In languages

| Language | Verdict | Form |
|---|---|---|
| Python | native | `float` is binary64; `decimal.Decimal(x)` and `fractions.Fraction(x)` show the exact value; `math.fsum` |
| JavaScript | native | every `number` is binary64; `toPrecision` shows more digits |
| TypeScript | native | as JavaScript |
| Java | native | `double`; `new BigDecimal(x)` shows the exact value; `BigDecimal` for decimal work |
| C | native | `double`; `printf("%.55f")` shows the exact value on glibc |
| C++ | native | as C; `std::setprecision` |
| C# | native | `double`; `decimal` for decimal work |
| Go | native | `float64`; `%.55f`; `math/big` for exact work |
| Rust | native | `f64`; `{:.55}` |
| Kotlin | native | `Double` of the JVM; `BigDecimal` |
| PowerShell | native | `[double]`; `[decimal]` |
| Bash | absent | integer arithmetic only |
| SQL | native | `REAL` and `DOUBLE PRECISION` round; `NUMERIC` is exact |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | n/a | no arithmetic on the page in this sense |

Every tier 1 language with floats uses the same binary formats on common hardware, so the same sums give the same errors. The languages differ in how much of the error they show and in the exact types they offer.

- **Java** shows the exact value through `BigDecimal(double)`. Its documentation warns that "The results of this constructor can be somewhat unpredictable", because `new BigDecimal(0.1)` is "actually equal to 0.1000000000000000055511151231257827021181583404541015625".[^5] The Java example uses `BigDecimal.valueOf(0.1)` and the string constructor for exact work.
- **C#** has `decimal`. Microsoft says it "is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors".[^6] The same page adds that it "does not eliminate the need for rounding".[^6]
- **Python** has `decimal`, where "Decimal numbers can be represented exactly", so `0.1 + 0.1 + 0.1 - 0.3` is exactly zero.[^7]
- **C, C++, Go and Rust** print the exact value when you ask for enough digits, as the examples do with 55 digits after the point.
- **Bash** has no floats, so this error does not occur. The other errors of integer arithmetic do ([[integer-division-and-modulo]]).

## Variations and related ideas

- **Decimal floating point.** A decimal type stores base 10 digits, so 0.1 is exact. It still rounds 1/3.[^7]
- **Fixed point and integer cents.** An amount kept as an integer number of cents adds without error ([[fixed-point-arithmetic]]).
- **Rational numbers.** A fraction type is exact for `+`, `-`, `*` and `/`, but its parts can grow ([[rational-numbers-in-code]]).
- **Arbitrary precision.** A float type with more bits makes each error smaller, but it does not remove representation error for 0.1 ([[arbitrary-precision-arithmetic]]).
- **Truncation error.** Numerical analysis counts two kinds of computation error: truncation errors and roundoff errors.[^2] This page is about the second kind.
- **Related pages.** [[floating-point-arithmetic]] describes the format, the special values and the ulp.

## Pitfalls

### The printed value hides the error

Python prints the shortest string that reads back as the same float, so `0.1` prints as `0.1`. The Python tutorial calls this "in a real sense, an illusion", and it says that other languages "may not display the difference by default".[^1] Print more digits, or the exact value, when you debug a numeric result.

### Rounding the inputs does not help

The nearest float to 0.1 is already as close as a float can be. Rounding it to one decimal gives the same float.[^1] Round the output for display, and compare with a tolerance.

### Rounding twice

A value rounded to two decimals and then to one can differ from the value rounded once. The Round-off error article gives 9.945309: rounded to 9.95 and then to 10.0, the error is 0.054691, and rounded once to 9.9, the error is 0.045309.[^2] The same thing can happen when a program computes in the x87 80-bit format and stores the result as binary64.[^2]

### Errors that grow with time

On February 25, 1991, a Patriot battery at Dhahran failed to intercept a Scud missile, and 28 soldiers were killed.[^8] The system had run for 100 hours, and its clock "had drifted by one-third of a second", the result of "truncation in a 24-bit fixed-point register".[^8] The error was small for each tick of the clock, but it grew with the run time. Keep time as an integer count of ticks, and convert it to seconds only at the end.

### A float for money

The sum of many prices drifts by a cent now and then. Keep money in a decimal type or in integer cents ([[fixed-point-arithmetic]]).

## Cost

- **Exact types are slower.** `decimal.Decimal`, `BigDecimal`, `Fraction` and `big.Rat` work in software. They are much slower than a float, which is a single hardware instruction for each operation.
- **Compensated sums.** Kahan summation does four float operations for each term instead of one.[^4] The Python tutorial says that `math.fsum` is slower than `sum`.[^1]
- **More digits.** A wider float, such as `long double`, makes each error smaller, and each value takes more memory.

## History

- **1991.** David Goldberg's survey explained ulps, relative error, and catastrophic and benign cancellation.[^3]
- **1991.** On February 25, a Patriot battery at Dhahran missed a Scud missile because its clock had drifted after 100 hours of running.[^8]

## References
