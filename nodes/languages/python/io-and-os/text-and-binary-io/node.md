---
tag: 1B6YGJ
kind: feature
owner: python
slug: text-and-binary-io
name: Text and binary I/O
aliases: [io module, TextIOWrapper, BufferedReader, BufferedWriter, FileIO, BytesIO, StringIO, raw I/O, buffered I/O, file object layers]
area: languages/python/io-and-os
level: advanced
summary: >-
  A Python file object has three layers: a raw byte stream, a buffer on top of it, and a text wrapper that decodes and translates newlines. io.BytesIO and io.StringIO are the same interfaces in memory.
grammar: "io.TextIOWrapper(buffer, encoding=None, errors=None, newline=None, line_buffering=False, write_through=False)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/io.html", locator: "Overview; Class hierarchy"}
see_also: [python/reading-and-writing-files, python/methods-of-file-objects, python/standard-streams, python/encoding-and-decoding]
sources:
  - url: https://docs.python.org/3.15/library/io.html
    title: io, Python 3.15 documentation
    locator: Overview; Class hierarchy; io.TextIOWrapper; io.BytesIO; Performance; Multi-threading
    accessed: 2026-11-16
  - url: https://pkg.go.dev/bufio
    title: Package bufio, Go standard library
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
with open('layers.txt', 'w', encoding='utf-8') as f:
    print(type(f).__name__)
    print(type(f.buffer).__name__)
    print(type(f.buffer.raw).__name__)
```

```output for=synopsis
TextIOWrapper
BufferedWriter
FileIO
```

A file opened in text mode is a `TextIOWrapper`. Its `buffer` attribute is the buffered binary stream, and the `raw` attribute of the buffer is the unbuffered file.

## Semantics

**r1.** "There are three main types of I/O: text I/O, binary I/O and raw I/O."[^1] "A concrete object belonging to any of these categories is called a file object. Other common terms are stream and file-like object."[^1]

**r2.** "Text I/O expects and produces str objects. This means that whenever the backing store is natively made of bytes (such as in the case of a file), encoding and decoding of data is made transparently as well as optional translation of platform-specific newline characters."[^1]

**r3.** "Binary I/O (also called buffered I/O) expects bytes-like objects and produces bytes objects. No encoding, decoding, or newline translation is performed."[^1]

**r4.** "Raw I/O (also called unbuffered I/O) is generally used as a low-level building-block for binary and text streams; it is rarely useful to directly manipulate a raw stream from user code."[^1] `open(name, 'rb', buffering=0)` returns a raw stream.[^1]

**r5.** The classes form a hierarchy. "At the top of the I/O hierarchy is the abstract base class IOBase."[^1] `RawIOBase`, `BufferedIOBase` and `TextIOBase` extend it. "TextIOWrapper, which extends TextIOBase, is a buffered text interface to a buffered raw stream (BufferedIOBase)."[^1]

| Layer | Abstract class | File class | In-memory class |
|---|---|---|---|
| Text | `TextIOBase` | `TextIOWrapper` | `StringIO` |
| Binary, buffered | `BufferedIOBase` | `BufferedReader`, `BufferedWriter`, `BufferedRandom` | `BytesIO` |
| Raw | `RawIOBase` | `FileIO` | |

**r6.** A `TextIOWrapper` can wrap any buffered binary stream, for example a `BytesIO`. Its `newline` argument works in two directions. When reading, "if newline is None, universal newlines mode is enabled. Lines in the input can end in '\n', '\r', or '\r\n', and these are translated into '\n' before being returned to the caller."[^1] When writing, "if newline is None, any '\n' characters written are translated to the system default line separator, os.linesep."[^1] "If newline is any of the other legal values, any '\n' characters written are translated to the given string."[^1] Example: [`wrap-bytes`](#wrap-bytes).

**r7.** A `TextIOWrapper` keeps written text in its own buffer. "If line_buffering is True, flush() is implied when a call to write contains a newline character or a carriage return."[^1] "If write_through is True, calls to write() are guaranteed not to be buffered".[^1] Example: [`flush`](#flush).

**r8.** `BytesIO` is "A binary stream using an in-memory bytes buffer."[^1] `getvalue()` returns "bytes containing the entire contents of the buffer."[^1] `getbuffer()` returns "a readable and writable view over the contents of the buffer without copying them."[^1] Example: [`bytesio`](#bytesio).

**r9.** `detach()` separates "the underlying binary buffer from the TextIOBase and return it. After the underlying buffer has been detached, the TextIOBase is in an unusable state."[^1] Example: [`detach`](#detach).

## Examples

```python run id=wrap-bytes
import io

