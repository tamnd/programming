---
tag: 1DDM2N
kind: feature
owner: python
slug: importing-from-a-package
name: Importing from a package
aliases: [from package import *, __all__, star import from a package, from package import submodule, wildcard import]
area: languages/python/modules-and-packages
level: core
summary: >-
  from package import * imports the submodules listed in the __all__ of the __init__.py file. Without __all__, it binds only the names that the package already has.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/modules.html#importing-from-a-package", locator: "6.4.1. Importing * From a Package"}
see_also: [python/packages, python/the-import-statement, python/intra-package-references, python/modules]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html#importing-from-a-package
  title: The Python Tutorial, 6. Modules
  locator: 6.4. Packages; 6.4.1. Importing * From a Package
  accessed: 2026-11-17
- url: https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement
  title: The Python Language Reference, 7. Simple statements
  locator: 7.11. The import statement
  accessed: 2026-11-17
- url: https://peps.python.org/pep-0008/#public-and-internal-interfaces
  title: "PEP 8: Style Guide for Python Code"
  locator: Imports; Public and Internal Interfaces
  accessed: 2026-11-17
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 7. Packages and Modules
  locator: 7.5.2. Type-Import-on-Demand Declarations
  accessed: 2026-11-17
- url: https://doc.rust-lang.org/book/ch07-04-bringing-paths-into-scope-with-the-use-keyword.html
  title: The Rust Programming Language, 7.4. Bringing Paths into Scope with the use Keyword
  locator: The Glob Operator
  accessed: 2026-11-17
---

## Synopsis

```text
# effects/__init__.py
__all__ = ["echo", "surround"]     # what from effects import * loads

from effects import *              # imports effects.echo and effects.surround
from effects import echo           # one submodule: the recommended form
from effects.echo import echofilter
```

## Semantics

