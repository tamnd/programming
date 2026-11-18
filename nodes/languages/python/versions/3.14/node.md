---
tag: 14S509
kind: release
owner: python
version: "3.14"
name: Python 3.14
area: languages/python/versions
level: core
summary: >-
  Python 3.14 came out on 2025-10-07. It adds deferred annotations, template strings, subinterpreters in the standard library and Zstandard, and it supports free threading.
released: 2025-10-07
eol: 2030-10
patches:
  - {v: 3.14.0, date: 2025-10-07}
  - {v: 3.14.1, date: 2025-12-02}
  - {v: 3.14.2, date: 2025-12-05}
  - {v: 3.14.3, date: 2026-02-03}
  - {v: 3.14.4, date: 2026-04-07}
  - {v: 3.14.5, date: 2026-05-10}
  - {v: 3.14.6, date: 2026-06-10}
  - {v: 3.14.7, date: 2026-08-05}
  - {v: 3.14.8, date: 2026-09-30}
  - {v: 3.14.9, date: 2026-10-21}
  - {v: 3.14.10, date: 2026-10-25}
adds: [deferred-annotations, annotationlib, concurrent-interpreters, template-strings, except-without-brackets, finally-control-flow-warning, remote-debugging, tail-call-interpreter, free-threading-supported, compression-zstd, compression-package, asyncio-introspection, concurrent-safe-warnings, repl-syntax-highlighting, keyword-typo-hints, float-from-number, map-strict, fractional-digit-grouping, python-configuration-c-api, emscripten-tier-3, android-binaries, jit-in-binaries]
removes: [ast-constant-aliases, asyncio-child-watchers, asyncio-implicit-event-loop, importlib-abc-resource-classes, itertools-copy-pickle, pkgutil-get-loader, pty-master-open, sqlite3-version, urllib-urlopener, email-localtime-isdst, argparse-booleanoptionalaction-parameters, int-trunc-delegation, notimplemented-in-boolean-context, pgp-signatures]
deprecates: [future-annotations, asyncio-policy-system, asyncio-iscoroutinefunction, codecs-open, argparse-filetype, complex-complex-arguments, functools-reduce-keywords, purepath-as-uri, nturl2path, os-popen-soft]
notes: {url: "https://docs.python.org/3.14/whatsnew/3.14.html"}
sources:
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#summary-release-highlights
    title: What's new in Python 3.14
    locator: Summary, release highlights
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0745/
    title: "PEP 745: Python 3.14 Release Schedule"
    locator: Release schedule; Bugfix releases; 3.14 lifespan
    accessed: 2026-11-18
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0649/
    title: "PEP 649: Deferred Evaluation Of Annotations Using Descriptors"
    locator: Specification
  - url: https://peps.python.org/pep-0749/
    title: "PEP 749: Implementing PEP 649"
    locator: Specification
  - url: https://peps.python.org/pep-0750/
    title: "PEP 750: Template Strings"
    locator: Specification
  - url: https://peps.python.org/pep-0734/
    title: "PEP 734: Multiple Interpreters in the Stdlib"
    locator: Specification
  - url: https://peps.python.org/pep-0758/
    title: "PEP 758: Allow except and except* expressions without parentheses"
    locator: Specification
  - url: https://peps.python.org/pep-0765/
    title: "PEP 765: Disallow return/break/continue that exit a finally block"
    locator: Specification
  - url: https://peps.python.org/pep-0768/
    title: "PEP 768: Safe external debugger interface for CPython"
    locator: Specification; Security implications
  - url: https://peps.python.org/pep-0784/
    title: "PEP 784: Adding Zstandard to the standard library"
    locator: Specification
  - url: https://peps.python.org/pep-0779/
    title: "PEP 779: Criteria for supported status for free-threaded Python"
    locator: Specification
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#improved-error-messages
    title: What's new in Python 3.14
    locator: Improved error messages
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#other-language-changes
    title: What's new in Python 3.14
    locator: Other language changes, Built-ins
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#removed
    title: What's new in Python 3.14
    locator: Removed
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#new-deprecations
    title: What's new in Python 3.14
    locator: Deprecated, New deprecations
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#porting-to-python-3-14
    title: What's new in Python 3.14
    locator: Porting to Python 3.14
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#notable-changes-in-3-14-5
    title: What's new in Python 3.14
    locator: Notable changes in 3.14.5
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#notable-changes-in-3-14-10
    title: What's new in Python 3.14
    locator: Notable changes in 3.14.10
  - url: https://docs.python.org/3.14/whatsnew/changelog.html
    title: Changelog, Python 3.14 documentation
    locator: Python 3.14.10 final back to Python 3.14.0 final
    accessed: 2026-11-18
