---
tag: T0EZRY
kind: feature
owner: python
slug: generic-alias-types
name: Generic alias types
aliases: [GenericAlias, types.GenericAlias, "list[int]", __class_getitem__, __origin__, __args__, union object, types.UnionType, typing.Union, PEP 585, PEP 560]
area: languages/python/static-typing
level: advanced
summary: >-
  At run time, list[int] is a types.GenericAlias object, made by list.__class_getitem__, and int | str is a union object. They keep their arguments, but Python does not check values against them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#emulating-generic-types", locator: "3.3.5. Emulating generic types"}
proposals: [pep-585, pep-560, pep-604]
see_also: [python/generic-functions-and-classes, python/basic-type-annotations, python/type-aliases, python/special-typing-forms, python/enums, python/collections-abc]
sources:
  - url: https://docs.python.org/3.15/library/stdtypes.html#types-genericalias
    title: Built-in Types, Python 3.15 documentation
    locator: Generic Alias Type; Special Attributes of GenericAlias objects; Union Type
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0585/
    title: "PEP 585: Type Hinting Generics In Standard Collections"
    locator: Abstract
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/typing.html#typing.get_origin
    title: typing, Support for type hints, Python 3.15 documentation
    locator: typing.get_origin; typing.get_args
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/datamodel.html#emulating-generic-types
    title: Data model, Python 3.15 documentation
    locator: 3.3.5. Emulating generic types; 3.3.5.1; 3.3.5.2
    accessed: 2026-11-18
  - url: https://peps.python.org/pep-0560/
    title: "PEP 560: Core support for typing module and generic types"
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
  - url: https://docs.python.org/3.15/whatsnew/3.14.html
    title: What’s New In Python 3.14
    locator: typing
    accessed: 2026-11-18
---

## Synopsis

```python run id=alias
import types
from typing import get_args, get_origin

t = dict[str, list[int]]
print(t, isinstance(t, types.GenericAlias))
print(get_origin(t), get_args(t))
d = t(a=[1, 2])
print(type(d), d)
```

```output for=alias
dict[str, list[int]] True
<class 'dict'> (<class 'str'>, list[int])
<class 'dict'> {'a': [1, 2]}
```

A subscription of `dict` gives an alias object. The object keeps the class and the arguments, and `get_origin` and `get_args` read them. A call of the alias makes a plain `dict`.

## Semantics

