---
tag: HQTP5Q
kind: feature
owner: python
slug: errors-and-exceptions
name: Errors and Exceptions
aliases: [error handling, Python errors, exception model, termination model, try statement overview]
area: languages/python/errors
level: intro
summary: >-
  Python has two kinds of errors. A syntax error stops the parser before the code runs. An exception is raised while the code runs, goes up the call stack, and can be handled with a try statement.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/executionmodel.html#exceptions", locator: "4.3. Exceptions"}
see_also: [python/syntax-errors, python/exceptions, python/handling-exceptions, python/raising-exceptions, python/the-exception-hierarchy]
sources:
  - url: https://docs.python.org/3.15/tutorial/errors.html
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8. Errors and Exceptions; 8.1. Syntax Errors; 8.2. Exceptions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/executionmodel.html#exceptions
    title: The Python Language Reference, 4. Execution model
    locator: 4.3. Exceptions
    accessed: 2026-11-16
  - url: https://go.dev/doc/faq#exceptions
    title: Frequently Asked Questions (FAQ), The Go Programming Language
    locator: Why does Go not have exceptions?
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
def parse_age(text):
    age = int(text)
    if age < 0:
        raise ValueError(f"age cannot be negative: {age}")
    return age

for text in ["42", "abc", "-3"]:
    try:
        print("age", parse_age(text))
    except ValueError as err:
        print("error:", err)
```

```output for=basics
age 42
error: invalid literal for int() with base 10: 'abc'
error: age cannot be negative: -3
```

`int` raises ValueError for bad text, and `parse_age` raises its own ValueError for a negative number. One `except` clause handles both, and the loop continues.

## Semantics

**r1.** "There are (at least) two distinguishable kinds of errors: syntax errors and exceptions."[^1] "Syntax errors, also known as parsing errors, are perhaps the most common kind of complaint you get while you are still learning Python".[^1] Example: [`syntax`](#syntax).

**r2.** "Even if a statement or expression is syntactically correct, it may cause an error when an attempt is made to execute it. Errors detected during execution are called exceptions and are not unconditionally fatal".[^1]

**r3.** "An exception is raised at the point where the error is detected; it may be handled by the surrounding code block or by any code block that directly or indirectly invoked the code block where the error occurred."[^2] Example: [`propagate`](#propagate).

**r4.** "The Python interpreter raises an exception when it detects a run-time error (such as division by zero). A Python program can also explicitly raise an exception with the raise statement."[^2] "Exception handlers are specified with the try … except statement."[^2]

**r5.** "Python uses the “termination” model of error handling: an exception handler can find out what happened and continue execution at an outer level, but it cannot repair the cause of the error and retry the failing operation (except by re-entering the offending piece of code from the top)."[^2] Example: [`retry`](#retry).

**r6.** "When an exception is not handled at all, the interpreter terminates execution of the program, or returns to its interactive main loop. In either case, it prints a stack traceback, except when the exception is SystemExit."[^2] Example: [`unhandled`](#unhandled).

**r7.** "Exception messages are not part of the Python API. Their contents may change from one version of Python to the next without warning".[^2]

## Examples

```python run id=propagate
def read_config():
    return {"port": "80"}["host"]

def start():
    return read_config()

try:
    start()
except KeyError as err:
    print("missing key", err)
```

```output for=propagate
missing key 'host'
```

The KeyError starts two calls down and passes up through `start` to the handler (rule r3).

```python run id=retry
attempts = iter(["x", "7"])

while True:
    try:
        n = int(next(attempts))
        break
    except ValueError:
        print("not a number, again")

print(n)
```

```output for=retry
not a number, again
7
```

The handler cannot resume `int`. The loop runs the whole try clause again with the next input (rule r5).

## Pitfalls

A missing colon is a syntax error, not an exception at run time:

```python fails id=syntax expect=exception:SyntaxError
while True print("hello")
```

The tutorial uses this line as its first example. "the error is detected at the function print(), since a colon (':') is missing just before it."[^1] Write `while True:` (rule r1).

An exception that nobody handles ends the program:

```python fails id=unhandled expect=exception:ZeroDivisionError
def ratio(a, b):
    return a / b

print(ratio(1, 0))
print("not reached")
```

The traceback names ZeroDivisionError, and the last `print` does not run (rule r6).

A handler that tests the message text breaks when the message changes:

```python wrong id=message
try:
    int("abc")
except ValueError as err:
    if str(err) == "invalid literal":
        print("bad number")
    else:
        print("some other error")
```

```output for=message
some other error
```

The real message is longer than the text in the test. Catch a specific exception type instead of testing its message (rule r7).

## Comparison

The Go FAQ says: "We believe that coupling exceptions to a control structure, as in the try-catch-finally idiom, results in convoluted code."[^3] "For plain error handling, Go’s multi-value returns make it easy to report an error without overloading the return value."[^3] Go also has "a couple of built-in functions to signal and recover from truly exceptional conditions."[^3] So a Go function returns an error value next to its result, and the caller tests it. In Python, `int('abc')` raises an exception, and code that does not handle it passes it up the stack (rule r3).