---

## What is new

Python 3.14.0 was released on 2025-10-07.[^1] Its largest changes are template string literals, deferred evaluation of annotations, and subinterpreters in the standard library. The examples on this page ran on CPython 3.14.10 and 3.15.2. Most of them fail on 3.13, because they use syntax or modules that 3.13 does not have.

### Deferred evaluation of annotations (PEP 649 and PEP 749)

The interpreter no longer evaluates the annotations of functions, classes and modules when it defines them. It stores them in an annotate function, and it evaluates them only when a program asks for them.[^4] So an annotation can name a class that is defined later in the file, without quotes.

The new module `annotationlib` reads annotations in several formats. `Format.VALUE` evaluates them, as `__annotations__` does. `Format.FORWARDREF` puts a proxy in place of each name that is not defined yet. `Format.STRING` gives the source text.[^5]

```python run id=deferred-annotations since=3.14
from annotationlib import Format, get_annotations


def move(robot: Robot, steps: int) -> None:
    pass


print(get_annotations(move, format=Format.STRING))
try:
    get_annotations(move)
except NameError as error:
    print("NameError:", error)


class Robot:
    pass


print(get_annotations(move))
```

```output for=deferred-annotations
{'robot': 'Robot', 'steps': 'int', 'return': 'None'}
NameError: name 'Robot' is not defined
{'robot': <class '__main__.Robot'>, 'steps': <class 'int'>, 'return': None}
```

On 3.13 the `def` line itself raises `NameError`, because the interpreter evaluates `Robot` when it defines the function. See [annotations at run time](/python/annotations-at-runtime/) and [annotation scopes](/python/annotation-scopes/).

### Template string literals (PEP 750)

A string literal with the prefix `t` makes a `string.templatelib.Template` object, not a `str`. The template keeps the static text and each interpolated value apart, so a function can process the values before it joins the parts, for example to escape HTML or to quote SQL.[^6]

```python run id=template-strings since=3.14
from string.templatelib import Interpolation, Template

name = "<script>"
template = t"Hello, {name}!"
print(type(template).__name__)
print(list(template))


def escape(template: Template) -> str:
    parts = []
    for part in template:
        if isinstance(part, Interpolation):
            parts.append(str(part.value).replace("<", "&lt;").replace(">", "&gt;"))
        else:
            parts.append(part)
    return "".join(parts)


print(escape(template))
```

```output for=template-strings
Template
['Hello, ', Interpolation('<script>', 'name', None, ''), '!']
Hello, &lt;script&gt;!
```

See [template string literals](/python/template-string-literals/) and [formatted string literals](/python/formatted-string-literals/).

### Multiple interpreters in the standard library (PEP 734)

The module `concurrent.interpreters` creates and runs subinterpreters from Python code. Each interpreter has its own modules and its own global state, and each one has its own GIL, so interpreters can run Python code in parallel on several cores.[^7] The class `concurrent.futures.InterpreterPoolExecutor` runs tasks in a pool of interpreters.[^1]

```python run id=subinterpreters since=3.14
from concurrent import interpreters

interp = interpreters.create()
interp.exec("data = [n * n for n in range(5)]; print('in', __name__, data)")


def square(n):
    return n * n


print(interp.call(square, 7))
print(len(interpreters.list_all()))
interp.close()
```

```output for=subinterpreters
in __main__ [0, 1, 4, 9, 16]
49
2
```

The code in the new interpreter runs as its own `__main__` module. See [subinterpreters](/python/subinterpreters/).

### except without brackets and control flow in finally (PEP 758 and PEP 765)

