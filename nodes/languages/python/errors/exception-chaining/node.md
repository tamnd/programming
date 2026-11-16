---
tag: PN6WTE
kind: feature
owner: python
slug: exception-chaining
name: Exception Chaining
aliases: [raise from, __cause__, __context__, __suppress_context__, from None, chained exceptions]
area: languages/python/errors
level: core
summary: >-
  An exception raised in a handler keeps the handled one in __context__. raise X from Y sets __cause__ to say X is a direct result of Y, and from None hides the old exception in the traceback.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-raise-statement", locator: "7.8. The raise statement"}
see_also: [python/raising-exceptions, python/handling-exceptions, python/user-defined-exceptions]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#exception-chaining
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.5. Exception Chaining
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-raise-statement
    title: The Python Language Reference, 7. Simple statements
    locator: 7.8. The raise statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#exception-context
    title: Built-in Exceptions
    locator: Exception context
    accessed: 2026-11-16
  - url: https://pkg.go.dev/errors
    title: errors package
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class ConfigError(Exception):
    pass

def port(config):
    try:
        return int(config['port'])
    except KeyError as exc:
        raise ConfigError('port is not set') from exc

try:
    port({})
except ConfigError as err:
    print(err)
    print(repr(err.__cause__))
```

```output for=basics
port is not set
KeyError('port')
```

The function changes a KeyError into an error of its own type. With `from exc`, the new exception keeps the KeyError as its cause.

## Semantics

**r1.** "If an unhandled exception occurs inside an except section, it will have the exception being handled attached to it and included in the error message".[^1] "When raising a new exception while another exception is already being handled, the new exception’s __context__ attribute is automatically set to the handled exception. An exception may be handled when an except or finally clause, or a with statement, is used."[^3] Example: [`implicit`](#implicit).

**r2.** "To indicate that an exception is a direct consequence of another, the raise statement allows an optional from clause".[^1] "If the second expression is an exception instance, it will be attached to the raised exception as the __cause__ attribute (which is writable). If the expression is an exception class, the class will be instantiated and the resulting exception instance will be attached to the raised exception as the __cause__ attribute."[^2]

**r3.** "The expression following from must be an exception or None."[^3] "Setting __cause__ also implicitly sets the __suppress_context__ attribute to True, so that using raise new_exc from None effectively replaces the old exception with the new one for display purposes (e.g. converting KeyError to AttributeError), while leaving the old exception available in __context__ for introspection when debugging."[^3] Example: [`from-none`](#from-none).

**r4.** "An explicitly chained exception in __cause__ is always shown when present. An implicitly chained exception in __context__ is shown only if __cause__ is None and __suppress_context__ is false."[^3] "In either case, the exception itself is always shown after any chained exceptions so that the final line of the traceback always shows the last exception that was raised."[^3] Example: [`display`](#display).

## Examples

```python run id=implicit
try:
    try:
        1 / 0
    except ZeroDivisionError:
        raise RuntimeError('cannot compute')
except RuntimeError as err:
    print(repr(err.__context__))
    print(err.__cause__, err.__suppress_context__)
```

```output for=implicit
ZeroDivisionError('division by zero')
None False
```

Python sets `__context__` without a from clause (rule r1).

```python run id=from-none
class Settings:
    def __init__(self, values):
        self._values = values
    def __getattr__(self, name):
        try:
            return self._values[name]
        except KeyError:
            raise AttributeError(name) from None

try:
    Settings({'debug': True}).color
except AttributeError as err:
    print(repr(err))
    print(err.__cause__, err.__suppress_context__)
    print(repr(err.__context__))
```

```output for=from-none
AttributeError('color')
None True
KeyError('color')
```

This is the case that the docs name: a KeyError becomes an AttributeError. The traceback does not show the KeyError, but `__context__` still holds it (rule r3).

```python run id=display
import traceback

try:
    try:
        {}['port']
    except KeyError as exc:
        raise RuntimeError('no port') from exc
except RuntimeError as err:
    for chunk in traceback.format_exception(err):
        line = chunk.strip()
        if line and not line.startswith('File'):
            print(line)
```

```output for=display
Traceback (most recent call last):
KeyError: 'port'
The above exception was the direct cause of the following exception:
Traceback (most recent call last):
RuntimeError: no port
```

The loop drops the lines for the stack frames. The cause comes first, and the last line is the exception that was raised last (rule r4).

## Pitfalls

A new exception in a handler without `from` looks like a second bug:

```python wrong id=no-from
import traceback

def port(config):
    try:
        return int(config['port'])
    except KeyError:
        raise ValueError('port is not set')

try:
    port({})
except ValueError as err:
    for chunk in traceback.format_exception(err):
        line = chunk.strip()
        if line and not line.startswith('File'):
            print(line)
```

```output for=no-from
Traceback (most recent call last):
KeyError: 'port'
During handling of the above exception, another exception occurred:
Traceback (most recent call last):
ValueError: port is not set
```

The text "During handling of the above exception" suggests that the handler failed by accident (rule r1). When the new exception is on purpose, bind the old one with `except KeyError as exc:` and write `raise ValueError('port is not set') from exc`, or write `from None`.

The value after `from` must be an exception:

```python fails id=bad-cause expect=exception:TypeError
try:
    {}['port']
except KeyError:
    raise ValueError('port is not set') from 'missing key'
```

A string is not an exception or None, so the raise statement fails with TypeError (rule r3).

## Comparison

Go wraps errors in values. "An easy way to create wrapped errors is to call fmt.Errorf and apply the %w verb to the error argument".[^4] "An error e wraps another error if e's type has one of the methods" `Unwrap() error` or `Unwrap() []error`.[^4] "The Is and As functions inspect an error's tree by examining first the error itself followed by the tree of each of its children in turn".[^4] In Python `raise ... from` stores the cause in `__cause__` (rule r2), and an except clause matches only the class of the outer exception, not its cause.
