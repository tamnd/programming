---
tag: ZG7TDD
kind: concept
slug: integer-representation
name: Integer representation
aliases: [integer representation, integer type, machine integer, fixed-width integer, signed integer, unsigned integer, two's complement, ones' complement, sign-magnitude, signed number representation, sign extension, zero extension, endianness, byte order, wraparound]
area: foundations/numbers-in-machines
level: core
summary: >-
  An integer type stores whole numbers in a fixed number of bits. Unsigned types read the bits as a binary number, and almost all machines store signed values in two's complement.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1945
  people: [John von Neumann, Maurice Wilkes, JF Bastien]
  note: The EDVAC report of 1945 suggested two's complement, and the EDSAC of 1949 used it. The IBM System/360 of 1964 made it the common choice, and C++20 and C23 made it the only representation that the standards allow.
see_also: [binary-number, bits-and-bytes, integer-overflow, floating-point-arithmetic, type-conversion, parse-an-integer, memory-address]
same_as:
  wikidata: Q729138
  wikipedia: Integer_(computer_science)
sources:
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, Python documentation
  locator: Bitwise Operations on Integer Types; int.to_bytes()
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-4.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 4
  locator: 4.2 Primitive Types and Values; 4.2.2 Integer Operations
- url: https://en.cppreference.com/w/cpp/language/types
  title: Fundamental types, C++ reference, cppreference.com
  locator: Properties
- url: https://en.cppreference.com/w/c/language/arithmetic_types
  title: Arithmetic types, C reference, cppreference.com
  locator: Integer types, Properties
- url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r4.html
  title: "P0907R4: Signed Integers are Two's Complement, JF Bastien, 2018"
  locator: Abstract
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Integer overflow
- url: https://doc.rust-lang.org/reference/behavior-not-considered-unsafe.html
  title: Behavior not considered unsafe, The Rust Reference
  locator: Integer overflow
- url: https://doc.rust-lang.org/std/primitive.i32.html
  title: Primitive Type i32, Rust standard library documentation
  locator: checked_add, wrapping_add, overflowing_add, saturating_add
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/checked-and-unchecked
  title: The checked and unchecked statements, C# language reference
  locator: Description
- url: https://en.cppreference.com/w/c/numeric/ckd_add
  title: ckd_add, C reference, cppreference.com
  locator: Return value
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Typed_arrays
  title: JavaScript typed arrays, MDN Web Docs
  locator: Typed array views; Endianness
- url: https://en.wikipedia.org/wiki/Two%27s_complement
  title: Two's complement, Wikipedia
  locator: History
- url: https://en.wikipedia.org/wiki/Signed_number_representations
  title: Signed number representations, Wikipedia
  locator: History
---

## Idea

A machine integer is a fixed number of bits, usually 8, 16, 32 or 64. With *N* bits there are 2ᴺ patterns, and the type decides which number each pattern means. An unsigned type reads the bits as a [binary number](/binary-number/), so 8 bits hold 0 to 255. A signed type must also hold negative numbers, and machines have used three ways to do that: sign-magnitude, ones' complement and two's complement. Today almost every processor and every major language uses two's complement, in which the top bit is worth −2ᴺ⁻¹ instead of +2ᴺ⁻¹.

