---
tag: 2KMHH1
kind: feature
owner: python
slug: old-string-formatting
name: Old string formatting
aliases: [percent formatting, percent operator, modulo operator on strings, printf-style formatting, string interpolation operator, sprintf in Python]
area: languages/python/strings-and-text
level: core
summary: >-
  format % values replaces each % specifier in the format string with a value, as C sprintf does. More than one value goes in a tuple, and named specifiers take a dict. A tuple value needs care.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#printf-style-string-formatting", locator: "printf-style String Formatting"}
see_also: [python/formatted-string-literals, python/the-string-format-method, python/format-specification-mini-language, python/manual-string-formatting]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#printf-style-string-formatting
    title: Built-in Types, Python 3.15 documentation
    locator: printf-style String Formatting
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#old-string-formatting
    title: The Python Tutorial, 7. Input and Output
    locator: 7.1.4. Old string formatting
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt
    title: Package fmt, Go standard library
    locator: Printing
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import math
print("%s has %d quote types." % ("Python", 2))
print("The value of pi is approximately %5.3f." % math.pi)
print("%(language)s has %(number)03d quote types." % {"language": "Python", "number": 2})
print("%-6s|%6s|%x|%#o|%e|%%" % ("ab", "cd", 255, 8, 1234.5))
```

```output for=basics
Python has 2 quote types.
The value of pi is approximately 3.142.
Python has 002 quote types.
ab    |    cd|ff|0o10|1.234500e+03|%
```

Each `%` specifier takes one value. `%%` gives one percent sign and takes no value.

## Semantics

**r1.** "String objects have one unique built-in operation: the % operator (modulo). This is also known as the string formatting or interpolation operator."[^1] "Given format % values (where format is a string), % conversion specifications in format are replaced with zero or more elements of values. The effect is similar to using the sprintf() function in the C language."[^1]

**r2.** "If format requires a single argument, values may be a single non-tuple object."[^1] "Otherwise, values must be a tuple with exactly the number of items specified by the format string, or a single mapping object (for example, a dictionary)."[^1] Examples: [`tuple-trap`](#tuple-trap), [`count`](#count).

**r3.** A specifier has these parts in this order: the `%`, an optional mapping key in parentheses, optional flags, an optional minimum width, an optional precision after a dot, an optional length modifier, and the conversion type.[^1] A width or precision of `*` is read from the next item of the tuple.[^1] Example: [`star`](#star).

**r4.** The flags are `#` (the alternate form), `0` (zero padding), `-` (left adjusted), a space (a blank before a positive number) and `+` (a sign).[^1] "A length modifier (h, l, or L) may be present, but is ignored".[^1]

**r5.** The conversion types include `d`, `i`, `o`, `x`, `X`, `e`, `E`, `f`, `F`, `g`, `G` and `c`. `r`, `s` and `a` convert any object with repr(), str() and ascii().[^1] For a string conversion, "If precision is N, the output is truncated to N characters."[^1]

**r6.** "The formatting operations described here exhibit a variety of quirks that lead to a number of common errors (such as failing to display tuples and dictionaries correctly). Using formatted string literals, the str.format() interface, or string.Template may help avoid these errors."[^1]

## Examples

```python run id=star
width, precision = 10, 2
print("[%*.*f]" % (width, precision, 3.14159))
print("[%.3s] [%5.1s]" % ("Python", "xyz"))
print("[%+d] [% d] [%c%c]" % (5, 5, 72, "i"))
```

```output for=star
[      3.14]
[Pyt] [    x]
[+5] [ 5] [Hi]
```

The two `*` take the width and the precision from the tuple (rule r3). `%.3s` cuts the text to 3 characters (rule r5).

```python run id=mapping
row = {"name": "Ada", "score": 97.5}
print("%(name)-8s %(score)6.1f %(name)r" % row)
```

```output for=mapping
Ada        97.5 'Ada'
```

With a dict, each specifier names its key, and a key can be used more than once.

## Pitfalls

A tuple value is taken as the list of values:

```python fails id=tuple-trap expect=exception:TypeError
point = (1, 2)
print("point: %s" % point)
```

The tuple gives two values for one specifier. Wrap it in a tuple of one item: `"point: %s" % (point,)`.

```python run id=tuple-fix
point = (1, 2)
print("point: %s" % (point,))
print(f"point: {point}")
```

```output for=tuple-fix
point: (1, 2)
point: (1, 2)
```

The number of values must match the number of specifiers:

```python fails id=count expect=exception:TypeError
"%s and %s" % ("spam",)
```

## Comparison

Go `fmt.Printf` comes from the same C tradition: "Package fmt implements formatted I/O with functions analogous to C's printf and scanf."[^3] Go takes the values as separate arguments, so a tuple trap cannot happen. Go has the verb `%v` for any value, where Python uses `%s` (rule r5).
