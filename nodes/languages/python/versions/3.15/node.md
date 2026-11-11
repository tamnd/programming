---
tag: CTSV9K
kind: release
owner: python
version: "3.15"
name: Python 3.15
area: languages/python/versions
level: core
summary: >-
  Python 3.15 came out on 2026-10-09. It adds lazy imports, frozendict, sentinel and a profiling package, and it makes UTF-8 the default text encoding.
released: 2026-10-09
eol: 2031-10
patches:
  - {v: 3.15.0, date: 2026-10-09}
  - {v: 3.15.1, date: 2026-10-21}
  - {v: 3.15.2, date: 2026-10-25}
adds: [lazy-imports, frozendict, sentinel, unpacking-in-comprehensions, utf8-mode-default, profiling-package, math-integer, startup-configuration-files, typeddict-extra-items, typeform, disjoint-base, re-prefixmatch, bytearray-take-bytes, match-unary-plus, attribute-error-hints, frame-pointers-default, abi3t, pybyteswriter, interpreter-finalization-guards]
removes: [sre-modules, cgi-http-request-handler, loader-load-module, zipimporter-load-module, purepath-is-reserved, platform-java-ver, glob-glob0-glob1, ctypes-setpointertype, code-co-lnotab, namedtuple-keyword-syntax, typeddict-empty-call, no-type-check-decorator, wave-markers, sysconfig-check-home, importlib-resources-files-package, rlock-arguments, c-api-legacy-config-getters]
deprecates: [profile-module, bytestring-abc, abstract-ast-nodes, hashlib-string-keyword, re-match-soft, struct-no-argument-new, module-version-attributes, webbrowser-macosx-osascript, bytes-warning-options, base64-alt-alphabet-plus-slash, http-cookies-js-output, imaplib-file-assignment, pygilstate-soft]
notes: {url: "https://docs.python.org/3.15/whatsnew/3.15.html"}
sources:
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#summary-release-highlights
    title: What's New In Python 3.15
    locator: Summary, release highlights
    accessed: 2026-11-09
  - url: https://peps.python.org/pep-0790/
    title: "PEP 790: Python 3.15 Release Schedule"
    locator: Release schedule; Bugfix releases; 3.15 lifespan
    accessed: 2026-11-09
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions
    accessed: 2026-11-09
  - url: https://peps.python.org/pep-0810/
    title: "PEP 810: Explicit lazy imports"
    locator: Specification
  - url: https://peps.python.org/pep-0814/
    title: "PEP 814: Add frozendict built-in type"
    locator: Specification
  - url: https://peps.python.org/pep-0661/
    title: "PEP 661: Sentinel Values"
    locator: Specification
  - url: https://peps.python.org/pep-0798/
    title: "PEP 798: Unpacking in Comprehensions"
    locator: Specification
  - url: https://peps.python.org/pep-0686/
    title: "PEP 686: Make UTF-8 mode default"
    locator: Specification
  - url: https://peps.python.org/pep-0799/
    title: "PEP 799: A dedicated profiling package for organizing Python profiling tools"
    locator: Specification
  - url: https://peps.python.org/pep-0791/
    title: "PEP 791: math.integer, submodule for integer-specific mathematics functions"
    locator: Specification
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#other-language-changes
    title: What's New In Python 3.15
    locator: Other language changes
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#improved-error-messages
    title: What's New In Python 3.15
    locator: Improved error messages
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#typing
    title: What's New In Python 3.15
    locator: Improved modules, typing
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#upgraded-experimental-jit-compiler
    title: What's New In Python 3.15
    locator: Optimizations, Upgraded experimental JIT compiler
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#removed
    title: What's New In Python 3.15
    locator: Removed
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#new-deprecations
    title: What's New In Python 3.15
    locator: Deprecated, New deprecations
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#removed-c-apis
    title: What's New In Python 3.15
    locator: C API changes, Removed C APIs
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#porting-to-python-3-15
    title: What's New In Python 3.15
    locator: Porting to Python 3.15
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#notable-changes-in-3-15-2
    title: What's New In Python 3.15
    locator: Notable changes in 3.15.2
  - url: https://docs.python.org/3.15/whatsnew/changelog.html
    title: Changelog, Python 3.15 documentation
    locator: Python 3.15.2 final, Python 3.15.1 final, Python 3.15.0 final
    accessed: 2026-11-09
---

## What is new

Python 3.15.0 was released on 2026-10-09.[^1] The examples on this page ran on CPython 3.15.2. Two of them, on UTF-8 mode and on error messages, also run on 3.14 but print other output; the others fail on 3.14.

