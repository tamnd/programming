---
tag: 7YCVZD
kind: concept
slug: integer-division-and-modulo
name: Integer division and modulo
aliases: [integer division, modulo, modulo operation, mod, remainder, rem, truncated division, floored division, Euclidean division, floor division, divmod]
area: foundations/numbers-in-machines
level: core
summary: >-
  Integer division gives a whole quotient and a remainder. Languages round the quotient toward zero, toward minus infinity or by the Euclidean rule, so the sign of the remainder differs.
applies_to: core-languages
viz: optional
formal: none
see_also: [integer-representation, overflow-semantics, twos-complement, signed-and-unsigned-integers, floating-point-arithmetic]
same_as:
  wikidata: Q1799665
  wikipedia: Modulo
sources:
  - url: https://en.wikipedia.org/wiki/Modulo
    title: Modulo, Wikipedia
    locator: the introduction; Variants of the definition; Common pitfalls; In programming languages
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/operator_arithmetic
    title: Arithmetic operators, C reference, cppreference.com
    locator: Multiplicative operators; Remainder
    accessed: 2026-11-16
  - url: https://docs.python.org/3/reference/expressions.html
    title: Expressions, The Python Language Reference
    locator: 6.7 Binary arithmetic operations
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/language/operator_arithmetic
    title: Arithmetic operators, C++ reference, cppreference.com
    locator: Multiplicative operators
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-15.html
    title: "The Java Language Specification, Java SE 27 Edition, Chapter 15: Expressions"
    locator: 15.17.2 Division Operator /; 15.17.3 Remainder Operator %
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Integer operators
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
    title: Arithmetic operators, C# reference, Microsoft Learn
    locator: Remainder operator %, Integer remainder
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html
    title: Operator expressions, The Rust Reference
    locator: Arithmetic and logical binary operators
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Remainder
    title: Remainder (%), MDN Web Docs
    locator: the introduction; Description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math, Java SE 27 API
    locator: floorDiv; floorMod
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/CSS/mod
    title: mod(), CSS, MDN Web Docs
    locator: the introduction
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i32.html
    title: i32, Rust standard library
    locator: div_euclid; rem_euclid
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_arithmetic_operators
    title: about_Arithmetic_Operators, PowerShell documentation, Microsoft Learn
    locator: Division and rounding
    accessed: 2026-11-16
---

## Idea

Integer division of *a* by *n* gives two integers: a quotient *q* and a remainder *r*. They satisfy *a* = *n* × *q* + *r*, and the size of *r* is less than the size of *n*. When *a* and *n* are both positive, there is one answer: 7 divided by 3 is 2 with remainder 1. When one of them is negative, there are two answers. -7 divided by 3 can be -2 with remainder -1, or -3 with remainder 2. Each language picks one rule.

Wikipedia lists the main rules[^1]:

- **Truncated division** rounds the quotient toward zero. "the remainder has the same sign as the dividend"[^1]. C, C++, Java, JavaScript, C#, Go and Rust use it for `/` and `%`.
- **Floored division** rounds the quotient down, toward minus infinity. Donald Knuth "promotes floored division"[^1], and with it "the remainder has the same sign as the divisor"[^1]. Python uses it for `//` and `%`.
- **Euclidean division** picks the quotient that makes the remainder 0 or more. Raymond T. Boute "promotes Euclidean division"[^1]. Rust has it in `div_euclid` and `rem_euclid`.

```python run id=rules
def truncated(a, n):
    q = abs(a) // abs(n)
    if (a < 0) != (n < 0):
        q = -q
    return q, a - n * q


def floored(a, n):
    return a // n, a % n


def euclidean(a, n):
    r = a % abs(n)
    return (a - r) // n, r


for a, n in ((7, 3), (-7, 3), (7, -3), (-7, -3)):
    qt, rt = truncated(a, n)
    qf, rf = floored(a, n)
    qe, re = euclidean(a, n)
    print(f"{a} / {n}: truncated {qt} r {rt}, floored {qf} r {rf}, "
          f"Euclidean {qe} r {re}")
```

```output for=rules
7 / 3: truncated 2 r 1, floored 2 r 1, Euclidean 2 r 1
-7 / 3: truncated -2 r -1, floored -3 r 2, Euclidean -3 r 2
7 / -3: truncated -2 r 1, floored -3 r -2, Euclidean -2 r 1
-7 / -3: truncated 2 r -1, floored 2 r -1, Euclidean 3 r 2
```

The three rules agree when the dividend is 0 or more and the divisor is positive. They differ only when a sign is negative.

## How it works

**The identity.** Each language keeps the quotient and the remainder consistent. In C, "if the quotient a/b is representable in the result type, then (a/b)*b + a%b == a"[^2]. In Python, "The floor division and modulo operators are connected by the following identity: x == (x//y)*y + (x%y)"[^3]. So the rule for the quotient fixes the sign of the remainder.

**Truncated division.** C99 fixed the direction of `/`: before C99 the quotient was "rounded in implementation-defined direction", and since C99 it is "truncated towards zero"[^2]. In C++, "the quotient is truncated towards zero (fractional part is discarded)"[^4]. Java says: "Integer division rounds toward 0"[^5], and the remainder "can be negative only if the dividend is negative"[^5]. Go uses the name "truncated division" in its specification and gives a table: -5 / 3 is -1 and -5 % 3 is -2[^6]. In C#, "The sign of the non-zero remainder matches the sign of the left-hand operand"[^7]. Rust says: "Rust uses a remainder defined with truncating division."[^8]

