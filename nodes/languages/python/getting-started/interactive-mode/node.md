---
tag: 4T47BQ
kind: feature
owner: python
slug: interactive-mode
name: Interactive mode
aliases: [Python REPL, Python shell, interactive interpreter]
area: languages/python/getting-started
level: intro
summary: >-
  In interactive mode the interpreter reads one statement at a time from a terminal, runs it, and prints the value of each expression. It is the Python REPL.
grammar: "python [-i] [-q] (Python Setup and Usage, 1.1 Command line); the prompts are sys.ps1 and sys.ps2"
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/interpreter.html#interactive-mode", locator: "2.1.2 Interactive Mode"}
see_also: [python/argument-passing, interactive-debugging]
sources:
  - url: https://docs.python.org/3.15/tutorial/interpreter.html#interactive-mode
    title: The Python Tutorial, Python 3.15
    locator: 2.1.2 Interactive Mode
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/interpreter.html#invoking-the-interpreter
    title: The Python Tutorial, Python 3.15
    locator: 2.1 Invoking the Interpreter
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.displayhook
    title: sys, Python 3.15 documentation
    locator: sys.displayhook
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/introduction.html#numbers
    title: The Python Tutorial, Python 3.15
    locator: 3.1.1 Numbers
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.ps1
    title: sys, Python 3.15 documentation
    locator: sys.ps1, sys.ps2
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.flags
    title: sys, Python 3.15 documentation
    locator: sys.flags, flags.interactive
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/appendix.html#error-handling
    title: The Python Tutorial, Python 3.15
    locator: 16.1.1 Error Handling
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-i
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.3 Miscellaneous options, -i and -q
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#envvar-PYTHONSTARTUP
    title: Python Setup and Usage, Python 3.15
    locator: 1.2 Environment variables, PYTHONSTARTUP
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/constants.html#constants-added-by-the-site-module
    title: Built-in Constants, Python 3.15 documentation
    locator: Constants added by the site module
    accessed: 2026-11-16
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#a-better-interactive-interpreter
    title: What's New In Python 3.13
    locator: A better interactive interpreter
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/appendix.html#interactive-mode
    title: The Python Tutorial, Python 3.15
    locator: 16.1 Interactive Mode
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#envvar-PYTHON_BASIC_REPL
    title: Python Setup and Usage, Python 3.15
    locator: 1.2 Environment variables, PYTHON_BASIC_REPL
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/interactive.html
    title: The Python Tutorial, Python 3.15
    locator: 14.1 Tab Completion and History Editing; 14.2 Alternatives to the Interactive Interpreter
    accessed: 2026-11-16
---

## Synopsis

```text
$ python3                 start the REPL (the version banner, then the >>> prompt)
$ python3 -q              start it with no banner
$ python3 -i script.py    run the script, then stay in the REPL with its globals
>>> statement             the primary prompt, sys.ps1, default '>>> '
... continuation          the secondary prompt, sys.ps2, default '... '
>>> _                     the value of the last expression that was shown
```

The interpreter is in interactive mode when it reads its commands from a terminal (a tty).[^1] To leave it, type the end-of-file character (Control-D on Unix, Control-Z then Enter on Windows) or call `quit()`.[^2]

## Semantics

