---
tag: GE4BW4
kind: feature
owner: python
slug: input-and-output
name: Input and Output
aliases: [print, input, print(), input(), console input and output, Python I/O]
area: languages/python/io-and-os
level: intro
summary: >-
  print() writes values as text to standard output or to a file, and input() reads one line from standard input. Files and the json module keep data for later use.
grammar: "print(*objects, sep=' ', end='\\n', file=None, flush=False); input(prompt, /)"
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/functions.html#print", locator: "print()"}
see_also: [python/fancier-output-formatting, python/reading-and-writing-files, python/standard-streams, read-from-standard-input, print-formatted-output]
sources:
  - url: https://docs.python.org/3.15/tutorial/inputoutput.html
    title: The Python Tutorial, 7. Input and output
    locator: 7. Input and output; 7.1. Fancier output formatting
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#print
    title: Built-in Functions, Python 3.15 documentation
    locator: print()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#input
    title: Built-in Functions, Python 3.15 documentation
    locator: input()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#EOFError
    title: Built-in Exceptions, Python 3.15 documentation
    locator: EOFError
    accessed: 2026-11-16
  - url: https://pkg.go.dev/fmt#Println
    title: Package fmt, Go standard library
    locator: func Println
    accessed: 2026-11-16
---

## Synopsis

```input id=names
Ada
Grace
```

```python run id=synopsis stdin=names
name = input('Name? ')
print('Hello,', name)
print('a', 'b', 'c', sep='-', end='!\n')
```

```output for=synopsis
Name? Hello, Ada
a-b-c!
```

`input()` writes the prompt and reads one line. `print()` writes its arguments, with `sep` between them and `end` after them. The prompt and the greeting are on one line because the input text is not echoed when it comes from a pipe.

## Semantics

**r1.** "There are several ways to present the output of a program; data can be printed in a human-readable form, or written to a file for future use."[^1] The tutorial names two ways to write values, "expression statements and the print() function", and a third way, "the write() method of file objects".[^1]

**r2.** `print()` writes "objects to the text stream file, separated by sep and followed by end."[^2] The arguments "are converted to strings like str() does".[^2] Example: [`print-args`](#print-args).

**r3.** `sep`, `end`, `file` and `flush` "must be given as keyword arguments".[^2] "If no objects are given, print() will just write end."[^2]

**r4.** "The file argument must be an object with a write(string) method; if it is not present or None, sys.stdout will be used."[^2] Example: [`print-file`](#print-file).

**r5.** "Output buffering is usually determined by file. However, if flush is true, the stream is forcibly flushed."[^2]

**r6.** `input(prompt)` writes the prompt "to standard output without a trailing newline. The function then reads a line from input, converts it to a string (stripping a trailing newline), and returns that. When EOF is read, EOFError is raised."[^3] Example: [`read-all`](#read-all).

**r7.** The other pages of this group cover the rest of the tutorial chapter: [fancier output formatting](python/fancier-output-formatting), [reading and writing files](python/reading-and-writing-files), [methods of file objects](python/methods-of-file-objects) and saving structured data with the json module.[^1]

## Examples

```python run id=print-args
print(1, 2.5, None, [3, 'x'])
print()
print('no newline', end='')
print(' after')
```

```output for=print-args
1 2.5 None [3, 'x']

no newline after
```

Each argument becomes text as `str()` makes it (rule r2). With no arguments, `print()` writes only `end`, so the second line is empty (rule r3).

```python run id=print-file
import io

buffer = io.StringIO()
print('first', file=buffer)
print('second', file=buffer)
print(repr(buffer.getvalue()))
```

```output for=print-file
'first\nsecond\n'
```

Any object with a `write()` method can take the output, here an in-memory text stream (rule r4).

```input id=lines
3
4
5
```

```python run id=read-all stdin=lines
total = 0
while True:
    try:
        line = input()
    except EOFError:
        break
    total += int(line)
print('total:', total)
```

```output for=read-all
total: 12
```

The loop reads lines until `input()` raises EOFError at the end of the input (rule r6). "the io.TextIOBase.read() and io.IOBase.readline() methods return an empty string when they hit EOF."[^4] So code that reads `sys.stdin` directly tests for `''` instead.

## Pitfalls

`input()` always returns a string:

```input id=two
2
```

```python fails id=input-str expect=exception:TypeError stdin=two
n = input()
print(n + 1)
```

The text `'2'` and the int `1` cannot be added. Convert the text first: `int(input())`.

`print()` cannot write to a binary file:

```python fails id=print-binary expect=exception:TypeError
import io

print('data', file=io.BytesIO())
```

"Since printed arguments are converted to text strings, print() cannot be used with binary mode file objects. For these, use file.write(...) instead."[^2]

## Comparison

Go splits the same work into separate functions. "Println formats using the default formats for its operands and writes to standard output. Spaces are always added between operands and a newline is appended."[^5] Python has one `print()` with the `sep`, `end` and `file` keyword arguments, where Go has `Print`, `Println` and `Fprintln`.