**JavaScript.** JavaScript numbers are floats, so `/` does not give a whole quotient. A program uses `Math.trunc(a / b)` or `Math.floor(a / b)`. The `%` operator "always takes the sign of the dividend"[^9]. MDN says: "Note that while in most languages, '%' is a remainder operator, in some (e.g., Python, Perl) it is a modulo operator."[^9]

**Floored division.** Python's `//` applies the floor function to the result of the mathematical division[^3], and "The modulo operator always yields a result with the same sign as its second operand (or zero)"[^3]. Java has `Math.floorDiv` and `Math.floorMod`. The floor modulus "has the same sign as the divisor y or is zero"[^10]. CSS `mod()` is floored too: "It always takes the sign of the divisor."[^11]

**Euclidean division.** The remainder is never negative. Rust's `rem_euclid` "Calculates the least nonnegative remainder of self when divided by rhs"[^12]. This rule is useful for an index into a ring of *n* slots, because the result is always a valid index.

**Division by zero and overflow.** Integer division by zero is an error in most languages. Java throws an `ArithmeticException` when the divisor of an integer division or remainder is 0[^5]. In C, "If the second operand is zero, the behavior is undefined."[^2] The division of the most negative value by -1 overflows. Go defines the result: the quotient is the dividend itself, and the remainder is 0, "due to two's-complement integer overflow"[^6]. See [[overflow-semantics]].

## In languages

| Language | Verdict | Quotient and remainder |
|---|---|---|
| Python | native | `//` and `%` are floored; `math.fmod` is truncated |
| JavaScript | idiom | `%` is truncated; no integer `/`, so `Math.trunc` or `Math.floor` |
| TypeScript | idiom | as JavaScript |
| Java | native | `/` and `%` are truncated; `Math.floorDiv` and `Math.floorMod` |
| Kotlin | native | `/`, `%` and `rem` are truncated; `mod` is floored |
| C# | native | `/` and `%` are truncated; `Math.DivRem` |
| Go | native | `/` and `%` are truncated; `big.Int` `Div` and `Mod` are Euclidean |
| Rust | native | `/` and `%` are truncated; `div_euclid` and `rem_euclid` |
| C | native | `/` and `%` are truncated since C99; `div` |
| C++ | native | `/` and `%` are truncated; `std::div` |
| Bash | native | `/` and `%` in `$(( ))`; the POSIX shell `%` is truncated |
| PowerShell | idiom | `/` can give a fraction; `%` is truncated; `[Math]::Floor` or `[Math]::Truncate` |
| SQL | native | `MOD(x, y)` is truncated |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | native | `mod()` is floored and `rem()` is truncated |

Wikipedia keeps a long table of the modulo operators of many languages, with the rule of each one[^1]. In it, Kotlin's `%` and `rem` are truncated and `mod` is floored, PowerShell's `%` is truncated, the `%` of ksh is truncated "same as POSIX sh", and SQL `mod(x,y)` is truncated[^1]. PowerShell's `/` gives a fraction when the division is not exact, and a cast of the result to `[int]` "rounds to the nearest even integer"[^13].

## Variations and related ideas

- **Rounded division.** "Common Lisp and IEEE 754 use rounded division"[^1], where the quotient is rounded to the nearest integer. Python's `math.remainder` follows that rule[^1].
- **Ceiling division.** "Common Lisp also uses ceiling division"[^1]. A program that needs the number of pages for *k* items, *p* to a page, computes the ceiling of *k* / *p*, often as `(k + p - 1) / p` for positive values.
- **Floating-point remainder.** Python's `math.fmod` is truncated, and Python's `%` on floats is floored[^1].
- **Unsigned division.** For unsigned values, the three rules give the same result.

## Pitfalls

- **An odd test with `%`.** Wikipedia: "to test if an integer is odd, one might be inclined to test if the remainder by 2 is equal to 1"[^1]. With truncated division, "when n (the dividend) is negative and odd, n mod 2 returns −1, and the function returns false"[^1]. Test `n % 2 != 0` instead.
- **A negative index.** `i % n` can be negative in C, Java, JavaScript, C#, Go and Rust when `i` is negative. Use a floored or Euclidean remainder, such as `Math.floorMod` or `rem_euclid`, to get an index from 0 to *n* - 1.
- **Precedence in Rust.** `-7i32.rem_euclid(3)` is `-(7i32.rem_euclid(3))`, which is -1, because a method call binds more tightly than the unary minus. The Rust example shows this. Write `(-7i32).rem_euclid(3)`.
- **Porting between languages.** Code that moves from Python to C or Java, or back, changes its results for negative values without an error.

## Cost

The truncated quotient and remainder come from the division of the hardware. A floored or Euclidean result needs one compare and one correction after that division, as the helper functions in the C, C++, C# and JavaScript examples show. For unsigned values, a division by a power of 2 is a right shift, and the remainder is a mask: `x % 8` is `x & 7`.

## History

C left the direction of signed division to the implementation until C99, which fixed it as truncation toward zero[^2]. Knuth's *The Art of Computer Programming* promotes floored division[^1], and Boute's 1992 paper "The Euclidean definition of the functions div and mod" promotes the Euclidean rule[^1].

## References
