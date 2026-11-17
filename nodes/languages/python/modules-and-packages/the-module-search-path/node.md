---
tag: Y54T1E
kind: feature
owner: python
slug: the-module-search-path
name: The module search path
aliases: [sys.path, PYTHONPATH, site-packages, module shadowing, sys.builtin_module_names, PYTHONSAFEPATH, -P option]
area: languages/python/modules-and-packages
level: core
summary: >-
  An import looks first for a built-in module and then in the directories of sys.path, in order. The script directory comes first, so a local file can hide a standard module.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/sys_path_init.html", locator: "The initialization of the sys.path module search path"}
see_also: [python/modules, python/the-import-statement, python/standard-modules, python/executing-modules-as-scripts, python/environment-variables-from-python, python/project-layout]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html#the-module-search-path
  title: The Python Tutorial, 6. Modules
  locator: 6.1.2. The Module Search Path
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/sys.html#sys.path
  title: sys, System-specific parameters and functions
  locator: sys.path; sys.builtin_module_names; sys.stdlib_module_names
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/using/cmdline.html#envvar-PYTHONPATH
  title: 1. Command line and environment
  locator: 1.2. Environment variables, PYTHONPATH
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/sys_path_init.html
  title: The initialization of the sys.path module search path
  locator: the whole page
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/whatsnew/3.13.html#improved-error-messages
  title: What’s New In Python 3.13
  locator: Improved error messages
  accessed: 2026-11-16
- url: https://go.dev/ref/spec#Import_declarations
  title: The Go Programming Language Specification
  locator: Import declarations
  accessed: 2026-11-16
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
  title: import, JavaScript reference, MDN
  locator: Module specifier resolution
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/site.html
  title: site, Site-specific configuration hook
  locator: the module description
  accessed: 2026-11-16
---

## Synopsis

```text
import spam
# 1. a built-in module named spam (sys.builtin_module_names)
# 2. spam.py, a spam/ package and so on, in each directory of sys.path, in order

sys.path = [
    script directory,       # or the current directory for -c, -m and the REPL
    PYTHONPATH entries,
    standard library directories,
    site-packages directories,   # added by the site module
]

$ PYTHONPATH=lib python main.py    # add lib before the standard library
$ python -P main.py                # do not add the script directory
```

## Semantics

