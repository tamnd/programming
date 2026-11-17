---
tag: FQAA65
kind: feature
owner: python
slug: modules
name: Modules
aliases: [module, import, module namespace, sys.modules, importlib.reload, circular import, __name__]
area: languages/python/modules-and-packages
level: intro
summary: >-
  A module is a .py file. The first import runs its code once and makes a module object, and the names that the file defines become the attributes of that object.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html", locator: "5. The import system; 5.3.1. The module cache"}
see_also: [python/the-import-statement, python/executing-modules-as-scripts, python/the-module-search-path, python/packages, python/python-scopes-and-namespaces, python/the-dir-function]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html
  title: The Python Tutorial, 6. Modules
  locator: 6. Modules; 6.1. More on Modules; 6.1.3. “Compiled” Python files; 6.3. The dir() Function
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/import.html
  title: The Python Language Reference, 5. The import system
  locator: 5. The import system; 5.3.1. The module cache
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/datamodel.html
  title: The Python Language Reference, 3. Data model
  locator: 3.2.9. Modules
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/importlib.html#importlib.reload
  title: importlib, The implementation of import
  locator: importlib.reload()
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/whatsnew/3.13.html#improved-error-messages
  title: What’s New In Python 3.13
  locator: Improved error messages
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/faq/programming.html
  title: Programming FAQ
  locator: What are the “best practices” for using import in a module?; How can I have modules that mutually import each other?
  accessed: 2026-11-16
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
  title: import, JavaScript reference, MDN
  locator: Description
  accessed: 2026-11-16
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Package initialization
  accessed: 2026-11-16
- url: https://doc.rust-lang.org/book/ch07-02-defining-modules-to-control-scope-and-privacy.html
  title: The Rust Programming Language, 7.2. Defining Modules to Control Scope and Privacy
  locator: Modules Cheat Sheet
  accessed: 2026-11-16
---

## Synopsis

```text
# counter.py: the module counter
count = 0

def bump():
    global count
    count += 1

# main.py
import counter        # the first import runs counter.py and binds the name counter
counter.bump()        # modname.itemname
counter.count         # 1
counter.__name__      # 'counter'
```

## Semantics

