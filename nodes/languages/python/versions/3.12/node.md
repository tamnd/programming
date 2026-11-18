---
tag: XT07TD
kind: release
owner: python
version: "3.12"
name: Python 3.12
area: languages/python/versions
level: core
summary: >-
  Python 3.12 came out on 2023-10-02. It adds the type parameter syntax, f-strings in the grammar, a per-interpreter GIL and sys.monitoring, and it removes distutils.
released: 2023-10-02
eol: 2028-10
patches:
  - {v: 3.12.0, date: 2023-10-02}
  - {v: 3.12.1, date: 2023-12-07}
  - {v: 3.12.2, date: 2024-02-06}
  - {v: 3.12.3, date: 2024-04-09}
  - {v: 3.12.4, date: 2024-06-06}
  - {v: 3.12.5, date: 2024-08-06}
  - {v: 3.12.6, date: 2024-09-06}
  - {v: 3.12.7, date: 2024-10-01}
  - {v: 3.12.8, date: 2024-12-03}
  - {v: 3.12.9, date: 2025-02-04}
  - {v: 3.12.10, date: 2025-04-08}
  - {v: 3.12.11, date: 2025-06-03}
  - {v: 3.12.12, date: 2025-10-09}
  - {v: 3.12.13, date: 2026-03-03}
  - {v: 3.12.14, date: 2026-08-12}
  - {v: 3.12.15, date: 2026-10-01}
  - {v: 3.12.16, date: 2026-10-25}
adds: [type-parameter-syntax, type-statement, f-strings-in-grammar, per-interpreter-gil, sys-monitoring, buffer-protocol-in-python, comprehension-inlining, import-error-hints, typeddict-kwargs, typing-override, invalid-escape-syntaxwarning, hashable-slices, neumaier-sum, tarfile-extraction-filter, itertools-batched, perf-profiler-support, immortal-objects, unstable-c-api-tier]
removes: [distutils, asynchat, asyncore, smtpd, imp, unittest-method-aliases, ssl-wrap-socket, ssl-match-hostname, locale-format, venv-setuptools, configparser-safeconfigparser, unicode-wstr]
deprecates: [datetime-utcnow, ast-constant-aliases, asyncio-child-watchers, bool-invert, shutil-rmtree-onerror, itertools-copy-pickle, sys-last-type, three-argument-throw, tarfile-no-filter, typing-hashable-sized, calendar-january-february, collections-abc-bytestring, module-package-attribute]
notes: {url: "https://docs.python.org/3.12/whatsnew/3.12.html"}
sources:
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#summary-release-highlights
    title: What's New In Python 3.12
    locator: Summary, release highlights
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0693/
    title: "PEP 693: Python 3.12 Release Schedule"
    locator: Release schedule; Bugfix releases; 3.12 Lifespan
    accessed: 2026-11-18
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0695/
    title: "PEP 695: Type Parameter Syntax"
    locator: Specification
  - url: https://peps.python.org/pep-0701/
    title: "PEP 701: Syntactic formalization of f-strings"
    locator: Specification
  - url: https://peps.python.org/pep-0684/
    title: "PEP 684: A Per-Interpreter GIL"
    locator: Specification
  - url: https://peps.python.org/pep-0669/
    title: "PEP 669: Low Impact Monitoring for CPython"
    locator: Specification
  - url: https://peps.python.org/pep-0709/
    title: "PEP 709: Inlined comprehensions"
    locator: Specification
  - url: https://peps.python.org/pep-0632/
    title: "PEP 632: Deprecate distutils module"
    locator: Migration Advice
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#improved-error-messages
    title: What's New In Python 3.12
    locator: Improved Error Messages
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#other-language-changes
    title: What's New In Python 3.12
    locator: Other Language Changes
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#new-features-related-to-type-hints
    title: What's New In Python 3.12
    locator: New Features Related to Type Hints
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#removed
    title: What's New In Python 3.12
    locator: Removed
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#deprecated
    title: What's New In Python 3.12
    locator: Deprecated
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#porting-to-python-3-12
    title: What's New In Python 3.12
    locator: Porting to Python 3.12
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#notable-changes-in-3-12-5
    title: What's New In Python 3.12
    locator: Notable changes in 3.12.4, 3.12.5 and 3.12.6
  - url: https://docs.python.org/3.12/whatsnew/3.12.html#notable-changes-in-3-12-16
    title: What's New In Python 3.12
    locator: Notable changes in 3.12.16
  - url: https://docs.python.org/3.12/whatsnew/changelog.html
    title: Changelog, Python 3.12 documentation
    locator: Python 3.12.16 final back to Python 3.12.0 final
    accessed: 2026-11-18
