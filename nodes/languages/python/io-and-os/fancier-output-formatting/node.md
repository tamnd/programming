---
tag: QQ0JAP
kind: feature
owner: python
slug: fancier-output-formatting
name: Fancier Output Formatting
aliases: [output formatting in Python, ways to format output, string formatting choices]
area: languages/python/io-and-os
level: intro
summary: >-
  Python has five ways to format output: f-strings, str.format(), manual padding with string methods, the % operator and string.Template. Each one fits a different case.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/inputoutput.html#fancier-output-formatting", locator: "7.1. Fancier output formatting"}
see_also: [python/formatted-string-literals, python/the-string-format-method, python/manual-string-formatting, python/old-string-formatting, python/repr-and-str, python/templating, python/format-specification-mini-language, print-formatted-output]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#fancier-output-formatting
    title: The Python Tutorial, 7. Input and output
    locator: 7.1. Fancier output formatting; 7.1.1 to 7.1.4
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#printf-style-string-formatting
    title: Built-in Types, Python 3.15 documentation
    locator: printf-style String Formatting
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt#Sprintf
    title: Package fmt, Go standard library
    locator: func Sprintf; Printing
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
name, score = 'Ada', 0.875
print(f'{name:>5}: {score:.1%}')
print('{:>5}: {:.1%}'.format(name, score))
print(name.rjust(5) + ': ' + str(round(score * 100, 1)) + '%')
print('%5s: %.1f%%' % (name, score * 100))
```

```output for=synopsis
  Ada: 87.5%
  Ada: 87.5%
  Ada: 87.5%
  Ada: 87.5%
```

The four lines give the same text with an f-string, `str.format()`, string methods and the `%` operator.

## Semantics

**r1.** "There are several ways to format output."[^1] The tutorial shows them in this order: formatted string literals, the `str.format()` method, manual string formatting and the `%` operator.[^1]

**r2.** A formatted string literal starts "with f or F before the opening quotation mark or triple quotation mark. Inside this string, you can write a Python expression between { and } characters that can refer to variables or literal values."[^1] See [formatted string literals](python/formatted-string-literals).

**r3.** "The str.format() method of strings requires more manual effort. You’ll still use { and } to mark where a variable will be substituted and can provide detailed formatting directives, but you’ll also need to provide the information to be formatted."[^1] See [the string format method](python/the-string-format-method).

**r4.** "Finally, you can do all the string handling yourself by using string slicing and concatenation operations to create any layout you can imagine."[^1] Methods such as `str.rjust()`, `str.ljust()`, `str.center()` and `str.zfill()` pad a string. See [manual string formatting](python/manual-string-formatting).

**r5.** "The % operator (modulo) can also be used for string formatting."[^1] See [old string formatting](python/old-string-formatting).

**r6.** The `string.Template` class substitutes values for named placeholders. It "offers much less control for formatting."[^1] See [templating](python/templating).

**r7.** For a quick view of values, use `str()` or `repr()`. "The str() function is meant to return representations of values which are fairly human-readable, while repr() is meant to generate representations which can be read by the interpreter (or will force a SyntaxError if there is no equivalent syntax)."[^1] See [repr and str](python/repr-and-str).

**r8.** The format specifiers after the `:` in f-strings and `str.format()` are the same mini-language. See [the format specification mini-language](python/format-specification-mini-language).

The choice for each case:

| Case | Use |
|---|---|
| Values in the code, the usual case | f-string |
| The template is data, made before the values exist | `str.format()` with a stored string |
| The template comes from a user or a translator | `string.Template` |
| Code that must look like C printf, or older code | the `%` operator |
| A quick debug print | `f'{x=}'`, or `repr()` |

## Examples

```python run id=table
for x in range(1, 4):
    print(f'{x:2d} {x*x:3d} {x*x*x:4d}')
for x in range(1, 4):
    print('{0:2d} {1:3d} {2:4d}'.format(x, x*x, x*x*x))
```

```output for=table
 1   1    1
 2   4    8
 3   9   27
 1   1    1
 2   4    8
 3   9   27
```

The two loops make the same table of squares and cubes as the tutorial (rules r2 and r3). A number after the `:` sets the width of the field.

```python run id=debug
count = 13
area = 'living room'
print(f'{count=} {area=}')
print(f'{area!r} {area!s}')
```

```output for=debug
count=13 area='living room'
'living room' living room
```

"The = specifier can be used to expand an expression to the text of the expression, an equal sign, then the representation of the evaluated expression"[^1]. The conversions `!r` and `!s` apply `repr()` and `str()` (rule r7).

```python run id=stored
row = '{name:<8}|{qty:>4}'
print(row.format(name='apples', qty=3))
print(row.format(name='pears', qty=12))
```

```output for=stored
apples  |   3
pears   |  12
```

`str.format()` fits a template that the program keeps in a variable (rule r3). An f-string cannot do this, because Python evaluates it where it is written.

## Pitfalls

The padding methods do not cut long values:

```python run id=no-truncate
print(repr('Antarctica'.ljust(6)))
print(repr('Antarctica'.ljust(6)[:6]))
```

```output for=no-truncate
'Antarctica'
'Antarc'
```

The tutorial says: "If the input string is too long, they don’t truncate it, but return it unchanged; this will mess up your column lay-out but that’s usually better than the alternative, which would be lying about a value."[^1] To cut the text, add a slice, as in `x.ljust(n)[:n]`.

A tuple is not one value for `%`:

```python fails id=percent-tuple expect=exception:TypeError
point = (2, 3)
print('point: %s' % point)
```

The `%` operator takes the tuple as two values for one specifier. The docs warn that this operator has "a variety of quirks that lead to a number of common errors (such as failing to display tuples and dictionaries correctly)."[^2] Write `'point: %s' % (point,)`, or use an f-string.

## Comparison

Go uses verbs in a format string, as the `%` operator of Python does. "Sprintf formats according to a format specifier and returns the resulting string."[^3] Go has no string literal that holds expressions, so it has no form like the f-string.
