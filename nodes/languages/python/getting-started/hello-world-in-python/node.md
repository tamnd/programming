---
tag: VJNBVT
kind: feature
owner: python
slug: hello-world-in-python
name: Hello world in Python
aliases: [first Python program, print hello world in Python]
area: languages/python/getting-started
level: intro
summary: >-
  A Python hello world is one line, print("Hello, world!"), in a file that the interpreter runs from top to bottom. The program needs no main function, and it exits with status 0.
realizes: [hello-world-program]
grammar: "print(*objects, sep=' ', end='\\n', file=None, flush=False) (Built-in Functions, print)"
spec: {standard: null, url: "https://docs.python.org/3.15/library/functions.html#print", locator: "print()"}
see_also: [python/argument-passing, python/interactive-mode]
sources:
  - url: https://docs.python.org/3.15/library/functions.html#print
    title: Built-in Functions, Python 3.15 documentation
    locator: print()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/__main__.html
    title: __main__, Top-level code environment, Python 3.15 documentation
    locator: What is the top-level code environment?; __name__ == '__main__'
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/modules.html#executing-modules-as-scripts
    title: The Python Tutorial, Python 3.15
    locator: 6.1.1 Executing modules as scripts
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.exit
    title: sys, Python 3.15 documentation
    locator: sys.exit()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/exceptions.html#SystemExit
    title: Built-in Exceptions, Python 3.15 documentation
    locator: SystemExit
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/appendix.html#error-handling
    title: The Python Tutorial, Python 3.15
    locator: 16.1.1 Error Handling
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/appendix.html#executable-python-scripts
    title: The Python Tutorial, Python 3.15
    locator: 16.1.2 Executable Python Scripts
    accessed: 2026-11-16
  - url: https://docs.python.org/3.0/whatsnew/3.0.html#print-is-a-function
    title: What's New In Python 3.0
    locator: Print Is A Function
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3105/
    title: "PEP 3105: Make print a function"
    locator: Specification
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.stdout
    title: sys, Python 3.15 documentation
    locator: sys.stdin, sys.stdout, sys.stderr
    accessed: 2026-11-16
---

## Synopsis

```python run id=hello
print("Hello, world!")
```

```output for=hello
Hello, world!
```

Save the line in a file `hello.py` and run it with `python3 hello.py` (`py hello.py` on Windows). The file is the whole program.

The signature of `print`:[^1]

```text
print(*objects, sep=' ', end='\n', file=None, flush=False)
```

## Semantics

**r1.** The interpreter runs the statements of the file from top to bottom. The file is the "top-level code environment", and its module name `__name__` is `"__main__"`. There is no function `main` that the interpreter calls.[^2] [^3]

**r2.** `print()` converts each positional argument to a string, as `str()` does, and writes the strings to the stream `file`, separated by `sep` and followed by `end`. The defaults are one space and one newline. `sep`, `end`, `file` and `flush` must be keyword arguments.[^1] Example: [`print-args`](#print-args).

**r3.** When `file` is `None` or not given, `print()` writes to `sys.stdout`. With `file=sys.stderr` it writes to the standard error stream.[^1]

**r4.** When the last statement of the program ends, the interpreter exits with status 0. `sys.exit(n)` raises `SystemExit` and ends the program with the status `n`. `sys.exit()` with no argument or with `None` gives status 0. `sys.exit("text")` prints the text to `stderr` and gives status 1.[^4] [^5] Example: [`exit-status`](#exit-status).

**r5.** An exception that the program does not handle prints a traceback to standard error and ends a script "with a nonzero exit status".[^6] On CPython the status is 1. Example: [`exit-status`](#exit-status).

**r6.** On Unix, the file can run as a command when its first line is `#!/usr/bin/env python3` and it has the executable mode (`chmod +x hello.py`). The `#!` must be the first two characters of the file. For Python the line is a comment, so it changes nothing when the file runs with `python3 hello.py`.[^7] Example: [`hello-script`](#hello-script).

**r7.** On Windows, the installer associates the `.py` extension with `python.exe`, so a double-click runs the file. The extension `.pyw` runs it with no console window.[^7]

## Examples

`print()` with several arguments, and with `sep` and `end` (rule r2). A call with no arguments writes only `end`.

```python run id=print-args
print("Hello", "world")
print("Hello", "world", sep=", ", end="!\n")
print()
print("a", 1, 2.5, None, [3])
```

```output for=print-args
Hello world
Hello, world!

a 1 2.5 None [3]
```

A hello world as a script file, with the shebang line of rule r6 and the guard of rule r1. The guard lets another module import the file without printing.

```python run id=hello-script
#!/usr/bin/env python3
"""Print a greeting."""


def main():
    print("Hello, world!")


if __name__ == "__main__":
    main()
```

```output for=hello-script
Hello, world!
```

The exit status in four cases (rules r4 and r5). The example runs each program in a child interpreter and prints the status that the operating system sees.

```python run id=exit-status
import subprocess
import sys

programs = {
    "normal end": "print('hi')",
    "sys.exit(3)": "import sys; sys.exit(3)",
    "sys.exit('bad input')": "import sys; sys.exit('bad input')",
    "uncaught exception": "1 / 0",
}
for label, code in programs.items():
    result = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True)
    last_error_line = result.stderr.strip().splitlines()[-1:] or [""]
    print(f"{label:22} status {result.returncode}  stderr: {last_error_line[0]!r}")
```

```output for=exit-status
normal end             status 0  stderr: ''
sys.exit(3)            status 3  stderr: ''
sys.exit('bad input')  status 1  stderr: 'bad input'
uncaught exception     status 1  stderr: 'ZeroDivisionError: division by zero'
```

## Versions

Python 3.0 replaced the `print` statement of Python 2 with the `print()` function. The keyword arguments `sep`, `end` and `file` replace the special syntax of the old statement (PEP 3105).[^8] [^9] The keyword argument `flush` came in 3.3, as the "Changed in version 3.3" note of `print()` says.[^1]

## Pitfalls

### The Python 2 form is a syntax error

`print "Hello"` is the statement of Python 2. In Python 3 it is a `SyntaxError`, and the program does not start.[^8]

```python fails id=print-statement expect=exception:SyntaxError
print "Hello, world!"
```

### `print((a, b))` is not `print(a, b)`

Extra parentheses make one tuple argument, so `print()` shows the tuple. The release notes of Python 3.0 warn about this case: `print((x, y))` is "Not the same as print(x, y)!"[^8]

```python wrong id=print-tuple
x, y = 1, 2
print((x, y))
```

```output for=print-tuple
(1, 2)
```

### Output to a pipe can come late

When the program is interactive, `sys.stdout` is line-buffered, so each line shows at once. Otherwise it is block-buffered, and `sys.stderr` is line-buffered in both cases.[^10] So when the output goes to a pipe or a file, a message to standard error can appear before text that was printed earlier. `flush=True` writes the buffer at once, and the option `-u` turns the buffering off.[^1] [^10]

### A shebang with a Windows line ending fails

The tutorial says that "On some platforms, this first line must end with a Unix-style line ending". A file saved with Windows line endings (`\r\n`) can then fail to start as a command, while `python3 hello.py` still works.[^7]

## Comparison

The concept page [Hello, world](/hello-world-program/) shows the same program in other languages. Python is one of the languages where top-level statements are the program, with no required `main` function (rule r1). The `if __name__ == "__main__":` guard plays the role of a `main` function by convention.[^2]