**r1.** "A module is a file containing Python definitions and statements. The file name is the module name with the suffix .py appended. Within a module, the module’s name (as a string) is available as the value of the global variable __name__."[^1] Example: [`once`](#once).

**r2.** The statements of a module run when it is imported. "These statements are intended to initialize the module. They are executed only the first time the module name is encountered in an import statement."[^1] Example: [`once`](#once).

**r3.** Python keeps each loaded module in `sys.modules`. "This mapping serves as a cache of all modules that have been previously imported".[^2] "During import, the module name is looked up in sys.modules and if present, the associated value is the module satisfying the import, and the process completes."[^2] Thus each import of the same name gives the same module object. Example: [`cache`](#cache).

**r4.** "Each module has its own private namespace, which is used as the global namespace by all functions defined in the module."[^1] Two modules can use the same global name, and the names do not clash. Code outside the module gets a name with "the same notation used to refer to its functions, modname.itemname."[^1] Example: [`namespaces`](#namespaces).

**r5.** The module object holds its namespace in a dictionary. "Attribute references are translated to lookups in this dictionary, e.g., m.x is equivalent to m.__dict__["x"]."[^3] An assignment to `m.x` changes the namespace of the module, so the functions of the module see the new value. Example: [`namespaces`](#namespaces).

**r6.** "The built-in function dir() is used to find out which names a module defines. It returns a sorted list of strings".[^1] Example: [`namespaces`](#namespaces).

**r7.** A changed source file does not reload itself. "For efficiency reasons, each module is only imported once per interpreter session. Therefore, if you change your modules, you must restart the interpreter".[^1] `importlib.reload(module)` runs the code again: "Python module’s code is recompiled and the module-level code re-executed, defining a new set of objects which are bound to names in the module’s dictionary".[^4] Example: [`reload`](#reload).

## Examples

The module code runs at the first import only (rules r1 and r2). The second `import counter` finds the module in `sys.modules` and does not run the file again.

```python norun reason=fragment file=counter.py group=once
print("loading counter")
count = 0


def bump():
    global count
    count += 1
    return count
```

```python run file=main.py group=once id=once
import counter
import counter

print(counter.__name__)
counter.bump()
counter.bump()
print(counter.count)
```

```output for=once
loading counter
counter
2
```

All imports of one name give one object, and the object is the value in `sys.modules` (rule r3). The name after `as` is only a second name for the same object.

```python norun reason=fragment file=settings.py group=cache
debug = False
```

```python run file=main.py group=cache id=cache
import sys

import settings
import settings as config

config.debug = True
print(settings is config)
print(sys.modules["settings"] is settings)
print(settings.debug)
```

```output for=cache
True
True
True
```

Two modules define the global `name`, and each function reads the global of its own module (rule r4). An assignment through the module object changes that namespace (rule r5), and `dir()` lists the names (rule r6).

```python norun reason=fragment file=north.py group=namespaces
name = "north"


def who():
    return name
```

```python norun reason=fragment file=south.py group=namespaces
name = "south"


def who():
    return name
```

```python run file=main.py group=namespaces id=namespaces
import north
import south

name = "main"
print(north.who(), south.who(), name)

south.name = "changed"
print(south.who(), south.__dict__["name"])
print([n for n in dir(north) if not n.startswith("__")])
```

```output for=namespaces
north south main
changed changed
['name', 'who']
```

`importlib.reload` runs the module code again in the same module object (rule r7).

```python norun reason=fragment file=greeting.py group=reload
print("running greeting.py")
text = "hello"
```

```python run file=main.py group=reload id=reload
import importlib

import greeting

old = greeting
greeting.text = "changed"
importlib.reload(greeting)
print(greeting is old, greeting.text)
```

```output for=reload
running greeting.py
running greeting.py
True hello
```

## Versions

The message of [`circular`](#circular) differs. CPython 3.12.3 says `cannot import name 'foo_var' from partially initialized module 'foo' (most likely due to a circular import)`. CPython 3.13.14 and 3.14.6 say `cannot import name 'foo_var' from 'foo'` and add a hint to rename `foo.py`, which does not apply here. The hint comes from a change in 3.13: "if a script has the same name as a third-party module that it attempts to import and this results in errors, we also display a more helpful error message".[^5] The other examples give the same output on the three versions.

## Pitfalls

### Circular imports with `from`

The FAQ shows two modules that import each other.[^6] `foo` starts to run and imports `bar`. Then `bar` asks `foo` for `foo_var`, but `foo` has not yet got to the line that sets it. "The last step fails, because Python isn’t done with interpreting foo yet and the global symbol dictionary for foo is still empty."[^6]

```python norun reason=fragment file=foo.py group=circular
from bar import bar_var

foo_var = 1
```

```python norun reason=fragment file=bar.py group=circular
from foo import foo_var

bar_var = 2
```

```python fails file=main.py group=circular id=circular expect=exception:ImportError
import foo
```

The FAQ says that "Circular imports are fine where both modules use the “import <module>” form of import. They fail when the 2nd module wants to grab a name out of the first (“from module import name”) and the import is at the top level."[^6] In the fixed version, `bar` keeps a reference to the module and reads the attribute later, when `foo` is complete.

```python norun reason=fragment file=foo.py group=circular-fixed
import bar

foo_var = 1


def total():
    return foo_var + bar.bar_var
```

```python norun reason=fragment file=bar.py group=circular-fixed
import foo

bar_var = 2


def read_foo():
    return foo.foo_var
```

```python run file=main.py group=circular-fixed id=circular-fixed
import bar
import foo

print(foo.total(), bar.read_foo())
```

```output for=circular-fixed
3 1
```

The FAQ also gives two other ways out: put the imports after the definitions that other modules need, or change the design "so that the recursive import is not necessary in the first place."[^6]

### An edited module does not change the program

An import of a module that is already in `sys.modules` does not read the file again (rule r7). In an interactive session, restart the interpreter or call `importlib.reload()`. A reload does not update other references: "Other references to the old objects (such as names external to the module) are not rebound to refer to the new objects".[^4] A name bound by `from greeting import text` keeps the old object.

## Comparison

**JavaScript** modules use `import` declarations, which "import read-only live bindings which are exported by another module."[^7] A JavaScript module also has a cycle problem: "With cyclic imports, an imported module may not have finished evaluating, so an imported binding may still be uninitialized."[^7]

**Go** runs the initialization of each package one time: "If multiple packages import a package, the imported package will be initialized only once. The importing of packages, by construction, guarantees that there can be no cyclic initialization dependencies."[^8] A Python import cycle is legal and fails only when a name is not ready.

**Rust** declares modules with `mod`. For `mod garden;` the compiler looks for the code "In the file src/garden.rs" or "In the file src/garden/mod.rs".[^9] In Python, the file itself is the module, and no declaration is necessary.

## Implementation notes

CPython stores the compiled bytecode of an imported module: "To speed up loading modules, Python caches the compiled version of each module in the __pycache__ directory under the name module.version.pyc".[^1] This makes only the load faster: "A program doesn’t run any faster when it is read from a .pyc file than when it is read from a .py file; the only thing that’s faster about .pyc files is the speed with which they are loaded."[^1]

## References
