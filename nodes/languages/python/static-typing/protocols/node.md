---
tag: RTW4ST
kind: feature
owner: python
slug: protocols
name: Protocols
aliases: [Protocol, typing.Protocol, structural subtyping, static duck typing, runtime_checkable, protocol class, PEP 544]
area: languages/python/static-typing
level: advanced
summary: >-
  A protocol class lists the methods and attributes that a type must have. A class matches the protocol when it has them, with no base class needed. This is structural subtyping, or static duck typing.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/protocol.html", locator: "Protocols"}
proposals: [pep-544]
see_also: [python/duck-typing, python/abstract-base-classes, python/collections-abc, python/generic-functions-and-classes, python/variance-in-python, python/type-checkers, subtyping]
sources:
  - url: https://mypy.readthedocs.io/en/stable/protocols.html
    title: Protocols and structural subtyping, mypy documentation
    locator: Protocols and structural subtyping; Defining subprotocols and subclassing protocols; Using isinstance() with protocols
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0544/
    title: "PEP 544: Protocols: Structural subtyping (static duck typing)"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.Protocol
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.Protocol; typing.runtime_checkable
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/protocol.html
    title: Protocols, Specification for the Python type system
    locator: Terminology; Explicitly declaring implementation; Type checking protocols
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.8.html#typing
    title: What’s New In Python 3.8
    locator: typing
    accessed: 2026-11-18
---

## Synopsis

```python run id=close-all
from collections.abc import Iterable
from typing import TYPE_CHECKING, Protocol


class SupportsClose(Protocol):
    def close(self) -> None: ...


class Resource:
    def __init__(self, name: str) -> None:
        self.name = name

    def close(self) -> None:
        print("closed", self.name)


def close_all(items: Iterable[SupportsClose]) -> None:
    for item in items:
        item.close()


close_all([Resource("db"), Resource("log")])
if TYPE_CHECKING:
    close_all([Resource("db"), "log"])
```

```output for=close-all
closed db
closed log
```

`Resource` does not name `SupportsClose` as a base class, but it has a `close` method of the right type, so it matches the protocol. A string has no `close` method. Saved as `closeall.py`:

```text
$ mypy --strict closeall.py
closeall.py:24: error: List item 1 has incompatible type "str"; expected "SupportsClose"  [list-item]
Found 1 error in 1 file (checked 1 source file)
```

## Semantics

**r1.** Python has two ways to decide that one type is a subtype of another. "Nominal subtyping is strictly based on the class hierarchy."[^1] "Structural subtyping is based on the operations that can be performed with an object. Class Dog is a structural subtype of class Animal if the former has all attributes and methods of the latter, and with compatible types."[^1] A protocol gives structural subtyping. PEP 544 specifies "static and runtime semantics of protocol classes that will provide a support for structural subtyping (static duck typing)."[^2]

