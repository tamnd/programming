---
tag: CKPD1Q
kind: feature
owner: python
slug: reading-and-writing-files
name: Reading and Writing Files
aliases: [open(), open a file, file modes, with open, text mode, binary mode, file encoding]
area: languages/python/io-and-os
level: intro
summary: >-
  open(name, mode) returns a file object. The mode picks reading, writing or appending, and text or binary. Use a with statement so that the file closes, and give the encoding of text files.
grammar: "open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)"
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#open", locator: "open()"}
see_also: [python/methods-of-file-objects, python/encoding-and-decoding, read-a-whole-file, read-a-file-line-by-line, write-a-file, append-to-a-file]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html#reading-and-writing-files
    title: The Python Tutorial, 7. Input and output
    locator: 7.2. Reading and writing files
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#open
    title: Built-in Functions, Python 3.15 documentation
    locator: open()
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0686/
    title: "PEP 686: Make UTF-8 mode default"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os#ReadFile
    title: Package os, Go standard library
    locator: func ReadFile; func WriteFile
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
with open('notes.txt', 'w', encoding='utf-8') as f:
    f.write('first line\n')
    f.write('second line\n')

with open('notes.txt', 'a', encoding='utf-8') as f:
    f.write('third line\n')

with open('notes.txt', encoding='utf-8') as f:
    print(f.read(), end='')
```

```output for=synopsis
first line
second line
third line
```

Mode `'w'` makes a new file, mode `'a'` adds to the end, and the default mode `'r'` reads. Each `with` statement closes its file at the end of the block.

## Semantics

**r1.** "open() returns a file object, and is most commonly used with two positional arguments: open(filename, mode)".[^1] "If the file cannot be opened, an OSError is raised."[^2]

**r2.** The mode is a string of characters.[^2]

| Character | Meaning |
|---|---|
| `'r'` | "open for reading (default)" |
| `'w'` | "open for writing, truncating the file first" |
| `'x'` | "open for exclusive creation, failing if the file already exists" |
| `'a'` | "open for writing, appending to the end of file if it exists" |
| `'b'` | "binary mode" |
| `'t'` | "text mode (default)" |
| `'+'` | "open for updating (reading and writing)" |

"Modes 'w+' and 'w+b' open and truncate the file. Modes 'r+' and 'r+b' open the file with no truncation."[^2] Example: [`exclusive`](#exclusive).

**r3.** "Normally, files are opened in text mode, that means, you read and write strings from and to the file, which are encoded in a specific encoding."[^1] "Files opened in binary mode (including 'b' in the mode argument) return contents as bytes objects without any decoding."[^2] "You can not specify encoding when opening file in binary mode."[^1] Example: [`binary`](#binary).

**r4.** In Python 3.15, "In text mode, if encoding is not specified, UTF-8 is used by default; if Python UTF-8 Mode is disabled, locale.getencoding() is called to get the current locale encoding."[^2] Before 3.15, the default was the locale encoding. PEP 686 made the change.[^3]

**r5.** "In text mode, the default when reading is to convert platform-specific line endings (\n on Unix, \r\n on Windows) to just \n. When writing in text mode, the default is to convert occurrences of \n back to platform-specific line endings."[^1] The `newline` argument changes this.[^2] Example: [`newlines`](#newlines).

**r6.** "It is good practice to use the with keyword when dealing with file objects. The advantage is that the file is properly closed after its suite finishes, even if an exception is raised at some point."[^1] Without `with`, call `f.close()`.[^1]

**r7.** "After a file object is closed, either by a with statement or by calling f.close(), attempts to use the file object will automatically fail."[^1] Example: [`closed`](#closed).

**r8.** The type of the file object depends on the mode: text modes give an `io.TextIOWrapper`, and binary modes give an `io.BufferedReader`, `io.BufferedWriter` or `io.BufferedRandom`.[^2]

## Examples

```python run id=exclusive
with open('data.txt', 'x', encoding='utf-8') as f:
    f.write('created once\n')

try:
    open('data.txt', 'x', encoding='utf-8')
except FileExistsError as err:
    print(type(err).__name__)
```

```output for=exclusive
FileExistsError
```

Mode `'x'` fails when the file exists, so it never overwrites a file by mistake (rule r2).

```python run id=binary
with open('blob.bin', 'wb') as f:
    f.write(bytes([0, 1, 2, 255]))

with open('blob.bin', 'rb') as f:
    data = f.read()
print(data, type(data).__name__, len(data))
```

```output for=binary
b'\x00\x01\x02\xff' bytes 4
```

In binary mode, Python reads and writes bytes objects, and it does not decode or change line endings (rule r3).

```python run id=newlines
with open('crlf.txt', 'wb') as f:
    f.write(b'one\r\ntwo\r\n')

with open('crlf.txt', encoding='utf-8') as f:
    print(repr(f.read()))
with open('crlf.txt', encoding='utf-8', newline='') as f:
    print(repr(f.read()))
```

```output for=newlines
'one\ntwo\n'
'one\r\ntwo\r\n'
```

The default reading mode turns `\r\n` into `\n`. With `newline=''`, the text keeps its line endings (rule r5).

```python run id=closed
with open('notes.txt', 'w', encoding='utf-8') as f:
    f.write('text\n')
print(f.closed)
try:
    f.write('more')
except ValueError as err:
    print(err)
```

```output for=closed
True
I/O operation on closed file.
```

The `with` statement closes the file, and a later write fails with ValueError (rules r6 and r7).

## Versions

| Version | Change |
|---|---|
| 3.3 | The `'x'` mode and the `opener` parameter were added.[^2] |
| 3.6 | `open()` accepts path-like objects such as `pathlib.Path`.[^2] |
| 3.11 | The `'U'` mode was removed.[^2] |
| 3.15 | "UTF-8 is now the default encoding, instead of the platform-dependent locale encoding (PEP 686)."[^2] |

## Pitfalls

Mode `'w'` erases the file:

```python run id=truncate
with open('log.txt', 'w', encoding='utf-8') as f:
    f.write('old entry\n')
with open('log.txt', 'w', encoding='utf-8') as f:
    f.write('new entry\n')
with open('log.txt', encoding='utf-8') as f:
    print(f.read(), end='')
```

```output for=truncate
new entry
```

The tutorial says that with `'w'`, "an existing file with the same name will be erased".[^1] Use `'a'` to add to a file, or `'x'` to refuse to replace it.

Text mode corrupts binary data. The default line ending change "is fine for text files, but will corrupt binary data like that in JPEG or EXE files."[^1] Open images, archives and programs with `'rb'` or `'wb'`.

A file opened without `with` may lose data. The tutorial warns: "Calling f.write() without using the with keyword or calling f.close() might result in the arguments of f.write() not being completely written to the disk, even if the program exits successfully."[^1]

Leave out `encoding` only when the code runs on Python 3.15 or later, or in UTF-8 mode. On older versions, the default is the locale encoding, so the same file can read differently on Windows and on Linux (rule r4).

## Comparison

Go has one-call helpers for small files. "ReadFile reads the named file and returns the contents."[^4] `os.WriteFile` writes a whole file in the same way. Python reaches the same result with `pathlib.Path.read_text()` and `write_text()`, or with `open()` and `with` when it reads a file in parts.