### Lazy imports (PEP 810)

The soft keyword `lazy` before an `import` or `from ... import` statement defers the import. The name is bound to a proxy, and the module loads on the first use of that name.[^4] This cuts the start-up time of programs that import many modules but use few of them on a given run.

```python run id=lazy-import since=3.15
import sys

lazy import json

print("json" in sys.modules)
print(json.dumps({"a": 1}))
print("json" in sys.modules)
```

```output for=lazy-import
False
{"a": 1}
True
```

A lazy import is allowed only at module level. It is a `SyntaxError` inside a function, a class body or a `try` block, and with `from ... import *` or `from __future__`.[^4] The option `-X lazy_imports`, the variable `PYTHON_LAZY_IMPORTS` and `sys.set_lazy_imports()` control the mode for the whole program.[^1]

### frozendict (PEP 814)

The built-in type `frozendict` is an immutable mapping. It keeps insertion order, and it is hashable when all its values are hashable, so it can be a dict key or a set member.[^5] It is not a subclass of `dict`.

```python run id=frozendict since=3.15
config = frozendict(host="localhost", port=8080)
print(config["port"])
print(hash(config) == hash(frozendict(port=8080, host="localhost")))
print(config == {"port": 8080, "host": "localhost"})
print(isinstance(config, dict))
try:
    config["port"] = 9090
except TypeError as error:
    print("TypeError:", error)
```

```output for=frozendict
8080
True
True
False
TypeError: 'frozendict' object does not support item assignment
```

Equality ignores the order of the keys, as for `dict`. A `frozendict` also matches a mapping pattern of a `match` statement, and it is one of the built-in classes that a class pattern such as `frozendict(x)` matches as a whole.

### sentinel (PEP 661)

The built-in `sentinel(name)` makes a unique marker object with a readable repr. It replaces the `MISSING = object()` idiom for "no argument given", where `None` is a valid value.[^6]

```python run id=sentinel since=3.15
MISSING = sentinel("MISSING")
print(MISSING)


def lookup(table, key, default=MISSING):
    if key in table:
        return table[key]
    if default is MISSING:
        raise KeyError(key)
    return default


print(lookup({"a": 1}, "b", None))
```

```output for=sentinel
MISSING
None
```

### Unpacking in comprehensions (PEP 798)

A list, set or generator comprehension can start with `*expr`, and a dict comprehension can start with `**expr`. Each item is unpacked into the result. This flattens one level without a second `for` clause.[^7]

```python run id=unpack-comprehension since=3.15
rows = [[1, 2], [3], [4, 5, 6]]
print([*row for row in rows])
print({*row for row in rows})
parts = [{"a": 1}, {"b": 2}, {"a": 3}]
print({**part for part in parts})
```

```output for=unpack-comprehension
[1, 2, 3, 4, 5, 6]
{1, 2, 3, 4, 5, 6}
{'a': 3, 'b': 2}
```

In 3.14 the same code is a `SyntaxError` with the message "iterable unpacking cannot be used in comprehension".

### UTF-8 is the default encoding (PEP 686)

UTF-8 mode is on by default. A call to `open()`, or another text I/O function, with no `encoding` argument now uses UTF-8 on every platform, not the encoding of the locale.[^8] The variable `PYTHONUTF8=0` or the option `-X utf8=0` turns the mode off. The argument `encoding="locale"` asks for the locale encoding in one call.[^11]

```python run id=utf8-mode version=3.15
import sys

print(sys.flags.utf8_mode)
```

```output for=utf8-mode
1
```

On 3.14 the same program prints `0`, unless the mode is turned on or the locale is `C` or `POSIX`.

### The profiling package (PEP 799)

The package `profiling` groups the profilers of the standard library. `profiling.tracing` is the deterministic profiler that `cProfile` provides, and `profiling.sampling` is a statistical sampling profiler that can attach to a running process.[^9] The module `profile` is deprecated (see below), and `cProfile` stays as another name for `profiling.tracing`.[^1]

### math.integer (PEP 791)

The submodule `math.integer` holds the functions of `math` that work only on integers: `comb`, `factorial`, `gcd`, `isqrt`, `lcm` and `perm`.[^10]

```python run id=math-integer since=3.15
import math.integer as mi

print(mi.isqrt(10**20 + 1))
print(mi.comb(10, 3), mi.gcd(12, 18))
print([n for n in dir(mi) if not n.startswith("_")])
```

```output for=math-integer
10000000000
120 6
['comb', 'factorial', 'gcd', 'isqrt', 'lcm', 'perm']
```

### Smaller language and library changes

