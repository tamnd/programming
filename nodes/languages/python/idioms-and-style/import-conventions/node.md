---
tag: CZC8E3
kind: feature
owner: python
slug: import-conventions
name: Import conventions
aliases: [import order, import groups, isort, wildcard import, star import, import alias, import numpy as np, circular import, local import]
area: languages/python/idioms-and-style
level: intro
summary: >-
  PEP 8 puts imports at the top of the file, one module per line, in three groups: the standard library, third-party packages and local code. It asks for absolute imports and no wildcard imports.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0008/#imports", locator: "PEP 8, Imports"}
proposals: [pep-8, pep-328, pep-810]
see_also: [python/coding-style, python/naming-conventions, python/the-import-statement, python/importing-from-a-package, python/intra-package-references, python/modules, python/the-module-search-path]
sources:
  - url: https://peps.python.org/pep-0008/#imports
    title: PEP 8, Style Guide for Python Code
    locator: Imports
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/faq/programming.html#what-are-the-best-practices-for-using-import-in-a-module
    title: Programming FAQ, Python 3.15 documentation
    locator: What are the best practices for using import in a module?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/unsorted-imports/
    title: unsorted-imports (I001), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement
    title: The Python Language Reference, Python 3.15 documentation
    locator: 7.11 The import statement; 7.11.1 Future statements
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/tutorial/modules.html#more-on-modules
    title: The Python Tutorial, Python 3.15 documentation
    locator: 6.1 More on Modules; 6.4.1 Importing * From a Package
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/undefined-local-with-import-star/
    title: undefined-local-with-import-star (F403), Ruff documentation
    locator: Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/unconventional-import-alias/
    title: unconventional-import-alias (ICN001), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/settings/#lint_flake8-import-conventions_aliases
    title: Settings, Ruff documentation
    locator: lint.flake8-import-conventions.aliases
    accessed: 2026-11-18
  - url: https://numpydoc.readthedocs.io/en/latest/format.html#import-conventions
    title: Style guide, numpydoc
    locator: Import conventions
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0328/
    title: PEP 328, Imports, Multi-Line and Absolute/Relative
    locator: Rationale for Absolute Imports
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/relative-imports/
    title: relative-imports (TID252), Ruff documentation
    locator: What it does
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/module-import-not-at-top-of-file/
    title: module-import-not-at-top-of-file (E402), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://google.github.io/styleguide/pyguide.html
    title: Google Python Style Guide
    locator: 2.2 Imports; 3.13 Imports formatting
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/sys.html#sys.stdlib_module_names
    title: sys, System-specific parameters and functions, Python 3.15 documentation
    locator: sys.stdlib_module_names
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.15.html
    title: What’s New In Python 3.15
    locator: "PEP 810: Explicit lazy imports"
    accessed: 2026-11-18
---

## Synopsis

```python run id=grouped
"""Report the size of the files in a folder."""

import os
import sys
from pathlib import Path

LIMIT = 1024


def report(folder):
    return sorted(p.name for p in Path(folder).iterdir() if p.is_file())


print(type(report(os.curdir)).__name__, "os" in sys.modules, LIMIT)
```

```output for=grouped
list True 1024
```

The docstring comes first, then the imports, then the constants and the definitions.

## Semantics

