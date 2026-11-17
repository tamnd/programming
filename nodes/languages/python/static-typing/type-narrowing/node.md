---
tag: JQXNZF
kind: feature
owner: python
slug: type-narrowing
name: Type narrowing
aliases: [narrowing, type guard, TypeGuard, TypeIs, assert_never, exhaustiveness check, isinstance narrowing, flow-sensitive typing, PEP 647, PEP 742]
area: languages/python/static-typing
level: advanced
summary: >-
  After a test such as isinstance(x, int), the checker gives x a narrower type in that branch. TypeIs and TypeGuard make a function into such a test, and assert_never checks that all cases are covered.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/narrowing.html", locator: "Type narrowing"}
proposals: [pep-647, pep-742]
see_also: [python/basic-type-annotations, python/type-checkers, python/match-statement, python/literal-and-final, python/variance-in-python, bottom-type]
sources:
  - url: https://mypy.readthedocs.io/en/stable/type_narrowing.html
    title: Type narrowing, mypy documentation
    locator: Type narrowing; Type narrowing expressions
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/narrowing.html
    title: Type narrowing, Specification for the Python type system
    locator: Type narrowing
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.TypeIs
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.TypeIs; typing.TypeGuard; typing.assert_never
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0742/
    title: "PEP 742: Narrowing types with TypeIs"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.10.html#pep-647-user-defined-type-guards
    title: What’s New In Python 3.10
    locator: "PEP 647: User-Defined Type Guards"
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0647/
    title: "PEP 647: User-Defined Type Guards"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.13.html
    title: What’s New In Python 3.13
    locator: New typing features
    accessed: 2026-11-18
---

## Synopsis

```python run id=basic
from typing import TYPE_CHECKING


def describe(x: int | str | None) -> str:
    if x is None:
        return "nothing"
    if TYPE_CHECKING:
        reveal_type(x)
    if isinstance(x, int):
        if TYPE_CHECKING:
            reveal_type(x)
        return f"number {x + 1}"
    if TYPE_CHECKING:
        reveal_type(x)
    return "text " + x.upper()


print(describe(None), describe(4), describe("ab"))
```

```output for=basic
nothing number 5 text AB
```

Each test removes types from `x`. After the `return` for `None`, `x` is `int | str`. In the `isinstance` branch, it is `int`, and after that branch, only `str` is left. Saved as `basic.py`:

```text
$ mypy --strict basic.py
basic.py:8: note: Revealed type is "int | str"
basic.py:11: note: Revealed type is "int"
basic.py:14: note: Revealed type is "str"
Success: no issues found in 1 source file
```

## Semantics

**r1.** "Type narrowing is when you convince a type checker that a broader type is actually more specific, for instance, that an object of type Shape is actually of the narrower type Square."[^1] The specification says little about it: "Type checkers should narrow the types of expressions in certain contexts. This behavior is currently largely unspecified."[^2]

