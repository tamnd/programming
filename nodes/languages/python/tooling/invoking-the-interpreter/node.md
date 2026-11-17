---
tag: RXMMS6
kind: feature
owner: python
slug: invoking-the-interpreter
name: Invoking the Interpreter
aliases: [sys.argv, sys.orig_argv, argument passing, "sys.argv[0]", python -c, python -m, "sys.path[0]", python -P, python -I, PYTHONSAFEPATH, module shadowing]
area: languages/python/tooling
level: intro
summary: >-
  How the interpreter turns its command line into sys.argv, what argv[0] is for a script, -c, -m and standard input, and which directory each mode puts first on sys.path.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/using/cmdline.html", locator: "1.1.1. Interface options"}
see_also: [python/the-python-interpreter, python/the-interpreter-and-its-environment, python/command-line-arguments, python/development-mode-and-x-options]
sources:
  - url: https://docs.python.org/3.15/tutorial/interpreter.html
    title: Using the Python Interpreter, The Python Tutorial 3.15
    locator: 2.1.1. Argument Passing
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: 1.1.1. Interface options; -I; -P
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.argv; sys.orig_argv
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os
    title: Package os, Go standard library
    locator: Variables (Args)
    accessed: 2026-11-16
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Compile and run Go program
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile

show = 'import sys; print(sys.argv)\n'
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'show.py').write_text(show)

    def run(label, *args, stdin=None):
        out = subprocess.run([sys.executable, *args], input=stdin, cwd=tmp,
                             capture_output=True, text=True).stdout
        print(f'{label:7}', out.replace(os.path.realpath(tmp), '<tmp>'), end='')

    run('script', 'show.py', 'a', '-v')
    run('-c', '-c', show, 'a', '-v')
    run('-m', '-m', 'show', 'a', '-v')
    run('stdin', '-', 'a', '-v', stdin=show)
```

```output for=synopsis
script  ['show.py', 'a', '-v']
-c      ['-c', 'a', '-v']
-m      ['<tmp>/show.py', 'a', '-v']
stdin   ['-', 'a', '-v']
```

The same one-line program runs in four ways, each with the arguments `a -v`. The arguments after the program are the same each time; only `sys.argv[0]` changes. The `-v` is not taken by the interpreter, because it comes after the program.

## Semantics

**r1.** "When known to the interpreter, the script name and additional arguments thereafter are turned into a list of strings and assigned to the argv variable in the sys module."[^1] "The length of the list is at least one; when no script and no arguments are given, sys.argv[0] is an empty string."[^1]

**r2.** `sys.argv[0]` shows how the program was given. For a script it is "the script name as given on the command line".[^2] "When the script name is given as '-' (meaning standard input), sys.argv[0] is set to '-'. When -c command is used, sys.argv[0] is set to '-c'."[^1] For `-m`, "the first element of sys.argv will be the full path to the module file".[^2]

**r3.** "An interface option terminates the list of options consumed by the interpreter, all consecutive arguments will end up in sys.argv".[^2] The script name is one such option. "Options found after -c command or -m module are not consumed by the Python interpreter’s option processing but left in sys.argv for the command or module to handle."[^1] Options before it are for the interpreter: "Arguments consumed by the interpreter itself will be present in sys.orig_argv and missing from sys.argv."[^3]

**r4.** `-m` finds a module by import, not by file name: "the given module is located using the standard import mechanism and executed as a script."[^2] "Since the argument is a module name, you must not give a file extension (.py)."[^2] "When a package name is supplied instead of a normal module, the interpreter will execute <pkg>.__main__ as the main module."[^2]

**r5.** Each mode puts a directory at the start of `sys.path`. For a script file, "the directory containing that file is added to the start of sys.path".[^2] With `-c`, `-m` or `-`, "the current directory will be added to the start of sys.path".[^2]

**r6.** That first entry can be turned off. `-P` means "Don’t prepend the potentially unsafe path sys._path0 to sys.path", and `PYTHONSAFEPATH` does the same.[^2] `-I` runs "in isolated mode", which "also implies -E, -P and -s options".[^2]

## Examples

### Interpreter options and program options

```python run id=orig-argv
import subprocess
import sys

code = 'import sys; print(sys.argv); print(sys.orig_argv[1:])'
result = subprocess.run([sys.executable, '-X', 'dev', '-W', 'error', '-c', code, 'data.txt'],
                        capture_output=True, text=True)
print(result.stdout, end='')
```

```output for=orig-argv
['-c', 'data.txt']
['-X', 'dev', '-W', 'error', '-c', 'import sys; print(sys.argv); print(sys.orig_argv[1:])', 'data.txt']
```

`-X dev` and `-W error` come before `-c`, so the interpreter uses them and they are not in `sys.argv` (r3). `sys.orig_argv` keeps the full command line; its first element, the interpreter path, is left out here.

### A local file hides a standard module

```python run id=shadow
import pathlib
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'random.py').write_text('print("local random.py ran")\n')
    pathlib.Path(tmp, 'main.py').write_text(
        'import random\nprint("has randint:", hasattr(random, "randint"))\n')
    for opts in ([], ['-P'], ['-I']):
        result = subprocess.run([sys.executable, *opts, 'main.py'], cwd=tmp,
                                capture_output=True, text=True)
        print(opts, result.stdout.strip().replace('\n', ' / '))
```

```output for=shadow
[] local random.py ran / has randint: False
['-P'] has randint: True
['-I'] has randint: True
```

`main.py` meant the standard `random` module. Its own directory is first on `sys.path` (r5), so `import random` loads the `random.py` next to it. With `-P` or `-I` that directory is not added (r6), and the standard module is found.

### What -c puts first on sys.path

```python run id=path0
import subprocess
import sys

code = "import sys; print(repr(sys.path[0]) if sys.path[0] == '' else 'not the current directory')"
for opts in ([], ['-P']):
    result = subprocess.run([sys.executable, *opts, '-c', code], capture_output=True, text=True)
    print(opts, result.stdout.strip())
```

```output for=path0
[] ''
['-P'] not the current directory
```

For `-c` the current directory is the empty string at the front of `sys.path` (r5). `-P` leaves it out.

## Pitfalls

- An interpreter option written after the program is passed to the program. `python script.py -O` does not optimize; it gives the script the argument `-O` (r3).
- A file in the script's directory with the name of a standard module hides that module (see the shadowing example). Rename the file, or run with `-P` or `-I`.
- `python -m pkg.module.py` is wrong: `-m` takes a module name, not a file name (r4).
- `sys.argv[0]` is not always a usable path. It can be `'-c'` or `'-'` (r2), and for a script the sys docs say "it is operating system dependent whether this is a full pathname or not".[^3] To find the program's own file, use `__file__` in the script.

## Comparison

In Go, `os.Args` holds "the command-line arguments, starting with the program name", like `sys.argv`.[^4] `go run` splits its command line the way the interpreter does (r3). Its usage is `go run [build flags] [-exec xprog] package [arguments...]`: flags before the package are for the go command, and the arguments after it go to the program.[^5]

[^1]: Using the Python Interpreter, The Python Tutorial 3.15.
[^2]: Command line and environment, Python 3.15 documentation.
[^3]: sys, Python 3.15 documentation.
[^4]: Package os, Go standard library.
[^5]: Command go, Go documentation.
