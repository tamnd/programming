---
tag: 4FQ10V
kind: feature
owner: python
slug: paramspec-and-typevartuple
name: ParamSpec and TypeVarTuple
aliases: [ParamSpec, typing.ParamSpec, Concatenate, P.args, P.kwargs, TypeVarTuple, typing.TypeVarTuple, Unpack, variadic generics, "**P", "*Ts", PEP 612, PEP 646, PEP 692]
area: languages/python/static-typing
level: research
summary: >-
  A ParamSpec stands for all the parameters of a callable, so a decorator can keep the signature of the function it wraps. A TypeVarTuple stands for any number of types.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/generics.html#paramspec", locator: "Generics, ParamSpec; TypeVarTuple"}
proposals: [pep-612, pep-646, pep-692]
see_also: [python/generic-functions-and-classes, python/decorators, python/typeddict, python/protocols, python/type-aliases, python/variance-in-python]
sources:
  - url: https://docs.python.org/3.15/library/typing.html#typing.ParamSpec
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.ParamSpec; typing.Concatenate; typing.TypeVarTuple; typing.Unpack
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0612/
    title: "PEP 612: Parameter Specification Variables"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0692/
    title: "PEP 692: Using TypedDict for more precise **kwargs typing"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.10.html#pep-612-parameter-specification-variables
    title: What’s New In Python 3.10
    locator: "PEP 612: Parameter Specification Variables"
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.11.html#pep-646-variadic-generics
    title: What’s New In Python 3.11
    locator: "PEP 646: Variadic generics"
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0646/
    title: "PEP 646: Variadic Generics"
    locator: Abstract; Acceptance
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.12.html#pep-692-using-typeddict-for-more-precise-kwargs-typing
    title: What’s New In Python 3.12
    locator: "PEP 692: Using TypedDict for more precise **kwargs typing"
    accessed: 2026-11-18
---

## Synopsis

```python run id=logged
from collections.abc import Callable
from typing import TYPE_CHECKING


def logged[**P, R](f: Callable[P, R]) -> Callable[P, R]:
    def inner(*args: P.args, **kwargs: P.kwargs) -> R:
        print("call", f.__name__, args, kwargs)
        return f(*args, **kwargs)

    return inner


@logged
def scale(x: float, factor: float = 2.0) -> float:
    return x * factor


print(scale(3.0, factor=0.5))
if TYPE_CHECKING:
    reveal_type(scale)
    scale("3")
```

```output for=logged
call scale (3.0,) {'factor': 0.5}
1.5
```

`**P` stands for the parameters of `f`. The wrapper `inner` takes the same parameters, so the decorated `scale` keeps its signature, and the checker still checks each call. Saved as `logged.py`:

