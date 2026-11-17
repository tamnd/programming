---
tag: NEW-python-basic-type-annotations
kind: feature
owner: python
slug: basic-type-annotations
name: Basic type annotations
aliases: [list[int], dict[str, int], tuple[int, ...], Optional, X | None, Union, Any, Callable, object type, built-in generics, PEP 585, PEP 604]
area: languages/python/static-typing
level: core
summary: >-
  The basic hints are classes such as int and str, built-in generics such as list[str] and dict[str, int], unions written X | Y, X | None for a value that can be None, Callable, Any and object.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/typing.html", locator: "typing, Annotating tuples; Annotating callable objects; Special types"}
proposals: [pep-484, pep-585, pep-604]
see_also: [python/type-hints, python/type-checkers, python/function-annotations, python/collections-abc, python/sequence-types, top-type, function-type]
sources:
  - url: https://docs.python.org/3.15/library/typing.html
    title: typing, Support for type hints, Python 3.15 documentation
    locator: Annotating tuples; Annotating callable objects; typing.Any; typing.Union; typing.Optional; typing.List
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0585/
    title: "PEP 585: Type Hinting Generics In Standard Collections"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0604/
    title: "PEP 604: Allow writing union types as X | Y"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.9.html#type-hinting-generics-in-standard-collections
    title: What’s New In Python 3.9
    locator: Type Hinting Generics in Standard Collections
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.10.html#pep-604-new-type-union-operator
    title: What’s New In Python 3.10
    locator: "PEP 604: New Type Union Operator"
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/dynamic_typing.html#any-vs-object
    title: Dynamically typed code, mypy documentation
    locator: Any vs. object
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0484/
    title: "PEP 484: Type Hints"
    locator: Union types
    accessed: 2026-11-18
---

## Synopsis

```python run id=basics
from collections.abc import Callable


def summary(
    names: list[str],
    ages: dict[str, int],
    point: tuple[int, int],
    note: str | None,
    show: Callable[[str], str],
) -> str:
    oldest = max(names, key=lambda name: ages[name])
    extra = note if note is not None else "-"
    return show(f"{oldest} {point} {extra}")


print(summary(["ann", "bo"], {"ann": 31, "bo": 40}, (2, 3), None, str.upper))
```

```output for=basics
BO (2, 3) -
```

Each parameter has a hint: a list of strings, a dictionary from strings to integers, a pair of integers, a string or `None`, and a function from a string to a string. `mypy --strict` accepts the file.

## Semantics

**r1.** A class is a type. Use `int`, `str`, `float`, `bool`, `bytes` and your own classes directly as hints.

**r2.** A collection class takes the types of its items in square brackets: `list[int]`, `set[str]`, `dict[str, int]`. "In type annotations you can now use built-in collection types such as list and dict as generic types instead of importing the corresponding capitalized types (e.g. List or Dict) from typing."[^4] The old names are deprecated: `typing.List` is a "Deprecated alias to list."[^1]

