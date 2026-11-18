---
tag: QC3FN6
kind: release
owner: python
version: "3.13"
name: Python 3.13
area: languages/python/versions
level: core
summary: >-
  Python 3.13 came out on 2024-10-07. It adds a new interactive shell, an experimental free-threaded build, an experimental JIT and defined locals() semantics, and it removes 19 old modules.
released: 2024-10-07
eol: 2029-10
patches:
  - {v: 3.13.0, date: 2024-10-07}
  - {v: 3.13.1, date: 2024-12-03}
  - {v: 3.13.2, date: 2025-02-04}
  - {v: 3.13.3, date: 2025-04-08}
  - {v: 3.13.4, date: 2025-06-03}
  - {v: 3.13.5, date: 2025-06-11}
  - {v: 3.13.6, date: 2025-08-06}
  - {v: 3.13.7, date: 2025-08-14}
  - {v: 3.13.8, date: 2025-10-07}
  - {v: 3.13.9, date: 2025-10-14}
  - {v: 3.13.10, date: 2025-12-02}
  - {v: 3.13.11, date: 2025-12-05}
  - {v: 3.13.12, date: 2026-02-03}
  - {v: 3.13.13, date: 2026-04-07}
  - {v: 3.13.14, date: 2026-06-10}
  - {v: 3.13.15, date: 2026-08-05}
  - {v: 3.13.16, date: 2026-09-30}
  - {v: 3.13.17, date: 2026-10-21}
  - {v: 3.13.18, date: 2026-10-25}
adds: [new-repl, colour-tracebacks, free-threaded-build, experimental-jit, locals-semantics, frame-locals-proxy, ios-tier-3, android-tier-3, typevar-defaults, warnings-deprecated, typeddict-readonly, typeis, copy-replace, dbm-sqlite3, docstring-dedent, static-attributes, firstlineno, python-finalization-error, keyword-argument-hints, two-year-bugfix-support]
removes: [pep-594-modules, lib2to3, tkinter-tix, locale-resetlocale, typing-io-re, chained-classmethod-descriptors, pathlib-path-context-manager, re-template, unittest-findtestcases, urlopen-cafile, typeddict-keyword-syntax]
deprecates: [array-u-format, ctypes-setpointertype, decimal-n-format, glob-glob0-glob1, cgi-http-request-handler, re-positional-flags, purepath-is-reserved, platform-java-ver, sqlite3-positional-connect, namedtuple-keyword-syntax, typeddict-empty-call, no-type-check-decorator, typing-anystr, wave-markers]
notes: {url: "https://docs.python.org/3.13/whatsnew/3.13.html"}
sources:
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#summary-release-highlights
    title: What's New In Python 3.13
    locator: Summary, release highlights
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0719/
    title: "PEP 719: Python 3.13 Release Schedule"
    locator: Release schedule; Bugfix releases; 3.13 Lifespan
    accessed: 2026-11-18
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0667/
    title: "PEP 667: Consistent views of namespaces"
    locator: Specification
  - url: https://peps.python.org/pep-0703/
    title: "PEP 703: Making the Global Interpreter Lock Optional in CPython"
    locator: Specification
  - url: https://peps.python.org/pep-0744/
    title: "PEP 744: JIT Compilation"
    locator: Specification
  - url: https://peps.python.org/pep-0696/
    title: "PEP 696: Type Defaults for Type Parameters"
    locator: Specification
  - url: https://peps.python.org/pep-0702/
    title: "PEP 702: Marking deprecations using the type system"
    locator: Specification
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#a-better-interactive-interpreter
    title: What's New In Python 3.13
    locator: A better interactive interpreter
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#improved-error-messages
    title: What's New In Python 3.13
    locator: Improved error messages
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#free-threaded-cpython
    title: What's New In Python 3.13
    locator: Free-threaded CPython
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#other-language-changes
    title: What's New In Python 3.13
    locator: Other Language Changes
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#removed-modules-and-apis
    title: What's New In Python 3.13
    locator: Removed Modules And APIs
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#new-deprecations
    title: What's New In Python 3.13
    locator: New Deprecations
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#porting-to-python-3-13
    title: What's New In Python 3.13
    locator: Porting to Python 3.13
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#notable-changes-in-3-13-4
    title: What's New In Python 3.13
    locator: Notable changes in 3.13.4
  - url: https://docs.python.org/3.13/whatsnew/3.13.html#notable-changes-in-3-13-18
    title: What's New In Python 3.13
    locator: Notable changes in 3.13.18
  - url: https://docs.python.org/3.13/whatsnew/changelog.html
    title: Changelog, Python 3.13 documentation
    locator: Python 3.13.18 final back to Python 3.13.0 final
    accessed: 2026-11-18
---

## What is new

