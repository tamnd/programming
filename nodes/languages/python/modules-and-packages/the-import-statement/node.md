---
tag: N5Z7X8
kind: feature
owner: python
slug: the-import-statement
name: The import statement
aliases: [import, from import, import as, from module import *, ModuleNotFoundError, ImportError, wildcard import, star import]
area: languages/python/modules-and-packages
level: intro
summary: >-
  import finds and loads a module, then binds a name to it. from import binds names from the module, as picks another name, and from module import * binds all public names.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement", locator: "7.11. The import statement"}
see_also: [python/modules, python/the-module-search-path, python/packages, python/importing-from-a-package, python/intra-package-references, python/naming-and-binding]
sources:
- url: https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement
  title: The Python Language Reference, 7. Simple statements
  locator: 7.11. The import statement
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/tutorial/modules.html
  title: The Python Tutorial, 6. Modules
  locator: 6.1. More on Modules
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/import.html
  title: The Python Language Reference, 5. The import system
  locator: 5. The import system
  accessed: 2026-11-16
- url: https://peps.python.org/pep-0008/#imports
  title: "PEP 8: Style Guide for Python Code"
  locator: Imports
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/faq/programming.html
  title: Programming FAQ
  locator: What are the “best practices” for using import in a module?
  accessed: 2026-11-16
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 7. Packages and Modules
  locator: 7.3. Compilation Units; 7.5. Import Declarations
  accessed: 2026-11-16
- url: https://doc.rust-lang.org/book/ch07-04-bringing-paths-into-scope-with-the-use-keyword.html
  title: The Rust Programming Language, 7.4. Bringing Paths into Scope with the use Keyword
  locator: Providing New Names with the as Keyword; The Glob Operator
  accessed: 2026-11-16
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
  title: import, JavaScript reference, MDN
  locator: Description
  accessed: 2026-11-16
---

## Synopsis

```text
import module                     # bind module
import module as name             # bind name to module
import pkg.sub.mod                # load pkg, pkg.sub and pkg.sub.mod; bind pkg
import pkg.sub.mod as name        # bind name to pkg.sub.mod
from module import a, b as c      # bind a and c to attributes (or submodules)
from module import (a,
                    b)            # parentheses allow line breaks
from module import *              # bind all public names; module level only
```

## Semantics

