---
tag: NEW-python
kind: language
slug: python
name: Python
area: languages
level: intro
summary: >-
  Python is a general-purpose language with dynamic, strong typing, automatic memory management and a large standard library. CPython is its reference implementation.
class: programming
tier: 1
paradigms: [imperative, object-oriented, functional, procedural]
typing: {discipline: dynamic, strength: strong, checking: [runtime, optional-static], inference: n/a}
memory: garbage-collected
execution: [interpreted-bytecode, jit]
appeared: 1991
designers: [guido-van-rossum]
influenced_by: [abc-language, modula-3, c++, icon, haskell]
standard: null
reference: {url: "https://docs.python.org/3/reference/", licence: psf-2.0}
licence_of_reference_impl: psf-2.0
file_extensions: [py, pyi, pyw]
hello_world: examples/python.md#hello
same_as: {wikidata: Q28865, wikipedia: Python_(programming_language), pldb: python, linguist: Python, rosetta: Python, hopl: null}
sources:
  - url: https://docs.python.org/3.15/faq/general.html#what-is-python
    title: General Python FAQ, Python 3.15 documentation
    locator: What is Python?; What is Python good for?
    accessed: 2026-11-09
  - url: https://docs.python.org/3.15/faq/general.html#why-was-python-created-in-the-first-place
    title: General Python FAQ, Python 3.15 documentation
    locator: Why was Python created in the first place?
  - url: https://peps.python.org/pep-0020/
    title: "PEP 20: The Zen of Python"
    locator: Abstract; The Zen of Python
  - url: https://docs.python.org/3.15/library/collections.html#collections.Counter.most_common
    title: collections, Container datatypes, Python 3.15 documentation
    locator: Counter objects, most_common
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: Python, extensions
    accessed: 2026-11-09
  - url: https://docs.python.org/3.15/reference/executionmodel.html#naming-and-binding
    title: The Python Language Reference, Python 3.15
    locator: 4.2 Naming and binding
  - url: https://docs.python.org/3.15/library/stdtypes.html#numeric-types-int-float-complex
    title: Built-in Types, Python 3.15 documentation
    locator: Numeric Types, int, float, complex
  - url: https://docs.python.org/3.15/library/typing.html
    title: typing, Support for type hints, Python 3.15 documentation
    locator: the note at the top of the page
  - url: https://docs.python.org/3.15/reference/datamodel.html#objects-values-and-types
    title: The Python Language Reference, Python 3.15
    locator: 3.1 Objects, values and types
  - url: https://docs.python.org/3.15/library/gc.html
    title: gc, Garbage Collector interface, Python 3.15 documentation
    locator: the introduction
  - url: https://docs.python.org/3.15/glossary.html#term-bytecode
    title: Glossary, Python 3.15 documentation
    locator: bytecode
  - url: https://docs.python.org/3.15/whatsnew/3.15.html#upgraded-experimental-jit-compiler
    title: What's New In Python 3.15
    locator: Optimizations, Upgraded experimental JIT compiler
  - url: https://docs.python.org/3.15/reference/introduction.html#alternate-implementations
    title: The Python Language Reference, Python 3.15
    locator: 1.1 Alternate Implementations
  - url: https://docs.python.org/3.15/tutorial/classes.html
    title: The Python Tutorial, Python 3.15
    locator: 9. Classes, the introduction
  - url: https://docs.python.org/3.15/howto/functional.html
    title: Functional Programming HOWTO, Python 3.15 documentation
    locator: Introduction; Generator expressions and list comprehensions
  - url: https://peps.python.org/pep-0602/
    title: "PEP 602: Annual Release Cycle for Python"
    locator: Abstract; Annual release cadence
  - url: https://devguide.python.org/versions/
    title: Status of Python versions, Python Developer's Guide
    locator: Supported versions; Unsupported versions; Status key
    accessed: 2026-11-09
  - url: https://www.python.org/downloads/
    title: Download Python, python.org
    locator: Looking for a specific release?
    accessed: 2026-11-09
  - url: https://peps.python.org/pep-0373/
    title: "PEP 373: Python 2.7 Release Schedule"
    locator: Update (April 2014); Maintenance releases
  - url: https://doc.pypy.org/en/latest/release-v8.0.1.html
    title: PyPy v8.0.1 release notes
    locator: the whole page
    accessed: 2026-11-09
  - url: https://github.com/oracle/graalpython/releases/tag/graal-25.5.5
    title: GraalPy release graal-25.5.5
    locator: the release notes
    accessed: 2026-11-09
  - url: https://github.com/micropython/micropython/blob/master/README.md
    title: MicroPython README
    locator: the introduction; LICENSE
    accessed: 2026-11-09
  - url: https://github.com/adafruit/circuitpython/blob/main/README.rst
    title: CircuitPython README
    locator: the introduction; Differences from MicroPython
    accessed: 2026-11-09
  - url: https://pyodide.org/en/stable/project/changelog.html
    title: Pyodide changelog
    locator: Version 314.0.7; Version 314.0.0, General
    accessed: 2026-11-09
  - url: https://github.com/brython-dev/brython/releases/tag/3.15.0
    title: Brython release 3.15.0
    locator: the release
    accessed: 2026-11-09
  - url: https://github.com/IronLanguages/ironpython3/releases/tag/v3.4.2
    title: IronPython 3 release v3.4.2
    locator: the release
    accessed: 2026-11-09
  - url: https://github.com/jython/jython/releases/tag/v2.7.5rc1
    title: Jython release v2.7.5rc1
    locator: the release; LICENSE.txt
    accessed: 2026-11-09
  - url: https://pypi.org/
    title: The Python Package Index
    locator: the front page
    accessed: 2026-11-09
  - url: https://docs.python.org/3.15/library/ensurepip.html
    title: ensurepip, Bootstrapping the pip installer, Python 3.15 documentation
    locator: the introduction
  - url: https://docs.python.org/3.15/library/venv.html
    title: venv, Creation of virtual environments, Python 3.15 documentation
    locator: the introduction
  - url: https://packaging.python.org/en/latest/specifications/pyproject-toml/
    title: pyproject.toml specification, Python Packaging User Guide
    locator: the introduction
    accessed: 2026-11-09
  - url: https://docs.pytest.org/en/stable/
    title: pytest documentation
    locator: the front page
    accessed: 2026-11-09
  - url: https://docs.astral.sh/ruff/
    title: Ruff documentation
    locator: the front page
    accessed: 2026-11-09
  - url: https://mypy.readthedocs.io/en/stable/
    title: mypy documentation
    locator: the front page
    accessed: 2026-11-09
  - url: https://docs.python.org/3.15/license.html#history-of-the-software
    title: History and License, Python 3.15 documentation
    locator: History of the software
  - url: https://peps.python.org/pep-0200/
    title: "PEP 200: Python 2.0 Release Schedule"
    locator: Release Schedule
  - url: https://docs.python.org/3.15/whatsnew/3.0.html
    title: What's New In Python 3.0
    locator: the introduction
  - url: https://peps.python.org/pep-0013/
    title: "PEP 13: Python Language Governance"
    locator: Current steering council; Creation of this document
    accessed: 2026-11-09
  - url: https://peps.python.org/pep-0255/
    title: "PEP 255: Simple Generators"
    locator: Motivation
  - url: https://docs.python.org/3.15/reference/lexical_analysis.html#indentation
    title: The Python Language Reference, Python 3.15
    locator: 2.1.8 Indentation
