---
tag: ZKQA9A
kind: feature
owner: python
slug: variance-in-python
name: Variance in Python
aliases: [variance, covariance, contravariance, invariance, covariant, contravariant, invariant, T_co, T_contra, infer_variance, variance inference]
area: languages/python/static-typing
level: advanced
summary: >-
  Variance tells whether Box[Circle] is a subtype of Box[Shape]. list and dict are invariant, Sequence and Mapping are covariant, and the argument types of Callable are contravariant.
grammar: null
spec: {standard: null, url: "https://typing.python.org/en/latest/spec/generics.html#variance", locator: "Generics, Variance; Variance Inference"}
proposals: [pep-484, pep-695]
see_also: [python/generic-functions-and-classes, python/basic-type-annotations, python/collections-abc, python/type-checkers, subtyping, function-type, parametric-polymorphism]
sources:
  - url: https://mypy.readthedocs.io/en/stable/generics.html#variance-of-generic-types
    title: Generics, mypy documentation
    locator: Variance of generic types
    accessed: 2026-11-18
  - url: https://typing.python.org/en/latest/spec/generics.html#variance
    title: Generics, Specification for the Python type system
    locator: Variance; Variance Inference; Auto Variance For Traditional Type Variables
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.TypeVar
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.TypeVar
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0695/
    title: "PEP 695: Type Parameter Syntax"
    locator: Variance Inference
    accessed: 2026-11-18
---

## Synopsis

```python fails id=list-invariant expect=exception:AttributeError
class Shape:
    pass


class Circle(Shape):
    def rotate(self) -> str:
        return "rotated"


def add_one(things: list[Shape]) -> None:
    things.append(Shape())


circles: list[Circle] = [Circle()]
add_one(circles)
print(len(circles), circles[0].rotate())
print(circles[1].rotate())
```

A `Circle` is a `Shape`, but a `list[Circle]` is not a `list[Shape]`. The function adds a plain `Shape` to the list of circles, and the last line fails. Saved as `listinv.py`, the checker rejects the call:

```text
$ mypy --strict listinv.py
listinv.py:15: error: Argument 1 to "add_one" has incompatible type "list[Circle]"; expected "list[Shape]"  [arg-type]
listinv.py:15: note: "list" is invariant -- see https://mypy.readthedocs.io/en/stable/common_issues.html#variance
listinv.py:15: note: Consider using "Sequence" instead, which is covariant
Found 1 error in 1 file (checked 1 source file)
```

## Semantics

**r1.** Variance is the rule that connects the subtypes of a type argument with the subtypes of the generic type. Mypy gives the three kinds. "Assuming that we have a pair of types A and B, and B is a subtype of A, these are defined as follows:"[^1]

- "A generic class MyCovGen[T] is called covariant in type variable T if MyCovGen[B] is always a subtype of MyCovGen[A]."[^1]
- "A generic class MyContraGen[T] is called contravariant in type variable T if MyContraGen[A] is always a subtype of MyContraGen[B]."[^1]
- "A generic class MyInvGen[T] is called invariant in T if neither of the above is true."[^1]

