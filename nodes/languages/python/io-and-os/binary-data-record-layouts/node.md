---
tag: CD8A85
kind: feature
owner: python
slug: binary-data-record-layouts
name: Working with Binary Data Record Layouts
aliases: [struct module, struct.pack, struct.unpack, struct.Struct, byte order, endianness, binary records]
area: languages/python/io-and-os
level: advanced
summary: >-
  The struct module packs Python values into bytes and unpacks them again. A format string such as '<IH' gives the byte order, the field types and so the size of each record.
grammar: "struct.pack(format, v1, v2, ...); struct.unpack(format, buffer)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/struct.html", locator: "struct module"}
see_also: [python/text-and-binary-io, python/reading-and-writing-files, python/sockets]
sources:
  - url: https://docs.python.org/3.15/library/struct.html
    title: struct, Python 3.15 documentation
    locator: module introduction; functions and exceptions; format strings; byte order, size and alignment; type codes; examples; applications; Struct
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#working-with-binary-data-record-layouts
    title: The Python Tutorial, 11. Brief tour of the standard library, part II
    locator: 11.3. Working with binary data record layouts
    accessed: 2026-11-16
  - url: https://pkg.go.dev/encoding/binary
    title: Package binary, Go standard library
    locator: Overview; func Read; func Write; BigEndian
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import struct

HEADER = struct.Struct('<4sHH')   # magic, version, record count
RECORD = struct.Struct('<HhI')    # sensor id, temperature, timestamp
rows = [(1, -40, 1700000000), (2, 215, 1700000060)]

data = HEADER.pack(b'TEMP', 1, len(rows))
data += b''.join(RECORD.pack(*row) for row in rows)
print(len(data), HEADER.size, RECORD.size)

magic, version, count = HEADER.unpack_from(data)
print(magic, version, count)
for record in RECORD.iter_unpack(data[HEADER.size:]):
    print(record)
```

```output for=synopsis
24 8 8
b'TEMP' 1 2
(1, -40, 1700000000)
(2, 215, 1700000060)
```

The file layout is an 8-byte header and then 8-byte records. `<` fixes little-endian order with no padding, so the sizes are the same on every machine.

## Semantics

**r1.** "This module converts between Python values and C structs represented as Python bytes objects. Compact format strings describe the intended conversions to/from Python values."[^1] The tutorial uses it for "working with variable length binary record formats."[^2]

**r2.** `struct.pack(format, v1, v2, ...)` returns "a bytes object containing the values v1, v2, … packed according to the format string format. The arguments must match the values required by the format exactly."[^1] `struct.unpack(format, buffer)` returns a tuple: "The result is a tuple even if it contains exactly one item. The buffer’s size in bytes must match the size required by the format, as reflected by calcsize()."[^1]

**r3.** "Each format string consists of an optional prefix character which describes the overall properties of the data and one or more format characters which describe the actual data values and padding."[^1] The prefixes are `@` (native), `=` (native order, standard size), `<` (little-endian), `>` (big-endian) and `!` (network order). "If the first character is not one of these, '@' is assumed."[^1] Example: [`byte-order`](#byte-order).

**r4.** In native mode, "C types are represented in the machine’s native format and byte order, and properly aligned by skipping pad bytes if necessary (according to the rules used by the C compiler)."[^1] "No padding is added when using non-native size and alignment, e.g. with ‘<’, ‘>’, ‘=’, and ‘!’."[^1] Example: [`padding`](#padding).

**r5.** In the tutorial's words, "Pack codes "H" and "I" represent two and four byte unsigned numbers respectively. The "<" indicates that they are standard size and in little-endian byte order".[^2] "A type code may be preceded by an integral repeat count. For example, the format string '4h' means exactly the same as 'hhhh'."[^1]

**r6.** For `s`, "the count is interpreted as the length of the byte string, not a repeat count like for the other type codes".[^1] "For packing, the byte string is truncated or padded with null bytes as appropriate to make it fit."[^1] Example: [`strings`](#strings).

**r7.** `struct.Struct(format)` compiles a format once. "Creating a Struct object once and calling its methods is more efficient than calling module-level functions with the same format since the format string is only compiled once."[^1] Its `size` attribute is "The calculated size of the struct".[^1] `unpack_from()` reads at an offset, and `iter_unpack()` "will read equally sized chunks from the buffer until all its contents have been consumed."[^1]

**r8.** If a value does not fit an integer format, "then struct.error is raised."[^1] Example: [`overflow`](#overflow).

## Examples

```python run id=byte-order
import struct

