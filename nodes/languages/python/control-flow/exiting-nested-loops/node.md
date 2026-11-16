---
tag: B0J8TC
kind: feature
owner: python
slug: exiting-nested-loops
name: Exiting nested loops
aliases: [break out of nested loops, labeled break in Python, break two loops, break outer loop, PEP 3136]
area: languages/python/control-flow
level: core
summary: >-
  break ends only the innermost loop, and Python has no labeled break. To leave several loops, return from a function, use itertools.product to make one loop, or use the else clause of the inner loop.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-break-statement", locator: "7.9. The break statement"}
see_also: [python/break-and-continue-statements, python/else-clauses-on-loops, python/for-statements, python/looping-techniques]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-break-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.9. The break statement
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3136/
    title: PEP 3136 - Labeled break and continue
    locator: Status; Rejection Notice; Introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/itertools.html#itertools.product
    title: itertools - Functions creating iterators for efficient looping, Python 3.15 documentation
    locator: itertools.product()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#else-clauses-on-loops
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.5. else Clauses on Loops
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Break_statements
    title: The Go Programming Language Specification
    locator: Break statements; Labeled statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=function-return
def find(grid, target):
    for r, row in enumerate(grid):
        for c, value in enumerate(row):
            if value == target:
                return r, c
    return None

grid = [[1, 2, 3], [4, 5, 6]]
print(find(grid, 5), find(grid, 9))
```

```output for=function-return
(1, 1) None
```

`return` leaves both loops at once. This is the most direct way to stop a nested search.

## Semantics

**r1.** break "terminates the nearest enclosing loop".[^1] PEP 3136 says: "In Python currently, break and continue can apply only to the innermost enclosing loop."[^2] Example: [`inner-only`](#inner-only).

**r2.** PEP 3136 proposed labels for break and continue. Its notice says: "This PEP is rejected."[^2]

**r3.** A return statement in a function ends all the loops of the function. Example: [`function-return`](#function-return).

**r4.** `itertools.product` is "Roughly equivalent to nested for-loops in a generator expression. For example, product(A, B) returns the same as ((x,y) for x in A for y in B)."[^3] So two nested loops become one loop, and one break ends it. Example: [`product`](#product).

**r5.** "In either kind of loop, the else clause is not executed if the loop was terminated by a break."[^4] An inner loop with `else: continue` followed by `break` passes a break out to the outer loop. Example: [`else-continue`](#else-continue).

## Examples

```python run id=product
from itertools import product

for a, b in product(range(1, 10), repeat=2):
    if a * b == 12 and a < b:
        print(a, b)
        break
```

```output for=product
2 6
```

```python run id=else-continue
grid = [[1, 2], [3, 4], [5, 6]]
for row in grid:
    for value in row:
        if value == 4:
            print("found", value)
            break
    else:
        continue
    break
print("row", row)
```

```output for=else-continue
found 4
row [3, 4]
```

When the inner loop ends with no break, `continue` goes to the next row. When the inner loop breaks, Python skips the `else` clause and runs the second `break`, which ends the outer loop (rule r5).

```python run id=flag
found = None
for row in [[1, 2], [3, 4]]:
    for value in row:
        if value > 2:
            found = value
            break
    if found is not None:
        break
print(found)
```

```output for=flag
3
```

A flag variable works too, but it needs a second test in the outer loop.

## Pitfalls

A single break does not end the outer loop:

```python wrong id=inner-only
for row in [[1, 2], [3, 4]]:
    for value in row:
        if value == 1:
            break
        print(value)
    print("next row")
```

```output for=inner-only
next row
3
4
next row
```

The break ends only the inner loop for the first row. The outer loop goes on with the second row (rule r1).

## Comparison

Go has labels. A labeled statement "may be the target of a goto, break or continue statement."[^5] For break, "If there is a label, it must be that of an enclosing" for, switch or select statement, "and that is the one whose execution terminates."[^5] So `break OuterLoop` in Go ends the outer loop directly. Python has no labels (rule r2), so a Python program uses one of the techniques of this page.
