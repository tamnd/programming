---
tag: Q9RVY4
kind: feature
owner: python
slug: namespace-packages
name: Namespace packages
aliases: [namespace package, implicit namespace package, PEP 420, portion, package without __init__.py, _NamespacePath, NamespacePath, NamespaceLoader, pkgutil.extend_path]
area: languages/python/modules-and-packages
level: advanced
summary: >-
  A namespace package is a package without __init__.py. Its __path__ joins directories of the same name from several sys.path entries, so separate distributions can share one parent package.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/import.html#namespace-packages", locator: "5.2.2. Namespace packages"}
see_also: [python/packages, python/the-module-search-path, python/module-objects, python/the-import-system, python/importing-from-a-package]
sources:
- url: https://docs.python.org/3.15/reference/import.html#namespace-packages
  title: The Python Language Reference, 5. The import system
  locator: 5.2. Packages; 5.2.1. Regular packages; 5.2.2. Namespace packages
  accessed: 2026-11-18
- url: https://peps.python.org/pep-0420/
  title: "PEP 420: Implicit Namespace Packages"
  locator: Abstract; Rationale; Specification; Dynamic path computation; Differences between namespace packages and regular packages; Packaging Implications
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/glossary.html#term-namespace-package
  title: Glossary, Python 3.15 documentation
  locator: namespace package
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/library/importlib.html#importlib.machinery.NamespaceLoader
  title: importlib — The implementation of import
  locator: importlib.machinery.NamespaceLoader; importlib.machinery.NamespacePath
  accessed: 2026-11-18
- url: https://docs.python.org/3.15/tutorial/modules.html#packages
  title: The Python Tutorial, 6. Modules
  locator: 6.4. Packages
  accessed: 2026-11-18
- url: https://go.dev/ref/spec#Packages
  title: The Go Programming Language Specification
  locator: Packages
  accessed: 2026-11-18
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-7.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 7. Packages and Modules
  locator: 7.4.3. Package Observability and Visibility
  accessed: 2026-11-18
---

## Synopsis

```text
project1/                  # one sys.path entry
    parent/                # no __init__.py: a portion of the namespace package parent
        child/
            one.py
project2/                  # another sys.path entry
    parent/
        child/
            two.py

import parent.child.one    # parent.__path__ lists project1/parent and project2/parent
import parent.child.two    # found in the second portion
```

## Semantics

