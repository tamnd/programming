---
tag: E8WVFT
kind: release
owner: python
version: "3.11"
name: Python 3.11
area: languages/python/versions
level: core
summary: >-
  Python 3.11 came out on 2022-10-24. It is 1.25 times faster than 3.10 on average, and it adds exception groups, exception notes, tomllib and exact error locations in tracebacks.
released: 2022-10-24
eol: 2027-10
patches:
  - {v: 3.11.0, date: 2022-10-24}
  - {v: 3.11.1, date: 2022-12-06}
  - {v: 3.11.2, date: 2023-02-07}
  - {v: 3.11.3, date: 2023-04-04}
  - {v: 3.11.4, date: 2023-06-06}
  - {v: 3.11.5, date: 2023-08-24}
  - {v: 3.11.6, date: 2023-10-02}
  - {v: 3.11.7, date: 2023-12-04}
  - {v: 3.11.8, date: 2024-02-06}
  - {v: 3.11.9, date: 2024-04-02}
  - {v: 3.11.10, date: 2024-09-07}
  - {v: 3.11.11, date: 2024-12-03}
  - {v: 3.11.12, date: 2025-04-08}
  - {v: 3.11.13, date: 2025-06-03}
  - {v: 3.11.14, date: 2025-10-09}
  - {v: 3.11.15, date: 2026-03-03}
  - {v: 3.11.16, date: 2026-08-13}
  - {v: 3.11.17, date: 2026-10-01}
  - {v: 3.11.18, date: 2026-10-25}
adds: [faster-cpython, specializing-adaptive-interpreter, exception-groups, except-star, exception-notes, tomllib, fine-grained-error-locations, safe-path-option, variadic-generics, typeddict-required, typing-self, literal-string, dataclass-transform, format-z-option, int-str-conversion-limit, starred-for-targets, zero-cost-exceptions, frozen-startup-modules]
removes: [asyncio-coroutine-decorator, binhex, inspect-getargspec, inspect-formatargspec, gettext-l-functions, open-u-mode, create-datagram-endpoint-reuse-address, unittest-namespace-discovery]
deprecates: [pep-594-modules, asynchat-asyncore-smtpd, lib2to3, sre-modules, chained-classmethod-descriptors, octal-escapes-over-377, int-trunc-delegation, locale-getdefaultlocale, locale-resetlocale, re-template, typing-text, typeddict-keyword-syntax, importlib-resources-legacy, configparser-legacy-interpolation]
notes: {url: "https://docs.python.org/3.11/whatsnew/3.11.html"}
sources:
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#summary-release-highlights
    title: What's New In Python 3.11
    locator: Summary, release highlights
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0664/
    title: "PEP 664: Python 3.11 Release Schedule"
    locator: Release schedule; Bugfix releases; 3.11 Lifespan
    accessed: 2026-11-18
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0654/
    title: "PEP 654: Exception Groups and except*"
    locator: Specification
  - url: https://peps.python.org/pep-0678/
    title: "PEP 678: Enriching Exceptions with Notes"
    locator: Specification
  - url: https://peps.python.org/pep-0680/
    title: "PEP 680: tomllib: Support for Parsing TOML in the Standard Library"
    locator: Specification
  - url: https://peps.python.org/pep-0657/
    title: "PEP 657: Include Fine Grained Error Locations in Tracebacks"
    locator: Specification
  - url: https://peps.python.org/pep-0659/
    title: "PEP 659: Specializing Adaptive Interpreter"
    locator: Specialization
  - url: https://peps.python.org/pep-0673/
    title: "PEP 673: Self Type"
    locator: Specification
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#faster-cpython
    title: What's New In Python 3.11
    locator: Faster CPython
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#other-language-changes
    title: What's New In Python 3.11
    locator: Other Language Changes
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#new-features-related-to-type-hints
    title: What's New In Python 3.11
    locator: New Features Related to Type Hints
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#deprecated
    title: What's New In Python 3.11
    locator: Deprecated
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#removed
    title: What's New In Python 3.11
    locator: Removed
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#porting-to-python-3-11
    title: What's New In Python 3.11
    locator: Porting to Python 3.11
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#notable-changes-in-3-11-4
    title: What's New In Python 3.11
    locator: Notable Changes in 3.11.4, 3.11.5, 3.11.10 and 3.11.13
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#notable-changes-in-3-11-18
    title: What's New In Python 3.11
    locator: Notable changes in 3.11.18
  - url: https://docs.python.org/3.11/whatsnew/changelog.html
    title: Changelog, Python 3.11 documentation
    locator: Python 3.11.18 final back to Python 3.11.0 final
    accessed: 2026-11-18
  - url: https://docs.python.org/3.11/whatsnew/3.11.html#other-cpython-implementation-changes
    title: What's New In Python 3.11
    locator: Other CPython Implementation Changes
