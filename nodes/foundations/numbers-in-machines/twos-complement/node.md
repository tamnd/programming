---
tag: G7KVME
kind: concept
slug: twos-complement
name: Two's complement
aliases: [two's complement, twos complement, 2's complement, invert and add one, most negative number, sign extension, signed binary]
area: foundations/numbers-in-machines
level: core
summary: >-
  Two's complement stores a negative integer as the bits of its magnitude inverted plus one. The same adder works for signed and unsigned values, and the range has one more negative value.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1945
  people: [John von Neumann]
  note: The First Draft of a Report on the EDVAC of 1945 suggested two's complement, the EDSAC of 1949 used it, and the IBM System/360 of 1964 made it the most widely used representation.
see_also: [integer-representation, signed-and-unsigned-integers, binary-number, bits-and-bytes, integer-overflow]
same_as:
  wikidata: Q232577
  wikipedia: Two%27s_complement
sources:
  - url: https://en.wikipedia.org/wiki/Two%27s_complement
    title: Two's complement, Wikipedia
    locator: the introduction; Procedure; History; Converting from two's complement representation; Sign extension; Most negative number
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-15.html
    title: The Java Language Specification, Java SE 27 Edition, Chapter 15
    locator: 15.15.4 Unary Minus Operator -
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Bitwise_NOT
    title: Bitwise NOT (~), MDN Web Docs
    locator: Description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/operator_arithmetic
    title: Arithmetic operators, C reference, cppreference.com
    locator: Overflows; Unary arithmetic, Notes
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.i32.html
    title: Primitive Type i32, The Rust Standard Library
    locator: checked_neg; wrapping_neg
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked
    title: The checked and unchecked statements, C# reference
    locator: the introduction
    accessed: 2026-11-16
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r4.html
    title: "P0907R4: Signed Integers are Two's Complement, JF Bastien, 2018"
    locator: Abstract; 1. Introduction
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/language/types
    title: Fundamental types, C++ reference, cppreference.com
    locator: Properties
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/arithmetic_types
    title: Arithmetic types, C reference, cppreference.com
    locator: Integer types, Properties
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/language/implicit_conversion
    title: Implicit conversions, C++ reference, cppreference.com
    locator: Integral conversions
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se27/html/jls-4.html
    title: The Java Language Specification, Java SE 27 Edition, Chapter 4
    locator: 4.2 Primitive Types and Values
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Numeric types
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html
    title: Built-in Types, Python documentation
    locator: Bitwise Operations on Integer Types
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Math.html
    title: Math (Java SE 27 and JDK 27)
    locator: abs(int)
    accessed: 2026-11-16
---

## Idea

Two's complement is the usual way to store signed integers in a fixed number of bits. A value that is 0 or more has its usual binary form. For a negative value, "negative numbers are represented by taking the bit complement of their magnitude and then adding one (−6 is 1010)."[^1] In 4 bits, 6 is `0110`, the inverted bits are `1001`, and adding 1 gives `1010`.

```python run id=negate
BITS = 8
MASK = (1 << BITS) - 1


def value(bits):
    """Read 8 bits as two's complement: the top bit is worth -128."""
    return bits - (1 << BITS) if bits & 0x80 else bits


def negate(bits):
    return (~bits + 1) & MASK  # invert every bit, then add 1


six = 0b00000110
rows = [
    ("6", six),
    ("inverted", ~six & MASK),
    ("plus 1", negate(six)),
    ("again", negate(negate(six))),
    ("negate 0", negate(0)),
    ("negate -128", negate(0x80)),
]
for label, bits in rows:
    print(f"{label + ':':<13}{bits:08b} = {value(bits)}")
```

```output for=negate
6:           00000110 = 6
inverted:    11111001 = -7
plus 1:      11111010 = -6
again:       00000110 = 6
negate 0:    00000000 = 0
negate -128: 10000000 = -128
```

The same steps also turn -6 back into 6. Zero has one form, because the carry out of the top bit is dropped. The most negative value, -128 in 8 bits, is its own negation, because +128 does not fit.

## How it works