```text
$ mypy --strict logged.py
logged.py:20: note: Revealed type is "def (x: float, factor: float =) -> float"
logged.py:21: error: Argument 1 to "scale" has incompatible type "str"; expected "float"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Semantics

**r1.** A `ParamSpec` stands for a list of parameters. "They are used to forward the parameter types of one callable to another callable – a pattern commonly found in higher order functions and decorators."[^1] PEP 612 says that `Callable[[int, str], bool]` and callback protocols cannot do this: "Neither of these support forwarding the parameter types of one callable over to another callable, making it difficult to annotate function decorators."[^2] Example: [`logged`](#logged).

**r2.** A type parameter list declares a `ParamSpec` with two asterisks: "In type parameter lists, parameter specifications can be declared with two asterisks (**)".[^1] For older versions, the call `P = ParamSpec('P')` makes one. Example: [`old-syntax`](#old-syntax).

**r3.** `P.args` and `P.kwargs` give the two parts of the parameters. "P.args represents the tuple of positional parameters in a given call and should only be used to annotate *args. P.kwargs represents the mapping of keyword parameters to their values in a given call, and should be only be used to annotate **kwargs."[^1] A wrapper must use the two together. Example: [`args-only`](#args-only).

**r4.** `Concatenate` adds parameters in front of a `ParamSpec`. It annotates "a higher-order callable which adds, removes, or transforms parameters of another callable."[^1] "The last parameter to Concatenate must be a ParamSpec or ellipsis (...)."[^1] Example: [`with-db`](#with-db).

**r5.** A `TypeVarTuple` stands for any number of types. "A normal type variable enables parameterization with a single type. A type variable tuple, in contrast, allows parameterization with an arbitrary number of types by acting like an arbitrary number of type variables wrapped in a tuple."[^1] A type parameter list declares it with one asterisk, as `*Ts`. Examples: [`rotate`](#rotate), [`call-later`](#call-later).

**r6.** A `TypeVarTuple` is always unpacked, with `*` or with `Unpack`. "Type variable tuples must always be unpacked."[^1] "However, note that at most one type variable tuple may appear in a single list of type arguments or type parameters".[^1] Example: [`rotate`](#rotate).

**r7.** `Unpack` of a `TypedDict` gives a type to each keyword argument in `**kwargs`. PEP 692 says: "The new approach revolves around using TypedDict to type **kwargs that comprise keyword arguments of different types."[^3] Example: [`kwargs`](#kwargs).

## Examples

```python run id=with-db
from collections.abc import Callable
from typing import TYPE_CHECKING, Concatenate


class Db:
    def query(self, sql: str) -> str:
        return "rows for " + sql


def with_db[**P, R](f: Callable[Concatenate[Db, P], R]) -> Callable[P, R]:
    def inner(*args: P.args, **kwargs: P.kwargs) -> R:
        return f(Db(), *args, **kwargs)

    return inner


@with_db
def find(db: Db, name: str) -> str:
    return db.query(f"name={name}")


print(find("ann"))
if TYPE_CHECKING:
    reveal_type(find)
```

```output for=with-db
rows for name=ann
```

The decorator gives the first argument, so the caller passes only `name`. The checker removes `db` from the signature (rule r4):

```text
$ mypy --strict withdb.py
withdb.py:24: note: Revealed type is "def (name: str) -> str"
Success: no issues found in 1 source file
```

```python run id=rotate
from typing import TYPE_CHECKING


def first_to_last[T, *Ts](tup: tuple[T, *Ts]) -> tuple[*Ts, T]:
    return (*tup[1:], tup[0])


r = first_to_last((1, "a", 2.5))
if TYPE_CHECKING:
    reveal_type(r)
print(r)
```

```output for=rotate
('a', 2.5, 1)
```

`T` gets `int`, and `Ts` gets the two types `str` and `float` (rules r5 and r6):

```text
$ mypy --strict rotate.py
rotate.py:10: note: Revealed type is "tuple[str, float, int]"
Success: no issues found in 1 source file
```

```python run id=call-later
from collections.abc import Callable
from typing import TYPE_CHECKING


def call_later[*Ts](callback: Callable[[*Ts], None], *args: *Ts) -> None:
    callback(*args)


def show(name: str, count: int) -> None:
    print(name, count)


call_later(show, "apples", 3)
if TYPE_CHECKING:
    call_later(show, "apples", "3")
```

```output for=call-later
apples 3
```

The annotation `*args: *Ts` gives each positional argument its own type, and the checker compares them with the parameters of `callback` (rule r5):

```text
$ mypy --strict calllater.py
calllater.py:15: error: Argument 1 to "call_later" has incompatible type "Callable[[str, int], None]"; expected "Callable[[str, str], None]"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=kwargs
from typing import TYPE_CHECKING, NotRequired, TypedDict, Unpack


class Options(TypedDict):
    timeout: float
    retries: NotRequired[int]


def connect(host: str, **kwargs: Unpack[Options]) -> str:
    return f"{host} {kwargs}"


print(connect("db", timeout=1.5))
if TYPE_CHECKING:
    connect("db", timeout=1.5, retry=2)
