---
tag: J7951Z
kind: feature
owner: python
slug: development-mode-and-x-options
name: Development Mode and -X Options
aliases: [Python Development Mode, "-X dev", PYTHONDEVMODE, sys.flags.dev_mode, "-X options", sys._xoptions, "-X utf8", "-X int_max_str_digits", "-X faulthandler", "-X importtime", "-X tracemalloc", ResourceWarning]
area: languages/python/tooling
level: advanced
summary: >-
  -X passes implementation-specific options to CPython. -X dev turns on development mode: more warnings, faulthandler, asyncio debug mode and argument checks, with no extra output for correct code.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/devmode.html", locator: "Effects of the Python Development Mode"}
see_also: [python/the-python-interpreter, python/invoking-the-interpreter, python/the-interpreter-and-its-environment, python/compiled-python-files, python/testing-async-code]
sources:
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: -X
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys._xoptions; sys.flags
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/devmode.html
    title: Python Development Mode, Python 3.15 documentation
    locator: Effects of the Python Development Mode; ResourceWarning Example
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/warnings.html
    title: warnings, Python 3.15 documentation
    locator: Default Warning Filter
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/faulthandler.html
    title: faulthandler, Python 3.15 documentation
    locator: module overview
    accessed: 2026-11-16
  - url: https://go.dev/doc/godebug
    title: Go, Backwards Compatibility, and GODEBUG
    locator: Introduction
    accessed: 2026-11-16
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Compile packages and dependencies (-race)
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile

script = '''\
import sys


def main():
    fp = open(sys.argv[1], encoding='utf-8')
    print(len(fp.readlines()))
    # the file is never closed


main()
'''
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'script.py').write_text(script)
    pathlib.Path(tmp, 'data.txt').write_text('one\ntwo\nthree\n')
    for opts in ([], ['-X', 'dev']):
        result = subprocess.run([sys.executable, *opts, 'script.py', 'data.txt'],
                                cwd=tmp, capture_output=True, text=True)
        print(opts, 'stdout:', result.stdout.strip())
        for line in result.stderr.splitlines():
            print('   ', line.replace(os.path.realpath(tmp), '<tmp>'))
```

```output for=synopsis
[] stdout: 3
['-X', 'dev'] stdout: 3
    <tmp>/script.py:10: ResourceWarning: unclosed file <_io.TextIOWrapper name='data.txt' mode='r' encoding='utf-8'>
      main()
    ResourceWarning: Enable tracemalloc to get the object allocation traceback
```

This is the script from the development mode docs: it opens a file and never closes it. A plain run prints the line count and nothing else. With `-X dev` the same run also reports the unclosed file, with the line that made the call.

## Semantics

**r1.** `-X` is "Reserved for various implementation-specific options."[^1] Each value turns on one feature of CPython, such as `-X faulthandler`, `-X tracemalloc`, `-X importtime` "to show how long each import takes", `-X utf8` and `-X int_max_str_digits`.[^1]

**r2.** `sys._xoptions` is "A dictionary of the various implementation-specific flags passed through the -X command-line option. Option names are either mapped to their values, if given explicitly, or to True."[^2] "This is a CPython-specific way of accessing options passed through -X."[^2]

**r3.** "The Python Development Mode introduces additional runtime checks that are too expensive to be enabled by default. It should not be more verbose than the default if the code is correct; new warnings are only emitted when an issue is detected."[^3] "It can be enabled using the -X dev command line option or by setting the PYTHONDEVMODE environment variable to 1."[^3]

**r4.** Development mode adds a default warning filter, so `DeprecationWarning`, `ImportWarning`, `PendingDeprecationWarning` and `ResourceWarning` are shown; "It behaves as if the -W default command line option is used."[^3] Without it, the default filters ignore those categories, except a `DeprecationWarning` raised by code in `__main__`.[^4]

**r5.** Development mode also installs debug hooks on the memory allocators, calls `faulthandler.enable()` "to dump the Python traceback on a crash", and enables asyncio debug mode.[^3] faulthandler handles "the SIGSEGV, SIGFPE, SIGABRT, SIGBUS, and SIGILL signals".[^5]

**r6.** It checks arguments that are normally checked lazily. "By default, for best performance, the errors argument is only checked at the first encoding/decoding error and the encoding argument is sometimes ignored for empty strings."[^3] Development mode checks them on every call, for `open()`, `str.encode()` and `bytes.decode()`.[^3]

**r7.** Development mode "can only be enabled at the Python startup. Its value can be read from sys.flags.dev_mode."[^3] It "does not prevent the -O command line option from removing assert statements".[^3]

## Examples

### What -X dev and PYTHONDEVMODE turn on

```python run id=effects
import os
import subprocess
import sys

