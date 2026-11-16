---
tag: 6YRWG9
kind: feature
owner: python
slug: clean-up-actions-with-finally
name: Defining Clean-up Actions
aliases: [finally clause, try finally, clean-up code, cleanup handler, finally block]
area: languages/python/errors
level: intro
summary: >-
  The finally clause of a try statement holds clean-up code. It runs when the try statement completes, with or without an exception, and an exception that is not handled is raised again after it.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#finally-clause", locator: "8.4.4. finally clause"}
see_also: [python/finally-and-control-flow, python/the-with-statement, python/handling-exceptions, python/exception-chaining]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#defining-clean-up-actions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.7. Defining Clean-up Actions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#finally-clause
    title: The Python Language Reference, 8. Compound statements
    locator: 8.4.4. finally clause
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#defer
    title: Effective Go
    locator: Defer
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
def divide(x, y):
    try:
        result = x / y
    except ZeroDivisionError:
        print('division by zero!')
    else:
        print('result is', result)
    finally:
        print('executing finally clause')

divide(2, 1)
divide(2, 0)
try:
    divide('2', '1')
except TypeError as err:
    print('caller got', type(err).__name__)
```

```output for=basics
result is 2.0
executing finally clause
division by zero!
executing finally clause
executing finally clause
caller got TypeError
```

The finally clause runs in all three calls. The TypeError has no handler in `divide`, so it goes to the caller after the finally clause.

## Semantics

**r1.** The finally clause "is intended to define clean-up actions that must be executed under all circumstances."[^1] "If a finally clause is present, the finally clause will execute as the last task before the try statement completes. The finally clause runs whether or not the try statement produces an exception."[^1]

**r2.** "If an exception occurs in any of the clauses and is not handled, the exception is temporarily saved. The finally clause is executed. If there is a saved exception it is re-raised at the end of the finally clause."[^2] This covers exceptions in the try clause and also those in an except or else clause.[^1]

**r3.** "If the finally clause raises another exception, the saved exception is set as the context of the new exception."[^2] Example: [`raise-in-finally`](#raise-in-finally).

**r4.** "In real world applications, the finally clause is useful for releasing external resources (such as files or network connections), regardless of whether the use of the resource was successful."[^1] Example: [`release`](#release).

**r5.** A break, continue or return in the try clause also runs the finally clause, and a return, break or continue in the finally clause discards the saved exception.[^2] See [finally and control flow](python/finally-and-control-flow).

## Examples

```python run id=release
class Connection:
    def __init__(self, name):
        self.name = name
        print('open', name)
    def send(self, data):
        if not data:
            raise ValueError('empty message')
        print('sent', data)
    def close(self):
        print('close', self.name)

def post(data):
    conn = Connection('db')
    try:
        conn.send(data)
    finally:
        conn.close()

post('hello')
try:
    post('')
except ValueError as err:
    print('error:', err)
```

```output for=release
open db
sent hello
close db
open db
close db
error: empty message
```

The connection closes after a good send and after a failed one (rule r4). The object is made before the try statement. If the constructor fails, there is nothing to close.

```python run id=raise-in-finally
try:
    try:
        1 / 0
    finally:
        raise RuntimeError('clean-up failed')
except RuntimeError as err:
    print(repr(err))
    print(repr(err.__context__))
```

```output for=raise-in-finally
RuntimeError('clean-up failed')
ZeroDivisionError('division by zero')
```

The exception from the finally clause replaces the ZeroDivisionError, which becomes its context (rule r3).

## Pitfalls

An exception in the clean-up code hides the first error:

```python wrong id=hidden
class Log:
    def __init__(self):
        self.closed = False
    def write(self, text):
        raise OSError('disk full')
    def close(self):
        if self.closed:
            raise ValueError('already closed')
        self.closed = True

log = Log()
log.close()
try:
    try:
        log.write('report')
    finally:
        log.close()
except Exception as err:
    print(type(err).__name__, err)
```

```output for=hidden
ValueError already closed
```

The real problem is the OSError, but the handler sees the ValueError from the finally clause. The OSError is only in `__context__` (rule r3). Keep clean-up code simple, so that it cannot fail.

## Comparison

Go uses defer for clean-up. Effective Go says: "Go's defer statement schedules a function call (the deferred function) to be run immediately before the function executing the defer returns."[^3] A deferred call runs when the function returns, and a finally clause runs when its try statement completes (rule r1).