**r1.** "When a module named spam is imported, the interpreter first searches for a built-in module with that name. These module names are listed in sys.builtin_module_names. If not found, it then searches for a file named spam.py in a list of directories given by the variable sys.path."[^1] Example: [`builtin-first`](#builtin-first).

**r2.** `sys.path` starts with one directory that depends on the command. For `python script.py`, Python will "prepend the script’s directory".[^2] For `python -m module`, it will "prepend the current working directory".[^2] For `python -c code` and the REPL, it will "prepend an empty string, which means the current working directory."[^2] Example: [`path0`](#path0).

**r3.** The `PYTHONPATH` environment variable adds directories after that first entry. Its format "is the same as the shell’s PATH: one or more directory pathnames separated by os.pathsep (e.g. colons on Unix or semicolons on Windows). Non-existent directories are silently ignored."[^3] Example: [`pythonpath`](#pythonpath).

**r4.** Next come the directories of the standard library: "The next items added are the directories containing standard Python modules as well as any extension modules that these modules depend on."[^4] "Finally, the site module is processed and site-packages directories are added to the module search path."[^4] Example: [`path0`](#path0).

**r5.** "After initialization, Python programs can modify sys.path."[^1] The sys documentation says: "A program is free to modify this list for its own purposes. Only strings should be added to sys.path; all other data types are ignored during import."[^2] Example: [`append`](#append).

**r6.** The first match wins, so a file in the script directory hides a library module of the same name. "This means that scripts in that directory will be loaded instead of modules of the same name in the library directory. This is an error unless the replacement is intended."[^1] Example: [`shadow`](#shadow).

**r7.** The option `-P` and the variable `PYTHONSAFEPATH` stop the first entry of rule r2: "To not prepend this potentially unsafe path, use the -P command line option or the PYTHONSAFEPATH environment variable."[^2] Example: [`safe-path`](#safe-path).

## Examples

The first entry of `sys.path` is the directory of the script, and the standard library comes later (rules r2 and r4).

```python run id=path0
import os
import sys

here = os.path.dirname(os.path.abspath(__file__))
print(sys.path[0] == here)
stdlib = os.path.dirname(os.__file__)
print(sys.path.index(stdlib) > 0)
```

```output for=path0
True
True
```

`tools.py` is in the directory `lib`, which is not on the path. A child process finds it only when `PYTHONPATH` names the directory (rule r3).

```python norun reason=fragment file=lib/tools.py group=pythonpath
NAME = "tools from lib"
```

```python run file=main.py group=pythonpath id=pythonpath
import os
import subprocess
import sys

code = "import tools; print(tools.NAME)"
env = dict(os.environ)
env.pop("PYTHONPATH", None)
for value in (None, "lib"):
    if value:
        env["PYTHONPATH"] = value
    child = subprocess.run([sys.executable, "-c", code], env=env, capture_output=True, text=True)
    result = child.stdout or child.stderr.splitlines()[-1]
    print(value, child.returncode, result.strip())
```

```output for=pythonpath
None 1 ModuleNotFoundError: No module named 'tools'
lib 0 tools from lib
```

A program can add a directory to `sys.path` before the import (rule r5).

```python norun reason=fragment file=lib/tools.py group=append
NAME = "tools from lib"
```

```python run file=main.py group=append id=append
import os
import sys

sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), "lib"))
import tools

print(tools.NAME)
```

```output for=append
tools from lib
```

On the Linux builds that run these examples, `math` is a built-in module. A file `math.py` in the script directory does not hide it, because built-in modules come before `sys.path` (rule r1).

```python norun reason=fragment file=math.py group=builtin-first
print("the local math.py runs")
```

```python run file=main.py group=builtin-first id=builtin-first
import sys

import math

print("math" in sys.builtin_module_names)
print(math.sqrt(9))
```

```output for=builtin-first
True
3.0
```

With `-P`, the directory of the script is not on the path, so the import of a module next to the script fails (rule r7).

```python norun reason=fragment file=helper.py group=safe-path
VALUE = 42
```

```python norun reason=fragment file=app.py group=safe-path
import helper

print(helper.VALUE)
```

```python run file=main.py group=safe-path id=safe-path
import subprocess
import sys

for flags in ([], ["-P"]):
    child = subprocess.run([sys.executable, *flags, "app.py"], capture_output=True, text=True)
    result = child.stdout or child.stderr.splitlines()[-1]
    print(flags, result.strip())
```

```output for=safe-path
[] 42
['-P'] ModuleNotFoundError: No module named 'helper'
```

## Versions

The Python 3.13 release notes say: "A common mistake is to write a script with the same name as a standard library module. When this results in errors, we now display a more helpful error message".[^5] In [`shadow`](#shadow), CPython 3.12.3 ends the message after `module 'random' has no attribute 'randint'`. CPython 3.13.14 and 3.14.6 add `(consider renaming '…/random.py' since it has the same name as the standard library module named 'random' and prevents importing that standard library module)`. The other examples give the same output on the three versions.

## Pitfalls

### A local file with the name of a library module

A file `random.py` next to the script hides the module `random` of the standard library (rule r6). The import succeeds, but it loads the wrong file, and the error comes later.

```python norun reason=fragment file=random.py group=shadow
print("this is the local random.py")
```

```python fails file=main.py group=shadow id=shadow expect=exception:AttributeError
import random

print(random.randint(1, 6))
```

Do not give your files the names of standard modules. `sys.stdlib_module_names` lists those names.[^2]

### `PYTHONPATH` in the shell profile

The variable applies to every Python on the machine: "PYTHONPATH will affect all installed Python versions/environments. Be wary of setting this in your shell profile or global environment variables. The site module offers more nuanced techniques as mentioned below."[^4] Set it for one command, or use a virtual environment.

## Comparison

**Go** leaves the meaning of an import path to the implementation: "The interpretation of the ImportPath is implementation-dependent but it is typically a substring of the full file name of the compiled package and may be relative to a repository of installed packages."[^6]

**JavaScript** also leaves the search to the host: "The ECMAScript specification does not define how module specifiers are resolved and leaves it to the host environment".[^7] Python defines one search in the language documentation, with `sys.path` as a list that a program can read and change.

## Implementation notes

CPython finds the standard library from the location of the executable. Without `PYTHONHOME`, "these directories are found by using the Python executable as a starting point and then looking for various ‘landmark’ files and directories."[^4] A virtual environment uses this: "If PYTHONHOME is not set, and a pyvenv.cfg file is found alongside the main executable, or in its parent directory, sys.prefix and sys.exec_prefix get set to the directory containing pyvenv.cfg".[^4]

The `site` module adds the site-packages directories when the interpreter starts: "This module is automatically imported during initialization. The automatic import can be suppressed using the interpreter’s -S option."[^8]

## References
