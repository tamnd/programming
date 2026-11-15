---
tag: N4DB6C
kind: feature
owner: python
slug: int-type
name: The int type
aliases: [Python int, Python integers, arbitrary-precision integers, bignum, int(), int.to_bytes, int.bit_length]
area: languages/python/values-and-types
level: intro
summary: >-
  int holds whole numbers with no fixed size, so it never overflows. int() converts numbers and strings in bases 2 to 36, and bitwise operators act as if negative numbers had infinite sign bits.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex", locator: "Numeric Types - int, float, complex"}
see_also: [python/numeric-literals, python/bitwise-and-shift-operators, python/bool-type, integer-representation, integer-overflow]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types - int, float, complex; Bitwise Operations on Integer Types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#additional-methods-on-integer-types
    title: Built-in Types, Python 3.15 documentation
    locator: Additional Methods on Integer Types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#int
    title: Built-in Functions, Python 3.15 documentation
    locator: int()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#integer-string-conversion-length-limitation
    title: Built-in Types, Python 3.15 documentation
    locator: Integer string conversion length limitation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#numbers-integral
    title: The Python Language Reference, Python 3.15
    locator: 3.2.4.1. numbers.Integral, Integers (int)
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Numeric_types
    title: The Go Programming Language Specification
    locator: Numeric types; Integer overflow
    accessed: 2026-11-16
---

## Synopsis

```python run id=big-int
x = 2 ** 100
print(x)
print(x.bit_length(), type(x).__name__)
print(int("ff", 16), int("0b101", 0), int(" -12_345\n"), int(-3.99))
```

```output for=big-int
1267650600228229401496703205376
101 int
255 5 -12345 -3
```

`2 ** 100` is exact, with no overflow. `int()` reads strings in other bases and truncates a float toward zero.

## Semantics

**r1.** "Integers have unlimited precision."[^1] An `int` represents numbers "in an unlimited range, subject to available (virtual) memory only".[^5] Example: [`big-int`](#big-int).

**r2.** Integer literals, including hex, octal and binary literals, give integers.[^1] `int()` with no argument returns `0`. With a number, it returns `x.__int__()` or `x.__index__()`, and for a floating-point number it "truncates towards zero".[^3] Example: [`big-int`](#big-int).

**r3.** `int()` with a string, or with a `base`, reads an integer in that base. The string can have a sign, leading zeros, whitespace around it and single underscores between digits. The allowed bases are 0 and 2 to 36. Base 0 reads the string like an integer literal in code, so the prefix `0b`, `0o` or `0x` selects the base, and leading zeros are not allowed.[^3] Example: [`not-an-integer-string`](#not-an-integer-string).

**r4.** The bitwise operations are "calculated as though carried out in two's complement with an infinite number of sign bits". A negative shift count raises `ValueError`. A left shift by `n` bits equals a multiplication by `pow(2, n)`, and a right shift by `n` bits equals a floor division by `pow(2, n)`.[^1] Example: [`bitwise-negative`](#bitwise-negative).

**r5.** `int` implements the `numbers.Integral` abstract base class and adds methods.[^2] `bit_length()` returns "the number of bits necessary to represent an integer in binary, excluding the sign and leading zeros". `bit_count()` returns the number of ones in the binary form of the absolute value. `to_bytes()` and `from_bytes()` convert between an integer and an array of bytes, with a byte order and an optional two's complement sign.[^2] Example: [`int-methods`](#int-methods).

**r6.** CPython limits conversions between `int` and `str` in decimal and other bases that are not a power of two, to "mitigate denial of service attacks". The default limit is 4300 digits, and a longer conversion raises `ValueError`. Hexadecimal, octal and binary conversions are unlimited, and `sys.set_int_max_str_digits()` changes the limit.[^4] Example: [`str-limit`](#str-limit).

## Examples

`bit_length()`, `bit_count()` and the byte conversions (rule r5). The examples come from Built-in Types.

```python run id=int-methods
n = -37
print(bin(n), n.bit_length(), (19).bit_count())
print((1024).to_bytes(2, byteorder="big"))
print(int.from_bytes(b"\xfc\x00", byteorder="big", signed=True))
```

```output for=int-methods
-0b100101 6 3
b'\x04\x00'
-1024
```

A negative number acts as if it had an infinite row of 1 bits on the left (rule r4).

```python run id=bitwise-negative
print(-1 & 0xFF, -8 >> 1, ~5, 1 << 70)
try:
    1 << -1
except ValueError:
    print("negative shift: ValueError")
```

```output for=bitwise-negative
255 -4 -6 1180591620717411303424
negative shift: ValueError
```

A product of two 4300-digit numbers can be computed, but `str()` refuses to convert it to decimal. `hex()` still works (rule r6).

```python run id=str-limit
import sys

i = int("2" * 4300)
print(len(str(i)), sys.int_info.default_max_str_digits)
square = i * i
try:
    str(square)
except ValueError:
    print("str(square): ValueError")
print(len(hex(square)))
```

```output for=str-limit
4300 4300
str(square): ValueError
7144
```

## Pitfalls

### `int()` truncates, it does not floor

`int(-3.99)` is `-3`, because `int()` truncates toward zero (rule r2). Use `math.floor()` to round down.

```python wrong id=int-truncates
import math

print(int(-3.99), math.floor(-3.99), int(3.99), math.floor(3.99))
```

```output for=int-truncates
-3 -4 3 3
```

### `int("3.5")` is an error

A string for `int()` must hold an integer in the given base (rule r3). Convert with `float()` first, or use `round()`.

```python fails id=not-an-integer-string expect=exception:ValueError
print(int("3.5"))
```

### `to_bytes()` with too few bytes

The default length is one byte, and a value that does not fit raises `OverflowError`. A negative value with `signed=False` raises `OverflowError` too.[^2]

```python fails id=to-bytes-too-small expect=exception:OverflowError
print((256).to_bytes())
```

## Comparison

Go has integer types of fixed sizes, from `int8` and `uint8` to `int64` and `uint64`, and an n-bit integer is "represented using two's complement arithmetic". Unsigned arithmetic wraps around, and signed overflow gives a defined value: "Overflow does not cause a run-time panic."[^6] A Python `int` has no size, so `2 ** 100` is exact and no operation wraps around (rule r1).
