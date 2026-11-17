---
tag: NEW-python-type-hints
kind: feature
owner: python
slug: type-hints
name: Type hints
aliases: [type hint, type annotations, typing, PEP 484, gradual typing, static type checking, optional typing]
area: languages/python/static-typing
level: core
summary: >-
  A type hint is an annotation that tells a static type checker the expected type of a name. Python does not check hints at run time. A separate tool, such as mypy, checks them before the program runs.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/concepts.html", locator: "Type system concepts, Static, dynamic, and gradual typing"}
proposals: [pep-484, pep-526, pep-649]
see_also: [python/function-annotations, python/annotated-assignment, python/annotations-at-runtime, python/annotation-scopes, python/duck-typing, python/dynamic-and-strong-typing, gradual-types, static-and-dynamic-typing]
sources:
  - url: https://docs.python.org/3.15/library/typing.html
    title: typing, Support for type hints, Python 3.15 documentation
    locator: introduction, Note
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/glossary.html#term-type-hint
    title: Glossary, Python 3.15 documentation
    locator: annotation; static type checker; type hint
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0484/
    title: "PEP 484: Type Hints"
    locator: Abstract; Rationale and Goals; Non-goals
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/concepts.html
    title: Type system concepts, Specification for the Python type system
    locator: Static, dynamic, and gradual typing
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0526/
    title: "PEP 526: Syntax for Variable Annotations"
    locator: Specification, local variables
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/getting_started.html
    title: Getting started, mypy documentation
    locator: Dynamic vs static typing; Strict mode and configuration
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.14.html#pep-649-pep-749-deferred-evaluation-of-annotations
    title: What’s New In Python 3.14
    locator: PEP 649 and PEP 749, Deferred evaluation of annotations
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=not-enforced
def area(width: float, height: float) -> float:
    return width * height


print(area(2, 3))
print(area("ab", 3))
```

```output for=not-enforced
6
ababab
```

The hints say that `area` takes two `float` values, but Python runs the second call and returns a string. A static type checker finds the error before the program runs. Saved as `area.py`, the code gives this result with mypy:

```text
$ mypy --strict area.py
area.py:6: error: Argument 1 to "area" has incompatible type "str"; expected "float"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Semantics

**r1.** A type hint is "An annotation that specifies the expected type for a variable, a class attribute, or a function parameter or return value."[^2] An annotation is "A label associated with a variable, a class attribute or a function parameter or return value, used by convention as a type hint."[^2]

