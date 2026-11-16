---
tag: WTGH99
kind: feature
owner: python
slug: distributions-and-conda
name: Distributions and conda
aliases: [conda, conda-forge, Anaconda, PyPy, Python install manager, PEP 668, EXTERNALLY-MANAGED, virtual environment, venv]
area: languages/python/ecosystem
level: core
summary: >-
  Python comes from python.org, from the OS, or from conda. conda installs its own packages, not those on PyPI. PEP 668 stops pip from changing a Python that the OS manages. Use a virtual environment.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0668/", locator: "PEP 668, Marking Python base environments as externally managed"}
see_also: [python/the-python-package-index, python/data-and-science-libraries]
sources:
  - url: https://www.python.org/downloads/
    title: Download Python
    locator: home of the downloads
    accessed: 2026-11-16
  - url: https://docs.python.org/3/using/windows.html
    title: Using Python on Windows
    locator: Python install manager
    accessed: 2026-11-16
  - url: https://docs.python.org/3/using/unix.html
    title: Using Python on Unix platforms
    locator: Getting and installing the latest version of Python
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0668/
    title: "PEP 668: Marking Python base environments as externally managed"
    locator: Abstract; Implementation
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/venv.html
    title: venv, Creation of virtual environments
    locator: introduction; How venvs work
    accessed: 2026-11-16
  - url: https://packaging.python.org/en/latest/key_projects/
    title: Project Summaries, Python Packaging User Guide
    locator: Non-PyPA projects, conda
    accessed: 2026-11-16
  - url: https://docs.conda.io/projects/conda/en/latest/index.html
    title: Conda Documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://conda-forge.org/
    title: conda-forge
    locator: home page
    accessed: 2026-11-16
  - url: https://pypy.org/
    title: PyPy
    locator: home page
    accessed: 2026-11-16
  - url: https://go.dev/doc/toolchain
    title: Go Toolchains
    locator: Introduction
    accessed: 2026-11-16
---

## Synopsis

```python run id=which-python
import sys

print(sys.implementation.name)
print(sys.version_info >= (3, 12))
print(type(sys.prefix != sys.base_prefix).__name__)
```

```output for=which-python
cpython
True
bool
```

A Python program runs on one installation of Python. The installation can come from python.org, from the package manager of the OS, or from conda. It can also be another implementation, such as PyPy. A virtual environment sits on top of one installation and keeps the packages of a project apart from it. The example asks which implementation runs it. The test `sys.prefix != sys.base_prefix` gives `True` in a virtual environment and `False` outside one, so the example prints only its type.

## Semantics

**r1.** On 2026-11-16, the downloads page of python.org offered "Download Python 3.15.2".[^1] For Windows, "To obtain Python from the CPython team, use the Python install manager."[^2] "Python comes preinstalled on most Linux distributions, and is available as a package on all others."[^3]

**r2.** PEP 668 addresses "conflicts between OS package managers and Python-specific package management tools like pip."[^4] It specifies "that Python-specific package management tools should neither install nor remove packages into the interpreter’s global context, by default, and should instead guide the end user towards using a virtual environment."[^4]