**r1.** The basic statement has two steps: "find a module, loading and initializing it if necessary" and "define a name or names in the current namespace for the scope where the import statement occurs, just as an assignment statement would".[^1] An import in a function thus binds a local name. Example: [`local`](#local).

**r2.** The name that is bound depends on the form. With `as`, "the name following as is bound directly to the imported module."[^1] For a top-level module, its name is bound. For a submodule, "the name of the top level package that contains the module is bound in the local namespace as a reference to the top level package. The imported module must be accessed using its full qualified name".[^1] Examples: [`forms`](#forms), [`dotted`](#dotted).

**r3.** The `from` form looks for each name as an attribute of the module first. If the attribute is not there, Python tries "to import a submodule with that name and then check the imported module again for that attribute if the attribute is not found, ImportError is raised."[^1] The `from` form "does not introduce the module name from which the imports are taken in the local namespace".[^2] Examples: [`forms`](#forms), [`missing-name`](#missing-name).

**r4.** "If the named module cannot be found, a ModuleNotFoundError is raised."[^3] A failure can also come from the code of the module: failures in the first step "may indicate either that the module could not be located, or that an error occurred while initializing the module, which includes execution of the module’s code."[^1] Example: [`not-found`](#not-found).

**r5.** `from module import *` binds "all public names defined in the module".[^1] If the module defines `__all__`, the names in it are the public names, and they "are all considered public and are required to exist."[^1] Without `__all__`, the public names are all names "which do not begin with an underscore character ('_')."[^1] Example: [`star`](#star).

**r6.** The star form "is only allowed at the module level. Attempting to use it in class or function definitions will raise a SyntaxError."[^1] Example: [`star-in-function`](#star-in-function).

**r7.** A statement with commas is the same as one statement for each clause: "the two steps are carried out separately for each clause, just as though the clauses had been separated out into individual import statements."[^1]

## Examples

The forms of rules r2 and r3, with modules of the standard library.

```python run id=forms
import collections.abc as cabc
import math
from json import dumps, loads as parse

print(math.sqrt(16))
print(cabc.Sequence.__name__)
print(dumps([1, 2]), parse("[3]"))
print("json" in globals(), "loads" in globals())
```

```output for=forms
4.0
Sequence
[1, 2] [3]
False False
```

`import xml.etree.ElementTree` loads three modules and binds only `xml` (rule r2).

```python run id=dotted
import sys
import xml.etree.ElementTree

print("xml" in globals(), "etree" in globals(), "ElementTree" in globals())
print(xml.etree.ElementTree.__name__)
print("xml.etree" in sys.modules)
```

```output for=dotted
True False False
xml.etree.ElementTree
True
```

An import in a function binds a local name (rule r1). The module stays in `sys.modules`, but the global namespace does not get the name.

```python run id=local
import sys


def area(r):
    import math
    return math.pi * r * r


print(round(area(2), 2))
print("math" in globals(), "math" in sys.modules)
```

```output for=local
12.57
False True
```

A module that is not on the search path (rule r4).

```python fails id=not-found expect=exception:ModuleNotFoundError
import no_such_module_here
```

A name that the module does not have (rule r3).

```python fails id=missing-name expect=exception:ImportError
from math import square_root
```

The star form with and without `__all__` (rule r5). `shapes` lists one name, so `_scale` and `square` are not bound. `colors` has no `__all__`, so every name without a leading underscore is bound.

```python norun reason=fragment file=shapes.py group=star
__all__ = ["circle"]


def _scale(x):
    return 2 * x


def circle(r):
    return 3.14 * _scale(r)


def square(a):
    return a * a
```

```python norun reason=fragment file=colors.py group=star
RED = "#f00"
_SECRET = "#123"
```

```python run file=main.py group=star id=star
from colors import *
from shapes import *

print(sorted(n for n in globals() if not n.startswith("__")))
```

```output for=star
['RED', 'circle']
```

The star form in a function is a syntax error (rule r6).

```python fails id=star-in-function expect=exception:SyntaxError
def setup():
    from math import *
    return pi
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### `from module import name` copies the binding

The `from` form stores "a reference to that value" in the current namespace.[^1] If the module later binds the name to another object, the copy does not change.

```python norun reason=fragment file=config.py group=copy
level = 1


def set_level(n):
    global level
    level = n
```

```python wrong file=main.py group=copy id=copy
import config
from config import level, set_level

set_level(5)
print(level, config.level)
```

```output for=copy
1 5
```

Read the value through the module, `config.level`, when it can change.

### Wildcard imports hide where names come from

PEP 8 says: "Wildcard imports (from <module> import *) should be avoided, as they make it unclear which names are present in the namespace, confusing both readers and many automated tools."[^4] The FAQ adds that the form "clutters the importer’s namespace, and makes it much harder for linters to detect undefined names."[^5] A later star import can also replace a name that an earlier statement bound.

### Imports in many places

PEP 8 says: "Imports are always put at the top of the file, just after any module comments and docstrings, and before module globals and constants."[^4] An import in a function is correct code (rule r1), and the FAQ allows it for a reason: "Only move imports into a local scope, such as inside a function definition, if it’s necessary to solve a problem such as avoiding a circular import or are trying to reduce the initialization time of a module."[^5]

## Comparison

**Java** imports names: "An import declaration allows a named class, interface, or static member to be referred to by a simple name".[^6] "A type-import-on-demand declaration (§7.5.2) imports all the accessible classes and interfaces of a named package, class, or interface as needed".[^6] Every compilation unit "implicitly imports every public class or interface declared in the predefined package java.lang".[^6]

**Rust** brings paths into scope with `use`, and it has an `as` form: `use std::io::Result as IoResult;`.[^7] Its glob form has the same problem as the Python star form: "Glob can make it harder to tell what names are in scope and where a name used in your program was defined."[^7]

**JavaScript** imports are bindings that follow the exporter. The bindings "are called live bindings because they are updated by the module that exported the binding, but cannot be re-assigned by the importing module."[^8] A Python `from` import is a plain reference, as in [`copy`](#copy).

## Implementation notes

The statement calls the function `__import__()`: "The search operation of the import statement is defined as a call to the __import__() function, with the appropriate arguments. The return value of __import__() is used to perform the name binding operation of the import statement."[^3] For a module name that is known only at run time, the reference points to `importlib.import_module()`, which "is provided to support applications that determine dynamically the modules to be loaded."[^1]

## References