---

## Overview

Python is a general-purpose, high-level language. Its FAQ describes it as an interpreted, interactive and object-oriented language with modules, exceptions, dynamic typing, very high-level data types and classes. It also supports procedural and functional programming.[^1]

Guido van Rossum started Python at CWI in Amsterdam in the Christmas holidays of 1989. He wanted a scripting language for system administration on the Amoeba operating system, with a syntax like ABC and a design that users could extend. The grouping of statements by indentation and the very high-level data types come from his work in the ABC group.[^2]

PEP 20, "The Zen of Python" by Tim Peters, states the design goals as 19 short aphorisms. They ask for readable code and for explicit code over implicit code. Errors must not pass silently, and there should be one obvious way to do a job.[^3]

The standard library is large. It covers text processing, internet protocols, testing, logging and the interfaces of the operating system. Third-party packages come from the Python Package Index (PyPI).[^1]

## A first program

```python run id=hello
print("Hello, world!")
```

```output for=hello
Hello, world!
```

The program below counts the words of a text and prints the three most frequent ones. It uses a generator expression with two `for` clauses and `collections.Counter` from the standard library. It also uses tuple unpacking in a `for` loop and an f-string with a format specification.

```python run id=words
from collections import Counter

text = """the quick brown fox
jumps over the lazy dog
and the dog sleeps"""

counts = Counter(word for line in text.splitlines() for word in line.split())
for word, n in counts.most_common(3):
    print(f"{word:<6}{n}")
```

```output for=words
the   3
dog   2
quick 1
```

Five words occur once. `most_common` puts elements with equal counts in the order in which it first saw them, so `quick` is third.[^4]

## The language at a glance

A source file has the extension `.py`. A stub file, which holds only type hints for type checkers, has `.pyi`. On Windows, a file with `.pyw` runs with no console window.[^5]

**Types.** Each value is an object, and each object has a type. A name has no type. An assignment binds the name to an object, and a later assignment can bind it to an object of another type.[^6] The interpreter checks types when an operation runs, and it does not convert between unrelated types on its own. Integers have unlimited precision.[^7]

