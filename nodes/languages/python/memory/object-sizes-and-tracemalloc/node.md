---
tag: T9ENW8
kind: feature
owner: python
slug: object-sizes-and-tracemalloc
name: Object sizes and tracemalloc
aliases: [sys.getsizeof, __sizeof__, tracemalloc, memory profiling, find a memory leak, PYTHONTRACEMALLOC, memory usage of an object]
area: languages/python/memory
level: advanced
summary: >-
  sys.getsizeof gives the bytes of one object and not of the objects it refers to. tracemalloc records where Python allocates memory, and two snapshots show which lines make memory grow.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/tracemalloc.html", locator: "tracemalloc, Trace memory allocations"}
see_also: [python/memory-management-in-cpython, python/reference-counting-in-cpython, python/weak-references]
sources:
- url: https://docs.python.org/3.15/library/sys.html#sys.getsizeof
  title: sys, System-specific parameters and functions
  locator: sys.getsizeof
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/library/tracemalloc.html
  title: tracemalloc, Trace memory allocations
  locator: the introduction; take_snapshot; get_traced_memory; start; Snapshot.compare_to; Statistic; StatisticDiff
  accessed: 2026-11-16
- url: https://docs.python.org/3.15/reference/datamodel.html#slots
  title: The Python Language Reference, 3. Data model
  locator: 3.3.2.4. __slots__
  accessed: 2026-11-16
- url: https://pkg.go.dev/unsafe#Sizeof
  title: Package unsafe, The Go Programming Language
  locator: func Sizeof
  accessed: 2026-11-16
- url: https://pkg.go.dev/runtime#ReadMemStats
  title: Package runtime, The Go Programming Language
  locator: func ReadMemStats
  accessed: 2026-11-16
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Runtime.html
  title: Class Runtime, Java SE 25 API
  locator: totalMemory(); freeMemory()
  accessed: 2026-11-16
---

## Synopsis

```text
import sys
sys.getsizeof(obj)            # bytes of obj itself
sys.getsizeof(obj, default)   # default if obj gives no size

import tracemalloc
tracemalloc.start(nframe=1)
snapshot = tracemalloc.take_snapshot()
snapshot.statistics("lineno")             # list of Statistic, largest first
after.compare_to(before, "lineno")        # list of StatisticDiff
tracemalloc.get_traced_memory()           # (current, peak)
tracemalloc.get_object_traceback(obj)
tracemalloc.stop()
```

```text
python -X tracemalloc=25 app.py
PYTHONTRACEMALLOC=25 python app.py
```

## Semantics

