---
tag: VG2K6H
kind: feature
owner: python
slug: import-hooks
name: Import hooks
aliases: [import hook, meta hook, meta path finder, path hook, path entry hook, path entry finder, custom importer, custom loader, sys.path_hooks, importlib.abc.MetaPathFinder, importlib.abc.Loader, exec_module, create_module, PEP 302]
area: languages/python/modules-and-packages
level: research
summary: >-
  A meta hook is a finder on sys.meta_path, and a path hook is a callable on sys.path_hooks. Finders return a spec with a loader, and the loader runs the code. So a program can import from any source.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html#import-hooks", locator: "5.3.3. Import hooks; 5.4.1. Loaders; 5.5.1. Path entry finders; 5.6. Replacing the standard import system"}
see_also: [python/the-import-system, python/module-objects, python/the-module-search-path, python/namespace-packages, python/the-import-statement]
sources:
- url: https://docs.python.org/3.15/reference/import.html#import-hooks
  title: The Python Language Reference, 5. The import system
  locator: 5.3.3. Import hooks; 5.3.4. The meta path; 5.4.1. Loaders; 5.5. The Path Based Finder; 5.5.1. Path entry finders; 5.6. Replacing the standard import system
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/importlib.html
  title: importlib — The implementation of import
  locator: importlib.abc.MetaPathFinder; importlib.abc.PathEntryFinder; importlib.abc.Loader; importlib.machinery.PathFinder.invalidate_caches; importlib.util.spec_from_loader; Setting up an importer
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-12.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 12. Execution
  locator: 12.2. Loading of Classes and Interfaces; 12.2.1. The Loading Process
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0302/
  title: "PEP 302: New Import Hooks"
  locator: Abstract; Motivation; Rationale
  accessed: 2026-11-18
---

## Synopsis

```text
sys.meta_path.insert(0, finder)     # a meta hook, asked before the default finders
    finder.find_spec(name, path, target=None)   # a ModuleSpec, or None to go on
    raise ModuleNotFoundError                   # stops the search at once

importlib.util.spec_from_loader(name, loader, origin="...", is_package=False)
    loader.create_module(spec)      # a module object, or None for the default one
    loader.exec_module(module)      # runs the code in module.__dict__

sys.path_hooks.insert(0, hook)      # a path hook: hook(entry) gives a path entry finder or raises ImportError
    entry_finder.find_spec(fullname, target=None)
sys.path_importer_cache[entry]      # the finder for each entry, or None
importlib.invalidate_caches()       # also deletes the None entries
```

## Semantics

**r1.** "The import machinery is designed to be extensible; the primary mechanism for this are the import hooks. There are two types of import hooks: meta hooks and import path hooks."[^1] "Meta hooks are registered by adding new finder objects to sys.meta_path".[^1] "Import path hooks are registered by adding new callables to sys.path_hooks".[^1]

