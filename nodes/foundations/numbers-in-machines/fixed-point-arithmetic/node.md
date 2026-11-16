---
tag: KZCXPG
kind: concept
slug: fixed-point-arithmetic
name: Fixed-point arithmetic
aliases: [fixed-point, fixed point, fixed-point number, binary scaling, scaled integer, Q format, Q notation, Q15, Q16.16, _Fract, _Accum]
area: foundations/numbers-in-machines
level: advanced
summary: >-
  A fixed-point number is an integer with an implied scale, such as cents for money or 1/65536 in a Q16.16 value. Addition is integer addition, and a product needs a rescale and a wider type.
applies_to: core-languages
viz: optional
formal: none
see_also: [integer-representation, floating-point-arithmetic, overflow-semantics, integer-division-and-modulo, twos-complement, integer-overflow]
same_as:
  wikidata: Q649900
  wikipedia: Fixed-point_arithmetic
sources:
  - url: https://en.wikipedia.org/wiki/Fixed-point_arithmetic
    title: Fixed-point arithmetic, Wikipedia
    locator: the introduction; Representation; Operations; Hardware support; Computer language implementations; Software application examples
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Q_(number_format)
    title: Q (number format), Wikipedia
    locator: the introduction; Multiplication
    accessed: 2026-11-16
  - url: https://gcc.gnu.org/onlinedocs/gcc/Fixed-Point.html
    title: Fixed-Point Types, Using the GNU Compiler Collection (GCC)
    locator: the whole page
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/math/BigDecimal.html
    title: BigDecimal, Java SE 27 API
    locator: the class description; setScale
    accessed: 2026-11-16
---

## Idea

A fixed-point number is an integer with a scale that the program knows but does not store. Wikipedia: "the value 1.23 can be stored in a variable as the integer value 123 with an implicit scaling factor of 1/100"[^1]. "Dollar amounts, for example, are often stored with exactly two fractional digits, representing the cents (1/100 of a dollar)."[^1] The scale is a negative power of the base, and "The most common variants are decimal (base 10) and binary (base 2)."[^1]

The hardware sees only integers. "Internally, however, there is no separation, and the distinction between the two groups of digits is defined only by the programs that handle such numbers."[^1] So the program must keep track of the scale of each value.

```python run id=q16
SHIFT = 16
ONE = 1 << SHIFT


def to_fix(x):
    return round(x * ONE)


def to_float(f):
    return f / ONE


def mul(a, b):
    return (a * b + (ONE >> 1)) >> SHIFT


def fits_int32(v):
    return -(2**31) <= v < 2**31


a, b = to_fix(1.5), to_fix(2.25)
print("1.5 as Q16.16:", a)
print("1.5 * 2.25:", to_float(mul(a, b)))
t = to_fix(0.1)
print("0.1 as Q16.16:", t, "=", to_float(t))
print("10 * 0.1:", to_float(t * 10))
big = to_fix(300.0)
print("300 * 300 fits in Q16.16:", "yes" if fits_int32(mul(big, big)) else "no")
print("raw 300 * 300 fits in 64 bits:", "yes" if -(2**63) <= big * big < 2**63 else "no")
```

```output for=q16
1.5 as Q16.16: 98304
1.5 * 2.25: 3.375
0.1 as Q16.16: 6554 = 0.100006103515625
10 * 0.1: 1.00006103515625
300 * 300 fits in Q16.16: no
raw 300 * 300 fits in 64 bits: yes
```

The block uses a binary scale of 1/65536, which is 16 fraction bits. 1.5 and 2.25 have exact binary fractions, so their product is exact. 0.1 is not exact in binary, and the error grows when the value is used 10 times. 90000 does not fit in the 32-bit Q16.16 format, but the raw product fits in 64 bits.

## How it works

**Addition and subtraction.** Two values with the same scale add as integers. These operations "yield the exact mathematical result, as long as no overflow occurs"[^1], and "If overflow happens, it occurs like with ordinary integers of the same signedness."[^1] Two values with different scales must first be converted to one scale.

**Multiplication.** The product of the two integers has the product of the two scales. "The result will be exact, with no rounding, provided that it does not overflow the receiving variable."[^1] Wikipedia's example: 0.123, stored as 123 with scale 1/1000, times 2.5, stored as 25 with scale 1/10, gives 3075 with scale 1/10000, which is 0.3075[^1]. The product needs more bits: "the product is up to twice the width of the two factors, so to accurately represent the result, the receiving variable requires twice the width as the operands"[^1]. To bring the product back to the scale of the operands, the program divides it by the scale. "In binary, it is common to use a scaling factor that is a power of two. After the multiplication, the scaling factor can be divided away by shifting right."[^1]

**Rounding after the rescale.** A right shift or an integer division gives "a flooring division"[^1]. To round half up, the program adds "a 'rounding addend' of half of the scaling factor before shifting"[^1]. The `mul` function in the first block does this with `ONE >> 1`.

**Division.** A quotient of two fixed-point values has the quotient of the two scales, and much of the precision can be lost. Wikipedia's example: 34.56 with scale 1/100, divided by 1.234 with scale 1/1000, gives the integer 3 with scale 10, which is 30[^1]. So a program scales the dividend up before it divides.

