---
tag: 4TZD91
kind: standard
slug: ieee-754
name: IEEE 754
aliases: [IEEE 754-2019, IEEE Standard for Floating-Point Arithmetic, ISO/IEC 60559, IEC 559, binary64, binary32, binary16]
area: foundations/numbers-in-machines
level: core
summary: >-
  IEEE 754 is the standard for floating-point arithmetic. It defines the binary and decimal formats, the rounding rules, the special values and the exceptions that almost all hardware and languages use.
see_also: [floating-point-arithmetic, rounding-modes, nan-and-infinity, subnormal-numbers, machine-epsilon, floating-point-rounding-error, binary-coded-decimal, bits-and-bytes]
same_as:
  wikidata: Q828287
sources:
  - url: https://standards.ieee.org/ieee/754/6210/
    title: IEEE 754-2019, IEEE Standard for Floating-Point Arithmetic, IEEE Standards Association
    locator: the scope text, Status, PAR Approval, Board Approval and History
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/IEEE_754
    title: IEEE 754, Wikipedia
    locator: the lead; History; Basic and interchange formats; Rounding rules; Required operations; Comparison predicates; Total-ordering predicate; Exception handling; Character representation
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number
    title: Number, MDN Web Docs
    locator: Number encoding
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Double-precision_floating-point_format
    title: Double-precision floating-point format, Wikipedia
    locator: IEEE 754 double-precision binary floating-point format, binary64; Exponent encoding
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Single-precision_floating-point_format
    title: Single-precision floating-point format, Wikipedia
    locator: IEEE 754 standard, binary32
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/struct.html
    title: struct, Interpret bytes as packed binary data, Python Software Foundation
    locator: Format Characters, notes 6 and 7
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/IEEE_754-1985
    title: IEEE 754-1985, Wikipedia
    locator: History
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/IEEE_754-2008_revision
    title: IEEE 754-2008 revision, Wikipedia
    locator: the lead; Summary of the revisions; Clause 5 Operations
    accessed: 2026-11-16
  - url: https://en.wikipedia.org/wiki/Half-precision_floating-point_format
    title: Half-precision floating-point format, Wikipedia
    locator: IEEE 754 half-precision binary floating-point format, binary16; Uses of half precision
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.double
    title: Double Struct, Microsoft Learn
    locator: Remarks
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.decimal
    title: Decimal Struct, Microsoft Learn
    locator: Remarks
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/decimal.html
    title: decimal, Decimal fixed-point and floating-point arithmetic, Python Software Foundation
    locator: the function IEEEContext
    accessed: 2026-11-16
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Math/f16round
    title: Math.f16round(), MDN Web Docs
    locator: Description; Examples
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/27/docs/api/java.base/java/lang/Float.html
    title: Float (Java SE 27 and JDK 27), Oracle
    locator: the methods floatToRawIntBits, floatToFloat16 and float16ToFloat
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/types/numeric_limits/is_iec559
    title: std::numeric_limits<T>::is_iec559, cppreference.com
    locator: the description
    accessed: 2026-11-16
  - url: https://en.cppreference.com/cpp/types/floating-point
    title: Fixed width floating-point types, cppreference.com
    locator: the table of types; Notes
    accessed: 2026-11-16
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.half
    title: Half Struct, Microsoft Learn
    locator: Remarks
    accessed: 2026-11-16
  - url: https://pkg.go.dev/math
    title: math package, The Go Authors
    locator: func Float32bits, func Float64bits, and func FMA
    accessed: 2026-11-16
  - url: https://doc.rust-lang.org/std/primitive.f64.html
    title: Primitive Type f64, The Rust Project
    locator: the description; the constant MANTISSA_DIGITS; the method to_bits
    accessed: 2026-11-16
  - url: https://www.postgresql.org/docs/current/datatype-numeric.html
    title: PostgreSQL Documentation, 8.1. Numeric Types, The PostgreSQL Global Development Group
    locator: 8.1.3 Floating-Point Types
    accessed: 2026-11-16
---

## What it is

IEEE 754 is the IEEE Standard for Floating-Point Arithmetic. The current edition is IEEE 754-2019. Its scope text says: "This standard specifies interchange and arithmetic formats and methods for binary and decimal floating-point arithmetic in computer programming environments."[^1] The same text adds that "numerical results and exceptions are uniquely determined by the values of the input data, sequence of operations, and destination formats"[^1]. So two conforming systems that do the same operations on the same formats give the same bits.

The standard defines five things[^2]:

- **Arithmetic formats.** These are sets of binary and decimal numbers: finite numbers (with signed zeros and subnormal numbers), infinities and NaN values.
- **Interchange formats.** These are the bit strings that store a value and move it between systems.
- **Rounding rules.** These say which representable value replaces an exact result.
- **Operations.** These are the arithmetic and other operations on the formats.
- **Exception handling.** These are the indications of conditions such as division by zero and overflow.

Almost every language that has a float type uses an IEEE 754 format for it. A JavaScript number "is a double-precision 64-bit binary format IEEE 754 value, like double in Java or C#"[^3]. The page [[floating-point-arithmetic]] explains the idea of a floating-point number. This page is about the standard itself.

## Syntax or structure

### Formats

A binary format has three fields: a sign bit, a biased exponent and a trailing significand (the fraction). For binary64, the standard specifies "Sign bit: 1 bit", "Exponent: 11 bits" and "Significand precision: 53 bits (52 explicitly stored)"[^4]. The first bit of the significand is not stored for normal numbers, because it is always 1.

The standard defines "five basic formats". There are "three binary floating-point basic formats (encoded with 32, 64 or 128 bits) and two decimal floating-point basic formats (encoded with 64 or 128 bits)"[^2]. It also defines interchange formats of other widths, such as binary16. The table shows the common formats[^2]:

| Name | Common name | Bits | Significand bits | Exponent bits | Largest finite value | Role |
|---|---|---|---|---|---|---|
| binary16 | half precision | 16 | 11 | 5 | 65504 | interchange |
| binary32 | single precision | 32 | 24 | 8 | about 3.40e38 | basic |
| binary64 | double precision | 64 | 53 | 11 | about 1.80e308 | basic |
| binary128 | quadruple precision | 128 | 113 | 15 | about 1.19e4932 | basic |
| decimal64 | | 64 | 16 decimal digits | | about 1.0e385 | basic |
| decimal128 | | 128 | 34 decimal digits | | about 1.0e6145 | basic |

The exponent field holds the exponent plus a bias. For binary32, "a value of 127 represents the actual exponent zero", and the field values 0 and 255 "are reserved for special numbers (subnormal numbers, signed zeros, infinities, and NaNs)"[^5]. The bias of binary64 is 1023[^4]. The encoding of the special values is the same in every binary format:

| Exponent field | Fraction field | Value |
|---|---|---|
| all zeros | zero | +0 or -0, by the sign bit |
| all zeros | not zero | a subnormal number |
| neither all zeros nor all ones | any | a normal number |
| all ones | zero | +infinity or -infinity |
| all ones | not zero | a NaN |

A decimal format stores its significand in one of two encodings. The standard allows "a compressed sequence of decimal digits using densely packed decimal or, alternatively, as a binary integer"[^2].

### Rounding

The standard defines five rounding rules. "Round to nearest, ties to even" is the default for binary formats[^2]. The other rules are round to nearest with ties away from zero, and the three directed roundings toward 0, toward +infinity and toward -infinity[^2]. The page [[rounding-modes]] shows them in each language.

The basic operations are correctly rounded. In the words of the summary of the standard, the result "is determined by applying the rounding function on the infinitely precise (mathematical) result", and "Such an operation is said to be correctly rounded"[^2].

### Operations

The required operations of each format include conversions to and from integers, the next and previous values, "add, subtract, multiply, divide, square root, fused multiply–add, remainder, minimum, maximum", conversions to and from strings, sign operations, comparisons and total ordering, classification, and the status flags[^2].

Two parts of this list surprise programmers:

- **Comparisons.** "Any comparison with a NaN is treated as unordered", and "−0 and +0 compare as equal"[^2]. The page [[nan-and-infinity]] shows the consequences.
- **totalOrder.** This predicate gives a total order. In it, "Negative zero is treated as smaller than positive zero", and NaN sorts after the infinities[^2].

### Exceptions

"The standard defines five exceptions, each of which returns a default value and has a corresponding status flag that is raised when the exception occurs"[^2]:

| Exception | Example | Default result |
|---|---|---|
| invalid operation | the square root of a negative number | a quiet NaN |
| division by zero | 1/0, or log(0) | +infinity or -infinity |
| overflow | a result above the largest finite value | +infinity or -infinity, in the round-to-nearest modes |
| underflow | a result below the smallest normal value | a subnormal number or zero |
| inexact | 1/3 | the correctly rounded result |

By default, an exception does not stop the program. The operation gives the default result, sets a flag and continues. Traps are optional[^2].

