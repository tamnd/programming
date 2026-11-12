---
tag: VTXJS0
kind: concept
slug: numeric-type
name: Numeric type
aliases: [number type, numeric types, arithmetic type, integer type, floating-point type, decimal type, complex type, bignum, numerical tower]
area: data-and-values/primitive-types
level: core
summary: >-
  A numeric type holds numbers of one kind, such as integers, floating-point numbers, decimals or complex numbers. Its range, its precision and its rules for division and mixing differ by language.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [John Backus]
  note: The first FORTRAN manual had two types of number, fixed point for integers and floating point, and the first letter of a variable name chose the type. Later languages added more widths, unsigned types, decimals, complex numbers and integers with no limit.
see_also: [integer-representation, floating-point-arithmetic, type-conversion, integer-overflow, boolean-type]
sources:
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Numeric types
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: Numeric Types - int, float, complex
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: 4.2 Primitive Types and Values; 4.2.2 Integer Operations
- url: https://en.cppreference.com/w/c/language/arithmetic_types.html
  title: Arithmetic types, C reference, cppreference.com
  locator: Integer types; Data models
- url: https://doc.rust-lang.org/reference/types/numeric.html
  title: Numeric types, The Rust Reference
  locator: Machine-dependent integer types
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/BigInt
  title: BigInt, MDN Web Docs
  locator: Description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number
  title: Number, MDN Web Docs
  locator: Number encoding
- url: https://en.wikipedia.org/wiki/Numerical_tower
  title: Numerical tower, Wikipedia
  locator: Introduction; In Scheme
- url: https://docs.python.org/3/library/numbers.html
  title: numbers, Numeric abstract base classes, Python documentation
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/floating-point-numeric-types
  title: Floating-point numeric types, C# language reference
  locator: Characteristics of the floating-point types
- url: https://archive.computerhistory.org/resources/text/Fortran/102649787.05.01.acc.pdf
  title: The FORTRAN Automatic Coding System for the IBM 704 EDPM, Programmer's Reference Manual, 1956
  locator: Chapter 2, Constants and Variables
- url: https://en.wikipedia.org/wiki/Integer_(computer_science)
  title: Integer (computer science), Wikipedia
  locator: Introduction
---

## Idea

A program counts things, measures things and computes money. Each of these needs numbers, but a computer has no single kind of number. A numeric type is one kind of number that a language gives: an integer type for whole numbers, a floating-point type for approximate real numbers, and in some languages decimal, rational, complex and unlimited integer types. The type decides which values exist, how exact they are, and what `/` does.

```python run id=idea
from decimal import Decimal
from fractions import Fraction

# Python has three built-in numeric types, and the library adds two more.
for v in (7, 7.5, 7 + 2j, Fraction(15, 2), Decimal("7.5")):
    print(type(v).__name__, v)

# Mixed arithmetic widens the narrower operand: int to float to complex.
print(type(1 + 2.0).__name__, type(2.0 + 1j).__name__)

# / always gives a float, and // gives the floor.
print(7 / 2, 7 // 2, -7 // 2, 7.0 // 2)

# An int has no fixed width, but a float keeps only 53 bits.
print(2**64, float(2**53 + 1) == 2**53)

# A binary float cannot hold 0.1 exactly, but Decimal and Fraction can.
print(0.1 + 0.2, Decimal("0.1") + Decimal("0.2"), Fraction(1, 10) + Fraction(2, 10))
```

```output for=idea
int 7
float 7.5
complex (7+2j)
Fraction 15/2
Decimal 7.5
float complex
3.5 3 -4 3.0
18446744073709551616 True
0.30000000000000004 0.3 3/10
```

The same value 7 or 7.5 has a different type in each line, and the type controls the result. Adding an `int` to a `float` gives a `float`. `/` gives a `float` even for two integers, and `//` rounds down, so `-7 // 2` is `-4`. The `int` 2 to the power 64 is exact, but a `float` cannot tell 2 to the power 53 from the next integer. `0.1 + 0.2` is not exactly `0.3` in binary floating point, but it is in `Decimal` and `Fraction`.

## How it works

**Kinds of numeric type.** The Go specification says that "An integer, floating-point, or complex type represents the set of integer, floating-point, or complex values, respectively. They are collectively called numeric types"[^1]. Python has "three distinct numeric types: integers, floating-point numbers, and complex numbers"[^2], and its library adds rationals and decimals. Java has "the integral types byte, short, int, long, and char, and the floating-point types float and double"[^3]. The main kinds are:

