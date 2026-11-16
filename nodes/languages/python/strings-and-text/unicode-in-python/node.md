---
tag: N9BA9V
kind: feature
owner: python
slug: unicode-in-python
name: Unicode in Python
aliases: [Unicode, code point, unicodedata, normalization, NFC, NFD, NFKC, casefold, combining character, grapheme cluster, iter_graphemes]
area: languages/python/strings-and-text
level: core
summary: >-
  A str is a sequence of Unicode code points, and len() counts code points. One visible character can be one or more code points, so normalize text before a compare. unicodedata gives the tools.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str", locator: "Text Sequence Type: str"}
see_also: [python/str-type, python/encoding-and-decoding, python/string-methods, python/bytes-and-bytearray]
sources:
  - url: https://docs.python.org/3.15/howto/unicode.html#comparing-strings
    title: Unicode HOWTO, Python 3.15 documentation
    locator: Definitions; Unicode Properties; Comparing Strings
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/unicodedata.html
    title: unicodedata, Unicode Database
    locator: normalize(); is_normalized(); iter_graphemes(); name(); category()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#str.casefold
    title: Built-in Types, Python 3.15 documentation
    locator: Text Sequence Type, str; str.casefold()
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0393/
    title: "PEP 393: Flexible String Representation"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/blog/strings#code-points-characters-and-runes
    title: Strings, bytes, runes and characters in Go, The Go Blog
    locator: Code points, characters, and runes
    accessed: 2026-11-16
  - url: https://pkg.go.dev/golang.org/x/text/unicode/norm
    title: Package norm, golang.org/x/text
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import unicodedata

s = "na\u00efve \u2615"
print(len(s), hex(ord(s[2])), unicodedata.category(s[2]))
print(unicodedata.name(s[-1]), "\N{HOT BEVERAGE}" == s[-1])
print("\N{BLACK CHESS KNIGHT}" == chr(0x265E))
```

```output for=basics
7 0xef Ll
HOT BEVERAGE True
True
```

Each item of a str is one code point. ord() and chr() convert between a code point and its number, and unicodedata reads the properties of a code point.

## Semantics

**r1.** "Strings are immutable sequences of Unicode code points."[^3] "A code point value is an integer in the range 0 to 0x10FFFF".[^1]

**r2.** The same text can have more than one code point sequence. "For example, a letter like ‘ê’ can be represented as a single code point U+00EA, or as U+0065 U+0302".[^1] "These will produce the same output when printed, but one is a string of length 1 and the other is of length 2."[^1] Example: [`compare`](#compare).

**r3.** normalize() "converts strings to one of several normal forms, where letters followed by a combining character are replaced with single characters."[^1] The forms are NFC, NFKC, NFD and NFKD.[^2] "Normal form D (NFD) is also known as canonical decomposition, and translates each character into its decomposed form. Normal form C (NFC) first applies a canonical decomposition, then composes pre-combined characters again."[^2] Example: [`normalize`](#normalize).

**r4.** The K forms also apply the compatibility decomposition. "The normal form KD (NFKD) will apply the compatibility decomposition, that is, replace all compatibility characters with their equivalents."[^2] Example: [`compat`](#compat).

**r5.** casefold() is for caseless matching. "Casefolding is similar to lowercasing but more aggressive because it is intended to remove all case distinctions in a string."[^3] The HOWTO compares two strings with `NFD(NFD(s1).casefold()) == NFD(NFD(s2).casefold())`, because "there are a few characters that make casefold() return a non-normalized string".[^1] Example: [`caseless`](#caseless).

**r6.** The unicodedata module "provides access to the Unicode Character Database (UCD) which defines character properties for all Unicode characters."[^2] name() and category() give the name and the general category of one code point.[^2]

**r7.** iter_graphemes() "Returns an iterator to iterate over grapheme clusters." It "uses extended grapheme cluster rules defined by Unicode Standard Annex #29".[^2] A grapheme cluster is what a reader sees as one character. Example: [`graphemes`](#graphemes).

**r8.** In CPython, the memory for a str depends on its largest code point. PEP 393 changed the type "to support multiple internal representations, depending on the character with the largest Unicode ordinal (1, 2, or 4 bytes)."[^4] The program sees only code points.

## Examples

```python run id=normalize
import unicodedata

decomposed = "e\u0301"
nfc = unicodedata.normalize("NFC", decomposed)
print(len(decomposed), len(nfc), nfc == "\u00e9")
print(len(unicodedata.normalize("NFD", "\u00e9")))
print(unicodedata.is_normalized("NFC", decomposed))
```

```output for=normalize
2 1 True
2
False
```

NFC joins the letter and the combining accent into one code point. NFD splits them (rule r3).

```python run id=compat
import unicodedata

text = "Ⅷ ﬁ x²"
print(unicodedata.normalize("NFC", text) == text)
print(unicodedata.normalize("NFKC", text))
```

```output for=compat
True
VIII fi x2
```

NFC keeps the roman numeral, the ligature and the superscript. NFKC replaces them with plain letters and digits (rule r4).

```python run id=caseless
import unicodedata

def nfd(s):
    return unicodedata.normalize("NFD", s)

def same_caseless(a, b):
    return nfd(nfd(a).casefold()) == nfd(nfd(b).casefold())

print("Straße".lower(), "Straße".casefold())
print("Straße".lower() == "STRASSE".lower())
print(same_caseless("Straße", "STRASSE"), same_caseless("\u00ea", "E\u0302"))
```

```output for=caseless
straße strasse
False
True True
```

lower() keeps the letter ß, but casefold() changes it to ss (rule r5).

```python run id=graphemes since=3.15
import unicodedata

s = "e\u0301\U0001F1EB\U0001F1F7\U0001F44D\U0001F3FD"
print(len(s))
print([(g.start, g.end) for g in unicodedata.iter_graphemes(s)])
```

```output for=graphemes
6
[(0, 2), (2, 4), (4, 6)]
```

The string has 6 code points: an accented letter, a flag and a thumbs-up with a skin tone. A reader sees 3 characters (rule r7).

## Pitfalls

Two strings that look the same can be unequal, and `len("é")` can be 1 or 2:

```python wrong id=compare
composed = "\u00e9"
decomposed = "e\u0301"
print(composed, decomposed)
print(composed == decomposed, len(composed), len(decomposed))
```

```output for=compare
é é
False 1 2
```

Normalize both strings to the same form before a compare, a dict lookup or a length check (rules r2 and r3).

A slice or an index can cut a grapheme cluster. `"e\u0301"[0]` is a plain `e` without its accent (rule r7).

## Comparison

A Go string is a sequence of bytes, and Go calls a code point a rune: "The Go language defines the word rune as an alias for the type int32, so programs can be clear when an integer value represents a code point."[^5] Go has the same problem with combining marks: "a character may be represented by a number of different sequences of code points, and therefore different sequences of UTF-8 bytes."[^5] In Go, normalization is in the package golang.org/x/text/unicode/norm: "Package norm contains types and functions for normalizing Unicode strings."[^6] Python has unicodedata.normalize() in the standard library (rule r3).