### Conversion to text

Conversion between a binary format and a decimal string is a required operation. A conversion to text must be such that the conversion back "will recover the original number"[^2]. The number of significant decimal digits that always recovers the value is 5 for binary16, 9 for binary32, 17 for binary64 and 36 for binary128[^2].

## Examples

The program below reads the fields of three numbers in binary16, binary32 and binary64. The module `struct` packs a float into each format. For the codes `f`, `d` and `e`, "the packed representation uses the IEEE 754 binary32, binary64 or binary16 format", whatever the native format of the platform is[^6]. The program then unpacks the same bytes as an unsigned integer and cuts the integer into its fields.

```python run id=fields
import struct

# name: (float code, integer code, exponent bits, fraction bits)
FORMATS = {
    "binary16": ("e", "H", 5, 10),
    "binary32": ("f", "I", 8, 23),
    "binary64": ("d", "Q", 11, 52),
}


def fields(x, name):
    code, ucode, ebits, fbits = FORMATS[name]
    bits = struct.unpack(">" + ucode, struct.pack(">" + code, x))[0]
    sign = bits >> (ebits + fbits)
    exponent = (bits >> fbits) & ((1 << ebits) - 1)
    fraction = bits & ((1 << fbits) - 1)
    return bits, sign, exponent, fraction


print("format     value bits              s exp fraction")
for name, (_, _, ebits, fbits) in FORMATS.items():
    bias = (1 << (ebits - 1)) - 1
    digits = (1 + ebits + fbits) // 4
    for x in [1.0, -2.5, 0.1]:
        bits, s, e, f = fields(x, name)
        print(f"{name:8} {x:7} {bits:0{digits}x} {s:>{18 - digits}} {e - bias:3} {f:x}")

print("special values in binary16:")
for x in [65504.0, 2.0 ** -24, float("inf"), -0.0, float("nan")]:
    print(f"  {x!r:>21} {fields(x, 'binary16')[0]:04x}")
try:
    struct.pack(">e", 65520.0)
except OverflowError as err:
    print("  65520.0 does not fit:", type(err).__name__)
```

```output for=fields
format     value bits              s exp fraction
binary16     1.0 3c00              0   0 0
binary16    -2.5 c100              1   1 100
binary16     0.1 2e66              0  -4 266
binary32     1.0 3f800000          0   0 0
binary32    -2.5 c0200000          1   1 200000
binary32     0.1 3dcccccd          0  -4 4ccccd
binary64     1.0 3ff0000000000000  0   0 0
binary64    -2.5 c004000000000000  1   1 4000000000000
binary64     0.1 3fb999999999999a  0  -4 999999999999a
special values in binary16:
                65504.0 7bff
  5.960464477539063e-08 0001
                    inf 7c00
                   -0.0 8000
                    nan 7e00
  65520.0 does not fit: OverflowError
```

The output shows the structure of the formats:

- The value 1.0 has the exponent 0 and the fraction 0 in each format. Only the bias changes, so the stored exponent field is 15, 127 or 1023.
- The value 0.1 has no exact binary form. Each format stores the fraction 0x999... rounded to its width, so the last hex digit is `6`, `d` or `a`. The page [[floating-point-rounding-error]] explains why.
- In binary16, 65504 is the largest finite value, 2 to the power -24 is the smallest subnormal value, and infinity, -0 and NaN have the encodings of the table above.
- 65520 is exactly halfway between 65504 and the next power of two, 65536. Ties to even rounds it up to 65536, which does not fit, so the result is infinity. The function `struct.pack` gives an error instead of infinity.

The example files of this page read the same fields in the other tier-1 languages. Each uses the function of its language that gives the bits of a float.

## Versions

The standard has three editions[^2][^1]:

| Edition | Published | Main changes |
|---|---|---|
| IEEE 754-1985 | 1985 | binary formats only: single, double and extended precision |
| IEEE 754-2008 | August 2008 | decimal formats, binary16 and binary128, fused multiply-add, the merge of IEEE 854-1987 |
| IEEE 754-2019 | July 2019 | clarifications, defect fixes and new recommended operations |

Before 1985, each computer maker had its own float format. The summary of the history says: "IBM used a hexadecimal floating-point format with seven bits always used for the exponent regardless of precision", and "Exception processing from divide-by-zero was different on different computers"[^2]. The Intel 8087, "which was announced in 1980, was the first chip to implement the draft standard"[^7]. William Kahan of the University of California drew up the floating-point specifications for the 8087. He then put forward a draft for the IEEE working group "based on his work for their coprocessor", written with Jerome Coonen and Harold Stone[^7]. This "Kahan-Coonen-Stone proposal" brought infinities, NaN values and denormal (subnormal) numbers into the standard[^7].