**r3.** The signal is "a marker file named EXTERNALLY-MANAGED, whose presence indicates that non-virtual-environment package installations are managed by some means external to Python, such as a distro’s package manager."[^4] Example: [`pep668`](#pep668).

**r4.** "A virtual environment is created on top of an existing Python installation, known as the virtual environment’s “base” Python, and by default is isolated from the packages in the base environment".[^5] A program can check "sys.prefix != sys.base_prefix to determine if the current interpreter is running from a virtual environment."[^5] Examples: [`which-python`](#which-python) and [`venv`](#venv).

**r5.** The file `pyvenv.cfg` of a virtual environment has an "include-system-site-packages key, set to true if venv is run with the --system-site-packages option, false otherwise."[^5] Example: [`venv`](#venv).

**r6.** "Conda provides package, dependency, and environment management for any language."[^7] It "is written in Python and widely used in the Python scientific computing community, due to its support for non-Python compiled libraries and extensions. It is used as the basis of the Anaconda Python distribution from Anaconda, Inc."[^6]

**r7.** "Conda is a completely separate tool from pip, virtualenv and wheel, but provides many of their combined features".[^6] Conda does not install packages from PyPI. "it can only manage packages built specifically for conda, which can be made available on a “conda channel”".[^6] "Note that pip can be installed into, and work side-by-side with conda for managing distributions from PyPI."[^6]

**r8.** conda-forge gives "Community-led recipes, infrastructure and distributions for conda."[^8] "conda-forge is a GitHub organization containing repositories of conda recipes."[^8]

**r9.** PyPy is "A fast, compliant alternative implementation of Python".[^9] "On average, PyPy 3.12 is about 4 times faster than CPython 3.11. We currently support Python 3.12, 3.11 and 2.7."[^9]

## Examples

The rule of PEP 668 for an installer such as pip (rules r2 and r3):

```python run id=pep668
def may_install(in_venv, marker_exists):
    return in_venv or not marker_exists

for in_venv in (False, True):
    for marker in (False, True):
        print(f'venv={in_venv!s:5} marker={marker!s:5} install={may_install(in_venv, marker)}')
```

```output for=pep668
venv=False marker=False install=True
venv=False marker=True  install=False
venv=True  marker=False install=True
venv=True  marker=True  install=True
```

Create a virtual environment and read its `pyvenv.cfg` (rules r4 and r5):

```python run id=venv
import pathlib
import tempfile
import venv

with tempfile.TemporaryDirectory() as tmp:
    env = pathlib.Path(tmp, '.venv')
    venv.create(env, with_pip=False)
    cfg = dict(line.split(' = ', 1)
               for line in (env / 'pyvenv.cfg').read_text().splitlines() if ' = ' in line)
    print(cfg['include-system-site-packages'])
    print((env / 'bin' / 'python').exists())
```

```output for=venv
false
True
```

The environment has "a bin (or Scripts on Windows) subdirectory".[^5]

## Pitfalls

`pip install` into the Python of a Linux distribution can fail with the error `externally-managed-environment` (rule r3). Do not force it. Make a virtual environment and install there.

conda does not install packages from PyPI, and pip does not install conda packages (rule r7). Know which tool installed each package in an environment.

A Python from conda, a Python from the OS and a Python from python.org are different installations. A virtual environment is isolated from the packages of its base (rule r4), so a package that you install in one of them is not in the others.

PyPy is not CPython, and it supports only the Python versions on its own list (rule r9). Check `sys.implementation.name` when the code depends on the implementation (example [`which-python`](#which-python)).

## Comparison

"The go command can use its bundled Go toolchain as well as other versions that it finds in the local PATH or downloads as needed."[^10] In Go, the go command picks the toolchain for a module. In Python, the user picks an installation, and a virtual environment ties a project to it (rule r4).

[^1]: [Download Python](https://www.python.org/downloads/)
[^2]: [Using Python on Windows](https://docs.python.org/3/using/windows.html)
[^3]: [Using Python on Unix platforms](https://docs.python.org/3/using/unix.html)
[^4]: [PEP 668: Marking Python base environments as externally managed](https://peps.python.org/pep-0668/)
[^5]: [venv, Creation of virtual environments](https://docs.python.org/3/library/venv.html)
[^6]: [Project Summaries, Python Packaging User Guide](https://packaging.python.org/en/latest/key_projects/)
[^7]: [Conda Documentation](https://docs.conda.io/projects/conda/en/latest/index.html)
[^8]: [conda-forge](https://conda-forge.org/)
[^9]: [PyPy](https://pypy.org/)
[^10]: [Go Toolchains](https://go.dev/doc/toolchain)
