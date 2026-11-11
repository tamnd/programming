---
tag: 0ZEXPA
kind: concept
slug: bits-and-bytes
name: Bits, bytes and bitwise operations
aliases: [bit, byte, bitwise operation, bit mask, bit flags, shift, logical shift, arithmetic shift, endianness, byte order, hexadecimal]
area: foundations/bits-and-representation
level: core
summary: >-
  Every value is stored as bits, grouped into bytes and words. Bitwise operators test, set, clear and shift the bits of an integer, and a byte order fixes how wide numbers become bytes.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1956
  people: [Claude Shannon, John Tukey, Werner Buchholz, Danny Cohen]
  note: Shannon used the word bit in 1948, Buchholz coined byte in 1956, the System/360 fixed it at 8 bits in 1964, and Cohen named big-endian and little-endian in 1980.
see_also: [integer-overflow, floating-point-arithmetic, utf-8, base64-encode-and-decode, parse-an-integer, characters-and-code-points, call-stack-and-heap]
same_as:
  wikidata: Q879126
  wikipedia: Bitwise_operation
sources:
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/bitwise-and-shift-operators
  title: Bitwise and shift operators, C# reference, Microsoft Learn
  locator: Right-shift operator >>
- url: https://go.dev/ref/spec#Arithmetic_operators
  title: The Go Programming Language Specification
  locator: Arithmetic operators, Integer operators
- url: https://en.cppreference.com/w/cpp/language/operator_arithmetic.html
  title: Arithmetic operators, cppreference.com
  locator: Built-in bitwise shift operators
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html#jls-15.19
  title: Chapter 15, Expressions, The Java Language Specification, Java SE 25 Edition
  locator: 15.19 Shift Operators
- url: https://doc.rust-lang.org/reference/expressions/operator-expr.html#overflow
  title: Operator expressions, The Rust Reference
  locator: Overflow
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Expressions_and_operators#bitwise_operators
  title: Expressions and operators, MDN Web Docs
  locator: Bitwise operators
- url: https://docs.python.org/3/library/stdtypes.html#bitwise-operations-on-integer-types
  title: Built-in Types, Python documentation
  locator: Bitwise Operations on Integer Types
- url: https://www.rfc-editor.org/ien/ien137.txt
  title: Danny Cohen, On Holy Wars and a Plea for Peace, IEN 137, 1980
  locator: Introduction
---

## Idea

A computer stores every value as bits, each a 0 or a 1. A byte is 8 bits and holds one of 256 patterns, and a machine word is 4 or 8 bytes. The meaning of a pattern depends on the type that reads it: the byte `10110110` is 182 as an unsigned number, -74 as a signed one, and part of a character in UTF-8. Bitwise operators work on the bits of an integer directly. AND keeps bits, OR sets them, XOR flips them, NOT inverts all of them, and shifts move them left or right. Programs use these operators for flags, masks, hashes, compression, graphics, network protocols and file formats, and to put multi-byte numbers into bytes in a fixed order.

```python run id=idea
x, m = 0b1011_0110, 0b0000_1111


def show(label, v):
    print(f"{label:<10} {v & 0xFF:08b}  {v & 0xFF:3}  0x{v & 0xFF:02X}")


show("x", x)
show("m", m)
show("x & m", x & m)  # keep the bits that are 1 in m
show("x | m", x | m)  # set the bits of m
show("x ^ m", x ^ m)  # flip the bits of m
show("~x", ~x)  # flip every bit
show("x << 2", x << 2)  # bits move left, zeros come in
show("x >> 2", x >> 2)  # bits move right, low bits drop

n = 0x12345678
print("big-endian bytes   ", n.to_bytes(4, "big").hex(" "))
print("little-endian bytes", n.to_bytes(4, "little").hex(" "))
```

```output for=idea
x          10110110  182  0xB6
m          00001111   15  0x0F
x & m      00000110    6  0x06
x | m      10111111  191  0xBF
x ^ m      10111001  185  0xB9
~x         01001001   73  0x49
x << 2     11011000  216  0xD8
x >> 2     00101101   45  0x2D
big-endian bytes    12 34 56 78
little-endian bytes 78 56 34 12
```

Each row shows a result as 8 bits, in decimal and in hexadecimal. The mask `m` has 1 bits in its low four places, so `x & m` keeps the low four bits of `x`, `x | m` sets them, and `x ^ m` flips them. `~x` flips all eight. A left shift by 2 multiplies by 4 and drops the bits that move past the eighth place, and a right shift by 2 divides by 4 and drops the low bits. The last two lines store the 32-bit number 0x12345678 as 4 bytes: big-endian order puts the most significant byte first, and little-endian order puts it last.

## How it works

**Binary and hexadecimal.** A number in base 2 has one digit for each bit, and bit i has the value 2 to the power i. Binary numbers are long, so programmers write them in hexadecimal, where each digit stands for exactly 4 bits: `0xB6` is `1011 0110`. Most languages accept literals in both bases, such as `0b1011_0110` and `0xB6`, with underscores or quotes to group the digits. Octal, with 3 bits for each digit, survives in Unix file permissions such as `0o755`.