The 2008 edition "added decimal arithmetic and formats" and "merged in IEEE 854 (the radix-independent floating-point standard)"[^8]. Its new operations "include fused multiply–add (FMA), explicit conversions, classification predicates (isNan(x), etc.), various min and max functions, a total ordering predicate, and two decimal-specific operations"[^8]. The binary16 type also comes from this edition: it "was introduced in the 2008 revision of the IEEE 754 standard"[^6]. It has a sign bit, 5 exponent bits and 11 significand bits, and its largest value is 65504[^9]. "Hardware and software for machine learning and neural networks often use half-precision arithmetic to accelerate computations"[^9].

The 2019 edition is "a minor revision of the previous version, incorporating mainly clarifications, defect fixes and new recommended operations"[^2]. The IEEE board approved it on 2019-06-13, and the IEEE published it on 2019-07-22[^1]. Its status is "Superseding 754-2008"[^1]. The international standard ISO/IEC 60559:2020 has the same content as IEEE 754-2019[^2]. Older language standards cite the earlier names. For example, the C# documentation says that "The Double type complies with the IEC 60559:1989 (IEEE 754) standard for binary floating-point arithmetic"[^10].

## Pitfalls

- **A short decimal string does not always give back the same float.** Only 17 significant digits are always sufficient for binary64[^2]. With 15 digits, the value of 0.1 + 0.2 changes. The last line shows that `repr` gives the 17 digits for this value:

```python wrong id=digits
x = 0.1 + 0.2
print(f"{x:.15g}", float(f"{x:.15g}") == x)
print(f"{x:.17g}", float(f"{x:.17g}") == x)
print(repr(x))
```

```output for=digits
0.3 False
0.30000000000000004 True
0.30000000000000004
```

- **Conformance is not the same in every library.** The basic operations are correctly rounded, but functions such as `sin` and `exp` are only recommended. A summary of the standard notes that the C mathematical functions are usually not correctly rounded[^2]. So the same program can print different last digits on two systems.
- **Extended precision changes results.** Some compilers keep intermediate results in a wider format, such as the x87 80-bit format. The result is still IEEE 754 arithmetic, but it differs from the result in binary64[^2].
- **The decimal type of a language is often not an IEEE 754 decimal format.** The C# `decimal` is "128-bits consisting of a 96-bit integer number, and a 32-bit set of flags"[^11]. This is not decimal128. The Python module `decimal` has the function `IEEEContext`, which returns a context "initialized to the proper values for one of the IEEE interchange formats"[^12].
- **A NaN payload is not portable.** The standard has "no requirement to preserve the payload of a quiet NaN or signaling NaN" in a conversion to text[^2]. Do not store data in the bits of a NaN.

## Tools and libraries

Each tier-1 language has a function that gives the bits of a float. These functions are the base of the example files:

- Python: the module `struct`, with the codes `e`, `f` and `d`[^6].
- JavaScript: `DataView` and the typed arrays, and `Math.f16round`, which "returns the nearest 16-bit half precision float representation of a number"[^13].
- Java: `Float.floatToRawIntBits`, `Double.doubleToRawLongBits`, and `Float.floatToFloat16`, which "Returns the floating-point binary16 value, encoded in a short", that is nearest to the argument[^14].
- C and C++: `memcpy` or `std::bit_cast` to an integer of the same width. In C++, `std::numeric_limits<T>::is_iec559` "is true for all floating-point types T which fulfill the requirements of IEC 559 (IEEE 754) standard"[^15]. The header `<stdfloat>` adds `std::float16_t`, `std::float32_t`, `std::float64_t` and `std::float128_t` if the implementation supports them[^16].
- C#: `BitConverter.DoubleToInt64Bits`, and the type `System.Half`, which "is an IEEE 754-compliant float16 type"[^17].
- Go: `math.Float32bits` and `math.Float64bits`, which return "the IEEE 754 binary representation"[^18].
- Rust: the method `to_bits`. The type `f64` is "the “binary64” type defined in IEEE 754-2008"[^19].
- SQL: in PostgreSQL, the types `real` and `double precision` "are implementations of IEEE Standard 754 for Binary Floating-Point Arithmetic (single and double precision, respectively)" on the supported platforms[^20].

## References
