---
tag: X5NG57
kind: feature
owner: python
slug: the-python-interpreter
name: Using the Python Interpreter
aliases: [python3, python command, py launcher, interactive mode, script mode, python -c, python -m, python -i, exit status, __main__.py]
area: languages/python/tooling
level: intro
summary: >-
  The python3 program reads Python source and runs it: from a prompt, a script file, standard input, a -c string or a module named with -m.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/interpreter.html", locator: "2.1. Invoking the Interpreter"}
see_also: [python/invoking-the-interpreter, python/the-interpreter-and-its-environment, python/development-mode-and-x-options, python/compiled-python-files, python/command-line-arguments]
sources:
  - url: https://docs.python.org/3.15/tutorial/interpreter.html
    title: Using the Python Interpreter, The Python Tutorial 3.15
    locator: 2.1. Invoking the Interpreter; 2.1.2. Interactive Mode
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: 1.1.1. Interface options; -i
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.exit
    accessed: 2026-11-16
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Compile and run Go program
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os
    title: Package os, Go standard library
    locator: func Exit
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import subprocess
import sys


def python(*args, stdin=None):
    result = subprocess.run([sys.executable, *args], input=stdin,
                            capture_output=True, text=True)
    return result.returncode, result.stdout


print(python('-c', 'print(6 * 7)'))
print(python('-', stdin='print("read from stdin")\n'))
print(python('-m', 'json.tool', '--compact', stdin='{"a": [1, 2]}'))
print(python('-c', 'raise SystemExit(3)'))
```

```output for=synopsis
(0, '42\n')
(0, 'read from stdin\n')
(0, '{"a":[1,2]}\n')
(3, '')
```

`sys.executable` is the interpreter that runs this example. It is started four times: with a `-c` string, with a script on standard input, with a standard module run as a program, and with code that exits with status 3. Each run returns its exit status and what it printed.

## Semantics

**r1.** The interpreter is a program, usually named with its version. The tutorial says it "is usually installed as /usr/local/bin/python3.15 on those machines where it is available", and that "other places are possible".[^1] "On Unix, the Python 3.x interpreter is by default not installed with the executable named python, so that it does not conflict with a simultaneously installed Python 2.x executable."[^1] On Windows, "If you have the py.exe launcher installed, you can use the py command."[^1]

**r2.** "The interpreter operates somewhat like the Unix shell: when called with standard input connected to a tty device, it reads and executes commands interactively; when called with a file name argument or with a file as standard input, it reads and executes a script from that file."[^1] A script can also be "a directory containing a __main__.py file, or a zipfile containing a __main__.py file".[^2]

**r3.** Two more ways take code from the command line. `python -c command` "executes the statement(s) in command, analogous to the shell’s -c option."[^1] `python -m module` "executes the source file for module as if you had spelled out its full name on the command line."[^1]

**r4.** "In non-interactive mode, the entire input is parsed before it is executed."[^2] A syntax error anywhere in a script stops the script before its first line runs.

**r5.** In interactive mode the interpreter prompts with the primary prompt `>>>` and, for continuation lines, the secondary prompt `...`.[^1] "Typing an end-of-file character (Control-D on Unix, Control-Z on Windows) at the primary prompt causes the interpreter to exit with a zero exit status."[^1] `quit()` also exits.[^1]

**r6.** `-i` runs a script, a `-c` string or a module and then enters interactive mode. "Interactive mode will start even when sys.stdin does not appear to be a terminal."[^2] "This can be useful to inspect global variables or a stack trace when a script raises an exception."[^2]

**r7.** A program ends with an exit status. `sys.exit` and `SystemExit` take "an integer giving the exit status (defaulting to zero)", and "zero is considered “successful termination” and any nonzero value is considered “abnormal termination” by shells and the like."[^3] "If another type of object is passed, None is equivalent to passing zero, and any other object is printed to stderr and results in an exit code of 1."[^3] Because the exit is an exception, "Cleanup actions specified by finally clauses of try statements are honored".[^3]

## Examples

### The whole script is parsed first

```python run id=parse-first
import subprocess
import sys

