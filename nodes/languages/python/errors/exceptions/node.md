---
tag: V7V0KA
kind: feature
owner: python
slug: exceptions
name: Exceptions
aliases: [exception object, run-time error, exception args, exception message, error type, BaseException args]
area: languages/python/errors
level: intro
summary: >-
  An exception is an object that reports an error found while the code runs. Its class is the error type, and its args hold the details. str() of the exception gives the message.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#exceptions", locator: "4.3. Exceptions"}
see_also: [python/errors-and-exceptions, python/syntax-errors, python/the-exception-hierarchy, python/handling-exceptions, python/raising-exceptions]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#exceptions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.2. Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html
    title: Built-in Exceptions
    locator: introduction; Base classes, BaseException, args
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
try:
    10 * (1/0)
except ZeroDivisionError as err:
    print(type(err).__name__)
    print(err.args)
    print(str(err))
```

```output for=basics
ZeroDivisionError
('division by zero',)
division by zero
```

The division fails while the code runs, so Python raises an exception. The handler gets the exception object, and the object gives its type, its arguments and its message.

## Semantics

**r1.** "Even if a statement or expression is syntactically correct, it may cause an error when an attempt is made to execute it. Errors detected during execution are called exceptions".[^1]

**r2.** "Exceptions are identified by class instances."[^3] "In Python, all exceptions must be instances of a class that derives from BaseException."[^2] "The instance can be received by the handler and can carry additional information about the exceptional condition."[^3]

**r3.** "The last line of the error message indicates what happened. Exceptions come in different types, and the type is printed as part of the message".[^1] "The rest of the line provides detail based on the type of exception and what caused it."[^1] Example: [`kinds`](#kinds).

**r4.** `args` is "The tuple of arguments given to the exception constructor."[^2] "If str() is called on an instance of this class, the representation of the argument(s) to the instance are returned, or the empty string when there were no arguments."[^2] Example: [`args`](#args).

**r5.** "Standard exception names are built-in identifiers (not reserved keywords)."[^1] "Two exception classes that are not related via subclassing are never equivalent, even if they have the same name."[^2] Example: [`same-name`](#same-name).

**r6.** "User code can raise built-in exceptions."[^2] An exception object is an ordinary object, so code can make one, keep it in a variable and look at it before it raises it, or without raising it at all.

## Examples

```python run id=kinds
actions = [
    lambda: 10 * (1/0),
    lambda: spam * 3,
    lambda: '2' + 2,
    lambda: [1, 2, 3][5],
    lambda: {'a': 1}['b'],
]
for action in actions:
    try:
        action()
    except Exception as err:
        print(type(err).__name__ + ':', err)
```

```output for=kinds
ZeroDivisionError: division by zero
NameError: name 'spam' is not defined
TypeError: can only concatenate str (not "int") to str
IndexError: list index out of range
KeyError: 'b'
```

Each line has the same form as the last line of a traceback: the type, then the detail (rule r3).

```python run id=args
err = OSError(2, 'No such file')
print(err.args)
print(str(err))
print(str(ValueError('bad size', 42)))
print(repr(str(RuntimeError())))
```

```output for=args
(2, 'No such file')
[Errno 2] No such file
('bad size', 42)
''
```

The code makes the objects but does not raise them (rule r6). With two arguments, `str` of a ValueError shows the tuple, and with none it is empty (rule r4). OSError is one of the classes that "expect a certain number of arguments and assign a special meaning to the elements of this tuple".[^2]

## Pitfalls

A class with the name of a built-in exception is a different class:

```python fails id=same-name expect=exception:ValueError
class ValueError(Exception):
    pass

try:
    int('abc')
except ValueError:
    print('not a number')
```

The class statement binds the name `ValueError` in the module to a new class, because the name is not a keyword. `int` raises the built-in ValueError, which is not related to the new class, so the except clause does not match (rule r5). Do not reuse the names of built-in exceptions.

An exception made with no arguments has an empty message:

```python wrong id=empty
try:
    raise RuntimeError()
except RuntimeError as err:
    print('error: [' + str(err) + ']')
    print('error:', repr(err))
```

```output for=empty
error: []
error: RuntimeError()
```

A log line that shows only `str(err)` loses the type, and it is blank when the args are empty (rule r4). Show the type too, or use `repr`.

## Comparison

In Go an error is a value: "The predeclared type error is defined as" an interface with one method, `Error() string`, and "It is the conventional interface for representing an error condition, with the nil value representing no error."[^4] A Go function returns the error as a result. In Python the error is an instance of an exception class (rule r2), and it is raised, not returned. Go has a second form for errors in execution: "Execution errors such as attempting to index an array out of bounds trigger a run-time panic".[^4] The Python IndexError in [`kinds`](#kinds) is an ordinary exception that a handler can catch.