**r2.** The checkers narrow on tests that they know, for example `isinstance()`, `issubclass()`, `type(obj) is int`, `callable()` and `obj is not None`.[^1] A comparison with a literal value narrows a literal type too. "Type narrowing is contextual. For example, based on the condition, mypy will narrow an expression only within an if branch".[^1] Example: [`basic`](#basic).

**r3.** A `return` or a `raise` narrows the code after it. "Mypy understands the implications return or exception raising can have for what type an object could be".[^1] Example: [`basic`](#basic).

**r4.** A function with the return type `TypeIs[T]` is a test that the checker understands. "If the return value is True, the type of its argument is the intersection of the argument’s original type and NarrowedType."[^3] "If the return value is False, the type of its argument is narrowed to exclude NarrowedType."[^3] PEP 742 says: "Unlike the existing typing.TypeGuard special form, TypeIs can narrow the type in both the if and else branches of a conditional."[^4] Example: [`type-is`](#type-is).

**r5.** `TypeGuard[T]` is the older form. "If the return value is True, the type of its argument is the type inside TypeGuard."[^3] "TypeIs requires the narrowed type to be a subtype of the input type, while TypeGuard does not."[^3] "When a TypeGuard function returns False, type checkers cannot narrow the type of the variable at all."[^3] Example: [`type-guard`](#type-guard).

**r6.** The checker trusts the function. "An incorrectly written TypeIs function can lead to unsound behavior in the type system; it is the user’s responsibility to write such functions in a type-safe manner."[^3] Example: [`wrong-type-is`](#wrong-type-is).

**r7.** `assert_never(x)` checks that no type is left. "For a call to assert_never to pass type checking, the inferred type of the argument passed in must be the bottom type, Never, and nothing else."[^3] "At runtime, this throws an exception when called."[^3] Example: [`never`](#never).

## Examples

```python run id=type-is
from typing import TYPE_CHECKING, TypeIs


def is_text(value: object) -> TypeIs[str | bytes]:
    return isinstance(value, (str, bytes))


def size(value: int | str | bytes) -> int:
    if is_text(value):
        if TYPE_CHECKING:
            reveal_type(value)
        return len(value)
    if TYPE_CHECKING:
        reveal_type(value)
    return value


print(size("abc"), size(b"ab"), size(7))
```

```output for=type-is
3 2 7
```

The function narrows `value` in both branches (rule r4):

```text
$ mypy --strict typeis.py
typeis.py:11: note: Revealed type is "str | bytes"
typeis.py:14: note: Revealed type is "int"
Success: no issues found in 1 source file
```

```python run id=type-guard
from typing import TYPE_CHECKING, TypeGuard


def all_str(values: list[object]) -> TypeGuard[list[str]]:
    return all(isinstance(v, str) for v in values)


def join(values: list[object]) -> str:
    if all_str(values):
        if TYPE_CHECKING:
            reveal_type(values)
        return " ".join(values)
    return "mixed"


print(join(["a", "b"]), join(["a", 1]))
```

```output for=type-guard
a b mixed
```

`list[str]` is not a subtype of `list[object]`, because a list is invariant, so this function needs `TypeGuard`, not `TypeIs` (rule r5):

```text
$ mypy --strict typeguard.py
typeguard.py:11: note: Revealed type is "list[str]"
Success: no issues found in 1 source file
```

## Versions

> [!since v="3.10"]
> "TypeGuard has been added to the typing module to annotate type guard functions and improve information provided to static type checkers during type narrowing."[^5] PEP 647 "specifies a way for programs to influence conditional type narrowing employed by a type checker based on runtime checks."[^6]

> [!since v="3.13"]
> "PEP 742: typing.TypeIs provides more intuitive type narrowing behavior, as an alternative to typing.TypeGuard."[^7]

## Pitfalls

```python fails id=never expect=exception:AssertionError
from typing import assert_never


def area(shape: tuple[str, float] | float) -> float:
    match shape:
        case (str(), float() as r):
            return 3.14 * r * r
        case _:
            assert_never(shape)


print(area(("circle", 1.0)))
print(area(2.0))
```

The `match` statement has no case for a plain `float`, so `assert_never` gets a value and raises `AssertionError` (rule r7). The checker finds the missing case before the program runs:

```text
$ mypy --strict never.py
never.py:9: error: Argument 1 to "assert_never" has incompatible type "float"; expected "Never"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

```python fails id=wrong-type-is expect=exception:TypeError
from typing import TypeIs


def is_int(value: object) -> TypeIs[int]:
    return not isinstance(value, str)


def double(value: int | list[int]) -> int:
    if is_int(value):
        return value * 2
    return sum(value) * 2


print(double(3))
print(double([1, 2]) + 1)
```

`is_int` returns `True` for a list too, so `double` returns a list, and the last line fails. The checker does not look inside `is_int`, and it reports nothing (rule r6):

```text
$ mypy --strict badis.py
Success: no issues found in 1 source file
```

```python fails id=stale expect=exception:TypeError
class Box:
    def __init__(self, item: int | None) -> None:
        self.item = item


def clear(b: Box) -> None:
    b.item = None


def bump(b: Box) -> int:
    if b.item is not None:
        clear(b)
        return b.item + 1
    return 0


print(bump(Box(None)))
print(bump(Box(1)))
```

The test narrows `b.item` to `int`. The call of `clear` sets it back to `None`, but mypy keeps the narrowed type after the call, and it reports nothing:

```text
$ mypy --strict stale.py
Success: no issues found in 1 source file
```

Read the attribute into a local variable, test the variable, and use it.

## Comparison

In Python, narrowing follows the tests that the program already makes at run time, such as `isinstance` (rule r2). So a union type works without a cast, and the checker follows the flow of the code. The type that has no values, which `assert_never` expects, is the [[bottom-type]]. For patterns that narrow a value, see [[python/match-statement]].