**r2.** "Meta hooks are called at the start of import processing, before any other import processing has occurred, other than sys.modules cache look up. This allows meta hooks to override sys.path processing, frozen modules, or even built-in modules."[^1] A hook sees only the names that are not in `sys.modules`. Examples: [`dict-importer`](#dict-importer), [`cached-module`](#cached-module).

**r3.** A meta path finder has a `find_spec()` method. "If the meta path finder knows how to handle the named module, it returns a spec object. If it cannot handle the named module, it returns None. If sys.meta_path processing reaches the end of its list without returning a spec, then a ModuleNotFoundError is raised. Any other exceptions raised are simply propagated up, aborting the import process."[^1] The finders are asked in list order, so the place of a hook in the list decides which finder wins. Examples: [`dict-importer`](#dict-importer), [`append-loses`](#append-loses).

**r4.** The spec holds the loader. "The import machinery calls the importlib.abc.Loader.exec_module() method with a single argument, the module object to execute."[^1] "In many cases, the finder and loader can be the same object; in such cases the find_spec() method would just return a spec with the loader set to self."[^1] For `create_module()`: "If the method returns None, the import machinery will create the new module itself."[^1] `importlib.util.spec_from_loader()` is "A factory function for creating a ModuleSpec instance based on a loader."[^2] Example: [`dict-importer`](#dict-importer).

**r5.** A loader can make a package. The finder sets `submodule_search_locations` "to a sequence, even an empty one, to indicate to the import system that the module is a package."[^2] The import system then asks the meta path finders for each submodule, with the `__path__` of the package as the second argument. Example: [`dict-importer`](#dict-importer).

**r6.** A path hook works inside the path based finder. Each hook "is called with a single argument, the path entry to be searched. This callable may either return a path entry finder that can handle the path entry, or it may raise ImportError."[^1] The result is kept in `sys.path_importer_cache`. "If sys.path_hooks iteration ends with no path entry finder being returned, then the path based finder’s find_spec() method will store None in sys.path_importer_cache".[^1] Path entries "can refer to URLs, database queries, or any other location that can be specified as a string."[^1] Examples: [`path-hook`](#path-hook), [`stale-entry`](#stale-entry).

**r7.** The reference names three ways to change imports. "The most reliable mechanism for replacing the entire import system is to delete the default contents of sys.meta_path, replacing them entirely with a custom meta path hook."[^1] "If it is acceptable to only alter the behaviour of import statements without affecting other APIs that access the import system, then replacing the builtin __import__() function may be sufficient."[^1] To block some modules, "it is sufficient to raise ModuleNotFoundError directly from find_spec() instead of returning None. The latter indicates that the meta path search should continue, while raising an exception terminates it immediately."[^1] Examples: [`blocker`](#blocker), [`import-function`](#import-function).

## Examples

One object is the finder and the loader (rules r3 and r4). It makes modules from source strings in a dictionary, and `tools` is a package because its spec has an empty `submodule_search_locations` (rule r5). The spec has no file location, so the modules have no `__file__`.

```python run id=dict-importer
import importlib.abc
import importlib.util
import sys

SOURCES = {
    "greeting": "def hello(name):\n    return 'hello, ' + name\n",
    "tools": "",
    "tools.maths": "def double(x):\n    return 2 * x\n",
}


class DictImporter(importlib.abc.MetaPathFinder, importlib.abc.Loader):
    def __init__(self, sources):
        self.sources = sources

    def find_spec(self, name, path, target=None):
        if name not in self.sources:
            return None
        is_package = any(key.startswith(name + ".") for key in self.sources)
        return importlib.util.spec_from_loader(name, self, origin="memory", is_package=is_package)

    def create_module(self, spec):
        return None

    def exec_module(self, module):
        code = compile(self.sources[module.__name__], f"<memory:{module.__name__}>", "exec")
        exec(code, module.__dict__)


sys.meta_path.insert(0, DictImporter(SOURCES))

import greeting
import tools.maths

print(greeting.hello("Ada"))
print(tools.maths.double(21))
print(greeting.__spec__.origin, type(greeting.__loader__).__name__)
print(hasattr(greeting, "__file__"), tools.__path__)
```

```output for=dict-importer
hello, Ada
42
memory DictImporter
False []
```

A finder at the front of `sys.meta_path` blocks a module (rule r7). It raises `ModuleNotFoundError`, so the default finders are not asked, although `sqlite3` is in the standard library.

```python run id=blocker
import sys


class Blocker:
    def __init__(self, *names):
        self.names = names

    def find_spec(self, name, path, target=None):
        if name.partition(".")[0] in self.names:
            raise ModuleNotFoundError(f"{name} is blocked", name=name)
        return None


sys.meta_path.insert(0, Blocker("sqlite3"))

for name in ("json", "sqlite3"):
    try:
        __import__(name)
        print(name, "imported")
    except ModuleNotFoundError as e:
        print(e)
```

```output for=blocker
json imported
sqlite3 is blocked
```

A path hook gives a finder for path entries of its own form (rule r6). Here the entry `memory://plugins` is a string that is not a directory. The hook raises `ImportError` for all other entries, so the default hooks handle them.

```python run id=path-hook
import importlib.util
import sys

PLUGINS = {"hello_plugin": "NAME = 'hello'\n"}


class MemoryLoader:
    def create_module(self, spec):
        return None

    def exec_module(self, module):
        exec(PLUGINS[module.__name__], module.__dict__)


class MemoryFinder:
    def __init__(self, entry):
        self.entry = entry

    def find_spec(self, fullname, target=None):
        if fullname not in PLUGINS:
            return None
        return importlib.util.spec_from_loader(fullname, MemoryLoader(), origin=f"{self.entry}/{fullname}")


def memory_hook(entry):
    if not entry.startswith("memory://"):
        raise ImportError("not a memory entry")
    return MemoryFinder(entry)


sys.path_hooks.insert(0, memory_hook)
sys.path.append("memory://plugins")

import hello_plugin

print(hello_plugin.NAME, hello_plugin.__spec__.origin)
print(type(sys.path_importer_cache["memory://plugins"]).__name__)
```

```output for=path-hook
hello memory://plugins/hello_plugin
MemoryFinder
```

A replaced `builtins.__import__` sees `import` statements only (rule r7). It does not see `importlib.import_module`, and it is called again for a module that is already in `sys.modules`.

```python norun reason=fragment file=alpha.py group=import-function
NAME = "alpha"
```

```python norun reason=fragment file=beta.py group=import-function
NAME = "beta"
```

```python run file=main.py group=import-function id=import-function
import builtins
import importlib

original_import = builtins.__import__


def traced_import(name, globals=None, locals=None, fromlist=(), level=0):
    if name in ("alpha", "beta"):
        print("__import__", name)
    return original_import(name, globals, locals, fromlist, level)


builtins.__import__ = traced_import
import alpha

beta = importlib.import_module("beta")
import alpha

builtins.__import__ = original_import
print(alpha.NAME, beta.NAME)
```

```output for=import-function
__import__ alpha
__import__ alpha
alpha beta
```

The examples give the same output on CPython 3.12.3, 3.13.14, 3.14.6 and 3.15.0b2.

## Versions

The protocol changed twice. The reference says of `find_module()`: "Changed in version 3.12: find_module() has been removed. Use find_spec() instead."[^1] The 3.15 importlib page says of `importlib.abc.Loader`: "Changed in version 3.15: Removed the load_module() method."[^2] A hook that defines only these methods is not used. The hooks on this page use `find_spec()`, `create_module()` and `exec_module()`.

## Pitfalls

### An appended finder comes after the path based finder

The importlib recipe for an importer has a comment that the finder must go in the proper location in the list.[^2] A finder added with `append` is asked after the default finders (rule r3). If a file `greeting.py` is on `sys.path`, the file wins.

```python norun reason=fragment file=greeting.py group=append-loses
WHERE = "the file greeting.py"
```

```python norun reason=fragment file=memimport.py group=append-loses
import importlib.util


class DictImporter:
    def __init__(self, sources):
        self.sources = sources

    def find_spec(self, name, path, target=None):
        if name not in self.sources:
            return None
        return importlib.util.spec_from_loader(name, self, origin="memory")

    def create_module(self, spec):
        return None

    def exec_module(self, module):
        exec(self.sources[module.__name__], module.__dict__)
```

```python wrong file=main.py group=append-loses id=append-loses
import sys

from memimport import DictImporter

sys.meta_path.append(DictImporter({"greeting": "WHERE = 'the dictionary'\n"}))

import greeting

print(greeting.WHERE)
```

```output for=append-loses
the file greeting.py
```

Use `sys.meta_path.insert(0, finder)` for a finder that must win.

### A hook does not see a module that is already imported

The cache is checked before any hook (rule r2). A hook added after a module was imported cannot block that module. This is also true for the modules that the interpreter imports when it starts.

```python run id=cached-module
import sys


class Blocker:
    def find_spec(self, name, path, target=None):
        if name == "json":
            raise ModuleNotFoundError("json is blocked", name=name)
        return None


import json

sys.meta_path.insert(0, Blocker())
import json

print("imported", json.__name__)
```

```output for=cached-module
imported json
```

### A path entry that was searched before the hook was added

The path based finder stores `None` for an entry that no hook accepted (rule r6), and it does not call the hooks again for that entry. A hook added later is not used.

```python norun reason=fragment file=memhook.py group=stale-entry
import importlib.util

PLUGINS = {"hello_plugin": "NAME = 'hello'\n"}


class MemoryLoader:
    def create_module(self, spec):
        return None

    def exec_module(self, module):
        exec(PLUGINS[module.__name__], module.__dict__)


class MemoryFinder:
    def find_spec(self, fullname, target=None):
        if fullname not in PLUGINS:
            return None
        return importlib.util.spec_from_loader(fullname, MemoryLoader(), origin="memory")


def memory_hook(entry):
    if not entry.startswith("memory://"):
        raise ImportError("not a memory entry")
    return MemoryFinder()
```

```python fails file=main.py group=stale-entry id=stale-entry expect=exception:ModuleNotFoundError
import sys

import memhook

sys.path.append("memory://plugins")
try:
    import hello_plugin
except ModuleNotFoundError:
    print("cached:", sys.path_importer_cache["memory://plugins"])

sys.path_hooks.insert(0, memhook.memory_hook)
import hello_plugin
```

Add the hook before the entry is searched, or clear the cache. `importlib.machinery.PathFinder.invalidate_caches()` "Calls importlib.abc.PathEntryFinder.invalidate_caches() on all finders stored in sys.path_importer_cache that define the method. Otherwise entries in sys.path_importer_cache set to None are deleted."[^2] `importlib.invalidate_caches()` calls it.

```python norun reason=fragment file=memhook.py group=stale-fixed
import importlib.util

PLUGINS = {"hello_plugin": "NAME = 'hello'\n"}


class MemoryLoader:
    def create_module(self, spec):
        return None

    def exec_module(self, module):
        exec(PLUGINS[module.__name__], module.__dict__)


class MemoryFinder:
    def find_spec(self, fullname, target=None):
        if fullname not in PLUGINS:
            return None
        return importlib.util.spec_from_loader(fullname, MemoryLoader(), origin="memory")


def memory_hook(entry):
    if not entry.startswith("memory://"):
        raise ImportError("not a memory entry")
    return MemoryFinder()
```

```python run file=main.py group=stale-fixed id=stale-fixed
import importlib
import sys

import memhook

sys.path.append("memory://plugins")
try:
    import hello_plugin
except ModuleNotFoundError:
    print("cached:", sys.path_importer_cache["memory://plugins"])

sys.path_hooks.insert(0, memhook.memory_hook)
importlib.invalidate_caches()
import hello_plugin

print(hello_plugin.NAME)
```

```output for=stale-fixed
cached: None
hello
```

## Comparison

**Java** also lets a program change how code is found. "The loading process is implemented by the class ClassLoader and its subclasses."[^3] "Different subclasses of ClassLoader may implement different loading policies."[^3] The specification also warns about caches: these activities "may not be completely transparent to a running application if, for example, a newly compiled version of a class is not found because an older version is cached by a class loader."[^3] Python has the same problem with `sys.path_importer_cache` and the directory caches of the finders.

## Implementation notes

The hooks come from PEP 302. Before it, "The only way to customize the import mechanism is currently to override the built-in __import__ function."[^4] The PEP gives a problem of that way: "__import__ gets called even for modules that are already in sys.modules, which is almost never what you want, unless you’re writing some sort of monitoring tool."[^4] [`import-function`](#import-function) shows this. The PEP also explains the cache: "Traversing sys.path_hooks for each path item for each new import can be expensive, so the results are cached in another new object in the sys module: sys.path_importer_cache."[^4] PEP 302 now has this note: "This document is no longer updated and provided for historical purposes only."[^4] The reference and the importlib page give the current protocol.

## References
