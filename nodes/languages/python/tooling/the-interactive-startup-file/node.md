---
tag: 4M134X
kind: feature
owner: python
slug: the-interactive-startup-file
name: The Interactive Startup File
aliases: [PYTHONSTARTUP, startup file, .pythonrc.py, sys.ps1, sys.ps2, interactive prompt, dynamic prompt, REPL startup]
area: languages/python/tooling
level: core
summary: >-
  PYTHONSTARTUP names a file of Python code that runs before the first interactive prompt, in the session's namespace. It is skipped for scripts, -i with a script, and -E.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/using/cmdline.html", locator: "PYTHONSTARTUP"}
see_also: [python/interactive-mode, python/interactive-input-editing-and-history-substitution, python/tab-completion-and-history-editing, python/the-customization-modules, python/the-interpreter-and-its-environment, python/eval-and-exec]
sources:
  - url: https://docs.python.org/3.15/using/cmdline.html
    title: Command line and environment, Python 3.15 documentation
    locator: PYTHONSTARTUP; -i; -E
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/appendix.html
    title: Appendix, The Python Tutorial 3.15
    locator: 16.1.3. The Interactive Startup File
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/sys.html
    title: sys, Python 3.15 documentation
    locator: sys.ps1, sys.ps2; sys.__interactivehook__
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/site.html
    title: site, Python 3.15 documentation
    locator: Readline configuration
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/readline.html
    title: readline, Python 3.15 documentation
    locator: Example
    accessed: 2026-11-18
  - url: https://pkg.go.dev/cmd/go
    title: Command go, Go documentation
    locator: Print Go environment information; Environment variables
    accessed: 2026-11-18
---

## Synopsis

```python run id=synopsis
import os
import pathlib
import subprocess
import sys
import tempfile

startup = '''\
import math
import sys
sys.ps1 = 'py> '
def sq(x):
    return x * x
print('startup file ran')
'''
with tempfile.TemporaryDirectory() as tmp:
    path = pathlib.Path(tmp, 'startup.py')
    path.write_text(startup)
    pathlib.Path(tmp, 'script.py').write_text("print('sq' in globals())\n")
    env = dict(os.environ, PYTHONSTARTUP=str(path), PYTHON_BASIC_REPL='1')
    runs = [('-i', ['-i', '-q'], 'sq(math.pi) > 9\n'),
            ('script', ['script.py'], None),
            ('-i script', ['-i', 'script.py'], ''),
            ('-E -i', ['-E', '-i', '-q'], "'sq' in globals()\n")]
    for label, args, stdin in runs:
        result = subprocess.run([sys.executable, *args], cwd=tmp, env=env, input=stdin,
                                capture_output=True, text=True)
        print(f'{label:10} stdout={result.stdout.splitlines()} stderr={result.stderr!r}')
```

```output for=synopsis
-i         stdout=['startup file ran', 'True'] stderr='py> py> \n'
script     stdout=['False'] stderr=''
-i script  stdout=['False'] stderr='>>> \n'
-E -i      stdout=['False'] stderr='>>> >>> \n'
```

The startup file imports `math`, defines `sq` and changes the prompt. In the first run, a plain interactive session, it runs first: the session can call `sq` and `math` without importing them, and the prompt is `py> ` (r1, r2). The other three runs do not read it. A script is not interactive (r3). `-i script.py` gives a prompt after the script, but "The PYTHONSTARTUP file is not read" (r3). `-E` ignores `PYTHONSTARTUP` like every other `PYTHON*` variable (r4).

## Semantics

**r1.** "If this is the name of a readable file, the Python commands in that file are executed before the first prompt is displayed in interactive mode."[^1] The tutorial compares it to the shell: "This is similar to the .profile feature of the Unix shells."[^2]

**r2.** The file shares the session's namespace. "The file is executed in the same namespace where interactive commands are executed so that objects defined or imported in it can be used without qualification in the interactive session. You can also change the prompts sys.ps1 and sys.ps2 and the hook sys.__interactivehook__ in this file."[^1]

**r3.** It is read only for a real interactive start. "This file is only read in interactive sessions, not when Python reads commands from a script, and not when /dev/tty is given as the explicit source of commands (which otherwise behaves like an interactive session)."[^2] For `-i`, which enters interactive mode after a script, `-c` or `-m`, the docs say: "The PYTHONSTARTUP file is not read."[^1]

**r4.** `-E` means "Ignore all PYTHON* environment variables, e.g. PYTHONPATH and PYTHONHOME , that might be set."[^1] So `-E`, and `-I`, which implies it, start without the startup file.