```python run id=types
x = 1
print(type(x).__name__)
x = "one"
print(type(x).__name__)
try:
    print(1 + x)
except TypeError as err:
    print(err)
print(2**100)
```

```output for=types
int
str
unsupported operand type(s) for +: 'int' and 'str'
1267650600228229401496703205376
```

Type hints are optional. The runtime does not enforce them; separate type checkers, IDEs and linters read them.[^8]

**Memory.** A program never frees memory by hand. The language requires only that the implementation does not collect objects that are still reachable.[^9] CPython frees most objects through reference counting as soon as the last reference goes away. The cycle collector of the `gc` module finds groups of objects that refer to each other.[^10] Other implementations use other collectors, so a program must not depend on the time at which an object is finalized.[^9]

**Execution.** CPython compiles source code to bytecode and runs the bytecode on a virtual machine. It caches the bytecode in `.pyc` files, and the bytecode can change between releases.[^11] CPython has an experimental JIT compiler.[^12] PyPy runs Python with a JIT compiler.[^13]

**Paradigms.** A module can be a list of statements and functions. Classes support multiple inheritance, and their design mixes ideas from C++ and Modula-3.[^14] Functions are objects, and comprehensions, generators and the `itertools` and `functools` modules support a functional style.[^15]

## The guide

The guide to Python has the 25 parts of the language template. Each row says what the part covers for Python. Two pages exist so far, and the table links them.

| Part | What it covers in Python |
|---|---|
| Overview and history | this page: the design goals, the versions, the implementations, the history |
| Getting started | installing CPython, the `python` command and the REPL, running a script, a virtual environment |
| Lexical structure and syntax | logical lines and indentation, soft keywords, literals, operator precedence, the PEG grammar |
| Values and types | `int`, `float`, `complex`, `bool`, `str`, `bytes`, `None`, the standard type hierarchy |
| Variables, binding and scope | name binding, local, enclosing, global and built-in scopes, `global` and `nonlocal`, assignment expressions |
| Control flow | `if`, `for` and `while` with `else`, `break` and `continue`, [the match statement](/python/match-statement/) |
| Functions | `def`, positional-only and keyword-only parameters, `*args` and `**kwargs`, defaults, closures, `lambda` |
| Built-in collections | `list`, `tuple`, `dict`, `set`, `frozenset`, `frozendict`, comprehensions, the iterator protocol |
| Strings and text | `str` as a sequence of code points, `bytes`, f-strings and t-strings, `re`, codecs and UTF-8 mode |
| User-defined types | classes, data classes, `enum`, protocols, generics with type parameters |
| Errors and exceptions | `try`, `except`, `else` and `finally`, the exception hierarchy, exception groups and `except*` |
| Memory and resources | reference counting, the cycle collector, `with` and context managers, `weakref` |
| Modules, packages and builds | `import`, packages, lazy imports, `pip`, `pyproject.toml`, wheels |
| Concurrency and parallelism | `threading`, `multiprocessing`, `asyncio`, `concurrent.futures`, the GIL and the free-threaded build |
| Metaprogramming | decorators, descriptors, metaclasses, `__getattr__`, `inspect`, `ast` |
| The standard library | a map of the modules; the api cards hang here |
| I/O, files and the OS | `open`, `pathlib`, `os`, `subprocess`, `datetime`, `socket` |
| Testing | `unittest`, `doctest`, `unittest.mock`, pytest |
| Tooling | the REPL, `pdb`, the `profiling` package, linters, formatters, type checkers |
| Ecosystem | PyPI, the Python Software Foundation, the Steering Council and the PEP process, libraries by need |
| Performance | the cost model of CPython, `timeit`, profiling, the JIT, extension modules |
| Interoperability | the C API and the stable ABI, `ctypes`, embedding CPython |
| Idioms and style | PEP 8, "easier to ask forgiveness than permission", iteration without indexes, context managers |
| Pitfalls | mutable default arguments, late binding in closures, `is` and `==`, a bare name in a `case` pattern |
| Versions and migration | the release pages, such as [Python 3.15](/python/3.15/), the move from Python 2, deprecations and removals |

## Versions and support

CPython puts out one feature release each year, in October.[^16] A feature release gets bugfix releases about every two months for two years (18 months for versions before 3.13), then security fixes only. Support ends five years after the first release.[^17] The pages of this guide state facts for Python 3.11 to 3.15, the versions that the core team supports on 2026-11-09. A fact that holds only from a later version gives that version.