**r2.** A mutable container is invariant, because the code that receives it can put a new item in it. "Most mutable containers are invariant."[^1] The specification gives the reason with a list: "the function might append an Employee instance to the list, which would violate the variable’s type in the caller."[^2] Examples: [`list-invariant`](#list-invariant), [`dict-invariant`](#dict-invariant).

**r3.** A read-only container is covariant. "The read-only collection classes in typing are all declared covariant in their type variable (e.g. Mapping and Sequence). The mutable collection classes (e.g. MutableMapping and MutableSequence) are declared invariant."[^2] So a parameter of type `Sequence[Shape]` accepts a `list[Circle]`. Example: [`sequence-covariant`](#sequence-covariant).

**r4.** A function type is contravariant in its argument types. "That is, Callable[[Shape], int] is a subtype of Callable[[Triangle], int], despite Shape being a supertype of Triangle."[^1] A function that accepts any shape can go where a function for circles is expected. Example: [`callable-contravariant`](#callable-contravariant).

**r5.** With a type parameter list, the checker infers the variance. "Type parameters are inferred to be invariant, covariant, or contravariant depending on how they are used."[^2] "Type parameters implicitly allocated using the new syntax always have inferred variance."[^2] A public attribute counts as mutable: "Without an underscore prefix, the class would be invariant, as the attribute would be understood as a public, mutable attribute".[^1] Example: [`inferred`](#inferred).

**r6.** With the old `TypeVar` form, the type variable declares its variance. "Manually created type variables may be explicitly marked covariant or contravariant by passing covariant=True or contravariant=True. By default, manually created type variables are invariant."[^3] "By convention, it is recommended to use names ending in _co for type variables defined with covariant=True and names ending in _contra for that defined with contravariant=True."[^2] Examples: [`legacy`](#legacy), [`legacy-invariant`](#legacy-invariant).

**r7.** Variance applies to classes only. "Variance is meaningful only when a type variable is bound to a generic class."[^2] A generic function has no variance, because the checker solves its type variables at each call.

## Examples

```python run id=sequence-covariant
from collections.abc import Sequence


class Shape:
    def name(self) -> str:
        return type(self).__name__


class Circle(Shape):
    pass


def names(shapes: Sequence[Shape]) -> list[str]:
    return [s.name() for s in shapes]


circles: list[Circle] = [Circle(), Circle()]
print(names(circles))
```

```output for=sequence-covariant
['Circle', 'Circle']
```

`Sequence` has no method that adds an item, so the function cannot put a plain `Shape` in the list (rule r3). `mypy --strict` accepts the file.

```python run id=callable-contravariant
from collections.abc import Callable


class Shape:
    pass


class Circle(Shape):
    radius = 2.0


def area_of_circle(c: Circle) -> float:
    return 3.14 * c.radius**2


def area_of_any(s: Shape) -> float:
    return 0.0


def total(circles: list[Circle], area: Callable[[Circle], float]) -> float:
    return sum(area(c) for c in circles)


print(total([Circle()], area_of_circle), total([Circle()], area_of_any))
```

```output for=callable-contravariant
12.56 0.0
```

`total` calls `area` with circles only, so a function that accepts every `Shape` is safe there (rule r4). `mypy --strict` accepts both calls.

```python run id=inferred
from typing import TYPE_CHECKING


class Shape:
    pass


class Circle(Shape):
    pass


class Box[T]:
    def __init__(self, content: T) -> None:
        self._content = content

    def get(self) -> T:
        return self._content


class Cell[T]:
    def __init__(self, content: T) -> None:
        self.content = content


def look(box: Box[Shape]) -> str:
    return type(box.get()).__name__


def peek(cell: Cell[Shape]) -> str:
    return type(cell.content).__name__


print(look(Box(Circle())))
if TYPE_CHECKING:
    circle_cell: Cell[Circle] = Cell(Circle())
    peek(circle_cell)
```

```output for=inferred
Circle
```

`Box` only returns its content, so the checker infers that `Box` is covariant. `Cell` has a public attribute that code can assign, so `Cell` is invariant (rule r5):

```text
$ mypy --strict inferred.py
inferred.py:36: error: Argument 1 to "peek" has incompatible type "Cell[Circle]"; expected "Cell[Shape]"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=legacy
from collections.abc import Iterator
from typing import Generic, TypeVar

T_co = TypeVar("T_co", covariant=True)


class Frozen(Generic[T_co]):
    def __init__(self, *items: T_co) -> None:
        self._items = items

    def __iter__(self) -> Iterator[T_co]:
        return iter(self._items)


def total(values: Frozen[float]) -> float:
    return sum(values)


ints: Frozen[int] = Frozen(1, 2, 3)
print(total(ints))
print(T_co.__covariant__, T_co.__contravariant__)
```

```output for=legacy
6
True False
```

`T_co` is declared covariant, so a `Frozen[int]` is a `Frozen[float]` for the checker (rule r6). At run time, the type variable keeps the flags.

## Versions

> [!since v="3.12"]
> "Type variables can now be declared using the type parameter syntax introduced by PEP 695. The infer_variance parameter was added."[^3] With the new syntax, the checker infers the variance.[^4]

## Pitfalls

```python wrong id=dict-invariant
from collections.abc import Mapping


def mean(scores: dict[str, float]) -> float:
    return sum(scores.values()) / len(scores)


def mean_ok(scores: Mapping[str, float]) -> float:
    return sum(scores.values()) / len(scores)


counts: dict[str, int] = {"ann": 3, "bo": 4}
print(mean(counts), mean_ok(counts))
```

```output for=dict-invariant
3.5 3.5
```

Both calls work at run time, but `dict` is invariant, so the checker rejects the first call (rule r2). A function that only reads the dictionary can take a `Mapping`:

```text
$ mypy --strict dictinv.py
dictinv.py:13: error: Argument 1 to "mean" has incompatible type "dict[str, int]"; expected "dict[str, float]"  [arg-type]
dictinv.py:13: note: "dict" is invariant -- see https://mypy.readthedocs.io/en/stable/common_issues.html#variance
dictinv.py:13: note: Consider using "Mapping" instead, which is covariant in the value type
Found 1 error in 1 file (checked 1 source file)
```

```python wrong id=legacy-invariant
from typing import Generic, TypeVar

T = TypeVar("T")


class Frozen(Generic[T]):
    def __init__(self, item: T) -> None:
        self._item = item

    def get(self) -> T:
        return self._item


def show(value: Frozen[float]) -> None:
    print(value.get())


ints: Frozen[int] = Frozen(1)
show(ints)
```

```output for=legacy-invariant
1
```

The class is read-only, but a plain `TypeVar` is invariant, and the checker does not infer the variance for the old form (rule r6):

```text
$ mypy --strict legacybad.py
legacybad.py:19: error: Argument 1 to "show" has incompatible type "Frozen[int]"; expected "Frozen[float]"  [arg-type]
Found 1 error in 1 file (checked 1 source file)
```

Use `TypeVar("T_co", covariant=True)`, or write the class with a type parameter list, as in `class Frozen[T]`.

## Comparison

In Python, variance exists for the checker only, and the program runs the same with any variance (rule r1). With the new syntax, the checker infers it from the body of the class, and with the old syntax, the type variable declares it (rules r5 and r6). The rules for read-only containers and for function arguments follow from [[subtyping]]: a value can go where a value of a supertype is expected, if each use of it stays safe. For function types, see [[function-type]].