---

## What is new

Python 3.11.0 was released on 2022-10-24. It is between 10 and 60 percent faster than 3.10, and 1.25 times faster on average on the standard benchmark suite.[^1] It adds a way to raise and handle several exceptions at once, notes on exceptions, a TOML parser, and tracebacks that point to the exact expression. The examples on this page ran on CPython 3.11.18, 3.12, 3.13.18, 3.14.10 and 3.15.2, with the same output. Most of them fail on 3.10.

### Faster CPython

The Faster CPython project made 3.11 about 25 percent faster than 3.10 on pyperformance, built with GCC on Ubuntu Linux. Start-up is 10 to 15 percent faster because the core modules are frozen into the interpreter. Frames are cheaper, and frame objects are made only when a debugger or an introspection function asks for them. Most calls from Python code to a Python function use no C stack space.[^10]

PEP 659 adds the specializing adaptive interpreter. When code runs many times, the interpreter replaces a generic bytecode instruction with a specialized one for the types it sees, and it caches results inline in the bytecode.[^8] A `try` statement costs nothing when no exception is raised.[^10] See [the specializing adaptive interpreter](/python/the-specializing-adaptive-interpreter/).

### Exception groups and except* (PEP 654)

The built-in types `ExceptionGroup` and `BaseExceptionGroup` carry several unrelated exceptions together. The new `except*` clause handles the part of a group that matches a type, and passes the rest on.[^4] This is for code that runs many tasks and must report every failure, such as `asyncio.TaskGroup`.

```python run id=exception-groups since=3.11
def check(values):
    errors = []
    for v in values:
        try:
            if not isinstance(v, int):
                raise TypeError(f"not an int: {v!r}")
            if v < 0:
                raise ValueError(f"negative: {v}")
        except (TypeError, ValueError) as error:
            errors.append(error)
    if errors:
        raise ExceptionGroup("bad values", errors)


try:
    check([1, -2, "x", -3])
except* ValueError as group:
    print("ValueError:", [str(e) for e in group.exceptions])
except* TypeError as group:
    print("TypeError:", [str(e) for e in group.exceptions])
```

```output for=exception-groups
ValueError: ['negative: -2', 'negative: -3']
TypeError: ["not an int: 'x'"]
```

See [raising and handling multiple unrelated exceptions](/python/raising-and-handling-multiple-unrelated-exceptions/).

### Exception notes (PEP 678)

`BaseException.add_note()` adds a string to the list `__notes__` of an exception. The traceback prints each note after the message.[^5] This lets code add context to an exception without a new exception type.

```python run id=exception-notes since=3.11
import traceback

try:
    try:
        int("x1")
    except ValueError as error:
        error.add_note("while reading line 7 of settings.txt")
        raise
except ValueError as error:
    print(error.__notes__)
    print("".join(traceback.format_exception_only(error)), end="")
```

```output for=exception-notes
['while reading line 7 of settings.txt']
ValueError: invalid literal for int() with base 10: 'x1'
while reading line 7 of settings.txt
```

See [enriching exceptions with notes](/python/enriching-exceptions-with-notes/).

### tomllib (PEP 680)

The new module `tomllib` reads TOML, the format of `pyproject.toml`. It can only parse; it cannot write TOML.[^6]

```python run id=tomllib since=3.11
import tomllib

text = """
[project]
name = "demo"
version = "1.0"
dependencies = ["requests>=2", "rich"]
"""
data = tomllib.loads(text)
print(data["project"]["name"], data["project"]["dependencies"])
```

```output for=tomllib
demo ['requests>=2', 'rich']
```

### Fine-grained error locations (PEP 657)

A traceback now marks the exact expression that failed, not only the line. In a line with several subscripts or calls, this shows which one raised the error.[^7]

```python run id=error-locations since=3.11
import traceback


def total(order):
    return order["price"] * order["count"] + order["tax"]["rate"]


try:
    total({"price": 2, "count": 3, "tax": None})
except TypeError:
    for line in traceback.format_exc().splitlines()[-3:]:
        print(line)
```

