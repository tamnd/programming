---
tag: 6JA921
kind: concept
slug: rational-numbers-in-code
name: Rational numbers in code
aliases: [rational number type, rational data type, fraction type, ratio, Fraction, Rational, BigRational, exact fractions]
area: foundations/numbers-in-machines
level: advanced
summary: >-
  A rational type stores a fraction as two integers in lowest terms, so 1/3 and 1/10 are exact. The cost is that the two integers can grow large during a long calculation.
applies_to: core-languages
viz: optional
formal: none
see_also: [arbitrary-precision-arithmetic, floating-point-arithmetic, integer-division-and-modulo, integer-overflow, integer-representation]
same_as:
  wikidata: Q7295757
  wikipedia: Rational_data_type
sources:
  - url: https://en.wikipedia.org/wiki/Rational_data_type
    title: Rational data type, Wikipedia
    locator: the introduction; Representation; Language support; Common Lisp
    accessed: 2026-11-16
  - url: https://www.lispworks.com/documentation/HyperSpec/Body/t_ratio.htm
    title: "System Class RATIO, Common Lisp HyperSpec"
    locator: Description
    accessed: 2026-11-16
  - url: https://hackage.haskell.org/package/base/docs/Data-Ratio.html
    title: Data.Ratio, Haskell base package documentation
    locator: Ratio; Rational; numerator
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/fractions.html
    title: fractions, Rational numbers, The Python Standard Library
    locator: class fractions.Fraction; limit_denominator; from_float
    accessed: 2026-11-16
  - url: https://docs.rs/num-rational/latest/num_rational/
    title: num_rational crate documentation, docs.rs
    locator: the crate description
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math/big
    title: big package, math/big, Go standard library
    locator: type Rat; SetFloat64; FloatString; Quo
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/GNU_Multiple_Precision_Arithmetic_Library
    title: GNU Multiple Precision Arithmetic Library, Wikipedia
    locator: the introduction; C++ library interface
    accessed: 2026-11-16
---

## Idea

A binary floating-point number cannot hold 1/10 or 1/3 exactly. A rational type can. Wikipedia says such a type represents "rational numbers like 1/3 and −11/17 without rounding"[^1], and the value "is usually represented as a fraction m/n where m and n are two integer numbers, either with a fixed or arbitrary precision"[^1].

Most rational types keep the fraction in a normal form: the numerator and the denominator have no common factor, and the denominator is positive. Then each value has one representation, so equality is a compare of the two integers. Each operation computes the new numerator and denominator with integer arithmetic, then divides both by their greatest common divisor (GCD).

```python run id=normal
from math import gcd


def make(n, d):
    if d == 0:
        raise ZeroDivisionError("denominator is 0")
    if d < 0:
        n, d = -n, -d
    g = gcd(n, d)
    return n // g, d // g


def add(a, b):
    return make(a[0] * b[1] + b[0] * a[1], a[1] * b[1])


def mul(a, b):
    return make(a[0] * b[0], a[1] * b[1])


print("make(6, -8):", make(6, -8))
print("1/10 + 2/10:", add(make(1, 10), make(2, 10)))
print("0.1 + 0.2 == 0.3:", "yes" if 0.1 + 0.2 == 0.3 else "no")
same = add(make(1, 10), make(2, 10)) == make(3, 10)
print("1/10 + 2/10 == 3/10:", "yes" if same else "no")
print("3/4 * 16:", mul(make(3, 4), make(16, 1)))
h = make(0, 1)
for k in range(1, 31):
    h = add(h, make(1, k))
    if k in (10, 20, 30):
        print(f"1/1 + ... + 1/{k} = {h[0]}/{h[1]}")
```

```output for=normal
make(6, -8): (-3, 4)
1/10 + 2/10: (3, 10)
0.1 + 0.2 == 0.3: no
1/10 + 2/10 == 3/10: yes
3/4 * 16: (12, 1)
1/1 + ... + 1/10 = 7381/2520
1/1 + ... + 1/20 = 55835135/15519504
1/1 + ... + 1/30 = 9304682830147/2329089562800
```

The last three lines show the main cost. The sum of 30 small fractions has a denominator of 13 digits, and the parts get longer as the sum goes on.

## How it works

**Normal form.** Common Lisp defines its `ratio` type in this form: "A ratio is a number representing the mathematical ratio of two non-zero integers, the numerator and denominator, whose greatest common divisor is one, and of which the denominator is positive and greater than one."[^2] So `(/ 6 8)` gives `3/4`, and `(* 3/4 16)` gives the integer `12`[^1]. Haskell's `numerator` and `denominator` give the parts "in reduced form: the numerator and denominator have no common factor and the denominator is positive"[^3]. Python's `Fraction(16, -10)` gives `Fraction(-8, 5)`[^4], and the documentation says: "The math.gcd() function is now used to normalize the numerator and denominator."[^4]

