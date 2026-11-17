---
tag: V0GR37
kind: feature
owner: python
slug: type-aliases
name: Type aliases and the type statement
aliases: [type alias, type statement, TypeAliasType, TypeAlias, generic type alias, recursive type alias, PEP 695, PEP 613]
area: languages/python/static-typing
level: core
summary: >-
  The type statement gives a name to a type, as in type Point = tuple[float, float]. The alias can be generic and recursive, and Python evaluates its value only when the program asks for it.
grammar: |
  type_stmt: 'type' identifier [type_params] "=" expression
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-type-statement", locator: "The Python Language Reference, 7.14 The type statement"}
proposals: [pep-695, pep-613]
see_also: [python/type-hints, python/basic-type-annotations, python/annotation-scopes, python/soft-keywords, python/annotations-at-runtime, type-constructor]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-type-statement
    title: The Python Language Reference, 7.14 The type statement
    locator: "7.14"
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#type-aliases
    title: typing, Support for type hints, Python 3.15 documentation
    locator: Type aliases; typing.TypeAlias; typing.TypeAliasType
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#generic-type-aliases
    title: The Python Language Reference, 8.10.3 Generic type aliases
    locator: "8.10.3"
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0695/
    title: "PEP 695: Type Parameter Syntax"
    locator: Generic Type Alias
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0613/
    title: "PEP 613: Explicit Type Aliases"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/whatsnew/3.12.html#pep-695-type-parameter-syntax
    title: What’s New In Python 3.12
    locator: "PEP 695: Type Parameter Syntax"
    accessed: 2026-11-18
---

## Synopsis

```python run id=alias
type Point = tuple[float, float]
type Path = list[Point]


def length(path: Path) -> float:
    total = 0.0
    for (x1, y1), (x2, y2) in zip(path, path[1:]):
        total += ((x2 - x1) ** 2 + (y2 - y1) ** 2) ** 0.5
    return total


print(length([(0, 0), (3, 4), (3, 0)]))
print(type(Point).__name__, Point.__name__, Point.__value__)
```

```output for=alias
9.0
TypeAliasType Point tuple[float, float]
```

`Point` and `Path` are names for types. The checker treats `Path` as `list[tuple[float, float]]`. At run time, each alias is an object of the class `TypeAliasType`.

## Semantics

**r1.** "The type statement declares a type alias, which is an instance of typing.TypeAliasType."[^1] An alias and its value are the same type for the checker: "In this example, Vector and list[float] will be treated equivalently by static type checkers".[^2] An alias does not make a new type. For a distinct type, see `NewType`.

**r2.** The value is lazy. "It is not evaluated when the type alias is created, but only when the value is accessed through the type alias’s __value__ attribute (see Lazy evaluation). This allows the type alias to refer to names that are not yet defined."[^1] So an alias can refer to itself, which makes a recursive type. Example: [`recursive`](#recursive).

**r3.** An alias can be generic. "Type aliases may be made generic by adding a type parameter list after the name."[^1] "The type statement can also be used to create a generic type alias: type ListOrSet[T] = list[T] | set[T]".[^3] PEP 695 says: "Similar to class and def statements, a type statement defines a scope for type parameters."[^4] Example: [`generic`](#generic).

**r4.** "type is a soft keyword."[^1] A program can still use `type` as a name, and the built-in `type()` works as before. For soft keywords, see [[python/soft-keywords]].

**r5.** The old forms still work. "For backwards compatibility, type aliases can also be created through simple assignment: Vector = list[float]".[^2] `TypeAlias` makes the intent clear: it is a "Special annotation for explicitly declaring a type alias."[^2] An alias made by assignment is a normal variable that holds the type object, and its value is evaluated at once. Example: [`old-style`](#old-style).

**r6.** A `TypeAliasType` is not a class. It is for hints only, so `isinstance` and calls do not accept it. Example: [`not-a-class`](#not-a-class).

## Examples

```python run id=recursive
type Tree = dict[str, Tree] | Leaf
type Leaf = int


def count(tree: Tree) -> int:
    if isinstance(tree, dict):
        return sum(count(child) for child in tree.values())
    return 1


print(count({"a": 1, "b": {"c": 2, "d": 3}}))
print(Tree.__value__)
```

```output for=recursive
3
dict[str, Tree] | Leaf
```

`Tree` refers to itself and to `Leaf`, which comes later in the file. Python evaluates the value only when the code reads `__value__` (rule r2). `mypy --strict` accepts the file.

```python run id=generic
type Pair[T] = tuple[T, T]
type Lookup[K, V] = dict[K, list[V]]


def swap[T](pair: Pair[T]) -> Pair[T]:
    return pair[1], pair[0]


index: Lookup[str, int] = {"a": [1, 2]}
print(swap((1, 2)), index)
print(Pair.__type_params__)
```

```output for=generic
(2, 1) {'a': [1, 2]}
(T,)
```

`Pair[int]` is `tuple[int, int]`, and `Lookup[str, int]` is `dict[str, list[int]]` (rule r3). For type parameters in functions and classes, see [[python/generic-functions-and-classes]].

```python run id=old-style
from typing import TypeAlias

Vector = list[float]
Matrix: TypeAlias = list[Vector]


def total(m: Matrix) -> float:
    return sum(sum(row) for row in m)


print(total([[1.0, 2.0], [3.5]]))
print(type(Vector).__name__, Vector)
```

```output for=old-style
6.5
GenericAlias list[float]
```

`Vector` is a variable that holds the object `list[float]`. The checker treats both names as aliases (rule r5).

## Versions

> [!since v="3.10"]
> `typing.TypeAlias` is added, from PEP 613.[^2][^5]

> [!since v="3.12"]
> The `type` statement and `TypeAliasType` are added, from PEP 695. "In addition, the PEP introduces a new way to declare type aliases using the type statement, which creates an instance of TypeAliasType".[^6] `TypeAlias` is deprecated: "TypeAlias is deprecated in favor of the type statement, which creates instances of TypeAliasType and which natively supports forward references."[^2]

## Pitfalls

```python fails id=not-a-class expect=exception:TypeError
type Names = list[str]
OldNames = list[str]

print(OldNames(["ann"]))
print(isinstance([], Names))
```

The old alias holds a generic alias object, and a call of it makes a list. The new alias is a `TypeAliasType`, and `isinstance` rejects it (rule r6). The checker reports the same line:

```text
$ mypy --strict notclass.py
notclass.py:5: error: Parameterized generics cannot be used with class or instance checks  [misc]
notclass.py:5: error: Argument 2 to "isinstance" has incompatible type "TypeAliasType"; expected "_ClassInfo"  [arg-type]
Found 2 errors in 1 file (checked 1 source file)
```

For a run-time check, use the class itself, as in `isinstance(x, list)`.

```python fails id=undefined-value expect=exception:NameError
type Config = dict[str, Setting]

print(Config.__name__)
print(Config.__value__)
```

The `type` statement succeeds, because the value is lazy (rule r2). The first read of `__value__` evaluates it, and the name `Setting` does not exist.

## Comparison

In Python, the `type` statement makes a lazy alias object, and the checker replaces the alias with its value (rule r1). Many languages with static types have a similar statement that gives a name to a type, and the alias is not a new type there either. In Python, a program can also read the alias at run time, through `__value__` and `__type_params__`. For a type that is distinct from its base, use `typing.NewType`.
