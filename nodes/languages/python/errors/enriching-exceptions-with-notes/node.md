---
tag: 8P0ZB1
kind: feature
owner: python
slug: enriching-exceptions-with-notes
name: Enriching Exceptions with Notes
aliases: [add_note, __notes__, exception notes, PEP 678, add context to an exception]
area: languages/python/errors
level: core
summary: >-
  add_note(note) adds a string to an exception after it was caught. The notes are in __notes__, and the standard traceback shows them after the exception, in the order they were added.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/exceptions.html#BaseException.add_note", locator: "Built-in Exceptions, BaseException.add_note"}
see_also: [python/raising-and-handling-multiple-unrelated-exceptions, python/exception-chaining, python/tracebacks-and-exception-objects]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#enriching-exceptions-with-notes
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.10. Enriching Exceptions with Notes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#BaseException.add_note
    title: Built-in Exceptions
    locator: BaseException.add_note; BaseException.__notes__
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/traceback.html#traceback.format_exception_only
    title: traceback, Print or retrieve a stack traceback
    locator: format_exception_only
    accessed: 2026-11-16
  - url: https://pkg.go.dev/errors
    title: errors package
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import traceback

def load(path, text):
    try:
        return int(text)
    except ValueError as e:
        e.add_note(f'while reading {path}')
        raise

try:
    load('count.txt', 'ten')
except ValueError as e:
    print(e.__notes__)
    print(''.join(traceback.format_exception_only(e)), end='')
```

```output for=basics
['while reading count.txt']
ValueError: invalid literal for int() with base 10: 'ten'
while reading count.txt
```

The function adds the file name to the exception and raises it again. The note is printed after the exception line.

## Semantics

**r1.** "There are cases where it is useful to add information after the exception was caught. For this purpose, exceptions have a method add_note(note) that accepts a string and adds it to the exception’s notes list."[^1] "The standard traceback rendering includes all notes, in the order they were added, after the exception."[^1] In the list from `traceback.format_exception_only`, "Following the message, the list contains the exception’s notes."[^3]

**r2.** `add_note` will "Add the string note to the exception’s notes which appear in the standard traceback after the exception string. A TypeError is raised if note is not a string."[^2] Example: [`not-string`](#not-string).

**r3.** `__notes__` is "A list of the notes of this exception, which were added with add_note(). This attribute is created when add_note() is called."[^2] Example: [`no-notes`](#no-notes).

**r4.** "when collecting exceptions into an exception group, we may want to add context information for the individual errors."[^1] Example: [`group`](#group).

## Examples

```python run id=group
excs = []
for i in range(3):
    try:
        raise OSError('operation failed')
    except Exception as e:
        e.add_note(f'Happened in Iteration {i+1}')
        excs.append(e)

eg = ExceptionGroup('We have some problems', excs)
for e in eg.exceptions:
    print(e, '|', e.__notes__[0])
```

```output for=group
operation failed | Happened in Iteration 1
operation failed | Happened in Iteration 2
operation failed | Happened in Iteration 3
```

The three exceptions have the same message. The notes tell them apart (rule r4).

```python run id=layers
def parse_line(n, line):
    try:
        return int(line)
    except ValueError as e:
        e.add_note(f'line {n}')
        raise

def parse_file(name, lines):
    try:
        return [parse_line(n, s) for n, s in enumerate(lines, 1)]
    except ValueError as e:
        e.add_note(f'file {name}')
        raise

try:
    parse_file('data.txt', ['1', '2', 'x'])
except ValueError as e:
    print(e.__notes__)
```

```output for=layers
['line 3', 'file data.txt']
```

Each layer adds what it knows, and the notes keep the order in which they were added (rule r1).

## Pitfalls

An exception has no `__notes__` until a note is added:

```python fails id=no-notes expect=exception:AttributeError
err = ValueError('bad')
print(err.__notes__)
```

The attribute is created by the first call to `add_note` (rule r3). Use `getattr(err, '__notes__', [])` when the exception may have no notes.

A note must be a string:

```python fails id=not-string expect=exception:TypeError
err = KeyError('id')
err.add_note(404)
```

Convert the value first, for example with `str(404)` or an f-string (rule r2).

## Comparison

Go adds context by wrapping the error in a new one. "An easy way to create wrapped errors is to call fmt.Errorf and apply the %w verb to the error argument".[^4] The wrapper is a new error with the context in its message. A Python note changes the same exception object, so its type and its handlers stay the same (rule r1).
