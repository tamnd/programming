---
tag: NJHT9M
kind: feature
owner: python
slug: string-literals
name: String literals
aliases: [Python string literal, quotes in Python, triple-quoted string, escape sequences, string prefixes]
area: languages/python/syntax
level: intro
summary: >-
  A Python string literal is text in single, double or triple quotes, with an optional prefix such as r, b, f or t. Backslash escapes give special characters, and adjacent literals join into one.
grammar: "STRING: [stringprefix] (stringcontent) (Language Reference, 2.5.3 Formal grammar)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#string-and-bytes-literals", locator: "2.5 String and Bytes literals"}
see_also: [python/raw-strings, python/bytes-literals, python/line-structure, python/source-code-encoding]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#string-and-bytes-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.5 String and Bytes literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#triple-quoted-strings
    title: The Python Language Reference, Python 3.15
    locator: 2.5.1 Triple-quoted strings
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#string-prefixes
    title: The Python Language Reference, Python 3.15
    locator: 2.5.2 String prefixes; 2.5.3 Formal grammar
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#escape-sequences
    title: The Python Language Reference, Python 3.15
    locator: 2.5.4 Escape sequences
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#unrecognized-escape-sequences
    title: The Python Language Reference, Python 3.15
    locator: 2.5.4.7 Unrecognized escape sequences
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#string-literal-concatenation
    title: The Python Language Reference, Python 3.15
    locator: 6.2.3.2 String literal concatenation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str
    title: Built-in Types, Python 3.15 documentation
    locator: Text Sequence Type, str
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/introduction.html#text
    title: The Python Tutorial, Python 3.15
    locator: 3.1.2 Text
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#String_literals
    title: The Go Programming Language Specification
    locator: String literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=quotes
single = 'Say "Hello", please.'
double = "Don't do that!"
escaped = "Say \"Hello\" to everyone!"
triple = """Line one
Line two with 'quotes' and "quotes"."""
print(single)
print(double)
print(escaped)
print(triple)
```

```output for=quotes
Say "Hello", please.
Don't do that!
Say "Hello" to everyone!
Line one
Line two with 'quotes' and "quotes".
```

## Semantics

**r1.** A string literal is text in single quotes or double quotes. The quote that starts the literal also ends it, and apart from that, the choice of quote "does not affect how the literal is parsed".[^1] Example: [`quotes`](#quotes).

**r2.** A literal can also be in three single or three double quotes. Inside it, quotes are allowed, except three of the starting kind in a row, and unescaped newlines are kept.[^2] Example: [`quotes`](#quotes).

**r3.** An optional prefix changes how the content is read: `b` for a bytes literal, `r` for a raw string, `f` for an f-string, `t` for a template string, and `u`, which has no effect. Prefixes are case-insensitive, and `r` can be combined with `f`, `t` or `b`. The prefix must be followed by the quote with no space.[^3]

**r4.** Without an `r` prefix, a backslash starts an escape sequence. The recognized sequences are `\` and a newline, `\\`, `\'`, `\"`, `\a`, `\b`, `\f`, `\n`, `\r`, `\t`, `\v`, an octal `\ooo` of up to three digits, a hex `\xhh` of exactly two digits, `\N{name}`, `\uxxxx` and `\Uxxxxxxxx`.[^4] Example: [`escapes`](#escapes).

**r5.** An unrecognized escape is left in the string unchanged: "the backslash is left in the result". In CPython 3.12 and later, it also gives a `SyntaxWarning`.[^5] Example: [`unknown-escape`](#unknown-escape).

**r6.** Adjacent string literals, "possibly using different quoting conventions", mean the same as their concatenation. This happens at the syntactic level, so it works only with literals.[^6] Example: [`concatenation`](#concatenation).

**r7.** A string literal makes a `str` object. Strings "are immutable sequences of Unicode code points", and there is no separate character type: indexing a string gives a string of length 1.[^7]

## Examples

Each escape gives one character (rule r4). `repr` shows the backslash and the newline as escapes, and `len` counts the characters.

```python run id=escapes
text = "a\\b\tc\n\x50\120\N{GREEK SMALL LETTER ALPHA}\u00e9"
print(repr(text))
print(len(text))
```

```output for=escapes
'a\\b\tc\nPPαé'
10
```

A backslash at the end of a line inside the string removes the newline (rule r4), and the tutorial uses it to start a triple-quoted text on the next line.[^8]

```python run id=triple
usage = """\
Usage: tool [OPTIONS]
  -h  Show this message
"""
print(usage, end="")
print(repr(usage[:6]))
```

```output for=triple
Usage: tool [OPTIONS]
  -h  Show this message
'Usage:'
```

Adjacent literals join, also when they are on several lines in brackets (rule r6).

```python run id=concatenation
pattern = ("[A-Za-z_]"       # letter or underscore
           "[A-Za-z0-9_]*")  # letters, digits or underscores
print(pattern)
print("spam " 'eggs' == "spam eggs")
```

```output for=concatenation
[A-Za-z_][A-Za-z0-9_]*
True
```

## Versions

> [!since v="3.12"]
> An unrecognized escape sequence gives a `SyntaxWarning`. Older versions gave a `DeprecationWarning`. The reference says that in a future version it will raise a `SyntaxError`.[^5]

## Pitfalls

### A Windows path in a normal string

`\n` and `\t` are escapes (rule r4), so a path such as `C:\new\table` loses two backslashes and gains a newline and a tab. Double the backslashes, or use a raw string.

```python wrong id=windows-path
path = "C:\new\table.txt"
print(len(path))
print(repr(path))
```

```output for=windows-path
14
'C:\new\table.txt'
```

The fixed forms are `"C:\\new\\table.txt"` and `r"C:\new\table.txt"`.

### An unknown escape keeps the backslash, with a warning

`\d` is not an escape, so `"\d+"` holds a backslash (rule r5). The result looks right, but CPython 3.12 warns about it when it compiles the code. Regular expressions should use raw strings.

```python run id=unknown-escape
import warnings

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    value = eval(r"'\d+'")
print(list(value))
print(caught[0].category.__name__, caught[0].message)
```

```output for=unknown-escape
['\\', 'd', '+']
SyntaxWarning invalid escape sequence '\d'
```

## Comparison

Go has two forms of string literal. A raw string literal is in back quotes, and in it "backslashes have no special meaning and the string may contain newlines". An interpreted string literal is in double quotes, and it cannot contain a newline.[^9] Python has four quote forms for one kind of literal (rules r1 and r2), and the `r` prefix gives the raw form (rule r3).