**r1.** Python has "two types of packages, regular packages and namespace packages."[^1] "A namespace package is a composite of various portions, where each portion contributes a subpackage to the parent package. Portions may reside in different locations on the file system."[^1] PEP 420 defines a portion as "a set of files in a single directory (possibly stored in a zip file) that contribute to a namespace package."[^2] Example: [`portions`](#portions).

**r2.** No file marks a namespace package. "Namespace packages cannot contain an __init__.py."[^2] "There will be no marker file or directory for specifying a namespace package."[^2] The glossary says that namespace packages "allow several individually installable packages to have a common parent package. Otherwise, it is recommended to use a regular package."[^3]

**r3.** For each directory in the parent path, the import system looks for a name `foo` in this order. "If <directory>/foo/__init__.py is found, a regular package is imported and returned. If not, but <directory>/foo.{py,pyc,so,pyd} is found, a module is imported and returned."[^2] "If not, but <directory>/foo is found and is a directory, it is recorded and the scan continues with the next directory in the parent path."[^2] "If the scan completes without returning a module or package, and at least one directory was recorded, then a namespace package is created."[^2] Examples: [`module-wins`](#module-wins), [`regular-wins`](#regular-wins).

**r4.** The namespace package "Has a __path__ attribute set to an iterable of the path strings that were found and recorded during the scan. Does not have a __file__ attribute."[^2] The reference adds: "Namespace packages do not use an ordinary list for their __path__ attribute. They instead use a custom iterable type which will automatically perform a new search for package portions on the next import attempt within that package if the path of their parent package (or sys.path for a top level package) changes."[^1] Examples: [`portions`](#portions), [`dynamic`](#dynamic).

**r5.** A namespace package can be nested. "When the import system searches a regular package’s __path__ and encounters a subdirectory that does not contain an __init__.py file, that subdirectory becomes a portion contributing to a namespace subpackage of the enclosing regular package."[^1]

**r6.** "Once a namespace package is created, there is no functional difference between it and a regular package."[^2] PEP 420 lists the differences in how the two are made. Among them: "Namespace packages have no __file__ attribute", "Namespace packages have no __init__.py module", and "Namespace packages have a different type of object for their __loader__ attribute."[^2] That loader is `importlib.machinery.NamespaceLoader`, which "is only made public for introspecting the __loader__ attribute on namespace packages".[^4] Example: [`attributes`](#attributes).

## Examples

Two directories on `sys.path` each give a part of `parent.child` (rules r1 and r4). The `__path__` of each namespace package lists both portions.

```python norun reason=fragment file=project1/parent/child/one.py group=portions
NAME = "one"
```

```python norun reason=fragment file=project2/parent/child/two.py group=portions
NAME = "two"
```

```python run file=main.py group=portions id=portions
import os
import sys

sys.path += ["project1", "project2"]

import parent.child.one
import parent.child.two

print(parent.child.one.NAME, parent.child.two.NAME)
print([os.path.relpath(p) for p in parent.__path__])
print([os.path.relpath(p) for p in parent.child.__path__])
```

```output for=portions
one two
['project1/parent', 'project2/parent']
['project1/parent/child', 'project2/parent/child']
```

The `__path__` of a namespace package follows `sys.path` (rule r4). A portion added after the first import is found by the next import.

```python norun reason=fragment file=project1/parent/child/one.py group=dynamic
NAME = "one"
```

```python norun reason=fragment file=project3/parent/child/three.py group=dynamic
NAME = "three"
```

```python run file=main.py group=dynamic id=dynamic
import os
import sys

sys.path.append("project1")
import parent.child.one

try:
    import parent.child.three
except ModuleNotFoundError as e:
    print("not found:", e.name)

sys.path.append("project3")
import parent.child.three

print(parent.child.three.NAME)
print([os.path.relpath(p) for p in parent.__path__])
```

```output for=dynamic
not found: parent.child.three
three
['project1/parent', 'project3/parent']
```

A namespace package has no `__init__.py` to run, and its loader is a `NamespaceLoader` (rule r6). PEP 420 says that it has no `__file__`. On the CPython versions tested here, the attribute exists and its value is `None`, and `__spec__.origin` is also `None`.

```python norun reason=fragment file=project1/parent/child/one.py group=attributes
NAME = "one"
```

```python run file=main.py group=attributes id=attributes
import sys
from importlib.machinery import NamespaceLoader

sys.path.append("project1")
import parent

print(parent.__file__)
print(isinstance(parent.__loader__, NamespaceLoader))
print(parent.__spec__.origin)
print("(namespace)" in repr(parent))
```

```output for=attributes
None
True
None
True
```

A directory has the lowest priority (rule r3). The directory `tools` in `project1` is recorded, but the module `tools.py` in `project2` is returned.

```python norun reason=fragment file=project1/tools/helpers.py group=module-wins
NAME = "helpers"
```

```python norun reason=fragment file=project2/tools.py group=module-wins
NAME = "tools.py"
```

```python run file=main.py group=module-wins id=module-wins
import os
import sys

sys.path += ["project1", "project2"]

import tools

print(tools.NAME)
print(os.path.relpath(tools.__file__))
```

```output for=module-wins
tools.py
project2/tools.py
```

## Versions

The type of a namespace `__path__` is a private class `_NamespacePath` on CPython 3.12.3, 3.13.14 and 3.14.6. On CPython 3.15.0b2 the class is `NamespacePath`. The 3.15 importlib page lists `importlib.machinery.NamespacePath`, which "Represents a namespace package’s path (module.__path__)", and the page marks it as a 3.15 addition.[^4] The examples on this page do not print the type, and they give the same output on the four versions.

## Pitfalls

### One `__init__.py` stops the scan

A regular package returns at once (rule r3). If one portion has an `__init__.py`, the import system does not look at the other directories, and their subpackages are not found.

```python norun reason=fragment file=project1/parent/__init__.py group=regular-wins
"""This file makes project1/parent a regular package."""
```

```python norun reason=fragment file=project1/parent/child/one.py group=regular-wins
NAME = "one"
```

```python norun reason=fragment file=project2/parent/child/two.py group=regular-wins
NAME = "two"
```

```python fails file=main.py group=regular-wins id=regular-wins expect=exception:ModuleNotFoundError
import sys

sys.path += ["project1", "project2"]

import parent.child.one
import parent.child.two
```

All the portions of a namespace package must leave out `__init__.py`.

### Any directory can be imported

Because no marker file is needed (rule r2), `import` accepts a directory that is not code. Here `config/` holds only a data file. The import succeeds, and the error comes later.

```python norun reason=fragment file=config/settings.toml group=any-directory
debug = true
```

```python fails file=main.py group=any-directory id=any-directory expect=exception:AttributeError
import config

print(config.__file__)
config.load()
```

The tutorial gives the reason that regular packages need `__init__.py`: "This prevents directories with a common name, such as string, from unintentionally hiding valid modules that occur later on the module search path."[^5] A namespace package does not hide a module (rule r3), but a mistyped or missing module name can still import an unrelated directory.

### Code in `__init__.py` of a portion

PEP 420 explains the old way, `pkgutil.extend_path`, and its problem: "the package’s __init__.py cannot practically define any names as it depends on the order of the package fragments on sys.path to determine which portion is imported first."[^2] A namespace package has no place for shared code at the top. Put shared code in a subpackage that one distribution owns.

## Comparison

**Go** ties a package to its source files: "A set of files sharing the same PackageName form the implementation of a package. An implementation may require that all source files for a package inhabit the same directory."[^6] Python can build one package from directories on different `sys.path` entries.

**Java** modules take the opposite rule for a package. A package is uniquely visible to a module M when no compilation unit of M declares it "and M reads exactly one other module that exports the package to M."[^7] So a package that two modules export to M is not uniquely visible to M. Two portions of one Python namespace package are the normal case.

## Implementation notes

PEP 420 gives the reason for the dynamic `__path__`: "this PEP requires that the list of path entries be dynamically computed when each portion is loaded. It is expected that the import machinery will do this efficiently by caching __path__ values and only refreshing them when it detects that the parent path has changed."[^2] It also explains why `__init__.py` was dropped: Linux vendors install portions "into the same file system directory. This creates a potential for conflict, as the portions are now attempting to provide the same file on the target system".[^2]

## References
