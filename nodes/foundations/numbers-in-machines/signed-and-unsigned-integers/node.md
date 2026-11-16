---
tag: AV46E1
kind: concept
slug: signed-and-unsigned-integers
name: Signed and unsigned integers
aliases: [signedness, signed integer, unsigned integer, unsigned type, signed and unsigned comparison, sign-compare, usual arithmetic conversions, unsigned wraparound]
area: foundations/numbers-in-machines
level: core
summary: >-
  A signed integer type holds negative and positive values, and an unsigned type of the same width holds only zero and positive values. Mixing the two in one expression can change a value.
applies_to: core-languages
viz: optional
formal: none
see_also: [integer-representation, binary-number, bits-and-bytes, integer-overflow, type-conversion, floating-point-arithmetic]
same_as:
  wikidata: Q7512911
  wikipedia: Signedness
sources:
  - url: https://en.wikipedia.org/wiki/Signedness
    title: Signedness, Wikipedia
    locator: the introduction; In programming languages
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/book/ch03-02-data-types.html
    title: Data Types, The Rust Programming Language
    locator: Integer Types
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/integral-numeric-types
    title: Integral numeric types, C# reference
    locator: Characteristics of the integral types
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/reference/expressions/operator-expr.html
    title: Operator expressions, The Rust Reference
    locator: Numeric cast
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/conversion
    title: Implicit conversions, C reference, cppreference.com
    locator: Usual arithmetic conversions; Integer conversions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Numeric types; Conversions between numeric types; Constant expressions
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/arithmetic-operators
    title: Arithmetic operators, C# reference
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python documentation
    locator: Numeric Types; int.to_bytes()
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/utility/intcmp
    title: std::cmp_equal, cmp_not_equal, cmp_less, cmp_greater, cmp_less_equal, cmp_greater_equal, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-4.html
    title: The Java Language Specification, Java SE 27 Edition, Chapter 4
    locator: 4.2 Primitive Types and Values
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Integer.html
    title: Integer (Java SE 27 and JDK 27)
    locator: compareUnsigned; divideUnsigned; toUnsignedString
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Unsigned_right_shift
    title: Unsigned right shift (>>>), MDN Web Docs
    locator: the introduction; Description; Examples
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/arithmetic_types
    title: Arithmetic types, C reference, cppreference.com
    locator: Character types
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i32.html
    title: Primitive Type i32, The Rust Standard Library
    locator: cast_unsigned; unsigned_abs
    accessed: 2026-11-16
  - url: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
    title: C++ Core Guidelines
    locator: ES.100, ES.101, ES.102, ES.106, ES.107
    accessed: 2026-11-16
  - url: https://google.github.io/styleguide/cppguide.html
    title: Google C++ Style Guide
    locator: Integer Types; On Unsigned Integers
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Two%27s_complement
    title: Two's complement, Wikipedia
    locator: the introduction
    accessed: 2026-11-16
---

## Idea

An integer type has a fixed number of bits, and its signedness decides what the bit patterns mean. "A numeric variable is signed if it can represent both positive and negative numbers, and unsigned if it can only represent non-negative numbers (zero or positive numbers)."[^1] The two kinds of the same width have the same number of values, but the ranges are not the same. An 8-bit unsigned type holds 0 to 255, and an 8-bit signed type holds -128 to 127.

The same bits can mean two numbers. The 8 bits `11111111` are 255 as an unsigned value and -1 as a signed value in two's complement:

```python run id=idea
for bits in (0b00000001, 0b01111111, 0b10000000, 0b11111111):
    signed = bits - 256 if bits >= 128 else bits
    print(f"{bits:08b}  unsigned {bits:3}  signed {signed:4}")
```

```output for=idea
00000001  unsigned   1  signed    1
01111111  unsigned 127  signed  127
10000000  unsigned 128  signed -128
11111111  unsigned 255  signed   -1
```

The patterns from `00000000` to `01111111` mean the same number in both types. The patterns with the top bit set are large numbers to an unsigned type and negative numbers to a signed type. Most problems with signedness come from this difference: a conversion keeps the bits and changes the number.

## How it works