Python 3.13.0 was released on 2024-10-07.[^1] Its largest changes are a new interactive shell, an experimental build without the global interpreter lock, and an experimental just-in-time compiler. The examples on this page ran on CPython 3.13.18, 3.14.10 and 3.15.2. Most of them fail on 3.12, because they use syntax or functions that 3.12 does not have.

### A new interactive shell

The default interactive shell is new, based on code from the PyPy project. It supports multiline editing with history, the commands `help`, `exit` and `quit` without brackets, colour prompts and tracebacks, help browsing with F1, history browsing with F2 and a paste mode with F3. The variable `PYTHON_BASIC_REPL` selects the old shell.[^9] See [interactive mode](/python/interactive-mode/).

### Defined semantics for locals() (PEP 667)

In a function, a generator, a coroutine or a comprehension, each call of `locals()` now returns an independent snapshot of the local variables. Before 3.13, CPython returned the same dictionary each time and updated it at times that were not specified. `frame.f_locals` in these scopes is now a write-through proxy, so a debugger can change a local variable reliably.[^4]

```python run id=locals-semantics since=3.13
import sys


def snapshot():
    x = 1
    first = locals()
    x = 2
    second = locals()
    print(first["x"], second["x"], first is second)


def write_through():
    y = 1
    sys._getframe().f_locals["y"] = 5
    print(y, type(sys._getframe().f_locals).__name__)


snapshot()
write_through()
```

```output for=locals-semantics
1 2 False
5 FrameLocalsProxy
```

On 3.12 and older, the two calls return one shared dictionary, which the second call updates:

```python run id=locals-old until=3.12
def snapshot():
    x = 1
    first = locals()
    x = 2
    second = locals()
    print(first["x"], second["x"], first is second)


snapshot()
```

```output for=locals-old
2 2 True
```

The change also affects `exec()` and `eval()` in a function when they have no explicit namespace. Their changes to local variables are never visible to a later `locals()` call. Pass a dictionary as the namespace to read the results.[^15] See [eval and exec](/python/eval-and-exec/) and [code and frame objects](/python/code-and-frame-objects/).

### Free-threaded build (PEP 703)

CPython can be built with `--disable-gil`, which removes the global interpreter lock, so threads run Python code in parallel on several cores.[^5] In 3.13 this build is experimental and is not the default. It is a separate executable, usually `python3.13t`, and the official Windows and macOS installers can install it.[^11] The function `sys._is_gil_enabled()` tells whether the GIL is on in the running process. A C extension that does not declare support for free threading turns the GIL back on when it is imported.[^11] See [free-threaded Python](/python/free-threaded-python/) and [the global interpreter lock](/python/the-global-interpreter-lock/).

### An experimental JIT compiler (PEP 744)

A CPython built with `--enable-experimental-jit` includes a just-in-time compiler. Hot bytecode is translated to an internal representation of micro-operations, optimized, and then turned into machine code with the copy-and-patch technique. The JIT is off by default in the official builds, and the speed-up is modest.[^6] See [the JIT compiler](/python/the-jit-compiler/).

### Better error messages

Tracebacks are in colour by default in a terminal. The variables `PYTHON_COLORS`, `NO_COLOR` and `FORCE_COLOR` control this. When a script has the same name as a module of the standard library and this causes an error, the message says so. For an unknown keyword argument, the message suggests the right one.[^10]

```python run id=keyword-hint since=3.13
import traceback

try:
    "a b c".split(max_split=1)
except TypeError as error:
    print(traceback.format_exception_only(error)[-1], end="")
```

```output for=keyword-hint
TypeError: split() got an unexpected keyword argument 'max_split'. Did you mean 'maxsplit'?
```

On 3.12 and older the message has no suggestion:

```python run id=keyword-hint-old until=3.12
import traceback

try:
    "a b c".split(max_split=1)
except TypeError as error:
    print(traceback.format_exception_only(error)[-1], end="")
```

```output for=keyword-hint-old
TypeError: 'max_split' is an invalid keyword argument for split()
```

### Typing

- PEP 696: a type parameter can have a default, as in `class Box[T = int]`. This works for `TypeVar`, `ParamSpec` and `TypeVarTuple`.[^7]
- PEP 702: the decorator `warnings.deprecated()` marks a function or a class as deprecated. Type checkers report its use, and a call emits `DeprecationWarning` at run time.[^8]
- PEP 705: `typing.ReadOnly` marks an item of a `TypedDict` as read-only for type checkers.[^1]
- PEP 742: `typing.TypeIs` narrows a type in both branches of a test. It is an alternative to `typing.TypeGuard`.[^1]