An `except` or `except*` clause can list several exception types without brackets, when it has no `as` clause.[^8] A `return`, `break` or `continue` statement that leaves a `finally` block now gives a `SyntaxWarning` at compile time, because it silently cancels an exception that is in flight.[^9]

```python run id=except-and-finally since=3.14
import warnings

try:
    int("x")
except ValueError, TypeError:
    print("caught")

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter("always")
    compile("def f():\n    try:\n        pass\n    finally:\n        return 1\n", "<demo>", "exec")
print(caught[0].category.__name__, caught[0].message)
```

```output for=except-and-finally
caught
SyntaxWarning 'return' in a 'finally' block
```

See [finally and control flow](/python/finally-and-control-flow/).

### Zstandard and the compression package (PEP 784)

The new package `compression` holds the module `compression.zstd`, which binds the Zstandard library. It also re-exports `lzma`, `bz2`, `gzip` and `zlib` as `compression.lzma` and the other names. The old module names are not deprecated.[^11] The modules `tarfile`, `zipfile` and `shutil` can read and write Zstandard archives.[^1]

```python run id=zstd since=3.14
from compression import zstd

data = b"Python 3.14 " * 1000
packed = zstd.compress(data)
print(len(data), len(packed) < 100, zstd.decompress(packed) == data)
```

```output for=zstd
12000 True True
```

See [data compression](/python/data-compression/).

### Free threading is supported (PEP 779)

The free-threaded build, which runs without the GIL, was experimental in 3.13. In 3.14 it is officially supported, but it is still not the default build.[^12] The specializing adaptive interpreter now works in this build, and the cost for single-threaded code is roughly 5 to 10 percent, by platform and compiler.[^1] See [free-threaded Python](/python/free-threaded-python/).

### Safe external debugger interface (PEP 768)

`sys.remote_exec(pid, script)` asks a running Python process to run a script at its next safe point. A debugger can attach to a live process this way, for example with `python -m pdb -p PID`.[^10] The variable `PYTHON_DISABLE_REMOTE_DEBUG`, the option `-X disable-remote-debug` and the configure flag `--without-remote-debug` turn the feature off.[^1]

### Better error messages

The interpreter suggests the right keyword when a word is close to one. It names the problem when a statement comes after the `else` of a conditional expression. A key or set element that is not hashable gets a message that says which role the value had.[^13]

```python run id=error-messages since=3.14
import traceback

for source in ("whille True:\n    pass\n", "x = 1 if True else pass\n"):
    try:
        compile(source, "<demo>", "exec")
    except SyntaxError as error:
        print(traceback.format_exception_only(error)[-1], end="")
try:
    {}[[1, 2]] = 3
except TypeError as error:
    print("TypeError:", error)
```

```output for=error-messages
SyntaxError: invalid syntax. Did you mean 'while'?
SyntaxError: expected expression after 'else', but statement is given
TypeError: cannot use 'list' as a dict key (unhashable type: 'list')
```

The same program on 3.13 and older prints shorter messages:

```python run id=error-messages-old until=3.13
import traceback

for source in ("whille True:\n    pass\n", "x = 1 if True else pass\n"):
    try:
        compile(source, "<demo>", "exec")
    except SyntaxError as error:
        print(traceback.format_exception_only(error)[-1], end="")
try:
    {}[[1, 2]] = 3
except TypeError as error:
    print("TypeError:", error)
```

```output for=error-messages-old
SyntaxError: invalid syntax
SyntaxError: invalid syntax
TypeError: unhashable type: 'list'
```

### Smaller language and library changes

- `float.from_number()` and `complex.from_number()` convert a number to the type, and raise `TypeError` for a value that is not a number.[^14]
- The format specification accepts `,` or `_` to group the digits after the decimal point, as in `f"{x:,.6_f}"`.[^14]
- `map()` takes `strict=True`, as `zip()` does, to check that all the iterables have the same length.[^14]
- `python -m asyncio ps PID` and `python -m asyncio pstree PID` show the asyncio tasks of a running process.[^1]
- The flag `-X context_aware_warnings` makes `warnings.catch_warnings` safe across threads and asyncio tasks. It is on by default in the free-threaded build.[^1]
- The interactive shell highlights Python syntax, and it completes module names after `import`.[^1] See [interactive mode](/python/interactive-mode/).