**Ranges.** A signed type of *n* bits stores the numbers from -(2ⁿ⁻¹) to 2ⁿ⁻¹ - 1, and an unsigned type stores 0 to 2ⁿ - 1. The Rust book gives this rule, so an `i8` holds -128 to 127 and a `u8` holds 0 to 255[^2]. The C# reference lists the ranges of its types: `int` is "-2,147,483,648 to 2,147,483,647" and `uint` is "0 to 4,294,967,295"[^3]. A signed type gives up half of the large positive values to hold the negative values.

**Conversions keep the bits.** A conversion between a signed and an unsigned type of the same width keeps the bit pattern in most languages. In Rust, a cast "between two integers of the same size (e.g. i32 -> u32) is a no-op"[^4]. In C, a value that does not fit an unsigned target is changed by a multiple of 2ⁿ until it fits: "In other words, unsigned integers implement modulo arithmetic."[^5] For a signed target, "the behavior is implementation-defined (which may include raising a signal)."[^5] Go defines both directions. A signed value "is sign extended to implicit infinite precision; otherwise it is zero extended. It is then truncated to fit in the result type's size."[^6]

**Mixed expressions.** An operation on a signed and an unsigned value needs one type for both. Languages choose four different rules:

- *Convert to unsigned.* In C and C++, after the integer promotions, "If the unsigned type has conversion rank greater than or equal to the rank of the signed type, then the operand with the signed type is implicitly converted to the unsigned type."[^5] So in `-1 < 1u`, the -1 becomes the largest `unsigned int`, and the result is false.
- *Convert to a wider signed type.* In C#, "If the operands are different integral or floating-point types, their values are converted to the closest containing type, if such a type exists."[^7] An `int` and a `uint` both become `long`, so `-1 < 1u` is true.
- *Refuse to compile.* Go says: "Explicit conversions are required when different numeric types are mixed in an expression or assignment."[^6] Rust also needs a conversion before it compares an `i32` with a `u32`.
- *Have one integer type.* Python integers "have unlimited precision"[^8], so there is no unsigned type to mix.

**Safe comparisons.** C++20 added `std::cmp_less` and its relatives. "Unlike builtin comparison operators, negative signed integers always compare less than (and not equal to) unsigned integers: the comparison is safe against non-value-preserving integer conversion."[^9] In Go, C# and Rust, the program converts both values to a wider signed type, or tests the sign first.

**Languages without unsigned types.** Java's integral types `byte`, `short`, `int` and `long` are "8-bit, 16-bit, 32-bit and 64-bit signed two's-complement integers, respectively"[^10]. The one unsigned integral type is `char`, which holds 16-bit UTF-16 code units[^10]. For unsigned work, the `Integer` class has methods such as `compareUnsigned`, which "Compares two int values numerically treating the values as unsigned."[^11] JavaScript numbers are not integers of a fixed width, but the operator `>>>` gives an unsigned 32-bit result. MDN shows `-9 >>> 2` as `1073741821`[^12]. Python has no fixed-width types. Its `int.to_bytes` and `int.from_bytes` take a `signed` argument, which selects two's complement for the bytes[^8].

## In languages

| Language | Verdict | Unsigned types and mixed rules |
|---|---|---|
| Python | absent | one `int` with no width; `signed=` in `to_bytes` and `from_bytes` |
| JavaScript | idiom | no unsigned `Number`; `>>> 0` and `Uint8Array` to `Uint32Array`, `BigUint64Array` |
| TypeScript | idiom | as JavaScript |
| Java | stdlib | only `char` is unsigned; `Integer` and `Long` have unsigned methods |
| Kotlin | native | `UByte`, `UShort`, `UInt`, `ULong` |
| C# | native | `byte`, `ushort`, `uint`, `ulong`, `nuint`; mixed operands become a wider type |
| Go | native | `uint8` to `uint64`, `uint`, `uintptr`; mixed operands do not compile |
| Rust | native | `u8` to `u128`, `usize`; mixed operands do not compile |
| C | native | `unsigned` types; mixed operands often become unsigned |
| C++ | native | as C; `std::cmp_less` and its relatives |
| Bash | absent | signed arithmetic only |
| PowerShell | native | the .NET types, such as `[uint32]` and `[byte]` |
| SQL | absent | `SMALLINT`, `INTEGER` and `BIGINT` are signed |
| Regex | n/a | no numbers |
| HTML | n/a | no numbers |
| CSS | n/a | no integer types with a width |

