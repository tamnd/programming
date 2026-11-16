---
tag: T6N0E9
kind: feature
owner: python
slug: encoding-and-decoding
name: Encoding and decoding
aliases: [str.encode, bytes.decode, character encoding, UTF-8, codecs, error handler, UnicodeDecodeError, UnicodeEncodeError, surrogateescape, UTF-8 Mode, mojibake]
area: languages/python/strings-and-text
level: core
summary: >-
  str.encode() turns text into bytes, and bytes.decode() turns bytes into text. The encoding is UTF-8 by default. The errors argument decides what happens to data that does not fit.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/codecs.html#error-handlers", locator: "codecs, Error Handlers"}
see_also: [python/bytes-and-bytearray, python/unicode-in-python, python/str-type]
sources:
  - url: https://docs.python.org/3.15/howto/unicode.html#encodings
    title: Unicode HOWTO, Python 3.15 documentation
    locator: Encodings; Converting to Bytes; The String Type
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/codecs.html#error-handlers
    title: codecs, Codec registry and base classes
    locator: Error Handlers; Encodings and Unicode
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/io.html#text-encoding
    title: io, Core tools for working with streams
    locator: Text encoding
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0686/
    title: "PEP 686: Make UTF-8 mode default"
    locator: Abstract; Motivation
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0540/
    title: "PEP 540: Add a new UTF-8 Mode"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://go.dev/blog/strings
    title: Strings, bytes, runes and characters in Go, The Go Blog
    locator: Code points, characters, and runes; Range loops
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
text = "Gr\u00fc\u00dfe, \u4e16\u754c"
data = text.encode("utf-8")
print(data)
print(len(text), len(data))
print(data.decode("utf-8") == text)
```

```output for=basics
b'Gr\xc3\xbc\xc3\x9fe, \xe4\xb8\x96\xe7\x95\x8c'
9 15
True
```

The text has 9 code points. UTF-8 uses 1 byte for each ASCII letter, 2 bytes for each of `ü` and `ß`, and 3 bytes for each of the two Chinese characters.

## Semantics

**r1.** "The rules for translating a Unicode string into a sequence of bytes are called a character encoding, or just an encoding."[^1] "Encodings are specified as strings containing the encoding’s name."[^1]

**r2.** str.encode() "returns a bytes representation of the Unicode string, encoded in the requested encoding."[^1] bytes.decode() goes the other way.[^1] For UTF-8: "If the code point is < 128, it’s represented by the corresponding byte value. If the code point is >= 128, it’s turned into a sequence of two, three, or four bytes".[^1]

**r3.** The errors argument "specifies the response when the input string can’t be converted according to the encoding’s rules."[^1] The default is `'strict'`, which raises UnicodeError or a subclass.[^2] Examples: [`strict-decode`](#strict-decode), [`strict-encode`](#strict-encode).

**r4.** For decoding, `'replace'` uses U+FFFD, `'ignore'` leaves the data out, and `'backslashreplace'` inserts a `\xNN` escape.[^1] For encoding, `'replace'` inserts a question mark, and `'xmlcharrefreplace'` and `'namereplace'` are also available.[^1] Examples: [`decode-errors`](#decode-errors), [`encode-errors`](#encode-errors).

**r5.** `'surrogateescape'` keeps bytes that do not decode. "On decoding, replace byte with individual surrogate code ranging from U+DC80 to U+DCFF. This code will then be turned back into the same byte when the 'surrogateescape' error handler is used when encoding the data."[^2] Example: [`round-trip`](#round-trip).

**r6.** "The default encoding of TextIOWrapper and open() is UTF-8. If Python UTF-8 Mode is disabled, the default encoding is locale-specific (locale.getencoding())."[^3] PEP 540 added the mode: "When UTF-8 Mode is active, Python will: use the utf-8 encoding, regardless of the locale currently set by the current platform".[^5] PEP 686 made it the default.[^4]

**r7.** "On decoding utf-8-sig will skip those three bytes if they appear as the first three bytes in the file. In UTF-8, the use of the BOM is discouraged and should generally be avoided."[^2] Example: [`bom`](#bom).

## Examples

```python run id=decode-errors
data = b"\x80abc"
for handler in ["replace", "backslashreplace", "ignore"]:
    print(handler, ascii(data.decode("utf-8", handler)))
```

```output for=decode-errors
replace '\ufffdabc'
backslashreplace '\\x80abc'
ignore 'abc'
```

The byte 0x80 cannot start a UTF-8 sequence. Each handler deals with it in its own way (rule r4).

```python run id=encode-errors
u = chr(40960) + "abcd" + chr(1972)
for handler in ["ignore", "replace", "xmlcharrefreplace", "backslashreplace", "namereplace"]:
    print(handler, u.encode("ascii", handler))
```

```output for=encode-errors
ignore b'abcd'
replace b'?abcd?'
xmlcharrefreplace b'&#40960;abcd&#1972;'
backslashreplace b'\\ua000abcd\\u07b4'
namereplace b'\\N{YI SYLLABLE IT}abcd\\u07b4'
```

```python run id=round-trip
raw = b"caf\xe9.txt"
name = raw.decode("utf-8", "surrogateescape")
print(ascii(name))
print(name.encode("utf-8", "surrogateescape") == raw)
```

```output for=round-trip
'caf\udce9.txt'
True
```

The byte 0xe9 is not valid UTF-8 here. It becomes the code point U+DCE9 and comes back as the same byte (rule r5).

```python run id=bom
data = "hi".encode("utf-8-sig")
print(data)
print(ascii(data.decode("utf-8")), ascii(data.decode("utf-8-sig")))
```

```output for=bom
b'\xef\xbb\xbfhi'
'\ufeffhi' 'hi'
```

The plain UTF-8 codec keeps the BOM as the code point U+FEFF. The utf-8-sig codec removes it (rule r7).

## Pitfalls

Bytes that are not valid in the encoding raise an error:

```python fails id=strict-decode expect=exception:UnicodeDecodeError
b"\x80abc".decode("utf-8")
```

Text that the encoding cannot hold raises an error:

```python fails id=strict-encode expect=exception:UnicodeEncodeError
"caf\u00e9".encode("ascii")
```

The wrong encoding can decode without an error and give the wrong text:

```python wrong id=mojibake
data = "caf\u00e9".encode("utf-8")
print(data.decode("latin-1"))
print(data.decode("utf-8"))
```

```output for=mojibake
cafÃ©
café
```

Latin-1 maps every byte to a code point, so it never fails. The two UTF-8 bytes of `é` become two Latin-1 characters. Decode with the encoding that wrote the data.

Code that reads files should name the encoding, as in `open(path, encoding='utf-8')`. PEP 686 gives the reason: "many Python developers using Unix forget that the default encoding is platform dependent."[^4] An older Python without UTF-8 Mode uses the locale encoding (rule r6).

## Comparison

Go has no decode step for a string: "Go source code is always UTF-8. A string holds arbitrary bytes."[^6] A string can hold bytes that are not valid UTF-8, and only some operations decode them: "A for range loop, by contrast, decodes one UTF-8-encoded rune on each iteration."[^6] A Python str always holds code points, so bytes from a file or a socket must go through decode() first (rule r2).
