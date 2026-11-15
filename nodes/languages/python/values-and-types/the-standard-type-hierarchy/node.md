---
tag: HM33EM
kind: feature
owner: python
slug: the-standard-type-hierarchy
name: The standard type hierarchy
aliases: [Python built-in types, Python type hierarchy, data model types, sequences sets and mappings, callable types]
area: languages/python/values-and-types
level: core
summary: >-
  Section 3.2 of the Language Reference lists the built-in types: three singletons, numbers, sequences, sets, mappings, callables, modules, classes and instances, file objects and internal types.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#the-standard-type-hierarchy", locator: "3.2. The standard type hierarchy"}
see_also: [python/objects-values-and-types, python/none-notimplemented-and-ellipsis, type-system]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#the-standard-type-hierarchy
    title: The Python Language Reference, Python 3.15
    locator: 3.2. The standard type hierarchy (3.2.1 to 3.2.13)
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Types
    title: The Go Programming Language Specification
    locator: Types
    accessed: 2026-11-16
---

## Synopsis

```python run id=type-map
import types

samples = [None, NotImplemented, ..., 1, True, 1.5, 2j, "s", b"b", (1,), [1],
           bytearray(b"x"), {1}, frozenset({1}), {"k": 1}, len, types]
print(" ".join(type(v).__name__ for v in samples))
```

```output for=type-map
NoneType NotImplementedType ellipsis int bool float complex str bytes tuple list bytearray set frozenset dict builtin_function_or_method module
```

One value of most groups in section 3.2, with the name of its type.

## Semantics

**r1.** Section 3.2 is "a list of the types that are built into Python". Extension modules can define more types, and future versions can add types, often through the standard library.[^1]

**r2.** Three types have a single value each: `None`, `NotImplemented` and `Ellipsis` (3.2.1 to 3.2.3).[^1]

**r3.** Numbers (3.2.4) are immutable. The integral types are `int`, with "an unlimited range, subject to available (virtual) memory only", and `bool`. `float` represents "machine-level double precision floating-point numbers", and `complex` is a pair of them.[^1]

**r4.** Sequences (3.2.5) are "finite ordered sets indexed by non-negative numbers". The immutable sequences are strings, tuples and bytes, and the mutable ones are lists and byte arrays.[^1]

**r5.** Set types (3.2.6) are "unordered, finite sets of unique, immutable objects": `set` is mutable and `frozenset` is immutable. Numbers that compare equal, such as `1` and `1.0`, count as one element.[^1] Example: [`equal-numbers-in-a-set`](#equal-numbers-in-a-set).

**r6.** Mappings (3.2.7) are finite sets of objects indexed by arbitrary index sets, read and written with `a[k]`. The two intrinsic mappings are dictionaries and frozen dictionaries. Frozen dictionaries are made by `frozendict()`, and Python 3.15 added them.[^1]

**r7.** Callable types (3.2.8) are the types that the call operation can be applied to: user-defined functions, instance methods, generator, coroutine and asynchronous generator functions, built-in functions and methods, classes, and class instances with a `__call__()` method.[^1] Example: [`callables`](#callables).

**r8.** The list ends with modules (3.2.9), custom classes and class instances (3.2.10, 3.2.11), file objects (3.2.12) and internal types (3.2.13). The internal types include code, frame, traceback and slice objects, and their definitions "may change with future versions of the interpreter".[^1]

## Examples

Each kind of callable has its own type, and `callable()` is true for all of them (rule r7).

```python run id=callables
def f():
    pass

class C:
    def m(self):
        pass

def gen():
    yield 1

for obj in [f, C().m, gen, len, [].append, C]:
    print(type(obj).__name__, callable(obj))
print(type(f.__code__).__name__, type(slice(1, 2)).__name__)
```

```output for=callables
function True
method True
function True
builtin_function_or_method True
builtin_function_or_method True
type True
code slice
```

## Pitfalls

### `True`, `1` and `1.0` in one set

`bool` is an integral type (rule r3), and `True == 1 == 1.0`, so a set keeps only the first of them (rule r5).

```python wrong id=equal-numbers-in-a-set
s = {1, 1.0, True}
print(s, len(s))
print(type(True).__mro__)
```

```output for=equal-numbers-in-a-set
{1} 1
(<class 'bool'>, <class 'int'>, <class 'object'>)
```

### Type names are not stable API

The internal types can change between versions (rule r8), and some type names, such as `builtin_function_or_method`, describe CPython objects. Test for behavior with `callable()`, `isinstance()` and the abstract base classes, not with the text of `type(x).__name__`.

## Comparison

Go predeclares its boolean, numeric and string types, and builds the composite types with type literals: "array, struct, pointer, function, interface, slice, map, and channel types".[^2] Go has sized numeric types such as `int8` and `float32`, but Python has one integer type with no size limit and one binary floating-point type (rule r3).
