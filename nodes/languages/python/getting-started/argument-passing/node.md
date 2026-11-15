---
tag: J6KV2R
kind: feature
owner: python
slug: argument-passing
name: Argument passing
aliases: [sys.argv, command-line arguments in Python]
area: languages/python/getting-started
level: intro
summary: >-
  When Python runs a script, it puts the script name and the arguments after it into the list sys.argv, as strings. The first item depends on how the program was started.
grammar: "python [options] [-c command | -m module-name | script | -] [args] (Python Setup and Usage, 1.1 Command line)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/sys.html#sys.argv", locator: "sys.argv"}
see_also: [parse-command-line-arguments, hello-world-program]
sources:
  - url: https://docs.python.org/3.15/using/cmdline.html#interface-options
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/interpreter.html#argument-passing
    title: The Python Tutorial, Python 3.15
    locator: 2.1.1 Argument Passing
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.argv
    title: sys, Python 3.15 documentation
    locator: sys.argv
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-m
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options, -m and -
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.orig_argv
    title: sys, Python 3.15 documentation
    locator: sys.orig_argv
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/argparse.html#argparse.ArgumentParser.parse_args
    title: argparse, Python 3.15 documentation
    locator: The parse_args() method
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/getopt.html#getopt.getopt
    title: getopt, Python 3.15 documentation
    locator: getopt.getopt()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/argparse.html#argparse.ArgumentParser.error
    title: argparse, Python 3.15 documentation
    locator: ArgumentParser.error()
    accessed: 2026-11-16
  - url: https://en.cppreference.com/w/c/language/main_function
    title: cppreference.com, Main function (C)
    locator: Parameters, argv
    accessed: 2026-11-16
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html#jls-12.1.4
    title: The Java Language Specification, Java SE 25 Edition
    locator: 12.1.4 Invoke a main Method
    accessed: 2026-11-16
  - url: https://nodejs.org/api/process.html#processargv
    title: Node.js documentation, Process
    locator: process.argv
    accessed: 2026-11-16
---

## Synopsis

The command line of the interpreter has options, then one interface option or a script, then the arguments of the program.[^1]

```text
python [options] script.py  arg1 arg2 ...   ->  sys.argv == ['script.py', 'arg1', 'arg2', ...]
python [options] -c command arg1 ...        ->  sys.argv == ['-c', 'arg1', ...]
python [options] -m module  arg1 ...        ->  sys.argv == ['/full/path/to/module.py', 'arg1', ...]
python [options] -          arg1 ...        ->  sys.argv == ['-', 'arg1', ...]
python [options]                            ->  sys.argv == ['']
```

A program reads the list after `import sys`: `sys.argv[0]` is the program, and `sys.argv[1:]` holds the arguments.

## Semantics