- **Fixed-width integers**, such as `int32` or `u8`. They are exact, but they hold only a range of values. See [integer representation](/integer-representation/).
- **Floating-point numbers**, such as `double` or `f64`. They cover a very large range, but they keep about 15 to 17 decimal digits. See [floating-point arithmetic](/floating-point-arithmetic/).
- **Unlimited integers**, such as Python `int`, JavaScript `BigInt`, Java `BigInteger` and Go `big.Int`. They grow as needed, so they do not overflow, but each operation costs more.
- **Decimal types**, such as C# `decimal`, Java `BigDecimal` and Python `Decimal`. They store base-10 digits, so `0.1` is exact.
- **Rationals and complex numbers**, such as Python `Fraction`, Go `big.Rat`, Go `complex128` and C `double complex`.

**Range and width.** A fixed-width type has a size that the language either fixes or leaves to the platform. Java, C#, Go and Rust fix the size of most types. C does not: `int` "is guaranteed to be at least 16 bits. Most current systems use 32 bits"[^4]. Each language also has a type with the size of a pointer for indexes and sizes. In Rust, "The usize type is an unsigned integer type with the same number of bits as the platform's pointer type"[^5].

**Literals.** A literal gets a type from its form. In most languages `7` is an integer and `7.0` is a floating-point number. A suffix chooses another type: `7L` in Java, `7u8` in Rust, `7n` for a JavaScript `BigInt` and `0.1m` for a C# `decimal`.

**Mixing types.** When an operator has two operands of different numeric types, the language either converts one of them or rejects the expression. Python "fully supports mixed arithmetic"[^2]: it widens the narrower operand from `int` to `float` to `complex`. C, C++, Java and C# have promotion rules, so `byte + byte` is an `int` in Java and `int + double` is a `double`. Go and Rust do not convert. In Go, "Explicit conversions are required when different numeric types are mixed in an expression or assignment"[^1]. A JavaScript `BigInt` "cannot be mixed with a Number value in operations; they must be coerced to the same type"[^6].

**Division.** Integer division is where the types differ most. C, C++, Java, C#, Go and Rust truncate toward zero, so `-7 / 2` is `-3` and `-7 % 2` is `-1`. Python `//` rounds toward minus infinity, so `-7 // 2` is `-4` and `-7 % 2` is `1`. JavaScript has no integer type for ordinary numbers: "The JavaScript Number type is a double-precision 64-bit binary format IEEE 754 value"[^7], so `7 / 2` is `3.5`. Integer division by zero is an error in most languages, but floating-point division by zero gives infinity or NaN.

**Overflow.** A fixed-width integer has a largest value. Java says that "The integer operators do not indicate overflow or underflow in any way"[^3], so `int` arithmetic wraps. Rust panics in a debug build, C# throws in a `checked` context, and signed overflow in C and C++ is undefined behavior. See [integer overflow](/integer-overflow/).

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `int` with no limit, `float`, `complex`; `Fraction` and `Decimal` in the library |
| JavaScript | native | `number`, a 64-bit float, and `bigint` for exact integers |
| TypeScript | native | `number` and `bigint`, the same values as JavaScript |
| Java | native | `byte`, `short`, `int`, `long`, `char`, `float`, `double`; `BigInteger` and `BigDecimal` |
| Kotlin | native | `Byte` to `Long`, unsigned `UInt` and `ULong`, `Float`, `Double`; no implicit widening |
| C# | native | signed and unsigned integers to 128 bits, `float`, `double`, `decimal`, `BigInteger` |
| Go | native | `int8` to `int64`, `uint8` to `uint64`, `float32`, `float64`, `complex128`; exact untyped constants |
| Rust | native | `i8` to `i128`, `u8` to `u128`, `isize`, `usize`, `f32`, `f64` |
| C | native | `char` to `long long` with platform sizes, `float`, `double`, `long double`, complex types |
| C++ | native | the types of C, `numeric_limits`, and concepts such as `std::integral` |
| Bash | idiom | integer arithmetic in `$(( ))` on 64-bit signed integers; no floating point without `bc` or `awk` |
| PowerShell | native | the .NET types `[int]`, `[long]`, `[double]`, `[decimal]` and `[bigint]` |
| SQL | native | `INTEGER`, `BIGINT`, `DECIMAL(p, s)`, `NUMERIC`, `REAL` and `DOUBLE PRECISION` |
| Regex | n/a | a pattern matches digits as text, but it has no numbers |
| HTML | idiom | `<input type="number">` accepts numbers, but the value is a string |
| CSS | idiom | properties take numbers, integers, lengths and percentages, and `calc()` computes with them |

