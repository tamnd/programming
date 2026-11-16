---
tag: Z2G8QS
kind: feature
owner: python
slug: formatted-string-literals
name: Formatted String Literals
aliases: [f-string, f-strings, formatted string literal, string interpolation, f prefix, debug specifier, self-documenting expression]
area: languages/python/strings-and-text
level: intro
summary: >-
  An f-string has an f prefix and holds expressions in braces. Python evaluates each expression at run time and puts its formatted value in the string. A field can add =, a conversion and a format spec.
grammar: "fstring: FSTRING_START fstring_middle* FSTRING_END (Language Reference, 2.5.9)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#formatted-string-literals", locator: "2.5.7. f-strings"}
see_also: [python/string-literals, python/format-specification-mini-language, python/the-string-format-method, python/template-string-literals, python/repr-and-str]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#formatted-string-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.5.7. f-strings
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#formatted-string-literals-f-strings
    title: Built-in Types, Python 3.15 documentation
    locator: Formatted String Literals (f-strings); Debug specifier; Conversion specifier; Format specifier
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#formatted-string-literals
    title: The Python Tutorial, 7. Input and Output
    locator: 7.1.1. Formatted String Literals
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0498/
    title: "PEP 498: Literal String Interpolation"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0701/
    title: "PEP 701: Syntactic formalization of f-strings"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt
    title: Package fmt, Go standard library
    locator: func Sprintf
    accessed: 2026-11-16
---

## Synopsis

```python run id=fields
import math
name, count = "eels", 13
print(f"My hovercraft is full of {name}.")
print(f"{name!r} {count * 2} {math.pi:.3f}")
print(f"{count=} {name.upper()=}")
```

```output for=fields
My hovercraft is full of eels.
'eels' 26 3.142
count=13 name.upper()='EELS'
```

Each pair of braces holds an expression. A field can add a conversion such as `!r`, a format spec after `:`, or `=`.

## Semantics

**r1.** "A formatted string literal or f-string is a string literal that is prefixed with ‘f’ or ‘F’. Unlike other string literals, f-strings do not have a constant value."[^1] "Replacement fields contain expressions which are evaluated at run time."[^1] PEP 498 says that "an f-string is really an expression evaluated at run time, not a constant value."[^4]

**r2.** "Any doubled curly braces ({{ or }}) outside replacement fields are replaced with the corresponding single curly brace".[^1] Example: [`braces`](#braces).

**r3.** "Each expression is evaluated in the context where the formatted string literal appears, in order from left to right. An empty expression is not allowed, and both lambda and assignment expressions := must be surrounded by explicit parentheses".[^1] Examples: [`order`](#order), [`empty-field`](#empty-field).

**r4.** "Reusing the outer f-string quoting type inside a replacement field is permitted".[^1] "Backslashes are also allowed in replacement fields and are evaluated the same way as in any other context".[^1] PEP 701 lifts "some of the restrictions originally formulated in PEP 498".[^5] Example: [`nested-quotes`](#nested-quotes).

**r5.** With a debug specifier `=`, "the resulting f-string will contain the expression’s source, the equal sign, and the value of the expression."[^2] "When a debug specifier but no format specifier is used, the default conversion instead uses repr()".[^2]

**r6.** "By default, the value of a replacement field expression is converted to a string using str()".[^2] The conversions `!s`, `!r` and `!a` call str(), repr() and ascii().[^2] "If the replacement field includes a format specifier introduced by a colon (:), the specifier is passed to format() as the second argument."[^2]

**r7.** "Top-level format specifiers may include nested replacement fields".[^1] Example: [`nested-spec`](#nested-spec).

**r8.** "Formatted string literals cannot be used as docstrings, even if they do not include expressions".[^1]

## Examples

```python run id=braces
items = 3
print(f"{{items}} = {items}")
print(f"{{{items}}}")
```

```output for=braces
{items} = 3
{3}
```

Two braces give one literal brace (rule r2).

```python run id=nested-quotes
data = {"x": 2}
words = ["a", "b", "c"]
print(f"value {data["x"]}")
print(f"list:\n{"\n".join(words)}")
```

```output for=nested-quotes
value 2
list:
a
b
c
```

The field uses the same double quotes as the f-string, and it holds a backslash (rule r4).

```python run id=nested-spec
number = 14.3
width, precision = 10, 3
print(f"[{number:{width}.{precision}f}]")
print(f"[{number:<{width}}]")
```

```output for=nested-spec
[    14.300]
[14.3      ]
```

The inner fields give the width and the precision (rule r7).

```python run id=debug
value = 14.3
print(f"{value=}")
print(f"{ value - 4 = }")
print(f"{value = :.1f}")
print(f"{'tea'=} {'tea'=!s}")
```

```output for=debug
value=14.3
 value - 4 = 10.3
value = 14.3
'tea'='tea' 'tea'=tea
```

The spaces around `=` stay in the result. With `=` alone, the value uses repr(), and `!s` changes that (rule r5).

## Pitfalls

The expressions run when the f-string is evaluated, not later:

```python wrong id=order
name = "Ada"
greeting = f"Hello, {name}"
name = "Grace"
print(greeting)
```

```output for=order
Hello, Ada
```

The f-string has a value at once (rule r1). To fill a pattern later, use `str.format()` or a function.

An empty field is a syntax error:

```python fails id=empty-field expect=exception:SyntaxError
print(f"value: {}")
```

An f-string is not a docstring:

```python run id=no-docstring
def area():
    f"Return the area."
    return 1

print(area.__doc__)
```

```output for=no-docstring
None
```

## Comparison

Go has no string interpolation in the language. A program calls `fmt.Sprintf`, which "formats according to a format specifier and returns the resulting string."[^6] The values come after the format string, as with Python `%` formatting. A Python f-string puts each expression in its field (rule r1).
