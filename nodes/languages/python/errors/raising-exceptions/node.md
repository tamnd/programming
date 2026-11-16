---
tag: 65ZGE1
kind: feature
owner: python
slug: raising-exceptions
name: Raising Exceptions
aliases: [raise statement, raise, re-raise, throw an exception, bare raise]
area: languages/python/errors
level: intro
summary: >-
  The raise statement starts an exception. Give it an exception instance, or a class that Python calls with no arguments. A raise with no expression in a handler raises the active exception again.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-raise-statement", locator: "7.8. The raise statement"}
see_also: [python/handling-exceptions, python/exception-chaining, python/user-defined-exceptions, python/exceptions]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#raising-exceptions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.3. Handling Exceptions; 8.4. Raising Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-raise-statement
    title: The Python Language Reference, 7. Simple statements
    locator: 7.8. The raise statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#NotImplementedError
    title: Built-in Exceptions
    locator: NotImplementedError
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#panic
    title: Effective Go
    locator: Panic
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
def set_width(width):
    if width <= 0:
        raise ValueError(f'width must be positive, not {width}')
    return width

for w in [10, -2]:
    try:
        print(set_width(w))
    except ValueError as err:
        print('error:', err)
```

```output for=basics
10
error: width must be positive, not -2
```

The function finds a bad argument and raises a ValueError with a message. The caller handles it.

## Semantics

**r1.** "The raise statement allows the programmer to force a specified exception to occur."[^1] The grammar is `raise_stmt: "raise" [expression ["from" expression]]`.[^2]

**r2.** The argument "must be either an exception instance or an exception class (a class that derives from BaseException, such as Exception or one of its subclasses). If an exception class is passed, it will be implicitly instantiated by calling its constructor with no arguments".[^1] Example: [`class`](#class).

**r3.** "If no expressions are present, raise re-raises the exception that is currently being handled, which is also known as the active exception. If there isn’t currently an active exception, a RuntimeError exception is raised indicating that this is an error."[^2] Example: [`reraise`](#reraise).

**r4.** "A traceback object is normally created automatically when an exception is raised and attached to it as the __traceback__ attribute."[^2]

**r5.** The `from` clause "is used for exception chaining".[^2] See [exception chaining](python/exception-chaining).

## Examples

```python run id=class
try:
    raise KeyError
except KeyError as err:
    print(repr(err), err.args)
```

```output for=class
KeyError() ()
```

`raise KeyError` is the same as `raise KeyError()` (rule r2).

```python run id=reraise
def load(text):
    try:
        return int(text)
    except ValueError:
        print('load failed for', repr(text))
        raise

try:
    load('x1')
except ValueError as err:
    print('caller got:', err)
```

```output for=reraise
load failed for 'x1'
caller got: invalid literal for int() with base 10: 'x1'
```

The inner handler logs the error and re-raises the same exception, so the caller can handle it too (rule r3). The tutorial calls this "The most common pattern for handling Exception".[^1]

```python run id=no-active
try:
    raise
except RuntimeError as err:
    print(type(err).__name__, err)
```

```output for=no-active
RuntimeError No active exception to reraise
```

A bare `raise` outside a handler has no active exception (rule r3).

```python run id=abstract
class Shape:
    def area(self):
        raise NotImplementedError('subclasses must define area')

class Square(Shape):
    def __init__(self, side):
        self.side = side
    def area(self):
        return self.side ** 2

print(Square(3).area())
try:
    Shape().area()
except NotImplementedError as err:
    print(err)
```

```output for=abstract
9
subclasses must define area
```

"In user defined base classes, abstract methods should raise this exception when they require derived classes to override the method".[^3]

## Pitfalls

`NotImplemented` is not an exception:

```python fails id=not-implemented expect=exception:TypeError
class Shape:
    def area(self):
        raise NotImplemented

Shape().area()
```

`NotImplemented` is a constant, not an exception class, so the raise statement fails with TypeError (rule r2). The docs warn: "NotImplementedError and NotImplemented are not interchangeable."[^3]

A string is not an exception:

```python fails id=string expect=exception:TypeError
raise 'disk full'
```

The expression "must be either a subclass or an instance of BaseException."[^2] Write `raise OSError('disk full')`, or use a more exact class.

The Python 2 form with a comma is a syntax error:

```python fails id=py2-raise expect=exception:SyntaxError
raise ValueError, 'bad value'
```

Pass the message to the constructor: `raise ValueError('bad value')`.

## Comparison

Go reports most errors with an error return value. Effective Go says: "The usual way to report an error to a caller is to return an error as an extra return value."[^4] For an error that the program cannot recover from, "there is a built-in function panic that in effect creates a run-time error that will stop the program".[^4] Python uses raise for all of these cases, and the caller decides which ones to handle.
