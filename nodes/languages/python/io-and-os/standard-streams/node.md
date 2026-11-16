---
tag: JPJV5D
kind: feature
owner: python
slug: standard-streams
name: Standard streams
aliases: [sys.stdin, sys.stdout, sys.stderr, stdin, stdout, stderr, standard input, standard output, standard error]
area: languages/python/io-and-os
level: intro
summary: >-
  sys.stdin, sys.stdout and sys.stderr are text files for the input, output and errors of a program. print() writes to sys.stdout unless file= names another stream.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/sys.html#sys.stdin", locator: "sys.stdin, sys.stdout, sys.stderr"}
see_also: [python/input-and-output, python/text-and-binary-io, python/error-output-redirection-and-program-termination, read-from-standard-input]
sources:
  - url: https://docs.python.org/3.15/library/sys.html#sys.stdin
    title: sys, Python 3.15 documentation
    locator: sys.stdin, sys.stdout, sys.stderr; sys.__stdin__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#print
    title: Built-in Functions, Python 3.15 documentation
    locator: print()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/io.html#io.TextIOWrapper.reconfigure
    title: io, Python 3.15 documentation
    locator: TextIOWrapper.reconfigure()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/contextlib.html#contextlib.redirect_stdout
    title: contextlib, Python 3.15 documentation
    locator: contextlib.redirect_stdout()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os#pkg-variables
    title: Package os, Go standard library
    locator: Variables, Stdin, Stdout and Stderr
    accessed: 2026-11-16
---

## Synopsis

```input id=two-lines
alpha
beta
```

```python run id=synopsis stdin=two-lines
import sys

for line in sys.stdin:
    print(line.upper(), end='')
print('done', file=sys.stderr)
sys.stdout.write('written\n')
```

```output for=synopsis
ALPHA
BETA
written
```

```output for=synopsis stream=stderr
done
```

The program reads all lines of standard input and writes them in upper case to standard output. It writes a note to standard error.

## Semantics

**r1.** `sys.stdin`, `sys.stdout` and `sys.stderr` are the "File objects used by the interpreter for standard input, output and errors".[^1] "stdin is used for all interactive input (including calls to input()); stdout is used for the output of print() and expression statements and for the prompts of input(); The interpreter’s own prompts and its error messages go to stderr."[^1]

**r2.** "These streams are regular text files like those returned by the open() function."[^1] A program can iterate over `sys.stdin`, and call `read()`, `readline()` and `write()` on the streams. Example: [`streams`](#streams).

**r3.** `print()` writes to `sys.stdout` when its `file` argument "is not present or None".[^2] `print(..., file=sys.stderr)` writes an error message.

**r4.** "When interactive, the stdout stream is line-buffered. Otherwise, it is block-buffered like regular text files. The stderr stream is line-buffered in both cases."[^1] "You can make both streams unbuffered by passing the -u command-line option or setting the PYTHONUNBUFFERED environment variable."[^1] `print(..., flush=True)` flushes one call.[^2]

**r5.** The encoding comes from the configuration of the interpreter. "Under all platforms, you can override the character encoding by setting the PYTHONIOENCODING environment variable before starting Python or by using the new -X utf8 command line option and PYTHONUTF8 environment variable."[^1]

**r6.** `sys.stdout.reconfigure()` changes the encoding, the error handler, the newline mode and the buffering of a stream at run time.[^3] "It is not possible to change the encoding or newline if some data has already been read from the stream. On the other hand, changing encoding after write is possible."[^3] Example: [`reconfigure`](#reconfigure).

**r7.** "To write or read binary data from/to the standard streams, use the underlying binary buffer object. For example, to write bytes to stdout, use sys.stdout.buffer.write(b'abc')."[^1]

**r8.** A program can assign a new object to `sys.stdout`. `sys.__stdout__` and the other two "contain the original values of stdin, stderr and stdout at the start of the program."[^1] `contextlib.redirect_stdout()` replaces `sys.stdout` for the body of a `with` statement.[^4] Example: [`redirect`](#redirect).

## Examples

```python run id=streams stdin=two-lines
import sys

first = sys.stdin.readline()
rest = sys.stdin.read()
print(repr(first), repr(rest))
print('to stderr', file=sys.stderr)
```

```output for=streams
'alpha\n' 'beta\n'
```

```output for=streams stream=stderr
to stderr
```

`readline()` reads one line with its newline, and `read()` reads the rest (rule r2). The first output block is standard output, and the second one is standard error (rule r3).

```python run id=reconfigure
import sys

sys.stdout.reconfigure(encoding='ascii', errors='backslashreplace')
print('café 😀')
```

```output for=reconfigure
caf\xe9 \U0001f600
```

After `reconfigure()`, the stream encodes the text as ASCII and writes an escape for each character that ASCII does not have (rule r6).

```python run id=redirect
import contextlib
import io

buffer = io.StringIO()
with contextlib.redirect_stdout(buffer):
    print('captured')
    help(len)
print('back on stdout')
print(buffer.getvalue().splitlines()[0])
```

```output for=redirect
back on stdout
captured
```

Inside the `with` block, `print()` and `help()` write to the buffer (rule r8). After the block, `sys.stdout` is the real stream again.

```python run id=bytes
import sys

sys.stdout.flush()
sys.stdout.buffer.write(b'raw bytes\n')
```

```output for=bytes
raw bytes
```

The binary buffer under the text stream takes bytes (rule r7). Flush the text stream first, so text that is still in its buffer does not come after the bytes.

## Pitfalls

The order of stdout and stderr can change:

```python run id=order
import subprocess
import sys

child = "import sys; print('one'); print('two', file=sys.stderr)"
result = subprocess.run([sys.executable, '-c', child],
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
print(result.stdout.split())
```

```output for=order
['two', 'one']
```

The child writes both streams to one pipe. When stdout is a pipe, it is block-buffered, and stderr is line-buffered (rule r4). So the message on stderr can come out before an earlier message on stdout. Use `flush=True`, the `-u` option or `PYTHONUNBUFFERED` when the order matters.

A replaced stream may have no buffer:

```python fails id=no-buffer expect=exception:AttributeError
import contextlib
import io

with contextlib.redirect_stdout(io.StringIO()):
    import sys
    sys.stdout.buffer.write(b'x')
```

The docs warn that "the standard streams may be replaced with file-like objects like io.StringIO which do not support the buffer attribute."[^1] A library that writes bytes must check for the attribute.

`redirect_stdout()` does not reach child processes. The docs say: "It also has no effect on the output of subprocesses."[^4] It also changes global state, so it is "not suitable for use in library code and most threaded applications."[^4]

## Comparison

Go also has three variables for the streams: "Stdin, Stdout, and Stderr are open Files pointing to the standard input, standard output, and standard error file descriptors."[^5] The Go docs add: "the Go runtime writes to standard error for panics and crashes".[^5] In the same way, the Python interpreter writes its error messages to `sys.stderr` (rule r1).
