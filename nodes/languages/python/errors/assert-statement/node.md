---
tag: 5PDDQG
kind: feature
owner: python
slug: assert-statement
name: The assert statement
aliases: [assert, AssertionError, debugging assertion, __debug__, python -O]
area: languages/python/errors
level: core
summary: >-
  assert expression raises AssertionError when the expression is false, with an optional second expression as the message. Python removes assert statements when it runs with -O.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-assert-statement", locator: "7.3. The assert statement"}
see_also: [python/raising-exceptions, python/the-exception-hierarchy, python/warnings]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-assert-statement
    title: The Python Language Reference, 7. Simple statements
    locator: 7.3. The assert statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/exceptions.html#AssertionError
    title: Built-in Exceptions
    locator: AssertionError
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/using/cmdline.html#cmdoption-O
    title: Command line and environment
    locator: -O
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#assertions
    title: Go FAQ
    locator: Why does Go not have assertions?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
def mean(values):
    assert len(values) > 0, 'mean of an empty list'
    return sum(values) / len(values)

print(mean([2, 4, 9]))
try:
    mean([])
except AssertionError as err:
    print('AssertionError:', err)
```

```output for=basics
5.0
AssertionError: mean of an empty list
```

The assert statement checks a condition that the code needs. When the condition is false, it raises AssertionError with the second expression as the argument.

## Semantics

**r1.** "Assert statements are a convenient way to insert debugging assertions into a program".[^1] The grammar is `assert_stmt: "assert" expression ["," expression]`.[^1]

**r2.** "The simple form, assert expression, is equivalent to" `if __debug__: if not expression: raise AssertionError`.[^1] The extended form `assert expression1, expression2` raises `AssertionError(expression2)`.[^1] Example: [`forms`](#forms).

**r3.** "the built-in variable __debug__ is True under normal circumstances, False when optimization is requested (command line option -O). The current code generator emits no code for an assert statement when optimization is requested at compile time."[^1] The option -O will "Remove assert statements and any code conditional on the value of __debug__."[^3] Example: [`optimize`](#optimize).

**r4.** "Note that it is unnecessary to include the source code for the expression that failed in the error message; it will be displayed as part of the stack trace."[^1]

**r5.** "Assignments to __debug__ are illegal. The value for the built-in variable is determined when the interpreter starts."[^1] Example: [`debug-assign`](#debug-assign).

## Examples

```python run id=forms
for check in [lambda: 0, lambda: [], lambda: None]:
    try:
        assert check()
    except AssertionError as err:
        print(repr(err))

x = 7
try:
    assert x % 2 == 0, f'{x} is odd'
except AssertionError as err:
    print(repr(err))
```

```output for=forms
AssertionError()
AssertionError()
AssertionError()
AssertionError('7 is odd')
```

The condition is tested for truth, so 0, an empty list and None all fail. The simple form gives an AssertionError with no arguments (rule r2).

```python run id=optimize
import subprocess
import sys

code = "assert 1 == 2, 'checked'\nprint('debug is', __debug__)"
for flags in [[], ['-O']]:
    result = subprocess.run([sys.executable, *flags, '-c', code], capture_output=True, text=True)
    lines = (result.stdout + result.stderr).strip().splitlines()
    print(flags, lines[-1])
```

```output for=optimize
[] AssertionError: checked
['-O'] debug is False
```

With -O the assert statement is gone, so the program goes on to the print (rule r3).

## Pitfalls

Do not use assert to check input:

```python wrong id=input
import subprocess
import sys

code = '''
def withdraw(balance, amount):
    assert amount > 0, 'amount must be positive'
    return balance - amount
print(withdraw(100, -50))
'''
result = subprocess.run([sys.executable, '-O', '-c', code], capture_output=True, text=True)
print(result.stdout, end='')
```

```output for=input
150
```

Under -O there is no check, and the negative amount adds money (rule r3). Raise an exception for bad input, for example `if amount <= 0: raise ValueError('amount must be positive')`.

Parentheses around both parts make a tuple:

```python wrong id=tuple
x = 5
assert (x > 10, 'x is too small')
print('passed')
```

```output for=tuple
passed
```

The expression is a tuple with two items, and a non-empty tuple is true, so the assert never fails. CPython warns about this line when it compiles it:

```python run id=tuple-warning
import warnings

with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    compile("assert (x > 10, 'x is too small')", 'demo.py', 'exec')
print(caught[0].category.__name__ + ':', caught[0].message)
```

```output for=tuple-warning
SyntaxWarning: assertion is always true, perhaps remove parentheses?
```

Write `assert x > 10, 'x is too small'`, with no parentheses around both parts (rule r1).

`__debug__` cannot be assigned:

```python fails id=debug-assign expect=exception:SyntaxError
__debug__ = False
```

The value is fixed when the interpreter starts (rule r5). Use -O to make it False.

## Comparison

"Go doesn’t provide assertions."[^4] The Go FAQ says that "programmers use them as a crutch to avoid thinking about proper error handling and reporting."[^4] A Go program tests the condition with an if statement and returns an error or panics. In Python, assert is for checks that may be removed, and an if statement with raise is for checks that must always run (rule r3).