---

## What is new

Python 3.12.0 was released on 2023-10-02.[^1] Its largest changes are a new syntax for generic types, a formal grammar for f-strings and better "Did you mean" suggestions. The library drops `distutils` and several other old modules. The examples on this page ran on CPython 3.12, 3.13.18, 3.14.10 and 3.15.2. Most of them fail on 3.11.

### Type parameter syntax (PEP 695)

A generic function or class can declare its type parameters in square brackets after its name, with no `TypeVar` call. The new `type` statement declares a type alias, whose value is evaluated lazily.[^4]

```python run id=type-parameters since=3.12
def first[T](items: list[T]) -> T:
    return items[0]


class Stack[T]:
    def __init__(self) -> None:
        self.items: list[T] = []


type Pair[T] = tuple[T, T]

print(first([3, 1, 2]))
print(first.__type_params__, Stack.__type_params__)
print(type(Pair).__name__, Pair.__value__, Pair.__type_params__)
```

```output for=type-parameters
3
(T,) (T,)
TypeAliasType tuple[T, T] (T,)
```

The type parameters live in a new kind of scope. See [annotation scopes](/python/annotation-scopes/).

### f-strings in the grammar (PEP 701)

The parser now reads f-strings with the grammar of the language, not with a separate routine. So the expression in a replacement field can reuse the quote of the f-string, contain a backslash, span several lines with comments, and nest without a limit on depth.[^5]

```python run id=f-strings since=3.12
songs = ["Eden", "Alkaline", "Ascensionism"]
print(f"Playlist: {", ".join(songs)}")
print(f"{"\n".join(songs)}")
print(f"{len([
    1,  # one
    2,  # two
])}")
print(f"{f"{f"{1 + 1}"}"}")
```

```output for=f-strings
Playlist: Eden, Alkaline, Ascensionism
Eden
Alkaline
Ascensionism
2
2
```

On 3.11 the second line of this program is a `SyntaxError`. A side effect of the change is that the module `tokenize` emits new tokens for f-strings.[^15] See [formatted string literals](/python/formatted-string-literals/).

### A per-interpreter GIL (PEP 684)

Each subinterpreter can now have its own global interpreter lock, so subinterpreters can run in parallel. In 3.12 this is available only through the C API.[^6] Python code got a module for it in 3.14. See [subinterpreters](/python/subinterpreters/) and [the global interpreter lock](/python/the-global-interpreter-lock/).

### Low-impact monitoring (PEP 669)

The namespace `sys.monitoring` lets a debugger, a profiler or a coverage tool ask for events such as the start of a function, a line or a branch. A tool pays only for the events that it turns on, which is much cheaper than `sys.settrace()`.[^7]

```python run id=monitoring since=3.12
import sys

mon = sys.monitoring
calls = []


def on_start(code, offset):
    calls.append(code.co_name)


def square(n):
    return n * n


mon.use_tool_id(mon.PROFILER_ID, "demo")
mon.register_callback(mon.PROFILER_ID, mon.events.PY_START, on_start)
mon.set_events(mon.PROFILER_ID, mon.events.PY_START)
square(2)
square(3)
mon.set_events(mon.PROFILER_ID, 0)
mon.free_tool_id(mon.PROFILER_ID)
print(calls)
```

```output for=monitoring
['square', 'square']
```

### Comprehension inlining (PEP 709)

List, dict and set comprehensions no longer create a new single-use function object each time they run. The interpreter inlines them, which makes a comprehension up to two times faster.[^8] A comprehension no longer appears as a separate frame in a traceback, and `locals()` in a comprehension now includes the variables from outside the comprehension.[^1] The loop variable of a comprehension stays local to it.

### Better error messages

A `NameError` at the top level can suggest a standard module to import, and a `NameError` in a method can suggest `self.<name>`. The text `import x from y` gets a hint to write `from y import x`. A failed `from module import name` suggests a close name in the module.[^10]

```python run id=import-hint since=3.12
import traceback

try:
    exec("from sys import getrecursionlimt")
except ImportError as error:
    print(traceback.format_exception_only(error)[-1], end="")
```

```output for=import-hint
ImportError: cannot import name 'getrecursionlimt' from 'sys' (unknown location). Did you mean: 'getrecursionlimit'?
```

On 3.11 the same program prints the message with no suggestion.

### Typing

