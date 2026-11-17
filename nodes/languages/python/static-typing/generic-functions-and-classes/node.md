---
tag: 31GE0X
kind: feature
owner: python
slug: generic-functions-and-classes
name: Generic functions and classes
aliases: [generics, type parameters, type parameter list, TypeVar, Generic, bound type variable, constrained type variable, type parameter defaults, PEP 695, PEP 696, "def f[T]"]
area: languages/python/static-typing
level: advanced
summary: >-
  A type parameter list, as in def first[T](items: list[T]) -> T, makes a function or a class generic. A type variable can have a bound, constraints or a default. Python does not check type arguments.
grammar: |
  type_params: "[" type_param ("," type_param)* "]"
  type_param:  typevar | typevartuple | paramspec
  typevar:     identifier (":" expression)? ("=" expression)?
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#type-parameter-lists", locator: "The Python Language Reference, 8.10 Type parameter lists"}
proposals: [pep-484, pep-695, pep-696]
see_also: [python/type-aliases, python/basic-type-annotations, python/annotation-scopes, python/collections-abc, parametric-polymorphism, bounded-quantification, type-erasure]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#type-parameter-lists
    title: The Python Language Reference, 8.10 Type parameter lists
    locator: "8.10, 8.10.1, 8.10.2"
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0695/
    title: "PEP 695: Type Parameter Syntax"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://mypy.readthedocs.io/en/stable/generics.html
    title: Generics, mypy documentation
    locator: Type variables with upper bounds; Value-constrained type variables
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#generics
    title: typing, Support for type hints, Python 3.15 documentation
    locator: Generics; User-defined generic types
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.12.html#pep-695-type-parameter-syntax
    title: What’s New In Python 3.12
    locator: "PEP 695: Type Parameter Syntax"
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.13.html
    title: What’s New In Python 3.13
    locator: typing
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0696/
    title: "PEP 696: Type Defaults for Type Parameters"
    locator: Abstract
    accessed: 2026-11-18
---

## Synopsis

```python run id=first
from collections.abc import Sequence
from typing import TYPE_CHECKING


def first[T](items: Sequence[T]) -> T:
    return items[0]


class Box[T]:
    def __init__(self, item: T) -> None:
        self.item = item

    def get(self) -> T:
        return self.item


word = first(["ann", "bo"])
box = Box(3.5)
if TYPE_CHECKING:
    reveal_type(word)
    reveal_type(box)
    reveal_type(box.get())
print(word, box.get(), first.__type_params__, Box.__type_params__)
```

```output for=first
ann 3.5 (T,) (T,)
```

`T` stands for one type, which the checker finds from each call. Saved as `first.py`:

```text
$ mypy --strict first.py
first.py:20: note: Revealed type is "str"
first.py:21: note: Revealed type is "first.Box[float]"
first.py:22: note: Revealed type is "float"
Success: no issues found in 1 source file
```

## Semantics

**r1.** "Functions (including coroutines), classes and type aliases may contain a type parameter list".[^1] "Semantically, this indicates that the function, class, or type alias is generic over a type variable. This information is primarily used by static type checkers, and at runtime, generic objects behave much like their non-generic counterparts."[^1] PEP 695 "specifies an improved syntax for specifying type parameters within a generic class, function, or type alias."[^2]

