---
tag: Y1GBDX
kind: feature
owner: python
slug: overloads
name: Overloads
aliases: [overload, "@overload", typing.overload, overloaded function, overload variants, get_overloads, function overloading]
area: languages/python/static-typing
level: advanced
summary: >-
  The @overload decorator gives one function several signatures for the checker, for example int to int and str to str. One plain definition after them does the work at run time.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/overload.html", locator: "Overloads"}
proposals: [pep-484]
see_also: [python/basic-type-annotations, python/generic-functions-and-classes, python/literal-and-final, python/type-checkers, python/function-overloading-alternatives, overloading]
sources:
  - url: https://docs.python.org/3.15/library/typing.html#typing.overload
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.overload; typing.get_overloads
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/more_types.html#function-overloading
    title: More types, mypy documentation
    locator: Function overloading; Runtime behavior; Type checking calls to overloads; Type checking the variants; Type checking the implementation
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/overload.html
    title: Overloads, Specification for the Python type system
    locator: Overloads
    accessed: 2026-11-18
---

## Synopsis

```python run id=first
from typing import TYPE_CHECKING, overload


@overload
def double(x: int) -> int: ...
@overload
def double(x: str) -> str: ...
def double(x: int | str) -> int | str:
    return x * 2


n = double(4)
s = double("ab")
if TYPE_CHECKING:
    reveal_type(n)
    reveal_type(s)
    double(1.5)
print(n, s)
```

```output for=first
8 abab
```

The two variants tell the checker that an `int` gives an `int` and a `str` gives a `str`. A hint of `int | str` on one function cannot say this. The last definition runs. Saved as `first.py`:

```text
$ mypy --strict first.py
first.py:15: note: Revealed type is "int"
first.py:16: note: Revealed type is "str"
first.py:17: error: No overload variant of "double" matches argument type "float"  [call-overload]
first.py:17: note: Possible overload variants:
first.py:17: note:     def double(x: int) -> int
first.py:17: note:     def double(x: str) -> str
Found 1 error in 1 file (checked 1 source file)
```

## Semantics

**r1.** "The @overload decorator allows describing functions and methods that support multiple different combinations of argument types. A series of @overload-decorated definitions must be followed by exactly one non-@overload-decorated definition (for the same function/method)."[^1] Example: [`first`](#first).

**r2.** The variants exist for the checker. "@overload-decorated definitions are for the benefit of the type checker only, since they will be overwritten by the non-@overload-decorated definition."[^1] Mypy says: "There is no automatic dispatch handling and you must manually handle the different types in the implementation (e.g. by using if statements and isinstance checks)."[^2] Example: [`no-dispatch`](#no-dispatch).

**r3.** The checker picks a variant for each call. "When you call an overloaded function, mypy will infer the correct return type by picking the best matching variant, after taking into consideration both the argument types and arity. However, a call is never type checked against the implementation."[^2] "If there are multiple equally good matching variants, mypy will select the variant that was defined first."[^2] Example: [`arity`](#arity).

**r4.** The checker checks the variants. "First, mypy will check and make sure that no overload variant is shadowing a subsequent one."[^2] It also checks the implementation: "The variants must also be compatible with the implementation type hints."[^2] Example: [`shadow`](#shadow).

**r5.** A stub file has the variants and no implementation. "Uses of the @overload decorator as shown above are suitable for stub files. In regular modules, a series of @overload-decorated definitions must be followed by exactly one non-@overload-decorated definition".[^3] Example: [`no-implementation`](#no-implementation).

**r6.** A program can read the variants at run time. `get_overloads` will "Return a sequence of @overload-decorated definitions for func."[^1] Example: [`introspect`](#introspect).

## Examples

```python run id=arity
from typing import TYPE_CHECKING, overload


@overload
def span(start: int) -> int: ...
@overload
def span(start: int, end: int) -> tuple[int, int]: ...
def span(start: int, end: int | None = None) -> int | tuple[int, int]:
    return start if end is None else (start, end)


print(span(3), span(3, 7))
if TYPE_CHECKING:
    span(3, None)
```

```output for=arity
3 (3, 7)
```

The variants depend on the number of arguments. The implementation accepts `span(3, None)`, but no variant does, and the checker uses the variants only (rule r3):

```text
$ mypy --strict event.py
event.py:14: error: No overload variant of "span" matches argument types "int", "None"  [call-overload]
event.py:14: note: Possible overload variants:
event.py:14: note:     def span(start: int) -> int
event.py:14: note:     def span(start: int, end: int) -> tuple[int, int]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=introspect
from typing import get_overloads, overload


@overload
def double(x: int) -> int: ...
@overload
def double(x: str) -> str: ...
def double(x: int | str) -> int | str:
    return x * 2


for variant in get_overloads(double):
    print(variant.__annotations__)
print(double.__name__, double(3))
```

```output for=introspect
{'x': <class 'int'>, 'return': <class 'int'>}
{'x': <class 'str'>, 'return': <class 'str'>}
double 6
```

The name `double` refers to the implementation, and the variants are kept in a registry (rule r6).

## Versions

> [!since v="3.11"]
> "Overloaded functions can now be introspected at runtime using get_overloads()."[^1]

## Pitfalls

```python wrong id=no-dispatch
from typing import TYPE_CHECKING, overload


@overload
def parse(text: str) -> int: ...
@overload
def parse(text: bytes) -> bytes: ...
def parse(text: str | bytes) -> int | bytes:
    return int(text)


b = parse(b"34")
if TYPE_CHECKING:
    reveal_type(b)
print(b, type(b).__name__)
```

```output for=no-dispatch
34 int
```

The second variant says that `bytes` gives `bytes`, but the implementation returns an `int` for each input. The checker compares the implementation with the union of the variants only, so it does not find the bug (rules r2 and r4):

```text
$ mypy --strict nodispatch.py
nodispatch.py:14: note: Revealed type is "bytes"
Success: no issues found in 1 source file
```

```python wrong id=shadow
from typing import overload


@overload
def describe(x: object) -> str: ...
@overload
def describe(x: int) -> int: ...
def describe(x: object) -> str | int:
    return x if isinstance(x, int) else str(x)


print(describe(5) + 1)
```

```output for=shadow
6
```

Each `int` is an `object`, so the first variant matches each call, and the second is never used (rule r4). Put the narrower variant first:

```text
$ mypy --strict shadow.py
shadow.py:7: error: Overloaded function signature 2 will never be matched: signature 1's parameter type(s) are the same or broader  [overload-cannot-match]
shadow.py:12: error: Unsupported operand types for + ("str" and "int")  [operator]
Found 2 errors in 1 file (checked 1 source file)
```

```python fails id=no-implementation expect=exception:NotImplementedError
from typing import overload


@overload
def double(x: int) -> int: ...
@overload
def double(x: str) -> str: ...


print(double(3))
```

A `.py` file needs the implementation (rule r5). "At runtime, calling an @overload-decorated function directly will raise NotImplementedError."[^1]

```text
$ mypy --strict noimpl.py
noimpl.py:4: error: An overloaded function outside a stub file must have an implementation  [no-overload-impl]
Found 1 error in 1 file (checked 1 source file)
```

## Comparison

In Python, an overload is a set of signatures for the checker and one function at run time (rule r2). In a language with static overloading, the compiler picks one of several function bodies from the argument types. In Python, there is one body, and it must test the types itself. For run-time dispatch on the type of the first argument, use `functools.singledispatch`. For other ways to get this effect in Python, see [[python/function-overloading-alternatives]]. For the general idea, see [[overloading]].
