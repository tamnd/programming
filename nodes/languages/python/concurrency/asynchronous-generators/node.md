---
tag: KV41ZF
kind: feature
owner: python
slug: asynchronous-generators
name: Asynchronous generators
aliases: [async generator, asynchronous generator function, asynchronous generator iterator, async comprehension, asynchronous comprehension, asend, athrow, aclose, aclosing, PEP 525, PEP 530]
area: languages/python/concurrency
level: advanced
summary: >-
  An async def function that contains yield is an asynchronous generator function. Its object is an asynchronous iterator for async for. An async for clause makes an asynchronous comprehension.
grammar: |
  async_generator: "async" "def" funcname "(" [parameter_list] ")" ":" suite  # the suite contains a yield expression
  comp_for: ["async"] "for" target_list "in" or_test [comp_iter]
spec: {standard: null, url: "https://docs.python.org/3.15/reference/expressions.html#asynchronous-generator-functions", locator: "6.2.8.2 Asynchronous generator functions; 6.2.6.4 Asynchronous comprehensions"}
proposals: [pep-525, pep-530]
see_also: [python/async-for-and-async-with, python/coroutines-async-and-await, python/generators, python/asyncio]
sources:
  - url: https://docs.python.org/3.15/reference/expressions.html#asynchronous-generator-functions
    title: The Python Language Reference, Python 3.15
    locator: 6.2.6.4 Asynchronous comprehensions; 6.2.7 Generator expressions; 6.2.8.2 Asynchronous generator functions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0525/
    title: "PEP 525: Asynchronous Generators"
    locator: Specification; Finalization; Asynchronous Generator Object
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/contextlib.html#contextlib.aclosing
    title: contextlib — Utilities for with-statement contexts, Python 3.15 documentation
    locator: aclosing
    accessed: 2026-11-16
---

## Synopsis

```python run id=ticker
import asyncio

async def ticker(to):
    for i in range(to):
        yield i
        await asyncio.sleep(0)

async def main():
    async for n in ticker(3):
        print(n)
    print([n * n async for n in ticker(4)])

asyncio.run(main())
```

```output for=ticker
0
1
2
[0, 1, 4, 9]
```

## Semantics

**r1.** "The presence of a yield expression in a function or method defined using async def further defines the function as an asynchronous generator function."[^1]

**r2.** "When an asynchronous generator function is called, it returns an asynchronous iterator known as an asynchronous generator object." It "is typically used in an async for statement in a coroutine function analogously to how a generator object would be used in a for statement."[^1] Example: [`ticker`](#ticker).

**r3.** Each step returns an awaitable. When it is awaited, the body runs to the next `yield`. "If __anext__() is used then the result is None. Otherwise, if asend() is used, then the result will be the value passed in to that method."[^1] Example: [`asend`](#asend).

**r4.** `agen.athrow(...)` returns an awaitable "that throws an exception into the agen generator." `agen.aclose()` returns an awaitable "that throws a GeneratorExit exception into the generator."[^2]

**r5.** "It is a SyntaxError to have a non-empty return statement in an asynchronous generator."[^2] "The expression yield from <expr> is a syntax error when used in an asynchronous generator function."[^1] Example: [`return-value`](#return-value).

**r6.** If the loop stops early, the cleanup code of the generator does not run at once. "To prevent this, the caller must explicitly close the async generator by calling aclose() method to finalize the generator and ultimately detach it from the event loop."[^1] `contextlib.aclosing()` "supports deterministic cleanup of async generators when they happen to exit early by break or an exception."[^3] Example: [`aclosing`](#aclosing).

**r7.** "If a comprehension contains async for clauses, or if it contains await expressions or other asynchronous comprehensions anywhere except the iterable expression in the leftmost for clause, it is called an asynchronous comprehension." Such a comprehension is allowed in an `async def` function.[^1] Example: [`ticker`](#ticker).

## Examples

```python run id=asend
import asyncio

async def running_total():
    total = 0
    while True:
        value = yield total
        total += value

async def main():
    acc = running_total()
    print(await acc.asend(None))
    print(await acc.asend(5))
    print(await acc.asend(10))
    await acc.aclose()

asyncio.run(main())
```

```output for=asend
0
5
15
```

The first `asend()` must send `None`, because the generator has not reached a `yield` yet (rule r3).

```python run id=aclosing
import asyncio
from contextlib import aclosing

async def numbers():
    try:
        for i in range(10):
            yield i
    finally:
        print("cleanup")

async def main():
    async with aclosing(numbers()) as gen:
        async for n in gen:
            if n == 2:
                break
    print("after loop")

asyncio.run(main())
```

```output for=aclosing
cleanup
after loop
```

`aclosing()` awaits `aclose()` when the block ends, so the `finally` part runs before the next line (rule r6).

## Versions

> [!since v="3.6"]
> "Asynchronous comprehensions were introduced in PEP 530."[^1] PEP 525 added asynchronous generators in the same release.[^2]

## Pitfalls

```python wrong id=late-cleanup
import asyncio

async def numbers():
    try:
        for i in range(10):
            yield i
    finally:
        print("cleanup")

async def main():
    async for n in numbers():
        if n == 2:
            break
    print("after loop")

asyncio.run(main())
```

```output for=late-cleanup
after loop
cleanup
```

The loop stopped early, but nothing closed the generator. The event loop closed it later, so "cleanup" came after "after loop" (rule r6). Use `aclosing()`, as in [`aclosing`](#aclosing).

```python fails id=return-value expect=exception:SyntaxError
async def numbers():
    yield 1
    return 2
```

An asynchronous generator cannot return a value (rule r5). A bare `return` is allowed.

```python fails id=yield-from expect=exception:SyntaxError
async def inner():
    yield 1

async def outer():
    yield from inner()
```

`yield from` is not allowed in an asynchronous generator (rule r5). Write `async for x in inner(): yield x`.

## Comparison

An asynchronous generator mirrors a plain generator: `send()`, `throw()` and `close()` become `asend()`, `athrow()` and `aclose()`, and each returns an awaitable (rules r3 and r4). PEP 525 says the behaviour "is designed to replicate the behaviour of synchronous generators, with the only difference in that the API is asynchronous."[^2] See [[python/generators]] and [[python/async-for-and-async-with]].
