---
tag: AG87XE
kind: feature
owner: python
slug: handling-exceptions
name: Handling Exceptions
aliases: [try except, except clause, catching exceptions, try statement, except as, else clause of try]
area: languages/python/errors
level: intro
summary: >-
  A try statement runs its try clause, and when an exception occurs it runs the first except clause that matches the exception type. An optional else clause runs only when no exception occurred.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-try-statement", locator: "8.4. The try statement"}
see_also: [python/errors-and-exceptions, python/exceptions, python/the-exception-hierarchy, python/raising-exceptions, python/exception-chaining]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html#handling-exceptions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.3. Handling Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-try-statement
    title: The Python Language Reference, 8. Compound statements
    locator: 8.4. The try statement; 8.4.1. except clause; 8.4.3. else clause
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#KeyboardInterrupt
    title: Built-in Exceptions
    locator: KeyboardInterrupt
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#errors
    title: Effective Go
    locator: Errors
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
for text in ['12', 'twelve', '7']:
    try:
        n = int(text)
    except ValueError:
        print('not a number:', text)
    else:
        print('square:', n * n)
```

```output for=basics
square: 144
not a number: twelve
square: 49
```

When `int` raises ValueError, the except clause runs. When it does not, the else clause runs.

## Semantics

**r1.** "First, the try clause (the statement(s) between the try and except keywords) is executed. If no exception occurs, the except clause is skipped and execution of the try statement is finished."[^1] "If an exception occurs during execution of the try clause, the rest of the clause is skipped. Then, if its type matches the exception named after the except keyword, the except clause is executed, and then execution continues after the try/except block."[^1]

**r2.** "If an exception occurs which does not match the exception named in the except clause, it is passed on to outer try statements; if no handler is found, it is an unhandled exception and execution stops with an error message."[^1] "Exception handlers do not handle only exceptions that occur immediately in the try clause, but also those that occur inside functions that are called (even indirectly) in the try clause."[^1] Example: [`called`](#called).

**r3.** "A try statement may have more than one except clause, to specify handlers for different exceptions. At most one handler will be executed."[^1] The search "inspects the except clauses in turn until one is found that matches the exception."[^2] "The raised exception matches an except clause whose expression evaluates to the class or a non-virtual base class of the exception object, or to a tuple that contains such a class."[^2] Example: [`several`](#several).

**r4.** "An expression-less except clause, if present, must be last; it matches any exception."[^2] Example: [`bare`](#bare).

**r5.** "When an exception has been assigned using as target, it is cleared at the end of the except clause."[^2] "This means the exception must be assigned to a different name to be able to refer to it after the except clause."[^2] Example: [`cleared`](#cleared).

**r6.** The else clause "is executed if the control flow leaves the try suite, no exception was raised, and no return, continue, or break statement was executed. Exceptions in the else clause are not handled by the preceding except clauses."[^2] "The use of the else clause is better than adding additional code to the try clause because it avoids accidentally catching an exception that wasn’t raised by the code being protected by the try … except statement."[^1]

**r7.** "Parentheses can be dropped if multiple exception types are provided and the as clause is not used."[^2] This form is new: "Changed in version 3.14: Support for optionally dropping grouping parentheses when using multiple exception types."[^2] Before 3.14, `except A, B:` is a SyntaxError. Example: [`no-parens`](#no-parens).

**r8.** "Before an except clause’s suite is executed, the exception is stored in the sys module, where it can be accessed from within the body of the except clause by calling sys.exception()."[^2]

## Examples

```python run id=called
def ratio(a, b):
    return a / b

def report(a, b):
    return f'{ratio(a, b):.2f}'

try:
    print(report(1, 4))
    print(report(1, 0))
except ZeroDivisionError as err:
    print('Handling run-time error:', err)
```

```output for=called
0.25
Handling run-time error: division by zero
```

The exception comes from `ratio`, two calls down, and the handler around the call catches it (rule r2).

```python run id=several
def convert(value):
    try:
        return int(value)
    except (TypeError, ValueError) as err:
        return f'{type(err).__name__}: {err}'
    except OverflowError:
        return 'too large'

print(convert('5'))
print(convert(None))
print(convert('x'))
print(convert(float('inf')))
```

```output for=several
5
TypeError: int() argument must be a string, a bytes-like object or a real number, not 'NoneType'
ValueError: invalid literal for int() with base 10: 'x'
too large
```

A tuple in an except clause matches any of its classes (rule r3).

```python run id=no-parens since=3.14
try:
    int('x')
except TypeError, ValueError:
    print('bad value')
```

```output for=no-parens
bad value
```

On 3.14 and later the parentheses are optional when there is no `as` (rule r7).

```python run id=sysexc
import sys

try:
    [][0]
except IndexError:
    print(repr(sys.exception()))
print(sys.exception())
```

```output for=sysexc
IndexError('list index out of range')
None
```

Inside the handler `sys.exception()` gives the exception, and after the handler it is None (rule r8).

## Pitfalls

A bare `except:` also catches KeyboardInterrupt:

```python wrong id=bare
def work():
    raise KeyboardInterrupt  # what Control-C does

for attempt in range(3):
    try:
        work()
    except:
        print('failed, try again')
```

```output for=bare
failed, try again
failed, try again
failed, try again
```

The user presses Control-C to stop the program, and the loop goes on. A bare except matches any exception (rule r4). KeyboardInterrupt "inherits from BaseException so as to not be accidentally caught by code that catches Exception and thus prevent the interpreter from exiting."[^3] Write `except Exception:`, or name the exact types.

The name after `as` is deleted after the clause:

```python fails id=cleared expect=exception:NameError
try:
    int('x')
except ValueError as err:
    print('failed')
print(err)
```

The last line raises NameError, because Python deletes `err` at the end of the except clause (rule r5). Assign the exception to another name inside the clause if you need it later.

The Python 2 form `except ValueError, e:` does not bind `e`:

```python fails id=py2-form expect=exception:NameError since=3.14
try:
    int('x')
except ValueError, e:
    print('caught', e)
```

On 3.14 and later this parses as a clause with two types, `ValueError` and `e` (rule r7). When the ValueError occurs, Python evaluates the tuple, `e` is not defined, and a NameError replaces the ValueError. On 3.13 and older it is a SyntaxError. Write `except ValueError as e:`.

## Comparison

Go has no try statement. Effective Go says: "Library routines must often return some sort of error indication to the caller."[^4] The caller tests the returned error value with an if statement, so each call has its own check. A Python handler covers all of its try clause and the functions that it calls (rule r2).
