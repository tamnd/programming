---
tag: GQVZWX
kind: feature
owner: python
slug: error-output-redirection-and-program-termination
name: Error Output Redirection and Program Termination
aliases: [sys.stderr, sys.exit, SystemExit, exit status, exit code, os._exit, atexit]
area: languages/python/io-and-os
level: intro
summary: >-
  Warnings and errors go to sys.stderr, which stays visible when stdout is redirected. sys.exit() ends a program by raising SystemExit, and its argument sets the exit status.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/sys.html#sys.exit", locator: "sys.exit()"}
see_also: [python/standard-streams, python/command-line-arguments, python/exceptions, python/clean-up-actions-with-finally, python/input-and-output]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#error-output-redirection-and-program-termination
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.4. Error output redirection and program termination
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.exit(); sys.stdin, sys.stdout, sys.stderr; sys.excepthook()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#SystemExit
    title: Built-in exceptions, Python 3.15 documentation
    locator: exception SystemExit
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/os.html#os._exit
    title: os, Python 3.15 documentation
    locator: os._exit(); os.EX_OK
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/atexit.html
    title: atexit, Python 3.15 documentation
    locator: module introduction
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os#Exit
    title: Package os, Go standard library
    locator: func Exit; Stdin, Stdout, Stderr
    accessed: 2026-11-16
---

## Synopsis

```python fails id=synopsis expect=exit:1
import sys

def load(name):
    try:
        with open(name, encoding='utf-8') as f:
            return f.read()
    except FileNotFoundError:
        print(f'{name}: no such file', file=sys.stderr)
        sys.exit(1)

print('starting')
load('missing.cfg')
print('not reached')
```

```output for=synopsis
starting
```

```output for=synopsis stream=stderr
missing.cfg: no such file
```

The message goes to standard error and the program ends with status 1. A shell that runs `python app.py > out.txt` still shows the message on the terminal.

## Semantics

**r1.** "The sys module also has attributes for stdin, stdout, and stderr. The latter is useful for emitting warnings and error messages to make them visible even when stdout has been redirected".[^1] "The interpreter's own prompts and its error messages go to stderr."[^2] `print(..., file=sys.stderr)` and `sys.stderr.write()` both write there.

**r2.** "The stderr stream is line-buffered in both cases."[^2] stdout is line-buffered only when interactive; "Otherwise, it is block-buffered like regular text files."[^2] When both go to one place, flush stdout first to keep the order.

**r3.** "The most direct way to terminate a script is to use sys.exit()."[^1] `sys.exit()` will "Raise a SystemExit exception, signaling an intention to exit the interpreter."[^2] "Cleanup actions specified by finally clauses of try statements are honored, and it is possible to intercept the exit attempt at an outer level."[^2] Example: [`finally`](#finally).

**r4.** The argument "can be an integer giving the exit status (defaulting to zero), or another type of object. If it is an integer, zero is considered "successful termination" and any nonzero value is considered "abnormal termination" by shells and the like."[^2] "Unix programs generally use 2 for command line syntax errors and 1 for all other kinds of errors."[^2]

**r5.** "None is equivalent to passing zero, and any other object is printed to stderr and results in an exit code of 1. In particular, sys.exit("some error message") is a quick way to exit a program when an error occurs."[^2] Example: [`message`](#message).

**r6.** `SystemExit` "inherits from BaseException instead of Exception so that it is not accidentally caught by code that catches Exception."[^3] "When it is not handled, the Python interpreter exits; no stack traceback is printed."[^3] Its `code` attribute holds "The exit status or error message that is passed to the constructor."[^3]

**r7.** For any other uncaught exception, "the interpreter calls sys.excepthook with three arguments, the exception class, exception instance, and a traceback object."[^2] The hook "prints out a given traceback and exception to sys.stderr."[^2] Example: [`uncaught`](#uncaught).

**r8.** `os._exit(n)` will "Exit the process with status n, without calling cleanup handlers, flushing stdio buffers, etc."[^4] "The standard way to exit is sys.exit(n). _exit() should normally only be used in the child process after a fork()."[^4]

**r9.** The atexit module registers "functions that are automatically executed "at exit", that is, upon normal program termination (for instance, if sys.exit() is called or the main module's execution completes)".[^5] "At exit, all registered exit handlers are called in the reverse order in which they were registered."[^5] Example: [`atexit`](#atexit).

## Examples

```python run id=finally
import sys

try:
    try:
        sys.exit(3)
    finally:
        print('cleanup runs')
except SystemExit as exc:
    print('caught', exc.code)
print('still running')
```

```output for=finally
cleanup runs
caught 3
still running
```

`sys.exit()` only raises an exception, so `finally` runs and an outer handler can stop the exit (rule r3).

```python fails id=message expect=exit:1
import sys

sys.exit('config file is empty')
```

```output for=message stream=stderr
config file is empty
```

A string argument goes to standard error, and the status is 1 (rule r5).

```python fails id=uncaught expect=exception:ValueError
def parse(text):
    return int(text)

print('parsing')
parse('x')
```

```output for=uncaught
parsing
```

An uncaught exception ends the program, and `sys.excepthook` prints the traceback on standard error (rule r7).

```python fails id=atexit expect=exit:2
import atexit
import sys

atexit.register(print, 'first registered')
atexit.register(print, 'second registered')
print('main done')
sys.exit(2)
```

```output for=atexit
main done
second registered
first registered
```

The handlers run after `sys.exit()`, the last one first (rule r9).

## Pitfalls

A bare `except:` or `except BaseException:` also catches `SystemExit`, so the program does not stop:

```python run id=bare-except
import sys

try:
    sys.exit(1)
except BaseException:
    print('exit swallowed')
print('still running')
```

```output for=bare-except
exit swallowed
still running
```

`except Exception:` does not catch it (rule r6).

`sys.exit()` in another thread does not stop the program. "Since exit() ultimately "only" raises an exception, it will only exit the process when called from the main thread, and the exception is not intercepted."[^2]

Keep the status small. "Most systems require it to be in the range 0–127, and produce undefined results otherwise."[^2]

`os._exit()` skips `finally` blocks, atexit handlers and the flush of buffered output (rule r8). Text printed just before it can be lost.

## Comparison

Go's `os.Exit` "causes the current program to exit with the given status code. Conventionally, code zero indicates success, non-zero an error. The program terminates immediately; deferred functions are not run."[^6] It is closer to `os._exit()` than to `sys.exit()`, which runs `finally` blocks (rules r3 and r8). Go has `os.Stderr` for the error stream, and "the Go runtime writes to standard error for panics and crashes".[^6]
