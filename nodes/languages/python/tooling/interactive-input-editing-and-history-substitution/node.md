---
tag: WWNRWX
kind: feature
owner: python
slug: interactive-input-editing-and-history-substitution
name: Interactive Input Editing and History Substitution
aliases: [line editing, command history, GNU Readline, readline, .python_history, PYTHON_HISTORY, PYTHON_BASIC_REPL, .inputrc, editline, REPL history]
area: languages/python/tooling
level: core
summary: >-
  How the interactive interpreter edits the input line and keeps a history: the readline library, the .python_history file, the history list API, and how errors behave at the prompt.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/interactive.html", locator: "14. Interactive Input Editing and History Substitution"}
see_also: [python/interactive-mode, python/tab-completion-and-history-editing, python/the-interactive-startup-file, python/the-customization-modules, python/the-python-interpreter, python/standard-streams]
sources:
  - url: https://docs.python.org/3.15/tutorial/interactive.html
    title: Interactive Input Editing and History Substitution, The Python Tutorial 3.15
    locator: 14. Interactive Input Editing and History Substitution; 14.1. Tab Completion and History Editing
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/readline.html
    title: readline, Python 3.15 documentation
    locator: module overview; History file; History list
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/site.html
    title: site, Python 3.15 documentation
    locator: Readline configuration
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: PYTHON_BASIC_REPL; PYTHON_HISTORY
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/appendix.html
    title: Appendix, The Python Tutorial 3.15
    locator: 16.1. Interactive Mode; 16.1.1. Error Handling
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Go is a tool for managing Go source code (the command list); Compile and run Go program
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import os
import subprocess
import sys

session = 'x = 6 * 7\nx\n1/0\nprint("still here", x)\n'
env = dict(os.environ, PYTHON_BASIC_REPL='1')
for label, args in [('interactive', ['-i', '-q']), ('from a file', ['-'])]:
    result = subprocess.run([sys.executable, *args], input=session, env=env,
                            capture_output=True, text=True)
    print(f'--- {label}: exit code {result.returncode}')
    print('stdout:', result.stdout.splitlines())
    print('stderr:', *(line.rstrip() for line in result.stderr.splitlines()), sep='\n  ')
```

```output for=synopsis
--- interactive: exit code 0
stdout: ['42', 'still here 42']
stderr:
  >>> >>> >>> Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
  ZeroDivisionError: division by zero
  >>> >>>
--- from a file: exit code 1
stdout: []
stderr:
  Traceback (most recent call last):
    File "<stdin>", line 3, in <module>
  ZeroDivisionError: division by zero
```

The same four lines go to the interpreter twice. With `-i` it reads them as an interactive session: the bare `x` prints its value, the error is reported, and the session goes on to the last line. Read as a file with `-`, the expression `x` prints nothing and the error ends the program. The prompts `>>>` go to standard error, not standard output. Standard input here is a pipe, so no line editing takes place; the rest of the page is about what happens at a terminal.

## Semantics

**r1.** "Some versions of the Python interpreter support editing of the current input line and history substitution, similar to facilities found in the Korn shell and the GNU Bash shell. This is implemented using the GNU Readline library, which supports various styles of editing."[^1] The `readline` module is the interface to that library, and "Settings made using this module affect the behaviour of both the interpreter’s interactive prompt and the prompts offered by the built-in input() function."[^2]

**r2.** The library can be a different one: "The underlying Readline library API may be implemented by the editline ( libedit ) library instead of GNU readline."[^2] Key bindings live in a file: "Readline keybindings may be configured via an initialization file, typically .inputrc in your home directory."[^2] For editline on macOS, "the initialization file located in your home directory is named .editrc".[^2]

**r3.** History is kept between sessions. "The default configuration also saves your history into a file named .python_history in your user directory. The history will be available again during the next interactive interpreter session."[^1] The `site` module sets this up: "The default behavior is to enable tab completion and to use ~/.python_history as the history save file."[^3] `PYTHON_HISTORY` "can be used to set the location of a .python_history file".[^4]

**r4.** There are two prompts to edit in. "The classic basic interpreter is supported on all platforms with minimal line control capabilities."[^5] The default is now a newer interactive shell: "This one supports color, multiline editing, history browsing, and paste mode."[^5] With `PYTHON_BASIC_REPL` set, "the interpreter will not attempt to load the Python-based REPL that requires readline , and will instead use the traditional parser-based REPL".[^4] In the new shell, "F2 allows for browsing command-line history with neither output nor the >>> and … prompts."[^5]

**r5.** An error does not end an interactive session. "When an error occurs, the interpreter prints an error message and a stack trace. In interactive mode, it then returns to the primary prompt; when input came from a file, it exits with a nonzero exit status after printing the stack trace."[^5] "All error messages are written to the standard error stream; normal output from executed commands is written to standard output."[^5]

**r6.** "Typing the interrupt character (usually Control - C or Delete ) to the primary or secondary prompt cancels the input and returns to the primary prompt."[^5] "Typing an interrupt while a command is executing raises the KeyboardInterrupt exception, which may be handled by a try statement."[^5]

**r7.** A program can read and change the history list. Two numbering rules apply. For `get_history_item`, "The item index is one-based."[^2] For `remove_history_item` and `replace_history_item`, "The position is zero-based."[^2] `write_history_file` keeps only the newest lines: `set_history_length` sets "the desired number of lines to save in the history file", and "The write_history_file() function uses this value to truncate the history file".[^2]

## Examples

### The history list

```python run id=history
import os
import readline
import tempfile