```

```output for=kwargs
db {'timeout': 1.5}
```

Each key of `Options` is a keyword argument with its own type. The checker finds the wrong name `retry` (rule r7):

```text
$ mypy --strict kwargs.py
kwargs.py:15: error: Unexpected keyword argument "retry" for "connect"  [call-arg]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=old-syntax
from collections.abc import Callable
from typing import ParamSpec, TypeVar

P = ParamSpec("P")
R = TypeVar("R")


def logged(f: Callable[P, R]) -> Callable[P, R]:
    def inner(*args: P.args, **kwargs: P.kwargs) -> R:
        return f(*args, **kwargs)

    return inner


@logged
def add(x: int, y: int) -> int:
    return x + y


print(add(1, 2), P, R)
```

```output for=old-syntax
3 ~P ~R
```

This form works on Python 3.10 and 3.11, before the type parameter lists (rule r2). At run time, `P` and `R` are objects, and their names start with `~`.

## Versions

> [!since v="3.10"]
> `ParamSpec` and `Concatenate` are added, from PEP 612. "Previously, there was no easy way to type annotate dependency of parameter types in such a precise manner."[^4]

> [!since v="3.11"]
> `TypeVarTuple` and `Unpack` are added, from PEP 646. "In other words, a TypeVarTuple is a variadic type variable, enabling variadic generics."[^5] PEP 646 "was accepted for Python 3.11, with the caveat that details around multiple unpackings in a type expression aren’t specified precisely."[^6]

> [!since v="3.12"]
> The type parameter lists accept `**P` and `*Ts`. "Parameter specifications can now be declared using the type parameter syntax introduced by PEP 695."[^1] `Unpack` of a `TypedDict` for `**kwargs` comes from PEP 692: "PEP 692 specifies a more precise way of typing **kwargs by relying on typed dictionaries".[^7]

## Pitfalls

```python fails id=any-callable expect=exception:TypeError
from collections.abc import Callable
from typing import Any


def logged[R](f: Callable[..., R]) -> Callable[..., R]:
    def inner(*args: Any, **kwargs: Any) -> R:
        return f(*args, **kwargs)

    return inner


@logged
def scale(x: float, factor: float = 2.0) -> float:
    return x * factor


print(scale(3.0))
print(scale("3"))
```

With `Callable[..., R]`, the decorated function accepts any arguments, so the checker does not find the string. The documentation names this problem: "The type checker can’t type check the inner function because *args and **kwargs have to be typed Any."[^1] Use a `ParamSpec` (rule r1):

```text
$ mypy --strict anycall.py
Success: no issues found in 1 source file
```

```python wrong id=args-only
from collections.abc import Callable


def logged[**P, R](f: Callable[P, R]) -> Callable[P, R]:
    def inner(*args: P.args) -> R:
        return f(*args)

    return inner


@logged
def add(x: int, y: int) -> int:
    return x + y


print(add(1, 2))
```

```output for=args-only
3
```

The program runs, because the call passes no keyword argument. The checker rejects `P.args` without `P.kwargs` (rule r3):

```text
$ mypy --strict argsonly.py
argsonly.py:5: error: ParamSpec must have "*args" typed as "P.args" and "**kwargs" typed as "P.kwargs"  [valid-type]
argsonly.py:6: error: Too few arguments  [call-arg]
argsonly.py:8: error: Incompatible return value type (got "def inner(*args: Any) -> R", expected "Callable[P, R]")  [return-value]
Found 3 errors in 1 file (checked 1 source file)
```

## Comparison

In Python, `ParamSpec` and `TypeVarTuple` let the checker follow the code of decorators and of functions that pass on their arguments, which are common in Python. Most other languages with static types have no form for "all the parameters of a function". C++ has variadic templates, and TypeScript has the types `Parameters<F>` and rest elements in tuple types. For decorators, see [[python/decorators]]. For type parameters in general, see [[python/generic-functions-and-classes]].