```python run id=builtins since=3.14
print(float.from_number(3), complex.from_number(2))
print(f"{1234567.891234:,.6_f}")
try:
    list(map(pow, [1, 2, 3], [2, 2], strict=True))
except ValueError as error:
    print("ValueError:", error)
```

```output for=builtins
3.0 (2+0j)
1,234,567.891_234
ValueError: map() argument 2 is shorter than argument 1
```

### Interpreter, build and release changes

- A new kind of interpreter uses tail calls between small C functions, one for each opcode, in place of one large `switch` statement. Built with Clang 19 or newer on x86-64 or AArch64, it is 3 to 5 percent faster on pyperformance. It is an opt-in build option, `--with-tail-call-interp`, and it does not change the behaviour of programs.[^1]
- Python 3.14.0 to 3.14.4 shipped an incremental garbage collector. Because of reports of high memory use in production, 3.14.5 went back to the generational collector of 3.13.[^18] See [the cyclic garbage collector](/python/the-cyclic-garbage-collector/).
- The official Windows and macOS binaries include the experimental JIT compiler. It is off by default, and `PYTHON_JIT=1` turns it on.[^1] See [the JIT compiler](/python/the-jit-compiler/).
- PEP 741 adds a C API to read and set the configuration of Python.[^1]
- Emscripten is a tier 3 platform (PEP 776), and python.org provides binary releases for Android.[^1]
- PEP 761: the release managers no longer sign releases with PGP. Use the Sigstore verification material, which releases have had since 3.11.[^1]

## Removed and deprecated

### Removed in 3.14

Most items below were deprecated in an earlier release.[^15]

- In `ast`: the classes `Num`, `Str`, `Bytes`, `NameConstant` and `Ellipsis`, and the attributes `Constant.n` and `Constant.s`. Use `ast.Constant` and `Constant.value`. Methods such as `visit_Num` on a `NodeVisitor` are no longer called; define `visit_Constant`.
- In `asyncio`: the child watcher classes and functions, such as `get_child_watcher()` and `ThreadedChildWatcher`.
- `asyncio.get_event_loop()` raises `RuntimeError` when there is no current event loop. It no longer makes a loop. Use `asyncio.run()` or `asyncio.Runner`.
- `importlib.abc.ResourceReader`, `Traversable` and `TraversableResources`. Use the classes in `importlib.resources.abc`.
- Copy, deep copy and pickle of `itertools` iterators.
- `pkgutil.get_loader()` and `pkgutil.find_loader()`.
- `pty.master_open()` and `pty.slave_open()`. Use `pty.openpty()`.
- `sqlite3.version` and `sqlite3.version_info`. Use `sqlite_version` and `sqlite_version_info`. A sequence of parameters with named placeholders raises `ProgrammingError`.
- `urllib.request.URLopener` and `FancyURLopener`, and `urllib.parse.Quoter`.
- The `isdst` parameter of `email.utils.localtime()`.
- The `type`, `choices` and `metavar` parameters of `argparse.BooleanOptionalAction`. Nested argument groups and nested mutually exclusive groups raise `ValueError`.
- Extra keyword arguments to `pathlib.Path`, and extra positional arguments to `PurePath.relative_to()` and `is_relative_to()`.
- `int()` no longer delegates to `__trunc__()`. A class must define `__int__()` or `__index__()`.[^14]
- `NotImplemented` in a boolean context raises `TypeError`. It gave a `DeprecationWarning` since 3.9.[^14]

```python run id=removed since=3.14
import ast
import asyncio

print(hasattr(ast, "Num"), hasattr(ast, "Str"))
try:
    asyncio.get_event_loop()
except RuntimeError as error:
    print("RuntimeError:", error)
```

```output for=removed
False False
RuntimeError: There is no current event loop in thread 'MainThread'.
```

On 3.12 and 3.13 the same program prints `True True` and three `DeprecationWarning` messages.

### Deprecated in 3.14

The version in brackets is the planned removal.[^16]