**r5.** The prompts are `sys.ps1` and `sys.ps2`. "Their initial values in this case are '>>> ' and '... ' . If a non-string object is assigned to either variable, its str() is re-evaluated each time the interpreter prepares to read a new interactive command; this can be used to implement a dynamic prompt."[^3]

**r6.** The interactive hook runs after the file. `sys.__interactivehook__` is called "when the interpreter is launched in interactive mode . This is done after the PYTHONSTARTUP file is read, so that you can set this hook there."[^3]

**r7.** There is one startup file, but it can load others. "If you want to read an additional start-up file from the current directory, you can program this in the global start-up file using code like if os.path.isfile('.pythonrc.py'): exec(open('.pythonrc.py').read()) ."[^2]

## Examples

### A dynamic prompt and the hook

```python run id=prompt
import os
import pathlib
import subprocess
import sys
import tempfile

startup = '''\
import sys

class Counter:
    def __init__(self):
        self.n = 0
    def __str__(self):
        self.n += 1
        return f'[{self.n}] '

sys.ps1 = Counter()
sys.__interactivehook__ = lambda: print('hook ran')
print('startup file ran')
'''
with tempfile.TemporaryDirectory() as tmp:
    path = pathlib.Path(tmp, 'startup.py')
    path.write_text(startup)
    env = dict(os.environ, PYTHONSTARTUP=str(path), PYTHON_BASIC_REPL='1')
    result = subprocess.run([sys.executable, '-i', '-q'], env=env, input='1 + 1\n2 + 2\n',
                            capture_output=True, text=True)
    print('stdout:', result.stdout.splitlines())
    print('stderr:', repr(result.stderr))
```

```output for=prompt
stdout: ['startup file ran', 'hook ran', '2', '4']
stderr: '[1] [2] [3] \n'
```

`sys.ps1` is an object, so its `str()` is taken again for each prompt and the number goes up (r5). The file replaces `sys.__interactivehook__`, and the new hook runs after the file (r6). Replacing the hook also removes the one `site` put there, which sets up history and tab completion. The `site` docs give this as the way to turn them off: "delete (or override) the sys.__interactivehook__ attribute in your sitecustomize or usercustomize module or your PYTHONSTARTUP file".[^4]

### Using the startup file from a script

```python run id=script
import os
import pathlib
import subprocess
import sys
import tempfile

script = '''\
import os
filename = os.environ.get('PYTHONSTARTUP')
if filename and os.path.isfile(filename):
    with open(filename) as fobj:
        startup_file = fobj.read()
    exec(startup_file)
print(sq(4))
'''
with tempfile.TemporaryDirectory() as tmp:
    pathlib.Path(tmp, 'startup.py').write_text('def sq(x):\n    return x * x\n')
    pathlib.Path(tmp, 'script.py').write_text(script)
    env = dict(os.environ, PYTHONSTARTUP=os.path.join(tmp, 'startup.py'))
    result = subprocess.run([sys.executable, 'script.py'], cwd=tmp, env=env,
                            capture_output=True, text=True)
    print(result.stdout, end='')
```

```output for=script
16
```

A script does not read the file (r3). The tutorial's advice is: "If you want to use the startup file in a script, you must do this explicitly in the script", with the code used here.[^2] The script then has `sq`, because `exec` ran the file in the script's globals.

## Pitfalls

- A startup file that works at the prompt does nothing for `python script.py` or `python -i script.py` (r3). Code that a program needs belongs in the program or in a module it imports.
- Assigning `sys.__interactivehook__` in the startup file replaces the `site` hook that loads history and completion (see the prompt example). To keep them, call the old hook from the new one.
- The readline docs suggest the startup file for history setup, with `atexit.register(readline.write_history_file, histfile)`.[^5] That example is the default behaviour already: "This code is actually automatically run when Python is run in interactive mode".[^5] A startup file needs it only to change that default.

## Comparison

Go has no prompt, so it has no file to run before one. Its closest feature is the go command's configuration file, which holds settings rather than code: "Defaults changed using 'go env -w' are recorded in a Go environment configuration file stored in the per-user configuration directory".[^6] Like `PYTHONSTARTUP`, it can be turned off from the environment: "Setting GOENV=off in the environment disables the use of the default configuration file."[^6]

[^2]: Appendix, The Python Tutorial 3.15.
[^1]: Command line and environment, Python 3.15 documentation.
[^3]: sys, Python 3.15 documentation.
[^5]: readline, Python 3.15 documentation.
[^6]: Command go, Go documentation.
[^4]: site, Python 3.15 documentation.
