---
tag: 93AVZX
kind: feature
owner: python
slug: standard-library-map
name: Standard library map
aliases: [library reference chapters, Python module index, sys.stdlib_module_names, sys.builtin_module_names, removed modules, dead batteries, find a standard module]
area: languages/python/stdlib
level: core
summary: >-
  The library reference groups the standard modules into chapters. sys.stdlib_module_names lists every standard module, and the Removed Modules chapter lists the old ones.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/index.html", locator: "The Python standard library"}
proposals: []
see_also: [python/the-standard-library-tour, python/built-in-functions, python/built-in-types-reference, python/the-python-package-index, python/the-ast-module]
sources:
  - url: https://docs.python.org/3.15/library/index.html
    title: The Python standard library, Python 3.15 documentation
    locator: Introduction paragraphs; the table of contents
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/intro.html
    title: Introduction, Python 3.15 documentation
    locator: Introduction; Notes on availability
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.stdlib_module_names
    title: sys — System-specific parameters and functions, Python 3.15 documentation
    locator: sys.stdlib_module_names; sys.builtin_module_names
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/removed.html
    title: Removed Modules, Python 3.15 documentation
    locator: Removed Modules
    accessed: 2026-11-16
---

## Synopsis

```python run id=names
import sys

for name in ["json", "tomllib", "requests", "email", "email.mime"]:
    print(name, name in sys.stdlib_module_names)
```

```output for=names
json True
tomllib True
requests False
email True
email.mime False
```

## Semantics

**r1.** "This library reference manual describes the standard library distributed with Python. It also describes some of the optional components that are commonly included in Python distributions."[^1]

**r2.** The chapters group the modules by task. The main chapters are text processing, binary data, data types, numeric and mathematical modules, functional programming, file and directory access, data persistence, data compression and archiving, file formats, cryptographic services, operating system services, command-line interfaces, concurrent execution, networking, internet data handling, structured markup, internet protocols, multimedia, internationalization, Tk, development tools, debugging and profiling, packaging, Python runtime services, importing modules, Python language services, and the Windows and Unix services.[^1]

**r3.** To find a module, "you can also browse the table of contents (in front of the manual), or look for a specific function, module or term in the index (in the back)."[^2]

**r4.** `sys.stdlib_module_names` is "A frozenset of strings containing the names of standard library modules. It is the same on all platforms."[^3] "For packages, only the main package is listed".[^3] Example: [`names`](#names).

**r5.** `sys.builtin_module_names` holds "the names of all modules that are compiled into this Python interpreter."[^3] Example: [`builtin`](#builtin).

**r6.** An "Availability: Unix" note "means that this function is commonly found on Unix systems. It does not make any claims about its existence on a specific operating system."[^2] Example: [`platform-module`](#platform-module).

**r7.** "The modules described in this chapter have been removed from the Python standard library. They are documented here to help people find replacements."[^4] Example: [`removed`](#removed).

## Examples

```python run id=classify
import ast
import sys

source = """
import os, numpy
from tomllib import loads
from yaml import safe_load
"""

for node in ast.walk(ast.parse(source)):
    if isinstance(node, ast.Import):
        names = [a.name for a in node.names]
    elif isinstance(node, ast.ImportFrom):
        names = [node.module]
    else:
        continue
    for n in names:
        top = n.split(".")[0]
        print(n, "stdlib" if top in sys.stdlib_module_names else "third party")
```

```output for=classify
os stdlib
numpy third party
tomllib stdlib
yaml third party
```

The script reads the imports of some source with [[python/the-ast-module]] and checks the top package of each one against `sys.stdlib_module_names` (rule r4).

```python run id=builtin
import sys

print("sys" in sys.builtin_module_names)
print("json" in sys.builtin_module_names, "json" in sys.stdlib_module_names)
```

```output for=builtin
True
False True
```

`sys` is compiled into the interpreter. `json` is a standard module, but it is a package of Python files, not a built-in module (rule r5).

## Pitfalls

```python wrong id=platform-module
import sys

print("winreg" in sys.stdlib_module_names)
try:
    import winreg
    print("imported")
except ImportError:
    print("not on this platform")
```

```output for=platform-module
True
not on this platform
```

The name is in the list on every platform, but `winreg` imports only on Windows. "Modules which are not available on some platforms and modules disabled at Python build are also listed."[^3]

```python fails id=removed expect=exception:ModuleNotFoundError
import sys

print("imp" in sys.stdlib_module_names)
import imp
```

`imp` is in the Removed Modules chapter, so new versions do not have it.[^4] The chapter names the replacement for each removed module.

## Comparison

The standard library is large, and the Python Package Index has many more packages (see [[python/the-python-package-index]]).[^1] Start with [[python/the-standard-library-tour]] for the common modules. The built-in functions and types are not in a module chapter: see [[python/built-in-functions]] and [[python/built-in-types-reference]].
