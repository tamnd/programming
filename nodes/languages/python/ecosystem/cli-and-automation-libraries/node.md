---
tag: 6W3T9V
kind: feature
owner: python
slug: cli-and-automation-libraries
name: CLI and automation libraries
aliases: [argparse, subprocess, shutil, Click, Typer, command-line interface, shell injection]
area: languages/python/ecosystem
level: core
summary: >-
  argparse parses the command line, subprocess runs other programs and shutil copies and removes files. Click and Typer are CLI libraries on PyPI. Pass arguments as a list, not through the shell.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/library/argparse.html", locator: "argparse, Parser for command-line options, arguments and subcommands"}
see_also: [python/the-python-package-index, python/http-and-networking-libraries]
sources:
  - url: https://docs.python.org/3/library/argparse.html
    title: argparse, Parser for command-line options, arguments and subcommands
    locator: introduction; Note
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/subprocess.html
    title: subprocess, Subprocess management
    locator: Using the subprocess Module; Security Considerations
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/shutil.html
    title: shutil, High-level file operations
    locator: introduction
    accessed: 2026-11-16
  - url: https://click.palletsprojects.com/en/stable/
    title: Welcome to Click, Click Documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://typer.tiangolo.com/
    title: Typer
    locator: introduction; Click code
    accessed: 2026-11-16
  - url: https://pkg.go.dev/flag
    title: Package flag
    locator: Overview
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os/exec
    title: Package exec
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=argparse
import argparse

parser = argparse.ArgumentParser(prog='resize')
parser.add_argument('files', nargs='+')
parser.add_argument('--width', type=int, default=800)
parser.add_argument('-v', '--verbose', action='store_true')

args = parser.parse_args(['a.png', 'b.png', '--width', '640', '-v'])
print(args.files, args.width, args.verbose)
args = parser.parse_args(['c.png'])
print(args.files, args.width, args.verbose)
```

```output for=argparse
['a.png', 'b.png'] 640 True
['c.png'] 800 False
```

`argparse` turns the command line into an object with one attribute for each argument. It converts the types, fills in the defaults and writes the help text. `subprocess` runs other programs, and `shutil` copies, moves and removes files. Click and Typer are libraries on PyPI for larger command-line programs.

## Semantics

**r1.** "The argparse module makes it easy to write user-friendly command-line interfaces. The program defines what arguments it requires, and argparse will figure out how to parse those out of sys.argv."[^1] "The argparse module also automatically generates help and usage messages."[^1] Example: [`argparse`](#argparse).

**r2.** "The module will also issue errors when users give the program invalid arguments."[^1] On an error, the parser calls `error`, and this method "prints a usage message, including the message, to sys.stderr and terminates the program with a status code of 2."[^1] Example: [`argparse-error`](#argparse-error).

**r3.** "While argparse is the default recommended standard library module for implementing basic command line applications, authors with more exacting requirements for exactly how their command line applications behave may find it doesn’t provide the necessary level of control."[^1]

**r4.** "The subprocess module allows you to spawn new processes, connect to their input/output/error pipes, and obtain their return codes."[^2] "The recommended approach to invoking subprocesses is to use the run() function for all use cases it can handle."[^2] Example: [`run`](#run).

**r5.** "Unlike some other popen functions, this library will not implicitly choose to call a system shell. This means that all characters, including shell metacharacters, can safely be passed to child processes."[^2] Example: [`no-shell`](#no-shell).

**r6.** "If the shell is invoked explicitly, via shell=True, it is the application’s responsibility to ensure that all whitespace and metacharacters are quoted appropriately to avoid shell injection vulnerabilities."[^2]

**r7.** "The shutil module offers a number of high-level operations on files and collections of files. In particular, functions are provided which support file copying and removal."[^3] Example: [`shutil`](#shutil).

**r8.** "Click is a Python package for creating beautiful command line interfaces in a composable way with as little code as necessary."[^4] It is the "Command Line Interface Creation Kit".[^4]

**r9.** "Typer is a library for building CLI applications that users will love using and developers will love creating. Based on Python type hints."[^5] "Typer used to depend on Click as well, a popular tool for building CLIs in Python."[^5] Newer versions include the source code of Click.[^5]

## Examples

An unknown option ends the program with the status 2 (rule r2):

```python run id=argparse-error
import argparse
import contextlib
import io

