---
tag: ND5GJQ
kind: feature
owner: python
slug: the-pdb-debugger
name: The pdb Debugger
aliases: [pdb, "python -m pdb", breakpoint(), pdb.set_trace, pdb.post_mortem, pdb.pm, PYTHONBREAKPOINT, sys.breakpointhook, post-mortem debugging, .pdbrc, debugger commands]
area: languages/python/tooling
level: core
summary: >-
  pdb is the standard Python debugger: breakpoints, stepping, stack frames and evaluating code in a frame. It starts with python -m pdb, from breakpoint(), or after an exception.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/pdb.html", locator: "Debugger commands"}
see_also: [interactive-debugging, python/tracing-and-monitoring, python/tracebacks-and-exception-objects, python/interactive-mode, python/development-mode-and-x-options, python/code-and-frame-objects]
sources:
  - url: https://docs.python.org/3.15/library/pdb.html
    title: pdb, Python 3.15 documentation
    locator: module overview; Command-line interface; pdb.post_mortem; Debugger commands
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/functions.html
    title: Built-in Functions, Python 3.15 documentation
    locator: breakpoint()
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: PYTHONBREAKPOINT
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.breakpointhook
    accessed: 2026-11-18
  - url: https://go.dev/doc/gdb
    title: Debugging Go Code with GDB, The Go Programming Language
    locator: Introduction
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile

script = '''\
def total(prices, rate):
    subtotal = sum(prices)
    tax = subtotal * rate
    return subtotal + tax


print(total([10, 20], 0.5))
'''
commands = ['break 3', 'continue', 'p subtotal, rate', 'next', 'p tax', 'return',
            'continue', 'quit']
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'shop.py').write_text(script)
    env = dict(os.environ, HOME=tmp)
    result = subprocess.run([sys.executable, '-m', 'pdb', 'shop.py'], cwd=tmp, env=env,
                            input='\n'.join(commands) + '\n', capture_output=True, text=True)
    print(result.stdout.replace(os.path.realpath(tmp), '<tmp>').rstrip())
```

```output for=synopsis
> <tmp>/shop.py(1)<module>()
-> def total(prices, rate):
(Pdb) Breakpoint 1 at <tmp>/shop.py:3
(Pdb) > <tmp>/shop.py(3)total()
-> tax = subtotal * rate
(Pdb) (30, 0.5)
(Pdb) > <tmp>/shop.py(4)total()
-> return subtotal + tax
(Pdb) 15.0
(Pdb) --Return--
> <tmp>/shop.py(4)total()->45.0
-> return subtotal + tax
(Pdb) 45.0
The program finished and will be restarted
> <tmp>/shop.py(1)<module>()
-> def total(prices, rate):
(Pdb)
```

`python -m pdb shop.py` stops before the first line (r2). The commands, which a person would type at the `(Pdb)` prompt, come from a pipe here, so each answer follows its prompt on the same line. `break 3` sets a breakpoint, `continue` runs to it, and `p` prints values in the stopped frame. `next` runs one line, `return` runs to the end of the function and shows the value it returns, and the last `continue` lets the program print `45.0`. pdb then starts the program again (r2), and `quit` ends it. `HOME` points at the temporary directory so that no personal `.pdbrc` file changes the run (r7).

## Semantics

**r1.** "The module pdb defines an interactive source code debugger for Python programs. It supports setting (conditional) breakpoints and single stepping at the source line level, inspection of stack frames, source code listing, and evaluation of arbitrary Python code in the context of any stack frame. It also supports post-mortem debugging and can be called under program control."[^1]

**r2.** From the command line, `python -m pdb` runs a script or, with `-m`, a module; "the debugger will pause execution just before the first line of the module."[^1] "When invoked as a module, pdb will automatically enter post-mortem debugging if the program being debugged exits abnormally. After post-mortem debugging (or after normal exit of the program), pdb will restart the program."[^1]

**r3.** In the program, the docs give `import pdb; pdb.set_trace()` or `breakpoint()` as "The typical usage to break into the debugger", inserted "at the location you want to break into the debugger".[^1] `breakpoint()` "calls sys.breakpointhook() , passing args and kws straight through. By default, sys.breakpointhook() calls pdb.set_trace() expecting no arguments."[^2]

**r4.** `PYTHONBREAKPOINT` chooses what `breakpoint()` does. "If this is set, it names a callable using dotted-path notation."[^3] "If not set, or set to the empty string, it is equivalent to the value “pdb.set_trace”. Setting this to the string “0” causes the default implementation of sys.breakpointhook() to do nothing but return immediately."[^3] `sys.breakpointhook` "can be set to any other function so that you can choose which debugger gets used."[^4]

**r5.** The main commands move through the program. `step` stops "either in a function that is called or on the next line in the current function"; `next` "executes called functions at (nearly) full speed, only stopping at the next line in the current function"; `return` will "Continue execution until the current function returns"; `continue` will "Continue execution, only stop when a breakpoint is encountered."[^1] `where` will "Print a stack trace, with the most recent frame at the bottom", and `up` and `down` move between frames.[^1] "Most commands can be abbreviated to one or two letters".[^1]

**r6.** `break` takes a line, a `filename:lineno` or a function. "If a second argument is present, it is an expression which must evaluate to true before the breakpoint is honored."[^1] "Commands that the debugger doesn’t recognize are assumed to be Python statements and are executed in the context of the program being debugged. Python statements can also be prefixed with an exclamation point ( ! )."[^1] "Entering a blank line repeats the last command entered."[^1]

**r7.** "If a file .pdbrc exists in the user’s home directory or in the current directory, it is read with 'utf-8' encoding and executed as if it had been typed at the debugger prompt".[^1] `pdb.post_mortem(t)` will "Enter post-mortem debugging of the given exception or traceback object"; `pdb.pm()` does it for "the exception found in sys.last_exc".[^1]

## Examples

### After an exception

```python run id=post-mortem
import os
import pathlib
import subprocess
import sys
import tempfile

