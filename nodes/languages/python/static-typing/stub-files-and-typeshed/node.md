---
tag: BSYXQF
kind: feature
owner: python
slug: stub-files-and-typeshed
name: Stub files and typeshed
aliases: [stub file, type stub, .pyi, pyi file, typeshed, py.typed, stub-only package, "-stubs package", "types- package", stubgen, PEP 561]
area: languages/python/static-typing
level: advanced
summary: >-
  A stub file has the suffix .pyi and gives the types of a module with no code. Typeshed has stubs for the standard library and many packages, and py.typed marks a package with its own types.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/distributing.html", locator: "Distributing type information"}
proposals: [pep-561, pep-484]
see_also: [python/type-checkers, python/overloads, python/basic-type-annotations, python/modules, python/packages, python/the-typing-specification]
sources:
  - url: https://typing.python.org/en/latest/spec/distributing.html
    title: Distributing type information, Specification for the Python type system
    locator: Stub files; Syntax; The Typeshed Project; Packaging Type Information; Stub-only Packages; Import resolution ordering
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/stubs.html
    title: Stub files, mypy documentation
    locator: Stub files; Creating a stub
    accessed: 2026-11-18
  - url: https://github.com/python/typeshed/blob/main/README.md
    title: typeshed README
    locator: About; Using with Type Checkers
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/installed_packages.html
    title: Using installed packages, mypy documentation
    locator: Using installed packages; Creating PEP 561 compatible packages
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0561/
    title: "PEP 561: Distributing and Packaging Type Information"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/stubgen.html
    title: Automatic stub generation (stubgen), mypy documentation
    locator: Automatic stub generation (stubgen)
    accessed: 2026-11-18
---

## Synopsis

```python run id=ignored
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as d:
    p = pathlib.Path(d)
    (p / "geo.py").write_text("def area(r):\n    return 3.14 * r * r\n")
    (p / "geo.pyi").write_text("def area(r: float) -> float: ...\n")
    sys.path.insert(0, d)
    import geo

    print(pathlib.Path(geo.__file__).name, geo.area(2))
```

```output for=ignored
geo.py 12.56
```

The module `geo.py` has no hints, and `geo.pyi` next to it gives the types. Python imports the `.py` file and does not read the stub. A checker reads the stub and does not read the `.py` file:

```text
$ cat geo.py
def area(r):
    return 3.14 * r * r
$ cat geo.pyi
def area(r: float) -> float: ...
$ cat main.py
from geo import area

print(area("2"))
$ mypy --strict main.py
main.py:3: error: Argument 1 to "area" has incompatible type "str"; expected "float"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

With no stub, `mypy --strict` reports that `area` is "missing a type annotation".

## Semantics

**r1.** "Stub files, also called type stubs, provide type information for untyped Python packages and modules."[^1] "They are the only way to add type information to extension modules."[^1] Mypy says: "A stub file is a file containing a skeleton of the public interface of that Python module, including classes, variables, functions – and most importantly, their types."[^2] Example: [`ignored`](#ignored).

**r2.** A stub is Python code with no bodies. "Stub files are syntactically valid Python files with a .pyi suffix."[^1] A function body is `...`, and a variable has a hint and no value. Example: [`stub-syntax`](#stub-syntax).

**r3.** The checker uses the stub in place of the code. "If a stub file is found for a module, the type checker should not read the corresponding “real” module."[^1] "If a directory contains both a .py and a .pyi file for the same module, the .pyi file takes precedence."[^2] Example: [`stale-stub`](#stale-stub).

**r4.** "Typeshed contains type annotations for the Python builtins and the Python standard library, as well as type stubs for third-party packages."[^3] "All major type checkers and language servers bundle the standard library stubs. Type stubs for third-party packages can be installed from PyPI."[^3] A typeshed package for the library `requests` has the name `types-requests`.[^4] The README gives the versions: "Typeshed supports Python versions 3.10 to 3.15."[^3]

**r5.** PEP 561 tells how a package ships its types. "This PEP provides a standardized means to leverage existing tooling to package and distribute type information with minimal work and an ordering for type checkers to resolve modules and collect this information for type checking."[^5] "Package maintainers who wish to support type checking of their code MUST add a marker file named py.typed to their package supporting typing."[^1] Example: [`py-typed`](#py-typed).

**r6.** A stub-only package has the name of the package with `-stubs` after it. "For stub-only packages adding a py.typed marker is not needed since the name *-stubs is enough to indicate it is a source of typing information."[^1]

**r7.** The checker looks for types in a fixed order. In this order, the user code comes before the typeshed stubs for the standard library, and "Stub packages - these packages SHOULD supersede any installed inline package."[^1] A package with a `py.typed` file comes after the stub packages.[^1]

**r8.** The tool `stubgen` of mypy writes a first stub. "Stubgen generates draft stubs. The auto-generated stub files often require some manual updates, and most types will default to Any."[^6]

## Examples

```python run id=stub-syntax
import ast