```output for=error-locations
    return order["price"] * order["count"] + order["tax"]["rate"]
                                             ~~~~~~~~~~~~^^^^^^^^
TypeError: 'NoneType' object is not subscriptable
```

The positions are in code objects, and `codeobject.co_positions()` returns them. The option `-X no_debug_ranges` and the variable `PYTHONNODEBUGRANGES` turn the feature off, to save a little memory and disk space.[^1] See [tracebacks and exception objects](/python/tracebacks-and-exception-objects/).

### Typing

- PEP 673: `typing.Self` is the type of the current instance, for methods that return `self` or a copy of it.[^9]
- PEP 646: variadic generics, with `TypeVarTuple`, for types such as array shapes.[^12]
- PEP 655: `Required` and `NotRequired` mark single items of a `TypedDict`.[^12]
- PEP 675: `LiteralString` accepts only strings that come from literals, to guard against injection.[^12]
- PEP 681: `dataclass_transform` marks a decorator or a base class that makes classes like `dataclasses` does.[^12]
- PEP 563, which turns all annotations into strings, was planned to become the default. That plan is on hold indefinitely.[^12] Later, [Python 3.14](/python/3.14/) made the evaluation of annotations lazy instead.

```python run id=self-type since=3.11
from typing import Self


class Shape:
    def scaled(self, k: float) -> Self:
        clone = type(self).__new__(type(self))
        clone.__dict__ = {key: v * k for key, v in self.__dict__.items()}
        return clone


class Square(Shape):
    def __init__(self, side):
        self.side = side


print(type(Square(2).scaled(3)).__name__, Square(2).scaled(3).side)
```

```output for=self-type
Square 6
```

### Smaller language and library changes

- The format option `z` turns a negative zero into a positive zero after rounding (PEP 682).[^11]
- A starred expression can be the iterable of a `for` statement, as in `for x in *a, *b:`.[^11]
- The option `-P` and the variable `PYTHONSAFEPATH` stop Python from adding the directory of the script, or the current directory, to `sys.path`. So a local file cannot shadow a module of the standard library.[^11]
- A `with` statement on an object that is not a context manager raises `TypeError`, not `AttributeError`.[^11]
- The conversion between `int` and `str` in base 10 and other bases that are not a power of two raises `ValueError` above 4300 digits, by default. This mitigates CVE-2020-10735, a denial of service. `sys.set_int_max_str_digits()` changes the limit.[^19]

```python run id=small-changes since=3.11
import sys

print(f"{-0.0001:.2f}", f"{-0.0001:z.2f}")
print(sys.get_int_max_str_digits())
try:
    int("1" * 5000)
except ValueError as error:
    print(str(error).split(";")[0])
seen = []
for x in *[1, 2], *[3]:
    seen.append(x)
print(seen)
```

```output for=small-changes
-0.00 0.00
4300
Exceeds the limit (4300 digits) for integer string conversion: value has 5000 digits
[1, 2, 3]
```

## Removed and deprecated

### Removed in 3.11

Most items below were deprecated in an earlier release.[^14]

- The decorator `@asyncio.coroutine`, for generator-based coroutines. Use `async def`.
- The module `binhex`, and the related functions of `binascii` such as `a2b_hqx()`.
- In `inspect`: `getargspec()` and `formatargspec()`. Use `inspect.signature()`.
- In `gettext`: `lgettext()`, `ldgettext()`, `lngettext()` and `ldngettext()`, and the `codeset` parameter.
- The mode `'U'` of `open()`, `io.open()`, `codecs.open()` and `fileinput.FileInput`. Text mode always uses universal newlines.[^15]
- The `reuse_address` parameter of `asyncio.loop.create_datagram_endpoint()`, for security reasons.
- The `__getitem__()` methods of `xml.dom.pulldom.DOMEventStream`, `wsgiref.util.FileWrapper` and `fileinput.FileInput`.
- Namespace package support in `unittest` discovery.

### Deprecated in 3.11

The version in brackets is the planned removal.[^13]

