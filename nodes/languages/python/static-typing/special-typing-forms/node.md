---
tag: NDN9JB
kind: feature
owner: python
slug: special-typing-forms
name: Special typing forms
aliases: [Self, typing.Self, Never, typing.Never, NoReturn, ClassVar, Annotated, typing.Annotated, TypeForm, NewType, typing.NewType, PEP 673, PEP 593, PEP 747]
area: languages/python/static-typing
level: advanced
summary: >-
  The typing module has forms with special rules: Self for the current class, Never for no value, ClassVar for class attributes, Annotated for metadata, NewType for distinct types and TypeForm.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/special-types.html", locator: "Special types in annotations; Type qualifiers"}
proposals: [pep-673, pep-593, pep-747]
see_also: [python/basic-type-annotations, python/type-aliases, python/type-narrowing, python/literal-and-final, python/generic-functions-and-classes, bottom-type]
sources:
  - url: https://docs.python.org/3.15/library/typing.html#typing.Self
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.Self; typing.Never; typing.NoReturn; typing.ClassVar; typing.Annotated; typing.TypeForm; typing.NewType; NewType
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0673/
    title: "PEP 673: Self Type"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/special-types.html
    title: Special types in annotations, Specification for the Python type system
    locator: NoReturn; Never
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0593/
    title: "PEP 593: Flexible function and variable annotations"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/qualifiers.html
    title: Type qualifiers, Specification for the Python type system
    locator: Annotated
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0747/
    title: "PEP 747: Annotating Type Forms"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.11.html#pep-673-self-type
    title: What’s New In Python 3.11
    locator: "PEP 673: Self type; typing"
    accessed: 2026-11-18
---

## Synopsis

```python run id=self
from typing import TYPE_CHECKING, Self


class Query:
    def __init__(self) -> None:
        self.parts: list[str] = []

    def where(self, cond: str) -> Self:
        self.parts.append(cond)
        return self


class UserQuery(Query):
    def active(self) -> Self:
        return self.where("active")


q = UserQuery().where("age > 30").active()
if TYPE_CHECKING:
    reveal_type(q)
print(type(q).__name__, q.parts)
```

```output for=self
UserQuery ['age > 30', 'active']
```

`where` is defined in `Query`, but on a `UserQuery` it returns a `UserQuery`, so the call of `active` after it type-checks. Saved as `query.py`:

```text
$ mypy --strict query.py
query.py:20: note: Revealed type is "query.UserQuery"
Success: no issues found in 1 source file
```

## Semantics