**r2.** A protocol is a class with `Protocol` as a base. "Such classes are primarily used with static type checkers that recognize structural subtyping (static duck-typing)".[^3] "The attributes (variables and methods) of a protocol that are mandatory for another class for it to be assignable to the protocol are called “protocol members”."[^4] Example: [`close-all`](#close-all).

**r3.** The types of the members must match, not only the names. "If a class defines all attributes and methods of a protocol with types that are assignable to the types of the protocol’s attributes and methods, it is said to implement the protocol and to be assignable to the protocol."[^4] Example: [`mismatch`](#mismatch).

**r4.** A class can also name the protocol as a base class. "In this case a class could use default implementations of protocol members."[^4] "The default implementations cannot be used if the assignable-to relationship is implicit and only structural".[^4] Example: [`explicit`](#explicit).

**r5.** A subclass of a protocol is a normal class, unless it names `Protocol` again. "Note that inheriting from an existing protocol does not automatically turn the subclass into a protocol".[^1] "Protocols cannot be instantiated, so there are no values whose runtime type is a protocol."[^4] Example: [`no-instance`](#no-instance).

**r6.** A protocol can be generic. "Protocol classes can be generic".[^3] Example: [`generic`](#generic).

**r7.** At run time, `isinstance` works only with a protocol that has the decorator `@runtime_checkable`. "Protocol classes without this decorator cannot be used as the second argument to isinstance() or issubclass()."[^3] The check is weak: "runtime_checkable will check only the presence of the required methods or attributes, not their type signatures or types."[^3] Examples: [`runtime`](#runtime), [`not-runtime`](#not-runtime), [`presence`](#presence).

## Examples

```python run id=explicit
from typing import TYPE_CHECKING, Protocol


class Greeter(Protocol):
    def name(self) -> str: ...

    def greet(self) -> str:
        return "hello " + self.name()


class English(Greeter):
    def name(self) -> str:
        return "ann"


class Plain:
    def name(self) -> str:
        return "bo"


def run(g: Greeter) -> str:
    return g.greet()


print(run(English()), hasattr(Plain(), "greet"))
if TYPE_CHECKING:
    run(Plain())
```

```output for=explicit
hello ann False
```

`English` names `Greeter` as a base, so it gets `greet` from the protocol. `Plain` does not, so it has no `greet`, and it does not match (rule r4):

```text
$ mypy --strict explicit.py
explicit.py:27: error: Argument 1 to "run" has incompatible type "Plain"; expected "Greeter"  [arg-type]
explicit.py:27: note: "Plain" is missing following "Greeter" protocol member:
explicit.py:27: note:     greet
Found 1 error in 1 file (checked 1 source file)
```

```python run id=generic
from typing import TYPE_CHECKING, Protocol


class Source[T](Protocol):
    def get(self) -> T: ...


class Counter:
    def __init__(self) -> None:
        self.n = 0

    def get(self) -> int:
        self.n += 1
        return self.n


def take_two[T](src: Source[T]) -> list[T]:
    return [src.get(), src.get()]


pair = take_two(Counter())
if TYPE_CHECKING:
    reveal_type(pair)
print(pair)
```

```output for=generic
[1, 2]
```

`Counter.get` returns an `int`, so `Counter` matches `Source[int]`, and the checker solves `T` as `int` (rule r6):

```text
$ mypy --strict generic.py
generic.py:23: note: Revealed type is "list[int]"
Success: no issues found in 1 source file
```

```python run id=runtime
from typing import Protocol, runtime_checkable


@runtime_checkable
class Named(Protocol):
    name: str


class User:
    def __init__(self, name: str) -> None:
        self.name = name


print(isinstance(User("ann"), Named), isinstance(42, Named))
print(Named in User.__mro__)
```

```output for=runtime
True False
False
```

The object of `User` has the attribute `name`, so `isinstance` returns `True`, though `Named` is not a base class of `User` (rule r7).

## Versions

> [!since v="3.8"]
> `typing.Protocol` and `@typing.runtime_checkable` are added, from PEP 544. "Protocol definitions. See PEP 544, typing.Protocol and @typing.runtime_checkable."[^5]

## Pitfalls

```python fails id=mismatch expect=exception:TypeError
from typing import Protocol


class SupportsRead(Protocol):
    def read(self, amount: int) -> bytes: ...


class Reader:
    def read(self) -> bytes:
        return b"data"


def head(src: SupportsRead) -> bytes:
    return src.read(2)


print(head(Reader()))
```

`Reader` has a method with the name `read`, but the method takes no `amount`. The call fails at run time, and the checker reports the difference (rule r3):

```text
$ mypy --strict mismatch.py
mismatch.py:17: error: Argument 1 to "head" has incompatible type "Reader"; expected "SupportsRead"  [arg-type]
mismatch.py:17: note: Following member(s) of "Reader" have conflicts:
mismatch.py:17: note:     Expected:
mismatch.py:17: note:         def read(self, amount: int) -> bytes
mismatch.py:17: note:     Got:
mismatch.py:17: note:         def read(self) -> bytes
Found 1 error in 1 file (checked 1 source file)
```

```python fails id=not-runtime expect=exception:TypeError
from typing import Protocol


class SupportsClose(Protocol):
    def close(self) -> None: ...


print(isinstance(open, SupportsClose))
```

The protocol has no `@runtime_checkable`, so `isinstance` raises `TypeError` (rule r7).

```python wrong id=presence
from typing import Protocol, runtime_checkable


@runtime_checkable
class SupportsRead(Protocol):
    def read(self, amount: int) -> bytes: ...


class Broken:
    read = "not a method"


print(isinstance(Broken(), SupportsRead))
```

```output for=presence
True
```

The run-time check finds an attribute with the name `read`, and it does not look at its type. The string `"not a method"` passes (rule r7). Use `isinstance` with a protocol only as a rough check.

```python fails id=no-instance expect=exception:TypeError
from typing import Protocol


class SupportsClose(Protocol):
    def close(self) -> None: ...


c = SupportsClose()
```

A protocol describes other classes, and it has no objects of its own (rule r5). The checker reports the same line:

```text
$ mypy --strict noinst.py
noinst.py:8: error: Cannot instantiate protocol class "SupportsClose"  [misc]
Found 1 error in 1 file (checked 1 source file)
```

## Comparison

In Python, a protocol is the static form of duck typing: the checker accepts an object that has the right methods, whatever its class is (rule r1). An abstract base class works by name: a class must inherit from it or register with it. For the run-time form of the idea, see [[python/duck-typing]]. For the abstract base classes, see [[python/abstract-base-classes]]. For the theory, see [[subtyping]].
