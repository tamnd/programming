---
tag: R45JM7
kind: feature
owner: python
slug: the-main-module
name: The __main__ module
aliases: [__main__, __main__.py, python -m package, __main__.__spec__, import __main__, top-level code environment, pickle __main__]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  The program runs in a module named __main__. python -m pkg runs pkg/__main__.py, __main__.__spec__ depends on how Python started, and objects defined in __main__ pickle under that name.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html#special-considerations-for-main", locator: "5.8. Special considerations for __main__; 5.8.1. __main__.__spec__"}
see_also: [python/executing-modules-as-scripts, python/module-objects, python/packages, python/intra-package-references, python/the-import-system]
sources:
- url: https://docs.python.org/3.15/library/__main__.html
  title: __main__ — Top-level code environment
  locator: What is the “top-level code environment”?; __main__.py in Python Packages; Idiomatic Usage; import __main__
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/reference/import.html#special-considerations-for-main
  title: The Python Language Reference, 5. The import system
  locator: 5.8. Special considerations for __main__; 5.8.1. __main__.__spec__
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-m
  title: 1. Command line and environment, Python Setup and Usage
  locator: 1.1.1. Interface options, -m and <script>
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/runpy.html
  title: runpy — Locating and executing Python modules
  locator: runpy.run_module
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/pickle.html#what-can-be-pickled-and-unpickled
  title: pickle — Python object serialization
  locator: What can be pickled and unpickled?
  accessed: 2026-11-18
- url: https://go.dev/ref/spec#Program_execution
  title: The Go Programming Language Specification
  locator: Program execution
  accessed: 2026-11-18
---

## Synopsis

```text
bandclass/
    __init__.py
    __main__.py          # runs for: python -m bandclass
    student.py

python file.py           # __main__.__spec__ is None
python -m pkg.mod        # __main__.__spec__.name == "pkg.mod"
python -m pkg            # runs pkg.__main__; __spec__.name == "pkg.__main__"
python some_dir          # runs some_dir/__main__.py; __spec__.name == "__main__"

import __main__          # the module of the program, not a file named __main__.py
```

## Semantics

**r1.** "__main__ is the name of the environment where top-level code is run. “Top-level code” is the first user-specified Python module that starts running."[^1] "The __main__ module is a special case relative to Python’s import system. As noted elsewhere, the __main__ module is directly initialized at interpreter startup, much like sys and builtins."[^2]