**r1.** `Self` is "Special type to represent the current enclosed class."[^1] PEP 673 "introduces a simple and intuitive way to annotate methods that return an instance of their class."[^2] "In general, if something returns self, as in the above examples, you should use Self as the return annotation."[^1] Examples: [`self`](#self), [`wrong-self`](#wrong-self).

**r2.** "Never and NoReturn represent the bottom type, a type that has no members."[^1] A function with the return type `NoReturn` does not return normally. "The checkers will also recognize that the code after calls to such functions is unreachable and will behave accordingly".[^3] "The NoReturn type is conventionally used in return annotations of functions, and Never is typically used in other locations, but the two types are completely interchangeable."[^3] Example: [`never`](#never).

**r3.** `ClassVar` marks a class attribute: "a variable annotation wrapped in ClassVar indicates that a given attribute is intended to be used as a class variable and should not be set on instances of that class."[^1] "ClassVar does not change Python runtime behavior, but it can be used by static type checkers."[^1] Example: [`class-var`](#class-var).

**r4.** `Annotated[T, x]` adds the value `x` to the type `T`. PEP 593 "introduces a mechanism to extend the type annotations from PEP 484 with arbitrary metadata."[^4] "If a library or tool encounters an annotation Annotated[T, x] and has no special logic for the metadata, it should ignore the metadata and simply treat the annotation as T."[^1] "There must be at least one metadata element (Annotated[int] is not valid)".[^5] Example: [`annotated`](#annotated).

**r5.** `NewType` makes a distinct type for the checker. "The static type checker will treat the new type as if it were a subclass of the original type."[^1] "Note that these checks are enforced only by the static type checker."[^1] "More precisely, the expression some_value is Derived(some_value) is always true at runtime."[^1] Examples: [`new-type`](#new-type), [`new-type-runtime`](#new-type-runtime).

**r6.** `TypeForm[T]` is the type of a value that is itself a type expression, such as `int` or `list[str]`. "TypeForm[T] describes the set of all type form objects that represent the type T or types assignable to T."[^1] PEP 747 says that before it, "Developers are forced to use an overly-wide type like object".[^6]

## Examples

```python fails id=never expect=exception:ValueError
from typing import NoReturn


def fail(message: str) -> NoReturn:
    raise ValueError(message)


def port(text: str) -> int:
    if not text.isdigit():
        fail(f"bad port {text!r}")
    return int(text)


print(port("8080"))
print(port("http"))
```

The checker knows that `fail` does not return, so `port` needs no `return` after the call (rule r2). The program prints `8080`, and then the second call raises `ValueError`:

```text
$ mypy --strict never.py
Success: no issues found in 1 source file
```

```python run id=class-var
from typing import TYPE_CHECKING, ClassVar


class Counter:
    created: ClassVar[int] = 0
    name: str

    def __init__(self, name: str) -> None:
        self.name = name
        Counter.created += 1


a, b = Counter("a"), Counter("b")
if TYPE_CHECKING:
    a.created = 5
print(Counter.created, a.name, b.name)
```

```output for=class-var
2 a b
```

`created` belongs to the class, and `name` belongs to each object (rule r3). The checker rejects the assignment through an object:

```text
$ mypy --strict classvar.py
classvar.py:15: error: Cannot assign to class variable "created" via instance  [misc]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=annotated
from dataclasses import dataclass
from typing import Annotated, get_type_hints


@dataclass
class Range:
    lo: int
    hi: int


class Config:
    port: Annotated[int, Range(1, 65535)] = 8080


hints = get_type_hints(Config, include_extras=True)
print(hints["port"].__metadata__)
print(get_type_hints(Config)["port"])
```

```output for=annotated
(Range(lo=1, hi=65535),)
<class 'int'>
```

The checker sees `int`. A library can read the `Range` object at run time, and with no `include_extras`, `get_type_hints` gives only the type (rule r4). `mypy --strict` accepts the file.

```python run id=new-type
from typing import TYPE_CHECKING, NewType

UserId = NewType("UserId", int)


def name_of(uid: UserId) -> str:
    return f"user {uid}"


uid = UserId(42)
total = uid + 1
print(name_of(uid), total)
if TYPE_CHECKING:
    reveal_type(total)
    name_of(42)
```

```output for=new-type
user 42 43
```

A plain `int` is not a `UserId`, and an operation on a `UserId` gives an `int` (rule r5):

```text
$ mypy --strict newtype.py
newtype.py:14: note: Revealed type is "int"
newtype.py:15: error: Argument 1 to "name_of" has incompatible type "int"; expected "UserId"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Versions

> [!since v="3.6"]
> `NoReturn` is added.[^1]

> [!since v="3.11"]
> `Self` and `Never` are added. "The new Self annotation provides a simple and intuitive way to annotate methods that return an instance of their class."[^7]

> [!since v="3.15"]
> `TypeForm` is added, from PEP 747. "TypeForm(obj) simply returns obj unchanged."[^1]

## Pitfalls

```python wrong id=wrong-self
from typing import Self


class Shape:
    def copy(self) -> Self:
        return Shape()


class Circle(Shape):
    pass


print(type(Circle().copy()).__name__)
```

```output for=wrong-self
Shape
```

`Self` means the class of the object, but `copy` always makes a `Shape`. The documentation says: "You should not use Self as the return annotation if the method is not guaranteed to return an instance of a subclass when the class is subclassed".[^1] The checker reports it (rule r1):

```text
$ mypy --strict wrongself.py
wrongself.py:6: error: Incompatible return value type (got "Shape", expected "Self")  [return-value]
Found 1 error in 1 file (checked 1 source file)
```

Return `type(self)()`, or annotate the method with `-> "Shape"`.

```python fails id=new-type-runtime expect=exception:TypeError
from typing import NewType

UserId = NewType("UserId", int)

uid = UserId(42)
print(type(uid).__name__, uid is UserId(uid))
print(isinstance(uid, UserId))
```

At run time, `UserId(42)` is the `int` 42, and `UserId` is not a class, so `isinstance` raises `TypeError` (rule r5). The checker reports the same line:

```text
$ mypy --strict newtypert.py
newtypert.py:7: error: Cannot use isinstance() with NewType type  [misc]
Found 1 error in 1 file (checked 1 source file)
```

To check data at run time, make a real class.

## Comparison

In Python, these forms tell the checker things that a class cannot say: the type of the current subclass, a type with no values, a class attribute, extra data or a type that exists only for the checker. `NewType` is like a newtype in Haskell or a branded type in TypeScript, and it costs nothing at run time. For the type with no values, see [[bottom-type]]. For `assert_never`, which takes a `Never`, see [[python/type-narrowing]].
