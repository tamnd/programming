---
tag: K7MDHF
kind: feature
owner: python
slug: tools-for-working-with-lists
name: Tools for working with lists
aliases: [array module, bisect, insort, heapq, heap queue, priority queue, binary search, sorted list, compact numeric array]
area: languages/python/collections
level: core
summary: >-
  The array, bisect and heapq modules work with list-like data. array stores numbers compactly, bisect keeps a list sorted with binary search, and heapq keeps the smallest item of a list at index 0.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/stdlib2.html#tools-for-working-with-lists", locator: "11.7. Tools for working with lists"}
see_also: [python/lists, python/lists-as-queues, python/sorting, python/the-collections-module, python/sequence-types]
sources:
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#tools-for-working-with-lists
    title: The Python Tutorial, 11. Brief Tour of the Standard Library, Part II
    locator: 11.7. Tools for working with lists
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/array.html
    title: "array: Efficient arrays of numeric values, Python 3.15"
    locator: Introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/bisect.html
    title: "bisect: Array bisection algorithm, Python 3.15"
    locator: Introduction; bisect_left(); insort_left()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/heapq.html
    title: "heapq: Heap queue algorithm, Python 3.15"
    locator: Introduction; heapify(); heappop(); nsmallest(); max-heap functions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Array_types
    title: The Go Programming Language Specification
    locator: Array types
    accessed: 2026-11-16
---

## Synopsis

```python run id=tour
from array import array
import bisect
from heapq import heapify, heappop, heappush

a = array("H", [4000, 10, 700, 22222])
print(sum(a), a[1:3])

scores = [(100, "perl"), (200, "tcl"), (400, "lua"), (500, "python")]
bisect.insort(scores, (300, "ruby"))
print(scores)

data = [1, 3, 5, 7, 9, 2, 4, 6, 8, 0]
heapify(data)
heappush(data, -5)
print([heappop(data) for i in range(3)])
```

```output for=tour
26932 array('H', [10, 700])
[(100, 'perl'), (200, 'tcl'), (300, 'ruby'), (400, 'lua'), (500, 'python')]
[-5, 0, 1]
```

These are the examples from the tutorial. Each module works with a list or with a list-like object.

## Semantics

**r1.** "Many data structure needs can be met with the built-in list type. However, sometimes there is a need for alternative implementations with different performance trade-offs."[^1]

**r2.** The array module "defines an object type which can compactly represent an array of basic values".[^2] "Arrays are mutable sequence types and behave very much like lists, except that the type of objects stored in them is constrained. The type is specified at object creation time by using a type code."[^2] Example: [`array-type`](#array-type).

**r3.** The bisect module "provides support for maintaining a list in sorted order without having to sort the list after each insertion."[^3] bisect_left() locates "the insertion point for x in a to maintain sorted order."[^3] "If x is already present in a, the insertion point will be before (to the left of) any existing entries."[^3] Example: [`grades`](#grades).

**r4.** "Min-heaps are binary trees for which every parent node has a value less than or equal to any of its children."[^4] "The interesting property of a min-heap is that its smallest element is always the root, heap[0]."[^4]

**r5.** heapify() transforms a list "into a min-heap, in-place, in linear time."[^4] heappop() pops "the smallest item from the heap, maintaining the min-heap invariant. If the heap is empty, IndexError is raised."[^4] Example: [`tasks`](#tasks).

**r6.** nsmallest() returns "a list with the n smallest elements from the dataset defined by iterable."[^4] "The API for max-heaps is named using a _max suffix."[^4] Example: [`max-heap`](#max-heap).

**r7.** The bisect functions "only call the __lt__() method and will return an insertion point between values in an array."[^3] They do not check that the list is sorted. Example: [`unsorted`](#unsorted).

## Examples

```python run id=grades
import bisect

def grade(score, breakpoints=[60, 70, 80, 90], grades="FDCBA"):
    i = bisect.bisect(breakpoints, score)
    return grades[i]

print([grade(s) for s in [33, 99, 77, 70, 89, 90, 100]])
print(bisect.bisect_left([1, 2, 2, 3], 2), bisect.bisect_right([1, 2, 2, 3], 2))
```

```output for=grades
['F', 'A', 'C', 'C', 'B', 'A', 'A']
1 3
```

`bisect_left` gives the position before equal items, and `bisect_right` gives the position after them (rule r3).

```python run id=tasks
import heapq

tasks = []
heapq.heappush(tasks, (3, "write tests"))
heapq.heappush(tasks, (1, "fix bug"))
heapq.heappush(tasks, (2, "review"))
print(tasks[0])
while tasks:
    priority, name = heapq.heappop(tasks)
    print(priority, name)
print(heapq.nsmallest(2, [5, 1, 4, 2]), heapq.nlargest(1, [5, 1, 4, 2]))
```

```output for=tasks
(1, 'fix bug')
1 fix bug
2 review
3 write tests
[1, 2] [5]
```

The item with the lowest priority number comes out first (rule r4).

```python run id=max-heap since=3.15
import heapq

data = [3, 9, 1, 7]
heapq.heapify_max(data)
heapq.heappush_max(data, 5)
print(data[0], [heapq.heappop_max(data) for _ in range(3)])
```

```output for=max-heap
9 [9, 7, 5]
```

The `_max` functions keep the largest item at index 0 (rule r6).

## Pitfalls

An array accepts only values that fit its type code:

```python fails id=array-type expect=exception:OverflowError
from array import array
a = array("H", [1, 2])
a.append(-1)
```

Type code `"H"` stores unsigned values, so -1 does not fit (rule r2).

bisect on a list that is not sorted gives a wrong position:

```python wrong id=unsorted
import bisect
items = [5, 1, 4]
bisect.insort(items, 3)
print(items)
```

```output for=unsorted
[5, 1, 3, 4]
```

Sort the list first (rule r7).

## Comparison

A Python array is close to a Go array or slice, because both hold one element type: "An array is a numbered sequence of elements of a single type, called the element type."[^5] A Go array also has a fixed length. A Python array can grow with `append` and `extend`, like a list (rule r2).
