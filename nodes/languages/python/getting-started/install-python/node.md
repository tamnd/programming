---
tag: 19MTQD
kind: feature
owner: python
slug: install-python
name: Install Python
aliases: [installing Python, Python install manager, check the Python version]
area: languages/python/getting-started
level: intro
summary: >-
  Python comes from python.org (an installer for macOS, the install manager for Windows), from the system package manager, or from a version manager such as pyenv or uv. python3 -V shows the version.
grammar: "python -V | python -VV (Python Setup and Usage, 1.1.1 Interface options); py install <TAG> (Using Python on Windows, 4.1.5)"
spec: {standard: null, url: "https://docs.python.org/3.15/using/cmdline.html#cmdoption-V", locator: "1.1.1 Interface options, -V"}
see_also: [python/running-a-python-program, python/project-layout, python/interactive-mode]
sources:
  - url: https://www.python.org/downloads/
    title: Download Python, python.org
    locator: Active Python releases
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/windows.html#python-install-manager
    title: Using Python on Windows, Python 3.15
    locator: 4.1 Python install manager, 4.1.1 Installation
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/windows.html#installing-runtimes
    title: Using Python on Windows, Python 3.15
    locator: 4.1.2 Basic use; 4.1.4 Listing runtimes; 4.1.5 Installing runtimes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/mac.html#installation-steps
    title: Using Python on macOS, Python 3.15
    locator: 5.1.1 Installation steps
    accessed: 2026-11-16
  - url: https://docs.brew.sh/Language-Runtimes-and-Packages
    title: Language Runtimes and Packages, Homebrew Documentation
    locator: Python
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/unix.html#on-linux
    title: Using Python on Unix platforms, Python 3.15
    locator: 2.1.1 On Linux
    accessed: 2026-11-16
  - url: https://packages.debian.org/stable/python3
    title: Package python3, Debian stable
    locator: Package python3 (3.13.5-1)
    accessed: 2026-11-16
  - url: https://packages.ubuntu.com/noble/python3
    title: Package python3, Ubuntu 24.04 (noble)
    locator: Package python3 (3.12.3-0ubuntu2.1)
    accessed: 2026-11-16
  - url: https://github.com/pyenv/pyenv
    title: "pyenv: Simple Python Version Management (README)"
    locator: Usage; Switch between Python versions; Windows
    accessed: 2026-11-16
  - url: https://docs.astral.sh/uv/guides/install-python/
    title: Installing Python, uv documentation
    locator: Getting started; Installing a specific version
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-V
    title: Python Setup and Usage, Python 3.15
    locator: 1.1.1 Interface options, -V
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.version_info
    title: sys, Python 3.15 documentation
    locator: sys.version_info; sys.version
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/platform.html#platform.python_version
    title: platform, Python 3.15 documentation
    locator: platform.python_version()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/windows.html#the-full-installer-deprecated
    title: Using Python on Windows, Python 3.15
    locator: 4.11 The full installer (deprecated)
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0668/
    title: "PEP 668: Marking Python base environments as externally managed"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/windows.html#troubleshooting
    title: Using Python on Windows, Python 3.15
    locator: 4.1.15 Troubleshooting
    accessed: 2026-11-16
---

## Synopsis

```text
Windows   install the Python install manager (python.org or the Microsoft Store)
          py install 3.15            add a runtime
          py list                    list the installed runtimes
macOS     the python.org .pkg installer, or: brew install python
Debian    sudo apt install python3   (Ubuntu too)
any Unix  pyenv install 3.15         build a version from source
          uv python install 3.15     download a prebuilt version
check     python3 -V                 prints "Python X.Y.Z" and exits
```

A page about a feature of one version gives that version. The examples of this site ran on CPython 3.12.3.

## Semantics

**r1.** The CPython team publishes the releases on python.org. In November 2026 the list of active releases has 3.15 and 3.14 in the bugfix status, 3.13 to 3.11 in the security status, and 3.16 as a pre-release. 3.10 is at end of life.[^1]

**r2.** Windows has no Python of its own. To get Python from the CPython team, the documentation says to use the Python install manager, from python.org/downloads or from the Microsoft Store. It adds the commands `python`, `py` and `pymanager`.[^2] `py install <tag>` adds a runtime, `py list` shows the installed ones, and `py -V:3.15` runs a given one.[^3]

