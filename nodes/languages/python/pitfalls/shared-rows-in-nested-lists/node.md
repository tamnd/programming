---
tag: PPBJBF
kind: feature
owner: python
slug: shared-rows-in-nested-lists
name: Shared rows in nested lists
aliases: [list of lists, "[[0] * 3] * 3", multidimensional list, two-dimensional list, aliasing by repetition, sequence repetition]
area: languages/python/pitfalls
level: intro
summary: >-
  The expression [[0] * 3] * 3 makes a list of three references to one row, so a change to one row shows in all rows. Make each row with a comprehension, as in [[0] * 3 for _ in range(3)].
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/stdtypes.html#common-sequence-operations", locator: "Common Sequence Operations, note 2"}
proposals: []
see_also: [python/lists, python/nested-list-comprehensions, python/copying-collections, python/a-word-about-names-and-objects, python/object-identity-and-interning, python/sequence-types]
sources:
  - url: https://docs.python.org/3.15/library/stdtypes.html#common-sequence-operations
    title: Built-in Types, Python 3.15 documentation
    locator: Common Sequence Operations, note 2; Mutable Sequence Types, s *= n
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/faq/programming.html#how-do-i-create-a-multidimensional-list
    title: Programming FAQ, Python 3.15 documentation
    locator: How do I create a multidimensional list?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/copy.html
    title: copy, Shallow and deep copy operations, Python 3.15 documentation
    locator: copy.copy(); copy.deepcopy()
    accessed: 2026-11-18
---

## Synopsis

```python run id=shared-rows
grid = [[0] * 3] * 3
grid[0][0] = 5
print(grid)

grid = [[0] * 3 for _ in range(3)]
grid[0][0] = 5
print(grid)
```

```output for=shared-rows
[[5, 0, 0], [5, 0, 0], [5, 0, 0]]
[[5, 0, 0], [0, 0, 0], [0, 0, 0]]
```

The first grid has one row three times. The second grid has three rows.

## Semantics

**r1.** `s * n` repeats references, not objects. "Note that items in the sequence s are not copied; they are referenced multiple times. This often haunts new Python programmers".[^1] The documentation gives the example `[[]] * 3`, where "all three elements of [[]] * 3 are references to this single empty list. Modifying any of the elements of lists modifies this single list."[^1]

**r2.** The FAQ shows the same trap for a two-dimensional list: "replicating a list with * doesn’t create copies, it only creates references to the existing objects. The *3 creates a list containing 3 references to the same list of length two. Changes to one row will show in all rows, which is almost certainly not what you want."[^2] Example: [`shared-rows`](#shared-rows).

**r3.** The inner `[0] * 3` is safe, because its items are integers. An assignment `row[0] = 5` puts a new object in the row. It does not change the integer 0, which is immutable. The trap is only for mutable items such as lists, dicts, sets and class instances.

**r4.** The fix makes a new row for each index. "You can also use a list comprehension: w, h = 2, 3 A = [[None] * w for i in range(h)]".[^2] The expression `[None] * w` runs once for each pass, so each pass makes a new list. Example: [`shared-rows`](#shared-rows).

**r5.** The augmented form `s *= n` has the same rule: "Items in the sequence are not copied; they are referenced multiple times, as explained for s * n under Common Sequence Operations."[^1]

**r6.** A shallow copy also shares the rows. `list(grid)`, `grid[:]` and `copy.copy(grid)` make a new outer list with the same rows. "A deep copy constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original."[^3] Example: [`shallow-copy`](#shallow-copy).

## Examples

```python run id=check-identity
grid = [[0] * 3] * 3
print([id(row) == id(grid[0]) for row in grid])

grid = [[0] * 3 for _ in range(3)]
print([row is grid[0] for row in grid])
```

```output for=check-identity
[True, True, True]
[True, False, False]
```

The `is` test shows the shared rows. In the second grid, only the first row is the first row.

```python run id=dict-of-lists
from collections import defaultdict

groups = dict.fromkeys(["a", "b"], [])
groups["a"].append(1)
print(groups)

groups = defaultdict(list)
groups["a"].append(1)
print(dict(groups))
```

```output for=dict-of-lists
{'a': [1], 'b': [1]}
{'a': [1]}
```

`dict.fromkeys` has the same trap: all the keys get the one list. `defaultdict(list)` calls `list` for each new key.

## Versions

The behavior is the same in all Python versions. It follows from rule r1.

## Pitfalls

```python wrong id=shallow-copy
import copy

board = [["."] * 2 for _ in range(2)]
backup = board[:]
board[0][0] = "X"
print(backup)

board = [["."] * 2 for _ in range(2)]
backup = copy.deepcopy(board)
board[0][0] = "X"
print(backup)
```

```output for=shallow-copy
[['X', '.'], ['.', '.']]
[['.', '.'], ['.', '.']]
```

The slice copied the outer list, but the backup still holds the same rows, so the change shows in it (rule r6). `copy.deepcopy` copies the rows too.

```python wrong id=list-of-objects
class Cell:
    def __init__(self):
        self.value = 0


cells = [Cell()] * 3
cells[0].value = 9
print([c.value for c in cells])
```

```output for=list-of-objects
[9, 9, 9]
```

The trap is not only for lists in lists. `[Cell()] * 3` calls `Cell` once (rule r1). Write `[Cell() for _ in range(3)]`.

## Comparison

In languages where a two-dimensional array is one block of values, each row is a separate part of the block. In Python, a list holds references, so two rows can be the same object. For large numeric grids, the FAQ points to a matrix type: "NumPy is the best known."[^2] For names and objects, see [[python/a-word-about-names-and-objects]]. For copies, see [[python/copying-collections]].
