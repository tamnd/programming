---
tag: 4KE99F
kind: feature
owner: python
slug: the-cost-model-of-cpython
name: The cost model of CPython
aliases: [time complexity, Big O, list complexity, dict complexity, amortized append, sys.getsizeof, LOAD_FAST, LOAD_GLOBAL, LOAD_ATTR, local and global names]
area: languages/python/performance
level: advanced
summary: >-
  A list is an array of references, and a dict and a set are hash tables. Indexing and append are O(1), but insert at the front and x in list are O(n). Locals are read by index, globals by name.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/builtins/time-complexity.html", locator: "Time complexity of operations on built-in types"}
see_also: [python/performance-measurement, python/slots]
sources:
  - url: https://docs.python.org/3/builtins/time-complexity.html
    title: Time complexity of operations on built-in types
    locator: introduction; list; dict, frozendict; set, frozenset; Notes
    accessed: 2026-11-16
  - url: https://docs.python.org/3/faq/design.html#how-are-lists-implemented-in-cpython
    title: Design and History FAQ
    locator: How are lists implemented in CPython?; How are dictionaries implemented in CPython?
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/dis.html
    title: dis, Disassembler for Python bytecode
    locator: LOAD_FAST; LOAD_GLOBAL; LOAD_ATTR
    accessed: 2026-11-16
  - url: https://docs.python.org/3/reference/executionmodel.html
    title: Execution model
    locator: 4.2.1. Binding of names
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/sys.html#sys.getsizeof
    title: sys, System-specific parameters and functions
    locator: sys.getsizeof
    accessed: 2026-11-16
  - url: https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up
    title: Programming FAQ
    locator: My program is too slow. How do I speed it up?
    accessed: 2026-11-16
  - url: https://pkg.go.dev/builtin
    title: Package builtin
    locator: func append; func cap
    accessed: 2026-11-16
---

## Synopsis

```python run id=search-cost
class Key:
    compares = 0

    def __init__(self, n):
        self.n = n

    def __eq__(self, other):
        Key.compares += 1
        return self.n == other.n

    def __hash__(self):
        return hash(self.n)

keys = [Key(i) for i in range(1000)]
as_list, as_set = list(keys), set(keys)

Key.compares = 0
print(Key(-1) in as_list, Key.compares)
Key.compares = 0
print(Key(-1) in as_set, Key.compares)
```

```output for=search-cost
False 1000
False 0
```

The cost of an operation in CPython comes from the data structure under it. A list is an array, so `x in as_list` compares `x` with each item: 1000 comparisons for 1000 items. A set is a hash table, so `x in as_set` computes one hash and finds no item to compare. The page "Time complexity of operations on built-in types" in the documentation gives the cost of each operation.

## Semantics

**r1.** "This page documents the time complexity of various operations on built-in types in CPython. Other Python implementations may have different performance characteristics. Additionally, the listed costs assume exact built-in types, as instances of subclasses may have different costs."[^1]

**r2.** "CPython’s lists are really variable-length arrays, not Lisp-style linked lists."[^2] "This makes indexing a list a[i] an operation whose cost is independent of the size of the list or the value of the index."[^2]

