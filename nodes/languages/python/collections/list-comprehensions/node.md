---
tag: E5S6WN
kind: feature
owner: python
slug: list-comprehensions
name: List comprehensions
aliases: [list comprehension, listcomp, comprehension with if, map and filter in Python, build a list in one line, did you forget parentheses around the comprehension target]
area: languages/python/collections
level: intro
summary: >-
  A list comprehension is an expression and a for clause in square brackets, with more for or if clauses after them. It makes a new list from the values of the expression.
grammar: "listcomp: '[' comprehension ']'"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#comprehensions", locator: "6.2.6. Comprehensions"}
see_also: [python/nested-list-comprehensions, python/set-and-dict-comprehensions, python/comprehension-scope, python/generator-expressions, python/lists]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#list-comprehensions
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1.3. List Comprehensions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#comprehensions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6. Comprehensions; 6.2.6.1. Filtering in comprehensions; 6.2.6.2. Complex comprehensions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Appending_and_copying_slices
    title: The Go Programming Language Specification
    locator: Appending to and copying slices
    accessed: 2026-11-16
---

## Synopsis

```python run id=squares
squares = [x ** 2 for x in range(10)]
print(squares)

vec = [-4, -2, 0, 2, 4]
print([x * 2 for x in vec])
print([x for x in vec if x >= 0])
print([abs(x) for x in vec])
```

```output for=squares
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
[-8, -4, 0, 4, 8]
[0, 2, 4]
[4, 2, 0, 2, 4]
```

Each comprehension makes a new list. The `if` clause keeps only some items.

## Semantics

**r1.** "A list comprehension consists of brackets containing an expression followed by a for clause, then zero or more for or if clauses. The result will be a new list resulting from evaluating the expression in the context of the for and if clauses which follow it."[^1]

**r2.** "The for clause has the same syntax as the header of a for statement, without a trailing colon."[^2] "For each element of this iterable, the element is bound to the for clause’s target as in a for statement, then the expression before for is evaluated with the target in scope and the result is added to the new list."[^2]

**r3.** "At run time, the expression after if is evaluated before each element is added to the resulting container, and if it is false, the element is skipped."[^2]

**r4.** "The elements of the new container are those that would be produced by considering each of the for or if clauses a block, nesting from left to right, and evaluating the expression to produce an element (or dictionary entry) each time the innermost block is reached."[^2] Example: [`two-fors`](#two-fors).

**r5.** "If the expression is a tuple (e.g. the (x, y) in the previous example), it must be parenthesized."[^1] Example: [`tuple-target`](#tuple-target).

**r6.** "Aside from the iterable expression in the leftmost for clause, the comprehension is executed in a separate implicitly nested scope. This ensures that names assigned to in the target list don’t “leak” into the enclosing scope."[^2] Example: [`no-leak`](#no-leak).

## Examples

```python run id=two-fors
pairs = [(x, y) for x in [1, 2, 3] for y in [3, 1, 4] if x != y]
print(pairs)

combs = []
for x in [1, 2, 3]:
    for y in [3, 1, 4]:
        if x != y:
            combs.append((x, y))
print(combs == pairs)
```

```output for=two-fors
[(1, 3), (1, 4), (2, 3), (2, 1), (2, 4), (3, 1), (3, 4)]
True
```

The clauses nest from left to right in the same order as the loops (rule r4).

```python run id=methods
fresh = ["  banana", "  loganberry ", "passion fruit  "]
print([f.strip() for f in fresh])
print([(x, x ** 2) for x in range(4)])
print([str(round(3.14159, i)) for i in range(1, 4)])
```

```output for=methods
['banana', 'loganberry', 'passion fruit']
[(0, 0), (1, 1), (2, 4), (3, 9)]
['3.1', '3.14', '3.142']
```

```python run id=no-leak
x = "old value"
squares = [x ** 2 for x in range(3)]
print(x, squares)

for x in range(3):
    pass
print(x)
```

```output for=no-leak
old value [0, 1, 4]
2
```

The comprehension has its own `x` (rule r6). A `for` statement binds `x` in the enclosing scope.

```python run id=condition-expression
nums = [3, -1, 4, -1, 5]
print([n if n > 0 else 0 for n in nums])
```

```output for=condition-expression
[3, 0, 4, 0, 5]
```

A conditional expression before `for` changes each item. An `if` clause after `for` drops items (rule r3).

## Pitfalls

A tuple expression needs parentheses:

```python fails id=tuple-target expect=exception:SyntaxError
pairs = [x, x ** 2 for x in range(6)]
```

Write `[(x, x ** 2) for x in range(6)]` (rule r5).

A comprehension that is used only for its side effects makes a list that the program throws away:

```python wrong id=side-effect
result = [print(n) for n in range(2)]
print(result)
```

```output for=side-effect
0
1
[None, None]
```

Use a `for` statement when the program does not need the list.

## Comparison

Go has no comprehensions. A Go program makes the new slice with a `for` loop and `append`: "The variadic function append appends zero or more values x to a slice s of type S and returns the resulting slice, also of type S."[^3] A Python list comprehension does the loop, the test and the append in one expression (rule r1).
