---
tag: 08JY25
kind: feature
owner: python
slug: lazy-imports
name: Lazy imports
aliases: [lazy import, lazy keyword, lazy from import, PEP 810, explicit lazy imports, __lazy_modules__, sys.set_lazy_imports, sys.set_lazy_imports_filter, sys.lazy_modules, types.LazyImportType, importlib.util.LazyLoader, reification]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  lazy import binds a proxy, and the module loads when the name is first used. Errors and import side effects move to that point. __lazy_modules__, sys.set_lazy_imports and a filter also control it.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#lazy-imports", locator: "7.11.1. Lazy imports; 7.11.1.1. Compatibility via __lazy_modules__"}
see_also: [python/the-import-statement, python/the-import-system, python/import-hooks, python/module-objects, python/soft-keywords]
sources:
- url: https://docs.python.org/3.15/reference/simple_stmts.html#lazy-imports
  title: The Python Language Reference, 7. Simple statements
  locator: 7.11. The import statement; 7.11.1. Lazy imports; 7.11.1.1. Compatibility via __lazy_modules__
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0810/
  title: "PEP 810: Explicit lazy imports"
  locator: Abstract; Syntax restrictions; Reification; Observable behavioral shifts (opt-in only); FAQ
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/sys.html#sys.set_lazy_imports
  title: sys — System-specific parameters and functions
  locator: sys.lazy_modules; sys.set_lazy_imports; sys.set_lazy_imports_filter
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/using/cmdline.html#envvar-PYTHON_LAZY_IMPORTS
  title: 1. Command line and environment, Python Setup and Usage
  locator: -X lazy_imports; PYTHON_LAZY_IMPORTS
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/importlib.html#importlib.util.LazyLoader
  title: importlib — The implementation of import
  locator: importlib.util.LazyLoader; Implementing lazy imports
  accessed: 2026-11-18
- url: https://go.dev/ref/spec#Package_initialization
  title: The Go Programming Language Specification
  locator: Package initialization; Program execution
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/types.html#types.LazyImportType
  title: types — Dynamic type creation and names for built-in types
  locator: types.LazyImportType
  accessed: 2026-11-18
---

## Synopsis

```text
lazy import json                       # binds a proxy; json loads at the first use of the name
lazy from pathlib import Path, PurePath
                                       # only at module level, not in def, class or try
                                       # not allowed: lazy from m import *, lazy from __future__ import ...

__lazy_modules__ = ["json"]            # before the imports: import json is then lazy

sys.set_lazy_imports("all")            # "normal" (the default) or "all"
sys.set_lazy_imports_filter(f)         # f(importing, imported, fromlist) -> bool
python -X lazy_imports=all app.py      # or PYTHON_LAZY_IMPORTS=all
```

## Semantics

