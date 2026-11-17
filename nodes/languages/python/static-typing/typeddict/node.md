---
tag: FV2V9A
kind: feature
owner: python
slug: typeddict
name: TypedDict
aliases: [typing.TypedDict, typed dictionary, Required, NotRequired, ReadOnly, total=False, PEP 589, PEP 655, PEP 705]
area: languages/python/static-typing
level: advanced
summary: >-
  A TypedDict gives a type to a dictionary with fixed string keys, where each key has its own value type. At run time, the value is a plain dict, and Python checks nothing.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/typeddict.html", locator: "Typed dictionaries"}
proposals: [pep-589, pep-655, pep-705]
see_also: [python/dictionaries, python/dataclasses, python/protocols, python/basic-type-annotations, python/collections-abc, record-type]
sources:
  - url: https://peps.python.org/pep-0589/
    title: "PEP 589: TypedDict: Type Hints for Dictionaries with a Fixed Set of Keys"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.TypedDict
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.TypedDict; typing.ReadOnly
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0655/
    title: "PEP 655: Marking individual TypedDict items as required or potentially-missing"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0705/
    title: "PEP 705: TypedDict: Read-only items"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/typed_dict.html
    title: TypedDict, mypy documentation
    locator: TypedDict; Class-based syntax; Supported operations
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/typeddict.html
    title: Typed dictionaries, Specification for the Python type system
    locator: Assignability; Supported and unsupported operations
    accessed: 2026-11-18
---

## Synopsis

```python run id=movie
from typing import TYPE_CHECKING, TypedDict


class Movie(TypedDict):
    title: str
    year: int


def label(m: Movie) -> str:
    return f"{m['title']} ({m['year']})"


film: Movie = {"title": "Up", "year": 2009}
print(label(film), type(film).__name__)
if TYPE_CHECKING:
    bad: Movie = {"title": "Up", "yaer": 2009}
```

```output for=movie
Up (2009) dict
```

`Movie` says that the dictionary has the key `"title"` with a `str` value and the key `"year"` with an `int` value. At run time, `film` is a plain `dict`. The checker finds the wrong key in `bad`. Saved as `movie.py`:

```text
$ mypy --strict movie.py
movie.py:16: error: Missing key "year" for TypedDict "Movie"  [typeddict-item]
movie.py:16: error: Extra key "yaer" for TypedDict "Movie"  [typeddict-unknown-key]
Found 2 errors in 1 file (checked 1 source file)
```

## Semantics

