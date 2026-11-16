---
tag: 1J76YD
kind: feature
owner: python
slug: context-managers
name: Context managers
aliases: [__enter__, __exit__, contextlib.contextmanager, contextlib.suppress, ExitStack, context manager protocol]
area: languages/python/errors
level: core
summary: >-
  A context manager has __enter__ and __exit__ methods, which the with statement calls around its block. contextlib makes them from generators and has ready ones such as suppress and ExitStack.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#with-statement-context-managers", locator: "3.3.9. With Statement Context Managers"}
see_also: [python/the-with-statement, python/clean-up-actions-with-finally, python/handling-exceptions]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#with-statement-context-managers
    title: The Python Language Reference, 3. Data model
    locator: 3.3.9. With Statement Context Managers
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-with-statement
    title: The Python Language Reference, 8. Compound statements
    locator: 8.5. The with statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/contextlib.html
    title: contextlib, Utilities for with-statement contexts
    locator: contextmanager; suppress; ExitStack
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#defer
    title: Effective Go
    locator: Defer
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class Timer:
    def __enter__(self):
        print('enter')
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        print('exit', exc_type.__name__ if exc_type else None)
        return False

with Timer() as t:
    print('body', type(t).__name__)

try:
    with Timer():
        raise KeyError('k')
except KeyError:
    print('KeyError went on')
```

```output for=basics
enter
body Timer
exit None
enter
exit KeyError
KeyError went on
```

The with statement calls `__enter__` before the block and `__exit__` after it. `__exit__` gets the exception, and because it returns False the exception goes on.

## Semantics

**r1.** "A context manager is an object that defines the runtime context to be established when executing a with statement. The context manager handles the entry into, and the exit from, the desired runtime context for the execution of the block of code."[^1] "Typical uses of context managers include saving and restoring various kinds of global state, locking and unlocking resources, closing opened files, etc."[^1]

**r2.** `__enter__`: "The with statement will bind this method’s return value to the target(s) specified in the as clause of the statement, if any."[^1]

**r3.** `__exit__(self, exc_type, exc_value, traceback)`: "The parameters describe the exception that caused the context to be exited. If the context was exited without an exception, all three arguments will be None."[^1] "If an exception is supplied, and the method wishes to suppress the exception (i.e., prevent it from being propagated), it should return a true value."[^1] "Note that __exit__() methods should not reraise the passed-in exception; this is the caller’s responsibility."[^1]

**r4.** `contextlib.contextmanager` "is a decorator that can be used to define a factory function for with statement context managers, without needing to create a class or separate __enter__() and __exit__() methods."[^3] The generator "must yield exactly one value, which will be bound to the targets in the with statement’s as clause, if any."[^3] "If an unhandled exception occurs in the block, it is reraised inside the generator at the point where the yield occurred."[^3] Example: [`generator`](#generator).

**r5.** `suppress` returns "a context manager that suppresses any of the specified exceptions if they occur in the body of a with statement and then resumes execution with the first statement following the end of the with statement."[^3] Example: [`suppress`](#suppress).

**r6.** ExitStack is "A context manager that is designed to make it easy to programmatically combine other context managers and cleanup functions, especially those that are optional or otherwise driven by input data."[^3] "Each instance maintains a stack of registered callbacks that are called in reverse order when the instance is closed".[^3] Example: [`exitstack`](#exitstack).

## Examples

```python run id=generator
from contextlib import contextmanager

@contextmanager
def tag(name):
    print(f'<{name}>')
    try:
        yield name
    finally:
        print(f'</{name}>')

with tag('p') as t:
    print('text in', t)

try:
    with tag('b'):
        raise ValueError('bad')
except ValueError as err:
    print('error:', err)
```

```output for=generator
<p>
text in p
</p>
<b>
</b>
error: bad
```

The code before `yield` runs on entry and the code after it runs on exit. The try statement around `yield` closes the tag when the block raises (rule r4).

```python run id=suppress
import os
from contextlib import suppress

with suppress(FileNotFoundError):
    os.remove('no-such-file.tmp')
print('went on')
```

```output for=suppress
went on
```

This is the same as a try statement with `except FileNotFoundError: pass` (rule r5).

```python run id=exitstack
from contextlib import ExitStack

class Resource:
    def __init__(self, name):
        self.name = name
    def __enter__(self):
        print('open', self.name)
        return self
    def __exit__(self, *exc):
        print('close', self.name)

with ExitStack() as stack:
    items = [stack.enter_context(Resource(n)) for n in ['a', 'b', 'c']]
    stack.callback(print, 'callback')
    print('using', len(items))
```

```output for=exitstack
open a
open b
open c
using 3
callback
close c
close b
close a
```

The number of resources comes from the data. ExitStack closes them in reverse order (rule r6).

## Pitfalls

A true return value from `__exit__` hides every exception:

```python wrong id=swallow
class Quiet:
    def __enter__(self):
        return self
    def __exit__(self, *exc):
        print('cleaning up')
        return True

def average(values):
    with Quiet():
        return sum(values) / len(values)

print(average([]))
```

```output for=swallow
cleaning up
None
```

The ZeroDivisionError is suppressed, and the function returns None (rule r3). Return False or None, unless suppression is the purpose.

A generator without try and finally does not clean up after an error:

```python wrong id=no-finally
from contextlib import contextmanager

@contextmanager
def tag(name):
    print(f'<{name}>')
    yield
    print(f'</{name}>')

try:
    with tag('b'):
        raise ValueError('bad')
except ValueError:
    print('error')
```

```output for=no-finally
<b>
error
```

The exception is raised again at the `yield` (rule r4), so the line after it does not run and the tag stays open. Put the `yield` in a try statement with a finally clause.

## Comparison

Go has no protocol like `__enter__` and `__exit__`. A Go function calls `defer` with the clean-up call, and "Deferred functions are executed in LIFO order".[^4] ExitStack gives the same last-in, first-out order in Python (rule r6).
