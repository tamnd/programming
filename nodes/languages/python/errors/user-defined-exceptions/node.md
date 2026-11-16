---
tag: QVY55R
kind: feature
owner: python
slug: user-defined-exceptions
name: User-defined Exceptions
aliases: [custom exceptions, exception subclass, define an exception class, custom error class, module base exception]
area: languages/python/errors
level: core
summary: >-
  A program can define its own exception classes. Derive them from Exception, give them names that end in Error, and add attributes that tell handlers about the error.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/errors.html#user-defined-exceptions", locator: "8.6. User-defined Exceptions"}
see_also: [python/the-exception-hierarchy, python/raising-exceptions, python/handling-exceptions, python/exception-chaining]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#user-defined-exceptions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.3. Handling Exceptions; 8.6. User-defined Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html
    title: Built-in Exceptions
    locator: introduction; Inheriting from built-in exceptions; BaseException; Exception
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#errors
    title: Effective Go
    locator: Errors
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
class InsufficientFundsError(Exception):
    def __init__(self, balance, amount):
        super().__init__(f'cannot withdraw {amount}, balance is {balance}')
        self.balance = balance
        self.amount = amount

def withdraw(balance, amount):
    if amount > balance:
        raise InsufficientFundsError(balance, amount)
    return balance - amount

try:
    withdraw(50, 80)
except InsufficientFundsError as err:
    print(err)
    print('short by', err.amount - err.balance)
```

```output for=basics
cannot withdraw 80, balance is 50
short by 30
```

The class derives from Exception. It passes a message to the base class and keeps the numbers as attributes for the handler.

## Semantics

**r1.** "Programs may name their own exceptions by creating a new exception class".[^1] "Exceptions should typically be derived from the Exception class, either directly or indirectly."[^1] BaseException "is not meant to be directly inherited by user-defined classes (for that, use Exception)."[^2]

**r2.** "Exception classes can be defined which do anything any other class can do, but are usually kept simple, often only offering a number of attributes that allow information about the error to be extracted by handlers for the exception."[^1]

**r3.** "Most exceptions are defined with names that end in “Error”, similar to the naming of the standard exceptions."[^1] "Many standard modules define their own exceptions to report errors that may occur in functions they define."[^1] Example: [`family`](#family).

**r4.** Built-in exception types "define __str__() to print all the arguments without explicitly accessing .args."[^1] The tutorial notes that `__str__` "may be overridden in exception subclasses".[^1] Example: [`str`](#str).

**r5.** "It’s recommended to only subclass one exception type at a time to avoid any possible conflicts between how the bases handle the args attribute, as well as due to possible memory layout incompatibilities."[^2] Example: [`layout`](#layout).

## Examples

```python run id=family
class ParseError(Exception):
    """Base class for the errors of this parser."""

class UnknownTagError(ParseError):
    pass

class UnclosedTagError(ParseError):
    pass

def parse(text):
    if text.startswith('<x>'):
        raise UnknownTagError('x')
    if not text.endswith('>'):
        raise UnclosedTagError(text)
    return 'ok'

for text in ['<b>', '<x>', '<b']:
    try:
        print(parse(text))
    except ParseError as err:
        print(type(err).__name__, err)
```

```output for=family
ok
UnknownTagError x
UnclosedTagError <b
```

A base class for the module lets a caller catch all its errors with one clause, and still tell them apart (rule r3).

```python run id=str
class HTTPError(Exception):
    def __init__(self, status, url):
        super().__init__(status, url)
        self.status = status
        self.url = url

    def __str__(self):
        return f'{self.status} for {self.url}'

err = HTTPError(404, '/missing')
print(err.args)
print(err)
```

```output for=str
(404, '/missing')
404 for /missing
```

Without `__str__`, the message would show the args tuple. The subclass gives a clearer message (rule r4).

## Pitfalls

A class derived from BaseException passes through `except Exception`:

```python fails id=base-exception expect=exception:JobFailed
class JobFailed(BaseException):
    pass

try:
    raise JobFailed('timeout')
except Exception:
    print('job failed, retrying')
```

Code that catches Exception to handle errors does not see this class (rule r1). Derive from Exception.

Two built-in exception types cannot always be combined:

```python fails id=layout expect=exception:TypeError
class StorageError(OSError, AttributeError):
    pass
```

The class statement fails because of the "memory layout incompatibilities" (rule r5). The docs say: "Some have custom memory layouts which makes it impossible to create a subclass that inherits from multiple exception types."[^2] Subclass one built-in type.

## Comparison

In Go, "errors have type error, a simple built-in interface."[^3] "A library writer is free to implement this interface with a richer model under the covers, making it possible not only to see the error but also to provide some context."[^3] Effective Go uses `os.PathError` as an example, a struct with the operation, the path and the system error. A Python exception class has the same role, but it must derive from BaseException, and in practice from Exception (rule r1).