readline.clear_history()
for line in ['x = 1', 'print(x)', 'x += 1']:
    readline.add_history(line)


def history():
    n = readline.get_current_history_length()
    return [readline.get_history_item(i) for i in range(1, n + 1)]


print(history())
print('item 1:', readline.get_history_item(1))
readline.remove_history_item(0)
readline.replace_history_item(0, 'print(x * 10)')
print(history())
with tempfile.TemporaryDirectory() as tmp:
    path = os.path.join(tmp, 'history')
    readline.add_history('x -= 1')
    readline.set_history_length(2)
    readline.write_history_file(path)
    with open(path) as f:
        print(f.read().splitlines())
    readline.clear_history()
    readline.read_history_file(path)
    print(history())
```

```output for=history
['x = 1', 'print(x)', 'x += 1']
item 1: x = 1
['print(x * 10)', 'x += 1']
['x += 1', 'x -= 1']
['x += 1', 'x -= 1']
```

`add_history` appends a line "as if it was the last line typed".[^2] Item 1 is the first line, but position 0 is also the first line (r7): `remove_history_item(0)` removes `x = 1`, and then `replace_history_item(0, ...)` replaces `print(x)`. The history then has three lines, and with a length of 2 only the newest two are written to the file. `read_history_file` loads them back. The readline docs show the same calls in a start-up example for `.python_history`, and say: "This code is actually automatically run when Python is run in interactive mode".[^2]

### Turning off the site setup

```python run id=hook
import subprocess
import sys

code = "import sys; print(hasattr(sys, '__interactivehook__'))"
for opts in ([], ['-S']):
    result = subprocess.run([sys.executable, *opts, '-c', code], capture_output=True, text=True)
    print(opts, result.stdout.strip())
```

```output for=hook
[] True
['-S'] False
```

The history file and tab completion are set up by a hook that the `site` module installs as `sys.__interactivehook__`. To turn them off, "delete (or override) the sys.__interactivehook__ attribute in your sitecustomize or usercustomize module or your PYTHONSTARTUP file".[^3] With `-S`, `site` is not imported and the hook is not there.

## Pitfalls

- Tests of the prompt through a pipe see the basic prompt with no editing. Code written for the readline prompt, such as key bindings, has no effect there (see the synopsis).
- The two library kinds use different files. "Also note that different libraries may use different history file formats. When switching the underlying library, existing history files may become unusable."[^2]
- `get_history_item` counts from 1, while `remove_history_item` and `replace_history_item` count from 0 (r7). Mixing them up changes the wrong line.
- With no limit set, the history file grows. The readline docs' own example sets `readline.set_history_length(1000)` because "default history len is -1 (infinite), which may grow unruly".[^2]
- An error at the prompt only returns to the prompt (r5). The same lines run from a file stop at the first error, so a session that worked line by line is not yet a working script.

## Comparison

Go has no interactive mode in its toolchain. The go command lists its commands as `bug`, `build`, `clean`, `doc`, `env`, `fix`, `fmt`, `generate`, `get`, `install`, `list`, `mod`, `run`, `telemetry`, `test`, `tool`, `version`, `vet` and `work`, and none of them reads statements one at a time.[^6] The nearest is `go run`, which will "compile and run Go program": the whole program, from files.[^6]

[^1]: Interactive Input Editing and History Substitution, The Python Tutorial 3.15.
[^5]: Appendix, The Python Tutorial 3.15.
[^2]: readline, Python 3.15 documentation.
[^4]: Command line and environment, Python 3.15 documentation.
[^3]: site, Python 3.15 documentation.
[^6]: Command go, Go documentation.