- A literal pattern in a `match` statement may start with a unary plus, as in `case +1:`, to mirror the unary minus.[^11] See [the match statement](/python/match-statement/).
- `re.prefixmatch()` and `Pattern.prefixmatch()` are other names for `re.match()` and `Pattern.match()`. The name says that the match is anchored at the start of the string only.[^1]
- `bytearray.take_bytes(n)` removes the first `n` bytes from the buffer and returns them as `bytes` without a copy. With no argument it takes the whole buffer.[^11]
- PEP 829 adds `.start` files. Each one names a callable that runs at start-up, so a package no longer needs an `import` line in a `.pth` file for this. Those `import` lines are deprecated, with no warning yet.[^1]

```python run id=prefix-and-take since=3.15
import re

print(re.prefixmatch(r"\d+", "123abc"))
print(re.prefixmatch(r"\d+", "abc123"))

buffer = bytearray(b"header:body")
print(buffer.take_bytes(7), buffer)
```

```output for=prefix-and-take
<re.Match object; span=(0, 3), match='123'>
None
b'header:' bytearray(b'body')
```

### Better error messages

When an `AttributeError` on a built-in type has no close match, the message now names the Python method that does the same job in other languages. For example, `push` suggests `append`, and `toUpperCase` suggests `upper`.[^12]

```python run id=attribute-hints version=3.15
import traceback

for action in (lambda: [1, 2, 3].push(4), lambda: "hi".toUpperCase()):
    try:
        action()
    except AttributeError as error:
        print(traceback.format_exception_only(error)[-1], end="")
```

```output for=attribute-hints
AttributeError: 'list' object has no attribute 'push'. Did you mean '.append'?
AttributeError: 'str' object has no attribute 'toUpperCase'. Did you mean '.upper'?
```

On 3.14 the same program prints the two messages with no hint.

### Typing

- PEP 728: a `TypedDict` can be `closed=True`, or it can declare the type of its other keys with `extra_items=`.[^13]
- PEP 747: `typing.TypeForm` annotates a value that is itself a type expression.[^13]
- PEP 800: the decorator `typing.disjoint_base` marks a class that cannot share a subclass with another disjoint base.[^13]

### Interpreter, C API and build

- The experimental JIT compiler has a tracing front end and register allocation. On x86-64 Linux, the pyperformance suite runs 7 to 8 percent faster (geometric mean) with the JIT than with the fully optimized interpreter.[^14]
- PEP 831: CPython is built with frame pointers by default, so system profilers and debuggers can walk Python stacks.[^1]
- PEP 803: the stable ABI `abi3t` lets one extension binary work on free-threaded builds.[^1]
- PEP 782 adds the `PyBytesWriter` API, and PEP 788 adds calls that protect C code from interpreter finalization.[^1]
- The official macOS binaries install free-threading support by default. The official 64-bit Windows binaries use the tail-calling interpreter, built with Visual Studio 2026.[^1]

## Removed and deprecated

### Removed in 3.15

Most items below were deprecated in an earlier release.[^15]

- `sre_compile`, `sre_constants` and `sre_parse`: the modules are gone. Use `re`.
- `http.server.CGIHTTPRequestHandler` and the `--cgi` option of `python -m http.server`.
- `importlib.abc.Loader.load_module()` and `zipimport.zipimporter.load_module()`. Use `exec_module()`.
- The `package` parameter of `importlib.resources.files()`.
- `pathlib.PurePath.is_reserved()`. Use `os.path.isreserved()` on Windows.
- `platform.java_ver()`.
- `glob.glob0()` and `glob.glob1()`. Use `glob.glob()` with `root_dir`.
- `ctypes.SetPointerType()`.
- The `co_lnotab` attribute of code objects. Use `co_lines()`.
- The `check_home` parameter of `sysconfig.is_python_build()`.
- Arbitrary arguments to the C implementation of `threading.RLock()`.
- In `typing`: the keyword-argument form of `NamedTuple("Name", x=int)`, the calls `TypedDict("TD")` and `TypedDict("TD", None)`, and `no_type_check_decorator()`.
- `ByteString` is no longer in `__all__` of `collections.abc` and `typing`. The name itself goes in 3.17.
- In `wave`: `getmark()`, `setmark()` and `getmarkers()`.
- The constructors of `ast` nodes raise `TypeError` for a missing required argument or an unknown keyword argument.
- `datetime.strptime()` raises `ValueError` for a format with `%d` and no year.
- `mmap.mmap.resize()` is absent on platforms that cannot resize a mapping.
- C API: `PyImport_ImportModuleNoBlock()`, `PyWeakref_GetObject()` and `PyWeakref_GET_OBJECT()`, `PySys_ResetWarnOptions()`, the four `PyUnicode_AsDecoded*` and `PyUnicode_AsEncoded*` functions, and `Py_GetPath()` with the other `Py_Get*` configuration getters. Use `PyConfig_Get()` for the getters.[^17]
- Build: CPython no longer falls back to its bundled copy of `libmpdec` on its own. Configure with `--without-system-libmpdec` to use the bundled copy.

