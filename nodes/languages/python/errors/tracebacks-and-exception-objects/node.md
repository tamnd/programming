---
tag: RXZA8S
kind: feature
owner: python
slug: tracebacks-and-exception-objects
name: Tracebacks and exception objects
aliases: [traceback module, __traceback__, traceback object, format_exc, TracebackException, extract_tb, with_traceback]
area: languages/python/errors
level: advanced
summary: >-
  A caught exception holds its stack trace in __traceback__, a chain of traceback objects. The traceback module extracts, formats and prints it, and TracebackException keeps it for later.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/datamodel.html#traceback-objects", locator: "3.2.13.3. Traceback objects"}
see_also: [python/exceptions, python/exception-chaining, python/enriching-exceptions-with-notes, python/handling-exceptions]
sources:
  - url: https://docs.python.org/3.15/reference/datamodel.html#traceback-objects
    title: The Python Language Reference, 3. Data model
    locator: 3.2.13.3. Traceback objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/traceback.html
    title: traceback, Print or retrieve a stack traceback
    locator: introduction; extract_tb; format_exception; clear_frames; TracebackException
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#BaseException.__traceback__
    title: Built-in Exceptions
    locator: BaseException.with_traceback; BaseException.__traceback__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#except-clause
    title: The Python Language Reference, 8. Compound statements
    locator: 8.4.1. except clause
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#recover
    title: Effective Go
    locator: Recover
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import traceback

def parse(text):
    return int(text)

def load(lines):
    return [parse(s) for s in lines]

try:
    load(['1', 'x'])
except ValueError as e:
    for frame in traceback.extract_tb(e.__traceback__):
        print(frame.name, frame.line)
```

```output for=basics
<module> load(['1', 'x'])
load return [parse(s) for s in lines]
parse return int(text)
```

The exception keeps the stack from the handler down to the place of the error. `extract_tb` gives one entry for each level, with the function name and the source line.

## Semantics

**r1.** "Traceback objects represent the stack trace of an exception. A traceback object is implicitly created when an exception occurs".[^1] "when the search for an exception handler unwinds the execution stack, at each unwound level a traceback object is inserted in front of the current traceback."[^1]

**r2.** "When an exception handler is entered, the stack trace is made available to the program."[^1] It is "the __traceback__ attribute of the caught exception".[^1] `__traceback__` is "A writable field that holds the traceback object associated with this exception."[^3]

**r3.** A traceback object has `tb_frame`, which "Points to the execution frame of the current level", and `tb_lineno`, which "Gives the line number where the exception occurred".[^1] "The special writable attribute tb_next is the next level in the stack trace (towards the frame where the exception occurred), or None if there is no next level."[^1] Example: [`walk`](#walk).

**r4.** The traceback module "provides a standard interface to extract, format and print stack traces of Python programs."[^2] `format_exception` returns "a list of strings, each ending in a newline and some containing internal newlines. When these lines are concatenated and printed, exactly the same text is printed as does print_exception()."[^2]

**r5.** TracebackException can "Capture an exception for later rendering."[^2] The module has "a utility for capturing enough information about an exception to print it later, without the need to save a reference to the actual exception. Since exceptions can be the roots of large objects graph, this utility can significantly improve memory management."[^2] Example: [`later`](#later).

**r6.** `with_traceback(tb)` "sets tb as the new traceback for the exception and returns the exception object."[^3]

## Examples

```python run id=walk
def inner():
    return 1 / 0

def outer():
    return inner()

try:
    outer()
except ZeroDivisionError as e:
    tb = e.__traceback__
    while tb is not None:
        print(tb.tb_frame.f_code.co_name, tb.tb_lineno)
        tb = tb.tb_next
```

```output for=walk
<module> 8
outer 5
inner 2
```

The chain starts at the frame of the handler and ends at the frame where the error happened (rule r3).

```python run id=later
import traceback

def risky(n):
    if n == 0:
        raise ValueError('n must not be 0')
    return 10 // n

saved = []
for n in [5, 0]:
    try:
        risky(n)
    except ValueError as e:
        saved.append(traceback.TracebackException.from_exception(e))

for te in saved:
    print([f.name for f in te.stack])
    print(''.join(te.format_exception_only()), end='')
```

```output for=later
['<module>', 'risky']
ValueError: n must not be 0
```

The list keeps a summary of the error, not the exception and its frames (rule r5).

## Pitfalls

A saved exception keeps the local variables of its frames alive:

```python wrong id=keep-alive
import weakref

class Buffer:
    pass

errors = []

def work():
    data = Buffer()
    ref = weakref.ref(data)
    try:
        raise ValueError('failed')
    except ValueError as e:
        errors.append(e)
    return ref

ref = work()
print('buffer alive:', ref() is not None)
```

```output for=keep-alive
buffer alive: True
```

`work` has returned, but `errors` holds the exception, the exception holds the traceback, and the traceback holds the frame with `data` (rule r3). The reference says that with the traceback, exceptions "form a reference cycle with the stack frame, keeping all locals in that frame alive until the next garbage collection occurs."[^4]

```python run id=clear
import traceback
import weakref

class Buffer:
    pass

errors = []

def work():
    data = Buffer()
    ref = weakref.ref(data)
    try:
        raise ValueError('failed')
    except ValueError as e:
        errors.append(e)
    return ref

ref = work()
for e in errors:
    traceback.clear_frames(e.__traceback__)
print('buffer alive:', ref() is not None)
```

```output for=clear
buffer alive: False
```

`clear_frames` "Clears the local variables of all the stack frames in a traceback".[^2] The code calls it after `work` has returned. Another way is to keep a TracebackException instead of the exception (rule r5).

## Comparison

A Go error value has no stack trace of its own. A panic prints the stack when the program stops, and a deferred function can stop the panic: "A call to recover stops the unwinding and returns the argument passed to panic."[^5] A Python exception carries its traceback, so a handler can print or keep the stack (rule r2).
