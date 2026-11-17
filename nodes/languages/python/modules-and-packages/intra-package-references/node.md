---
tag: R1KXZ5
kind: feature
owner: python
slug: intra-package-references
name: Intra-package references
aliases: [relative import, absolute import, from . import, from .. import, __package__, attempted relative import with no known parent package, PEP 328, PEP 366]
area: languages/python/modules-and-packages
level: core
summary: >-
  A module in a package can import its siblings with an absolute name or a relative name that starts with dots. Relative imports use the package of the module, so they fail in a file run as a script.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html#package-relative-imports", locator: "5.7. Package Relative Imports"}
see_also: [python/packages, python/importing-from-a-package, python/the-import-statement, python/executing-modules-as-scripts, python/the-module-search-path]
sources:
- url: https://docs.python.org/3.15/tutorial/modules.html#intra-package-references
  title: The Python Tutorial, 6. Modules
  locator: 6.4.2. Intra-package References
  accessed: 2026-11-17
- url: https://docs.python.org/3.15/reference/import.html#package-relative-imports
  title: The Python Language Reference, 5. The import system
  locator: 5.7. Package Relative Imports
  accessed: 2026-11-17
- url: https://docs.python.org/3.15/reference/simple_stmts.html#the-import-statement
  title: The Python Language Reference, 7. Simple statements
  locator: 7.11. The import statement
  accessed: 2026-11-17
- url: https://docs.python.org/3.15/reference/datamodel.html#module.__package__
  title: The Python Language Reference, 3. Data model
  locator: 3.2.9.1. Import-related attributes on module objects, module.__package__
  accessed: 2026-11-17
- url: https://peps.python.org/pep-0366/
  title: "PEP 366: Main module explicit relative imports"
  locator: Abstract; Proposed Change
  accessed: 2026-11-17
- url: https://peps.python.org/pep-0008/#imports
  title: "PEP 8: Style Guide for Python Code"
  locator: Imports
  accessed: 2026-11-17
- url: https://peps.python.org/pep-0338/
  title: "PEP 338: Executing modules as scripts"
  locator: Import Statements and the Main Module
  accessed: 2026-11-17
- url: https://doc.rust-lang.org/book/ch07-03-paths-for-referring-to-an-item-in-the-module-tree.html
  title: The Rust Programming Language, 7.3. Paths for Referring to an Item in the Module Tree
  locator: the introduction; Starting Relative Paths with super
  accessed: 2026-11-17
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/import
  title: import, JavaScript reference, MDN
  locator: Module specifier resolution
  accessed: 2026-11-17
- url: https://peps.python.org/pep-0328/
  title: "PEP 328: Imports: Multi-Line and Absolute/Relative"
  locator: Rationale for Relative Imports; Guido’s Decision
  accessed: 2026-11-17
---

## Synopsis

```text
# in sound/effects/surround.py (the package sound.effects)
from sound.effects import echo     # absolute
from . import echo                 # relative: the current package, sound.effects
from .echo import echofilter       # a name from a sibling module
from .. import formats             # the parent package, sound
from ..filters import equalizer    # sound.filters.equalizer

import .echo                       # SyntaxError: only the from form is relative

$ python -m sound.effects.surround   # relative imports work
$ python sound/effects/surround.py   # ImportError: no known parent package
```

## Semantics