code = '''\
import asyncio, faulthandler, sys, warnings
warnings.warn('old', PendingDeprecationWarning)
loop = asyncio.new_event_loop()
print(sys.flags.dev_mode, faulthandler.is_enabled(), loop.get_debug())
loop.close()
'''
runs = [('plain', [], {}), ('-X dev', ['-X', 'dev'], {}),
        ('PYTHONDEVMODE=1', [], {'PYTHONDEVMODE': '1'})]
for label, opts, extra in runs:
    result = subprocess.run([sys.executable, *opts, '-c', code],
                            env={**os.environ, **extra}, capture_output=True, text=True)
    warned = 'PendingDeprecationWarning' in result.stderr
    print(f'{label:16}', result.stdout.strip(), 'warned:', warned)
```

```output for=effects
plain            False False False warned: False
-X dev           True True True warned: True
PYTHONDEVMODE=1  True True True warned: True
```

The columns are `sys.flags.dev_mode`, `faulthandler.is_enabled()`, the debug flag of a new asyncio loop, and whether the `PendingDeprecationWarning` reached stderr. A plain run ignores that warning (r4). The option and the variable have the same effect (r3, r5).

### An error handler checked early

```python run id=errors
import subprocess
import sys

code = "print('abc'.encode('utf-8', 'stric'))"
for opts in ([], ['-X', 'dev']):
    result = subprocess.run([sys.executable, *opts, '-c', code], capture_output=True, text=True)
    print(opts, result.returncode, (result.stdout or result.stderr.splitlines()[-1]).strip())
```

```output for=errors
[] 0 b'abc'
['-X', 'dev'] 1 LookupError: unknown error handler name 'stric'
```

`'stric'` is a typo for `'strict'`. ASCII text never needs the error handler, so a plain run never looks it up and the typo hides until some input fails to encode. Development mode checks the name on the first call (r6).

### Reading -X options

```python run id=xoptions
import subprocess
import sys

code = "import sys; print(sys._xoptions, sys.flags.utf8_mode, sys.flags.int_max_str_digits)"
for opts in (['-X', 'utf8'], ['-X', 'int_max_str_digits=640', '-X', 'mykey=1'], ['-X', 'utf8=0']):
    result = subprocess.run([sys.executable, *opts, '-c', code], capture_output=True, text=True)
    print(opts, result.stdout.strip())

big = "int('1' * 641)"
result = subprocess.run([sys.executable, '-X', 'int_max_str_digits=640', '-c', big],
                        capture_output=True, text=True)
print(result.stderr.splitlines()[-1])
```

```output for=xoptions
['-X', 'utf8'] {'utf8': True} 1 4300
['-X', 'int_max_str_digits=640', '-X', 'mykey=1'] {'int_max_str_digits': '640', 'mykey': '1'} 0 640
['-X', 'utf8=0'] {'utf8': '0'} 0 4300
ValueError: Exceeds the limit (640 digits) for integer string conversion: value has 641 digits; use sys.set_int_max_str_digits() to increase the limit
```

A bare option maps to `True` and `name=value` maps to the string (r2). The interpreter also sets the matching `sys.flags` fields: `utf8_mode` and `int_max_str_digits`. `mykey` means nothing to CPython; it is kept in `sys._xoptions` for the program to read. The last line shows the 640-digit limit at work; 4300 is the default limit.

## Pitfalls

- Development mode makes no noise for correct code (r3), so a quiet `-X dev` run is a good result. Run the test suite with it, not only the program.
- `-X dev` comes before the script or `-c`. After the script it is an argument to the program.
- Development mode cannot be switched on from inside a running program (r7). Set `PYTHONDEVMODE=1` in the environment of a child process, or pass `-X dev` on its command line.
- `sys._xoptions` is CPython-specific (r2). Code that must run on other implementations should not depend on it.
- `-O` with `-X dev` still strips `assert` statements (r7).

## Comparison

Go has no single development switch. Runtime behaviour is tuned with `GODEBUG`: "A GODEBUG setting is a key=value pair that controls the execution of certain parts of a Go program", and "The environment variable GODEBUG can hold a comma-separated list of these settings."[^6] "Unrecognized settings in the GODEBUG environment variable are ignored", much like an unknown `-X` name in Python.[^6] Extra checks are build flags instead: `go build -race` and `go test -race` "enable data race detection."[^7]

[^3]: Python Development Mode, Python 3.15 documentation.
[^1]: Command line and environment, Python 3.15 documentation.
[^2]: sys, Python 3.15 documentation.
[^4]: warnings, Python 3.15 documentation.
[^5]: faulthandler, Python 3.15 documentation.
[^6]: Go, Backwards Compatibility, and GODEBUG.
[^7]: Command go, Go documentation.
