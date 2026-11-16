---
tag: JXTPVH
kind: feature
owner: python
slug: else-clauses-on-loops
name: Else Clauses on Loops
aliases: [for else, while else, loop else clause, else after a for loop, nobreak clause]
area: languages/python/control-flow
level: core
summary: >-
  A for or while loop can have an else clause. The else clause runs when the loop ends without a break. A break, a return or an exception skips it. It is useful for a search that finds nothing.
grammar: "for_stmt: 'for' target_list 'in' starred_expression_list ':' suite ['else' ':' suite]; while_stmt: 'while' assignment_expression ':' suite ['else' ':' suite] (Language Reference, 8.2 and 8.3)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-for-statement", locator: "8.2. The while statement; 8.3. The for statement"}
see_also: [python/break-and-continue-statements, python/for-statements, python/while-statement, python/exiting-nested-loops]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#else-clauses-on-loops
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.5. else Clauses on Loops
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-for-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.2. The while statement; 8.3. The for statement
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Break_statements
    title: The Go Programming Language Specification
    locator: For statements; Break statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=primes
for n in range(2, 10):
    for x in range(2, n):
        if n % x == 0:
            print(n, "equals", x, "*", n // x)
            break
    else:
        print(n, "is a prime number")
```

```output for=primes
2 is a prime number
3 is a prime number
4 equals 2 * 2
5 is a prime number
6 equals 2 * 3
7 is a prime number
8 equals 2 * 4
9 equals 3 * 3
```

This is the example of the tutorial. The `else` belongs to the inner `for` loop, not to the `if`. It runs when the inner loop finds no factor.

## Semantics

**r1.** "If the loop finishes without executing the break, the else clause executes."[^1] Example: [`primes`](#primes).

**r2.** "In a for loop, the else clause is executed after the loop finishes its final iteration, that is, if no break occurred. In a while loop, it’s executed after the loop’s condition becomes false."[^1]

**r3.** "In either kind of loop, the else clause is not executed if the loop was terminated by a break. Of course, other ways of ending the loop early, such as a return or a raised exception, will also skip execution of the else clause."[^1] Example: [`return-skips`](#return-skips).

**r4.** The else clause also runs when the loop body never runs. For a for loop, the reference says: "When the iterator is exhausted, the suite in the else clause, if present, is executed".[^2] An empty iterable is exhausted at once. Example: [`empty`](#empty).

**r5.** "A continue statement executed in the first suite skips the rest of the suite and continues with the next item, or with the else clause if there is no next item."[^2] So continue does not skip the else clause.

**r6.** The tutorial compares it with try: "a try statement’s else clause runs when no exception occurs, and a loop’s else clause runs when no break occurs."[^1]

## Examples

```python run id=search
def find(items, target):
    for i, item in enumerate(items):
        if item == target:
            print("found at", i)
            break
    else:
        print("not found")

find(["a", "b", "c"], "b")
find(["a", "b", "c"], "z")
```

```output for=search
found at 1
not found
```

```python run id=return-skips
def first_negative(numbers):
    for n in numbers:
        if n < 0:
            return n
    else:
        print("else runs")
    return None

print(first_negative([3, -1, 2]))
print(first_negative([3, 1]))
```

```output for=return-skips
-1
else runs
None
```

```python run id=empty
for item in []:
    print("never")
else:
    print("else runs for an empty list")
```

```output for=empty
else runs for an empty list
```

## Pitfalls

The else clause does not test whether the loop body ran. It runs after each loop that ends without a break, also after a loop that ran many times:

```python wrong id=not-if-empty
names = ["ann", "bob"]
for name in names:
    print("hello", name)
else:
    print("the list is empty")
```

```output for=not-if-empty
hello ann
hello bob
the list is empty
```

To test for an empty list, use `if not names:` before the loop.

Indentation decides where an `else` belongs. An `else` at the level of the `for` keyword belongs to the loop. An `else` at the level of an `if` in the body belongs to the `if`.[^1]

## Comparison

Go has no else clause on loops. A Go for statement can stop early with break, which "terminates execution of the innermost" for, switch or select statement.[^3] To know whether the loop ended early, a Go program sets a variable before the break and tests it after the loop. The Python else clause does this test for you (rule r1).