### Deprecated in 3.15

These now warn, or are marked deprecated in the docs. The version in brackets is the planned removal.[^16]

- `profile` (3.17). Use `profiling.tracing`.
- `collections.abc.ByteString` and `typing.ByteString` warn on access (3.17).
- `webbrowser.MacOSXOSAScript` (3.17).
- The options `-b` and `-bb` become no-ops in 3.17.
- In `hashlib`, the `string=` keyword of the constructors (3.19).
- `http.cookies.BaseCookie.js_output()` and `Morsel.js_output()` (3.19).
- Assignment to `imaplib.IMAP4.file` (3.19).
- Creating an instance of an abstract `ast` node class (an error in 3.20).
- `struct.Struct.__new__()` with no argument, and a second `__init__()` call (3.20).
- `__version__` attributes in many standard modules (3.20). Use `sys.version_info`.
- `isinstance()` checks against a protocol that is not decorated with `runtime_checkable` but inherits from one that is (3.20).
- In `base64.b64decode()` and `urlsafe_b64decode()`, the characters `+` and `/` in the input when an alternative alphabet is given.
- Soft deprecations, with no removal date: `re.match()` in favour of `re.prefixmatch()`, the `struct` format codes `F` and `D` in favour of `Zf` and `Zd`, and the `PyGILState` C API.

## Migration

Most code that runs on 3.14 without warnings runs on 3.15. These changes can break it.[^18]

- **Text encoding.** A file opened with no `encoding` was read with the locale encoding. It is now read as UTF-8. On Windows with a legacy code page, this changes how non-ASCII text decodes. Pass `encoding=` on each `open()`, or run with `-X utf8=0` while you port.
- **sqlite3.** All parameters of `sqlite3.connect()` except `database` are keyword-only. The first three parameters of `Connection.create_function()` and `create_aggregate()` are positional-only, and so is the first parameter of `set_authorizer()`, `set_progress_handler()` and `set_trace_callback()`.
- **argparse.** For `add_argument("-f", "-foo")`, the destination is now `foo`, taken from the single-dash long option. It was `f` before.
- **base64.** `urlsafe_b64decode()` accepts input with no padding. Pass `padded=True` to require it.
- **unittest.** `assertWarns()` no longer swallows warnings that do not match.
- **resource.** `RLIM_INFINITY` is always a positive number.
- **xml.etree.** An `iterparse()` iterator that is not exhausted or closed emits `ResourceWarning`.
- **Removed modules and functions.** Code that uses an item of the list above fails with `ImportError` or `AttributeError`. Run the test suite on 3.14 with `-W error::DeprecationWarning` first, to find these uses before you upgrade.
- **`__cached__`.** The import system no longer sets the module attribute `__cached__`. Use `__spec__.cached`.[^11]

The bugfix release 3.15.2 fixes a deadlock in `importlib` that 3.15.0 introduced, when threads import submodules of the same package for the first time. It also makes `ssl` reject certificate chains that contain one of a set of compromised eMudhra intermediate CA certificates (CVE-2026-115557).[^19] Upgrade from 3.15.0 or 3.15.1 to the newest bugfix release.

## Support dates

The release manager of 3.15 is Hugo van Kemenade.[^2] CPython releases have two phases of support.[^3]

| Phase | From | To |
|---|---|---|
| Bugfix (binary installers, regular releases) | 2026-10-09 | about October 2028 |
| Security (source-only releases, as needed) | about October 2028 | October 2031 |

PEP 790 plans a bugfix release about every two months, from 3.15.3 on 2026-12-01 to 3.15.14 on 2028-10-03. It marks 3.15.14 as the last regular bugfix release with binary installers.[^2] After that, security fixes come as source releases until five years after 3.15.0.[^3]

The dates of the published releases come from the changelog.[^20] PEP 790 planned 3.15.2 for 2026-10-26, but the release came out one day earlier.

| Release | Date |
|---|---|
| 3.15.0 | 2026-10-09 |
| 3.15.1 | 2026-10-21 |
| 3.15.2 | 2026-10-25 |
| 3.15.3 (planned) | 2026-12-01 |

## References