raw = io.BytesIO('naïve\r\nline two\r\n'.encode('utf-8'))
reader = io.TextIOWrapper(raw, encoding='utf-8')
print(reader.readlines())

out = io.BytesIO()
writer = io.TextIOWrapper(out, encoding='utf-8', newline='\r\n')
writer.write('a\nb\n')
writer.flush()
print(out.getvalue())
```

```output for=wrap-bytes
['naïve\n', 'line two\n']
b'a\r\nb\r\n'
```

The reader decodes the UTF-8 bytes and turns `\r\n` into `\n`. The writer with `newline='\r\n'` turns each `\n` into `\r\n` (rule r6).

```python run id=flush
import io

out = io.BytesIO()
writer = io.TextIOWrapper(out, encoding='utf-8')
writer.write('hello')
print(out.getvalue())
writer.flush()
print(out.getvalue())

lines = io.BytesIO()
eager = io.TextIOWrapper(lines, encoding='utf-8', line_buffering=True)
eager.write('one line\n')
print(lines.getvalue())
```

```output for=flush
b''
b'hello'
b'one line\n'
```

The text stays in the wrapper until `flush()`. With `line_buffering=True`, a write that holds a newline flushes at once (rule r7).

```python run id=bytesio
import io

b = io.BytesIO(b'abcdef')
view = b.getbuffer()
view[2:4] = b'56'
print(b.getvalue())
del view
b.seek(0, io.SEEK_END)
b.write(b'!')
print(b.getvalue())
```

```output for=bytesio
b'ab56ef'
b'ab56ef!'
```

A change through the view changes the buffer (rule r8). The docs warn: "As long as the view exists, the BytesIO object cannot be resized or closed."[^1] So the example deletes the view before it writes more bytes.

```python run id=detach
import io

raw = io.BytesIO(b'header\n\x00\x01\x02')
text = io.TextIOWrapper(raw, encoding='utf-8')
print(repr(text.readline()))
binary = text.detach()
print(binary.closed, binary.tell(), len(binary.getvalue()))
```

```output for=detach
'header\n'
False 10 10
```

`detach()` returns the binary stream, and the stream stays open (rule r9). The wrapper read ahead into its own buffer, so the position of `binary` is 10, the end of the data, and not 7, right after the header line. To mix text and bytes on one stream, seek the binary stream to a known byte position first, or read only through the binary layer.

## Pitfalls

Each layer accepts only its own type:

```python fails id=str-to-bytes expect=exception:TypeError
import io

io.BytesIO().write('text')
```

"All streams are careful about the type of data you give to them. For example giving a str object to the write() method of a binary stream will raise a TypeError. So will giving a bytes object to the write() method of a text stream."[^1]

Raw writes can be partial. "Raw I/O is a low-level interface and methods generally must have their return values checked and be explicitly retried to ensure an operation completes."[^1] "High-level I/O objects like Binary I/O and Text I/O implement retry behavior."[^1]

Text I/O costs more. "Text I/O over a binary storage (such as a file) is significantly slower than binary I/O over the same storage, because it requires conversions between unicode and binary data using a character codec."[^1] Also, `tell()` and `seek()` "are both quite slow due to the reconstruction algorithm used."[^1]

Do not share a text stream between threads. "TextIOWrapper objects are not thread-safe."[^1] The buffered binary classes "protect their internal structures using a lock".[^1]

## Comparison

Go builds the same layers from small interfaces. "Package bufio implements buffered I/O. It wraps an io.Reader or io.Writer object, creating another object (Reader or Writer) that also implements the interface but provides buffering and some help for textual I/O."[^2]