**r1.** A module can name a sibling with its full name: "you can use absolute imports to refer to submodules of siblings packages. For example, if the module sound.filters.vocoder needs to use the echo module in the sound.effects package, it can use from sound.effects import echo."[^1] Example: [`absolute-and-relative`](#absolute-and-relative).

**r2.** "Relative imports use leading dots. A single leading dot indicates a relative import, starting with the current package. Two or more leading dots indicate a relative import to the parent(s) of the current package, one level per dot after the first."[^2] "So if you execute from . import mod from a module in the pkg package then you will end up importing pkg.mod. If you execute from ..subpkg2 import mod from within pkg.subpkg1 you will import pkg.subpkg2.mod."[^3] Example: [`absolute-and-relative`](#absolute-and-relative).

**r3.** Only the `from` form can be relative: "relative imports may only use the second form; the reason for this is that: import XXX.YYY.ZZZ should expose XXX.YYY.ZZZ as a usable expression, but .moduleY is not a valid expression."[^2] Example: [`import-dot`](#import-dot).

**r4.** The dots count from the package of the module. "Note that relative imports are based on the name of the current module’s package."[^1] The attribute `__package__` holds that name: "This attribute is used instead of __name__ to calculate explicit relative imports for main modules."[^4] Example: [`package-name`](#package-name).

**r5.** A file run as a script has no package. "Since the main module does not have a package, modules intended for use as the main module of a Python application must always use absolute imports."[^1] PEP 366 made relative imports work in a main module "if the module is executed using the -m switch."[^5] Example: [`as-script`](#as-script).

**r6.** PEP 8 prefers absolute imports: "Absolute imports are recommended, as they are usually more readable and tend to be better behaved (or at least give better error messages) if the import system is incorrectly configured".[^6] It also says: "explicit relative imports are an acceptable alternative to absolute imports, especially when dealing with complex package layouts where using absolute imports would be unnecessarily verbose".[^6]

## Examples

The layout of the tutorial, with absolute and relative imports in `surround.py` (rules r1 and r2). Both forms give the same module objects.

```python norun reason=fragment file=sound/__init__.py group=absolute-and-relative
"""The sound package."""
```

```python norun reason=fragment file=sound/formats/__init__.py group=absolute-and-relative
"""File formats."""
```

```python norun reason=fragment file=sound/filters/__init__.py group=absolute-and-relative
"""Filters."""
```

```python norun reason=fragment file=sound/filters/equalizer.py group=absolute-and-relative
NAME = "equalizer"
```

```python norun reason=fragment file=sound/effects/__init__.py group=absolute-and-relative
"""Sound effects."""
```

```python norun reason=fragment file=sound/effects/echo.py group=absolute-and-relative
NAME = "echo"
```

```python norun reason=fragment file=sound/effects/surround.py group=absolute-and-relative
import sound.effects.echo as absolute_echo

from . import echo
from .. import formats
from ..filters import equalizer

print(echo is absolute_echo)
print(echo.__name__, formats.__name__, equalizer.__name__)
```

```python run file=main.py group=absolute-and-relative id=absolute-and-relative
import sound.effects.surround
```

```output for=absolute-and-relative
True
sound.effects.echo sound.formats sound.filters.equalizer
```

`__package__` is the base of the relative names (rule r4). For a package, it is the name of the package itself, so `from . import x` in `__init__.py` finds a submodule of that package.

```python norun reason=fragment file=shop/__init__.py group=package-name
from . import cart

print("shop:", __name__, __package__)
```

```python norun reason=fragment file=shop/cart.py group=package-name
print("cart:", __name__, __package__)
```

```python run file=main.py group=package-name id=package-name
import shop

print("main:", __name__, repr(__package__))
```

```output for=package-name
cart: shop.cart shop
shop: shop shop
main: __main__ None
```

The same file as a module of the package and as a script (rule r5). With `-m` the relative import works. With the path of the file, it fails.

```python norun reason=fragment file=shop/__init__.py group=as-script
"""The shop package."""
```

```python norun reason=fragment file=shop/prices.py group=as-script
PRICE = 3
```

```python norun reason=fragment file=shop/cart.py group=as-script
from .prices import PRICE

print("total:", 2 * PRICE)
```

```python run file=main.py group=as-script id=as-script
import subprocess
import sys

for args in (["-m", "shop.cart"], ["shop/cart.py"]):
    child = subprocess.run([sys.executable, *args], capture_output=True, text=True)
    result = child.stdout or child.stderr.splitlines()[-1]
    print(args[-1], "->", result.strip())
```

```output for=as-script
shop.cart -> total: 6
shop/cart.py -> ImportError: attempted relative import with no known parent package
```

`import .name` is not valid syntax (rule r3).

```python fails id=import-dot expect=exception:SyntaxError
import .helpers
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6.

## Pitfalls

### A script inside the package

Rule r5 has a second effect. `python shop/cart.py` puts `shop/` on `sys.path`, so an absolute `import prices` would work there and fail when the module is imported as `shop.cart`. PEP 338 describes this problem: "the package containing the executed module is added to sys.path, so its sibling modules are actually imported as top level modules."[^7] Run modules of a package with `python -m package.module` from the directory that holds the package.

### Too many dots

Each dot after the first goes up one level (rule r2). A relative import that goes above the top package fails with `ImportError`.

```python norun reason=fragment file=shop/__init__.py group=too-many-dots
"""The shop package."""
```

```python norun reason=fragment file=shop/cart.py group=too-many-dots
from .. import settings
```

```python fails file=main.py group=too-many-dots id=too-many-dots expect=exception:ImportError
import shop.cart
```

The top package cannot refer to the directory above it. Put `settings` in the package, or import it with an absolute name.

## Comparison

**Rust** paths have the same two kinds. "An absolute path is the full path starting from a crate root" and "A relative path starts from the current module and uses self, super, or an identifier in the current module."[^8] `super` is like `..`: "This is like starting a filesystem path with the .. syntax that means to go to the parent directory."[^8]

**JavaScript** relative specifiers name files, not packages. Specifiers "that start with /, ./, or ../" are "resolved relative to the current module URL."[^9] A Python relative import uses the package name in `__package__`, not the location of the file, which is why the script run in [`as-script`](#as-script) fails.

## Implementation notes

PEP 328 gives the reasons for relative imports: "Several use cases were presented, the most important of which is being able to rearrange the structure of large packages without having to edit sub-packages. In addition, a module inside a package can’t easily import itself without relative imports."[^10] PEP 366 added `__package__`: "When it is present, relative imports will be based on this attribute rather than the module __name__ attribute."[^5]

## References