**The width of the parts.** If the numerator and the denominator are bignums, the rational type is exact for all four operations, except division by zero. If they are fixed-width integers, the parts can overflow. Haskell's `Rational` is "Arbitrary-precision rational numbers, represented as a ratio of two Integer values"[^3], but the general `Ratio a` type works with any integral type, and its instances "inherit the deficiencies from the type parameter's"[^3]. The Rust crate `num-rational` has both kinds: `BigRational` is an "Alias for arbitrary precision rationals"[^5], and `Rational64` is an "Alias for a Ratio of 64-bit-sized integers"[^5].

**Conversion from a float.** A float has an exact binary value, so a conversion to a rational type gives that exact value, not the decimal value in the source code. Python's documentation says that "Fraction(1.1) does not return Fraction(11, 10) as one might expect"[^4]. It gives `Fraction(2476979795053773, 2251799813685248)`[^4]. A string such as `'1.1'` gives `11/10`. Go's `Rat.SetFloat64` also "sets z to exactly f"[^6]. To get a simple fraction back, Python has `limit_denominator`, which "Finds and returns the closest Fraction to self that has denominator at most max_denominator"[^4].

**Conversion to text and floats.** Go's `FloatString` gives a decimal string with a set number of digits, and "The last digit is rounded to nearest, with halves rounded away from zero."[^6] A conversion to a float rounds again.

## In languages

| Language | Verdict | Rational type |
|---|---|---|
| Python | stdlib | `fractions.Fraction` |
| JavaScript | idiom | none; a pair of `bigint` values |
| TypeScript | idiom | as JavaScript |
| Java | library | none in the JDK; Apache Commons Math `Fraction` |
| Kotlin | idiom | none; a pair of `BigInteger` values |
| C# | idiom | none; a pair of `BigInteger` values |
| Go | stdlib | `math/big` `Rat` |
| Rust | library | the `num-rational` crate |
| C | library | GMP `mpq_t` |
| C++ | library | GMP `mpq_class`; `std::ratio` at compile time only |
| Bash | absent | integer arithmetic only |
| PowerShell | idiom | none; a pair of `[bigint]` values |
| SQL | absent | no rational type; `NUMERIC` is decimal |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | n/a | no rational type |

Wikipedia lists the languages with a rational type in the language or the core library: C++ has had "compile-time rational arithmetic"[^1] in the `<ratio>` header since 2011, Go has rationals in `math/big`, Haskell has `Rational`, Python has `Fraction`, and Common Lisp, Clojure, Julia, Ruby, Raku and Smalltalk have rationals too[^1]. For C and C++ there is GMP, and "The Apache Commons Math library provides rational numbers for Java with its Fraction class."[^1] GMP operates "on signed integers, rational numbers, and floating-point numbers"[^7], and its C++ class for rationals is `mpq_class`[^7].

## Variations and related ideas

- **Rational literals.** Some languages have a syntax for rational values. In J, "1r3 is one-third"[^1]. Julia uses `//`, as in `6//9 == 2//3`[^1], Haskell uses `%`[^3], and in Common Lisp `1/3` is a literal of type `ratio`[^1].
- **Integer division that gives a rational.** In Common Lisp, `(/ 6 8)` gives `3/4`[^1], so the division of two integers is exact. Most languages give a truncated or floored integer, or a float. See [[integer-division-and-modulo]].
- **Limited-precision rationals.** By default, Raku uses its `Rat` type, which has "rational numbers with limited-precision"[^1], and its `FatRat` type "implements arbitrary-precision rational numbers"[^1].
- **Decimal types.** A decimal type is exact for 0.1 but not for 1/3. See [[floating-point-arithmetic]].

## Pitfalls

- **Growth of the parts.** A loop that adds or multiplies many fractions can make the numerator and the denominator grow at each step, as the first block shows. Each operation then gets slower. Round to a float, or use `limit_denominator`, when an exact result is not necessary.
- **Overflow with fixed-width parts.** A rational type on 64-bit integers can overflow in the product of two denominators, before the GCD reduces it. The Rust example shows this.
- **A float in, a float's value out.** `Fraction(0.1)` is not 1/10. Python warns that "Fraction.from_float(0.3) is not the same value as Fraction(3, 10)"[^4]. Build a rational from integers or from a string.
- **Division by zero.** A zero denominator is an error. Python raises `ZeroDivisionError`[^4], and Go's `Quo` panics "If y == 0"[^6].
- **A rational type does not make a square root exact.** The square root of 2 and π are not rational. An operation that leaves the rational numbers, such as a square root, gives a float or an approximation.

## Cost

Each addition needs some multiplications of the parts and a GCD to normalize the result, so a rational operation costs many integer operations. With bignum parts, the cost also grows with the length of the parts. See [[arbitrary-precision-arithmetic]]. Wikipedia says that the operations "may be translated by the compiler into a sequence of integer machine instructions, or into library calls"[^1].

## History

Common Lisp has the `ratio` type in its standard[^2]. C++ added compile-time rational arithmetic in the `<ratio>` header in its 2011 revision[^1]. Python's `Fraction` constructor accepts `float` and `decimal.Decimal` values since version 3.2[^4].

## References
