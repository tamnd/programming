---
tag: 44SVQP
kind: feature
owner: python
slug: nested-list-comprehensions
name: Nested list comprehensions
aliases: [nested comprehension, comprehension inside a comprehension, transpose a matrix, list of lists, flatten a list, 2D list]
area: languages/python/collections
level: core
summary: >-
  The expression of a list comprehension can be another list comprehension. The inner one runs once for each item of the outer for clause, so it can build a list of lists, such as a matrix transpose.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/datastructures.html#nested-list-comprehensions", locator: "5.1.4. Nested List Comprehensions"}
see_also: [python/list-comprehensions, python/comprehension-scope, python/sequence-types, python/unpacking-argument-lists]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#nested-list-comprehensions
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1.3. List Comprehensions; 5.1.4. Nested List Comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#complex-comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6.2. Complex comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#common-sequence-operations
    title: Built-in Types, Python 3.15 documentation
    locator: Common Sequence Operations, note 2
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Composite_literals
    title: The Go Programming Language Specification
    locator: Composite literals
    accessed: 2026-11-16
---

## Synopsis

```python run id=transpose
matrix = [
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12],
]
print([[row[i] for row in matrix] for i in range(4)])
print(list(zip(*matrix)))
```

```output for=transpose
[[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]]
[(1, 5, 9), (2, 6, 10), (3, 7, 11), (4, 8, 12)]
```

The outer comprehension runs over the column numbers. For each column, the inner comprehension collects that item of each row.

## Semantics

**r1.** "The initial expression in a list comprehension can be any arbitrary expression, including another list comprehension."[^1]

**r2.** "the inner list comprehension is evaluated in the context of the for that follows it".[^1] Example: [`as-loops`](#as-loops).

**r3.** A comprehension with two `for` clauses is different. Its clauses are "nesting from left to right", and it makes one flat list.[^2] Example: [`flatten`](#flatten).

**r4.** "In the real world, you should prefer built-in functions to complex flow statements. The zip() function would do a great job for this use case".[^1]

**r5.** For `s * n`, "items in the sequence s are not copied; they are referenced multiple times."[^3] So a nested comprehension is the safe way to make a grid of new lists. Example: [`grid-alias`](#grid-alias).

## Examples

```python run id=as-loops
matrix = [[1, 2, 3], [4, 5, 6]]
transposed = []
for i in range(3):
    transposed_row = []
    for row in matrix:
        transposed_row.append(row[i])
    transposed.append(transposed_row)
print(transposed)
print(transposed == [[row[i] for row in matrix] for i in range(3)])
```

```output for=as-loops
[[1, 4], [2, 5], [3, 6]]
True
```

The inner comprehension is the inner loop (rule r2).

```python run id=flatten
vec = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
print([num for elem in vec for num in elem])
print([[num * 10 for num in elem] for elem in vec])
```

```output for=flatten
[1, 2, 3, 4, 5, 6, 7, 8, 9]
[[10, 20, 30], [40, 50, 60], [70, 80, 90]]
```

Two `for` clauses give one flat list. A comprehension inside the expression gives a list of lists (rule r3).

```python run id=grid
grid = [[0] * 3 for _ in range(2)]
grid[0][1] = 5
print(grid)
```

```output for=grid
[[0, 5, 0], [0, 0, 0]]
```

Each row is a new list, so a change to one row does not change the other row.

## Pitfalls

Repetition of a list of lists repeats one inner list:

```python wrong id=grid-alias
grid = [[0] * 3] * 2
grid[0][1] = 5
print(grid)
```

```output for=grid-alias
[[0, 5, 0], [0, 5, 0]]
```

Both rows are the same list (rule r5). Use the nested comprehension of the `grid` example.

The order of the `for` clauses matters. In `[num for elem in vec for num in elem]`, the clause that defines `elem` must come first:

```python fails id=wrong-order expect=exception:NameError
vec = [[1, 2], [3]]
print([num for num in elem for elem in vec])
```

## Comparison

A Go program writes a 2D value as a composite literal or builds it with nested loops. The Go spec says: "Within a composite literal of array, slice, or map type T, elements or map keys that are themselves composite literals may elide the respective literal type if it is identical to the element or key type of T."[^4] So `[][]int{{1, 2}, {3, 4}}` is valid Go. A Python nested comprehension builds the inner lists from data, with no explicit loops (rule r1).
