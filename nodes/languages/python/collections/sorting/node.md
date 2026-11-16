---
tag: XE7GG5
kind: feature
owner: python
slug: sorting
name: Sorting
aliases: [sort, sorted, list.sort, key function, reverse sort, stable sort, itemgetter, attrgetter, cmp_to_key, sort by multiple keys, Timsort]
area: languages/python/collections
level: core
summary: >-
  list.sort() sorts a list in place and returns None, and sorted() returns a new sorted list from any iterable. Both take a key function and a reverse flag, and both sorts are stable.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/howto/sorting.html", locator: "Sorting Techniques"}
see_also: [python/lists, python/comparing-sequences-and-other-types, python/tools-for-working-with-lists, python/lambda-expressions, python/itertools]
sources:
  - url: https://docs.python.org/3.15/howto/sorting.html
    title: Sorting Techniques, Python 3.15 documentation
    locator: Sorting Basics; Key Functions; Sort Stability; Comparison Functions; Strategies For Unorderable Types
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#sorted
    title: Built-in Functions, Python 3.15 documentation
    locator: sorted()
    accessed: 2026-11-16
  - url: https://pkg.go.dev/slices#SortFunc
    title: slices package, Go standard library
    locator: SortFunc; SortStableFunc
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
print(sorted([5, 2, 3, 1, 4]))
a = [5, 2, 3, 1, 4]
a.sort(reverse=True)
print(a)
print(sorted("This is a test string from Andrew".split(), key=str.casefold))
print(sorted({1: "D", 2: "B", 3: "B"}))
```

```output for=basics
[1, 2, 3, 4, 5]
[5, 4, 3, 2, 1]
['a', 'Andrew', 'from', 'is', 'string', 'test', 'This']
[1, 2, 3]
```

`sorted()` accepts any iterable. A dict gives its keys.

## Semantics

**r1.** "Python lists have a built-in list.sort() method that modifies the list in-place. There is also a sorted() built-in function that builds a new sorted list from an iterable."[^1] list.sort() "modifies the list in-place (and returns None to avoid confusion)."[^1] Example: [`sort-result`](#sort-result).

**r2.** "The value of the key parameter should be a function (or other callable) that takes a single argument and returns a key to use for sorting purposes. This technique is fast because the key function is called exactly once for each input record."[^1]

**r3.** "Sorts are guaranteed to be stable. That means that when multiple records have the same key, their original order is preserved."[^1] "The built-in sorted() function is guaranteed to be stable."[^2] This lets a program sort in several passes, the secondary key first. Example: [`multi-pass`](#multi-pass).

**r4.** "Both list.sort() and sorted() accept a reverse parameter with a boolean value."[^1] "The reverse parameter still maintains sort stability (so that records with equal keys retain the original order)."[^1]

**r5.** "The sort routines use < when making comparisons between two objects. So, it is easy to add a standard sort order to a class by defining an __lt__() method".[^1] Example: [`lt`](#lt).

**r6.** "Python provides functools.cmp_to_key to wrap the comparison function to make it usable as a key function".[^1] Example: [`cmp`](#cmp).

**r7.** "most cross-type comparisons raise a TypeError."[^1] Also, "None is not comparable to other types."[^1] Example: [`mixed`](#mixed).

## Examples

```python run id=keys
from operator import itemgetter, attrgetter

students = [("john", "A", 15), ("jane", "B", 12), ("dave", "B", 10)]
print(sorted(students, key=lambda s: s[2]))
print(sorted(students, key=itemgetter(1, 2)))
print(sorted(students, key=itemgetter(2), reverse=True))
```

```output for=keys
[('dave', 'B', 10), ('jane', 'B', 12), ('john', 'A', 15)]
[('john', 'A', 15), ('dave', 'B', 10), ('jane', 'B', 12)]
[('john', 'A', 15), ('jane', 'B', 12), ('dave', 'B', 10)]
```

`itemgetter(1, 2)` sorts by grade and then by age.

```python run id=multi-pass
from operator import itemgetter

data = [("red", 1), ("blue", 1), ("red", 2), ("blue", 2)]
print(sorted(data, key=itemgetter(0)))

students = [("john", "A", 15), ("jane", "B", 12), ("dave", "B", 10)]
s = sorted(students, key=itemgetter(2))
print(sorted(s, key=itemgetter(1), reverse=True))
```

```output for=multi-pass
[('blue', 1), ('blue', 2), ('red', 1), ('red', 2)]
[('dave', 'B', 10), ('jane', 'B', 12), ('john', 'A', 15)]
```

The second sort keeps the age order inside each grade (rule r3). The result is grade descending and age ascending.

```python run id=lt
class Student:
    def __init__(self, name, age):
        self.name, self.age = name, age

    def __lt__(self, other):
        return self.age < other.age

people = [Student("john", 15), Student("dave", 10), Student("jane", 12)]
print([p.name for p in sorted(people)])
print(min(people).name)
```

```output for=lt
['dave', 'jane', 'john']
dave
```

```python run id=cmp
from functools import cmp_to_key

def by_length_then_text(a, b):
    if len(a) != len(b):
        return len(a) - len(b)
    return (a > b) - (a < b)

print(sorted(["bb", "a", "ccc", "ab"], key=cmp_to_key(by_length_then_text)))
print(sorted(["bb", "a", "ccc", "ab"], key=lambda w: (len(w), w)))
```

```output for=cmp
['a', 'ab', 'bb', 'ccc']
['a', 'ab', 'bb', 'ccc']
```

A tuple key often does the same job as a comparison function (rule r6).

## Pitfalls

`list.sort()` returns None:

```python wrong id=sort-result
names = ["b", "a"]
ordered = names.sort()
print(ordered, names)
```

```output for=sort-result
None ['a', 'b']
```

Use `sorted(names)` when the program needs a new list (rule r1).

Values that cannot be compared cannot be sorted:

```python fails id=mixed expect=exception:TypeError
sorted([3.3, None, 1.1])
```

Remove the None values first, for example `sorted(x for x in data if x is not None)` (rule r7).

## Comparison

Go sorts with a comparison function, and its basic sort is not stable. The Go documentation says: "SortFunc sorts the slice x in ascending order as determined by the cmp function. This sort is not guaranteed to be stable."[^3] A Go program calls SortStableFunc to keep equal items in order. Every Python sort is stable (rule r3), and the usual tool is a key function, not a comparison function.