- PEP 692: `**kwargs: Unpack[Movie]` types the keyword arguments of a function with a `TypedDict`, so each keyword can have its own type.[^12]
- PEP 698: the decorator `typing.override` says that a method overrides a method of a base class. A type checker reports an error when no base class has the method.[^12]

### Smaller language and library changes

- A backslash pair that is not a valid escape sequence, such as `"\d"`, gives `SyntaxWarning`, not `DeprecationWarning`. A later release will make it a `SyntaxError`. Use raw strings for regular expressions.[^11]
- `sum()` of floats uses Neumaier summation, which is more accurate.[^11]
- `slice` objects are hashable, so they can be dict keys and set members.[^11]
- `itertools.batched(iterable, n)` yields tuples of `n` items, and the last one can be shorter.[^1] See [itertools](/python/itertools/).
- The extraction methods of `tarfile` and `shutil.unpack_archive()` take a `filter` argument, which can refuse dangerous members, such as a file outside the target directory.[^11]
- `sqlite3` and `uuid` have a command-line interface, and `pathlib.Path` supports subclasses.[^1]
- CPython supports the Linux `perf` profiler, and it protects against stack overflow on the platforms that support it.[^1]
- PEP 683 makes some objects immortal, so their reference counts never change. PEP 697 adds an unstable tier of the C API.[^1]

```python run id=library since=3.12
import warnings
from itertools import batched

print(list(batched("ABCDEFG", 3)))
print(sum([0.1] * 10) == 1.0)
print({slice(1, 3): "middle"}[slice(1, 3)])

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    compile(r'pattern = "\d+"', "<demo>", "exec")
print(caught[0].category.__name__)
```

```output for=library
[('A', 'B', 'C'), ('D', 'E', 'F'), ('G',)]
True
middle
SyntaxWarning
```

On 3.11, `sum([0.1] * 10) == 1.0` is `False`, and the warning for `"\d"` is a `DeprecationWarning`:

```python run id=library-old until=3.11
import warnings

print(sum([0.1] * 10) == 1.0)
with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    compile(r'pattern = "\d+"', "<demo>", "exec")
print(caught[0].category.__name__)
```

```output for=library-old
False
DeprecationWarning
```

## Removed and deprecated

### Removed in 3.12

Most items below were deprecated in an earlier release.[^13]

- The package `distutils` (PEP 632). The third-party package `setuptools` still provides it.[^9]
- `venv` no longer installs `setuptools` in a new environment, and `ensurepip` no longer bundles it. So `setuptools`, `pkg_resources` and `easy_install` are not there by default. Run `pip install setuptools` in the environment if a project needs them at run time.
- The modules `asynchat`, `asyncore` and `smtpd` (PEP 594). Use `asyncio`, or the PyPI package `aiosmtpd` for `smtpd`.
- The module `imp`. Use `importlib`; the docs give a table of replacements.
- The aliases of `unittest.TestCase` methods, such as `assertEquals`, `failUnless` and `assertRegexpMatches`, and `assertDictContainsSubset()`.
- `ssl.wrap_socket()` and `ssl.match_hostname()`. Use `SSLContext.wrap_socket()`.
- `locale.format()`. Use `locale.format_string()`.
- In `configparser`: `SafeConfigParser` and `readfp()`. Use `ConfigParser` and `read_file()`.
- The `keyfile` and `certfile` parameters of `ftplib`, `imaplib`, `poplib` and `smtplib`. Use `context`.
- `importlib` cleanups: `find_loader()` and `find_module()` support, `importlib.abc.Finder`, and `pkgutil.ImpImporter`.
- `io.OpenWrapper`, `sqlite3.enable_shared_cache()` and `sqlite3.OptimizedUnicode`, `gzip.GzipFile.filename` and `ssl.RAND_pseudo_bytes()`.
- PEP 623: the C API no longer keeps a `wstr` copy of each `str` object, which saves at least 8 bytes per string.[^1]

```python run id=removed since=3.12
import importlib.util
import unittest

print(hasattr(unittest.TestCase, "assertEquals"))
print(importlib.util.find_spec("imp"), importlib.util.find_spec("asyncore"))
```

```output for=removed
False
None None
```

### Deprecated in 3.12

The version in brackets is the planned removal.[^14]

