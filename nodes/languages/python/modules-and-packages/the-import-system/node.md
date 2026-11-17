---
tag: 5CHH7N
kind: feature
owner: python
slug: the-import-system
name: The import system
aliases: [import machinery, importlib, sys.modules, sys.meta_path, finder, loader, module spec, ModuleSpec, path based finder, PathFinder, path entry finder, sys.path_importer_cache, importlib.import_module, importlib.invalidate_caches]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  An import checks sys.modules, then asks each finder on sys.meta_path for a module spec. The loader of the spec runs the module. The path based finder searches sys.path with path entry finders.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html", locator: "5.1. importlib; 5.3. Searching; 5.4. Loading; 5.5. The Path Based Finder"}
see_also: [python/import-hooks, python/module-objects, python/the-module-search-path, python/the-import-statement, python/namespace-packages, python/modules]
sources:
- url: https://docs.python.org/3.15/reference/import.html
  title: The Python Language Reference, 5. The import system
  locator: 5.1. importlib; 5.3. Searching; 5.3.1. The module cache; 5.3.2. Finders and loaders; 5.3.4. The meta path; 5.4. Loading; 5.4.3. Module specs; 5.5. The Path Based Finder; 5.5.1. Path entry finders
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/importlib.html
  title: importlib — The implementation of import
  locator: importlib.import_module; importlib.invalidate_caches; importlib.util.find_spec
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0451/
  title: "PEP 451: A ModuleSpec Type for the Import System"
  locator: Abstract
  accessed: 2026-11-18
- url: https://go.dev/ref/spec#Import_declarations
  title: The Go Programming Language Specification
  locator: Import declarations
  accessed: 2026-11-18
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
  title: import, JavaScript reference, MDN
  locator: Module specifier resolution
  accessed: 2026-11-18
---

## Synopsis

```text
import foo.bar.baz
  for each of foo, foo.bar, foo.bar.baz:
    1. sys.modules[name]                    # the cache; None means "not found"
    2. for finder in sys.meta_path:          # BuiltinImporter, FrozenImporter, PathFinder
           spec = finder.find_spec(name, parent.__path__ or None, None)
    3. module = new module from spec; set __name__, __spec__, ...
       sys.modules[name] = module
       spec.loader.exec_module(module)       # on error: del sys.modules[name]

importlib.import_module("pkg.mod")          # import by a name in a string
importlib.util.find_spec("pkg.mod")         # find without loading (parents are imported)
importlib.invalidate_caches()               # after a module file is made at run time
```

## Semantics