```python run id=typing since=3.13
import warnings
from warnings import deprecated


class Box[T = int]:
    pass


(param,) = Box.__type_params__
print(param, param.has_default(), param.__default__)


@deprecated("use new_api() instead")
def old_api():
    return 42


with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    print(old_api())
print(caught[0].category.__name__, caught[0].message)
```

```output for=typing
T True <class 'int'>
42
DeprecationWarning use new_api() instead
```

### Smaller language and library changes

- `copy.replace(obj, **changes)` makes a modified copy of a named tuple, a data class, a `datetime` object or any object with a `__replace__()` method.[^1] See [named tuples](/python/named-tuples/) and [data classes](/python/dataclasses/).
- The compiler strips the common leading white space from each line of a docstring. This makes `.pyc` files about 5 percent smaller, and it affects tools such as `doctest`.[^12]
- A class has the attribute `__static_attributes__`, the names that its methods assign through `self.<name>`, and `__firstlineno__`, the first line of the class definition.[^12]
- The new exception `PythonFinalizationError`, a subclass of `RuntimeError`, signals that an operation is blocked while the interpreter shuts down.[^12]
- The new module `dbm.sqlite3` is the default backend of `dbm`.[^1]
- `ssl.create_default_context()` sets the flags `VERIFY_X509_PARTIAL_CHAIN` and `VERIFY_X509_STRICT`.[^1]

```python run id=library since=3.13
import copy
from collections import namedtuple
from dataclasses import dataclass
from datetime import date

Point = namedtuple("Point", "x y")


@dataclass(frozen=True)
class User:
    name: str
    admin: bool = False


print(copy.replace(Point(1, 2), y=5))
print(copy.replace(User("ann"), admin=True))
print(copy.replace(date(2024, 10, 7), year=2025))


class Vector:
    """
    A point in the plane.

        It has two coordinates.
    """

    def __init__(self, x, y):
        self.x = x
        self.y = y


print(repr(Vector.__doc__))
print(Vector.__static_attributes__)
```

```output for=library
Point(x=1, y=5)
User(name='ann', admin=True)
2025-10-07
'\nA point in the plane.\n\n    It has two coordinates.\n'
('x', 'y')
```

### Platforms and support policy

- PEP 730 and PEP 738: iOS and Android are officially supported platforms, at tier 3. `wasm32-wasi` is tier 2, and `wasm32-emscripten` is no longer supported.[^1]
- PEP 602 was changed so that a feature release has two years of bugfix support, followed by three years of security fixes. 3.13 is the first release with this policy. 3.9 to 3.12 have one and a half years of bugfix support and three and a half years of security fixes.[^1]

## Removed and deprecated

### Removed in 3.13

Most items below were deprecated in an earlier release.[^13]

- PEP 594: the 19 "dead batteries" `aifc`, `audioop`, `cgi`, `cgitb`, `chunk`, `crypt`, `imghdr`, `mailcap`, `msilib`, `nis`, `nntplib`, `ossaudiodev`, `pipes`, `sndhdr`, `spwd`, `sunau`, `telnetlib`, `uu` and `xdrlib`. They were deprecated in 3.11. Most have a copy on PyPI with the prefix `standard-`, and the docs name other replacements, such as `subprocess` for `pipes` and `base64` for `uu`.
- The `2to3` program and the module `lib2to3`.
- `tkinter.tix`.
- `locale.resetlocale()`. Use `locale.setlocale(locale.LC_ALL, "")`.
- The namespaces `typing.io` and `typing.re`. Import the names from `typing`.
- The keyword-argument form of `TypedDict("TD", x=int)`.
- Chained `classmethod` descriptors, which wrapped another descriptor such as `property`.
- `pathlib.Path` objects as context managers.
- `re.template()` and the flag `re.TEMPLATE`.
- `unittest.findTestCases()`, `makeSuite()` and `getTestCaseNames()`. Use the methods of `TestLoader`.
- The `cafile`, `capath` and `cadefault` parameters of `urllib.request.urlopen()`. Use `context`.
- `configparser.LegacyInterpolation` and the subscript access to `importlib.metadata.EntryPoint`.

```python run id=dead-batteries since=3.13
import importlib.util

dead = ["aifc", "audioop", "cgi", "cgitb", "chunk", "crypt", "imghdr", "mailcap", "msilib", "nis", "nntplib", "ossaudiodev", "pipes", "sndhdr", "spwd", "sunau", "telnetlib", "uu", "xdrlib", "lib2to3"]
print([name for name in dead if importlib.util.find_spec(name)])
```

```output for=dead-batteries
[]
```

### Deprecated in 3.13

The version in brackets is the planned removal.[^14]

