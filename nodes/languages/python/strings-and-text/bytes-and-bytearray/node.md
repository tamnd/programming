---
tag: K6NJVP
kind: feature
owner: python
slug: bytes-and-bytearray
name: bytes and bytearray
aliases: [bytes, bytearray, binary data, byte string, bytes.hex, bytes.fromhex, mutable bytes, b prefix]
area: languages/python/strings-and-text
level: core
summary: >-
  bytes is an immutable sequence of integers from 0 to 255, and bytearray is the mutable form. An index gives an int, not a character. bytes and str do not mix: code must encode or decode.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#binary-sequence-types-bytes-bytearray-memoryview", locator: "Binary Sequence Types: bytes, bytearray, memoryview"}
see_also: [python/str-type, python/strings, python/memoryview-and-the-buffer-protocol, python/old-string-formatting]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#bytes-objects
    title: Built-in Types, Python 3.15 documentation
    locator: Bytes Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#bytearray-objects
    title: Built-in Types, Python 3.15 documentation
    locator: Bytearray Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#bytes-and-bytearray-operations
    title: Built-in Types, Python 3.15 documentation
    locator: Bytes and Bytearray Operations; printf-style Bytes Formatting
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#text-sequence-type-str
    title: Built-in Types, Python 3.15 documentation
    locator: Text Sequence Type, str (class str)
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Conversions_to_and_from_a_string_type
    title: The Go Programming Language Specification
    locator: Conversions to and from a string type
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
data = b"Hi!\x00\xff"
print(len(data), data[0], data[0:1], list(data))
buf = bytearray(data)
buf[0] = ord("h")
buf += b"\x10"
print(buf)
print(bytes.fromhex("48 69 21"), data.hex(" "))
```

```output for=basics
5 72 b'H' [72, 105, 33, 0, 255]
bytearray(b'hi!\x00\xff\x10')
b'Hi!' 48 69 21 00 ff
```

A bytes literal has the b prefix. An index gives an int, and a slice gives bytes. A bytearray can change in place.

## Semantics

**r1.** "Bytes objects are immutable sequences of single bytes."[^1] "Only ASCII characters are permitted in bytes literals (regardless of the declared source code encoding). Any binary values over 127 must be entered into bytes literals using the appropriate escape sequence."[^1]

**r2.** "bytes objects actually behave like immutable sequences of integers, with each value in the sequence restricted such that 0 <= x < 256 (attempts to violate this restriction will trigger ValueError)."[^1] Example: [`range`](#range).

**r3.** "for a bytes object b, b[0] will be an integer, while b[0:1] will be a bytes object of length 1. (This contrasts with text strings, where both indexing and slicing will produce a string of length 1)"[^1]

**r4.** Besides literals, there are other ways to make bytes: "A zero-filled bytes object of a specified length: bytes(10)", "From an iterable of integers: bytes(range(20))" and "Copying existing binary data via the buffer protocol: bytes(obj)".[^1] Example: [`zero-fill`](#zero-fill).

**r5.** "bytearray objects are a mutable counterpart to bytes objects."[^2] "There is no dedicated literal syntax for bytearray objects, instead they are always created by calling the constructor".[^2] They support the mutable sequence operations.[^2] Example: [`mutate`](#mutate).

**r6.** fromhex() reads two hexadecimal digits for each byte, "with ASCII whitespace being ignored."[^1] hex() returns "a string object containing two hexadecimal digits for each byte in the instance", with an optional separator.[^1]

**r7.** "The methods on bytes and bytearray objects don’t accept strings as their arguments, just as the methods on strings don’t accept bytes as their arguments."[^3] Example: [`mix`](#mix).

**r8.** "Passing a bytes object to str() without the encoding or errors arguments falls under the first case of returning the informal string representation".[^4] With an encoding, "str(bytes, encoding, errors) is equivalent to bytes.decode(encoding, errors)."[^4] Example: [`str-of-bytes`](#str-of-bytes).

**r9.** Bytes objects have their own % operator: "Given format % values (where format is a bytes object), % conversion specifications in format are replaced with zero or more elements of values."[^3] Example: [`percent`](#percent).

## Examples

```python run id=mutate
buf = bytearray(b"hello world")
buf[0:5] = b"HELLO"
buf.extend(b"!!")
del buf[5]
print(buf, len(buf))
print(bytes(buf).upper(), buf.replace(b"!", b"?"))
```

```output for=mutate
bytearray(b'HELLOworld!!') 12
b'HELLOWORLD!!' bytearray(b'HELLOworld??')
```

A slice assignment, extend() and del change the bytearray in place (rule r5). replace() returns a new object.

```python run id=percent
print(b"%s: %d bytes, %x" % (b"header", 16, 255))
```

```output for=percent
b'header: 16 bytes, ff'
```

The format and the result are bytes (rule r9).

```python run id=str-of-bytes
data = "café".encode("utf-8")
print(data, len(data))
print(str(data))
print(str(data, "utf-8"), data.decode("utf-8"))
```

```output for=str-of-bytes
b'caf\xc3\xa9' 5
b'caf\xc3\xa9'
café café
```

Without an encoding, str() gives the representation of the bytes, not the text (rule r8).

## Pitfalls

`bytes(10)` does not give the byte 10:

```python wrong id=zero-fill
print(bytes(10))
print(bytes([10]))
```

```output for=zero-fill
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
b'\n'
```

An int argument gives that number of zero bytes. Put the value in a list to get one byte (rule r4).

A value outside 0 to 255 is an error:

```python fails id=range expect=exception:ValueError
bytes([256])
```

A str argument to a bytes method is an error:

```python fails id=mix expect=exception:TypeError
b"abc".replace("a", "f")
```

Write `b"abc".replace(b"a", b"f")`, or decode the bytes to a str first (rule r7).

A loop over bytes gives ints, not characters (rule r3):

```python run id=loop
for item in b"Hi":
    print(item, chr(item))
```

```output for=loop
72 H
105 i
```

## Comparison

In Go, a string is already a sequence of bytes, and `[]byte` is the mutable form. "Converting a value of a string type to a slice of bytes type yields a non-nil slice whose successive elements are the bytes of the string."[^5] "Converting a slice of bytes to a string type yields a string whose successive bytes are the elements of the slice."[^5] So Go `[]byte(s)` is close to Python `bytearray(s.encode())`. A Python str holds code points, not bytes, so the conversion in Python always needs an encoding (rule r8).
