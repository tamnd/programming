---
tag: Z0P99C
kind: feature
owner: python
slug: numeric-literals
name: Numeric literals
aliases: [integer literals, float literals, imaginary literals, 0x 0o 0b prefixes, underscores in numbers, Python number syntax]
area: languages/python/syntax
level: intro
summary: >-
  Python has integer, floating-point and imaginary literals. Integers can be decimal, binary, octal or hex, underscores can group digits, and a literal has no sign.
grammar: "NUMBER: integer | floatnumber | imagnumber (Language Reference, 2.6 Numeric literals)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#numeric-literals", locator: "2.6 Numeric literals"}
see_also: [python/string-literals, python/identifiers-in-python]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#numeric-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.6 Numeric literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#integer-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.6.1 Integer literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#floating-point-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.6.2 Floating-point literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#imaginary-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.6.3 Imaginary literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/floatingpoint.html
    title: The Python Tutorial, Python 3.15
    locator: "15 Floating-Point Arithmetic: Issues and Limitations"
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Integer_literals
    title: The Go Programming Language Specification
    locator: Integer literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=number-forms
print(2147483647, 1_000_000, 0b1010, 0o17, 0xDead_Beef)
print(2.5, 10., .001, 1e3, 6.02214076e+23, 96_485.332_123)
print(4.2j, 10j, 3 + 4.2j)
```

```output for=number-forms
2147483647 1000000 10 15 3735928559
2.5 10.0 0.001 1000.0 6.02214076e+23 96485.332123
4.2j 10j (3+4.2j)
```

## Semantics

**r1.** There are three kinds of numeric literal: integers, floating-point numbers and imaginary numbers. The value of a literal is the value that `int`, `float` or `complex` gives for the same text, but not every valid input of those constructors is a valid literal.[^1]

**r2.** A numeric literal has no sign. `-1` is an expression: the unary operator `-` and the literal `1`.[^1] Example: [`no-sign`](#no-sign).

**r3.** An integer literal has no limit of length "apart from what can be stored in available memory". The prefixes `0b`, `0o` and `0x` give binary, octal and hexadecimal, and the hex digits `A` to `F` are case-insensitive.[^2] Example: [`number-forms`](#number-forms).

**r4.** Single underscores can group the digits, and they do not change the value. An underscore can only be between two digits, or just after a base prefix: `0x_1f` is valid, but `_123`, `321_` and `123__321` are not.[^2] The same rule applies to the digits of floats.[^3]

**r5.** A non-zero decimal integer cannot start with `0`, so `0123` is an error. The rule avoids confusion with the C-style octal literals that Python used before version 3.0. A float can have leading zeros: `077.010` is `77.01`.[^2] [^3] Example: [`leading-zero`](#leading-zero).

**r6.** A float literal has an integer part and a fraction part around a `.`, and one of them can be empty. An exponent `e` or `E`, with an optional sign, means "times ten raised to the power of". With an exponent, the point can be left out: `1e3` is a float.[^3]

**r7.** Python has no complex literals. An imaginary literal is a float or an integer part followed by `j` or `J`, and it denotes a complex number with a zero real part, so `3+4.2j` is three tokens. The number before `j` is always a float.[^4] Example: [`imaginary`](#imaginary).

## Examples

The types of the literals (rules r6 and r7). `1e3` and `10j` have no point, but they are not integers.

```python run id=imaginary
for value in [1000, 1e3, 10j, 3 + 4.2j]:
    print(f"{value!r:10} {type(value).__name__}")
print((10j).imag, (3 + 4.2j).real)
```

```output for=imaginary
1000       int
1000.0     float
10j        complex
(3+4.2j)   complex
10.0 3.0
```

The minus sign is an operator (rule r2). The tokenizer gives two tokens for `-1`.

```python run id=no-sign
import io
import tokenize

for token in tokenize.generate_tokens(io.StringIO("-1\n").readline):
    if token.string.strip():
        print(tokenize.tok_name[token.type], repr(token.string))
```

```output for=no-sign
OP '-'
NUMBER '1'
```

## Pitfalls

### A leading zero

A number copied from a fixed-width field, such as a month `08`, is an error as a literal (rule r5). Write `8`, or `0o10` for an octal value.

```python fails id=leading-zero expect=exception:SyntaxError
month = 08
```

### Decimal fractions are approximate

Most decimal fractions "cannot be represented exactly as binary fractions", so the float literal `0.1` is close to one tenth, not equal to it.[^5]

```python wrong id=float-sum
print(0.1 + 0.1 + 0.1 == 0.3)
print(0.1 + 0.1 + 0.1)
```

```output for=float-sum
False
0.30000000000000004
```

Compare with a tolerance, such as `math.isclose`, or use the `decimal` module for exact decimal arithmetic.[^5]

## Comparison

In Go, a leading `0` is an octal prefix, with `0o` and `0O`: the spec gives "0, 0o, or 0O for octal".[^6] So `0123` is 83 in Go and an error in Python (rule r5). Both languages allow an underscore after a base prefix and between digits.