Every general-purpose language has integer and floating-point types. They differ in whether integers have a limit, in the rules for mixing types, in the rounding of integer division, and in what happens on overflow.

The example files show these differences. Python shows the `numbers` tower, the rounding of `int()`, `//` and `round()`, and an exact 100 factorial. JavaScript shows that `number` is exact only up to 2 to the power 53, that it has no integer division, and that `BigInt` and `number` do not mix. Java shows truncating division, promotion of `byte` to `int`, an `int` product that wraps before it becomes a `long`, and `BigDecimal`. C# shows `decimal`, the fixed sizes and literal suffixes. Go shows that `int32` and `int` need a conversion, exact untyped constants, complex numbers and `math/big`. Rust shows the default types `i32` and `f64`, `div_euclid` and `checked_add`. C shows the platform sizes, promotion of `unsigned char` to `int` and a `float` that cannot hold 16777217. C++ shows a template limited to numeric types by a concept, `numeric_limits` and `std::midpoint`.

## Variations and related ideas

- **The numerical tower.** Scheme puts its numeric types in a hierarchy, so that "every integer is a rational with an implicit denominator of 1, and all reals are complex with an implicit imaginary part of 0"[^8]. A conversion up the tower is safe, because "coercing an integer to a rational loses no information"[^8]. The Python `numbers` module "defines a hierarchy of numeric abstract base classes which progressively define more operations"[^9].
- **Exact constants.** Go computes constant expressions exactly, so `1 << 100` is a valid constant and `0.1+0.2 == 0.3` is true when all three values are constants. The value gets a fixed type only where the program uses it.
- **Generic numeric code.** C++ concepts such as `std::integral`, Rust traits and C# generic math let one function work for many numeric types.
- **Units.** A number that has a unit, such as metres, can have its own type, so that a program cannot add metres to seconds.

## Pitfalls

- **Floats for money.** `0.1 + 0.2` is `0.30000000000000004` in binary floating point. Use a decimal type or an integer number of cents. C# says that "The decimal type is appropriate when the required degree of precision is determined by the number of digits to the right of the decimal point"[^10].
- **Overflow before widening.** In Java, `long x = big * big` multiplies two `int` values and wraps before the result becomes a `long`. Convert one operand first.
- **Negative division.** Code that ports from Python to C or Java gets a different result for `-7 / 2` and `-7 % 2`. Use `Math.floorDiv` in Java or `div_euclid` in Rust when the rounding must go down.
- **Large integers in JavaScript.** An integer above 2 to the power 53 loses digits in a `number`. IDs from a database or JSON can change value. Keep them as strings or as `BigInt`.
- **Platform sizes.** In C, `long` is 32 bits on 64-bit Windows and 64 bits on 64-bit Linux and macOS. Use the types of `<stdint.h>` when the size matters.
- **Integer division where a fraction is meant.** `7 / 2 * 2.0` is `6.0` in C, because `7 / 2` is the integer `3`.

## Cost

Fixed-width integer and floating-point operations map to machine instructions, so most of them take one or a few cycles. Division costs more than multiplication. Unlimited integers, decimals and rationals are done in software, and the time of an operation grows with the number of digits. A wider type uses more memory and more cache, which matters in large arrays.

## History

The first FORTRAN manual of 1956 had two numeric types. It said that "Two types of constant are permissible: fixed point (restricted to integers) and floating point (characterized by being written with a decimal point)"[^11]. For variables, "Fixed point variables are distinguished by the fact that their first character is I, J, K, L, M, or N"[^11]. That rule is why many programs still use `i`, `j` and `n` as names of integers.

Integer types of many sizes followed, because machines had words of many sizes. Wikipedia says that an integer type is "a data type that represents some range of mathematical integers" and that "Integral data types may be of different sizes and may or may not be allowed to contain negative values"[^12]. Scheme made the hierarchy of numeric types part of the language as the numerical tower[^8]. Python has an `int` with "unlimited precision"[^2], and JavaScript added `BigInt` because "BigInt values represent integer values which are too high or too low to be represented by the number primitive"[^6].

## References