**Bit weights.** In two's complement, "The weight of each bit is a power of two, except for the most significant bit, whose weight is the negative of the corresponding power of two."[^1] In 8 bits, the top bit is worth -128 and the others are worth 64, 32, 16, 8, 4, 2 and 1. So `11111010` is -128 + 64 + 32 + 16 + 8 + 2 = -6. The top bit also tells the sign: it is 1 for each negative value.

**Negation.** Inverting each bit of *x* gives -*x* - 1, as the row for the inverted bits shows: the bits of 6 inverted are -7. So inverting and adding 1 gives -*x*. The Java Language Specification states this rule for all integer values: -x equals (~x)+1[^2]. For JavaScript numbers, "Bitwise NOTing any 32-bit integer x yields -(x + 1)"[^3].

**One adder for both signs.** Addition, subtraction and multiplication give the same bits for signed and unsigned operands. Wikipedia says that "the same arithmetic implementations can be used on signed as well as unsigned integers" and that they "differ only in the integer overflow situations"[^1]. The machine has one add instruction, and the program decides how to read the result:

```python run id=adder
def value(bits):
    return bits - 256 if bits & 0x80 else bits


for a, b in [(0b11111010, 0b00001010), (0b11111111, 0b11111111), (0b01111111, 0b00000001)]:
    s = (a + b) & 0xFF
    print(f"{a:08b} + {b:08b} = {s:08b}  unsigned {a} + {b} = {s}  signed {value(a)} + {value(b)} = {value(s)}")
```

```output for=adder
11111010 + 00001010 = 00000100  unsigned 250 + 10 = 4  signed -6 + 10 = 4
11111111 + 11111111 = 11111110  unsigned 255 + 255 = 254  signed -1 + -1 = -2
01111111 + 00000001 = 10000000  unsigned 127 + 1 = 128  signed 127 + 1 = -128
```

The first two sums are right as signed values, and they overflow as unsigned values. The last sum is right as an unsigned value and overflows as a signed value: two positive operands give a negative result.

**The range is not symmetric.** Two's complement "has only one representation for zero, with room for one extra negative number (the range of a 4-bit number is −8 to +7)."[^1] With *N* bits, the range is -2ᴺ⁻¹ to 2ᴺ⁻¹ - 1. So the most negative value has no positive partner: "The two's complement of the most negative number representable (e.g. a one as the most-significant bit and all other bits zero) is itself."[^1] Each language handles this case in its own way. Java says that "negation of the maximum negative int or long results in that same maximum negative number. Overflow occurs in this case, but no exception is thrown."[^2] In C, unary minus "invokes undefined behavior due to signed integer overflow when applied to INT_MIN, LONG_MIN, or LLONG_MIN"[^4]. Rust's `checked_neg` computes the negation, "returning None if self == MIN"[^5]. C# wraps by default, and for an overflow in a `checked` context, "a System.OverflowException is thrown."[^6]

**Sign extension.** To store a value in more bits, "the most-significant bit must be repeated in all the extra bits."[^1] So -6 in 8 bits, `11111010`, becomes `1111111111111010` in 16 bits. A right shift of a signed value also copies the top bit, so the sign does not change.

**Why the standards require it.** For a long time, C and C++ also allowed ones' complement and sign-magnitude. The proposal P0907 starts: "There is One True Representation for signed integers, and that representation is two’s complement."[^7] It notes that "None of [MSVC], [GCC], and [LLVM] support other representations."[^7] For C++, two's complement became "the only representation allowed by the standard" as of C++20[^8], and C23 made the same change for C[^9]. C++20 also defined the conversion of a value that does not fit into a signed type: the result is the value modulo 2ᴺ[^10]. P0907 did not make signed overflow defined: "If a signed operation would naturally produce a value that is not within the range of the result type, the behavior is undefined."[^7] Java defines its integral types as "signed two's-complement integers"[^11] from the start, and the Go specification says: "The value of an n-bit integer is n bits wide and represented using two's complement arithmetic."[^12]

## In languages