The verdict tells whether a language has unsigned integer types. *Native* languages have them as types of the language. Java has only `char`, and its standard library gives unsigned operations on signed types. JavaScript gets an unsigned view with an operator or a typed array. Python and Bash have no unsigned integer type.

The example files show the same ideas in each language. Python reads two bytes as unsigned and as signed. JavaScript uses `>>>` and typed arrays. Java uses the unsigned methods of `Integer`. C and C++ show a mixed comparison that gives the wrong answer, and C++ fixes it with `std::cmp_greater`. C# shows that `int` and `uint` become `long`. Go and Rust convert by hand, because a mixed expression does not compile.

## Variations and related ideas

- **Sign representations.** A signed type needs a way to store the sign. Almost all machines use two's complement, and older machines also used ones' complement and sign-magnitude. See [[twos-complement]] and [[ones-complement-and-sign-magnitude]].
- **Overflow.** An unsigned result that does not fit wraps modulo 2ⁿ in C, C++, Go and C#. Signed overflow has different rules in each language. See [[overflow-semantics]] and [[integer-overflow]].
- **Character types.** In C, plain `char` is "Equivalent to either signed char or unsigned char (which one is implementation-defined and may be controlled by a compiler commandline switch)"[^13]. Java's `char` is unsigned[^10].
- **Size types.** `size_t` in C and C++, `usize` in Rust and `nuint` in C# are unsigned types with the width of an address. The C# reference says that `nint` and `nuint` "are native-sized integers"[^3].
- **Same bits, other view.** Rust's `cast_unsigned` "Returns the bit pattern of self reinterpreted as an unsigned integer of the same size."[^14] Its `unsigned_abs` "Computes the absolute value of self without any wrapping or panicking."[^14] It returns an unsigned value, so the absolute value of `i32::MIN` fits.

## Pitfalls

- **A mixed comparison in C and C++.** `-1 < 1u` is false, because -1 becomes the largest `unsigned int`. "Compilers often issue a warning when comparisons are made between signed and unsigned numbers or when one is cast to the other."[^1] Turn on `-Wsign-compare`, and use `std::cmp_less` in C++.
- **An unsigned count that goes below zero.** In C++, `v.size() - 1` on an empty vector is the largest `size_t`, not -1. A loop such as `for (auto i = vec.size()-1; i >= 0; i -= 2)` is marked as a bug in the C++ Core Guidelines[^15], because an unsigned `i` is always 0 or more.
- **An unsigned type does not stop negative values.** The C++ Core Guidelines say: "Using unsigned doesn’t actually eliminate the possibility of negative values."[^15] Their example assigns `-2` to an `unsigned int` and gets 4294967294. The Google C++ Style Guide says: "In particular, do not use unsigned types to say a number will never be negative. Instead, use assertions for this."[^16]
- **Bytes in Java are signed.** A byte with the bits `11001000` reads as -56, not 200. Mask with `& 0xFF`, or use `Byte.toUnsignedInt`.
- **A negative constant in Go.** The specification lists `uint(-1)` as an illegal constant expression: "-1 cannot be represented as a uint"[^6]. A conversion of a variable that holds -1 compiles and wraps.
- **A narrowing cast in C.** A conversion to a signed type that cannot hold the value is implementation-defined[^5]. Test the range before the conversion.

## Cost

The choice of signedness does not change the speed of most operations. Wikipedia says that with two's complement, "the fundamental arithmetic operations of addition, subtraction, and multiplication are identical to those for unsigned binary numbers"[^17]. The difference is in what the processor reports: "arithmetic instructions usually set different CPU flags such as the carry flag for unsigned arithmetic and the overflow flag for signed."[^1] Comparison, division, right shift and widening conversion have separate signed and unsigned forms, but each form is one instruction on common processors.

The style guides choose by meaning, not by speed. The C++ Core Guidelines say "ES.101: Use unsigned types for bit manipulation" and "ES.102: Use signed types for arithmetic"[^15]. The Google C++ Style Guide says: "Unsigned integers are good for representing bitfields and modular arithmetic."[^16]

## History

Java has no unsigned integral type other than `char`[^10]. Java version 1.8 added unsigned methods to `Integer`, such as `compareUnsigned`, `divideUnsigned` and `toUnsignedString`, each marked "Since: 1.8"[^11]. C++20 added the functions `std::cmp_less` and its relatives[^9], and Rust 1.87.0 added `cast_unsigned`[^14].

## References