**r2.** The checker solves each type variable from the arguments of a call. In `first(["ann", "bo"])`, `T` is `str`, so the result is a `str`. Example: [`first`](#first).

**r3.** A type parameter has a scope. "The type parameters are accessible within the scope of the generic object, but not elsewhere. Thus, after a declaration def func[T](): pass, the name T is not available in the module scope."[^1] Example: [`scope`](#scope).

**r4.** A bound limits a type variable to the subtypes of one type. "A single expression after the colon indicates a bound (e.g. T: int). Semantically, this means that the typing.TypeVar can only represent types that are a subtype of this bound."[^1] Example: [`bound`](#bound).

**r5.** Constraints limit a type variable to a fixed list of types. "A parenthesized tuple of expressions after the colon indicates a set of constraints (e.g. T: (str, bytes))."[^1] Mypy explains the difference from a union: "Importantly, this is different from a union type, since combinations of str and bytes are not accepted".[^3] Example: [`constrained`](#constrained).

**r6.** A type parameter can have a default. "All three flavors of type parameters can also have a default value, which is used when the type parameter is not explicitly provided."[^1] Example: [`default`](#default).

**r7.** The old form uses `TypeVar` and the base class `Generic`. "Generic classes implicitly inherit from Generic. For compatibility with Python 3.11 and lower, it is also possible to inherit explicitly from Generic to indicate a generic class".[^4] Example: [`legacy`](#legacy).

**r8.** Python does not check type arguments at run time. `Box[int]("text")` makes a box that holds a string. Example: [`erased`](#erased).

## Examples

```python run id=bound
from collections.abc import Sized
from typing import TYPE_CHECKING


def longest[T: Sized](a: T, b: T) -> T:
    return a if len(a) >= len(b) else b


print(longest("ab", "c"), longest([1], [2, 3]))
if TYPE_CHECKING:
    reveal_type(longest("ab", "c"))
    longest(1, 2)
```

```output for=bound
ab [2, 3]
```

The bound `Sized` lets the body call `len`, and the result keeps the type of the arguments. The call with integers is in the `TYPE_CHECKING` block, so only the checker sees it (rule r4):

```text
$ mypy --strict bound.py
bound.py:11: note: Revealed type is "str"
bound.py:12: error: Value of type variable "T" of "longest" cannot be "int"  [type-var]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=constrained
from typing import TYPE_CHECKING


def join[S: (str, bytes)](a: S, b: S) -> S:
    return a + b


print(join("ab", "cd"), join(b"ab", b"cd"))
if TYPE_CHECKING:
    join("ab", b"cd")
```

```output for=constrained
abcd b'abcd'
```

`S` is `str` for the whole call or `bytes` for the whole call. A mix is an error (rule r5):

```text
$ mypy --strict constrained.py
constrained.py:10: error: Value of type variable "S" of "join" cannot be "Sequence[object]"  [type-var]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=default
from typing import TYPE_CHECKING


class Result[T = str]:
    def __init__(self, value: T) -> None:
        self.value = value


def show(r: Result) -> None:
    if TYPE_CHECKING:
        reveal_type(r)
    print(r.value)


show(Result("ok"))
print(Result.__type_params__[0].__default__)
```

```output for=default
ok
<class 'str'>
```

The hint `Result` with no argument means `Result[str]`, because of the default (rule r6):

```text
$ mypy --strict default.py
default.py:11: note: Revealed type is "default.Result[str]"
Success: no issues found in 1 source file
```

```python run id=legacy
from typing import Generic, TypeVar

T = TypeVar("T")


class Stack(Generic[T]):
    def __init__(self) -> None:
        self.items: list[T] = []

    def push(self, item: T) -> None:
        self.items.append(item)

    def pop(self) -> T:
        return self.items.pop()


s = Stack[int]()
s.push(1)
s.push(2)
print(s.pop(), T.__name__)
```

```output for=legacy
2 T
```

The old form declares `T` as a module variable, and the class lists it in `Generic[T]` (rule r7). The checkers accept both forms.

## Versions

> [!since v="3.12"]
> Type parameter lists are added, from PEP 695. "PEP 695 introduces a new, more compact and explicit way to create generic classes and functions".[^5]

> [!since v="3.13"]
> "PEP 696: Type parameters (typing.TypeVar, typing.ParamSpec, and typing.TypeVarTuple) now support defaults."[^6][^7]

## Pitfalls

```python fails id=scope expect=exception:NameError
def identity[T](x: T) -> T:
    return x


print(identity(5))
print(T)
```

`T` exists only in the function and its hints, so the module has no `T` (rule r3). With the old form, `T = TypeVar("T")` is a module variable, so the two forms differ here.

```python wrong id=erased
class Box[T]:
    def __init__(self, item: T) -> None:
        self.item = item


box = Box[int]("text")
print(box.item, type(box).__name__)
```

```output for=erased
text Box
```

The type argument `int` has no effect at run time, and the object is a plain `Box` (rule r8). The checker reports the call:

```text
$ mypy --strict erased.py
erased.py:6: error: Argument 1 to "Box" has incompatible type "str"; expected "int"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

## Comparison

In Python, generics exist for the checker, and a generic object at run time is the same as a plain one (rule r1). This is like [[type-erasure]]: the type arguments are not kept in the objects. The bound of a type variable is a form of [[bounded-quantification]]. For the subtype rules of generic types, see [[python/variance-in-python]]. For the general idea, see [[parametric-polymorphism]].