print(struct.pack('>h', 1023))
print(struct.pack('<h', 1023))
print(struct.pack('!I', 1))
print(struct.unpack('>h', b'\x03\xff'), struct.unpack('<h', b'\x03\xff'))
```

```output for=byte-order
b'\x03\xff'
b'\xff\x03'
b'\x00\x00\x00\x01'
(1023,) (-253,)
```

The same two bytes give different numbers in the two orders. "The form '!' represents the network byte order which is always big-endian".[^1]

```python run id=strings
import struct

packed = struct.pack('<5sB', b'hi', 7)
print(packed)
name, level = struct.unpack('<5sB', packed)
print(name.rstrip(b'\x00'), level)
print(struct.pack('<2s', b'hello'))
```

```output for=strings
b'hi\x00\x00\x00\x07'
b'hi' 7
b'he'
```

The short name gets null bytes and the long one is cut (rule r6). Unpacking gives back all 5 bytes, so the program strips the nulls itself.

```python run id=named
import struct
from collections import namedtuple

Student = namedtuple('Student', 'name serialnum school gradelevel')
record = b'raymond   \x32\x12\x08\x01\x08'
print(Student._make(struct.unpack('<10sHHb', record)))
```

```output for=named
Student(name=b'raymond   ', serialnum=4658, school=264, gradelevel=8)
```

The struct docs say: "Unpacked fields can be named by assigning them to variables or by wrapping the result in a named tuple".[^1]

```python run id=file
import struct

RECORD = struct.Struct('>Hd')
with open('points.bin', 'wb') as f:
    for i, value in enumerate([1.5, -2.25, 1e10]):
        f.write(RECORD.pack(i, value))

with open('points.bin', 'rb') as f:
    while chunk := f.read(RECORD.size):
        print(RECORD.unpack(chunk))
```

```output for=file
(0, 1.5)
(1, -2.25)
(2, 10000000000.0)
```

Fixed-size records let the reader take `RECORD.size` bytes at a time. Open the file in binary mode, because struct works on bytes.

## Pitfalls

Without a prefix, the format uses native alignment, and the size can change:

```python run id=padding
import struct

print(struct.calcsize('@ci'), struct.calcsize('<ci'))
print(struct.calcsize('@ic'))
```

```output for=padding
8 5
5
```

In the native `@ci` format, three pad bytes come after the `c` so the int is aligned. "The ordering of type codes may have an impact on size in native mode since padding is implicit."[^1] For files and networks, the docs advise: "The first character should typically be < or > (or !)."[^1]

A value out of range raises struct.error:

```python fails id=overflow expect=exception:error
import struct

struct.pack('>h', 99999)
```

The message is "'h' format requires -32768 <= number <= 32767".[^1]

`unpack()` wants exactly the right number of bytes (rule r2). Use `unpack_from()` to read a record from a longer buffer.

Native results are not portable. The docs note that native examples "may not match what the reader’s machine produces as that depends on the platform and compiler."[^1]

## Comparison

"Package binary implements simple translation between numbers and byte sequences and encoding and decoding of varints."[^3] Go's `binary.Read` "reads structured binary data from r into data", and the byte order is an argument such as `binary.BigEndian`.[^3] For `Read`, the data "must be a pointer to a fixed-size value or a slice of fixed-size values", so the Go type gives the layout.[^3] Python gives the layout with a format string instead.