source = 'print("first line")\nprint("unclosed"\n'
result = subprocess.run([sys.executable, '-'], input=source,
                        capture_output=True, text=True)
print(result.returncode, repr(result.stdout))
print(result.stderr.splitlines()[-1])
```

```output for=parse-first
1 ''
SyntaxError: '(' was never closed
```

The first line is correct, but it does not run: the error on line 2 is found when the whole input is parsed (r4).

### A directory and a zip file as programs

```python run id=main-file
import pathlib
import subprocess
import sys
import tempfile
import zipfile

with tempfile.TemporaryDirectory() as tmp:
    app = pathlib.Path(tmp, 'app')
    app.mkdir()
    (app / '__main__.py').write_text('print("running the app directory")\n')
    with zipfile.ZipFile(pathlib.Path(tmp, 'app.zip'), 'w') as z:
        z.writestr('__main__.py', 'print("running the zip file")\n')
    for target in (app, pathlib.Path(tmp, 'app.zip')):
        result = subprocess.run([sys.executable, target], capture_output=True, text=True)
        print(result.stdout, end='')
```

```output for=main-file
running the app directory
running the zip file
```

The interpreter runs the `__main__.py` inside the directory or the archive (r2). This is how a program made of several files can be started with one path.

### Interactive mode after a command

```python run id=inspect
import subprocess
import sys

result = subprocess.run([sys.executable, '-i', '-c', 'x = 21'], input='x * 2\n',
                        capture_output=True, text=True)
print('stdout:', repr(result.stdout))
print('stderr:', repr(result.stderr))
```

```output for=inspect
stdout: '42\n'
stderr: '>>> >>> \n'
```

The `-c` code sets `x`, then `-i` starts interactive mode even though standard input is a pipe (r6). The expression typed at the prompt sees `x`. The prompts go to standard error, and the result goes to standard output.

### Exit statuses

```python run id=status
import subprocess
import sys

for code in ['pass', 'raise SystemExit(2)', 'raise SystemExit("bad input")', '1 / 0']:
    result = subprocess.run([sys.executable, '-c', code], capture_output=True, text=True)
    print(f'{code:30} {result.returncode} {result.stderr.splitlines()[-1:]}')
```

```output for=status
pass                           0 []
raise SystemExit(2)            2 []
raise SystemExit("bad input")  1 ['bad input']
1 / 0                          1 ['ZeroDivisionError: division by zero']
```

A string passed to `SystemExit` is printed to standard error and gives status 1 (r7). An exception that is not caught also gives status 1, after the traceback.

## Pitfalls

- The command `python` may not exist, or may start another version (r1). Use `python3`, the versioned name such as `python3.15`, or `py` on Windows, and check with `python3 --version`.
- A `-c` command usually contains spaces and quotes. The tutorial says "it is usually advised to quote command in its entirety."[^1]
- One syntax error stops a whole script, including the lines before it (r4). The interactive prompt is different: it runs each statement as it is entered.

## Comparison

Python reads and runs source in one step, and the same program also gives an interactive prompt (r2, r5). Go builds first: `go run` "compiles and runs the named main Go package".[^4] Python's `sys.exit(n)` corresponds to Go's `os.Exit(n)`: "Exit causes the current program to exit with the given status code."[^5] One difference: `sys.exit` raises an exception, so `finally` blocks still run (r7), while for `os.Exit` "The program terminates immediately; deferred functions are not run."[^5]

[^1]: Using the Python Interpreter, The Python Tutorial 3.15.
[^2]: Command line and environment, Python 3.15 documentation.
[^3]: sys, Python 3.15 documentation.
[^4]: Command go, Go documentation.
[^5]: Package os, Go standard library.
