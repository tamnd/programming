---
tag: 9SF2JW
kind: concept
slug: ones-complement-and-sign-magnitude
name: Ones' complement and sign-magnitude
aliases: [ones' complement, ones complement, 1's complement, sign-magnitude, sign and magnitude, signed magnitude, negative zero, end-around carry, Internet checksum]
area: foundations/numbers-in-machines
level: advanced
summary: >-
  Sign-magnitude and ones' complement are two older ways to store signed integers. Each has a negative zero and a symmetric range, and both left the C and C++ standards for two's complement.
applies_to: core-languages
viz: optional
formal: none
see_also: [twos-complement, integer-representation, signed-and-unsigned-integers, floating-point-arithmetic, binary-number, bits-and-bytes]
same_as:
  wikidata: Q1361630
  wikipedia: Ones%27_complement
sources:
  - url: https://en.wikipedia.org/wiki/Signed_number_representations
    title: Signed number representations, Wikipedia
    locator: the introduction; History; Sign–magnitude; Ones' complement
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Ones%27_complement
    title: Ones' complement, Wikipedia
    locator: the introduction; Number representation; Basics; Negative zero
    accessed: 2026-11-16
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r4.html
    title: "P0907R4: Signed Integers are Two's Complement, JF Bastien, 2018"
    locator: Abstract; 1. Introduction; 6. Survey of Signed Integer Representations
    accessed: 2026-11-16
  - url: https://en.cppreference.com/c/language/arithmetic_types
    title: Arithmetic types, C reference, cppreference.com
    locator: Integer types, Properties
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/language/types
    title: Fundamental types, C++ reference, cppreference.com
    locator: Properties
    accessed: 2026-11-16
  - url: https://www.rfc-editor.org/rfc/rfc1071.txt
    title: "RFC 1071: Computing the Internet Checksum, Braden, Borman and Partridge, 1988"
    locator: 1. Introduction
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Constants
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Two%27s_complement
    title: Two's complement, Wikipedia
    locator: History
    accessed: 2026-11-16
---

## Idea

A signed integer type must show the sign of a value in its bits. Wikipedia lists "sign–magnitude, ones' complement, two's complement, and offset binary" as "The four best-known methods"[^1]. All of them store a value that is 0 or more in the usual binary form, and they differ for negative values:

- **Sign-magnitude.** The top bit is the sign, and the other bits are the absolute value. Wikipedia gives the example of 43, which is `00101011` in 8 bits, and -43, which is `10101011`[^1].
- **Ones' complement.** "In such a system, a number is negated (converted from positive to negative or vice versa) by computing its ones' complement."[^2] The ones' complement of a value is the value with each bit inverted.

```python run id=forms
BITS = 8


def sign_magnitude(n):
    return (0x80 if n < 0 else 0) | abs(n)


def ones_complement(n):
    return n if n >= 0 else ~(-n) & 0xFF


def twos_complement(n):
    return n & 0xFF


print("value  sign-magnitude  ones' complement  two's complement")
for n in (43, -43, 0, 127, -127, -128):
    sm = f"{sign_magnitude(n):08b}" if -127 <= n else "none"
    oc = f"{ones_complement(n):08b}" if -127 <= n else "none"
    tc = f"{twos_complement(n):08b}"
    print(f"{n:5}  {sm:>14}  {oc:>16}  {tc:>16}")
print("negative zero:", f"{0x80:08b}", f"{0xFF:08b}")
```

```output for=forms
value  sign-magnitude  ones' complement  two's complement
   43        00101011          00101011          00101011
  -43        10101011          11010100          11010101
    0        00000000          00000000          00000000
  127        01111111          01111111          01111111
 -127        11111111          10000000          10000001
 -128            none              none          10000000
negative zero: 10000000 11111111
```

Both older forms have two zeros: `00000000` and a negative zero. In 8 bits, the negative zero is `10000000` in sign-magnitude and `11111111` in ones' complement. Both forms also have a symmetric range, -127 to 127 in 8 bits, so -128 has no form. Two's complement has one zero and the range -128 to 127.

## How it works

**Sign-magnitude.** The form is like a number written with a minus sign. "This approach is directly comparable to the common way of showing a sign (placing a "+" or "−" next to the number's magnitude)."[^1] Negation inverts only the top bit. Addition is harder: "Addition and subtraction require different behavior depending on the sign bit"[^1]. For example, the sum 5 + (-3) becomes the subtraction 5 - 3 of the magnitudes.

**Ones' complement.** Negation inverts each bit, so `-x` has the same bits as `~x`. "That is, inverting all of the bits of a number (the logical complement) produces the same result as subtracting the value from 0."[^2] An *N*-bit form holds -(2ᴺ⁻¹ - 1) to 2ᴺ⁻¹ - 1. Addition uses a binary adder with one extra step. When a carry goes out of the top bit, "the bit must be added back in at the right-most bit"[^2]. This step is the *end-around carry*.

```python run id=end-around
def add_ones(a, b, bits=8):
    mask = (1 << bits) - 1
    total = a + b
    if total > mask:  # a carry left the top bit
        total = (total & mask) + 1  # the end-around carry
    return total & mask


def value(bits):
    return -(~bits & 0xFF) if bits & 0x80 else bits


four, minus_three = 0b00000100, 0b11111100
s = add_ones(four, minus_three)
print(f"{four:08b} + {minus_three:08b} = {s:08b} = {value(s)}")
s = add_ones(0b00000101, 0b11111010)
print(f"5 + -5 = {s:08b} = {value(s)}")
```

```output for=end-around
00000100 + 11111100 = 00000001 = 1
5 + -5 = 11111111 = 0
```