- `datetime.datetime.utcnow()` and `utcfromtimestamp()`. Use `now(tz=datetime.UTC)` and `fromtimestamp(ts, tz=datetime.UTC)`.
- The `ast` classes `Num`, `Str`, `Bytes`, `NameConstant` and `Ellipsis` warn on use (3.14). Use `ast.Constant`.
- The asyncio child watchers (3.14). `asyncio.get_event_loop()` warns when it has to make a loop.
- `~` on a `bool` (an error in 3.16). Use `not`.
- The `onerror` argument of `shutil.rmtree()`. Use `onexc`.
- Copy, deep copy and pickle of `itertools` iterators (3.14).
- `tarfile` extraction with no `filter`, until 3.14 makes `'data'` the default.
- `sys.last_type`, `sys.last_value` and `sys.last_traceback`. Use `sys.last_exc`.
- The three-argument form of `throw()` on generators and coroutines.
- `typing.Hashable` and `typing.Sized`, and `collections.abc.ByteString`.
- `calendar.January` and `calendar.February`. Use `JANUARY` and `FEBRUARY`.
- Setting `__package__` or `__cached__` on a module (3.14).
- In `sqlite3`: the default adapters and converters, and named placeholders with a sequence of parameters (an error in 3.14).
- `os.fork()` in a process with several threads can warn, on POSIX.

## Migration

Most code that runs on 3.11 without warnings runs on 3.12. These changes can break it.[^15]

- **distutils and setuptools.** `import distutils` fails. A project that builds with `setup.py` must declare `setuptools` as a build dependency, and a new virtual environment has no `setuptools` unless you install it.[^13]
- **Removed modules.** Code that imports `imp`, `asynchat`, `asyncore` or `smtpd` fails with `ModuleNotFoundError`.
- **unittest.** Old test suites that call `assertEquals()` and the other aliases fail with `AttributeError`. Rename the calls.
- **Escapes.** Each invalid escape such as `"\d"` in a normal string literal now prints a `SyntaxWarning` when the module compiles. Change the literal to a raw string.
- **random.** `randrange(10.0)` raises `TypeError`, and non-integer arguments raise `TypeError`, not `ValueError`.
- **re.** Numerical group references must be ASCII digits, and group names in bytes patterns must be ASCII letters, digits and underscores.
- **os.** Path functions accept only `bytes` for byte paths, not `bytearray` or `memoryview`.
- **functools.cached_property.** It no longer holds a lock, so two threads can both run the getter for one instance.
- **tokenize.** Tools that read tokens get the new f-string tokens.

Several bugfix releases changed behaviour for safety. 3.12.4 fixed `is_global` and `is_private` of `ipaddress`. 3.12.5 made the `email` generator refuse to write headers that would parse as more than one header. 3.12.6 made `email.utils.getaddresses()` and `parseaddr()` stricter, with a `strict=False` option for the old behaviour.[^16] The security release 3.12.11 fixed several ways to get around the `tarfile` extraction filters with crafted links.[^18] The security release 3.12.16 makes `ssl` reject certificate chains that contain one of a set of compromised eMudhra intermediate CA certificates.[^17]

## Support dates

The release manager of 3.12 is Thomas Wouters.[^2] CPython releases have two phases of support.[^3]

| Phase | From | To |
|---|---|---|
| Bugfix (binary installers, regular releases) | 2023-10-02 | 2025-04-08 (3.12.10) |
| Security (source-only releases, as needed) | after 3.12.10 | October 2028 |

Development of 3.12 began on 2022-05-08. The first alpha came on 2022-10-24, beta 1 on 2023-05-22 and the first release candidate on 2023-08-06.[^2] 3.12 had bugfix releases for about 18 months. 3.12.10 was the tenth and last bugfix release with binary installers. After it, security fixes come as source releases until about October 2028, five years after 3.12.0.[^2] 3.12 is one of the releases that PEP 602 gave one and a half years of bugfix support, before the policy grew to two years with 3.13.[^3]

The dates of the releases come from the changelog, and they agree with PEP 693.[^18]

| Release | Date |
|---|---|
| 3.12.0 | 2023-10-02 |
| 3.12.1 | 2023-12-07 |
| 3.12.2 | 2024-02-06 |
| 3.12.3 | 2024-04-09 |
| 3.12.4 | 2024-06-06 |
| 3.12.5 | 2024-08-06 |
| 3.12.6 | 2024-09-06 |
| 3.12.7 | 2024-10-01 |
| 3.12.8 | 2024-12-03 |
| 3.12.9 | 2025-02-04 |
| 3.12.10 | 2025-04-08 |
| 3.12.11 | 2025-06-03 |
| 3.12.12 | 2025-10-09 |
| 3.12.13 | 2026-03-03 |
| 3.12.14 | 2026-08-12 |
| 3.12.15 | 2026-10-01 |
| 3.12.16 | 2026-10-25 |

## References