| Language | Verdict | Two's complement in the language |
|---|---|---|
| Python | native | `int` has no width; bit operations act as if it had infinite sign bits |
| JavaScript | native | bit operators work on 32-bit two's complement values; `BigInt.asIntN` |
| TypeScript | native | as JavaScript |
| Java | native | `byte`, `short`, `int`, `long` are two's complement by definition |
| Kotlin | native | `Byte`, `Short`, `Int`, `Long` as in Java |
| C# | native | `sbyte`, `short`, `int`, `long`; `checked` and `unchecked` |
| Go | native | each signed type, by the specification |
| Rust | native | each signed type; `wrapping_neg`, `checked_neg` |
| C | native | required since C23; `-INT_MIN` is undefined |
| C++ | native | required since C++20 |
| Bash | native | `$(( ))` arithmetic on signed integers |
| PowerShell | native | the .NET signed types |
| SQL | n/a | the database stores the value; SQL does not show the bits |
| Regex | n/a | no numbers |
| HTML | n/a | no numbers |
| CSS | n/a | no integer types with a width |

Each general-purpose language uses two's complement for its fixed-width signed types. Python is a special case. Its `int` has no width, but "The result of bitwise operations is calculated as though carried out in two’s complement with an infinite number of sign bits."[^13] So `~5` is -6 in Python, as it is in C.

The example files show the same facts in each language: -*x* equals `~x + 1`, the bits of -1 are all ones, the negation of the most negative value overflows, and a widening conversion copies the sign bit.

## Variations and related ideas

- **Ones' complement and sign-magnitude.** These older forms have two zeros and need different adders for signed and unsigned values. See [[ones-complement-and-sign-magnitude]].
- **Offset binary.** A value is stored as *x* + *K*, so the bits sort in the order of the values. The exponent of an IEEE 754 float uses this form. See [[ieee-754]].
- **Two's complement as arithmetic modulo 2ᴺ.** An *N*-bit two's complement value -*x* has the same bits as the unsigned value 2ᴺ - *x*. This is why one adder works for both. See [[signed-and-unsigned-integers]].
- **Overflow rules.** Two's complement tells what bits a result has. It does not tell whether an overflow wraps, traps or is undefined. See [[overflow-semantics]].

## Pitfalls

- **Negating the most negative value.** `-x`, `abs(x)` and `x / -1` fail for the most negative value. Java's `Math.abs` notes that for `Integer.MIN_VALUE`, "the most negative representable int value, the result is that same value, which is negative."[^14] In C, the same operation is undefined behavior[^4]. Test for the minimum first, or use `Math.absExact`, `checked_neg` or a wider type.
- **Thinking that the C standard defines overflow.** C23 and C++20 fix the representation, but signed overflow is still undefined behavior[^7]. A program that relies on wraparound must use unsigned types or the wrapping functions of its compiler.
- **Losing the sign when widening.** A value read from a byte with an unsigned type and then stored in a wider signed type is not sign-extended. The Java example shows that `(int) b` copies the sign of a `byte`, and `b & 0xFF` gives 0 to 255. Choose the extension that matches the meaning of the data.
- **Comparing bits as unsigned values.** The bits of a negative value are larger than the bits of each positive value when they are read as unsigned. Sorting raw bytes or words gives the negative values last.

## Cost

Two's complement makes signed arithmetic cost the same as unsigned arithmetic. The hardware has one adder, and subtraction is addition of the negation. Wikipedia notes that "the advantage of using two's complement is the elimination of examining the signs of the operands to determine whether addition or subtraction is needed."[^1] Sign extension can be one instruction: "Some processors do this in a single instruction; on other processors, a conditional must be used followed by code to set the relevant bits or bytes."[^1]

## History

"The method of complements had long been used to perform subtraction in decimal adding machines and mechanical calculators."[^1] John von Neumann suggested two's complement in his *First Draft of a Report on the EDVAC* of 1945, and the EDSAC of 1949 used it[^1]. Many early computers used ones' complement or sign-magnitude instead. The IBM System/360 of 1964 "made two's complement the most widely used binary representation in the computer industry."[^1] The PDP-8 of 1965, the Data General Nova of 1969 and the PDP-11 of 1970 also used it[^1].

JF Bastien's proposal P0907 of 2018 made two's complement the only signed representation in C++20[^7][^8], and C23 followed[^9].

## References