**r1.** Place: "Imports are always put at the top of the file, just after any module comments and docstrings, and before module globals and constants."[^1] The FAQ gives the reason: "Doing so makes it clear what other modules your code requires and avoids questions of whether the module name is in scope."[^2] Example: [`grouped`](#grouped).

**r2.** One module per line: "Imports should usually be on separate lines".[^1] Write `import os` and `import sys` on two lines, not `import os, sys`. Several names from one module can share a line, as in `from subprocess import Popen, PIPE`.[^1]

**r3.** Groups: "Imports should be grouped in the following order: Standard library imports. Related third party imports. Local application/library specific imports. You should put a blank line between each group of imports."[^1] Ruff and isort add a group for `__future__` at the start, and a group for relative imports at the end, so the order is "future", "standard-library", "third-party", "first-party" and "local-folder".[^3] Within a group, Ruff sorts by module name.[^3] Example: [`sorted-groups`](#sorted-groups).

**r4.** A future statement is special. "A future statement must appear near the top of the module."[^4] Only the docstring, comments, blank lines and other future statements can come before it.[^4]

**r5.** Wildcard imports: "Wildcard imports (from <module> import *) should be avoided, as they make it unclear which names are present in the namespace, confusing both readers and many automated tools."[^1] The tutorial says that the practice "is frowned upon, since it often causes poorly readable code. However, it is okay to use it to save typing in interactive sessions."[^5] The language allows the wildcard form only at the top level of a module: it "is only allowed at the module level. Attempting to use it in class or function definitions will raise a SyntaxError."[^4] Ruff checks for wildcard imports with F403.[^6] Examples: [`star-shadows`](#star-shadows), [`star-in-function`](#star-in-function).

**r6.** Aliases: some packages have a short name that all users expect. Ruff checks "for imports that are typically imported using a common convention, like import pandas as pd, and enforces that convention."[^7] Its default list includes `numpy` as `np`, `pandas` as `pd`, `matplotlib.pyplot` as `plt` and `xml.etree.ElementTree` as `ET`.[^8] The numpydoc guide uses `import numpy as np` and says "Do not abbreviate scipy."[^9] Do not invent an alias for a module that has no common one. Example: [`alias`](#alias).

**r7.** Absolute imports: "Absolute imports are recommended, as they are usually more readable and tend to be better behaved".[^1] "However, explicit relative imports are an acceptable alternative to absolute imports, especially when dealing with complex package layouts".[^1] PEP 328 made absolute imports the default because "they’re the more common use case".[^10] Ruff checks for relative imports with TID252.[^11]

**r8.** Imports inside a function: the FAQ says to "Only move imports into a local scope, such as inside a function definition, if it’s necessary to solve a problem such as avoiding a circular import or are trying to reduce the initialization time of a module."[^2] Ruff checks for an import that is not at the top of the file with E402.[^12] Examples: [`circular`](#circular), [`module-form`](#module-form).

**r9.** The Google style guide is stricter: "Use import statements for packages and modules only, not for individual types, classes, or functions."[^13] Then each use of a name shows the module that it comes from.

## Examples

```python run id=sorted-groups
import sys

FIRST_PARTY = {"shop"}
names = ["requests", "shop.cart", "os", "json", "numpy", "shop", "collections.abc"]


def group(name):
    top = name.partition(".")[0]
    if top in sys.stdlib_module_names:
        return 1
    if top in FIRST_PARTY:
        return 3
    return 2


for name in sorted(names, key=lambda n: (group(n), n)):
    print(group(name), name)
```

```output for=sorted-groups
1 collections.abc
1 json
1 os
2 numpy
2 requests
3 shop
3 shop.cart
```

This sort is what a tool such as isort does (rule r3). `sys.stdlib_module_names` is "A frozenset of strings containing the names of standard library modules."[^14] A tool cannot know which other packages are third-party and which are part of the project, so it reads that from the project settings.

```python run id=alias
import xml.etree.ElementTree as ET

root = ET.fromstring("<menu><item>pho</item><item>rice</item></menu>")
print([item.text for item in root.iter("item")])
```

```output for=alias
['pho', 'rice']
```

`ET` is the common alias for `xml.etree.ElementTree` (rule r6). A reader who knows the alias knows the module.

```python run id=module-form
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "shop.py").write_text(
        "from tax import rate\n"
        "def price(amount):\n"
        "    return round(amount * (1 + rate()), 2)\n"
    )
    pathlib.Path(folder, "tax.py").write_text(
        "import shop\n"
        "def rate():\n"
        "    return 0.1\n"
        "def total(amounts):\n"
        "    return sum(shop.price(a) for a in amounts)\n"
    )
    sys.path.insert(0, folder)
    import shop
    import tax

    print(shop.price(100), tax.total([10, 20]))
```

```output for=module-form
110.0 33.0
```

The two modules import each other. `tax` uses the form `import shop` and reads `shop.price` only when `total` runs, so the cycle works. The FAQ says: "Circular imports are fine where both modules use the “import <module>” form of import."[^2]

## Versions

> [!since v="3.15"]
> The soft keyword `lazy` marks an import as lazy, as in `lazy import json`. "When you mark an import as lazy, Python defers the actual module loading until the imported name is first used. This gives you the organizational benefits of declaring all imports at the top of the file while only paying the loading cost for modules you actually use."[^15] So fewer imports need to move into a function to make startup fast (rule r8). "Neither star imports nor future imports can be lazy".[^15]

## Pitfalls

```python wrong id=star-shadows
print(pow(2, 10))

from math import *

print(pow(2, 10))
```

```output for=star-shadows
1024
1024.0
```

`math` has its own `pow`, and the wildcard import replaced the built-in `pow` with it. Nothing in the second call shows that the name changed (rule r5). Write `import math` and call `math.pow`, or import only the names that you need.

```python fails id=star-in-function expect=exception:SyntaxError
source = "def area(r):\n    from math import *\n    return pi * r ** 2\n"
compile(source, "area.py", "exec")
```

A wildcard import inside a function is a syntax error, because the compiler must know all the local names of a function (rule r5). Write `from math import pi` in the function, or import at the top of the module.

```python fails id=circular expect=exception:ImportError
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as folder:
    pathlib.Path(folder, "shop.py").write_text(
        "from tax import rate\n"
        "def price(amount):\n"
        "    return amount * (1 + rate())\n"
    )
    pathlib.Path(folder, "tax.py").write_text(
        "from shop import price\n"
        "def rate():\n"
        "    return 0.1\n"
    )
    sys.path.insert(0, folder)
    import shop
```

`shop` starts to run and imports `tax`. Then `tax` asks for the name `price` from `shop`, but `shop` has not defined it yet. Circular imports "fail when the 2nd module wants to grab a name out of the first (“from module import name”) and the import is at the top level."[^2] Use the form `import shop` in one of the modules, as in [`module-form`](#module-form), or move the shared code into a third module.

## Comparison

The import rules of PEP 8 are style. The interpreter accepts imports in any order and in any place, except the wildcard form and the future statement (rules r4 and r5). Linters such as Ruff check the style, and some of them can sort the imports. For the import statement itself, see [[python/the-import-statement]]. For packages and relative imports, see [[python/importing-from-a-package]] and [[python/intra-package-references]].
