---
tag: PAPJR8
kind: feature
owner: python
slug: dataclass-transform-and-typing-extensions
name: dataclass_transform and typing_extensions
aliases: [dataclass_transform, typing.dataclass_transform, "@dataclass_transform", data class transforms, typing_extensions, typing-extensions, typing backports, PEP 681]
area: languages/python/static-typing
level: advanced
summary: >-
  @dataclass_transform tells the checker that a library decorator, base class or metaclass makes classes like @dataclass does. typing_extensions gives new typing features to older Python versions.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/dataclasses.html", locator: "Dataclasses, The dataclass_transform decorator"}
proposals: [pep-681]
see_also: [python/dataclasses, python/decorators, python/metaclasses, python/type-checkers, python/special-typing-forms, python/the-typing-specification]
sources:
  - url: https://peps.python.org/pep-0681/
    title: "PEP 681: Data Class Transforms"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.dataclass_transform
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.dataclass_transform
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/dataclasses.html
    title: Dataclasses, Specification for the Python type system
    locator: The dataclass_transform decorator; Specification
    accessed: 2026-11-18
  - url: https://typing-extensions.readthedocs.io/en/latest/
    title: typing_extensions documentation
    locator: Welcome to typing_extensions’s documentation; Versioning and backwards compatibility; Python version support
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.11.html#pep-681-data-class-transforms
    title: What’s New In Python 3.11
    locator: "PEP 681: Data class transforms"
    accessed: 2026-11-18
---

## Synopsis

```python run id=model
import dataclasses
from typing import TYPE_CHECKING, dataclass_transform

REGISTRY: list[str] = []


@dataclass_transform()
def model[T](cls: type[T]) -> type[T]:
    REGISTRY.append(cls.__name__)
    return dataclasses.dataclass(cls)


@model
class Customer:
    id: int
    name: str


c = Customer(id=1, name="ann")
print(c, REGISTRY)
if TYPE_CHECKING:
    Customer(id="1")
```

```output for=model
Customer(id=1, name='ann') ['Customer']
```

The decorator `model` of a library makes a data class and records it. The checker cannot see this in the code of `model`, but `@dataclass_transform()` tells it, so it knows the `__init__` of `Customer`. Saved as `model.py`:

```text
$ mypy --strict model.py
model.py:22: error: Missing positional argument "name" in call to "Customer"  [call-arg]
model.py:22: error: Argument "id" to "Customer" has incompatible type "str"; expected "int"  [arg-type]
Found 2 errors in 1 file (checked 1 source file)
```

## Semantics

**r1.** PEP 681 says: "Several popular libraries have behaviors that are similar to dataclasses, but these behaviors cannot be described using standard type annotations. Such projects include attrs, pydantic, and object relational mapper (ORM) packages such as SQLAlchemy and Django."[^1] "The presence of @dataclass_transform() tells a static type checker that the decorated object performs runtime “magic” that transforms a class in a similar way to @dataclasses.dataclass."[^2] Example: [`model`](#model).

**r2.** "@dataclass_transform may be used to decorate a class, metaclass, or a function that is itself a decorator."[^2] For a class, "dataclass-like semantics will be assumed for any class that directly or indirectly derives from the decorated class or uses the decorated class as a metaclass."[^3] Example: [`base-class`](#base-class).

**r3.** The arguments of the decorator set the defaults. For example, `kw_only_default` "Indicates whether the kw_only parameter is assumed to be True or False if it is omitted by the caller."[^2] `field_specifiers` "Specifies a static list of supported classes or functions that describe fields, similar to dataclasses.field()."[^2] Example: [`base-class`](#base-class).

**r4.** The decorator has no effect on what the code does. The checker trusts it and does not look at the code of the library. Example: [`no-magic`](#no-magic).

**r5.** "typing_extensions complements the standard-library typing module, providing runtime support for type hints as specified by PEP 484 and subsequent PEPs."[^4] Its first purpose: "Enable use of new type system features on older Python versions."[^4] It also gives features of PEPs that are not yet accepted.[^4] Example: [`backport`](#backport).

**r6.** "typing_extensions also re-exports all names from the typing module, including those that have always been present in the module."[^4] It is a package on PyPI, not part of the standard library. It uses semantic versioning.[^4]

## Examples

```python run id=base-class
import dataclasses
from typing import TYPE_CHECKING, dataclass_transform


@dataclass_transform(kw_only_default=True)
class Model:
    def __init_subclass__(cls) -> None:
        dataclasses.dataclass(kw_only=True)(cls)


class User(Model):
    name: str
    admin: bool = False


print(User(name="bo"), User(name="ann", admin=True).admin)
if TYPE_CHECKING:
    User("bo")
```

```output for=base-class
User(name='bo', admin=False) True
```

Each subclass of `Model` is a data class, and its fields are keyword-only, so the checker rejects the positional argument (rules r2 and r3):

```text
$ mypy --strict base.py
base.py:18: error: Too many positional arguments for "User"  [call-arg]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=backport
import sys

if sys.version_info >= (3, 13):
    from typing import TypeIs
else:
    from typing_extensions import TypeIs


def is_text(x: object) -> TypeIs[str]:
    return isinstance(x, str)


print(is_text("a"), is_text(1), TypeIs.__module__)
```

```output for=backport
True False typing
```

On Python 3.13 and later, the code imports `TypeIs` from `typing`, and on older versions from `typing_extensions` (rule r5). The checkers understand the test of `sys.version_info`. Many projects import from `typing_extensions` with no test, because it re-exports the names of `typing` (rule r6).

## Versions

> [!since v="3.11"]
> `typing.dataclass_transform` is added, from PEP 681. "The presence of @dataclass_transform() tells a static type checker that the decorated object performs runtime “magic” that transforms a class, giving it dataclass-like behaviors."[^5]

> [!since v="3.12"]
> The argument `frozen_default` is added.[^2]

Of its own versions, typing_extensions says: "typing_extensions currently supports Python versions 3.10 and higher."[^4]

## Pitfalls

```python fails id=no-magic expect=exception:TypeError
from typing import dataclass_transform


@dataclass_transform()
def model[T](cls: type[T]) -> type[T]:
    return cls


@model
class Customer:
    id: int
    name: str


print(Customer(id=1, name="ann"))
```

The decorator says that it makes a data class, but it returns the class with no change. So `Customer` has no `__init__` with parameters, and the call fails at run time. The checker trusts the decorator (rule r4):

```text
$ mypy --strict nomagic.py
Success: no issues found in 1 source file
```

## Comparison

In Python, `@dataclass_transform` describes the work of libraries such as attrs, pydantic and SQLAlchemy, whose classes the checker cannot follow in their code (rule r1). Other languages do this with macros or code generation, such as Lombok in Java or the derive macros of Rust, which the compiler runs. A Python checker does not run the library, so it needs this description. For the data classes of the standard library, see [[python/dataclasses]].
