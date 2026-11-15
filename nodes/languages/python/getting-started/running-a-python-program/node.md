---
tag: R4ZZJJ
kind: feature
owner: python
slug: running-a-python-program
name: Running a Python program
aliases: [python command, python -m, python -c, py launcher]
area: languages/python/getting-started
level: intro
summary: >-
  The python command runs a program from a file, a directory or zip file with __main__.py, a module name (-m), a string (-c) or standard input. The command name differs between Unix and Windows.
grammar: "python [-bBdEhiIOPqRsSuvVWx?] [-c command | -m module-name | script | - ] [args] (Python Setup and Usage, 1.1 Command line)"
spec: {standard: null, url: "https://docs.python.org/3.15/using/cmdline.html#command-line", locator: "1.1 Command line"}
see_also: [python/argument-passing, python/interactive-mode, python/hello-world-in-python]
sources:
  - url: https://docs.python.org/3.15/using/cmdline.html#command-line
    title: Python Setup and Usage, Python 3.15
    locator: 1.1 Command line
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#interface-options
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdarg-script
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options, script
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/__main__.html#main-py-in-python-packages
    title: __main__, Top-level code environment, Python 3.15 documentation
    locator: __main__.py in Python Packages
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-m
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options, -m
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-c
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options, -c
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/interpreter.html#invoking-the-interpreter
    title: The Python Tutorial, Python 3.15
    locator: 2.1 Invoking the Interpreter
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/json.html#module-json.tool
    title: json, Python 3.15 documentation
    locator: Command-line interface
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0394/
    title: "PEP 394: The python Command on Unix-Like Systems"
    locator: Recommendation, For Python runtime distributors
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/windows.html#basic-use
    title: Using Python on Windows, Python 3.15
    locator: 4.1.2 Basic use
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/windows.html#troubleshooting
    title: Using Python on Windows, Python 3.15
    locator: Troubleshooting
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/glossary.html#term-bytecode
    title: Glossary, Python 3.15 documentation
    locator: bytecode
    accessed: 2026-11-16
---

## Synopsis

```text
python3 script.py [args]        run a file
python3 folder [args]           run folder/__main__.py
python3 app.zip [args]          run __main__.py inside a zip file
python3 -m module [args]        find a module on sys.path and run it as __main__
python3 -c "statements" [args]  run the statements in the string
python3 - [args] < file         read the program from standard input
python3                         start the interactive interpreter
py script.py                    Windows: the same, through the Python install manager
```

The options of the interpreter come before the program. The first item that is not an option is the program, and the rest goes to the program in `sys.argv`.[^1] [^2]

## Semantics

**r1.** With a file name, the interpreter runs the file as the module `__main__`, and it puts the directory of the file at the start of `sys.path`. So the script can import the other modules in its own directory.[^3]

