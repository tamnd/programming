---
tag: HKDT1C
kind: feature
owner: python
slug: operating-system-interface
name: Operating System Interface
aliases: [os module, os.getcwd, os.chdir, os.system, shutil, working directory, Python os]
area: languages/python/io-and-os
level: intro
summary: >-
  The os module gives a portable interface to the operating system: the working directory, files, processes and the environment. The shutil module adds high-level file tasks such as copy and move.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/os.html", locator: "os module introduction"}
see_also: [python/reading-and-writing-files, python/file-wildcards, python/pathlib, python/files-and-directories, python/input-and-output]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib.html#operating-system-interface
    title: The Python Tutorial, 10. Brief tour of the standard library
    locator: 10.1. Operating system interface
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/os.html
    title: os, Python 3.15 documentation
    locator: module introduction; os.name; os.chdir(); os.getcwd(); os.listdir(); os.makedirs(); os.system()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/contextlib.html#contextlib.chdir
    title: contextlib, Python 3.15 documentation
    locator: contextlib.chdir()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/shutil.html
    title: shutil, Python 3.15 documentation
    locator: module introduction; shutil.copyfile(); shutil.copy(); shutil.move(); shutil.which()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/os
    title: Package os, Go standard library
    locator: Overview; func Getwd; func Chdir
    accessed: 2026-11-16
---

## Synopsis

```python run id=synopsis
import os
import shutil

os.makedirs('work/logs', exist_ok=True)
os.chdir('work')
print(os.path.basename(os.getcwd()))

with open('data.db', 'w', encoding='utf-8') as f:
    f.write('records\n')
print(shutil.copyfile('data.db', 'archive.db'))
print(shutil.move('archive.db', 'logs'))
print(sorted(os.listdir('.')), os.listdir('logs'))
```

```output for=synopsis
work
archive.db
logs/archive.db
['data.db', 'logs'] ['archive.db']
```

The program makes two directories, moves into the first one, copies a file and moves the copy into `logs`.

## Semantics

**r1.** "The os module provides dozens of functions for interacting with the operating system".[^1] It "provides a portable way of using operating system dependent functionality."[^2] "Extensions peculiar to a particular operating system are also available through the os module, but using them is of course a threat to portability."[^2]

**r2.** "All functions in this module raise OSError (or subclasses thereof) in the case of invalid or inaccessible file names and paths, or other arguments that have the correct type, but are not accepted by the operating system."[^2]

**r3.** `os.getcwd()` returns "a string representing the current working directory."[^2] `os.chdir(path)` changes "the current working directory to path."[^2] Relative paths in `open()` and the other functions start from this directory. `contextlib.chdir()` "changes the current working directory upon entering and restores the old one on exit."[^3] Example: [`chdir`](#chdir).

**r4.** `os.listdir()` returns "a list containing the names of the entries in the directory given by path. The list is in arbitrary order, and does not include the special entries '.' and '..'".[^2] `os.makedirs()` "makes all intermediate-level directories needed to contain the leaf directory."[^2] With the default `exist_ok=False`, "a FileExistsError is raised if the target directory already exists."[^2]

**r5.** `os.system(command)` runs "the command (a string) in a subshell."[^2] "On Unix, the return value is the exit status of the process encoded in the format specified for wait()."[^2] `os.waitstatus_to_exitcode()` turns it into an exit code.[^2] Example: [`system`](#system).

**r6.** "For daily file and directory management tasks, the shutil module provides a higher-level interface that is easier to use".[^1] `shutil.copyfile()` copies "the contents (no metadata)" of a file.[^4] `shutil.copy()` also copies the permission mode, and its target can be a directory.[^4] `shutil.move()` moves "a file or directory (src) to another location and return the destination."[^4]

**r7.** `os.name` is "The name of the operating system dependent module imported. The following names have currently been registered: 'posix', 'nt', 'java'."[^2]

**r8.** "The built-in dir() and help() functions are useful as interactive aids for working with large modules like os".[^1]

## Examples

```python run id=chdir
import contextlib
import os

os.makedirs('sub', exist_ok=True)
start = os.getcwd()
with contextlib.chdir('sub'):
    print(os.path.basename(os.getcwd()))
print(os.getcwd() == start)
```

```output for=chdir
sub
True
```

The `with` block works in `sub`, and the old directory comes back at the end of the block (rule r3).

```python run id=system
import os

print('before', flush=True)
status = os.system('echo from the shell; exit 3')
print(status, os.waitstatus_to_exitcode(status))
```

```output for=system
before
from the shell
768 3
```

The shell writes its output straight to standard output. On Unix, the status 768 holds the exit code 3 in its high byte, and `waitstatus_to_exitcode()` takes it out (rule r5). The `flush=True` keeps `before` ahead of the shell output.

```python run id=which
import shutil

print(shutil.which('sh') is not None)
print(shutil.which('no-such-command-here'))
```

```output for=which
True
None
```

`shutil.which()` returns "the path to an executable which would be run if the given cmd was called. If no cmd would be called, return None."[^4]

## Pitfalls

`from os import *` hides the built-in `open()`:

```python fails id=star-import expect=exception:TypeError
from os import *

f = open('notes.txt', 'w')
```

The tutorial says: "Be sure to use the import os style instead of from os import *. This will keep os.open() from shadowing the built-in open() function which operates much differently."[^1] Here `os.open()` wants integer flags, not the mode string.

Prefer `subprocess` to `os.system()`. The docs say: "The subprocess module provides more powerful facilities for spawning new processes and retrieving their results; using that module is recommended to using this function."[^2] `os.system()` passes the string to a shell, so a file name with spaces or quotes in it can change the command.

`os.chdir()` changes the whole process. `contextlib.chdir()` has the same limit: "As this changes a global state, the working directory, it is not suitable for use in most threaded or async contexts."[^3] Prefer full paths built from a base directory.

The copy functions lose some metadata. "Even the higher-level file copying functions (shutil.copy(), shutil.copy2()) cannot copy all file metadata."[^4]

## Comparison

"Package os provides a platform-independent interface to operating system functionality. The design is Unix-like, although the error handling is Go-like; failing calls return values of type error rather than error numbers."[^5] Go names the same calls `os.Getwd` and `os.Chdir`.[^5] Python raises OSError where Go returns an error value (rule r2).