**r1.** When the interpreter reads commands from a tty, it is in interactive mode. It prints a banner with the version number and a copyright notice, then the primary prompt `>>> `. It shows the secondary prompt `... ` on the continuation lines of a compound statement, such as the body of an `if` or a `for`.[^1] Example: [`repl-prompts`](#repl-prompts).

**r2.** After each statement that is an expression, the interpreter calls `sys.displayhook` with the value. The default hook does nothing for `None`. For any other value it prints `repr(value)` and saves the value in `builtins._`.[^3] So the REPL echoes `2 + 3` as `5`, and it does not echo a call that returns `None`, such as `print()`. Example: [`repl-echo`](#repl-echo).

**r3.** In interactive mode, the interpreter assigns the last printed expression to the variable `_`. The tutorial uses it to "continue calculations" as with a desk calculator.[^4] The name `_` lives in the `builtins` module, so a global variable named `_` hides it (rule r2). Example: [`repl-underscore`](#repl-underscore).

**r4.** `sys.ps1` and `sys.ps2` hold the two prompts. "These are only defined if the interpreter is in interactive mode."[^5] A statement typed at the prompt can test `hasattr(sys, "ps1")`. The attribute does not exist yet while the script of `python -i script.py` runs, because the interactive loop sets it when it starts; `sys.flags.interactive` tells if `-i` was given.[^6] When a prompt is not a string, the interpreter calls `str()` on it before each new command, so an object can make a dynamic prompt.[^5] Example: [`repl-detect`](#repl-detect).

**r5.** When a statement raises an exception that is not handled, the interpreter prints the traceback and returns to the primary prompt. The session goes on. When the input came from a file, the interpreter exits with a nonzero status instead.[^7] Example: [`repl-error`](#repl-error).

**r6.** The interrupt character (usually Control-C) at a prompt cancels the input line and returns to the primary prompt. While a command runs, it raises `KeyboardInterrupt`.[^7]

**r7.** The option `-i` enters interactive mode after the script, the `-c` command or the `-m` module has run, and it does so even when standard input is not a terminal. The option `-q` hides the banner.[^8]

**r8.** If the environment variable `PYTHONSTARTUP` names a readable file, the interpreter runs that file before the first prompt, in the namespace of the session. It runs only for an interactive start, not for `-i`.[^9] [^8]

**r9.** `quit` and `exit` are objects that the `site` module adds. Calling either one raises `SystemExit`. When printed, they show a message like "Use quit() or Ctrl-D (i.e. EOF) to exit".[^10] `help()` starts the interactive help system.

## Examples

The examples below start a child interpreter with `-i -q` and feed it lines through a pipe, so the run is the same each time. A person at a terminal sees the same results, with the prompts in between.

The REPL echoes the value of each expression, and `_` holds the last one (rules r2 and r3).

```python run id=repl-echo
import subprocess
import sys

session = """\
2 + 3
x = 10
x * 4
_ + 1
None
print('hi')
"""
result = subprocess.run(
    [sys.executable, "-i", "-q"], input=session, capture_output=True, text=True
)
print(result.stdout, end="")
```

```output for=repl-echo
5
40
41
hi
```

The assignment `x = 10` shows nothing, because it is not an expression. `None` shows nothing (rule r2), and `print('hi')` shows its own output but not its return value, which is `None`.

The prompts go to standard error. A compound statement gets the secondary prompt until an empty line ends it (rule r1).

```python run id=repl-prompts
import subprocess
import sys

session = "for i in range(2):\n    print(i)\n\n"
result = subprocess.run(
    [sys.executable, "-i", "-q"], input=session, capture_output=True, text=True
)
print("stdout:", repr(result.stdout))
print("stderr:", repr(result.stderr))
```

```output for=repl-prompts
stdout: '0\n1\n'
stderr: '>>> ... ... >>> \n'
```

An error does not end the session (rule r5).

```python run id=repl-error
import subprocess
import sys

session = "1 / 0\nprint('still here')\n"
result = subprocess.run(
    [sys.executable, "-i", "-q"], input=session, capture_output=True, text=True
)
print(result.stdout, end="")
print(result.stderr.splitlines()[-2])
```

```output for=repl-error
still here
ZeroDivisionError: division by zero
```

A program can tell if it runs in interactive mode, because `sys.ps1` exists only there (rule r4).

```python run id=repl-detect
import subprocess
import sys

check = "import sys; print(hasattr(sys, 'ps1'), sys.flags.interactive)"


def run(args, stdin=""):
    result = subprocess.run([sys.executable, *args], input=stdin, capture_output=True, text=True)
    return result.stdout.strip()


print("python -c:     ", run(["-c", check]))
print("python -i -c:  ", run(["-i", "-q", "-c", check]))
print("at the prompt: ", run(["-i", "-q"], stdin=check + "\n"))
```

```output for=repl-detect
python -c:      False 0
python -i -c:   False 1
at the prompt:  True 1
```

With `-i`, the command runs before the interactive loop starts, so `sys.ps1` is not set yet, but `sys.flags.interactive` is already 1.

## Versions

> [!since v="3.13"]
> CPython has a second REPL, written in Python and based on code from PyPy. It starts by default when the REPL runs in a terminal. It adds multiline editing, colour, `help`, `exit` and `quit` without parentheses, help browsing with F1, history browsing with F2 and a paste mode with F3.[^11] [^12]

The environment variable `PYTHON_BASIC_REPL` turns the new REPL off and starts the parser-based one.[^13] With input from a pipe, as in the examples above, the session is not at a terminal, and the examples ran on CPython 3.12.

Tab completion and a history file (`.python_history` in the home directory) are on by default on systems with GNU Readline.[^14]

## Pitfalls

### A script does not echo its expressions

The echo of rule r2 happens only in interactive mode. In a script, an expression statement computes its value and drops it. Code copied from a REPL session must call `print()`.

```python wrong id=no-echo
total = 2 + 3
total
print("done")
```

```output for=no-echo
done
```

### Assigning to `_` stops the update

`_` is a name in `builtins`. After `_ = 'mine'`, the global `_` hides the built-in one, and the REPL updates only the hidden one (rule r3).

```python wrong id=repl-underscore
import subprocess
import sys

session = "_ = 'mine'\n1 + 1\n_\n"
result = subprocess.run(
    [sys.executable, "-i", "-q"], input=session, capture_output=True, text=True
)
print(result.stdout, end="")
```

```output for=repl-underscore
2
'mine'
```

`del _` removes the global name, and the REPL's `_` shows again.

### Piped input without `-i` is a script

Without `-i`, input from a pipe is read as a script, not as a session (rule r1). Nothing is echoed.

```python wrong id=pipe-no-i
import subprocess
import sys

result = subprocess.run([sys.executable, "-q"], input="2 + 3\n", capture_output=True, text=True)
print(repr(result.stdout))
```

```output for=pipe-no-i
''
```

### The basic REPL needs `exit()` with parentheses

In the parser-based REPL, a bare `exit` is an expression. The REPL prints the object, which shows a hint and does not exit (rule r9). The REPL of 3.13 and later accepts the bare word.[^12]

## Comparison

The tutorial names two enhanced interactive interpreters for Python: IPython, with "tab completion, object exploration and advanced history management", and bpython.[^14] The REPLs of other languages are on the page of each language.

## Implementation notes

CPython has two REPLs. The default one of 3.13 and later is written in Python and uses code from the PyPy project. The other one is the traditional parser-based REPL, and `PYTHON_BASIC_REPL` selects it.[^11] [^13]