stub = """
from typing import overload

VERSION: str

class Point:
    x: float
    def __init__(self, x: float) -> None: ...

@overload
def scale(v: int) -> int: ...
@overload
def scale(v: float) -> float: ...
"""
tree = ast.parse(stub)
print([type(n).__name__ for n in tree.body])
```

```output for=stub-syntax
['ImportFrom', 'AnnAssign', 'ClassDef', 'FunctionDef', 'FunctionDef']
```

The stub is valid Python, so `ast.parse` accepts it (rule r2). The variable `VERSION` has no value, and the bodies are `...`. A stub has the variants of an overload and no implementation.

```python run id=py-typed
import pathlib
import sys
import tempfile
from importlib import resources

with tempfile.TemporaryDirectory() as d:
    pkg = pathlib.Path(d) / "shapes"
    pkg.mkdir()
    (pkg / "__init__.py").write_text("def area(r: float) -> float:\n    return 3.14 * r * r\n")
    (pkg / "py.typed").write_text("")
    sys.path.insert(0, d)
    import shapes

    print(shapes.area(1.0), resources.files("shapes").joinpath("py.typed").is_file())
```

```output for=py-typed
3.14 True
```

The package `shapes` has hints in its code and an empty `py.typed` file, so a checker uses its hints when the package is installed (rule r5). The file has no effect at run time.

When a library has no types and its stubs are in typeshed but not installed, mypy names the stub package:

```text
$ cat useyaml.py
import yaml
$ mypy --strict useyaml.py
useyaml.py:1: error: Library stubs not installed for "yaml"  [import-untyped]
useyaml.py:1: note: Hint: "python3 -m pip install types-PyYAML"
useyaml.py:1: note: (or run "mypy --install-types" to install all missing stub packages)
useyaml.py:1: note: See https://mypy.readthedocs.io/en/stable/running_mypy.html#missing-imports
Found 1 error in 1 file (checked 1 source file)
```

## Versions

> [!since v="3.7"]
> PEP 561 gives the rules for `py.typed` and the `-stubs` packages.[^5]

## Pitfalls

```python fails id=stale-stub expect=exception:TypeError
import pathlib
import sys
import tempfile

with tempfile.TemporaryDirectory() as d:
    p = pathlib.Path(d)
    (p / "old.py").write_text('def area(r, unit):\n    return f"{3.14 * r * r} {unit}"\n')
    (p / "old.pyi").write_text("def area(r: float) -> float: ...\n")
    sys.path.insert(0, d)
    from old import area

    print(area(2.0) + 1)
```

The code now takes a second argument and returns a string, but the stub still has the old signature. The checker trusts the stub (rule r3), so it accepts the call:

```text
$ cat old.pyi
def area(r: float) -> float: ...
$ cat useold.py
from old import area

print(area(2.0) + 1)
$ mypy --strict useold.py
Success: no issues found in 1 source file
```

Change the stub when you change the code. Mypy has a tool for this: "Mypy also ships with two tools for making it easier to create and maintain stubs: Automatic stub generation (stubgen) and Automatic stub testing (stubtest)."[^2]

```text
$ stubgen geo.py -o out
Processed 1 modules
Generated out/geo.pyi
$ cat out/geo.pyi
def area(r): ...
```

The stub from `stubgen` has the names but no types, because `geo.py` has no hints (rule r8). Add the types before you use the stub.

## Comparison

In Python, a stub is a second file that a checker reads and the interpreter ignores (rule r3). TypeScript has the same plan: a `.d.ts` file gives the types of JavaScript code, and the project DefinitelyTyped publishes `@types/` packages, as typeshed publishes `types-` packages. A C header is similar, but the compiler checks the header against the code, and a Python checker does not check a stub against its module. For the checkers, see [[python/type-checkers]].