**r1.** "When an import statement is preceded by the lazy keyword, the import becomes lazy: the module is not loaded immediately at the import statement. Instead, a lazy proxy object is created and bound to the name. The actual module is loaded on first use of that name."[^1] `lazy` "is a soft keyword that only has special meaning when it appears immediately before an import or from statement."[^1] Example: [`deferred`](#deferred).

**r2.** "Lazy imports are only permitted at module scope. Using lazy inside a function, class body, or try/except/finally block raises a SyntaxError. Star imports cannot be lazy (lazy from module import * is a syntax error), and future statements cannot be lazy."[^1] Example: [`try-block`](#try-block).

**r3.** "When using lazy from ... import, each imported name is bound to a lazy proxy object. The first access to any of these names triggers loading of the entire module and resolves only that specific name to its actual value. Other names remain as lazy proxies until they are accessed."[^1] Example: [`lazy-from`](#lazy-from).

**r4.** "If an error occurs during module loading (such as ImportError or SyntaxError), it is raised at the point where the lazy import is first used, not at the import statement itself."[^1] PEP 810 adds: "Exceptions during reification prevent the replacement of the lazy object, and subsequent uses of the lazy object will retry the whole reification."[^2] Examples: [`error-on-use`](#error-on-use), [`retry`](#retry).

**r5.** "A lazily imported module does not appear in sys.modules until first use. After reification, it must appear in sys.modules."[^2] `sys.lazy_modules` is "A set of fully qualified module name strings that have been lazily imported in the current interpreter but not yet loaded."[^3] Example: [`deferred`](#deferred).

**r6.** A module can ask for lazy imports without the keyword. "Any regular (non-lazy) import statement at module scope whose target appears in __lazy_modules__ is treated as a lazy import, exactly as if the lazy keyword had been used."[^1] The reference gives the reason: "This is useful when supporting Python versions older than 3.15 while using lazy imports in 3.15+".[^1] Example: [`lazy-modules`](#lazy-modules).

**r7.** The mode applies to the whole program. `sys.set_lazy_imports()` takes `"normal"`, where "Only imports explicitly marked with the lazy keyword are lazy", or `"all"`, where "All top-level imports become potentially lazy".[^3] The filter "is called for every potentially lazy import to determine whether it should actually be lazy."[^3] "The filter should return True to allow the import to be lazy, or False to force an eager import."[^3] The option `-X lazy_imports` and the variable `PYTHON_LAZY_IMPORTS` accept the same two values.[^4] Example: [`filter`](#filter).

**r8.** "Import-time side effects in lazily imported modules occur at first use of the binding, not at module import time."[^2] "Because modules are imported on first use, the order in which modules are imported may differ from how they appear in code."[^2] Example: [`registry`](#registry).

## Examples

The module runs when the program first uses the name `heavy` (rules r1 and r5). Until then it is in `sys.lazy_modules` and not in `sys.modules`.

```python norun reason=fragment file=heavy.py group=deferred
print("loading heavy")
VALUE = 42
```

```python run file=main.py group=deferred id=deferred since=3.15
import sys

lazy import heavy

print("start:", "heavy" in sys.modules, "heavy" in sys.lazy_modules)
print(heavy.VALUE)
print("after use:", "heavy" in sys.modules, "heavy" in sys.lazy_modules)
```

```output for=deferred
start: False True
loading heavy
42
after use: True False
```

With `lazy from`, the first name that is used loads the whole module (rule r3). The module runs once for both names.

```python norun reason=fragment file=shapes.py group=lazy-from
print("loading shapes")

def circle(r):
    return f"circle {r}"

def square(s):
    return f"square {s}"
```

```python run file=main.py group=lazy-from id=lazy-from since=3.15
import sys

lazy from shapes import circle, square

print("shapes" in sys.modules)
print(circle(2))
print("shapes" in sys.modules)
print(square(3))
```

```output for=lazy-from
False
loading shapes
circle 2
True
square 3
```

A missing module is not an error at the `lazy import` statement (rule r4). The error comes at the first use.

```python fails id=error-on-use expect=exception:ModuleNotFoundError since=3.15
lazy import no_such_module

print("the import statement did not fail")
no_such_module.run()
```

`__lazy_modules__` makes an ordinary `import` statement lazy (rule r6). The name must be set before the import.

```python norun reason=fragment file=heavy.py group=lazy-modules
print("loading heavy")
VALUE = 42
```

```python run file=main.py group=lazy-modules id=lazy-modules since=3.15
__lazy_modules__ = ["heavy"]

import sys

import heavy

print("heavy" in sys.modules)
print(heavy.VALUE)
```

```output for=lazy-modules
False
loading heavy
42
```

A program makes its own modules lazy and keeps the others eager (rule r7). The filter returns `True` only for names that start with `app`, so `heavy` loads at its `import` statement.

```python norun reason=fragment file=app_report.py group=filter
print("loading app_report")
TITLE = "report"
```

```python norun reason=fragment file=heavy.py group=filter
print("loading heavy")
VALUE = 42
```

```python run file=main.py group=filter id=filter since=3.15
import sys

def only_app(importing, imported, fromlist):
    return imported.startswith("app")

sys.set_lazy_imports_filter(only_app)
sys.set_lazy_imports("all")

import app_report
import heavy

print("app_report" in sys.modules, "heavy" in sys.modules)
print(app_report.TITLE)
```

```output for=filter
loading heavy
False True
loading app_report
report
```

The older library tool is `importlib.util.LazyLoader`, "A class which postpones the execution of the loader of a module until the module has an attribute accessed."[^5] This example is the importlib recipe "Implementing lazy imports". It needs no lazy syntax.

```python norun reason=fragment file=heavy.py group=lazy-loader
print("loading heavy")
VALUE = 42
```

```python run file=main.py group=lazy-loader id=lazy-loader
import importlib.util
import sys

def lazy_import(name):
    spec = importlib.util.find_spec(name)
    loader = importlib.util.LazyLoader(spec.loader)
    spec.loader = loader
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    loader.exec_module(module)
    return module

heavy = lazy_import("heavy")
print("created")
print(heavy.VALUE)
```

```output for=lazy-loader
created
loading heavy
42
```

PEP 810 explains why it is not enough: "Most critically, LazyLoader does not support from ... import statements."[^2]

## Versions

The 3.15 reference documents lazy imports and `__lazy_modules__`.[^1] Before 3.15, `lazy import` is a syntax error. An older interpreter ignores `__lazy_modules__`, so [`lazy-modules`](#lazy-modules) prints `loading heavy`, `True` and `42` on CPython 3.12.3, 3.13.14 and 3.14.6. The examples marked `since=3.15` were run on CPython 3.15.0b2. [`lazy-loader`](#lazy-loader) gives the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Pitfalls

### `lazy` in a `try` block

Code that handles a missing optional module puts the import in a `try` block. A lazy import is not allowed there (rule r2), and its error would come later in any case (rule r4).

```python fails id=try-block expect=exception:SyntaxError since=3.15
try:
    lazy import json
except ImportError:
    json = None
```

Use an eager import for an optional module.

### A module that registers itself when it is imported

Some modules add an entry to a registry as an import side effect. A lazy import delays this until the name is used (rule r8), so the registry is empty.

```python norun reason=fragment file=registry.py group=registry
FORMATS = []
```

```python norun reason=fragment file=csv_format.py group=registry
import registry

registry.FORMATS.append("csv")
```

```python wrong file=main.py group=registry id=registry since=3.15
import registry

lazy import csv_format

print(registry.FORMATS)
```

```output for=registry
[]
```

PEP 810 recommends: "Use explicit initialization functions instead of import-time side effects".[^2]

### A failed lazy import runs the module again

The lazy object stays in place after an error (rule r4). Each use tries the import again, so the top-level code of the module runs again. The error is chained to an `ImportError` that names the lazy import.

```python norun reason=fragment file=broken.py group=retry
print("running broken")
1 / 0
```

```python run file=main.py group=retry id=retry since=3.15
lazy import broken

for attempt in (1, 2):
    try:
        broken.VALUE
    except ZeroDivisionError as e:
        print(attempt, type(e).__name__, type(e.__cause__).__name__)
```

```output for=retry
running broken
1 ZeroDivisionError ImportError
running broken
2 ZeroDivisionError ImportError
```

## Comparison

**Go** loads every imported package before the program starts: "If a package has imports, the imported packages are initialized before initializing the package itself."[^6] "Program execution begins by initializing the program and then invoking the function main in package main."[^6] A Go program has no lazy form of import. In Python an import is a statement that runs, and `lazy` moves its work to the first use of the name.

## Implementation notes

The proxy has its own type. `types.LazyImportType` is "The type of lazy import proxy objects. These objects are created when a module is lazily imported and serve as placeholders until the module is actually accessed."[^7] PEP 810 notes that the import happens with the import state of that time: "reification still calls __import__ to resolve the import, which uses the state of the import system (e.g. sys.path, sys.meta_path, sys.path_hooks and __import__) at reification time, not the state when the lazy import statement was evaluated."[^2] PEP 810 also has the note "This PEP is a historical document", and it names the reference section on lazy imports as the canonical documentation.[^2]

## References
