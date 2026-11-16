---
tag: 8HNB9B
kind: feature
owner: python
slug: methods-of-file-objects
name: Methods of File Objects
aliases: [f.read, f.readline, f.readlines, f.write, f.writelines, f.seek, f.tell, file methods, iterate over a file]
area: languages/python/io-and-os
level: intro
summary: >-
  A file object reads with read(), readline() and iteration, writes with write() and writelines(), and moves with seek() and tell(). Text files and binary files differ in what seek() accepts.
grammar: "f.read(size=-1); f.readline(size=-1); f.write(s); f.seek(offset, whence=0); f.tell()"
spec: {standard: null, url: "https://docs.python.org/3.15/library/io.html#io.IOBase", locator: "io.IOBase, io.TextIOBase"}
see_also: [python/reading-and-writing-files, python/standard-streams, read-a-file-line-by-line, read-a-whole-file, write-a-file]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#methods-of-file-objects
    title: The Python Tutorial, 7. Input and output
    locator: 7.2.1. Methods of file objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/io.html#io.IOBase
    title: io, Python 3.15 documentation
    locator: io.IOBase; io.TextIOBase.seek(); io.UnsupportedOperation
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os#File.Seek
    title: Package os, Go standard library
    locator: func (*File) Seek
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
with open('poem.txt', 'w', encoding='utf-8') as f:
    print(f.write('roses are red\n'))
    f.writelines(['violets are blue\n', 'files have methods\n'])

with open('poem.txt', encoding='utf-8') as f:
    print(repr(f.readline()))
    for line in f:
        print(line.rstrip('\n'))
```

```output for=synopsis
14
'roses are red\n'
violets are blue
files have methods
```

`write()` returns the number of characters it wrote. `readline()` reads one line, and the `for` loop reads the rest of the lines.

## Semantics

**r1.** `f.read(size)` "reads some quantity of data and returns it as a string (in text mode) or bytes object (in binary mode)."[^1] "When size is omitted or negative, the entire contents of the file will be read and returned".[^1] "If the end of the file has been reached, f.read() will return an empty string ('')."[^1] Example: [`read-parts`](#read-parts).

**r2.** "f.readline() reads a single line from the file; a newline character (\n) is left at the end of the string, and is only omitted on the last line of the file if the file doesn’t end in a newline."[^1] So "if f.readline() returns an empty string, the end of the file has been reached, while a blank line is represented by '\n', a string containing only a single newline."[^1]

**r3.** "For reading lines from a file, you can loop over the file object. This is memory efficient, fast, and leads to simple code".[^1] "If you want to read all the lines of a file in a list you can also use list(f) or f.readlines()."[^1]

**r4.** "f.write(string) writes the contents of string to the file, returning the number of characters written."[^1] "Other types of objects need to be converted – either to a string (in text mode) or a bytes object (in binary mode) – before writing them".[^1]

**r5.** `writelines()` writes a list of lines. "Line separators are not added, so it is usual for each of the lines provided to have a line separator at the end."[^2]

**r6.** "f.tell() returns an integer giving the file object’s current position in the file represented as number of bytes from the beginning of the file when in binary mode and an opaque number when in text mode."[^1]

**r7.** "To change the file object’s position, use f.seek(offset, whence)."[^1] "A whence value of 0 measures from the beginning of the file, 1 uses the current file position, and 2 uses the end of the file as the reference point."[^1] Example: [`seek-binary`](#seek-binary).

**r8.** "In text files (those opened without a b in the mode string), only seeks relative to the beginning of the file are allowed (the exception being seeking to the very file end with seek(0, 2)) and the only valid offset values are those returned from the f.tell(), or zero."[^1] Example: [`seek-text`](#seek-text).

**r9.** "File objects have some additional methods, such as isatty() and truncate() which are less frequently used".[^1] `truncate()` resizes "the stream to the given size in bytes (or the current position if size is not specified)."[^2]

## Examples

```python run id=read-parts
with open('abc.txt', 'w', encoding='utf-8') as f:
    f.write('abcdefgh\n')

with open('abc.txt', encoding='utf-8') as f:
    print(repr(f.read(3)))
    print(repr(f.read(3)))
    print(repr(f.read()))
    print(repr(f.read()))
```

```output for=read-parts
'abc'
'def'
'gh\n'
''
```

Each `read(3)` returns at most three characters from the current position. At the end of the file, `read()` returns `''` (rule r1).

```python run id=seek-binary
with open('work.bin', 'wb+') as f:
    f.write(b'0123456789abcdef')
    print(f.seek(5), f.read(1))
    print(f.seek(-3, 2), f.read(1))
    print(f.tell())
```

```output for=seek-binary
5 b'5'
13 b'd'
14
```

This is the example of the tutorial. `seek()` returns the new position. `seek(-3, 2)` counts three bytes back from the end, and `tell()` shows the position after the read (rules r6 and r7).

```python run id=seek-text
import io

with open('text.txt', 'w+', encoding='utf-8') as f:
    f.write('héllo\nworld\n')
    f.seek(0)
    f.readline()
    mark = f.tell()
    print(f.readline(), end='')
    f.seek(mark)
    print(f.readline(), end='')
    try:
        f.seek(-3, 2)
    except io.UnsupportedOperation as err:
        print('error:', err)
```

```output for=seek-text
world
world
error: can't do nonzero end-relative seeks
```

In text mode, a position from `tell()` is a safe target for `seek()`. A seek from the end with a nonzero offset raises `io.UnsupportedOperation` (rule r8), which is "An exception inheriting OSError and ValueError that is raised when an unsupported operation is called on a stream."[^2]

## Pitfalls

`write()` does not add a newline:

```python run id=no-newline
with open('list.txt', 'w', encoding='utf-8') as f:
    for word in ['one', 'two']:
        f.write(word)
    f.writelines(['three', 'four'])

with open('list.txt', encoding='utf-8') as f:
    print(f.read())
```

```output for=no-newline
onetwothreefour
```

Add `'\n'` to each line, or use `print(word, file=f)`, which ends each line with a newline (rules r4 and r5).

`write()` takes only a string in text mode:

```python fails id=write-int expect=exception:TypeError
with open('count.txt', 'w', encoding='utf-8') as f:
    f.write(42)
```

Convert the value first, as in `f.write(str(42))` (rule r4).

`read()` with no size reads the whole file. The tutorial warns: "it’s your problem if the file is twice as large as your machine’s memory."[^1] For a large file, loop over the lines or read in blocks with `read(size)`.

A text position is not a character count. In the example [`seek-text`](#seek-text), the text has a two-byte `é`, and `tell()` returns "an opaque number" (rule r6). Do not compute a text offset with arithmetic.

## Comparison

Go uses the same whence values: "Seek sets the offset for the next Read or Write on file to offset, interpreted according to whence: 0 means relative to the origin of the file, 1 means relative to the current offset, and 2 means relative to the end."[^3] A Go `*os.File` works with bytes only. Python adds the text layer, and that layer limits `seek()` (rule r8).
