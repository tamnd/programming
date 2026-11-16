---
tag: 65Z2GH
kind: feature
owner: python
slug: the-exception-hierarchy
name: The Exception Hierarchy
aliases: [exception class hierarchy, BaseException, Exception base class, built-in exception tree, LookupError, ArithmeticError]
area: languages/python/errors
level: intro
summary: >-
  The built-in exception classes form a tree with BaseException at the root. Exception is the base of the normal errors. An except clause for a class also catches all its subclasses.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/exceptions.html#exception-hierarchy", locator: "Built-in Exceptions, Exception hierarchy"}
see_also: [python/exceptions, python/handling-exceptions, python/user-defined-exceptions, python/errors-and-exceptions]
sources:
  - url: https://docs.python.org/3.15/builtins/exceptions.html#exception-hierarchy
    title: Built-in Exceptions
    locator: introduction; Base classes; SystemExit; KeyboardInterrupt; OSError; OS exceptions; Exception hierarchy
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/errors.html#handling-exceptions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.3. Handling Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#exceptions
    title: The Python Language Reference, 4. Execution model
    locator: 4.3. Exceptions
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Errors
    title: The Go Programming Language Specification
    locator: Errors; Run-time panics
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
print([c.__name__ for c in ZeroDivisionError.__mro__])
try:
    {'a': 1}['b']
except LookupError as err:
    print('lookup failed:', type(err).__name__)
```

```output for=basics
['ZeroDivisionError', 'ArithmeticError', 'Exception', 'BaseException', 'object']
lookup failed: KeyError
```

ZeroDivisionError is a subclass of ArithmeticError, which is a subclass of Exception and BaseException. A KeyError is a LookupError, so `except LookupError` catches it.

## Semantics

**r1.** "In Python, all exceptions must be instances of a class that derives from BaseException."[^1] The top of the tree is:[^1]

```text
BaseException
 ├── BaseExceptionGroup
 ├── GeneratorExit
 ├── KeyboardInterrupt
 ├── SystemExit
 └── Exception
      ├── ArithmeticError
      ├── LookupError
      ├── OSError
      ├── ...
      └── Warning
```

**r2.** "A class in an except clause matches exceptions which are instances of the class itself or one of its derived classes (but not the other way around".[^2] "In a try statement with an except clause that mentions a particular class, that clause also handles any exception classes derived from that class (but not exception classes from which it is derived)."[^1] Example: [`groups`](#groups).

**r3.** Exception: "All built-in, non-system-exiting exceptions are derived from this class. All user-defined exceptions should also be derived from this class."[^1] "BaseException is the common base class of all exceptions. One of its subclasses, Exception, is the base class of all the non-fatal exceptions."[^2]

**r4.** "Exceptions which are not subclasses of Exception are not typically handled, because they are used to indicate that the program should terminate. They include SystemExit which is raised by sys.exit() and KeyboardInterrupt which is raised when a user wishes to interrupt the program."[^2] SystemExit "inherits from BaseException instead of Exception so that it is not accidentally caught by code that catches Exception."[^1] Example: [`exit`](#exit).

**r5.** Some classes are "used mostly as base classes for other exceptions".[^1] ArithmeticError is "The base class for those built-in exceptions that are raised for various arithmetic errors: OverflowError, ZeroDivisionError, FloatingPointError."[^1] LookupError is "The base class for the exceptions that are raised when a key or index used on a mapping or sequence is invalid: IndexError, KeyError."[^1]

**r6.** The subclasses of OSError "get raised depending on the system error code."[^1] "The constructor often actually returns a subclass of OSError, as described in OS exceptions below. The particular subclass depends on the final errno value."[^1] EnvironmentError and IOError are kept for compatibility: "starting from Python 3.3, they are aliases of OSError."[^1] Example: [`oserror`](#oserror).

**r7.** The except clause "must reference the class of the instance or a non-virtual base class thereof."[^3] Example: [`virtual`](#virtual).

## Examples

```python run id=groups
for action in [lambda: [1, 2][9], lambda: {}['k'], lambda: 1 / 0, lambda: 2.0 ** 5000]:
    try:
        action()
    except LookupError as err:
        print('LookupError', type(err).__name__)
    except ArithmeticError as err:
        print('ArithmeticError', type(err).__name__)
```

```output for=groups
LookupError IndexError
LookupError KeyError
ArithmeticError ZeroDivisionError
ArithmeticError OverflowError
```

Two handlers for base classes catch four kinds of error (rules r2 and r5).

```python run id=exit
import sys

try:
    try:
        sys.exit(3)
    except Exception:
        print('except Exception caught it')
except BaseException as err:
    print('outer handler:', type(err).__name__, err.code)
```

```output for=exit
outer handler: SystemExit 3
```

`except Exception` does not match SystemExit, so it goes to the outer handler (rule r4).

```python run id=oserror
import errno

try:
    open('/no/such/dir/file.txt')
except OSError as err:
    print(type(err).__name__, err.errno == errno.ENOENT)

print(type(OSError(errno.ENOENT, 'gone')).__name__)
print(IOError is OSError)
```

```output for=oserror
FileNotFoundError True
FileNotFoundError
True
```

`except OSError` catches FileNotFoundError. The OSError constructor gives a subclass that depends on the errno value, and IOError is the same class (rule r6).

## Pitfalls

A handler for a base class hides the handlers after it:

```python wrong id=order
try:
    int('abc')
except Exception:
    print('some error')
except ValueError:
    print('not a number')
```

```output for=order
some error
```

"At most one handler will be executed."[^2] The tutorial says that with the base class first, its example "would have printed B, B, B".[^2] The `except Exception` clause matches the ValueError first. Put the most specific classes first (rule r2).

A virtual base class does not match:

```python fails id=virtual expect=exception:TimeoutError
from abc import ABCMeta

class Retryable(Exception, metaclass=ABCMeta):
    pass

Retryable.register(TimeoutError)
print(issubclass(TimeoutError, Retryable))

try:
    raise TimeoutError('slow')
except Retryable:
    print('retry')
```

`issubclass` says True, but the except clause does not use the registration, and the TimeoutError is not caught (rule r7). Use a real base class, or a tuple of the classes.

## Comparison

Go has no tree of error classes. "The predeclared type error is defined as" an interface with the method `Error() string`,[^4] and any type with that method is an error. Python selects a handler by the class of the exception and its base classes (rule r2). Run-time errors in Go have the interface type runtime.Error, which "satisfies the predeclared interface type error."[^4]
