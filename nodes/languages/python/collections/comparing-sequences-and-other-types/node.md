---
tag: DDGH9H
kind: feature
owner: python
slug: comparing-sequences-and-other-types
name: Comparing sequences and other types
aliases: [lexicographical comparison, compare lists, compare tuples, tuple ordering, list equality, mixed type comparison, NaN comparison, compare dicts]
area: languages/python/collections
level: core
summary: >-
  Lists, tuples and strings compare item by item in lexicographical order, and a shorter prefix is smaller. A list never equals a tuple, and ordering across unrelated types raises TypeError.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#value-comparisons", locator: "6.10.1. Value comparisons"}
see_also: [python/sequence-types, python/sorting, python/sets, python/dictionaries, python/hashability]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#comparing-sequences-and-other-types
    title: The Python Tutorial, 5. Data Structures
    locator: 5.8. Comparing Sequences and Other Types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#value-comparisons
    title: The Python Language Reference, Python 3.15
    locator: 6.10.1. Value comparisons
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Comparison_operators
    title: The Go Programming Language Specification
    locator: Comparison operators
    accessed: 2026-11-16
---

## Synopsis

```python run id=lexical
print((1, 2, 3) < (1, 2, 4), [1, 2, 3] < [1, 2, 4])
print("ABC" < "C" < "Pascal" < "Python")
print((1, 2, 3, 4) < (1, 2, 4), (1, 2) < (1, 2, -1))
print((1, 2, 3) == (1.0, 2.0, 3.0))
print((1, 2, ("aa", "ab")) < (1, 2, ("abc", "a"), 4))
```

```output for=lexical
True True
True
True True
True
True
```

These are the examples from the tutorial. The first pair of items that differ decides the result.

## Semantics

**r1.** "The comparison uses lexicographical ordering: first the first two items are compared, and if they differ this determines the outcome of the comparison; if they are equal, the next two items are compared, and so on, until either sequence is exhausted."[^1]

**r2.** "If two items to be compared are themselves sequences of the same type, the lexicographical comparison is carried out recursively."[^1] "If one sequence is an initial sub-sequence of the other, the shorter sequence is the smaller (lesser) one."[^1]

**r3.** "Strings (instances of str) compare lexicographically using the numerical Unicode code points (the result of the built-in function ord()) of their characters."[^2] Example: [`case`](#case).

**r4.** "Sequences (instances of tuple, list, or range) can be compared only within each of their types, with the restriction that ranges do not support order comparison. Equality comparison across these types results in inequality, and ordering comparison across these types raises TypeError."[^2] Examples: [`cross-type`](#cross-type), [`list-vs-tuple`](#list-vs-tuple).

**r5.** "Mappings (instances of dict) compare equal if and only if they have equal (key, value) pairs."[^2] Sets "define order comparison operators to mean subset and superset tests. Those relations do not define total orderings".[^2] Example: [`sets-dicts`](#sets-dicts).

**r6.** "The default behavior for equality comparison (== and !=) is based on the identity of the objects."[^2] "A default order comparison (<, >, <=, and >=) is not provided; an attempt raises TypeError."[^2]

**r7.** "The built-in containers typically assume identical objects are equal to themselves."[^2] For NaN, "not-a-number values are not equal to themselves."[^2] Example: [`nan`](#nan).

**r8.** "comparing objects of different types with < or > is legal provided that the objects have appropriate comparison methods. For example, mixed numeric types are compared according to their numeric value, so 0 equals 0.0, etc. Otherwise, rather than providing an arbitrary ordering, the interpreter will raise a TypeError exception."[^1]

## Examples

```python run id=case
print("apple" < "banana", "Zebra" < "apple", ord("Z"), ord("a"))
print(sorted(["b", "C", "a"]), sorted(["b", "C", "a"], key=str.lower))
```

```output for=case
True True 90 97
['C', 'a', 'b'] ['a', 'b', 'C']
```

Upper case letters have lower code points than lower case letters (rule r3).

```python run id=sort-records
people = [("bob", 25), ("ann", 30), ("bob", 20)]
print(sorted(people))
print(max(people))
```

```output for=sort-records
[('ann', 30), ('bob', 20), ('bob', 25)]
('bob', 25)
```

Tuples sort by the first item, then by the second item (rule r1).

```python run id=sets-dicts
print({1, 2} < {1, 2, 3}, {1, 2} < {2, 3}, {2, 3} < {1, 2})
print({"a": 1, "b": 2} == {"b": 2, "a": 1})
print(range(0, 3) == range(0, 3, 1), [] == [])
```

```output for=sets-dicts
True False False
True
True True
```

`{1, 2}` and `{2, 3}` are neither smaller nor larger than each other (rule r5).

```python run id=nan
nan = float("nan")
print(nan == nan, [nan] == [nan])
```

```output for=nan
False True
```

The list comparison finds the same object in both places, so it treats the items as equal (rule r7).

## Pitfalls

A list and a tuple with the same items are not equal:

```python wrong id=list-vs-tuple
print([1, 2] == (1, 2))
```

```output for=list-vs-tuple
False
```

Convert one side first, for example `tuple([1, 2]) == (1, 2)` (rule r4).

Ordering between unrelated types is an error:

```python fails id=cross-type expect=exception:TypeError
print([1, "a"] < [1, 2])
```

The first items are equal, so Python compares `"a"` with `2`, and that raises TypeError (rule r8).

## Comparison

Go arrays and structs support `==` but not `<`. The Go spec says: "Array types are comparable if their array element types are comparable. Two array values are equal if their corresponding element values are equal."[^3] Go strings are ordered and "compared lexically byte-wise".[^3] Python orders lists and tuples too (rule r1), so `sorted()` works on a list of tuples with no extra code.