**r3.** For a list, append is O(1), get item and set item are O(1), and `x in l` is O(n). Insert, pop and delete at index k are O(n - k).[^1] "The largest costs come from growing beyond the current allocation size (because everything must move), or from inserting or deleting somewhere near the beginning (because everything after that must move). If you need to add or remove at both ends, consider using a collections.deque instead."[^1] Examples: [`search-cost`](#search-cost) and [`front`](#front).

**r4.** The O(1) of append is amortized: "An individual operation may occasionally be O(n) when the underlying storage is resized, but this cost is spread over many operations, depending on the history of the container."[^1] "When items are appended or inserted, the array of references is resized. Some cleverness is applied to improve the performance of appending items repeatedly; when the array must be grown, some extra space is allocated so the next few times don’t require an actual resize."[^2] Example: [`growth`](#growth).

**r5.** "CPython’s dictionaries are implemented as resizable hash tables."[^2] For a dict, `key in d`, get item, set item and delete item are O(1).[^1] "The times listed for dict objects are average-case times, as they assume the hash function for the objects is sufficiently robust to make collisions uncommon."[^1] "In the worst case, when every key hashes to the same value, each of the O(1) operations below instead takes O(n) time."[^1] A set has the same costs and the same caveats.[^1] Example: [`search-cost`](#search-cost).

**r6.** "If a name is bound in a block, it is a local variable of that block, unless declared as nonlocal or global."[^4] The compiler reads a local name with `LOAD_FAST`, which "Pushes a reference to the local co_varnames[var_num] onto the stack."[^3] It reads a global or a builtin name with `LOAD_GLOBAL`, which "Loads the global named co_names[namei>>1] onto the stack."[^3] `LOAD_ATTR` reads an attribute: it "replaces STACK[-1] with getattr(STACK[-1], co_names[namei>>1])".[^3] Example: [`names`](#names).

**r7.** `sys.getsizeof` returns "the size of an object in bytes".[^5] "Only the memory consumption directly attributed to the object is accounted for, not the memory consumption of objects it refers to."[^5] Example: [`sizes`](#sizes).

**r8.** "Abstractions tend to create indirections and force the interpreter to work more. If the levels of indirection outweigh the amount of useful work done, your program will be slower."[^6]

## Examples

A list moves its items when you insert at the front. A `collections.deque` adds at both ends at O(1) (rule r3). The example measures the effect with the size of the work, not with a clock: it counts the items that move.

```python run id=front
import collections

def moved_by_inserts(n):
    items, moved = [], 0
    for i in range(n):
        moved += len(items)
        items.insert(0, i)
    return moved

print(moved_by_inserts(10), moved_by_inserts(100), moved_by_inserts(1000))

d = collections.deque()
for i in range(1000):
    d.appendleft(i)
print(d[0], d[-1], len(d))
```

```output for=front
45 4950 499500
999 0 1000
```

With n inserts at index 0, the items that move are 0 + 1 + ... + (n - 1). Ten times more inserts move about a hundred times more items, so the loop is O(n²).

The size of a list grows in steps. Most appends do not resize it (rule r4):

```python run id=growth
import sys

items, resizes, size = [], 0, sys.getsizeof([])
for i in range(1000):
    items.append(i)
    if sys.getsizeof(items) != size:
        resizes += 1
        size = sys.getsizeof(items)
print(resizes < 50, resizes > 0)
```

```output for=growth
True True
```

The bytecode reads a local name, a global name and a builtin name in different ways (rule r6):

```python run id=names
import dis

TOTAL = 10

def add(items, x):
    items.append(x)
    return len(items) + TOTAL

kinds = {'LOAD_FAST': 'local, by index', 'LOAD_GLOBAL': 'global or builtin, by name',
         'LOAD_ATTR': 'attribute, by name'}
for ins in dis.get_instructions(add):
    for prefix, kind in kinds.items():
        if ins.opname.startswith(prefix):
            print(f'{ins.argval:7} {kind}')
```

```output for=names
items   local, by index
append  attribute, by name
x       local, by index
len     global or builtin, by name
items   local, by index
TOTAL   global or builtin, by name
```

`sys.getsizeof` counts only the object, not the objects that it refers to (rule r7):

```python run id=sizes
import sys

text = 'x' * 10_000
box = [text]
print(sys.getsizeof(text) > 10_000)
print(sys.getsizeof(box) < 100)
print(sys.getsizeof(box) + sys.getsizeof(text) > 10_000)
```

```output for=sizes
True
True
True
```

## Pitfalls

`x in a_list` in a loop is O(n) for each test. When you test many values, make a set once and test the set (rule r5).

`items.insert(0, x)` and `items.pop(0)` move each other item (rule r3). For a queue, use `collections.deque`.

The O(1) of a dict and a set is an average. Keys that all have the same hash make each operation O(n) (rule r5).

`sys.getsizeof` of a container does not include the items (rule r7). Add the sizes of the items when you need the full size.

The costs are for CPython and for the exact built-in types (rule r1). A subclass, or another implementation such as PyPy, can have different costs.

## Comparison

In Go, "The append built-in function appends elements to the end of a slice. If it has sufficient capacity, the destination is resliced to accommodate the new elements. If it does not, a new underlying array will be allocated."[^7] Go shows the capacity with the builtin `cap`, and `make` can set it.[^7] A Python list has the same model, an array with extra space (rule r4), but Python does not show the capacity. `sys.getsizeof` gives only the size in bytes (example [`growth`](#growth)).

## References

[^1]: [Time complexity of operations on built-in types](https://docs.python.org/3/builtins/time-complexity.html)
[^2]: [Design and History FAQ](https://docs.python.org/3/faq/design.html#how-are-lists-implemented-in-cpython)
[^3]: [dis, Disassembler for Python bytecode](https://docs.python.org/3/library/dis.html)
[^4]: [Execution model](https://docs.python.org/3/reference/executionmodel.html)
[^5]: [sys, System-specific parameters and functions](https://docs.python.org/3/library/sys.html#sys.getsizeof)
[^6]: [Programming FAQ](https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up)
[^7]: [Package builtin](https://pkg.go.dev/builtin)