**r3.** A tuple is a special case. "tuple accepts any number of type arguments".[^1] "To denote a tuple which could be of any length, and in which all elements are of the same type T, use the literal ellipsis ...: tuple[T, ...]. To denote an empty tuple, use tuple[()]."[^1] Other containers take a fixed number of arguments: "list only accepts one type argument".[^1] Example: [`two-args`](#two-args).

**r4.** A union is a value of one of several types. "Union[X, Y] is equivalent to X | Y and means either X or Y."[^1] "Using that shorthand is recommended."[^1] For a value that can be `None`, write `X | None`. "Optional[X] is equivalent to X | None (or Union[X, None])."[^1] Example: [`union-runtime`](#union-runtime).

**r5.** `Optional` does not mean an optional argument. "Note that this is not the same concept as an optional argument, which is one that has a default."[^1] A default of `None` needs `X | None` in the hint. PEP 484 says of the old shortcut: "This is no longer the recommended behavior. Type checkers should move towards requiring the optional type to be made explicit."[^7] Example: [`default-none`](#default-none).

**r6.** A function value has the type `Callable[[A, B], R]`. "Callable[[int], str] signifies a function that takes a single parameter of type int and returns a str."[^1] "The subscription syntax must always be used with exactly two values: the argument list and the return type."[^1] Import `Callable` from `collections.abc`, because `typing.Callable` is a "Deprecated alias to collections.abc.Callable."[^1] Example: [`callback`](#callback).

**r7.** `Any` turns the checks off. "Every type is assignable to Any. Any is assignable to every type."[^1] `object` is a normal type that holds every value: "Unlike Any, object is an ordinary static type (it is similar to Object in Java), and only operations valid for all types are accepted for object values."[^6] Example: [`any-vs-object`](#any-vs-object).

## Examples

```python run id=union-runtime
from typing import Optional, Union

print(int | None)
print(Optional[int] == (int | None))
print(Union[int, str] == (int | str))
print(isinstance(1, int | str), isinstance(1.5, int | str))
```

```output for=union-runtime
int | None
True
True
True False
```

The `|` form and the `typing` forms make equal objects. A union also works in `isinstance`: "This new syntax is also accepted as the second argument to isinstance() and issubclass()".[^5]

```python fails id=callback expect=exception:AttributeError
from collections.abc import Callable


def apply(values: list[int], step: Callable[[int], int]) -> list[int]:
    return [step(v) for v in values]


def double(n: int) -> int:
    return n * 2


def shout(text: str) -> str:
    return text.upper()


print(apply([1, 2], double))
print(apply([1, 2], lambda n: n + 1))
print(apply([1, 2], shout))
```

The first two calls pass a function from `int` to `int`. The third passes a function that takes a `str`, and it fails at run time. The checker reports it first (rule r6):

```text
$ mypy --strict callback.py
callback.py:18: error: Argument 2 to "apply" has incompatible type "Callable[[str], str]"; expected "Callable[[int], int]"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Versions

> [!since v="3.9"]
> The built-in collections take type arguments, as in `list[int]`, from PEP 585.[^2][^4]

> [!since v="3.10"]
> The `X | Y` form of a union is added, from PEP 604. "A new type union operator was introduced which enables the syntax X | Y."[^5]

## Pitfalls

```python wrong id=default-none
def greet(name: str = None) -> str:
    return "hello " + (name or "world")


def greet_ok(name: str | None = None) -> str:
    return "hello " + (name or "world")


print(greet(), greet_ok("ann"))
```

```output for=default-none
hello world hello ann
```

Both functions run, but the hint of `greet` says `str`, and its default is `None` (rule r5). The checker rejects it:

```text
$ mypy --strict default_none.py
default_none.py:1: error: Incompatible default for parameter "name" (default has type "None", parameter has type "str")  [assignment]
default_none.py:1: note: PEP 484 prohibits implicit Optional. Accordingly, mypy has changed its default to no_implicit_optional=True
default_none.py:1: note: Use https://github.com/hauntsaninja/no_implicit_optional to automatically upgrade your codebase
Found 1 error in 1 file (checked 1 source file)
```

```python fails id=any-vs-object expect=exception:TypeError
from typing import Any


def size_any(value: Any) -> int:
    return len(value)


def size_object(value: object) -> int:
    return len(value)


print(size_any("abc"))
print(size_any(5))
```

With `Any`, the checker accepts `len(value)` and the call `size_any(5)`, and the program fails at run time. With `object`, the checker rejects `len(value)`, because not every object has a length (rule r7):

```text
$ mypy --strict obj.py
obj.py:9: error: Argument 1 to "len" has incompatible type "object"; expected "Sized"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

Use `object` for "any value" when the code only passes the value on, and narrow it with `isinstance` before you use it.

```python wrong id=two-args
pairs: list[int, str] = [1, "a"]
print(pairs)
```

```output for=two-args
[1, 'a']
```

Python accepts `list[int, str]` at run time, but the type is wrong: a list has one item type (rule r3). Use `list[int | str]`, or `tuple[int, str]` for a fixed pair.

```text
$ mypy --strict twoargs.py
twoargs.py:1: error: "list" expects 1 type argument, but 2 given  [type-arg]
Found 1 error in 1 file (checked 1 source file)
```

## Comparison

In Python, a hint is an expression, and the common types are the classes that the program uses at run time, such as `list[int]` (rule r2). The same objects work in `isinstance` for unions. The type `Any` has no check at all, and `object` is the top of the class tree with the checks on (rule r7). For the theory of a type that holds every value, see [[top-type]]. For the abstract collection types, such as `Sequence` and `Mapping`, see [[python/collections-abc]].