**r2.** A package can be a program. With `-m`, "When a package name is supplied instead of a normal module, the interpreter will execute <pkg>.__main__ as the main module."[^3] A directory or a zip file works the same way as the script argument: the path can refer to "a directory containing a __main__.py file, or a zipfile".[^3] Example: [`package-main`](#package-main).

**r3.** The library page advises a short `__main__.py`: "The content of __main__.py typically isn’t fenced with an if __name__ == '__main__' block. Instead, those files are kept short and import functions to execute from other modules."[^1] Example: [`package-main`](#package-main).

**r4.** "When Python is started with the -m option, __spec__ is set to the module spec of the corresponding module or package. __spec__ is also populated when the __main__ module is loaded as part of executing a directory, zipfile or other sys.path entry."[^2] For a file run by its path, "__main__.__spec__ is always None in the last case, even if the file could technically be imported directly as a module instead."[^2] For a package, `runpy` sets the name to `mod_name + '.__main__'`.[^4] Example: [`spec-by-start`](#spec-by-start).

**r5.** Any module can import the program: "other modules running within that same program can import the top-level environment’s scope (namespace) by importing the __main__ module. This doesn’t import a __main__.py file but rather whichever module that received the special name '__main__'."[^1] Example: [`import-main`](#import-main).

**r6.** "Python inserts an empty __main__ module in sys.modules at interpreter startup, and populates it by running top-level code."[^1] So a module that imports `__main__` while the program runs its top-level code gets a module that is only partly filled. Example: [`import-main`](#import-main).

**r7.** The main module and an import of the same file are two modules: "even when __main__ corresponds with an importable module and __main__.__spec__ is set accordingly, they’re still considered distinct modules."[^2]

## Examples

`python -m greet` runs `greet/__main__.py` (rules r2 and r3). Its `__name__` is `'__main__'` and its spec name is `greet.__main__`, so the relative import works.

```python norun reason=fragment file=greet/__init__.py group=package-main
"""The greet package."""
```

```python norun reason=fragment file=greet/core.py group=package-main
def hello(name):
    return f"hello, {name}"
```

```python norun reason=fragment file=greet/__main__.py group=package-main
import sys

from .core import hello

print(hello(sys.argv[1]))
print(__name__, __spec__.name)
```

```python run file=main.py group=package-main id=package-main
import subprocess
import sys

child = subprocess.run([sys.executable, "-m", "greet", "Ada"], capture_output=True, text=True)
print(child.stdout, end="")
```

```output for=package-main
hello, Ada
__main__ greet.__main__
```

The same code started in four ways (rule r4). Only a file run by its path has no spec.

```python norun reason=fragment file=tool.py group=spec-by-start
print(__name__, __spec__.name if __spec__ else None)
```

```python norun reason=fragment file=app/__main__.py group=spec-by-start
print(__name__, __spec__.name if __spec__ else None)
```

```python norun reason=fragment file=pkg/__init__.py group=spec-by-start
"""A package with a __main__ module."""
```

```python norun reason=fragment file=pkg/__main__.py group=spec-by-start
print(__name__, __spec__.name if __spec__ else None)
```

```python run file=main.py group=spec-by-start id=spec-by-start
import subprocess
import sys

for args in (["tool.py"], ["-m", "tool"], ["-m", "pkg"], ["app"]):
    child = subprocess.run([sys.executable, *args], capture_output=True, text=True)
    print(" ".join(args), "->", child.stdout.strip())
```

```output for=spec-by-start
tool.py -> __main__ None
-m tool -> __main__ tool
-m pkg -> __main__ pkg.__main__
app -> __main__ __main__
```

A helper module reads a setting from the program with `import __main__` (rule r5). The import gives the module that is running, as far as its top-level code has gone (rule r6).

```python norun reason=fragment file=namely.py group=import-main
import __main__


def user_name():
    return getattr(__main__, "my_name", "not set")


print("at import:", user_name())
```

```python run file=main.py group=import-main id=import-main
import sys

import namely

my_name = "Dinsdale"
print("later:", namely.user_name())
print(sys.modules["__main__"] is sys.modules[__name__])
```

```output for=import-main
at import: not set
later: Dinsdale
True
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Pitfalls

### A class defined in `__main__` and pickle

pickle stores a class by name: "classes are pickled by fully qualified name, so the same restrictions in the unpickling environment apply."[^5] "Thus the defining module must be importable in the unpickling environment, and the module must contain the named object".[^5] A class of the program has the module name `__main__`, so another program cannot load the data.

```python norun reason=fragment file=load.py group=pickle-main
import pickle

with open("point.pickle", "rb") as f:
    print(pickle.load(f))
```

```python wrong file=main.py group=pickle-main id=pickle-main
import pickle
import subprocess
import sys


class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y


data = pickle.dumps(Point(1, 2))
print(b"__main__" in data)
with open("point.pickle", "wb") as f:
    f.write(data)

child = subprocess.run([sys.executable, "load.py"], capture_output=True, text=True)
print(child.stderr.splitlines()[-1].split(":")[0])
```

```output for=pickle-main
True
AttributeError
```

Define the class in a module that both programs import. Then pickle stores the name of that module.

```python norun reason=fragment file=shapes.py group=pickle-module
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y
```

```python norun reason=fragment file=load.py group=pickle-module
import pickle

with open("point.pickle", "rb") as f:
    p = pickle.load(f)
print(type(p).__module__, p.x, p.y)
```

```python run file=main.py group=pickle-module id=pickle-module
import pickle
import subprocess
import sys

from shapes import Point

with open("point.pickle", "wb") as f:
    pickle.dump(Point(1, 2), f)

child = subprocess.run([sys.executable, "load.py"], capture_output=True, text=True)
print(child.stdout.strip())
```

```output for=pickle-module
shapes 1 2
```

### An `if __name__` guard in a zip file

A `__main__.py` in a package has the name `pkg.__main__` when it is imported, so a guard there works. The library page warns: "This won’t work for __main__.py files in the root directory of a .zip file though. Hence, for consistency, a minimal __main__.py without a __name__ check is preferred."[^1]

### Two copies of the program module

Rule r7 means that `import main` inside the program makes a second module from `main.py`, with its own globals. Keep the state in a module that is not the program. The page [[python/executing-modules-as-scripts]] shows this case.

## Comparison

**Go** has one fixed form for the program: "A complete program is created by linking a single, unimported package called the main package with all the packages it imports, transitively."[^6] In Python, any module, package, directory or zip file can be the main module, and its name is `__main__` in each case (rule r1).

## Implementation notes

`python -m` uses `runpy`. `runpy.run_module` sets `__spec__` "appropriately for the actually imported module (that is, __spec__.name will always be mod_name or mod_name + '.__main__', never run_name)."[^4] The library page names two users of the `__main__` namespace: "The __main__ scope is used in the implementation of pdb and rlcompleter."[^1]

## References
