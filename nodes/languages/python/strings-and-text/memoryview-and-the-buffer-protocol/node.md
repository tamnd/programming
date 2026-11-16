---
tag: 98GRPJ
kind: feature
owner: python
slug: memoryview-and-the-buffer-protocol
name: memoryview and the buffer protocol
aliases: [memoryview, buffer protocol, zero-copy slice, __buffer__, __release_buffer__, collections.abc.Buffer, PEP 688, PEP 3118, bytes-like object]
area: languages/python/strings-and-text
level: advanced
summary: >-
  A memoryview reads and writes the memory of a bytes, bytearray or array object without a copy. A slice of a view is a view too. Python classes can export a buffer with __buffer__.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#memory-views", locator: "Memory Views"}
see_also: [python/bytes-and-bytearray, python/slicing, python/str-type]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#memory-views
    title: Built-in Types, Python 3.15 documentation
    locator: Memory Views
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#emulating-buffer-types
    title: The Python Language Reference, 3. Data model
    locator: 3.3.11. Emulating buffer types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/c-api/buffer.html
    title: Buffer Protocol, Python/C API Reference Manual
    locator: Buffer Protocol; Buffer structure
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0688/
    title: "PEP 688: Making the buffer protocol accessible in Python"
    locator: Abstract; Motivation
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Slice_types
    title: The Go Programming Language Specification
    locator: Slice types
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
data = bytearray(b"abcdef")
copy = data[0:3]
view = memoryview(data)[0:3]
data[0] = ord("Z")
print(copy, view.tobytes())
view[1] = ord("Y")
print(data, view.readonly, memoryview(b"abc").readonly)
```

```output for=basics
bytearray(b'abc') b'Zbc'
bytearray(b'ZYcdef') False True
```

A slice of a bytearray is a copy. A slice of a memoryview shares the memory, so a change on one side shows on the other side.

## Semantics

**r1.** "memoryview objects allow Python code to access the internal data of an object that supports the buffer protocol without copying."[^1] "Built-in objects that support the buffer protocol include bytes and bytearray."[^1]

**r2.** "One-dimensional slicing will result in a subview".[^1] An index on a byte view gives an int.[^1] The C API documentation says that buffers "can also be used as a zero-copy slicing mechanism."[^3]

**r3.** "If the underlying object is writable, the memoryview supports one-dimensional slice assignment. Resizing is not allowed".[^1] A view on bytes is read-only. Examples: [`basics`](#basics), [`resize`](#resize).

**r4.** "A memoryview has the notion of an element, which is the atomic memory unit handled by the originating object."[^1] "The itemsize attribute will give you the number of bytes in a single element."[^1] cast() makes a view with a new format or shape, and "the buffer itself is not copied."[^1] Example: [`cast`](#cast).

**r5.** release() releases the buffer. "Many objects take special actions when a view is held on them (for example, a bytearray would temporarily forbid resizing); therefore, calling release() is handy to remove these restrictions".[^1] A with statement releases the view at the end of the block.[^1] Examples: [`held`](#held), [`release`](#release).

**r6.** "The buffer protocol provides a way for Python objects to expose efficient access to a low-level memory array."[^2] `__buffer__(self, flags)` is "Called when a buffer is requested from self (for example, by the memoryview constructor)".[^2] "The method must return a memoryview object."[^2] Example: [`dunder`](#dunder).

**r7.** PEP 688 gave Python code this API. Before it, the buffer protocol was "currently accessible only to C code".[^4] It also says: "Currently, there is no way for Python code to inspect whether an object supports the buffer protocol."[^4] The ABC `collections.abc.Buffer` now does this check.[^2] Example: [`dunder`](#dunder).

## Examples

```python run id=cast
import array

nums = array.array("h", [1, 2, 3, 258])
m = memoryview(nums)
print(m.format, m.itemsize, len(m), m.nbytes)
raw = m.cast("B")
print(raw.format, len(raw), raw.tolist())
raw[0] = 7
print(nums.tolist())
```

```output for=cast
h 2 4 8
B 8 [1, 0, 2, 0, 3, 0, 2, 1]
[7, 2, 3, 258]
```

The cast view shows the 8 bytes of the array, and a write through it changes the array (rule r4). The output is from x86-64 Linux, where an `h` item has 2 bytes in little-endian order. The documentation notes that "all byte lengths may depend on the operating system."[^1]

```python run id=release
buf = bytearray(b"abc")
with memoryview(buf) as m:
    m[0] = ord("x")
buf.append(ord("d"))
print(buf)
```

```output for=release
bytearray(b'xbcd')
```

The with block releases the view, so the bytearray can grow again (rule r5).

```python run id=dunder
from collections.abc import Buffer

class Packet:
    def __init__(self, payload):
        self._data = bytearray(payload)

    def __buffer__(self, flags):
        return memoryview(self._data)

p = Packet(b"ping")
print(bytes(p), memoryview(p)[1])
print(isinstance(p, Buffer), isinstance("ping", Buffer))
```

```output for=dunder
b'ping' 105
True False
```

The class exports the memory of its bytearray. bytes() and memoryview() accept it, and the ABC test finds the method (rules r6 and r7). A str does not export a buffer.

## Pitfalls

A bytearray cannot grow while a view on it is held:

```python fails id=held expect=exception:BufferError
buf = bytearray(b"abc")
m = memoryview(buf)
buf.append(ord("d"))
```

Release the view first, or use a with block (rule r5).

A slice assignment cannot change the size of the memory:

```python fails id=resize expect=exception:ValueError
data = bytearray(b"abcefg")
v = memoryview(data)
v[2:3] = b"spam"
```

The right side must have the same length as the slice (rule r3).

## Comparison

A Go slice shares memory by default. "A slice is a descriptor for a contiguous segment of an underlying array".[^5] "A slice therefore shares storage with its array and with other slices of the same array".[^5] So `b[2:5]` on a Go `[]byte` is like `memoryview(buf)[2:5]` in Python, and Python `buf[2:5]` is a copy (rule r2).