**r1.** "The importlib module provides a rich API for interacting with the import system. For example importlib.import_module() provides a recommended, simpler API than built-in __import__() for invoking the import machinery."[^1] "The most important difference between these two functions is that import_module() returns the specified package or module (e.g. pkg.mod), while __import__() returns the top-level package or module (e.g. pkg)."[^2] Example: [`import-module`](#import-module).

**r2.** A dotted name is imported one level at a time: "Python first tries to import foo, then foo.bar, and finally foo.bar.baz. If any of the intermediate imports fail, a ModuleNotFoundError is raised."[^1] Example: [`trace`](#trace).

**r3.** "The first place checked during import search is sys.modules. This mapping serves as a cache of all modules that have been previously imported, including the intermediate paths."[^1] "However, if the value is None, then a ModuleNotFoundError is raised."[^1] Deleting a key "will invalidate the cache entry for the named module, causing Python to search anew for the named module upon its next import."[^1] Example: [`cache`](#cache).

**r4.** If the cache does not have the name, "Python’s import protocol is invoked to find and load the module. This protocol consists of two conceptual objects, finders and loaders."[^1] "Finders do not actually load modules. If they can find the named module, they return a module spec, an encapsulation of the module’s import-related information, which the import machinery then uses when loading the module."[^1] PEP 451 added the spec type. It "will provide all the import-related information used to load a module and will be available without needing to load the module first."[^3]

**r5.** The finders are in `sys.meta_path`. "These finders are queried in order to see if they know how to handle the named module."[^1] Each one returns a spec or `None`. For a submodule, "the second argument is the value of the parent package’s __path__ attribute."[^1] "Python’s default sys.meta_path has three meta path finders, one that knows how to import built-in modules, one that knows how to import frozen modules, and one that knows how to import modules from an import path (i.e. the path based finder)."[^1] Examples: [`trace`](#trace), [`meta-path`](#meta-path).

**r6.** "The module will exist in sys.modules before the loader executes the module code. This is crucial because the module code may (directly or indirectly) import itself".[^1] "If loading fails, the failing module – and only the failing module – gets removed from sys.modules. Any module already in the sys.modules cache, and any module that was successfully loaded as a side-effect, must remain in the cache."[^1] Example: [`failed-load`](#failed-load).

**r7.** The path based finder "itself doesn’t know how to import anything. Instead, it traverses the individual path entries, associating each of them with a path entry finder that knows how to handle that particular kind of path."[^1] It keeps "a cache mapping path entries to path entry finders. This cache is maintained in sys.path_importer_cache".[^1] For a path entry that is not in the cache, it calls the hooks in `sys.path_hooks`. Example: [`meta-path`](#meta-path).

## Examples

`importlib.import_module` returns the submodule, and `__import__` returns the top package (rule r1). A relative name needs the `package` argument.

```python norun reason=fragment file=shop/__init__.py group=import-module
"""The shop package."""
```

```python norun reason=fragment file=shop/cart.py group=import-module
NAME = "cart"
```

```python norun reason=fragment file=shop/prices.py group=import-module
NAME = "prices"
```

```python run file=main.py group=import-module id=import-module
import importlib

print(importlib.import_module("shop.cart").__name__)
print(__import__("shop.cart").__name__)
print(importlib.import_module(".prices", package="shop").NAME)
```

```output for=import-module
shop.cart
shop
prices
```

A finder at the front of `sys.meta_path` sees each level of a dotted import (rules r2 and r5). It returns `None`, so the next finders do the work. The second import comes from `sys.modules` and asks no finder.

```python norun reason=fragment file=shop/__init__.py group=trace
"""The shop package."""
```

```python norun reason=fragment file=shop/cart.py group=trace
NAME = "cart"
```

```python run file=main.py group=trace id=trace
import os
import sys


class Tracer:
    def find_spec(self, name, path, target=None):
        if name.startswith("shop"):
            where = None if path is None else [os.path.relpath(p) for p in path]
            print("find_spec", name, where)
        return None


sys.meta_path.insert(0, Tracer())
import shop.cart

print("imported", shop.cart.NAME)
import shop.cart

print("second import done")
```

```output for=trace
find_spec shop None
find_spec shop.cart ['shop']
imported cart
second import done
```

The cache decides what `import` returns (rule r3). A `None` entry blocks the import, and a deleted entry gives a fresh module object.

```python norun reason=fragment file=config.py group=cache
print("running config.py")
```

```python run file=main.py group=cache id=cache
import sys

import config

first = config
del sys.modules["config"]
import config

print(first is config)

sys.modules["config"] = None
try:
    import config
except ModuleNotFoundError as e:
    print(type(e).__name__, e.name)
```

```output for=cache
running config.py
running config.py
False
ModuleNotFoundError config
```

The last three finders on `sys.meta_path` are the default ones (rule r5). The path based finder keeps one path entry finder for each entry of `sys.path` (rule r7).

```python run id=meta-path
import sys

print([f.__name__ for f in sys.meta_path[-3:]])

import importlib.util

importlib.util.find_spec("no_such_module")
entry = sys.path[0]
print(type(sys.path_importer_cache[entry]).__name__)
```

```output for=meta-path
['BuiltinImporter', 'FrozenImporter', 'PathFinder']
FileFinder
```

The module is in `sys.modules` while its code runs, and a failed module is removed (rule r6). `helper`, which loaded before the error, stays.

```python norun reason=fragment file=helper.py group=failed-load
VALUE = 1
```

```python norun reason=fragment file=broken.py group=failed-load
import sys

import helper

print("broken in sys.modules:", "broken" in sys.modules)
raise RuntimeError("setup failed")
```

```python run file=main.py group=failed-load id=failed-load
import sys

try:
    import broken
except RuntimeError as e:
    print("error:", e)

print("broken" in sys.modules, "helper" in sys.modules)
```

```output for=failed-load
broken in sys.modules: True
error: setup failed
False True
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Pitfalls

### A module file made while the program runs

The finders cache what they saw in each directory. For a module "that was created since the interpreter began execution (e.g., created a Python source file), you may need to call invalidate_caches() in order for the new module to be noticed by the import system."[^2]

```python run id=new-file
import importlib

with open("generated.py", "w") as f:
    f.write("ANSWER = 42\n")

importlib.invalidate_caches()
generated = importlib.import_module("generated")
print(generated.ANSWER)
```

```output for=new-file
42
```

### Two module objects for one name

After `del sys.modules[name]`, the next import makes a second module (rule r3). The reference warns: "if you keep a reference to the module object, invalidate its cache entry in sys.modules, and then re-import the named module, the two module objects will not be the same."[^1] Old references then see old values, as `first` does in [`cache`](#cache). `importlib.reload()` instead "will reuse the same module object".[^1]

### `find_spec` imports the parents

`importlib.util.find_spec` does not load the module itself, but "If name is for a submodule (contains a dot), the parent module is automatically imported."[^2] Code in the `__init__.py` of the parent runs.

## Comparison

**Go** leaves the import path to the tools: "The interpretation of the ImportPath is implementation-dependent but it is typically a substring of the full file name of the compiled package".[^4] The package is found when the program is built. Python finds each module at run time, with finders that a program can change.

**JavaScript** leaves resolution to the host: "The ECMAScript specification does not define how module specifiers are resolved and leaves it to the host environment (e.g., browsers, Node.js, Deno)."[^5] In Python the import system is part of the language reference, and `importlib` implements it in Python code.

## Implementation notes

CPython implements import in `importlib`. The library page gives this as its first purpose: "to provide the implementation of the import statement (and thus, by extension, the __import__() function) in Python source code."[^2] The default finders on `sys.meta_path` are class objects, not instances, so [`meta-path`](#meta-path) prints their `__name__`. A search for a missing module visits every entry of `sys.path`, so after it the cache has a finder for `sys.path[0]`.

## References