The second sum gives the negative zero. A program must treat both zeros as equal: "the existence of two forms of the same value necessitates two comparisons when checking for equality with zero."[^1]

**Why two's complement won.** Two's complement needs no end-around carry and no sign test, and it has one zero. Wikipedia says that "Two's complement is the easiest to implement in hardware, which may be the ultimate reason for its widespread popularity."[^1] Sign-magnitude machines also had a cost: "These systems did ones' complement math internally, so numbers would have to be converted to ones' complement values when they were transmitted from a register to the math unit"[^1].

**The standards.** C and C++ allowed all three forms for a long time. The proposal P0907 says that C11 "allows three representations for signed integral types"[^3], and that "None of [MSVC], [GCC], and [LLVM] support other representations."[^3] The old minimum range of an 8-bit signed type was "-127 to 127"[^4], which "corresponds to the limits of ones' complement or sign-and-magnitude"[^5]. As of C++20, two's complement "is the only representation allowed by the standard"[^5], and C23 did the same[^4].

## In languages

| Language | Verdict | Where the forms appear |
|---|---|---|
| Python | idiom | bit operations by hand; `-0.0` is a sign-magnitude float |
| JavaScript | idiom | bit operations by hand; `-0` and `Object.is` |
| TypeScript | idiom | as JavaScript |
| Java | idiom | bit operations by hand; `-0.0` and `Double.compare` |
| Kotlin | idiom | bit operations by hand; `-0.0` |
| C# | idiom | bit operations by hand; `-0.0` and `double.IsNegative` |
| Go | idiom | bit operations by hand; `math.Copysign` and `math.Signbit` |
| Rust | idiom | bit operations by hand; `-0.0` and `is_sign_negative` |
| C | idiom | allowed for `int` before C23; `signbit` for floats |
| C++ | idiom | allowed for `int` before C++20; `std::signbit` for floats |
| Bash | idiom | bit operations in `$(( ))` |
| PowerShell | idiom | the .NET operators |
| SQL | n/a | SQL does not show the bits of a value |
| Regex | n/a | no numbers |
| HTML | n/a | no numbers |
| CSS | n/a | no integer types with a width |

No language in use today stores its integers in these forms, so a program that reads such data converts it by hand. Two places still use them. Floating-point numbers store their significand in sign-magnitude: "Sign–magnitude is the most common way of representing the significand in floating-point values."[^1] So each language with IEEE 754 floats has a `-0.0` that is equal to `0.0` but has another sign bit. The Internet checksum uses ones' complement addition.

The example files show the same two facts in each language. They convert 8-bit values to sign-magnitude and to ones' complement with bit operations, and they show the negative zero of a float.

## Variations and related ideas

- **Two's complement.** The form that replaced both. See [[twos-complement]].
- **Offset binary.** A value is stored as *x* + *K*, so the bits sort in the order of the values. The exponent of an IEEE 754 float uses this form. Wikipedia: "The IEEE 754 floating-point standard defines the exponent field of a single-precision (32-bit) number as an 8-bit excess-127 field."[^1]
- **The Internet checksum.** IP, UDP and TCP use a ones' complement sum. RFC 1071 says: "Adjacent octets to be checksummed are paired to form 16-bit integers, and the 1's complement sum of these 16-bit integers is formed."[^6] A receiver checks the sum: "If the result is all 1 bits (-0 in 1's complement arithmetic), the check succeeds."[^6]
- **Negative zero in floats.** IEEE 754 floats have `+0.0` and `-0.0`. They compare equal, but `1 / -0.0` is negative infinity. The example files show this. See [[floating-point-arithmetic]].

## Pitfalls

- **Two zeros.** Code that compares bits instead of values sees two different zeros. In Java, `-0.0 == 0.0` is true, but `Double.compare` puts `-0.0` before `0.0`, as the Java example shows.
- **A negative zero constant that is not negative.** In Go, the constant `-0.0` is positive zero, because Go has "no constants denoting the IEEE 754 negative zero, infinity, and not-a-number values"[^7]. The Go example uses `math.Copysign(0, -1)` instead.
- **Reading old data.** Data from a machine with another signed form has the wrong value if it is read as two's complement. In 8 bits, the ones' complement `11111110` is -1, but in two's complement it is -2.
- **Thinking that C still allows them.** Code that tests for ones' complement or sign-magnitude, such as a check of `-1 & 3`, is dead code under C23 and C++20.

## Cost

Sign-magnitude needs a sign test and a subtractor for addition, and it converts values for the adder on some machines[^1]. Ones' complement needs the end-around carry, and Wikipedia says: "This phenomenon does not occur in two's complement arithmetic."[^2] Both forms need two comparisons to test for zero[^1]. Two's complement has none of these costs.

## History

"One of the great debates was the format of negative numbers"[^1] in the early days of computing. "IBM was one of the early supporters of sign–magnitude, with their 704, 709 and 709x series computers being perhaps the best-known systems to use it."[^1] Ones' complement machines included the UNIVAC 1101, the CDC 160, the CDC 1604, the CDC 6600, the LINC and the DEC PDP-1[^2]. "Successors of the CDC 6600 continued to use ones' complement arithmetic until the late 1980s"[^2]. The descendants of the UNIVAC 1107 "continue to use ones' complement"[^2], but current Unisys ClearPath Dorado machines emulate that instruction set on two's complement processors[^2].

John von Neumann suggested two's complement in 1945[^8]. The IBM System/360 of 1964 "made two's complement the most widely used binary representation in the computer industry."[^8] The Internet checksum of RFC 1071 (1988) still uses ones' complement sums[^6]. P0907 of 2018 removed the other forms from C++20[^3][^5], and C23 followed[^4].

## References