**r3.** On macOS, the release team makes a `.pkg` installer for each release. It puts the interpreter in `/Library/Frameworks/Python.framework`, adds that location to the shell path, and places links in `/usr/local/bin/`. After the install, the documentation says to run `Install Certificates.command` to finish.[^4] Homebrew is another source: `brew install python` gives its current default Python 3 as the commands `python3` and `pip3`.[^5]

**r4.** On Linux, "Python comes preinstalled on most Linux distributions, and is available as a package on all others."[^6] The version is the one of the distribution: the package `python3` is 3.13.5 in Debian stable and 3.12.3 in Ubuntu 24.04. In both, the `venv` module is in the separate package `python3-venv`.[^7] [^8]

**r5.** A version manager keeps several versions side by side. pyenv builds most versions from source (`pyenv install 3.10.4`) and selects one per shell, per directory or per user (`pyenv shell`, `pyenv local`, `pyenv global`). It does not work on Windows outside WSL.[^9] uv downloads prebuilt interpreters (`uv python install 3.12`); the uv guide notes that "Python does not publish official distributable binaries", so these come from the python-build-standalone project.[^10]

**r6.** `python3 -V` (or `--version`) prints the version number and exits. `-VV` adds the build details.[^11] Example: [`version-check`](#version-check).

**r7.** Inside a program, `sys.version_info` is a tuple of five fields: major, minor, micro, release level and serial. The fields have names, so `sys.version_info.major` is `sys.version_info[0]`. Tuples compare field by field, so `sys.version_info >= (3, 10)` is the right test for a minimum version.[^12] `platform.python_version()` returns the version as a string `'major.minor.patchlevel'`.[^13] Example: [`version-check`](#version-check), [`require-version`](#require-version).

## Examples

The version from the command line and from inside the program agree (rules r6 and r7). The example does not print the version itself, so it gives the same output on any Python 3.8 or later.

```python run id=version-check
import platform
import subprocess
import sys

print(sys.version_info >= (3, 8))
print(sys.version_info.major == sys.version_info[0])
result = subprocess.run([sys.executable, "-V"], capture_output=True, text=True)
print(result.stdout.strip() == "Python " + platform.python_version())
print(result.stdout.split()[0])
```

```output for=version-check
True
True
True
Python
```

A program that needs a minimum version can test `sys.version_info` before it does anything else (rule r7).

```python run id=require-version
import sys

MINIMUM = (3, 8)
if sys.version_info < MINIMUM:
    sys.exit("This program needs Python %d.%d or later" % MINIMUM)
print("version OK")
```

```output for=require-version
version OK
```

## Versions

The documentation marks the full Windows installer as deprecated, and says that it "will not be produced for Python 3.16 or later". The install manager of rule r2 replaces it.[^14] The option `-VV` came with 3.6.[^11]

## Pitfalls

### Version strings do not compare as versions

Strings compare character by character, so `"3.9"` is greater than `"3.10"`. Compare tuples of integers, as `sys.version_info` is (rule r7).

```python wrong id=version-compare
print("3.9" < "3.10")
print((3, 9) < (3, 10))
```

```output for=version-compare
False
True
```

### Slicing `sys.version`

The first three characters of `sys.version` were the version up to 3.9. For 3.10 and later they are `3.1`. The documentation of `sys.version` says: "Do not extract version information out of it".[^12]

```python wrong id=version-slice
version = "3.12.3 (main, Apr 10 2024, 05:33:47) [GCC 13.2.0]"
print(version[:3])
print(version.split()[0])
```

```output for=version-slice
3.1
3.12.3
```

### `pip install` into the system Python fails

A distribution or Homebrew can mark its Python as externally managed. Then pip refuses to install packages into it outside a virtual environment, and points the user to one (PEP 668).[^15] [^5] Make a virtual environment with `python3 -m venv .venv` and install the packages there.

### `python` opens the Microsoft Store

On Windows, `python` can open the Store app instead of running Python. The troubleshooting table of the documentation says to check that the install manager is installed, and that the aliases for "Python (default)" are on in "Manage app execution aliases".[^16]

### macOS has an Apple `python3`

Recent versions of macOS have a `/usr/bin/python3` for the Apple developer tools. The documentation says never to change or delete it. The python.org installer puts its own `python3` first in the path.[^4]

## Comparison

pyenv and uv solve the same problem in two ways. pyenv compiles each version from its source release, so the machine needs the build dependencies of Python first.[^9] uv downloads a ready interpreter, and it installs a missing version on demand when a command needs it.[^10] The python.org installers and the Windows install manager give the builds of the CPython team.[^2] [^4]
