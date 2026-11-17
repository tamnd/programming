---
tag: X9CATP
kind: feature
owner: python
slug: packages
name: Packages
aliases: [package, __init__.py, regular package, subpackage, submodule, dotted module names, __path__]
area: languages/python/modules-and-packages
level: core
summary: >-
  A package is a module that holds other modules. A regular package is a directory with an __init__.py file, and its submodules have dotted names such as sound.effects.echo.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html#packages", locator: "5.2. Packages; 5.2.1. Regular packages; 5.4.2. Submodules"}
see_also: [python/modules, python/the-import-statement, python/importing-from-a-package, python/intra-package-references, python/the-module-search-path, python/project-layout]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html#packages
  title: The Python Tutorial, 6. Modules
  locator: 6.4. Packages
  accessed: 2026-11-17
- url: https://docs.python.org/3.15/reference/import.html#packages
  title: The Python Language Reference, 5. The import system
  locator: 5.2. Packages; 5.2.1. Regular packages; 5.3.1. The module cache; 5.4.2. Submodules
  accessed: 2026-11-17
- url: https://docs.python.org/3.15/reference/datamodel.html#module.__path__
  title: The Python Language Reference, 3. Data model
  locator: 3.2.9.1. Import-related attributes on module objects, module.__path__ and module.__package__
  accessed: 2026-11-17
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 7. Packages and Modules
  locator: 7.1. Package Members
  accessed: 2026-11-17
- url: https://doc.rust-lang.org/book/ch07-02-defining-modules-to-control-scope-and-privacy.html
  title: The Rust Programming Language, 7.2. Defining Modules to Control Scope and Privacy
  locator: Modules Cheat Sheet
  accessed: 2026-11-17
---

## Synopsis

```text
sound/                  # the package sound
    __init__.py         # runs when sound is imported
    effects/            # the subpackage sound.effects
        __init__.py
        echo.py         # the module sound.effects.echo

import sound.effects.echo               # runs sound/__init__.py, then effects/__init__.py, then echo.py
sound.effects.echo.echofilter(...)      # use the full name
from sound.effects import echo          # bind echo
from sound.effects.echo import echofilter
```

## Semantics

