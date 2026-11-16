---
tag: 3QJ6R8
kind: feature
owner: python
slug: str-type
name: The str type
aliases: [str, str class, str(), text sequence type, code point, ord, chr, no char type]
area: languages/python/strings-and-text
level: core
summary: >-
  str is an immutable sequence of Unicode code points. There is no character type: an index gives a string of length 1. ord() and chr() convert between a character and its code point.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str", locator: "Text Sequence Type: str"}
see_also: [python/strings, python/string-methods, python/repr-and-str, python/sequence-types]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str
    title: Built-in Types, Python 3.15 documentation
    locator: Text Sequence Type, str; class str
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#ord
    title: Built-in Functions, Python 3.15 documentation
    locator: ord(); chr()
    accessed: 2026-11-16
  - url: https://go.dev/blog/strings
    title: Strings, bytes, runes and characters in Go
    locator: Code points, characters, and runes
    accessed: 2026-11-16
---

## Synopsis

```python run id=code-points
s = "café"
print(len(s), s[3], type(s[3]).__name__)
print(ord("é"), chr(233), [ord(c) for c in "ab"])
print(str(42) + str(3.5), repr(str()))
```

```output for=code-points
4 é str
233 é [97, 98]
423.5 ''
```

Each item of a str is a code point. The item is itself a str.

## Semantics

**r1.** "Strings are immutable sequences of Unicode code points."[^1] They support the common sequence operations: `in`, `+`, `*`, indexing, slicing, `len`, `min`, `max`, `count` and `index`.[^1]

**r2.** There is no separate character type, so "indexing a string produces strings of length 1. That is, for a non-empty string s, s[0] == s[0:1]."[^1] Example: [`no-char`](#no-char).

**r3.** `ord()` returns the code point of a one-character string, and "This is the inverse of chr()."[^2] For `chr()`, "The valid range for the argument is from 0 through 1,114,111 (0x10FFFF in base 16). ValueError will be raised if it is outside that range."[^2] Example: [`chr-range`](#chr-range).

**r4.** `str(object)` gives a string version of the object. "If object is not provided, returns the empty string."[^1] "If object does not have a __str__() method, then str() falls back to returning repr(object)."[^1]

**r5.** "If at least one of encoding or errors is given, object should be a bytes-like object (e.g. bytes or bytearray). In this case, if object is a bytes (or bytearray) object, then str(bytes, encoding, errors) is equivalent to bytes.decode(encoding, errors)."[^1] Without them, str() of a bytes object gives its representation. Example: [`from-bytes`](#from-bytes).

**r6.** "There is also no mutable string type, but str.join() or io.StringIO can be used to efficiently construct strings from multiple fragments."[^1]

## Examples

```python run id=no-char
s = "spam"
print(s[0] == s[0:1], len(s[0]))
print("a" < "b", "Z" < "a", min("spam"), "pa" in s)
```

```output for=no-char
True 1
True True a True
```

A character is a string of length 1 (rule r2). Strings compare by code point, and `"Z"` (90) is less than `"a"` (97).

```python run id=from-bytes
data = b"caf\xc3\xa9"
print(str(data, "utf-8"))
print(str(data))
```

```output for=from-bytes
café
b'caf\xc3\xa9'
```

With an encoding, str() decodes the bytes. Without it, str() gives the representation of the bytes object (rule r5).

## Pitfalls

str() of bytes without an encoding does not decode. The result holds the `b` and the quotes:

```python wrong id=bytes-repr
data = "hello".encode()
text = str(data)
print(text, len(text))
```

```output for=bytes-repr
b'hello' 8
```

Use `data.decode()` or `str(data, "utf-8")` (rule r5).

A code point past 0x10FFFF is an error:

```python fails id=chr-range expect=exception:ValueError
chr(0x110000)
```

## Comparison

Go has a type for one code point: "The Go language defines the word rune as an alias for the type int32, so programs can be clear when an integer value represents a code point."[^3] A Go string "holds arbitrary bytes".[^3] Python has no character type. An item of a Python str is a str of one code point (rule r2), and `ord()` gives its number (rule r3).
