---
tag: 4VSBKP
kind: feature
owner: python
slug: bytes-literals
name: Bytes literals
aliases: [b prefix, b'' literal, Python bytes literal, binary literal string, rb prefix]
area: languages/python/syntax
level: core
summary: >-
  A bytes literal is a string literal with a b prefix. It makes a bytes object, an immutable sequence of integers from 0 to 255, and it may contain only ASCII characters and escapes.
grammar: "stringprefix: <(\"r\" | \"u\" | \"b\" | \"br\" | \"rb\"), case-insensitive> (Language Reference, 2.5.3 Formal grammar)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#bytes-literals", locator: "2.5.5 Bytes literals"}
see_also: [python/string-literals, python/raw-strings, python/source-code-encoding]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#bytes-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.5.5 Bytes literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#bytes-objects
    title: Built-in Types, Python 3.15 documentation
    locator: Bytes Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#unrecognized-escape-sequences
    title: The Python Language Reference, Python 3.15
    locator: 2.5.4 Escape sequences; 2.5.4.7 Unrecognized escape sequences
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#string-prefixes
    title: The Python Language Reference, Python 3.15
    locator: 2.5.2 String prefixes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#string-literal-concatenation
    title: The Python Language Reference, Python 3.15
    locator: 6.2.3.2 String literal concatenation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10.1 Value comparisons
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/whatsnew/3.0.html#text-vs-data-instead-of-unicode-vs-8-bit
    title: What's New In Python 3.0
    locator: Text Vs. Data Instead Of Unicode Vs. 8-bit
    accessed: 2026-11-16
---

## Synopsis

```python run id=png-magic
magic = b'\x89PNG\r\n\x1a\n'
print(type(magic).__name__, len(magic))
print(list(magic))
print(magic[1:4], magic[1])
```

```output for=png-magic
bytes 8
[137, 80, 78, 71, 13, 10, 26, 10]
b'PNG' 80
```

## Semantics

**r1.** A bytes literal has the prefix `b` or `B`, and it makes an instance of `bytes` instead of `str`. The quote forms are those of string literals: single, double and triple quotes.[^1] [^2]

**r2.** A bytes literal may contain only ASCII characters, "regardless of the declared source code encoding". A byte of value 128 or more, and the zero byte, must be written as an escape, usually `\xhh` or an octal escape.[^1] [^2] Example: [`non-ascii`](#non-ascii).

**r3.** The escapes of string literals work in bytes literals, except `\N{...}`, `\u...` and `\U...`. In a bytes literal, those three are unrecognized escapes, so the backslash stays in the result.[^3] Example: [`unicode-escape`](#unicode-escape).

**r4.** The prefix `r` can be combined with `b` as `br` or `rb`, for a raw bytes literal in which backslashes are normal characters.[^4]

**r5.** A bytes object is an immutable sequence of integers, each one in the range `0 <= x < 256`. So `b[0]` is an integer, while `b[0:1]` is a bytes object of length 1. The repr of a bytes object uses the literal form `b'...'`.[^2] Example: [`png-magic`](#png-magic).

**r6.** Adjacent bytes literals join into one, as string literals do, but bytes literals "may only be combined with other byte literals; not with string literals of any kind".[^5] Example: [`mixed-concat`](#mixed-concat).

**r7.** At run time, text and data do not mix. Strings and binary sequences "cannot be directly compared",[^6] and the Python 3.0 release notes say that "any attempt to mix text and data in Python 3.0 raises" `TypeError`.[^7] Example: [`bytes-vs-str`](#bytes-vs-str).

## Examples

`encode` and `decode` convert between `str` and `bytes` (rule r7). The non-ASCII character takes two bytes in UTF-8, and they show as escapes.

```python run id=encode-decode
data = "café".encode("utf-8")
print(data, len(data))
print(data.decode("utf-8"))
print(bytes.fromhex("63 61 66 c3 a9") == data)
```

```output for=encode-decode
b'caf\xc3\xa9' 5
café
True
```

`\u` is not an escape in a bytes literal (rule r3). The literal keeps six characters, and CPython 3.12 warns about the unrecognized escape.

```python run id=unicode-escape
import warnings

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    value = eval(r"b'\u00e9'")
print(value, len(value))
print(caught[0].category.__name__)
```

```output for=unicode-escape
b'\\u00e9' 6
SyntaxWarning
```

## Pitfalls

### A non-ASCII character in a bytes literal

A character such as `é` is a `SyntaxError` in a bytes literal (rule r2). Write its bytes as escapes, or encode a string.

```python fails id=non-ascii expect=exception:SyntaxError
label = b"café"
```

### A bytes literal next to a string literal

The two kinds cannot join (rule r6).

```python fails id=mixed-concat expect=exception:SyntaxError
header = b"Content-Type: " "text/plain"
```

### Comparing bytes with a string

`==` between bytes and a string is not an error. It is always `False` (rule r7), so a test such as `reply == "OK"` never succeeds when `reply` is bytes. `+` raises `TypeError`.

```python wrong id=bytes-vs-str
reply = b"OK"
print(reply == "OK", reply == b"OK", reply.decode() == "OK")
try:
    reply + "!"
except TypeError as error:
    print("TypeError:", error)
```

```output for=bytes-vs-str
False True True
TypeError: can't concat str to bytes
```
