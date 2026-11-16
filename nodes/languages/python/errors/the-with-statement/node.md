---
tag: X98AVT
kind: feature
owner: python
slug: the-with-statement
name: Predefined Clean-up Actions
aliases: [with statement, with open, with as, context expression, multiple context managers]
area: languages/python/errors
level: intro
summary: >-
  The with statement uses an object's own clean-up action. with open(...) as f closes the file when the block ends, also when the block raises an exception.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-with-statement", locator: "8.5. The with statement"}
see_also: [python/context-managers, python/clean-up-actions-with-finally, python/handling-exceptions]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#predefined-clean-up-actions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.8. Predefined Clean-up Actions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-with-statement
    title: The Python Language Reference, 8. Compound statements
    locator: 8.5. The with statement
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#defer
    title: Effective Go
    locator: Defer
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
with open('notes.txt', 'w') as f:
    f.write('first line\nsecond line\n')
print(f.closed)

with open('notes.txt') as f:
    for line in f:
        print(line, end='')
print(f.closed)
```

```output for=basics
True
first line
second line
True
```

Each with statement closes its file at the end of the block, and the program does not call `close`.

## Semantics

**r1.** "Some objects define standard clean-up actions to be undertaken when the object is no longer needed, regardless of whether or not the operation using the object succeeded or failed."[^1] "The with statement allows objects like files to be used in a way that ensures they are always cleaned up promptly and correctly."[^1]

**r2.** "After the statement is executed, the file f is always closed, even if a problem was encountered while processing the lines. Objects which, like files, provide predefined clean-up actions will indicate this in their documentation."[^1] Example: [`error`](#error).

**r3.** "The with statement is used to wrap the execution of a block with methods defined by a context manager".[^2] "This allows common try…except…finally usage patterns to be encapsulated for convenient reuse."[^2] The grammar is `with_stmt: "with" ( "(" with_stmt_contents ","? ")" | with_stmt_contents ) ":" suite`.[^2]

**r4.** "If a target was included in the with statement, the return value from __enter__() is assigned to it."[^2] "The with statement guarantees that if the __enter__() method returns without an error, then __exit__() will always be called."[^2] See [context managers](python/context-managers) for these methods.

**r5.** "With more than one item, the context managers are processed as if multiple with statements were nested".[^2] "You can also write multi-item context managers in multiple lines if the items are surrounded by parentheses."[^2] Example: [`several`](#several).

## Examples

```python run id=error
with open('data.txt', 'w') as out:
    out.write('1\n2\nx\n')

total = 0
try:
    with open('data.txt') as f:
        for line in f:
            total += int(line)
except ValueError as err:
    print('bad line:', err)
print(total, f.closed)
```

```output for=error
bad line: invalid literal for int() with base 10: 'x\n'
3 True
```

`int` fails on the third line, and the file is closed before the handler runs (rule r2).

```python run id=several
with open('a.txt', 'w') as a, open('b.txt', 'w') as b:
    a.write('alpha\n')
    b.write('beta\n')

with (
    open('a.txt') as a,
    open('b.txt') as b,
):
    print(a.read() + b.read(), end='')
print(a.closed, b.closed)
```

```output for=several
alpha
beta
True True
```

The first statement has two items on one line. The second puts them in parentheses on several lines (rule r5).

## Pitfalls

A file opened without with stays open:

```python wrong id=no-with
f = open('log.txt', 'w')
f.write('started\n')
print(f.closed)
```

```output for=no-with
False
```

The tutorial says this kind of code "leaves the file open for an indeterminate amount of time after this part of the code has finished executing."[^1] Write `with open('log.txt', 'w') as f:` (rule r1).

The name after `as` is still bound after the block, but the file is closed:

```python fails id=after expect=exception:ValueError
with open('notes.txt', 'w') as f:
    f.write('one\n')
f.write('two\n')
```

The with statement does not delete `f`. It closes the file, so the second write raises ValueError. Do all the work on the file inside the block.

## Comparison

Go uses defer for the same job. Effective Go calls it "an unusual but effective way to deal with situations such as resources that must be released regardless of which path a function takes to return. The canonical examples are unlocking a mutex or closing a file."[^3] A Go program writes `defer f.Close()` after it opens the file. In Python the file object has the clean-up action, and the with statement calls it at the end of the block (rule r1).