- In `http.server`: `CGIHTTPRequestHandler` and the `--cgi` option (3.15).
- `pathlib.PurePath.is_reserved()` (3.15). Use `os.path.isreserved()`.
- `platform.java_ver()` (3.15).
- `ctypes.SetPointerType()` (3.15).
- `glob.glob0()` and `glob.glob1()`. Use `glob.glob()` with `root_dir`.
- In `wave`: `getmark()`, `setmark()` and `getmarkers()` (3.15).
- In `typing`: the keyword-argument form of `NamedTuple("Point", x=int)` (3.15), a `NamedTuple` or `TypedDict` call with no fields, and `no_type_check_decorator()` (3.15).
- `typing.AnyStr` (out of `__all__` in 3.16, removed in 3.18). Use the type parameter syntax.
- In `sqlite3`: more than one positional argument to `connect()`, and keyword arguments for the callbacks of `create_function()`, `create_aggregate()` and the `set_*` methods. They become keyword-only or positional-only in 3.15.
- The format code `'u'` of `array` (3.16). Use `'w'`.
- The `maxsplit`, `count` and `flags` arguments of `re.split()`, `re.sub()` and `re.subn()` as positional arguments.
- The format specifier `'N'` of `decimal.Decimal` (3.18).
- Assignment to `__code__` of a function with a code object of another kind, such as a generator.

## Migration

Most code that runs on 3.12 without warnings runs on 3.13. These changes can break it.[^15]

- **Removed modules.** An import of one of the 19 PEP 594 modules or of `lib2to3` fails with `ModuleNotFoundError`. Install the `standard-` package from PyPI, or move to the replacement that the docs name.[^13]
- **locals().** Code that calls `locals()` once and expects the dictionary to change later, or that reads the results of `exec()` through `locals()`, now sees a snapshot. Pass an explicit namespace.
- **f_locals.** `frame.f_locals` in a function is a proxy, not a dictionary. Use `dict(frame.f_locals)` for a snapshot.
- **functools.partial.** A `partial` object as a class attribute gives `FutureWarning`, because in a later release it binds as a method. Wrap it in `staticmethod()`.
- **pathlib.** `Path.glob()` and `rglob()` with a pattern that ends in `**` return files and directories, not only directories. Add a trailing slash to match only directories.
- **gzip and zipfile.** `GzipFile.mode` is the string `'rb'` or `'wb'`, not an integer. The file that `ZipFile.open()` returns for reading has the mode `'rb'`.
- **getpass.** `getpass.getuser()` raises `OSError` on each failure.
- **Free threading.** Packages with C extensions need pip 24.1 or newer to install in the free-threaded build.[^11]

The bugfix release 3.13.4 adds the value `os.path.ALLOW_MISSING` for the `strict` parameter of `os.path.realpath()`, and it fixes path traversal problems in the extraction filters of `tarfile` (CVE-2025-4435 and others).[^16] The bugfix release 3.13.18 makes `ssl` reject certificate chains that contain one of a set of compromised eMudhra intermediate CA certificates.[^17] Upgrade from an older 3.13 release to the newest one.

## Support dates

The release manager of 3.13 is Thomas Wouters.[^2] CPython releases have two phases of support.[^3]

| Phase | From | To |
|---|---|---|
| Bugfix (binary installers, regular releases) | 2024-10-07 | 2026-09-30 (3.13.16) |
| Security (source-only releases, as needed) | October 2026 | October 2029 |

Development of 3.13 began on 2023-05-22. The first alpha came on 2023-10-13, beta 1 on 2024-05-08 and the first release candidate on 2024-08-01.[^2] PEP 719 marks 3.13.16 as the last regular bugfix release with binary installers. The security release 3.13.17 also had binary installers, as an exception.[^2] Security fixes come as source releases until about October 2029, five years after 3.13.0.[^3]

The dates of the releases come from the changelog.[^18] For 3.13.16, PEP 719 gives a date one day later, 2026-10-01. 3.13.5 was a hotfix release.

| Release | Date |
|---|---|
| 3.13.0 | 2024-10-07 |
| 3.13.1 | 2024-12-03 |
| 3.13.2 | 2025-02-04 |
| 3.13.3 | 2025-04-08 |
| 3.13.4 | 2025-06-03 |
| 3.13.5 | 2025-06-11 |
| 3.13.6 | 2025-08-06 |
| 3.13.7 | 2025-08-14 |
| 3.13.8 | 2025-10-07 |
| 3.13.9 | 2025-10-14 |
| 3.13.10 | 2025-12-02 |
| 3.13.11 | 2025-12-05 |
| 3.13.12 | 2026-02-03 |
| 3.13.13 | 2026-04-07 |
| 3.13.14 | 2026-06-10 |
| 3.13.15 | 2026-08-05 |
| 3.13.16 | 2026-09-30 |
| 3.13.17 | 2026-10-21 |
| 3.13.18 | 2026-10-25 |

## References
