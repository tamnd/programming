---
tag: R6775Y
kind: feature
owner: python
slug: built-in-types-reference
name: Built-in types reference
aliases: [Built-in Types, stdtypes, methods of list, methods of dict, methods of str, special attributes, __qualname__, integer string conversion limit, int_max_str_digits]
area: languages/python/stdlib
level: core
summary: >-
  The page Built-in Types describes the types in the interpreter: numbers, sequences, text, bytes, sets, mappings and more. This page maps it, with the methods of each type and the special attributes.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html", locator: "Built-in Types"}
proposals: []
see_also: [python/built-in-functions, python/the-standard-type-hierarchy, python/sequence-types, python/string-methods, python/dictionaries, python/sets, python/truth-value-testing]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html
    title: Built-in Types, Python 3.15 documentation
    locator: Built-in Types; Common Sequence Operations; Special Attributes; Integer string conversion length limitation
    accessed: 2026-11-16
---

## Synopsis

```python run id=methods
for t in (list, dict, set):
    print(t.__name__, [m for m in dir(t) if not m.startswith("_")])
```

```output for=methods
list ['append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']
dict ['clear', 'copy', 'fromkeys', 'get', 'items', 'keys', 'pop', 'popitem', 'setdefault', 'update', 'values']
set ['add', 'clear', 'copy', 'difference', 'difference_update', 'discard', 'intersection', 'intersection_update', 'isdisjoint', 'issubset', 'issuperset', 'pop', 'remove', 'symmetric_difference', 'symmetric_difference_update', 'union', 'update']
```

## Semantics

**r1.** "The principal built-in types are numerics, sequences, mappings, classes, instances and exceptions."[^1]

**r2.** The page has these parts, and each part has its own page in this guide:

- Truth value testing, Boolean operations and comparisons: [[python/truth-value-testing]] and [[python/comparison-operators]].
- Numeric types `int`, `float`, `complex` and `bool`, with bitwise operations and the extra methods of each type: [[python/numbers]].
- Iterators and generators: [[python/generators]].
- Sequence types `list`, `tuple` and `range`, with the common and the mutable operations: [[python/sequence-types]].
- Text `str`, with the string methods, f-strings and t-strings: [[python/string-methods]].
- Binary sequences `bytes`, `bytearray` and `memoryview`: [[python/bytes-and-bytearray]].
- Sets `set` and `frozenset`: [[python/sets]].
- Mappings `dict` and `frozendict`, with the view objects: [[python/dictionaries]].
- Context managers, the generic alias and union types, modules, functions, methods, code and type objects, and `None`, `Ellipsis` and `NotImplemented`.[^1]

**r3.** "Some collection classes are mutable. The methods that add, subtract, or rearrange their members in place, and don’t return a specific item, never return the collection instance itself but None."[^1] Example: [`sort-returns-none`](#sort-returns-none).

**r4.** "practically all objects can be compared for equality, tested for truth value, and converted to a string (with the repr() function or the slightly different str() function)."[^1]

**r5.** The common sequence operations work on all sequence types.[^1] Example: [`sequence-ops`](#sequence-ops).

**r6.** Some objects have special read-only attributes: `__name__`, `__qualname__`, `__module__`, `__doc__` and `__type_params__`. "Some of these are not reported by the dir() built-in function."[^1] Example: [`attributes`](#attributes).

**r7.** "CPython has a global limit for converting between int and str to mitigate denial of service attacks."[^1] "The default limit is 4300 digits as provided in sys.int_info.default_max_str_digits."[^1] Example: [`int-limit`](#int-limit).

## Examples

```python run id=sequence-ops
for seq in ("banana", ("b", "a", "n", "a", "n", "a"), range(0, 12, 2)):
    print(type(seq).__name__, len(seq), seq[1:3], seq.count(seq[1]), seq.index(seq[-1]))
```

```output for=sequence-ops
str 6 an 3 1
tuple 6 ('a', 'n') 3 1
range 6 range(2, 6, 2) 1 5
```

The same code runs on a string, a tuple and a range: length, slicing, `count` and `index` (rule r5). A slice of a range is a range.

```python run id=numbers
print((255).bit_length(), (1024).to_bytes(2, "big"))
print((0.75).as_integer_ratio(), (3.0).is_integer())
print((3 + 4j).conjugate(), abs(3 + 4j))
```

```output for=numbers
8 b'\x04\x00'
(3, 4) True
(3-4j) 5.0
```

Each numeric type has its own methods, for example `int.bit_length`, `int.to_bytes`, `float.as_integer_ratio` and `complex.conjugate`.[^1]

```python run id=sets-and-dicts
a = {1, 2, 3}
b = {2, 3, 4}
print(a | b, a & b, a - b, a ^ b)
prices = {"tea": 3, "cake": 5}
keys = prices.keys()
prices["jam"] = 4
print(list(keys), prices.get("milk", 0))
```

```output for=sets-and-dicts
{1, 2, 3, 4} {2, 3} {1} {1, 4}
['tea', 'cake', 'jam'] 0
```

Sets use the operators `|`, `&`, `-` and `^`. A dictionary view such as `keys()` shows later changes to the dictionary.[^1]

```python run id=attributes
def outer():
    def inner():
        """Do nothing."""
    return inner

f = outer()
print(f.__name__, f.__qualname__, f.__doc__)
```

```output for=attributes
inner outer.<locals>.inner Do nothing.
```

`__qualname__` is "The qualified name of the class, function, method, descriptor, or generator instance", so it shows where the function is defined (rule r6).[^1]

## Versions

> [!since v="3.11"]
> The limit on integer string conversion is new.[^1]

> [!since v="3.15"]
> `frozendict` is a new built-in mapping type.[^1]

## Pitfalls

```python wrong id=sort-returns-none
numbers = [3, 1, 2]
result = numbers.sort()
print(result, numbers)
```

```output for=sort-returns-none
None [1, 2, 3]
```

`list.sort` changes the list and returns `None` (rule r3). Use `sorted(numbers)` to get a new list.

```python fails id=int-limit expect=exception:ValueError
import sys

print(sys.int_info.default_max_str_digits)
big = int("9" * 5000)
```

The string has 5000 digits, which is more than the default limit (rule r7). "When an operation would exceed the limit, a ValueError is raised".[^1] Use `sys.set_int_max_str_digits()` to change the limit for trusted data.

## Comparison

The Built-in Types page is one long reference. Use [[python/built-in-functions]] for the functions that work on these types, and [[python/the-standard-type-hierarchy]] for the data model view of the same types.