**AND, OR, XOR and NOT.** The bitwise operators apply a logic function to each pair of bits at the same position. AND gives 1 where both bits are 1, OR where at least one is 1, and XOR where exactly one is 1. NOT flips each bit. With a mask, a value with 1 bits at the positions of interest, these give the four basic operations on a set of flags: test a bit with `x & m`, set it with `x | m`, clear it with `x & ~m` and toggle it with `x ^ m`. Go has a separate operator for the third, `x &^ m`. XOR has two useful properties: `x ^ x` is 0, and applying the same mask twice gives the original value back, which hashes, checksums and simple ciphers use.

**Shifts.** A left shift moves every bit toward the high end and puts zeros in the low positions, which multiplies by a power of two. A right shift moves bits toward the low end, and there are two kinds. A logical shift puts zeros in the high positions. An arithmetic shift copies the sign bit, so that a negative number stays negative and the result is a division by a power of two, rounded down. The C# documentation says that for `int` and `long` "the right-shift operator performs an arithmetic shift: the value of the most significant bit (the sign bit) of the left-hand operand is propagated to the high-order empty bit positions."[^1] Java, JavaScript and C# give the logical right shift its own operator, `>>>`. Go and Rust choose by the type: "The shift operators implement arithmetic shifts if the left operand is a signed integer and logical shifts if it is an unsigned integer."[^2]

**Width.** Bitwise results depend on the number of bits in the type, and languages handle the edges differently. In C and C++, a shift by a count at or above the width has no defined result: "If the value of rhs is negative or is not less than the number of bits in lhs, the behavior is undefined."[^3] Java takes the count modulo the width: for an `int`, "only the five lowest-order bits of the right-hand operand are used as the shift distance".[^4] Go defines the result instead: "There is no upper limit on the shift count."[^2] Rust counts a shift at or above the width as overflow, with "Using << or >> where the right-hand argument is greater than or equal to the number of bits in the type of the left-hand argument, or is negative" in its list of overflows, and "Integer operators will panic when they overflow when compiled in debug mode."[^5] JavaScript numbers are floating point, so its operators first convert to 32 bits: "A bitwise operator treats their operands as a set of 32 bits (zeros and ones), rather than as decimal, hexadecimal, or octal numbers."[^6] Python integers have no width at all: "The result of bitwise operations is calculated as though carried out in two’s complement with an infinite number of sign bits."[^7]

**Byte order.** A number wider than one byte has to be stored as several bytes, and machines disagree on their order. Big-endian machines store the most significant byte at the lowest address, and little-endian machines store the least significant byte there. Danny Cohen named the two camps in 1980, after the egg war in *Gulliver's Travels*: "This is an attempt to stop a war."[^8] He described the question as "which bit should travel first, the bit from the little end of the word, or the bit from the big end of the word?"[^8] Today x86 and most ARM systems are little-endian, while network protocols send numbers in big-endian order, which is also called network byte order. Code that reads or writes bytes for a file or a network states the order, with functions such as `to_le_bytes`, `binary.BigEndian` or `DataView`, or builds the bytes with shifts and masks. Then the result does not depend on the machine.

**Bit tricks.** Many small algorithms work on whole words at once. `x & (x - 1)` clears the lowest set bit, so a loop of it counts the 1 bits. `x & -x` isolates the lowest set bit. `x != 0 && (x & (x - 1)) == 0` tests for a power of two. A word of 64 bits can hold a set of 64 small integers, and AND, OR and XOR then compute intersection, union and symmetric difference in one instruction. Modern processors have instructions to count bits, count leading and trailing zeros, and rotate, and languages expose them as `bit_count`, `popcount`, `leading_zeros` and `rotl`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | operators on `int` of any width; `to_bytes`, `struct` |
| JavaScript | native | operators on 32-bit integers; `>>>`; `BigInt`; `DataView` |
| TypeScript | native | as JavaScript |
| Java | native | operators on `int` and `long`; `>>>`; `ByteBuffer` |
| Kotlin | native | infix functions `and`, `or`, `xor`, `shl`, `shr`, `ushr` |
| C# | native | operators; `>>>`; `[Flags]` enums; `BinaryPrimitives` |
| Go | native | operators with `&^`; `math/bits`; `encoding/binary` |
| Rust | native | operators with `!`; checked and wrapping shifts; byte methods |
| C | native | operators on integers; unsigned types for bit patterns |
| C++ | native | the operators of C; `<bit>`; `std::bitset` |
| Bash | native | operators in arithmetic expansion |
| PowerShell | native | `-band`, `-bor`, `-bxor`, `-bnot`, `-shl`, `-shr` |
| SQL | native | dialect operators on integers; bit string types |
| Regex | n/a | patterns match characters, not bits |
| HTML | n/a | no values to operate on |
| CSS | n/a | no values to operate on |