**r1.** "Packages are a way of structuring Python’s module namespace by using “dotted module names”. For example, the module name A.B designates a submodule named B in a package named A."[^1] A package is a module: "all packages are modules, but not all modules are packages. Or put another way, packages are just a special kind of module. Specifically, any module that contains a __path__ attribute is considered a package."[^2] Example: [`attributes`](#attributes).

**r2.** "A regular package is typically implemented as a directory containing an __init__.py file. When a regular package is imported, this __init__.py file is implicitly executed, and the objects it defines are bound to names in the package’s namespace."[^2] An import of a submodule runs the files of all the packages above it: "Importing parent.one will implicitly execute parent/__init__.py and parent/one/__init__.py."[^2] Example: [`init-runs`](#init-runs).

**r3.** The `__init__.py` file decides what is a package. "The __init__.py files are required to make Python treat directories containing the file as packages (unless using a namespace package, a relatively advanced feature). This prevents directories with a common name, such as string, from unintentionally hiding valid modules that occur later on the module search path."[^1] "A subdirectory inside a regular package that does not contain an __init__.py file is treated as an implicit namespace package (a “namespace subpackage”) rooted in that parent."[^2] Example: [`no-init`](#no-init).

**r4.** After `import sound.effects.echo`, the submodule "must be referenced with its full name."[^1] With `import item.subitem.subsubitem`, "each item except for the last must be a package; the last item can be a module or a package but can’t be a class or function or variable defined in the previous item."[^1] Example: [`not-a-package`](#not-a-package).

**r5.** An import of a package does not import its submodules. When a submodule is loaded, Python binds it in the parent: "after importing spam.foo, spam will have an attribute foo which is bound to the submodule."[^2] `sys.modules` holds every level: "if foo.bar.baz was previously imported, sys.modules will contain entries for foo, foo.bar, and foo.bar.baz."[^2] Example: [`attributes`](#attributes).

**r6.** `__path__` is "A (possibly empty) sequence of strings enumerating the locations where the package’s submodules will be found. Non-package modules should not have a __path__ attribute."[^3] `__package__` is "the name of the module’s package (which can be equal to module.__name__ if the module itself is a package)."[^3] Example: [`attributes`](#attributes).

**r7.** An `__init__.py` file can import from its submodules to give the package a short public interface. The reference shows `from .foo import Foo` in `spam/__init__.py`, which "puts name bindings for foo and Foo in the spam module".[^2] Example: [`facade`](#facade).

## Examples

Each `__init__.py` runs once, before the module below it (rule r2). The second import runs nothing.

```python norun reason=fragment file=sound/__init__.py group=init-runs
print("init sound")
```

```python norun reason=fragment file=sound/effects/__init__.py group=init-runs
print("init sound.effects")
```

```python norun reason=fragment file=sound/effects/echo.py group=init-runs
print("load sound.effects.echo")


def echofilter(text):
    return text + " " + text[-2:]
```

```python run file=main.py group=init-runs id=init-runs
import sound.effects.echo
import sound.effects.echo

print(sound.effects.echo.echofilter("hello"))
```

```output for=init-runs
init sound
init sound.effects
load sound.effects.echo
hello lo
```

`import sound` does not load `sound.effects` (rule r5). After the submodule is loaded, it is an attribute of its parent, and `sys.modules` has all three names. Only the packages have `__path__` (rules r1 and r6).

```python norun reason=fragment file=sound/__init__.py group=attributes
"""The sound package."""
```

```python norun reason=fragment file=sound/effects/__init__.py group=attributes
"""Sound effects."""
```

```python norun reason=fragment file=sound/effects/echo.py group=attributes
def echofilter(text):
    return text + " " + text[-2:]
```

```python run file=main.py group=attributes id=attributes
import os
import sys

import sound

try:
    sound.effects
except AttributeError:
    print("sound.effects is not loaded")

from sound.effects import echo

print(sound.effects.echo is echo)
print(echo.__name__, echo.__package__)
print(hasattr(sound, "__path__"), hasattr(echo, "__path__"))
print([os.path.relpath(p) for p in sound.effects.__path__])
print(sorted(n for n in sys.modules if n.startswith("sound")))
```

```output for=attributes
sound.effects is not loaded
True
sound.effects.echo sound.effects
True False
['sound/effects']
['sound', 'sound.effects', 'sound.effects.echo']
```

`sound/effects/__init__.py` imports `echofilter` from its submodule, so users can call `sound.effects.echofilter` (rule r7). The import also binds `echo` in the package.

```python norun reason=fragment file=sound/__init__.py group=facade
"""The sound package."""
```

```python norun reason=fragment file=sound/effects/__init__.py group=facade
from .echo import echofilter
```

```python norun reason=fragment file=sound/effects/echo.py group=facade
def echofilter(text):
    return text + " " + text[-2:]
```

```python run file=main.py group=facade id=facade
import sound.effects

print(sound.effects.echofilter("hi"))
print(sound.effects.echo.__name__)
```

```output for=facade
hi hi
sound.effects.echo
```

The last part of a dotted `import` must be a module, not a function (rule r4).

```python norun reason=fragment file=sound/__init__.py group=not-a-package
"""The sound package."""
```

```python norun reason=fragment file=sound/echo.py group=not-a-package
def echofilter(text):
    return text + " " + text[-2:]
```

```python fails file=main.py group=not-a-package id=not-a-package expect=exception:ModuleNotFoundError
import sound.echo.echofilter
```

Use `from sound.echo import echofilter` to bind the function.

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### A forgotten `__init__.py`

A subdirectory without `__init__.py` does not cause an error (rule r3). It becomes a namespace package, which has no `__init__.py` code to run and no `__file__`.

```python norun reason=fragment file=sound/__init__.py group=no-init
"""The sound package."""
```

```python norun reason=fragment file=sound/filters/karaoke.py group=no-init
NAME = "karaoke"
```

```python wrong file=main.py group=no-init id=no-init
import sound.filters.karaoke

print(sound.filters.karaoke.NAME)
print(sound.filters.__file__)
```

```output for=no-init
karaoke
None
```

Add `sound/filters/__init__.py` when the directory is meant to be a regular package.

### Slow or noisy code in `__init__.py`

The `__init__.py` files of all parent packages run before any submodule (rule r2). A slow import or a print in `sound/__init__.py` affects every user of every submodule. Keep `__init__.py` short.

### `import package` and then `package.submodule`

`import sound` followed by `sound.effects` fails with `AttributeError` unless some code loaded `sound.effects` before (rule r5), as in [`attributes`](#attributes). Import the submodule that you use, or import it in `__init__.py` as in [`facade`](#facade).

## Comparison

**Java** packages also have dotted names, but the hierarchy gives no access rights: "there is no special access relationship between a package named oliver and another package named oliver.twist".[^4] In Python, an import of `sound.effects` always runs `sound/__init__.py` first (rule r2).

**Rust** builds a tree of modules from `mod` declarations in the crate. For a submodule `mod vegetables;` declared in `src/garden.rs`, the compiler looks "In the file src/garden/vegetables.rs" or "In the file src/garden/vegetables/mod.rs".[^5] Python finds subpackages from the directories alone.

## Implementation notes

"Python has only one type of module object, and all modules are of this type, regardless of whether the module is implemented in Python, C, or something else."[^2] A package is such a module with a `__path__`. The reference warns against a literal reading of the directory picture: "don’t take this analogy too literally since packages and modules need not originate from the file system."[^2]

## References
