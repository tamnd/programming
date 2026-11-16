---
tag: 7DGH9M
kind: feature
owner: python
slug: finally-and-control-flow
name: finally and control flow
aliases: [return in finally, break in finally, continue in finally, PEP 765, finally swallows exception]
area: languages/python/errors
level: advanced
summary: >-
  A return, break or continue in a try clause runs the finally clause on the way out. The same statements in a finally clause discard a pending exception, and from 3.14 the compiler warns about them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#finally-clause", locator: "8.4.4. finally clause"}
see_also: [python/clean-up-actions-with-finally, python/handling-exceptions, python/warnings]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#finally-clause
    title: The Python Language Reference, 8. Compound statements
    locator: 8.4.4. finally clause
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/errors.html#defining-clean-up-actions
    title: The Python Tutorial, 8. Errors and Exceptions
    locator: 8.7. Defining Clean-up Actions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0765/
    title: PEP 765, Disallow return/break/continue that exit a finally block
    locator: Abstract; Specification
    accessed: 2026-11-16
  - url: https://go.dev/doc/effective_go#defer
    title: Effective Go
    locator: Defer
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Defer_statements
    title: The Go Programming Language Specification
    locator: Defer statements
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
def first_even(numbers):
    for n in numbers:
        try:
            if n % 2 == 0:
                return n
        finally:
            print('checked', n)
    return None

print(first_even([3, 5, 8, 9]))
```

```output for=basics
checked 3
checked 5
checked 8
8
```

The return statement for 8 leaves the try clause, and the finally clause runs before the function returns.

## Semantics

**r1.** "When a return, break or continue statement is executed in the try suite of a try…finally statement, the finally clause is also executed ‘on the way out.’"[^1] "If the try statement reaches a break, continue or return statement, the finally clause will execute just prior to the break, continue or return statement’s execution."[^2]

**r2.** "The return value of a function is determined by the last return statement executed. Since the finally clause always executes, a return statement executed in the finally clause will always be the last one executed."[^1] Example: [`override`](#override).

**r3.** "If the finally clause executes a return, break or continue statement, the saved exception is discarded."[^1] The tutorial says: "This can be confusing and is therefore discouraged."[^2] Example: [`swallow`](#swallow).

**r4.** "Changed in version 3.14: The compiler emits a SyntaxWarning when a return, break or continue appears in a finally block (see PEP 765)."[^1] The PEP says that "Python’s compiler may emit a SyntaxWarning or SyntaxError when a return, break or continue would transfer control flow from within a finally block to a location outside of it."[^3] A loop or a function that is wholly inside the finally clause does not count. Example: [`warning`](#warning).

## Examples

```python run id=loop
for attempt in range(1, 4):
    try:
        if attempt == 1:
            continue
        print('work', attempt)
        if attempt == 2:
            break
    finally:
        print('end of attempt', attempt)
```

```output for=loop
end of attempt 1
work 2
end of attempt 2
```

The finally clause runs for `continue` in the first pass and for `break` in the second (rule r1).

```python run id=value
def snapshot():
    count = 1
    try:
        return count
    finally:
        count = 2
        print('finally sets count to', count)

print(snapshot())
```

```output for=value
finally sets count to 2
1
```

The return statement evaluates `count` before the finally clause runs, so the new binding does not change the result.

```python run id=warning since=3.14
import warnings

source = '''
def f():
    try:
        return 'try'
    finally:
        return 'finally'
'''
with warnings.catch_warnings(record=True) as caught:
    warnings.simplefilter('always')
    compile(source, 'demo.py', 'exec')
for w in caught:
    print(w.category.__name__, w.lineno, w.message)
```

```output for=warning
SyntaxWarning 6 'return' in a 'finally' block
```

The compiler gives the warning when it compiles the code, before it runs (rule r4).

## Pitfalls

A return in a finally clause replaces the result:

```python wrong id=override
def is_ready():
    try:
        return True
    finally:
        return False

print(is_ready())
```

```output for=override
False
```

The return in the finally clause runs last, so the function gives False (rule r2). On 3.14 and later the compiler warns about this code (rule r4).

A return in a finally clause also discards an exception:

```python wrong id=swallow
def load(path):
    try:
        raise FileNotFoundError(path)
    finally:
        return 'default'

print(load('settings.toml'))
```

```output for=swallow
default
```

The FileNotFoundError is lost, and the caller does not know that the load failed (rule r3). Put the return after the try statement, or handle the exception in an except clause.

## Comparison

In Go a deferred function runs "immediately before the function executing the defer returns",[^4] so `return` in the body runs deferred calls on the way out, like rule r1. The Go spec says that "deferred functions are executed after any result parameters are set by that return statement but before the function returns to its caller."[^5] With named results, "the deferred function may access and modify the result parameters before they are returned."[^5] So a deferred call can change the result, like a return in a finally clause (rule r2).