Every example file does the same work: it keeps a set of three permission flags, clears one and toggles another, counts the 1 bits, shifts negative numbers, and writes 0x12345678 as little-endian bytes. The operators are almost the same in every language, because most of them come from C. The differences are at the edges: what happens to a shift count past the width, whether `>>` keeps the sign, whether a byte is signed, and how wide the integers are.

Three groups appear. C, C++, Java, C#, Go and Rust have integer types of fixed widths, so a bit pattern has a known size, and the language defines or forbids what happens past it. JavaScript has one number type and converts to 32 bits for each bitwise operation, with `BigInt` for wider patterns. Python has integers of any width, so masks must state the width that a result needs.

## Variations and related ideas

- **Bit sets and bitmaps.** An array of words used as a set of small integers, with one bit for each possible member. `std::bitset`, Java's `BitSet` and database bitmap indexes work this way.
- **Bit fields.** C and C++ structs can give a member a width in bits, and network headers and hardware registers use such layouts. Their order in memory depends on the compiler, so portable code uses shifts and masks.
- **Flags enums.** C# `[Flags]` enums, Rust's `bitflags` crate and the `enum.Flag` class of Python give names to the bits of a set of options.
- **Signed integers.** Two's complement represents negative integers so that the same adder works for both signs, and it fixes the result of `~x` as `-x - 1`. See [integer overflow](/integer-overflow/).
- **Floating point.** A float is also a pattern of bits, with fields for the sign, the exponent and the fraction. `bit_cast` in C++ and `to_bits` in Rust expose them. See [floating-point arithmetic](/floating-point-arithmetic/).
- **Text and binary encodings.** UTF-8 uses the high bits of each byte to mark how long a sequence is, and Base64 cuts bytes into groups of 6 bits. See [UTF-8](/utf-8/) and [Base64](/base64-encode-and-decode/).
- **Parsing integers.** Converting text to a number and back is a separate step from storing it in bits. See [parse an integer](/parse-an-integer/).

## Pitfalls

- **Precedence of `&` and `==`.** In C, C++, Java, JavaScript and C#, comparison binds more tightly than bitwise AND, so `x & MASK == 0` means `x & (MASK == 0)`. The wrong block in the C file shows this. Put parentheses around every bitwise expression inside a comparison. Python and Go give `&` higher precedence than comparison, so the same text works there.
- **Shifts past the width.** A shift by 32 or more on a 32-bit value is undefined in C and C++, takes the count modulo 32 in Java, C# and JavaScript, gives 0 in Go, and panics in a debug build of Rust. The fails block in the Rust file shows the compiler rejecting one. Check the count, or use a wider type.
- **Signed values.** Right shift of a negative number keeps the sign in most languages, so it does not give the bits that a logical shift would. Java's `byte` is signed, so a byte read from a file must be masked with `& 0xFF`. Use unsigned types, or `>>>`, for bit patterns.
- **NOT with no fixed width.** In Python, `~x` of a positive number is negative. The wrong block in the Python file shows this. Mask the result to the width you mean.
- **32-bit conversion in JavaScript.** `x | 0` and `~~x` truncate numbers to 32 bits, so they give wrong results for values above about 2.1 billion. The wrong block in the JavaScript file shows this. Use `Math.trunc` for numbers, and `BigInt` for wide bit patterns.
- **Byte order taken from the machine.** Copying the memory of an integer into a file, or casting a byte pointer to an integer pointer, writes the order of the machine that runs the code, and in C the cast can also break alignment and aliasing rules. State the order and build the bytes with shifts, or use the functions of the standard library.

## Cost

AND, OR, XOR, NOT and shifts by a constant take one processor instruction and one cycle on a fixed-width integer. A shift by a variable count also takes one instruction on most processors, plus a mask or a check where the language requires one. Counting bits, counting leading zeros and rotating take one instruction on processors that have them, and a short sequence of operations otherwise. On integers of any width, as in Python and `BigInt`, each operation takes time in proportion to the number of words in the value, plus the cost of the object. Converting byte order costs one byte-swap instruction for each value, and nothing when the order of the machine matches the format.

## History

Gottfried Leibniz described binary arithmetic in 1703. Claude Shannon used the word bit in his 1948 paper on communication, and credited it to John Tukey. Werner Buchholz coined the word byte in 1956, during the design of the IBM Stretch computer, for a group of bits that encodes one character. The IBM System/360, from 1964, fixed the byte at 8 bits and addressed memory by the byte, and most later machines did the same. The System/360 stored numbers in big-endian order, while the DEC PDP-11, from 1970, and later the Intel x86 stored them in little-endian order.

C, from the early 1970s, gave programmers the operators `&`, `|`, `^`, `~`, `<<` and `>>` on integers, and its precedence for `&` and `|` came from B, where they also served as logical operators before `&&` and `||` existed. Danny Cohen's 1980 note *On Holy Wars and a Plea for Peace* named the byte order camps big-endian and little-endian.[^8] Java and JavaScript, both from the mid-1990s, have `>>>`, because Java has no unsigned types and JavaScript took its operators from Java. C++20 made right shift of negative values arithmetic and added the `<bit>` header.

## References