- PEP 594: the 19 modules `aifc`, `audioop`, `cgi`, `cgitb`, `chunk`, `crypt`, `imghdr`, `mailcap`, `msilib`, `nis`, `nntplib`, `ossaudiodev`, `pipes`, `sndhdr`, `spwd`, `sunau`, `telnetlib`, `uu` and `xdrlib` (3.13).
- `asynchat`, `asyncore` and `smtpd` (3.12).
- `lib2to3` and the `2to3` tool. They may not parse code for Python 3.10 or newer.
- The undocumented modules `sre_compile`, `sre_constants` and `sre_parse`.
- Chained `classmethod` descriptors.
- Octal escapes above `0o377`, such as `"\477"`.
- `int()` on an object that has only `__trunc__()`.
- `locale.getdefaultlocale()` (3.15) and `locale.resetlocale()` (3.13).
- `re.template()` and `re.TEMPLATE` (3.13).
- `typing.Text`, and the keyword-argument form of `TypedDict` (3.13).
- The old functions of `importlib.resources`, such as `open_text()` and `read_binary()`.
- `configparser.LegacyInterpolation` (3.13), and the old `configparser` names `SafeConfigParser` and `readfp()` (3.12).

## Migration

Most code that runs on 3.10 without warnings runs on 3.11. These changes can break it.[^15]

- **File mode.** `open(name, "rU")` raises `ValueError`. Remove the `U`.
- **random.** The population of `random.sample()` must be a sequence. A set is no longer converted. A sample larger than the population raises `ValueError`.
- **asyncio.** `@asyncio.coroutine` is gone, and `loop.set_default_executor()` accepts only a `ThreadPoolExecutor`.
- **ast.** `compile()` checks the positions of `ast` nodes and raises `ValueError` for invalid ones.
- **Integer strings.** A program that converts very long numbers between `int` and `str` in base 10 fails with `ValueError` above 4300 digits. Raise the limit with `sys.set_int_max_str_digits()`, the option `-X int_max_str_digits` or the variable `PYTHONINTMAXSTRDIGITS`.[^19]
- **object.__getstate__().** The new default method can confuse code that tests whether `__getstate__` exists.[^11]
- **Deprecations.** Run the test suite with `-W error::DeprecationWarning` to find uses of the PEP 594 modules, which 3.13 removes.

Several bugfix releases changed behaviour.[^16] 3.11.4 adds the `filter` argument to the extraction methods of `tarfile` and to `shutil.unpack_archive()`. 3.11.5 builds the python.org Windows and macOS installers with OpenSSL 3.0. 3.11.10 fixes `is_global` and `is_private` in `ipaddress`, and makes `email` stricter about headers and addresses (CVE-2023-27043). 3.11.13 adds `os.path.ALLOW_MISSING` and fixes path traversal problems in the `tarfile` filters (CVE-2025-4435 and others). The security release 3.11.18 makes `ssl` reject certificate chains that contain one of a set of compromised eMudhra intermediate CA certificates.[^17]

## Support dates

The release manager of 3.11 is Pablo Galindo Salgado.[^2] CPython releases have two phases of support.[^3]

| Phase | From | To |
|---|---|---|
| Bugfix (binary installers, regular releases) | 2022-10-24 | 2024-04-02 (3.11.9) |
| Security (source-only releases, as needed) | after 3.11.9 | October 2027 |

Development of 3.11 began on 2021-05-03. The first alpha came on 2021-10-05, beta 1 on 2022-05-08 and the first release candidate on 2022-08-08.[^2] 3.11 had bugfix releases about every two months for about 18 months. 3.11.9, the ninth bugfix release, was the last one with binary installers. After it, security fixes come as source releases until about October 2027, five years after 3.11.0.[^2]

The dates of the releases come from the changelog.[^18] PEP 664 gives other dates for three releases: 2023-02-08 for 3.11.2, 2023-04-05 for 3.11.3 and 2026-08-12 for 3.11.16.[^2]

| Release | Date |
|---|---|
| 3.11.0 | 2022-10-24 |
| 3.11.1 | 2022-12-06 |
| 3.11.2 | 2023-02-07 |
| 3.11.3 | 2023-04-04 |
| 3.11.4 | 2023-06-06 |
| 3.11.5 | 2023-08-24 |
| 3.11.6 | 2023-10-02 |
| 3.11.7 | 2023-12-04 |
| 3.11.8 | 2024-02-06 |
| 3.11.9 | 2024-04-02 |
| 3.11.10 | 2024-09-07 |
| 3.11.11 | 2024-12-03 |
| 3.11.12 | 2025-04-08 |
| 3.11.13 | 2025-06-03 |
| 3.11.14 | 2025-10-09 |
| 3.11.15 | 2026-03-03 |
| 3.11.16 | 2026-08-13 |
| 3.11.17 | 2026-10-01 |
| 3.11.18 | 2026-10-25 |

## References