**r1.** The interpreter turns the script name and the arguments after it into a list of strings, and assigns the list to `sys.argv`. The list always has at least one item.[^2] Example: [`argv-basic`](#argv-basic).

**r2.** `sys.argv[0]` is the script name. It is the empty string `''` when the interpreter gets no script and no arguments, `'-'` when the script is standard input, and `'-c'` for `-c` *command*.[^2] [^3] Example: [`argv-forms`](#argv-forms).

**r3.** With `-m` *module*, `sys.argv[0]` is the full path of the module file. While the interpreter looks for the module, the first item is `"-m"`.[^4] Example: [`argv-forms`](#argv-forms).

**r4.** Whether `sys.argv[0]` for a script is a full path or a relative one is "operating system dependent". It is the name that the user typed, not a normalized path.[^3]

**r5.** An interface option (`-c`, `-m`, `-` or a script name) ends the options of the interpreter. Every argument after it goes into `sys.argv`, even when it looks like an interpreter option.[^1] [^2] Example: [`argv-options`](#argv-options).

**r6.** Each item of `sys.argv` is a `str`. On Unix the operating system passes the arguments as bytes, and Python decodes them with the filesystem encoding and the "surrogateescape" error handler. The original bytes come back with `os.fsencode(arg)`.[^3]

**r7.** `sys.orig_argv` holds the full command line. The options that the interpreter uses itself are in `sys.orig_argv`, and they are not in `sys.argv`.[^5] Example: [`argv-options`](#argv-options).

**r8.** `sys.argv` is a normal list. A program can change it, and the change does not affect the interpreter. `ArgumentParser.parse_args()` of `argparse` takes its list of strings from `sys.argv` when it gets none.[^6] `getopt.getopt()` takes the list as a parameter, and its documentation names `sys.argv[1:]` as the typical value.[^7]

## Examples

A script that prints its own name and its arguments. The block runs as `python main.py alpha beta 42`.

```python run id=argv-basic args="alpha beta 42"
import os
import sys

print("program:", os.path.basename(sys.argv[0]))
print("count:", len(sys.argv) - 1)
for position, value in enumerate(sys.argv[1:], start=1):
    print(position, repr(value))
```

```output for=argv-basic
program: main.py
count: 3
1 'alpha'
2 'beta'
3 '42'
```

The argument `42` arrives as the string `'42'` (rule r6).

The first item for each way to start the interpreter. The example starts a child interpreter with `subprocess` for each form, and prints the `sys.argv` that the child sees. For `-m` it prints only the last part of the path, because the full path changes from one machine to the next.

```python run id=argv-forms
import os
import subprocess
import sys
import tempfile

SHOW = "import os, sys; print([os.path.basename(sys.argv[0])] + sys.argv[1:])"


def child(*args, stdin="", cwd=None):
    result = subprocess.run(
        [sys.executable, *args], input=stdin, capture_output=True, text=True, cwd=cwd
    )
    return result.stdout.strip()


with tempfile.TemporaryDirectory() as folder:
    with open(os.path.join(folder, "show.py"), "w") as f:
        f.write(SHOW + "\n")
    print("script: ", child("show.py", "a", "b", cwd=folder))
    print("-m:     ", child("-m", "show", "a", "b", cwd=folder))
print("-c:     ", child("-c", SHOW, "a", "b"))
print("stdin -:", child("-", "a", "b", stdin=SHOW))
print("no args:", child(stdin="import sys; print(sys.argv)"))
```

```output for=argv-forms
script:  ['show.py', 'a', 'b']
-m:      ['show.py', 'a', 'b']
-c:      ['-c', 'a', 'b']
stdin -: ['-', 'a', 'b']
no args: ['']
```

Options before the interface option belong to the interpreter. Options after it belong to the program. Here `-B` and `-O` are interpreter options, and `-O` after `-c` goes to the program.

```python run id=argv-options
import subprocess
import sys

code = "import sys; print(sys.argv); print(sys.orig_argv[1:])"
result = subprocess.run(
    [sys.executable, "-B", "-O", "-c", code, "-O", "--help"],
    capture_output=True,
    text=True,
)
print(result.stdout, end="")
```

```output for=argv-options
['-c', '-O', '--help']
['-B', '-O', '-c', 'import sys; print(sys.argv); print(sys.orig_argv[1:])', '-O', '--help']
```

A small command that adds its arguments. The arguments are strings, so the program converts them first.

```python run id=argv-sum args="3 4 5"
import sys

numbers = [int(arg) for arg in sys.argv[1:]]
print(sum(numbers))
```

```output for=argv-sum
12
```

## Versions

> [!since v="3.10"]
> The `sys` documentation gives 3.10 as the version that added `sys.orig_argv`. Earlier versions have only `sys.argv`.[^5]

The meaning of `sys.argv` for a script, for `-c` and for `-` has not changed in Python 3.[^2]

## Pitfalls

### The arguments are strings

`sum()` of the raw list fails, because `sys.argv[1:]` holds strings, not numbers (rule r6). Convert each item, as in [`argv-sum`](#argv-sum).

```python fails id=argv-strings expect=exception:TypeError args="3 4 5"
import sys

print(sum(sys.argv[1:]))
```

### A missing argument is an IndexError

`sys.argv[1]` fails when the user gives no argument. Check `len(sys.argv)` first, or use `argparse`, which prints a usage message.

```python fails id=argv-missing expect=exception:IndexError
import sys

name = sys.argv[1]
print("hello", name)
```

```python fails id=argv-missing-fix expect=exit:2
import sys

if len(sys.argv) < 2:
    print("usage: greet NAME", file=sys.stderr)
    sys.exit(2)
print("hello", sys.argv[1])
```

With no argument, the fixed program prints `usage: greet NAME` to standard error and exits with status 2. `ArgumentParser.error()` of `argparse` also ends the program "with a status code of 2".[^8]

### `sys.argv[0]` is not always a usable path

`sys.argv[0]` can be `''`, `'-'` or `'-c'` (rule r2), and for a script it can be relative to a directory that the program has since left (rule r4). To find the file of the running module, use `__file__`, and make it absolute with `os.path.abspath()` or `pathlib.Path.resolve()` early in the program.

## Comparison

**C** passes the arguments to `main(int argc, char *argv[])`. As in Python, `argv[0]` is the program name, and cppreference notes that it is an empty string "if the program name is not available from the host environment".[^9]

**Java** passes only the arguments after the class name to `main(String[] args)`. In the JLS example `java Test reboot Bob Dot Enzo`, the array holds the four strings, and the class name is not in it.[^10] So `args[0]` in Java is `sys.argv[1]` in Python.

**Node.js** puts two items before the arguments in `process.argv`: "The first element will be process.execPath", and the second is the path of the script.[^11] So the first argument is `process.argv[2]`.

## Implementation notes

The command line options of this page are those of CPython. The setup documentation says that "Other implementations' command line schemes may differ".[^1] The list `sys.argv` itself is part of the `sys` module, which the other implementations also provide.