parser = argparse.ArgumentParser(prog='resize')
parser.add_argument('--width', type=int)

with contextlib.redirect_stderr(io.StringIO()):
    try:
        parser.parse_args(['--width', 'wide'])
    except SystemExit as e:
        print('exit status', e.code)
```

```output for=argparse-error
exit status 2
```

`subprocess.run` with `capture_output=True` and `text=True` returns the output as `str`. `check=True` raises `CalledProcessError` when the status is not 0 (rule r4):

```python run id=run
import subprocess
import sys

r = subprocess.run([sys.executable, '-c', 'print("hi from a child")'],
                   capture_output=True, text=True, timeout=30)
print(r.returncode, r.stdout.strip())

try:
    subprocess.run([sys.executable, '-c', 'raise SystemExit(3)'], check=True, timeout=30)
except subprocess.CalledProcessError as e:
    print('failed with status', e.returncode)
```

```output for=run
0 hi from a child
failed with status 3
```

A list of arguments goes to the program as it is. No shell reads the `;` (rule r5):

```python run id=no-shell
import subprocess
import sys

name = 'report.txt; echo injected'
r = subprocess.run([sys.executable, '-c', 'import sys; print(sys.argv[1:])', name],
                   capture_output=True, text=True, timeout=30)
print(r.stdout.strip())
```

```output for=no-shell
['report.txt; echo injected']
```

Copy a directory tree and remove it with `shutil` (rule r7):

```python run id=shutil
import pathlib
import shutil
import tempfile

with tempfile.TemporaryDirectory() as tmp:
    src = pathlib.Path(tmp, 'src')
    (src / 'sub').mkdir(parents=True)
    (src / 'a.txt').write_text('a')
    (src / 'sub' / 'b.txt').write_text('b')

    dst = pathlib.Path(tmp, 'dst')
    shutil.copytree(src, dst)
    print(sorted(p.relative_to(dst).as_posix() for p in dst.rglob('*')))
    shutil.rmtree(dst)
    print(dst.exists())
```

```output for=shutil
['a.txt', 'sub', 'sub/b.txt']
False
```

## Pitfalls

Do not build a shell command from user input with `shell=True` (rule r6). Pass a list of arguments, as in the example [`no-shell`](#no-shell).

`subprocess.run` does not raise an exception when the program fails, unless you give `check=True`. Without it, read `returncode` (example [`run`](#run)).

Give a `timeout` to `subprocess.run` when the program can stop answering. Each example on this page gives `timeout=30`.

`shutil.rmtree` does not ask before it deletes. It does this: "Delete an entire directory tree".[^3] Check the path before you call it.

`parse_args` without arguments reads `sys.argv`. In tests, give it a list, as in the example [`argparse`](#argparse).

## Comparison

"Package flag implements command-line flag parsing."[^6] "Package exec runs external commands."[^7] Like `subprocess`, "the os/exec package intentionally does not invoke the system shell and does not expand any glob patterns or handle other expansions, pipelines, or redirections typically done by shells."[^7] In both languages, a program gets a list of arguments and no shell reads them (rule r5).

[^1]: [argparse, Parser for command-line options, arguments and subcommands](https://docs.python.org/3/library/argparse.html)
[^2]: [subprocess, Subprocess management](https://docs.python.org/3/library/subprocess.html)
[^3]: [shutil, High-level file operations](https://docs.python.org/3/library/shutil.html)
[^4]: [Welcome to Click](https://click.palletsprojects.com/en/stable/)
[^5]: [Typer](https://typer.tiangolo.com/)
[^6]: [Package flag](https://pkg.go.dev/flag)
[^7]: [Package exec](https://pkg.go.dev/os/exec)