**r1.** `from package import item` accepts a submodule or any other name: "the item can be either a submodule (or subpackage) of the package, or some other name defined in the package, like a function, class or variable. The import statement first tests whether the item is defined in the package; if not, it assumes it is a module and attempts to load it. If it fails to find it, an ImportError exception is raised."[^1] Example: [`specific`](#specific).

**r2.** The star form does not search the directory for submodules. The tutorial explains why: "This could take a long time and importing sub-modules might have unwanted side-effects that should only happen when the sub-module is explicitly imported."[^1]

**r3.** The package gives the list in `__all__`: "if a package’s __init__.py code defines a list named __all__, it is taken to be the list of module names that should be imported when from package import * is encountered."[^1] The reference adds that "The names given in __all__ are all considered public and are required to exist."[^2] Example: [`with-all`](#with-all).

**r4.** "If __all__ is not defined, the statement from sound.effects import * does not import all submodules from the package sound.effects into the current namespace; it only ensures that the package sound.effects has been imported (possibly running any initialization code in __init__.py) and then imports whatever names are defined in the package."[^1] That includes "any submodules of the package that were explicitly loaded by previous import statements."[^1] Example: [`without-all`](#without-all).

**r5.** A name in the `__init__.py` file wins over a submodule with the same name. "Be aware that submodules might become shadowed by locally defined names."[^1] Example: [`shadowed`](#shadowed).

**r6.** The tutorial advises against the star form: "it is still considered bad practice in production code. Remember, there is nothing wrong with using from package import specific_submodule! In fact, this is the recommended notation unless the importing module needs to use submodules with the same name from different packages."[^1]

## Examples

`__all__` names two of the three submodules, so the star import loads those two (rule r3). `reverse` does not load.

```python norun reason=fragment file=effects/__init__.py group=with-all
__all__ = ["echo", "surround"]
```

```python norun reason=fragment file=effects/echo.py group=with-all
print("load echo")
```

```python norun reason=fragment file=effects/surround.py group=with-all
print("load surround")
```

```python norun reason=fragment file=effects/reverse.py group=with-all
print("load reverse")
```

```python run file=main.py group=with-all id=with-all
from effects import *

print(sorted(n for n in globals() if not n.startswith("__")))
```

```output for=with-all
load echo
load surround
['echo', 'surround']
```

Without `__all__`, the star import binds the names of `__init__.py` that do not start with an underscore, and the submodules that are already loaded (rule r4). Here `echo` was loaded before, and `surround` was not.

```python norun reason=fragment file=effects/__init__.py group=without-all
VERSION = "1.0"
_cache = {}
```

```python norun reason=fragment file=effects/echo.py group=without-all
print("load echo")
```

```python norun reason=fragment file=effects/surround.py group=without-all
print("load surround")
```

```python run file=main.py group=without-all id=without-all
import effects.echo
from effects import *

print(sorted(n for n in globals() if not n.startswith("__")))
```

```output for=without-all
load echo
['VERSION', 'echo', 'effects']
```

The case from the tutorial: `__init__.py` defines a function `reverse`, so the star import binds the function, and the submodule `reverse` does not load (rule r5).

```python norun reason=fragment file=effects/__init__.py group=shadowed
__all__ = ["echo", "reverse"]


def reverse(msg):
    return msg[::-1]
```

```python norun reason=fragment file=effects/echo.py group=shadowed
print("load echo")
```

```python norun reason=fragment file=effects/reverse.py group=shadowed
print("load reverse")
```

```python run file=main.py group=shadowed id=shadowed
import sys

from effects import *

print(reverse("abc"))
print("effects.reverse" in sys.modules)
```

```output for=shadowed
load echo
cba
False
```

The recommended form names the submodule or the function (rules r1 and r6).

```python norun reason=fragment file=effects/__init__.py group=specific
"""Sound effects."""
```

```python norun reason=fragment file=effects/echo.py group=specific
def echofilter(text):
    return text + " " + text[-2:]
```

```python run file=main.py group=specific id=specific
from effects import echo
from effects.echo import echofilter

print(echo.echofilter("hello"))
print(echofilter is echo.echofilter)
```

```output for=specific
hello lo
True
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### A name in `__all__` that does not exist

The names in `__all__` "are required to exist" (rule r3). A name that is neither defined in `__init__.py` nor a submodule makes the star import fail.

```python norun reason=fragment file=effects/__init__.py group=missing
__all__ = ["echo", "flanger"]
```

```python norun reason=fragment file=effects/echo.py group=missing
print("load echo")
```

```python fails file=main.py group=missing id=missing expect=exception:AttributeError
from effects import *
```

The tutorial says: "It is up to the package author to keep this list up-to-date when a new version of the package is released."[^1]

### Expecting the star form to load all submodules

Without `__all__`, a submodule that nothing has imported yet is not bound (rule r4). The result then depends on what other code imported before. Name the submodules you use.

### `__all__` and the public interface

PEP 8 connects `__all__` with the public interface: "To better support introspection, modules should explicitly declare the names in their public API using the __all__ attribute."[^3] A star import is still not a good way to use that interface: "Wildcard imports (from <module> import *) should be avoided, as they make it unclear which names are present in the namespace, confusing both readers and many automated tools."[^3]

## Comparison

**Java** has an on-demand import, `import PackageOrTypeName . * ;`, that "allows all accessible classes and interfaces of a named package, class, or interface to be imported as needed."[^4] The Python star form instead runs the code of each submodule in `__all__` when the statement executes, as in [`with-all`](#with-all).

**Rust** has the glob form `use std::collections::*;`, which "brings all public items defined in std::collections into the current scope."[^5] The book gives the same warning as the Python tutorial: "Glob can make it harder to tell what names are in scope and where a name used in your program was defined."[^5]

## Implementation notes

The `from` form of the import statement checks for an attribute first and then tries to import a submodule with that name (rule r1).[^2] The star form follows the same order for the names in `__all__`. In [`shadowed`](#shadowed), the attribute `reverse` exists, so CPython never loads the submodule `effects.reverse`.

## References
