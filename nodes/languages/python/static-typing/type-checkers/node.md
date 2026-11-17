---
tag: 9Y4Q3D
kind: feature
owner: python
slug: type-checkers
name: Type checkers
aliases: [static type checker, mypy, pyright, pyrefly, ty, mypy --strict, strict mode, reveal_type, type ignore, TYPE_CHECKING]
area: languages/python/static-typing
level: core
summary: >-
  A type checker such as mypy, pyright, Pyrefly or ty reads the hints and reports type errors without running the code. The checkers follow one specification, but their defaults and some rules differ.
grammar: null
spec: {standard: null, url: "https://mypy.readthedocs.io/en/stable/getting_started.html", locator: "mypy, Getting started"}
proposals: [pep-484]
see_also: [python/type-hints, python/basic-type-annotations, python/annotations-at-runtime, python/linters-and-formatters, python/editors-and-language-servers, static-and-dynamic-typing, gradual-types]
sources:
  - url: https://github.com/microsoft/pyright/blob/main/docs/mypy-comparison.md
    title: Differences Between Pyright and Mypy
    locator: Why Does Pyright’s Behavior Differ from Mypy’s?; Type Checking Unannotated Code; Inferred Return Types
    accessed: 2026-11-18
  - url: https://github.com/microsoft/pyright/blob/main/README.md
    title: Pyright, README
    locator: Static Type Checker for Python
    accessed: 2026-11-18
  - url: https://pyrefly.org/
    title: "Pyrefly: A Fast Python Type Checker and Language Server"
    locator: home page
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ty/
    title: ty, Introduction
    locator: Introduction; Highlights
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/getting_started.html
    title: Getting started, mypy documentation
    locator: Dynamic vs static typing; Strict mode and configuration
    accessed: 2026-11-18
  - url: https://github.com/microsoft/pyright/blob/main/docs/configuration.md
    title: Pyright configuration
    locator: typeCheckingMode
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/command_line.html#cmdoption-mypy-strict
    title: The mypy command line, mypy documentation
    locator: "--strict"
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.reveal_type
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.reveal_type(); typing.cast(); typing.TYPE_CHECKING
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0484/
    title: "PEP 484: Type Hints"
    locator: Compatibility with other uses of function annotations, type ignore comments
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/error_codes.html
    title: Error codes, mypy documentation
    locator: Silencing errors based on error codes
    accessed: 2026-11-18
---

## Synopsis

```python run id=reveal
from typing import TYPE_CHECKING

prices = [3, 4]
average = sum(prices) / len(prices)
if TYPE_CHECKING:
    reveal_type(prices)
    reveal_type(average)
print(TYPE_CHECKING, average)
```

```output for=reveal
False 3.5
```

At run time, `TYPE_CHECKING` is `False`, so the `reveal_type` calls do not run. A checker reads the same file and reports the types that it infers. Saved as `reveal.py`:

```text
$ mypy --strict reveal.py
reveal.py:6: note: Revealed type is "list[int]"
reveal.py:7: note: Revealed type is "float"
Success: no issues found in 1 source file
```

## Semantics

**r1.** A type checker reads the source, and it does not run it. It infers the types of the expressions, compares them with the hints and reports each mismatch. Python ignores the hints at run time, so the checker is a separate step, for example in the editor or in continuous integration. For the hints, see [[python/type-hints]].

**r2.** The main checkers:

- mypy. "Mypy is the “OG” in the world of Python type checkers. It was started by Jukka Lehtosalo in 2012".[^1]
- pyright. "Pyright is a full-featured, standards-based static type checker for Python. It is designed for high performance and can be used with large Python source bases."[^2]
- Pyrefly: "A fast type checker and language server for Python with powerful IDE features".[^3]
- ty: "An extremely fast Python type checker and language server, written in Rust."[^4]

**r3.** The checkers follow the typing specification, but each checker has its own behavior where the specification is silent. "Although PEP 484 spells out many type checking behaviors, it intentionally leaves many other behaviors undefined. This approach has allowed different type checkers to innovate and differentiate."[^1] So one program can pass one checker and fail another.

**r4.** The defaults differ. "By default, mypy will not type check dynamically typed functions."[^5] "By default, pyright performs type checking for all code regardless of whether it contains type annotations."[^1] Pyright has the modes "off", "basic", "standard" and "strict", and "The default value for this setting is "standard"."[^6]