```python run id=idea
BITS = 8


def sign_magnitude(n):
    return (1 << BITS - 1 if n < 0 else 0) | abs(n)


def ones_complement(n):
    return n if n >= 0 else ~(-n) & 0xFF  # invert every bit


def twos_complement(n):
    return n & 0xFF  # the same as inverting every bit and adding 1


for n in (5, -5, 0):
    print(f"{n:3}: sign-magnitude {sign_magnitude(n):08b}  ones' {ones_complement(n):08b}  two's {twos_complement(n):08b}")
print("negative zero: sign-magnitude", f"{sign_magnitude(0) | 0x80:08b}", " ones'", f"{0xFF:08b}")


def read_signed(bits):
    """In two's complement the top bit is worth -2^7 instead of +2^7."""
    return bits - (1 << BITS) if bits & 1 << BITS - 1 else bits


print("range:", read_signed(0x80), "to", read_signed(0x7F), "| unsigned 0 to", 0xFF)
# The adder does not know about signs: the same bits give both answers.
a, b = 0b11111011, 0b00000111  # 251 and 7, or -5 and 7
s = (a + b) & 0xFF
print(f"{a:08b} + {b:08b} = {s:08b}  unsigned: {a} + {b} = {s} (mod 256)  signed: {read_signed(a)} + {b} = {read_signed(s)}")
print("127 + 1 =", read_signed((127 + 1) & 0xFF), "  -(-128) =", read_signed(-(-128) & 0xFF))
# Sign extension: copy the top bit to widen a value from 8 to 16 bits.
print(f"-5 in 16 bits: {(-5) & 0xFFFF:016b}")
```

```output for=idea
  5: sign-magnitude 00000101  ones' 00000101  two's 00000101
 -5: sign-magnitude 10000101  ones' 11111010  two's 11111011
  0: sign-magnitude 00000000  ones' 00000000  two's 00000000
negative zero: sign-magnitude 10000000  ones' 11111111
range: -128 to 127 | unsigned 0 to 255
11111011 + 00000111 = 00000010  unsigned: 251 + 7 = 2 (mod 256)  signed: -5 + 7 = 2
127 + 1 = -128   -(-128) = -128
-5 in 16 bits: 1111111111111011
```

Sign-magnitude and ones' complement each have two zeros, and they need special rules in the adder. In two's complement, `-5` is the bits of 5 inverted plus 1, there is one zero, and the adder for unsigned numbers also gives the right signed answer: `11111011 + 00000111` is 2 whether the first operand means 251 or −5. The price is a range that is not symmetric: 8 bits hold −128 to 127, so −(−128) has no 8-bit answer and wraps back to −128. To widen a signed value, the machine copies the top bit into the new bits, which is sign extension.

## How it works

**Width and range.** An unsigned type of *N* bits holds 0 to 2ᴺ − 1. A two's complement type of *N* bits holds −2ᴺ⁻¹ to 2ᴺ⁻¹ − 1, so a 32-bit `int` holds −2,147,483,648 to 2,147,483,647. Languages name the widths in different ways: `int8_t` to `int64_t` in C and C++, `i8` to `i128` in Rust, `sbyte` to `long` and `Int128` in C#, and `int8` to `int64` in Go. Some types follow the machine: `size_t`, `usize`, `uintptr` and `nint` are as wide as an address, and C's `int` and `long` have only minimum widths. Python and JavaScript's `BigInt` have no fixed width; Python computes bitwise operations "as though carried out in two's complement with an infinite number of sign bits"[^1].

**What the standards require.** Java defines `byte`, `short`, `int` and `long` as "8-bit, 16-bit, 32-bit and 64-bit signed two's-complement integers", and `char` as a 16-bit unsigned type[^2]. C and C++ allowed all three representations for a long time. C++20 made two's complement "the only representation allowed by the standard"[^3], and C23 did the same[^4]. The C++ proposal, P0907, starts with the sentence "There is One True Representation for signed integers, and that representation is two's complement"[^5]. Neither change made signed overflow defined in C or C++; it is still undefined behavior.

**Overflow.** An *N*-bit result that does not fit keeps its low *N* bits, which is arithmetic modulo 2ᴺ. Languages differ in what the program sees:

- *Wrap.* Java's "integer operators do not indicate overflow or underflow in any way"[^2]. The Go specification says that signed operations "may legally overflow", that "overflow does not cause a run-time panic", and that a compiler may not assume that overflow does not occur[^6]. C# wraps in its default unchecked context.
- *Trap.* Rust inserts "dynamic checks that panic on overflow" in debug builds, and a release build may wrap with two's complement results[^7]. Rust also has methods that say what to do: `checked_add` returns `None`, `wrapping_add` wraps, `overflowing_add` returns the wrapped value and a flag, and `saturating_add` stops at the limit[^8]. In a C# `checked` context, overflow throws `System.OverflowException`[^9].
- *Undefined.* In C and C++, unsigned arithmetic wraps, but signed overflow is undefined behavior, so the compiler can assume that it never happens. C23 adds `ckd_add`, `ckd_sub` and `ckd_mul`, which return true when the mathematical result does not fit[^10].
- *Grow.* Python integers and JavaScript `BigInt` values grow as needed and never overflow.

