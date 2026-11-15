---
tag: VTP0B6
kind: feature
owner: python
slug: raw-strings
name: Raw strings
aliases: [raw string literal, r prefix, r-string, raw bytes literal, rb prefix]
area: languages/python/syntax
level: core
summary: >-
  A raw string literal has the prefix r. Its backslashes are normal characters, so it suits regular expressions and Windows paths, but it cannot end in an odd number of backslashes.
grammar: "stringprefix: <(\"r\" | \"u\" | \"b\" | \"br\" | \"rb\"), case-insensitive> (Language Reference, 2.5.3 Formal grammar)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/lexical_analysis.html#raw-string-literals", locator: "2.5.6 Raw string literals"}
see_also: [python/string-literals, python/bytes-literals]
sources:
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#raw-string-literals
    title: The Python Language Reference, Python 3.15
    locator: 2.5.6 Raw string literals
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#string-prefixes
    title: The Python Language Reference, Python 3.15
    locator: 2.5.2 String prefixes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/re.html#raw-string-notation
    title: re, Python 3.15 documentation
    locator: Raw string notation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/re.html
    title: re, Python 3.15 documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/design.html#why-can-t-raw-strings-r-strings-end-with-a-backslash
    title: Design and History FAQ, Python 3.15
    locator: Why can't raw strings (r-strings) end with a backslash?
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

```python run id=raw-vs-normal
print("C:\this\name")
print(r"C:\this\name")
print(len("\n"), len(r"\n"))
```

```output for=raw-vs-normal
C:	his
ame
C:\this\name
1 2
```

## Semantics

**r1.** A string or bytes literal with the prefix `r` or `R` is a raw literal. It treats backslashes as literal characters, so escape sequences have no special meaning.[^1] Example: [`raw-vs-normal`](#raw-vs-normal).

**r2.** The prefix `r` can be combined with `f`, `t` or `b`, so `fr`, `rf`, `tr`, `rt`, `br` and `rb` are valid prefixes. Prefixes are case-insensitive.[^2] Example: [`raw-prefixes`](#raw-prefixes).

**r3.** A quote can still be escaped with a backslash in a raw literal, but the backslash stays in the result: `r"\""` is two characters.[^1] Example: [`raw-quote`](#raw-quote).

**r4.** A raw literal "cannot end in a single backslash", because the backslash would escape the closing quote. In general, "even a raw string cannot end in an odd number of backslashes".[^1] Example: [`trailing-backslash`](#trailing-backslash).

**r5.** A backslash followed by a newline in a raw literal is those two characters, "*not* as a line continuation".[^1]

**r6.** Raw strings are the usual notation for regular expression patterns. The `re` module and string literals both use the backslash, so without raw strings a pattern that matches one backslash must be written `'\\\\'`.[^3] [^4] Example: [`regex`](#regex).

## Examples

The prefix `r` with `b` and with `f` (rule r2). In the f-string, the braces still hold an expression, but the backslashes stay.

```python run id=raw-prefixes
folder = "logs"
print(rb"\x00" == b"\\x00", Rb"\d" == bR"\d")
print(rf"C:\{folder}\new")
```

```output for=raw-prefixes
True True
C:\logs\new
```

A backslash before a quote keeps both characters (rule r3).

```python run id=raw-quote
text = r"\""
print(len(text), list(text))
```

```output for=raw-quote
2 ['\\', '"']
```

The same pattern with and without the raw notation (rule r6).

```python run id=regex
import re

print(re.findall(r"\d+", "10 apples, 25 pears"))
print(re.search(r"\\", r"a\b").group() == "\\")
print(re.search(r"\W(.)\1\W", " ff ") is not None, re.search("\\W(.)\\1\\W", " ff ") is not None)
```

```output for=regex
['10', '25']
True
True True
```

## Pitfalls

### A raw string that ends in a backslash

A folder path such as `r"C:\logs\"` does not end the string: the last backslash escapes the quote (rule r4).

```python fails id=trailing-backslash expect=exception:SyntaxError
folder = r"C:\logs\"
print(folder)
```

The design FAQ gives three workarounds, and notes that "all Windows system calls accept forward slashes too".[^5]

```python run id=trailing-workarounds
print(r"C:\logs" "\\")
print(r"C:\logs\ "[:-1])
print("C:\\logs\\")
```

```output for=trailing-workarounds
C:\logs\
C:\logs\
C:\logs\
```

### Raw does not mean any text

A raw string is still a literal: a quote of the same kind ends it, and the tutorial notes that it "may not end in an odd number of" backslashes.[^6] To hold text with any characters, read it from a file or join several literals.

## Comparison

Go has one raw form: a raw string literal between back quotes, in which "backslashes have no special meaning and the string may contain newlines". Between the back quotes, "any character may appear except back quote".[^7] A Python raw literal uses the normal quotes with a prefix (rule r1), and it can contain an escaped quote, with the backslash kept (rule r3).
