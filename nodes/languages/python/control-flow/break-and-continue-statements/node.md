---
tag: S4YSAT
kind: feature
owner: python
slug: break-and-continue-statements
name: Break and continue Statements
aliases: [break statement, continue statement, exit a loop early, skip an iteration, break out of a for loop]
area: languages/python/control-flow
level: intro
summary: >-
  break ends the innermost for or while loop at once and skips the else clause of the loop. continue skips the rest of the loop body and starts the next pass. Both work only inside a loop.
grammar: "break_stmt: 'break'; continue_stmt: 'continue' (Language Reference, 7.9 and 7.10)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-break-statement", locator: "7.9. The break statement; 7.10. The continue statement"}
see_also: [python/for-statements, python/while-statement, python/else-clauses-on-loops, python/exiting-nested-loops]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#break-and-continue-statements
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.4. break and continue Statements
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-break-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.9. The break statement; 7.10. The continue statement
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Break_statements
    title: The Go Programming Language Specification
    locator: Break statements; Continue statements
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3136/
    title: PEP 3136 - Labeled break and continue
    locator: Status; Rejection Notice
    accessed: 2026-11-16
---

## Synopsis

```python run id=both
for n in range(1, 8):
    if n % 2 == 0:
        continue
    if n > 5:
        break
    print(n)
```

```output for=both
1
3
5
```

`continue` skips the even numbers. `break` ends the loop at `7`.

## Semantics

**r1.** "The break statement breaks out of the innermost enclosing for or while loop".[^1] "It terminates the nearest enclosing loop, skipping the optional else clause if the loop has one."[^2] Example: [`inner-only`](#inner-only).

**r2.** "The continue statement continues with the next iteration of the loop".[^1] Example: [`both`](#both).

**r3.** Each of the two statements "may only occur syntactically nested in a for or while loop, but not nested in a function or class definition within that loop."[^2] Example: [`break-in-function`](#break-in-function).

**r4.** "If a for loop is terminated by break, the loop control target keeps its current value."[^2] Example: [`keeps-value`](#keeps-value).

**r5.** "When break passes control out of a try statement with a finally clause, that finally clause is executed before really leaving the loop." The rule for continue is the same: the finally clause runs "before really starting the next loop cycle".[^2] Example: [`finally`](#finally).

## Examples

```python run id=inner-only
for n in range(2, 10):
    for x in range(2, n):
        if n % x == 0:
            print(f"{n} equals {x} * {n//x}")
            break
```

```output for=inner-only
4 equals 2 * 2
6 equals 2 * 3
8 equals 2 * 4
9 equals 3 * 3
```

This is the example of the tutorial. The `break` ends only the inner loop, and the outer loop goes on with the next `n` (rule r1).

```python run id=keeps-value
for i, name in enumerate(["ann", "bob", "cid"]):
    if name == "bob":
        break
print(i, name)
```

```output for=keeps-value
1 bob
```

```python run id=finally
for i in range(3):
    try:
        if i == 1:
            continue
        if i == 2:
            break
        print("body", i)
    finally:
        print("finally", i)
```

```output for=finally
body 0
finally 0
finally 1
finally 2
```

## Pitfalls

A `break` in a function that is defined in a loop body is not in a loop:

```python fails id=break-in-function expect=exception:SyntaxError
for i in range(3):
    def stop():
        break
```

Use a return value or an exception to tell the loop to stop.

`break` ends only one loop. To leave two nested loops at once, see the techniques for exiting nested loops.

## Comparison

In Go, a break statement "terminates execution of the innermost" for, switch or select statement "within the same function".[^3] So a Go break in a switch ends the switch, not the loop. A Python break in a match statement ends the loop, because match is not a loop (rule r1). A Go break or continue can also name the label of an enclosing statement, "and that is the one whose execution terminates."[^3] PEP 3136 proposed labels for break and continue in Python, and its status is "Rejected".[^4] So a Python break always ends the innermost loop.