**r2.** Python does not check hints. "The Python runtime does not enforce function and variable type annotations. They can be used by third party tools such as type checkers, IDEs, linters, etc."[^1] PEP 484 says: "no type checking happens at runtime. Instead, the proposal assumes the existence of a separate off-line type checker which users can run over their source code voluntarily."[^3] Example: [`not-enforced`](#not-enforced).

**r3.** A static type checker is "An external tool that reads Python code and analyzes it, looking for issues such as incorrect types."[^2] For the tools, see [[python/type-checkers]].

**r4.** Hints are optional. "It should also be emphasized that Python will remain a dynamically typed language, and the authors have no desire to ever make type hints mandatory, even by convention."[^3]

**r5.** The type system is gradual. "Gradual typing is a way to combine static and dynamic typing. Type-annotated Python allows opting in to static type checking at a fine level of granularity, so that some type errors can be caught statically, without running the program."[^4] The type `Any` marks a value whose type is not known: "Because Any indicates a statically unknown type, the static type checker can’t check type correctness of operations on expressions typed as Any."[^4] Example: [`any-hides`](#any-hides).

**r6.** Hints go in these places:

- on a parameter, after the name and a colon: `width: float`
- on the return value, after `->`: `-> float`
- on a variable or a class attribute, in an annotated assignment: `count: int = 0`

For the syntax, see [[python/function-annotations]] and [[python/annotated-assignment]].

**r7.** Hints of modules, classes and functions are kept, and a program can read them. "Type hints of global variables, class attributes, and functions, but not local variables, can be accessed using typing.get_type_hints()."[^2] "Annotations for local variables will not be evaluated".[^5] Examples: [`stored`](#stored), [`local-not-evaluated`](#local-not-evaluated).

**r8.** A checker treats a function with no hints as dynamic code. "By default, mypy will not type check dynamically typed functions. This means that with a few exceptions, mypy will not report any errors with regular unannotated Python."[^6] Strict mode adds checks: "If you run mypy with the --strict flag, you will basically never get a type related error at runtime without a corresponding mypy error, unless you explicitly circumvent mypy somehow."[^6] Example: [`untyped`](#untyped).

## Examples

```python run id=stored
import typing


class Point:
    x: int
    y: int = 0


def scale(p: Point, factor: float = 1.0) -> Point:
    return p


print(scale.__annotations__)
print(typing.get_type_hints(Point))
print(hasattr(Point, "x"), Point.y)
```

```output for=stored
{'p': <class '__main__.Point'>, 'factor': <class 'float'>, 'return': <class '__main__.Point'>}
{'x': <class 'int'>, 'y': <class 'int'>}
False 0
```

The function and the class keep their hints. The hint `x: int` with no value makes no attribute, so `Point` has no `x` (rule r7).

```python run id=deferred
from annotationlib import Format, get_annotations


def send(order: Order) -> None:
    pass


print(get_annotations(send, format=Format.STRING))


class Order:
    pass


print(get_annotations(send))
```

```output for=deferred
{'order': 'Order', 'return': 'None'}
{'order': <class '__main__.Order'>, 'return': None}
```

From Python 3.14, a hint can name a class that is defined later in the module. Python evaluates the hints only when a program asks for them.[^7]

```python run id=local-not-evaluated
def total() -> int:
    count: NoSuchName = 3
    return count


print(total())
```

```output for=local-not-evaluated
3
```

Python does not evaluate the hint of a local variable, so the unknown name causes no error at run time (rule r7). The checker finds it:

```text
$ mypy --strict local.py
local.py:2: error: Name "NoSuchName" is not defined  [name-defined]
local.py:3: error: Returning Any from function declared to return "int"  [no-any-return]
Found 2 errors in 1 file (checked 1 source file)
```

## Versions

> [!since v="3.5"]
> The `typing` module is added, from PEP 484.[^1][^3]

> [!since v="3.6"]
> Variable annotations are added, from PEP 526.[^5]

> [!since v="3.14"]
> "The annotations on functions, classes, and modules are no longer evaluated eagerly. Instead, annotations are stored in special-purpose annotate functions and evaluated only when necessary (except if from __future__ import annotations is used)."[^7] "It is no longer necessary to enclose annotations in strings if they contain forward references."[^7] Example: [`deferred`](#deferred).

## Pitfalls

```python wrong id=any-hides
import json
from typing import Any


def port(text: str) -> int:
    config: Any = json.loads(text)
    value: int = config["port"]
    return value


print(repr(port("{\"port\": \"80\"}")))
```

```output for=any-hides
'80'
```

The function says that it returns an `int`, but it returns the string `'80'`. The checker accepts the code, because a value of type `Any` can go to a name of any type (rule r5):

```text
$ mypy --strict any_hides.py
Success: no issues found in 1 source file
```

Check the data from outside, for example with `isinstance`, before you give it a precise type.

```python fails id=untyped expect=exception:TypeError
def label(count):
    return "items: " + count


def label_typed(count: int) -> str:
    return "items: " + str(count)


print(label_typed(3))
print(label(3))
```

The function `label` has no hints, so mypy with no options does not check its body (rule r8). Strict mode reports the function:

```text
$ mypy untyped.py
Success: no issues found in 1 source file
$ mypy --strict untyped.py
untyped.py:1: error: Function is missing a type annotation  [no-untyped-def]
untyped.py:10: error: Call to untyped function "label" in typed context  [no-untyped-call]
Found 2 errors in 1 file (checked 1 source file)
```

```python fails id=missing-name expect=exception:NameError
import typing


def send(order: Order) -> None:
    pass


print(typing.get_type_hints(send))
```

The definition of `send` works, because Python does not evaluate the hint then. The call of `get_type_hints` evaluates it and raises `NameError`, because no `Order` exists (rule r7).

## Comparison

In Python, the types in hints are for tools, and the interpreter does not check them (rule r2). A language with static types, such as one where the compiler checks each call, rejects a program with a type error before it runs. Python runs the program, and the checker is a separate step. A program can be partly typed, and the checker checks only the typed parts (rule r5). For the theory, see [[gradual-types]]. For the run-time types, see [[python/dynamic-and-strong-typing]].