**r2.** With a directory or a zip file, the interpreter runs the file `__main__.py` inside it, and it puts the directory or the zip file at the start of `sys.path`.[^3] Example: [`run-forms`](#run-forms).

**r3.** `-m module` finds the module with the normal import system and runs it as `__main__`. The name has no `.py` extension. For a package, the interpreter runs `package.__main__`.[^4] The current directory goes at the start of `sys.path`.[^5] Example: [`run-forms`](#run-forms).

**r4.** `-c command` runs the statements in the string. The string can hold several statements separated by newlines. The current directory goes at the start of `sys.path`.[^6]

**r5.** `-` or no argument with input from a pipe makes the interpreter read the program from standard input. When standard input is a terminal and there is no program, the interpreter starts in interactive mode.[^2] [^7]

**r6.** "In non-interactive mode, the entire input is parsed before it is executed."[^2] A syntax error anywhere in a file stops the run before the first statement. Example: [`parse-first`](#parse-first).

**r7.** Many standard modules have a command line interface for `-m`. For example `python -m json.tool` reads JSON from standard input and prints it with indentation.[^8] [^5] Example: [`json-tool`](#json-tool).

**r8.** On Unix-like systems, PEP 394 expects the command `python3` to run a Python 3 interpreter. The command `python` may run Python 3, run Python 2, or be missing; the distributor decides.[^9]

**r9.** On Windows, the Python install manager provides `python` and `py`. The documentation recommends `py` when several versions are installed, and `py -V:3.15` selects a version. A `python3` command exists only to catch the Unix habit, and is "not meant to be widely used or recommended".[^10] When `python` opens the Microsoft Store, the troubleshooting table points to the "Manage app execution aliases" settings.[^11]

## Examples

The forms of rules r1 to r5, each in a child interpreter. The example writes one module `greet.py` and one directory `app` with a `__main__.py`, then runs them.

```python run id=run-forms
import os
import subprocess
import sys
import tempfile


def run(*args, stdin="", cwd=None):
    result = subprocess.run(
        [sys.executable, *args], input=stdin, capture_output=True, text=True, cwd=cwd
    )
    return result.stdout.strip()


with tempfile.TemporaryDirectory() as folder:
    with open(os.path.join(folder, "greet.py"), "w") as f:
        f.write("print('hello from', __name__)\n")
    os.mkdir(os.path.join(folder, "app"))
    with open(os.path.join(folder, "app", "__main__.py"), "w") as f:
        f.write("print('hello from the app folder')\n")

    print("file:  ", run("greet.py", cwd=folder))
    print("-m:    ", run("-m", "greet", cwd=folder))
    print("folder:", run("app", cwd=folder))
print("-c:    ", run("-c", "print('one'); print('two')"))
print("stdin: ", run("-", stdin="print(6 * 7)"))
```

```output for=run-forms
file:   hello from __main__
-m:     hello from __main__
folder: hello from the app folder
-c:     one
two
stdin:  42
```

A file with a syntax error on its last line prints nothing, because the interpreter parses the whole file first (rule r6).

```python run id=parse-first
import subprocess
import sys

program = "print('first line')\nprint('second line')\nprint('third line'\n"
result = subprocess.run([sys.executable, "-"], input=program, capture_output=True, text=True)
print("stdout:", repr(result.stdout))
print("status:", result.returncode)
print("error: ", result.stderr.strip().splitlines()[-1].split(":")[0])
```

```output for=parse-first
stdout: ''
status: 1
error:  SyntaxError
```

A standard module as a command (rule r7).

```python run id=json-tool
import subprocess
import sys

result = subprocess.run(
    [sys.executable, "-m", "json.tool"],
    input='{"name": "Ada", "langs": ["Python", "C"]}',
    capture_output=True,
    text=True,
)
print(result.stdout, end="")
```

```output for=json-tool
{
    "name": "Ada",
    "langs": [
        "Python",
        "C"
    ]
}
```

## Versions

The `json` module itself became runnable as `python -m json` in 3.14, as its "Changed in version 3.14" note says. On older versions only `python -m json.tool` works, which is why the example uses it.[^8]

## Pitfalls

### `-m` takes a module name, not a file name

`python -m greet.py` looks for a module named `py` inside a package `greet`, and fails. Write `python -m greet`.[^5]

```python run id=m-with-py
import os
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    with open(os.path.join(folder, "greet.py"), "w") as f:
        f.write("print('hello')\n")
    bad = subprocess.run([sys.executable, "-m", "greet.py"], capture_output=True, text=True, cwd=folder)
    good = subprocess.run([sys.executable, "-m", "greet"], capture_output=True, text=True, cwd=folder)
print("-m greet.py status:", bad.returncode)
print("-m greet output:   ", good.stdout.strip())
```

```output for=m-with-py
-m greet.py status: 1
-m greet output:    hello
```

### A file named like a standard module hides it

The directory of the script comes first on `sys.path` (rule r1). A file `json.py` next to a script makes `import json` in that script load the local file, not the standard module.[^3]

```python run id=shadow
import os
import subprocess
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    with open(os.path.join(folder, "json.py"), "w") as f:
        f.write("print('this is the local json.py')\n")
    with open(os.path.join(folder, "main.py"), "w") as f:
        f.write("import json\nprint(hasattr(json, 'dumps'))\n")
    result = subprocess.run([sys.executable, "main.py"], capture_output=True, text=True, cwd=folder)
print(result.stdout, end="")
```

```output for=shadow
this is the local json.py
False
```

### `python` can be Python 2, or nothing

A script that starts with `#!/usr/bin/env python` depends on what the system calls `python` (rule r8). PEP 394 says that this shebang is right for scripts that run only in an activated virtual environment, and that a script which starts the interpreter again should use `sys.executable` instead of a fixed command name.[^9] The examples on this page do that.

## Comparison

The run forms of other languages are on their own pages. In C, a separate build step makes a binary, and the binary runs. In CPython there is no separate step: `python3 script.py` compiles the source to bytecode and runs it, and it caches the bytecode of imported modules in `.pyc` files.[^12]

## Implementation notes

The options on this page are those of CPython. The setup documentation says that the command line schemes of other implementations "may differ".[^1]
