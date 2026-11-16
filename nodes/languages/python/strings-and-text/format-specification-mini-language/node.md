---
tag: RZMPRN
kind: feature
owner: python
slug: format-specification-mini-language
name: The format specification mini-language
aliases: [format spec, format specifier, format(), __format__, fill and align, width and precision, thousands separator, presentation type]
area: languages/python/strings-and-text
level: core
summary: >-
  A format spec such as >10.2f sets the fill, alignment, sign, width, grouping, precision and type of one value. f-strings, str.format() and format() use it, and a class can read it in __format__.
grammar: "format_spec: [options][width_and_precision][type] (string module, Format specification mini-language)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/string.html#format-specification-mini-language", locator: "Format specification mini-language"}
see_also: [python/formatted-string-literals, python/the-string-format-method, python/manual-string-formatting, python/float-type]
sources:
  - url: https://docs.python.org/3.15/library/string.html#format-specification-mini-language
    title: string, Common string operations
    locator: Format specification mini-language
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#format
    title: Built-in Functions, Python 3.15 documentation
    locator: format()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__format__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.1. Basic customization, object.__format__
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt
    title: Package fmt, Go standard library
    locator: Printing, width and precision
    accessed: 2026-11-16
---

## Synopsis

```python run id=parts
print(f"[{'left':<8}] [{'right':>8}] [{'mid':*^9}]")
print(f"[{42:+d}] [{42:06d}] [{-42:=6d}] [{255:#x}] [{5:08b}]")
print(f"[{1234567.891:,.2f}] [{0.4967:.1%}] [{12345.678:.3e}] [{1/3:.3g}]")
```

```output for=parts
[left    ] [   right] [***mid***]
[+42] [000042] [-   42] [0xff] [00000101]
[1,234,567.89] [49.7%] [1.235e+04] [0.333]
```

Each spec starts after the colon. The parts come in a fixed order: fill and align, sign, `#`, `0`, width, grouping, precision, type.

## Semantics

**r1.** "The general form of a standard format specifier is: format_spec: [options][width_and_precision][type]".[^1] "Each formattable type may define how the format specification is to be interpreted."[^1] "A general convention is that an empty format specification produces the same result as if you had called str() on the value."[^1]

**r2.** "If a valid align value is specified, it can be preceded by a fill character that can be any character and defaults to a space if omitted."[^1] The align values are `<` (the default for most objects), `>` (the default for numbers), `=` (padding after the sign) and `^` (centered).[^1]

**r3.** The sign option is "only valid for number types".[^1] `+` shows a sign for all numbers, `-` only for negative numbers, and a space gives a leading space for positive numbers.[^1]

**r4.** "The width is a decimal integer defining the minimum total field width, including any prefixes, separators, and other formatting characters."[^1] "When no explicit alignment is given, preceding the width field by a zero ('0') character enables sign-aware zero-padding for numeric types, excluding complex."[^1]

**r5.** The precision "is a decimal integer indicating how many digits should be displayed after the decimal point for presentation types 'f' and 'F', or before and after the decimal point for presentation types 'g' or 'G'. For string presentation types the field indicates the maximum field size".[^1] "The precision is not allowed for integer presentation types."[^1] Examples: [`truncate`](#truncate), [`int-precision`](#int-precision).

**r6.** The grouping option `,` or `_` inserts a separator every 3 digits for type `d` and the float types. "For integer presentation types 'b', 'o', 'x', and 'X', underscores are inserted every 4 digits."[^1]

**r7.** The type sets the presentation. For integers, `b`, `c`, `d`, `o`, `x`, `X` and `n`. For floats, `e`, `E`, `f`, `F`, `g`, `G`, `n` and `%`. "'%' Percentage. Multiplies the number by 100 and displays in fixed ('f') format, followed by a percent sign."[^1] "The rounding mode for float matches that of the round() builtin."[^1]

**r8.** "A call to format(value, format_spec) is translated to type(value).__format__(value, format_spec)".[^2] "The interpretation of the format_spec argument is up to the type implementing __format__()".[^3] Example: [`custom`](#custom).

## Examples

```python run id=truncate
name = "Bartholomew"
print(f"[{name:.4}] [{name:10.4}] [{name:>10.4}]")
print(format(3.14159, ".2f"), format(1234, "_"), format(0xDEADBEEF, "_x"))
```

```output for=truncate
[Bart] [Bart      ] [      Bart]
3.14 1_234 dead_beef
```

For a string, the precision cuts the text (rule r5). For hex, the underscore comes every 4 digits (rule r6).

```python run id=floats
for x in [0.0000123, 123.456, 1e20]:
    print(f"{x:f} | {x:.3e} | {x:g} | {x}")
print(f"{2.5:.0f} {3.5:.0f} {float('inf'):F}")
```

```output for=floats
0.000012 | 1.230e-05 | 1.23e-05 | 1.23e-05
123.456000 | 1.235e+02 | 123.456 | 123.456
100000000000000000000.000000 | 1.000e+20 | 1e+20 | 1e+20
2 4 INF
```

`f` gives 6 places by default. `g` chooses fixed or scientific form by the size of the number. `.0f` rounds 2.5 to 2 and 3.5 to 4, as round() does (rule r7).

```python run id=custom
class Temperature:
    def __init__(self, celsius):
        self.celsius = celsius

    def __format__(self, spec):
        if spec.endswith("F"):
            return format(self.celsius * 9 / 5 + 32, spec[:-1] + "f") + "°F"
        return format(self.celsius, spec or "g") + "°C"

t = Temperature(21.5)
print(f"{t} {t:.0F} {t:>8.1f}")
```

```output for=custom
21.5°C 71°F     21.5°C
```

The class reads its own spec. Here a final `F` means Fahrenheit, and the rest of the spec goes to the float (rule r8).

## Pitfalls

An integer type does not take a precision:

```python fails id=int-precision expect=exception:ValueError
f"{42:.2d}"
```

Use `f"{42:.2f}"` to show decimal places, or `f"{42:05d}"` to pad with zeros.

A spec for a number fails on a string:

```python fails id=spec-on-str expect=exception:ValueError
value = "3.5"
f"{value:.1f}"
```

Convert the text first: `f"{float(value):.1f}"`.

## Comparison

Go `fmt` puts width and precision in the verb: "Width is specified by an optional decimal number immediately preceding the verb."[^4] "Precision is specified after the (optional) width by a period followed by a decimal number."[^4] So `%9.2f` in Go is like `9.2f` in Python. Python adds fill characters, centering and grouping, and a type can define its own spec with `__format__` (rule r8).