**r1.** `sys.getsizeof()` returns "the size of an object in bytes".[^1] It is shallow: "Only the memory consumption directly attributed to the object is accounted for, not the memory consumption of objects it refers to."[^1] A list of large strings has the size of a list of small strings. Example: [`shallow`](#shallow).

**r2.** "getsizeof() calls the object’s __sizeof__ method and adds an additional garbage collector overhead if the object is managed by the garbage collector."[^1] "All built-in objects will return correct results, but this does not have to hold true for third-party extensions as it is implementation specific."[^1] For a deep size, the sys documentation points to a "recursive sizeof recipe".[^1] Example: [`deep`](#deep).

**r3.** "The tracemalloc module is a debug tool to trace memory blocks allocated by Python."[^2] It gives the traceback where each block was allocated, statistics "per filename and per line number", and the differences between two snapshots "to detect memory leaks".[^2]

**r4.** Tracing starts with `tracemalloc.start()`, or at startup "by setting the PYTHONTRACEMALLOC environment variable to 1, or by using -X tracemalloc command line option."[^2] "By default, a trace of an allocated memory block only stores the most recent frame (1 frame)."[^2] More frames cost more: "Storing more frames increases the memory and CPU overhead of the tracemalloc module."[^2]

**r5.** A snapshot holds the traces at one moment. "The snapshot does not include memory blocks allocated before the tracemalloc module started to trace memory allocations."[^2] `Snapshot.statistics()` returns `Statistic` objects with a `count`, a `size` and a `traceback`.[^2] Example: [`trace`](#trace).

**r6.** `after.compare_to(before, key_type)` "Compute the differences with an old snapshot."[^2] The result is sorted "from the biggest to the smallest", first by the "absolute value of StatisticDiff.size_diff".[^2] So the first item names the line whose memory grew most. Example: [`leak`](#leak).

**r7.** `tracemalloc.get_traced_memory()` gives "the current size and peak size of memory blocks traced by the tracemalloc module as a tuple: (current: int, peak: int)."[^2] `reset_peak()` sets the peak "to the current size".[^2]

## Examples

`getsizeof` counts the list and not the strings in it (rule r1).

```python run id=shallow
import sys

small = ["x"]
large = ["x" * 10_000]
print(sys.getsizeof(small) == sys.getsizeof(large))
print(sys.getsizeof(large[0]) > 10_000)
```

```output for=shallow
True
True
```

A deep size follows the references and counts each object once (rule r2). This version knows only dicts, lists, tuples and sets.

```python run id=deep
import sys


def total_size(obj, seen=None):
    seen = set() if seen is None else seen
    if id(obj) in seen:
        return 0
    seen.add(id(obj))
    size = sys.getsizeof(obj)
    if isinstance(obj, dict):
        size += sum(total_size(k, seen) + total_size(v, seen) for k, v in obj.items())
    elif isinstance(obj, (list, tuple, set, frozenset)):
        size += sum(total_size(item, seen) for item in obj)
    return size


data = {"name": "x" * 10_000, "tags": ["a", "b"]}
print(sys.getsizeof(data) < 1_000)
print(total_size(data) > 10_000)
```

```output for=deep
True
True
```

A class with `__slots__` has no `__dict__`, so each instance is smaller. The data model says that "The space saved over using __dict__ can be significant."[^3]

```python run id=slots
import sys


class Plain:
    def __init__(self):
        self.x = 1
        self.y = 2


class Slotted:
    __slots__ = ("x", "y")

    def __init__(self):
        self.x = 1
        self.y = 2


p, s = Plain(), Slotted()
print(hasattr(p, "__dict__"), hasattr(s, "__dict__"))
print(sys.getsizeof(s) < sys.getsizeof(p) + sys.getsizeof(p.__dict__))
```

```output for=slots
True False
True
```

The line that allocates the most (rule r5). The first statistic is line 4, the list comprehension, with at least one block for each string.

```python run id=trace
import tracemalloc

tracemalloc.start()
rows = [str(i) * 10 for i in range(10_000)]
snapshot = tracemalloc.take_snapshot()
top = snapshot.statistics("lineno")[0]
print(top.traceback[0].lineno, top.size > 500_000, top.count >= 10_000)
current, peak = tracemalloc.get_traced_memory()
print(peak >= current > 500_000)
tracemalloc.stop()
```

```output for=trace
4 True True
True
```

A leak found with two snapshots (rule r6). The function keeps each request in a global list. The first difference names line 7, the `append` call.

```python run id=leak
import tracemalloc

leak = []


def handle(request):
    leak.append(request * 100)


tracemalloc.start()
before = tracemalloc.take_snapshot()
for i in range(1_000):
    handle(str(i))
after = tracemalloc.take_snapshot()
diff = after.compare_to(before, "lineno")[0]
print(diff.traceback[0].lineno, diff.size_diff > 100_000, diff.count_diff >= 1_000)
tracemalloc.stop()
```

```output for=leak
7 True True
```

The examples give the same output on CPython 3.12.3, 3.13.14 and 3.14.6. They test ranges and not exact byte counts, because the counts change between versions (see Implementation notes).

## Pitfalls

### `getsizeof` as the memory of a structure

A dict of lists of strings is many objects, and `getsizeof` counts one (rule r1). Use a recursive function such as [`total_size`](#deep), or compare `tracemalloc` snapshots taken before and after the structure is built.

### `tracemalloc` started late

Blocks allocated before `start()` are not in any snapshot (rule r5). To see imports and startup, start tracing with `-X tracemalloc` or `PYTHONTRACEMALLOC` (rule r4).

### Exact sizes in tests

A test that asserts `sys.getsizeof(()) == 40` passes on 3.12 and fails on 3.14 (see the table in Implementation notes). Compare two sizes, or test a range.

## Comparison

**Go** has `unsafe.Sizeof`, which is shallow like `sys.getsizeof`: "The size does not include any memory possibly referenced by x. For instance, if x is a slice, Sizeof returns the size of the slice descriptor, not the size of the memory referenced by the slice".[^4] For the whole program, "ReadMemStats populates m with memory allocator statistics."[^5]

**Java** has no standard function for the size of one object. `Runtime.totalMemory()` "Returns the total amount of memory in the Java virtual machine", and the API adds: "Note that the amount of memory required to hold an object of any given type may be implementation-dependent."[^6]

## Implementation notes

The sizes depend on the version and the platform. These values are from `sys.getsizeof` on x86_64 Linux:

| Value | 3.12.3 | 3.13.14 | 3.14.6 |
|---|---|---|---|
| `0` | 28 | 28 | 28 |
| `2**100` | 40 | 40 | 40 |
| `1.5` | 24 | 24 | 24 |
| `""` | 41 | 41 | 41 |
| `"a"` | 42 | 42 | 42 |
| `"a" * 100` | 141 | 141 | 141 |
| `b""` | 33 | 33 | 33 |
| `()` | 40 | 40 | 48 |
| `[]` | 56 | 56 | 56 |
| `{}` | 64 | 64 | 64 |
| `set()` | 216 | 216 | 216 |
| `None` | 16 | 16 | 16 |

`tracemalloc` sees the blocks that Python allocates (rule r3). The `tracemalloc` module uses memory of its own, and `get_tracemalloc_memory()` returns "the memory usage in bytes of the tracemalloc module used to store traces of memory blocks."[^2]

## References