**r1.** "GenericAlias objects are generally created by subscripting a class."[^1] "GenericAlias objects are intended primarily for use with type annotations."[^1] PEP 585 "proposes to enable support for the generics syntax in all standard collections currently available in the typing module."[^2] Example: [`alias`](#alias).

**r2.** The alias has the attributes `__origin__`, `__args__` and `__parameters__`. `__origin__` "points at the non-parameterized generic class".[^1] `__args__` "is a tuple (possibly of length 1) of generic types passed to the original __class_getitem__() of the generic class".[^1] The functions `typing.get_origin` and `typing.get_args` read the same data, and they work for the forms of the `typing` module too. `get_origin` will "Get the unsubscripted version of a type".[^3] Example: [`alias`](#alias).

**r3.** A subscription of a class calls `__class_getitem__`. "A class can generally only be parameterized if it defines the special class method __class_getitem__()."[^4] "Usually, the subscription of an object using square brackets will call the __getitem__() instance method defined on the object’s class. However, if the object being subscribed is itself a class, the class method __class_getitem__() may be called instead."[^4] PEP 560 added the method: "It is proposed to add two special methods __class_getitem__ and __mro_entries__ to the core CPython for better support of generic types."[^5] Examples: [`class-getitem`](#class-getitem), [`enum-index`](#enum-index).

**r4.** "When defined on a class, __class_getitem__() is automatically a class method."[^4] It is for type hints only: "Using __class_getitem__() on any class for purposes other than type hinting is discouraged."[^4] For a generic class that the checkers understand, use a type parameter list or `typing.Generic`.[^4] Example: [`class-getitem`](#class-getitem).

**r5.** Python does not check values against an alias. "The Python runtime does not enforce type annotations. This extends to generic types and their type parameters."[^1] "Furthermore, parameterized generics erase type parameters during object creation".[^1] "The builtin functions isinstance() and issubclass() do not accept GenericAlias types for their second argument".[^1] Examples: [`unchecked`](#unchecked), [`isinstance-alias`](#isinstance-alias).

**r6.** "A union object holds the value of the | (bitwise or) operation on multiple type objects."[^1] Unions compare as sets: "Unions of unions are flattened", "Redundant types are removed" and "When comparing unions, the order is ignored".[^1] `isinstance` accepts a union of classes. PEP 604 "allows it to appear in isinstance and issubclass calls."[^6] Example: [`union`](#union).

## Examples

```python run id=class-getitem
import types


class Box:
    def __class_getitem__(cls, item: object) -> types.GenericAlias:
        return types.GenericAlias(cls, item)

    def __getitem__(self, key: int) -> str:
        return f"item {key}"


print(Box[int], Box[int].__origin__ is Box)
print(Box()[3])
```

```output for=class-getitem
__main__.Box[int] True
item 3
```

`Box[int]` subscribes the class, so Python calls `__class_getitem__`. `Box()[3]` subscribes an object, so Python calls `__getitem__` (rule r3). Mypy does not understand a custom `__class_getitem__`, as the data model says: "Custom implementations of __class_getitem__() on classes defined outside of the standard library may not be understood by third-party type-checkers such as mypy."[^4]

```text
$ mypy --strict box.py
box.py:12: error: The type "type[Box]" is not generic and not indexable  [misc]
Found 1 error in 1 file (checked 1 source file)
```

```python run id=enum-index
from enum import Enum


class Color(Enum):
    RED = "r"
    BLUE = "b"


print(Color["RED"], type(Color).__name__)
```

```output for=enum-index
Color.RED EnumType
```

The metaclass of `Color` defines `__getitem__`, so `Color["RED"]` looks up a member and does not make an alias (rule r3).

```python run id=union
import typing

u = int | str
print(u, u == (str | int), (int | str) | int == u)
print(isinstance(3, u), isinstance(3.0, u))
print(typing.Optional[int] == (int | None))
```

```output for=union
int | str True True
True False
True
```

The order and the repeated `int` have no effect, and `isinstance` accepts the union (rule r6).

## Versions

> [!since v="3.9"]
> The built-in collections accept subscription, from PEP 585. "In type annotations you can now use built-in collection types such as list and dict as generic types instead of importing the corresponding capitalized types (e.g. List or Dict) from typing."[^7]

> [!since v="3.10"]
> The `X | Y` union objects are added, from PEP 604.[^1][^6]

> [!since v="3.14"]
> "Union objects are now instances of typing.Union. Previously, they were instances of types.UnionType, which remains an alias for typing.Union."[^1] "Use == to compare unions for equality, not is."[^8]

## Pitfalls

```python wrong id=unchecked
t = list[str]
values = t([1, 2, 3])
print(values, type(values))
```

```output for=unchecked
[1, 2, 3] <class 'list'>
```

The alias makes a `list` of numbers. The type argument `str` is not checked, and the result does not keep it (rule r5). The checker finds the numbers:

```text
$ mypy --strict unchecked.py
unchecked.py:2: error: List item 0 has incompatible type "int"; expected "str"  [list-item]
unchecked.py:2: error: List item 1 has incompatible type "int"; expected "str"  [list-item]
unchecked.py:2: error: List item 2 has incompatible type "int"; expected "str"  [list-item]
Found 3 errors in 1 file (checked 1 source file)
```

```python fails id=isinstance-alias expect=exception:TypeError
print(isinstance([1], list))
print(isinstance([1], list[int]))
```

The checker reports the same line:

```text
$ mypy --strict isalias.py
isalias.py:2: error: Parameterized generics cannot be used with class or instance checks  [misc]
Found 1 error in 1 file (checked 1 source file)
```

To test a list at run time, test the `list` class and then each element (rule r5).

## Comparison

In Python, `list[int]` is an ordinary object at run time, so a program can store it, print it and read its arguments. Java erases type arguments at run time too, but `List<Integer>` there is not a value. For generic classes that the checker understands, see [[python/generic-functions-and-classes]]. For the classes that support subscription, see [[python/collections-abc]].