**Conversions.** A narrowing conversion keeps the low bits, so 200 in a signed 8-bit type is −56. A widening conversion of a signed value copies the sign bit, and a widening conversion of an unsigned value adds zero bits, so the byte `0xFB` becomes −5 or 251 depending on its type. In C and C++, an operation on a signed and an unsigned operand of the same rank first converts the signed operand to unsigned, so `-1 < 1u` is false. C++20 adds `std::cmp_less` and `std::in_range`, which compare the mathematical values.

**Shifts.** A left shift by *k* multiplies by 2ᵏ and drops the bits that go past the top. A right shift has two forms: an arithmetic shift copies the sign bit, so `-16 >> 2` is −4, and a logical shift fills with zeros. Java, JavaScript and C# write the logical shift as `>>>`; C, C++, Go and Rust choose by the type, so they shift an unsigned value logically.

**Byte order.** A multi-byte integer is stored in memory as a sequence of bytes. A little-endian machine, such as x86 or the usual configuration of ARM, puts the lowest byte first; a big-endian layout, which network protocols use, puts the highest byte first. Code that reads or writes files and network data must name the order. JavaScript typed arrays use the order of the platform, and a `DataView` lets the program choose, with big-endian as the default[^11].

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `int` of any size; `int.to_bytes`, `int.from_bytes`; `struct`, `array` and `ctypes` for fixed widths |
| JavaScript | native | 32-bit bitwise operators; `Int8Array` to `BigInt64Array`; `DataView`; `BigInt.asIntN` |
| TypeScript | native | as JavaScript |
| Java | native | `byte`, `short`, `int`, `long` in two's complement; `char`; unsigned methods of `Integer` and `Long` |
| Kotlin | native | `Byte` to `Long`; `UByte` to `ULong` |
| C# | native | `sbyte` to `long`, `byte` to `ulong`, `Int128`, `nint`; `checked` and `unchecked` |
| Go | native | `int8` to `int64`, `uint8` to `uint64`, `int`, `uintptr`; defined wraparound |
| Rust | native | `i8` to `i128`, `u8` to `u128`, `isize`, `usize`; `checked_*`, `wrapping_*`, `saturating_*` |
| C | native | `signed char` to `long long`; `<stdint.h>`; `<stdckdint.h>` |
| C++ | native | as C; `std::numeric_limits`, `std::bit_cast`, `std::cmp_less`, `std::byteswap` |
| Bash | native | fixed-width signed arithmetic in `$(( ))`, usually 64 bits, with no overflow check |
| PowerShell | native | .NET integer types such as `[int]`, `[long]`, `[byte]` |
| SQL | native | `SMALLINT`, `INTEGER`, `BIGINT`; overflow is an error |
| Regex | n/a | no numbers |
| HTML | n/a | no numbers |
| CSS | n/a | integer values with no stated width |

All the general-purpose languages in the table have integer types, so the verdict is native. They differ in three choices: which widths they name, whether they have unsigned types, and what happens on overflow. Java has no unsigned `int` or `long`, but its `Integer` and `Long` classes have unsigned methods. Python has one integer type with no width, and it gives fixed-width values only through `int.to_bytes` and modules such as `struct`, `array` and `ctypes`.

The example files show these choices. Python packs a value into bytes in each byte order. JavaScript reads one byte as unsigned and signed. Java shows the wrap of `int` and the error of `Math.addExact`. C# shows `checked` and `unchecked`. Go shows wraparound and the most negative value divided by −1. Rust shows the four kinds of addition. C shows `ckd_add` and the signed and unsigned comparison, and C++ shows `std::bit_cast` and `std::cmp_less`. The C and C++ files print bytes in the order of the machine, so their output is for a little-endian machine.