**r5.** The strict mode of mypy "enables a defined subset of optional error-checking flags. This subset primarily includes checks for inadvertent type unsoundness".[^7] "If you run mypy with the --strict flag, you will basically never get a type related error at runtime without a corresponding mypy error, unless you explicitly circumvent mypy somehow."[^5] Example: [`none-attr`](#none-attr).

**r6.** Tools that talk to the checker:

- `reveal_type(x)` asks the checker for the type that it infers. "At runtime, this function prints the runtime type of its argument to sys.stderr and returns the argument unchanged".[^8]
- `TYPE_CHECKING` "A special constant that is assumed to be True by static type checkers. It’s False at runtime."[^8] Example: [`guard`](#guard).
- A `# type: ignore` comment silences an error on its line: "The # type: ignore comment should be put on the line that the error refers to".[^9] With mypy, "It’s possible to silence specific error codes on a line using # type: ignore[code]. This way you won’t accidentally ignore other, potentially more serious errors."[^10] Example: [`ignore-code`](#ignore-code).
- `cast(T, value)` changes the type for the checker only. "This returns the value unchanged."[^8] Example: [`cast-lies`](#cast-lies).

## Examples

```python fails id=none-attr expect=exception:AttributeError
def first(names: list[str]) -> str | None:
    return names[0] if names else None


print(first(["ann"]).upper())
print(first([]).upper())
```

The first call works and the second call fails at run time. The checker finds the bug on both lines, because the result can be `None` (rule r5):

```text
$ mypy --strict none_attr.py
none_attr.py:5: error: Item "None" of "str | None" has no attribute "upper"  [union-attr]
none_attr.py:6: error: Item "None" of "str | None" has no attribute "upper"  [union-attr]
Found 2 errors in 1 file (checked 1 source file)
```

```python run id=ignore-code
def double(n: int) -> int:
    return n * 2


print(double("ab"))  # type: ignore[arg-type]
print(double(2.5))
```

```output for=ignore-code
abab
5.0
```

The comment silences the error on line 5 only. Mypy still reports line 6 (rule r6):

```text
$ mypy --strict ignore.py
ignore.py:6: error: Argument 1 to "double" has incompatible type "float"; expected "int"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=guard
import sys
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from fractions import Fraction


def half(value: Fraction) -> Fraction:
    return value / 2


print(TYPE_CHECKING)
print("fractions" in sys.modules)
```

```output for=guard
False
False
```

The checker imports `Fraction` and checks the hints, and the program at run time does not import the module. This works because Python 3.14 does not evaluate the hints when it defines the function (rule r6).

## Versions

The checkers are separate tools with their own releases. Each release can add checks for new typing features. The examples on this page use mypy 2.4.1 with Python 3.14.

## Pitfalls

```python wrong id=cast-lies
import json
from typing import cast

port = cast(int, json.loads("\"80\""))
print(repr(port))
```

```output for=cast-lies
'80'
```

The checker treats `port` as an `int`, but the value is the string `'80'`, and the checker reports nothing (rule r6):

```text
$ mypy --strict cast.py
Success: no issues found in 1 source file
```

Use `cast` only when you know more than the checker. To check a value from outside, use `isinstance`.

```python run id=no-return-hint
def double(n: int):
    return n * 2


result = double(4)
print(result + 1)
```

```output for=no-return-hint
9
```

The function has no return hint. Mypy treats its result as `Any`, and strict mode reports the missing hint. "By comparison, mypy never infers return types and assumes that functions without a return type annotation have a return type of Any."[^1] Pyright infers the return type `int` from the `return` statement.[^1]

```text
$ mypy noreturn.py
Success: no issues found in 1 source file
$ mypy --strict noreturn.py
noreturn.py:1: error: Function is missing a return type annotation  [no-untyped-def]
Found 1 error in 1 file (checked 1 source file)
```

## Comparison

In Python, the type checker is a tool outside the language, and more than one checker exists (rule r2). A language with a static type system has one checker, in its compiler, and a program with a type error does not build. In Python, the program runs whatever the checker says, and two checkers can give different results for one program (rule r3). Pick one checker for a project, set its mode in the configuration and run it on each change. For the linters, see [[python/linters-and-formatters]].
