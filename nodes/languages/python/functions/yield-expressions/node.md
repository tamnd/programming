---
tag: ENDCZE
kind: feature
owner: python
slug: yield-expressions
name: Yield expressions, send, throw, close and yield from
aliases: [yield expression, yield from, generator send, generator throw, generator close, GeneratorExit, StopIteration value, subgenerator, delegating generator]
area: languages/python/functions
level: advanced
summary: >-
  yield is an expression. Its value is None after next() and the sent value after send(). throw() and close() raise an exception at the paused yield, and yield from passes control to a subiterator.
grammar: 'yield_expression: "yield" yield_list | yield_from; yield_from: "yield" "from" expression'
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#yield-expressions", locator: "6.2.8. Yield expressions"}
see_also: [python/generators, python/generator-expressions, python/return-statement, python/defining-functions]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#yield-expressions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.8. Yield expressions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#generator-methods
    title: Built-in Types, Python 3.15 documentation
    locator: Generator iterators
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0342/
    title: "PEP 342: Coroutines via Enhanced Generators"
    locator: Specification Summary
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0380/
    title: "PEP 380: Syntax for Delegating to a Subgenerator"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-return-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.6. The return statement
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=averager
def averager():
    total = 0
    count = 0
    average = None
    while True:
        value = yield average
        total += value
        count += 1
        average = total / count

avg = averager()
next(avg)
print(avg.send(10))
print(avg.send(20))
print(avg.send(60))
```

```output for=averager
10.0
15.0
30.0
```

The first `next()` runs the body to the first `yield`. Each `send()` gives a value to the paused `yield` and returns the next average.

## Semantics

**r1.** "The yield expression is used when defining a generator function or an asynchronous generator function and thus can only be used in the body of a function definition."[^1] Example: [`outside-function`](#outside-function).

**r2.** "The value of the yield expression after resuming depends on the method which resumed the execution. If __next__() is used (typically via either a for or the next() builtin) then the result is None. Otherwise, if send() is used, then the result will be the value passed in to that method."[^1] Example: [`value-of-yield`](#value-of-yield).

**r3.** "When send() is called to start the generator, it must be called with None as the argument, because there is no current yield expression that could receive the value."[^2] Example: [`send-first`](#send-first).

**r4.** throw() "Raises an exception at the point where the generator is currently suspended."[^2] "If the generator function does not catch the passed-in exception, or raises a different exception, then that exception propagates to the caller."[^2] Example: [`throw`](#throw).

**r5.** close() "Raises a GeneratorExit exception at the point where the generator function is currently suspended (equivalent to calling throw(GeneratorExit))."[^2] "If the generator yields a value, a RuntimeError is raised."[^2] PEP 342 allows "yield to be used in try/finally blocks, since garbage collection or an explicit close() call would now allow the finally clause to execute."[^3] Example: [`close`](#close).

**r6.** "When yield from <expr> is used, the supplied expression must be an iterable. The values produced by iterating that iterable are passed directly to the caller of the current generator’s methods."[^1] PEP 380 says: "the subgenerator is allowed to return with a value, and the value is made available to the delegating generator."[^4] Example: [`yield-from`](#yield-from).

**r7.** "In a generator function, the return statement indicates that the generator is done and will cause StopIteration to be raised. The returned value (if any) is used as an argument to construct StopIteration and becomes the StopIteration.value attribute."[^5] Example: [`return-value`](#return-value).

## Examples

```python run id=value-of-yield
def show():
    while True:
        received = yield
        print("received", received)

g = show()
next(g)
next(g)
g.send("hello")
```

```output for=value-of-yield
received None
received hello
```

`next()` makes the paused `yield` give `None`. `send()` makes it give the sent value (rule r2).

```python run id=throw
def worker():
    while True:
        try:
            yield "ready"
        except ValueError as e:
            print("handled", e)

w = worker()
print(next(w))
print(w.throw(ValueError("bad input")))
try:
    w.throw(KeyError("other"))
except KeyError as e:
    print("caller got", repr(e))
```

```output for=throw
ready
handled bad input
ready
caller got KeyError('other')
```

The generator catches the ValueError and gives the next value. It does not catch the KeyError, so the caller gets it (rule r4).

```python run id=close
def reader():
    try:
        while True:
            yield "line"
    finally:
        print("cleanup")

r = reader()
print(next(r))
r.close()
print(next(r, "closed"))
```

```output for=close
line
cleanup
closed
```

`close()` raises GeneratorExit at the paused `yield`, so the `finally` clause runs (rule r5).

```python run id=yield-from
def inner():
    yield 1
    yield 2
    return "inner done"

def outer():
    result = yield from inner()
    print("inner returned", result)
    yield 3

print(list(outer()))
```

```output for=yield-from
inner returned inner done
[1, 2, 3]
```

The values of `inner` go straight to the caller of `outer`, and the return value of `inner` becomes the value of `yield from` (rule r6).

```python run id=return-value
def gen():
    yield 1
    return 99

g = gen()
next(g)
try:
    next(g)
except StopIteration as stop:
    print("value", stop.value)
```

```output for=return-value
value 99
```

## Pitfalls

The first call must be `next()` or `send(None)`:

```python fails id=send-first expect=exception:TypeError
def echo():
    while True:
        value = yield
        print(value)

e = echo()
e.send("hi")
```

The call raises TypeError, because no `yield` is paused yet to receive the value (rule r3).

A yield outside a function body is a syntax error:

```python fails id=outside-function expect=exception:SyntaxError
yield 1
```

A generator that yields again after GeneratorExit makes `close()` raise RuntimeError (rule r5). Do not catch GeneratorExit in a loop that continues to yield.

## Comparison

Go has no `send` or `throw` on its iterators. In a range-over-function loop, the iterator function calls `yield`, and "If the loop body terminates (such as by a break statement), yield returns false and must not be called again."[^6] So the only value that goes back to a Go iterator is a bool. A Python program can send any value into a paused generator with `send()` (rule r2).