**r1.** A `TypedDict` is for a dictionary that holds the fields of one object. PEP 589 "proposes a type constructor typing.TypedDict to support the use case where a dictionary object has a specific set of string keys, each with a value of a specific type."[^1] "This expectation is not checked at runtime but is only enforced by type checkers."[^2] "At runtime “TypedDict instances” are simply dicts."[^2] Examples: [`movie`](#movie), [`unchecked`](#unchecked).

**r2.** "By default, all keys must be present in a TypedDict. It is possible to mark individual keys as non-required using NotRequired".[^2] The class argument `total=False` makes all keys non-required, and `Required` marks one key as required again.[^2] PEP 655 says that this is "intended to be enforced only by static type checkers and need not be enforced by Python itself at runtime."[^3] Examples: [`optional`](#optional), [`optional-key`](#optional-key).

**r3.** `ReadOnly` marks a key that code must not change. "This PEP proposes a new type qualifier, typing.ReadOnly".[^4] "There is no runtime checking for this property."[^2] Example: [`readonly`](#readonly).

**r4.** A `TypedDict` has a second form, a call. "This functional syntax allows defining keys which are not valid identifiers, for example because they are keywords or contain hyphens".[^2] Example: [`functional`](#functional).

**r5.** A `TypedDict` is not a `dict[str, ...]` for the checker. "A TypedDict object is not a subtype of the regular dict[...] type (and vice versa), since dict allows arbitrary keys to be added and removed, unlike TypedDict."[^5] "As a consequence, every TypedDict type is assignable to Mapping[str, object]."[^6] Example: [`as-dict`](#as-dict).

**r6.** A `TypedDict` is a type for the checker only. "In particular, TypedDict type objects cannot be used in isinstance() tests such as isinstance(d, Movie)."[^6] Example: [`no-isinstance`](#no-isinstance).

## Examples

```python run id=optional
from typing import NotRequired, TypedDict


class User(TypedDict):
    name: str
    email: NotRequired[str]


def contact(u: User) -> str:
    return u.get("email", "no email")


print(contact({"name": "ann"}), contact({"name": "bo", "email": "bo@example.com"}))
print(sorted(User.__required_keys__), sorted(User.__optional_keys__))
```

```output for=optional
no email bo@example.com
['name'] ['email']
```

The key `"email"` can be missing, so the code reads it with `get` (rule r2). At run time, the class lists its required and optional keys. `mypy --strict` accepts the file.

```python run id=readonly
from typing import TYPE_CHECKING, ReadOnly, TypedDict


class Movie(TypedDict):
    title: ReadOnly[str]
    year: int


def update(m: Movie) -> None:
    m["year"] += 1
    if TYPE_CHECKING:
        m["title"] = "Other"


film: Movie = {"title": "Up", "year": 2009}
update(film)
print(film, sorted(Movie.__readonly_keys__))
```

```output for=readonly
{'title': 'Up', 'year': 2010} ['title']
```

The function can change `"year"`, but not `"title"` (rule r3):

```text
$ mypy --strict readonly.py
readonly.py:12: error: ReadOnly TypedDict key "title" TypedDict is mutated  [typeddict-readonly-mutated]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=functional
from typing import TypedDict

Header = TypedDict("Header", {"content-type": str, "from": str})

h: Header = {"content-type": "text/plain", "from": "ann@example.com"}
print(h["content-type"], Header.__required_keys__ == {"content-type", "from"})
```

```output for=functional
text/plain True
```

The key `"content-type"` has a hyphen, and `"from"` is a keyword, so the class form cannot declare them (rule r4).

## Versions

> [!since v="3.8"]
> `typing.TypedDict` is added, from PEP 589.[^2][^1]

> [!since v="3.11"]
> "Added support for marking individual keys as Required or NotRequired. See PEP 655."[^2] "Added support for generic TypedDicts."[^2]

> [!since v="3.13"]
> "Support for the ReadOnly qualifier was added. See PEP 705."[^2]

> [!since v="3.15"]
> "Support for the closed and extra_items class arguments was added. See PEP 728."[^2]

## Pitfalls

```python wrong id=unchecked
from typing import TypedDict


class Movie(TypedDict):
    title: str
    year: int


film = Movie(title="Up", year="2009")
print(film, film["year"] + "!")
```

```output for=unchecked
{'title': 'Up', 'year': '2009'} 2009!
```

A call of `Movie` makes a plain `dict`, and Python does not check the value types (rule r1). The checker finds the string:

```text
$ mypy --strict unchecked.py
unchecked.py:9: error: Incompatible types (expression has type "str", TypedDict item "year" has type "int")  [typeddict-item]
unchecked.py:10: error: Unsupported operand types for + ("int" and "str")  [operator]
Found 2 errors in 1 file (checked 1 source file)
```

```python fails id=optional-key expect=exception:KeyError
from typing import NotRequired, TypedDict


class User(TypedDict):
    name: str
    email: NotRequired[str]


def contact(u: User) -> str:
    return u["email"]


print(contact({"name": "ann"}))
```

The key `"email"` can be missing, but the function reads it with `[]`, and the program fails (rule r2). Mypy 2.4.1 does not report this, even in strict mode:

```text
$ mypy --strict optbad.py
Success: no issues found in 1 source file
```

Read a key that is not required with `get`, or test it with `in` first.

```python wrong id=as-dict
from collections.abc import Mapping
from typing import TypedDict


class Movie(TypedDict):
    title: str
    year: int


def show(d: Mapping[str, object]) -> None:
    print(", ".join(f"{k}={v}" for k, v in d.items()))


def clear(d: dict[str, object]) -> None:
    d.clear()


film: Movie = {"title": "Up", "year": 2009}
show(film)
clear(film)
print(film)
```

```output for=as-dict
title=Up, year=2009
{}
```

A function that takes a `dict` can remove the required keys, so the checker rejects the call of `clear`. The call of `show` is safe, because a `Mapping` is read-only (rule r5):

```text
$ mypy --strict mapping.py
mapping.py:20: error: Argument 1 to "clear" has incompatible type "Movie"; expected "dict[str, object]"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

```python fails id=no-isinstance expect=exception:TypeError
from typing import TypedDict


class Movie(TypedDict):
    title: str
    year: int


print(isinstance({"title": "Up", "year": 2009}, Movie))
```

A `TypedDict` class is not a run-time type, so `isinstance` raises `TypeError` (rule r6). To check data from outside, test each key and the type of its value.

## Comparison

In Python, a `TypedDict` adds types to a value that stays a plain dictionary (rule r1). This suits data that is already a dictionary, such as parsed JSON. A data class makes a new class with attributes, and its objects are not dictionaries. For data classes, see [[python/dataclasses]]. For the general idea of a value with named fields, see [[record-type]].