script = '''\
import pdb


def average(xs):
    return sum(xs) / len(xs)


def report(groups):
    return [average(g) for g in groups]


try:
    report([[1, 2], [], [3]])
except ZeroDivisionError as exc:
    pdb.post_mortem(exc.__traceback__)
'''
commands = ['p xs', 'up', 'p groups', 'quit']
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'stats.py').write_text(script)
    env = dict(os.environ, HOME=tmp)
    result = subprocess.run([sys.executable, 'stats.py'], cwd=tmp, env=env,
                            input='\n'.join(commands) + '\n', capture_output=True, text=True)
    print(result.stdout.replace(os.path.realpath(tmp), '<tmp>').rstrip())
```

```output for=post-mortem
> <tmp>/stats.py(5)average()
-> return sum(xs) / len(xs)
(Pdb) []
(Pdb) > <tmp>/stats.py(9)report()
-> return [average(g) for g in groups]
(Pdb) [[1, 2], [], [3]]
(Pdb)
```

`post_mortem` opens the debugger in the frame where the exception was raised (r7). The local `xs` is the empty list that caused the division by zero. `up` moves to the caller, where `groups` shows which input it came from. Nothing ran again: the frames are the ones left by the exception.

### breakpoint() and PYTHONBREAKPOINT

```python run id=breakpoint
import os
import subprocess
import sys

code = "x = 1\nbreakpoint()\nprint('after', x)\n"
for value in (None, '0', 'builtins.print'):
    env = {k: v for k, v in os.environ.items() if k != 'PYTHONBREAKPOINT'}
    if value is not None:
        env['PYTHONBREAKPOINT'] = value
    result = subprocess.run([sys.executable, '-c', code], env=env, input='p x\n!x = 5\ncontinue\n',
                            capture_output=True, text=True)
    print(f'{value!s:15}', result.stdout.splitlines())
```

```output for=breakpoint
None            ['> <string>(3)<module>()', '(Pdb) 1', '(Pdb) (Pdb) after 5']
0               ['after 1']
builtins.print  ['', 'after 1']
```

With `PYTHONBREAKPOINT` unset, `breakpoint()` starts pdb (r3). `p x` prints 1, and `!x = 5` is a Python statement run in the program's frame (r6), so the program goes on to print `after 5`. With `0`, `breakpoint()` does nothing (r4). With `builtins.print`, `breakpoint()` calls `print()`, which prints an empty line. The same program needs no edit to turn its breakpoints off.

### A conditional breakpoint

```python run id=condition
import os
import pathlib
import subprocess
import sys
import tempfile

script = '''\
total = 0
for i in range(5):
    total += i * i
print(total)
'''
commands = ['break 3, i == 3', 'continue', 'p i, total', 'clear 1', 'continue', 'quit']
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'loop.py').write_text(script)
    env = dict(os.environ, HOME=tmp)
    result = subprocess.run([sys.executable, '-m', 'pdb', 'loop.py'], cwd=tmp, env=env,
                            input='\n'.join(commands) + '\n', capture_output=True, text=True)
    print(result.stdout.replace(os.path.realpath(tmp), '<tmp>').rstrip())
```

```output for=condition
> <tmp>/loop.py(1)<module>()
-> total = 0
(Pdb) Breakpoint 1 at <tmp>/loop.py:3
(Pdb) > <tmp>/loop.py(3)<module>()
-> total += i * i
(Pdb) (3, 5)
(Pdb) Deleted breakpoint 1 at <tmp>/loop.py:3
(Pdb) 30
The program finished and will be restarted
> <tmp>/loop.py(1)<module>()
-> total = 0
(Pdb)
```

Line 3 runs five times, but the breakpoint stops only when `i == 3` (r6). At that point `total` is 0 + 1 + 4 = 5. `clear 1` removes breakpoint 1, and the program runs to the end.

## Pitfalls

- A `breakpoint()` left in code stops the program wherever it runs, including in production or in a test run. Remove it, or set `PYTHONBREAKPOINT=0` (r4).
- A one-letter variable can be taken as a command: with a variable named `n` or `c`, typing it runs `next` or `continue` (r5). Use `p n`, or the `!` prefix (r6).
- `python -m pdb` restarts the program when it ends (r2). Use `quit` to leave.
- A `.pdbrc` in the home or current directory runs its commands at start (r7). The examples point `HOME` at the temporary directory to avoid that.

## Comparison

For Go, the Go project's GDB guide points to a separate debugger: "Note that Delve is a better alternative to GDB when debugging Go programs built with the standard toolchain. It understands the Go runtime, data structures, and expressions better than GDB."[^5] Of GDB itself it says it "is not a reliable debugger for Go programs, particularly heavily concurrent ones."[^5] pdb, by contrast, is a module of the standard library, and it evaluates Python code in any frame of the running program (r1, r6).

[^1]: pdb, Python 3.15 documentation.
[^2]: Built-in Functions, Python 3.15 documentation.
[^3]: Command line and environment, Python 3.15 documentation.
[^4]: sys, Python 3.15 documentation.
[^5]: Debugging Go Code with GDB, The Go Programming Language.