| Version | Status | First release | Newest release | End of life |
|---|---|---|---|---|
| 3.16 | in development | 2027-10-06 (planned) | none | 2032-10 |
| 3.15 | bugfix | 2026-10-09 | 3.15.2 (2026-10-25) | 2031-10 |
| 3.14 | bugfix | 2025-10-07 | 3.14.10 (2026-10-25) | 2030-10 |
| 3.13 | security | 2024-10-07 | 3.13.18 (2026-10-25) | 2029-10 |
| 3.12 | security | 2023-10-02 | 3.12.16 (2026-10-25) | 2028-10 |
| 3.11 | security | 2022-10-24 | 3.11.18 (2026-10-25) | 2027-10 |
| 3.10 | end of life | 2021-10-04 | 3.10.22 (2026-10-01) | 2026-10-01 |

The statuses and the end-of-life dates come from the developer's guide.[^17] The newest releases come from python.org.[^18] Support for Python 2 ended on 2020-01-01. Its last release, 2.7.18, came out on 2020-04-20.[^19]

## Implementations

The language reference names CPython as the original implementation and says that new features usually appear there first.[^13] Each implementation below follows a Python version; the table gives the newest release on 2026-11-09.

| Implementation | Runs on | Python version | Newest release | Licence |
|---|---|---|---|---|
| CPython | C, on most operating systems | 3.15 | 3.15.2 (2026-10-25) | PSF-2.0 |
| PyPy | most operating systems, with a JIT compiler | 2.7, 3.11 and 3.12 (beta) | 8.0.1 (2026-10-14) | MIT |
| GraalPy | GraalVM; a native standalone build, or embedded in a JVM application | 3.13 | 25.5.5 (2026-11-03) | UPL-1.0 |
| MicroPython | microcontrollers and small systems | the 3.4 syntax with some later features | 1.29.0 (2026-08-24) | MIT |
| CircuitPython | microcontroller boards; a fork of MicroPython | as MicroPython | 10.3.1 (2026-09-14) | MIT |
| Pyodide | browsers and Node.js; CPython compiled to WebAssembly | 3.14 | 314.0.7 (2026-09-14) | MPL-2.0 |
| Brython | browsers; Python translated to JavaScript | 3.15 | 3.15.0 (2026-10-10) | BSD-3-Clause |
| IronPython | .NET | 3.4 | 3.4.2 (2024-12-20) | Apache-2.0 |
| Jython | the JVM | 2.7 | 2.7.5rc1 (2026-11-07, a release candidate) | PSF-2.0 |

Sources: PyPy[^20], GraalPy[^21], MicroPython[^22], CircuitPython[^23], Pyodide[^24], Brython[^25], IronPython[^26] and Jython[^27]. The compat tables of the feature pages have columns for CPython, PyPy, GraalPy and MicroPython.

## Ecosystem

PyPI is the public index of Python packages.[^28] The `pip` installer is a separate project, and CPython installs it by default through `ensurepip`.[^29] The `venv` module makes a virtual environment: an isolated set of packages on top of one Python installation.[^30] A project describes its build system, its metadata and the settings of its tools in a `pyproject.toml` file.[^31]

| Need | Tool |
|---|---|
| install packages | `pip`[^29] |
| isolate the packages of a project | `venv`[^30] |
| describe a project | `pyproject.toml`[^31] |
| run tests | `unittest` in the standard library; pytest[^32] |
| lint and format | Ruff[^33] |
| check types statically | mypy[^34] |

The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

Guido van Rossum made Python at CWI in the Netherlands as a successor of ABC. He continued the work at CNRI in Reston, Virginia, from 1995. In May 2000 the core team moved to BeOpen.com, and in 2001 the Python Software Foundation was formed to own the intellectual property of Python.[^35] The versions 0.9.0 to 1.2 came out from 1991 to 1995.[^35]

| Date | Event |
|---|---|
| February 1991 | the first public post, to USENET[^2] |
| 2000-10-16 | Python 2.0[^36] |
| 2008-12-03 | Python 3.0, the first release that broke backward compatibility on purpose[^37] |
| July 2018 | Guido van Rossum stepped down as "Benevolent Dictator for Life"[^38] |
| 2020-01-01 | the end of support for Python 2[^19] |

PEP 13 defines the governance of Python. An elected Steering Council of five people leads the project. The council for 2026 is Barry Warsaw, Donghee Na, Pablo Galindo Salgado, Savannah Ostrowski and Thomas Wouters.[^38]

Python took its exceptions from Modula-3 and its indentation from ABC.[^2] Its classes mix ideas from C++ and Modula-3.[^14] List comprehensions come from Haskell.[^15] Generators follow the generators of Icon.[^39]

## Coming from another language

- Indentation groups statements. There are no braces and no `end` keywords.[^40]
- A variable has no declaration and no type. An assignment binds a name to an object.[^6]
- `int` has no fixed size, so integer arithmetic does not overflow.[^7]
- A `match` statement has no fall through, and a bare name in a pattern captures the value. See [the match statement](/python/match-statement/).

## References