## Variations and related ideas

- **Sign-magnitude and ones' complement.** Floating-point numbers store their significand in sign-magnitude, and the Internet checksum adds 16-bit words in ones' complement. See [floating-point arithmetic](/floating-point-arithmetic/).
- **Offset binary.** A biased form stores *x* + *K*, so the bits sort in the same order as the values. The exponent of an IEEE 754 float is stored this way.
- **Arbitrary precision.** Python `int`, JavaScript `BigInt`, Java `BigInteger`, C# `BigInteger` and Go `math/big` store integers of any size as arrays of machine words.
- **Saturating arithmetic.** A result that is too large becomes the largest value. Signal processing and image code use it, and SIMD instruction sets have saturating instructions.
- **Bit-precise types.** C23 adds `_BitInt(N)` for any width up to a limit of the implementation, and hardware description languages have integers of any width.
- **Checked conversions.** Rust `try_from`, C++ `std::in_range` and Go code that compares against `math.MaxInt8` test that a value fits before a narrowing conversion. See [type conversion](/type-conversion/) and [parse an integer](/parse-an-integer/).

## Pitfalls

- **Signed overflow in C and C++.** The compiler may assume that signed overflow does not occur, so it can remove a check such as `if (x + 1 < x)`. Check before the operation, use `ckd_add`, or use unsigned arithmetic. See [integer overflow](/integer-overflow/).
- **The most negative value.** `abs`, negation and division by −1 of the most negative value have no answer in the same type. Java's `Math.abs(Integer.MIN_VALUE)` returns the negative value itself, and in C the operation is undefined behavior.
- **Signed and unsigned mixes.** In C and C++, `-1 < 1u` is false and `sizeof` values are unsigned, so `for (unsigned i = n - 1; i >= 0; i--)` never ends. Compiler warnings such as `-Wsign-compare` and `-Wtype-limits` find many of these.
- **Bytes that become negative.** Java's `byte` is signed, so a byte of 200 read from a file is −56, and `(int) b` keeps −56. Mask with `& 0xFF` to get 0 to 255.
- **Silent narrowing.** A cast to a smaller type drops the high bits with no error in C, C++, Java, C#, Go and Rust's `as`. Check the range first, or use a conversion that fails.
- **Division of negative numbers.** C, C++, Java, C#, Go and Rust round the integer quotient toward zero, so `-7 / 2` is −3 and `-7 % 2` is −1, and the JavaScript `%` operator has the same sign rule. Python rounds toward negative infinity, so `-7 // 2` is −4 and `-7 % 2` is 1.
- **Byte order in data.** Bytes copied from memory to a file or a socket carry the order of the machine. Convert with explicit functions, such as `int.to_bytes`, `DataView`, `binary.BigEndian` or `to_be_bytes`.

## Cost

An add, subtract, compare or shift of a machine integer is one instruction, which usually takes one cycle. Multiplication takes a few cycles, and division takes more. A checked operation adds a branch on the overflow flag, which the processor nearly always predicts correctly, but it can stop the compiler from vectorizing a loop. Wider types use more memory and cache: an array of `int64` is twice the size of an array of `int32`. Arbitrary-precision integers cost O(*n*) for addition of *n*-word values and more for multiplication.

## History

John von Neumann suggested two's complement in his *First Draft of a Report on the EDVAC* of 1945, and the EDSAC of 1949 used it[^12]. Other early machines chose differently: the IBM 7090 used sign-magnitude, and the PDP-1, the CDC 6600 and the UNIVAC 1100 series used ones' complement[^13]. The IBM System/360 of 1964 made two's complement the most widely used representation[^12], and the PDP-8, the PDP-11, the VAX and the Intel 8080 used it too[^13].

The C standard allowed all three representations[^5]. Java fixed two's complement and the widths of its types in its specification[^2]. JF Bastien's proposal P0907 of 2018 made two's complement the only representation in C++20[^5], and C23 followed[^4].

## References