**Q notation.** Binary fixed-point formats are often named in Q notation. "Q7.8 means that the signed fixed point numbers in this format have 7 bits for the integer part and 8 bits for the fraction part"[^2], with one more bit for the sign, so Q7.8 is a 16-bit word[^2]. "There is an ARM variation of the Q notation that explicitly adds the sign bit to the integer part."[^2] In it, the same format is Q8.8[^2]. The Q multiplication code in Wikipedia multiplies two 16-bit values into a 32-bit value, adds a rounding constant, shifts, and saturates the result to the 16-bit range[^2].

## In languages

| Language | Verdict | Fixed-point support |
|---|---|---|
| Python | idiom | an `int` with a scale; `decimal` for decimal values |
| JavaScript | idiom | a `number` or `bigint` with a scale |
| TypeScript | idiom | as JavaScript |
| Java | idiom | a `long` with a scale; `BigDecimal` has a scale |
| Kotlin | idiom | as Java |
| C# | idiom | a `long` with a scale; `decimal` |
| Go | idiom | an `int64` with a scale |
| Rust | idiom | an `i64` with a scale; crates |
| C | idiom | an integer with a scale; GCC `_Fract` and `_Accum` on some targets |
| C++ | idiom | an integer with a scale, often in a class |
| Bash | idiom | an integer with a scale in `$(( ))` |
| PowerShell | idiom | `[decimal]` or an integer with a scale |
| SQL | native | `NUMERIC` and `DECIMAL` with a precision and a scale |
| Regex | n/a | no arithmetic |
| HTML | n/a | no arithmetic |
| CSS | n/a | no fixed-point type |

Older languages had fixed-point types: Wikipedia names "COBOL, Ada, JOVIAL, and Coral 66"[^1], and says that "More modern languages usually do not offer any fixed-point data types or support for scaling factor conversion."[^1] In their place, "they can be implemented by the programmer, with explicit scaling conversion, in any programming language that supports explicit integer types"[^1]. That is the idiom in each language above. On the other hand, "all relational databases and the SQL notation support fixed-point decimal arithmetic and storage of numbers"[^1].

C has a proposal for fixed-point types. "As an extension, GNU C supports fixed-point types as defined in the N1169 draft of ISO/IEC DTR 18037."[^3] The types are `_Fract` and `_Accum`, with `_Sat` forms that saturate[^3], but "Not all targets support fixed-point types."[^3]

Java's `BigDecimal` is a decimal type with a scale that can change: it "consists of an arbitrary precision integer unscaled value and a 32-bit integer scale"[^4]. Its `setScale` method changes the scale, and it multiplies or divides the unscaled value "by the appropriate power of ten to maintain its overall value"[^4]. A program that keeps every `BigDecimal` at scale 2 uses it as a decimal fixed-point type for money.

## Variations and related ideas

- **Decimal and binary scale.** A decimal scale, such as 1/100, holds amounts of money exactly. A binary scale, such as 1/65536, lets the program rescale with a shift.
- **Saturation.** A fixed-point operation can clamp the result to the largest or smallest value in place of a wrap. The Q multiplication code in Wikipedia does this[^2], and the GCC `_Sat` types do it too[^3]. See [[overflow-semantics]].
- **Integer multiples of a unit.** The term also covers "representing fractional values as integer multiples of some fixed small unit, e.g., a fractional amount of hours as an integer multiple of ten-minute intervals"[^1].
- **Floating point.** A floating-point number stores its scale as an exponent in each value. See [[floating-point-arithmetic]].

## Pitfalls

- **Overflow in the product.** The product of two values needs about twice the bits of each value. A Q16.16 multiplication in 32 bits overflows for most values. Multiply in a 64-bit type, then shift.
- **Mixed scales.** An addition of a value in cents to a value in dollars gives a wrong result with no error, because the hardware sees only integers. Keep the scale in the type name or in a wrapper type.
- **Truncation in the rescale.** A right shift rounds toward minus infinity, and C integer division rounds toward zero (see [[integer-division-and-modulo]]). Both give a small bias that grows in a long loop. Add the rounding addend first.
- **A decimal value in a binary scale.** 0.1 has no exact binary fraction, so a binary fixed-point format rounds it, as the first block shows. Money needs a decimal scale.
- **A float in the middle.** A conversion of a fixed-point value to a float and back can change the last digit. Keep the arithmetic in integers.

## Cost

Fixed-point arithmetic is integer arithmetic, plus a shift or a division to rescale a product. "Fixed-point number representation is often contrasted to the more complicated and computationally demanding floating-point representation."[^1] Since most processors now have a fast floating-point unit, Wikipedia says that fixed point is used "only in special situations, such as in low-cost embedded microprocessors and microcontrollers; in applications that demand high speed or low power consumption or small chip area, like image, video, and digital signal processing; or when their use is more natural for the problem"[^1]. It "is still the norm for field-programmable gate array (FPGA) implementations"[^1].

## History

"Fixed-point representation was the norm in mechanical calculators."[^1] COBOL, Ada, JOVIAL and Coral 66 had fixed-point types, and "Most of those languages were designed between 1955 and 1990."[^1] In 2008, ISO published a draft technical report that adds the `_Fract` and `_Accum` types to C "for the benefit of programs running on embedded DSP processors"[^1]. "The popular TrueType font format uses 32-bit signed binary fixed-point with 26 bits to the left of the decimal for some numeric values in its instructions."[^1]

## References