- `from __future__ import annotations` (PEP 563). It is not removed before 3.13 reaches its end of life in 2029.[^17] See [future statements](/python/future-statements/).
- The asyncio policy system, such as `asyncio.set_event_loop_policy()` and `DefaultEventLoopPolicy` (3.16). Use `asyncio.run()` or `asyncio.Runner` with `loop_factory`.
- `asyncio.iscoroutinefunction()` (3.16). Use `inspect.iscoroutinefunction()`.
- `functools.reduce()` with `function` or `sequence` as keyword arguments, in the Python implementation (positional-only in 3.16).
- `pathlib.PurePath.as_uri()` (3.19). Use `pathlib.Path.as_uri()`.
- `codecs.open()`. Use `open()`.
- `argparse.FileType`.
- A complex number as the `real` or `imag` argument of `complex()`.
- The module `nturl2path`. Use `urllib.request.url2pathname()` and `pathname2url()`.
- Soft deprecations, with no removal date: `os.popen()` and the `os.spawn*` functions. Use `subprocess`.

## Migration

Most code that runs on 3.13 without warnings runs on 3.14. These changes can break it.[^17]

- **multiprocessing.** On Unix platforms other than macOS, the default start method of `multiprocessing` and `ProcessPoolExecutor` is `forkserver`, not `fork`. Code that relies on a child process inheriting state can fail with `NameError` or a pickling error. Windows and macOS still use `spawn`.
- **asyncio.** Code that calls `asyncio.get_event_loop()` outside a running loop to make a loop now fails. Replace the `run_until_complete()` pattern with `asyncio.run()`.[^15]
- **Annotations.** Code that reads `__annotations__` from the class namespace, or reads class annotations through an instance, can break. Use `annotationlib.get_annotations()`.
- **functools.partial.** A `partial` object is now a method descriptor, so as a class attribute it binds `self`. Wrap it in `staticmethod()` to keep the old behaviour.
- **pickle.** `pickle.dump()` and `pickle.dumps()` raise `PicklingError` for some failures that raised other exceptions, such as `AttributeError`. The original exception is chained.
- **typing.** `types.UnionType` is now another name for `typing.Union`.
- **re.** The pattern `\B` on its own now matches the empty string.
- **Removed items.** Code that uses an item of the list above fails with `ImportError`, `AttributeError` or `RuntimeError`. Run the test suite on 3.13 with `-W error::DeprecationWarning` first, to find these uses.

The bugfix release 3.14.5 brings back the generational garbage collector, so `gc.get_threshold()` and the other `gc` statistics have their 3.13 meaning again.[^18] The bugfix release 3.14.10 makes `ssl` reject certificate chains that contain one of a set of compromised eMudhra intermediate CA certificates (CVE-2026-115557).[^19] Upgrade from an older 3.14 release to the newest bugfix release.

## Support dates

The release manager of 3.14 is Hugo van Kemenade.[^2] CPython releases have two phases of support.[^3]

| Phase | From | To |
|---|---|---|
| Bugfix (binary installers, regular releases) | 2025-10-07 | about October 2027 |
| Security (source-only releases, as needed) | about October 2027 | October 2030 |

Development of 3.14 began on 2024-05-08. The first alpha came on 2024-10-15, beta 1 on 2025-05-07 and the first release candidate on 2025-07-22.[^2] PEP 745 plans a bugfix release about every two months, from 3.14.11 on 2026-12-01 to 3.14.16 on 2027-10-05. It marks 3.14.16 as the last regular bugfix release with binary installers.[^2] After that, security fixes come as source releases until about October 2030.[^3]

The dates of the published releases come from the changelog.[^20] For 3.14.8 and 3.14.10, PEP 745 gives a date one day later.

| Release | Date |
|---|---|
| 3.14.0 | 2025-10-07 |
| 3.14.1 | 2025-12-02 |
| 3.14.2 | 2025-12-05 |
| 3.14.3 | 2026-02-03 |
| 3.14.4 | 2026-04-07 |
| 3.14.5 | 2026-05-10 |
| 3.14.6 | 2026-06-10 |
| 3.14.7 | 2026-08-05 |
| 3.14.8 | 2026-09-30 |
| 3.14.9 | 2026-10-21 |
| 3.14.10 | 2026-10-25 |
| 3.14.11 (planned) | 2026-12-01 |

## References
