---
tag: W525ZC
kind: feature
owner: python
slug: async-for-and-async-with
name: async for and async with
aliases: [async for, async with, asynchronous iterator, asynchronous iterable, asynchronous context manager, __aiter__, __anext__, __aenter__, __aexit__, StopAsyncIteration, aiter, anext, asynccontextmanager]
area: languages/python/concurrency
level: advanced
summary: >-
  async for loops over an asynchronous iterator and awaits __anext__ for each item. async with awaits __aenter__ and __aexit__ of an asynchronous context manager. Both work only in a coroutine.
grammar: |
  async_for_stmt: "async" for_stmt
  async_with_stmt: "async" with_stmt
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#the-async-for-statement", locator: "8.9.2 The async for statement; 8.9.3 The async with statement"}
proposals: [pep-492]
see_also: [python/coroutines-async-and-await, python/asynchronous-generators, python/asyncio]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-async-for-statement
    title: The Python Language Reference, Python 3.15
    locator: 8.9.2 The async for statement; 8.9.3 The async with statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#asynchronous-iterators
    title: The Python Language Reference, Python 3.15
    locator: 3.4.3 Asynchronous Iterators; 3.4.4 Asynchronous Context Managers
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#anext
    title: Built-in Functions, Python 3.15 documentation
    locator: aiter(); anext()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/contextlib.html#contextlib.asynccontextmanager
    title: contextlib — Utilities for with-statement contexts, Python 3.15 documentation
    locator: asynccontextmanager
    accessed: 2026-11-16
---

## Synopsis

```python run id=countdown
import asyncio

class Countdown:
    def __init__(self, start):
        self.n = start

    def __aiter__(self):
        return self

    async def __anext__(self):
        if self.n == 0:
            raise StopAsyncIteration
        await asyncio.sleep(0)
        self.n -= 1
        return self.n + 1

class Session:
    async def __aenter__(self):
        print("open")
        return self

    async def __aexit__(self, exc_type, exc, tb):
        print("close")
        return False

async def main():
    async with Session():
        async for n in Countdown(3):
            print(n)

asyncio.run(main())
```

```output for=countdown
open
3
2
1
close
```

## Semantics

**r1.** "An asynchronous iterable provides an __aiter__ method that directly returns an asynchronous iterator, which can call asynchronous code in its __anext__ method."[^1]

**r2.** `__anext__` "Must return an awaitable resulting in a next value of the iterator. Should raise a StopAsyncIteration error when the iteration is over." `__aiter__()` "must return an asynchronous iterator object. Returning anything else will result in a TypeError error."[^2] Example: [`aiter-coroutine`](#aiter-coroutine).

**r3.** `async for TARGET in ITER: SUITE else: SUITE2` is like a loop that calls `iter = (ITER).__aiter__()` once and then `TARGET = await iter.__anext__()` for each item, until `StopAsyncIteration`. The `else` part runs when the loop ends that way, as in a `for` loop.[^1] Example: [`by-hand`](#by-hand).

**r4.** "An asynchronous context manager is a context manager that is able to suspend execution in its enter and exit methods."[^1] `__aenter__` and `__aexit__` are "Semantically similar" to `__enter__` and `__exit__`, "the only difference being that it must return an awaitable."[^2]

**r5.** `async with EXPRESSION as TARGET: SUITE` awaits `__aenter__()` and binds its result to `TARGET`. If the suite raises, it awaits `__aexit__(*sys.exc_info())`, and the exception is raised again unless that result is true. Otherwise it awaits `__aexit__(None, None, None)`.[^1] Example: [`suppress`](#suppress).

**r6.** "It is a SyntaxError to use an async for statement outside the body of a coroutine function." The same is true for `async with`.[^1]

**r7.** `aiter(x)` returns `x.__aiter__()`. `anext(it)` and `anext(it, default)` return an awaitable that gives the next item; with a default, the default comes back when the iterator is done.[^3] Example: [`by-hand`](#by-hand).

**r8.** `@contextlib.asynccontextmanager` makes an asynchronous context manager from an asynchronous generator function, "without needing to create a class or separate __aenter__() and __aexit__() methods."[^4] Example: [`context-decorator`](#context-decorator).

## Examples

```python run id=by-hand
import asyncio

class Letters:
    def __init__(self, text):
        self.chars = list(text)

    def __aiter__(self):
        return self

    async def __anext__(self):
        if not self.chars:
            raise StopAsyncIteration
        return self.chars.pop(0)

async def main():
    it = aiter(Letters("ab"))
    print(await anext(it))
    print(await anext(it))
    print(await anext(it, "done"))

asyncio.run(main())
```

```output for=by-hand
a
b
done
```

This is what `async for` does, one step at a time (rules r3 and r7).

```python run id=suppress
import asyncio

class Ignore:
    def __init__(self, kind):
        self.kind = kind

    async def __aenter__(self):
        return self

    async def __aexit__(self, exc_type, exc, tb):
        if exc_type is not None and issubclass(exc_type, self.kind):
            print("ignored:", exc)
            return True
        return False

async def main():
    async with Ignore(KeyError):
        {}["missing"]
    print("still running")

asyncio.run(main())
```

```output for=suppress
ignored: 'missing'
still running
```

`__aexit__` got the exception and returned `True`, so the exception stopped there (rule r5).

```python run id=context-decorator
import asyncio
from contextlib import asynccontextmanager

@asynccontextmanager
async def connection(name):
    print("connect", name)
    try:
        yield name.upper()
    finally:
        print("disconnect", name)

async def main():
    async with connection("db") as conn:
        print("using", conn)

asyncio.run(main())
```

```output for=context-decorator
connect db
using DB
disconnect db
```

## Pitfalls

```python fails id=sync-iterable expect=exception:TypeError
import asyncio

async def main():
    async for x in [1, 2, 3]:
        print(x)

asyncio.run(main())
```

A list has `__iter__`, not `__aiter__`, so `async for` cannot use it (rule r1). Use a plain `for` for a list.

```python fails id=aiter-coroutine expect=exception:TypeError
import asyncio

class Wrong:
    async def __aiter__(self):
        return self

    async def __anext__(self):
        raise StopAsyncIteration

async def main():
    async for x in Wrong():
        pass

asyncio.run(main())
```

`__aiter__` must return the iterator itself, not a coroutine. Write it with `def`, not `async def` (rule r2).

```python fails id=with-on-async expect=exception:TypeError
import asyncio
from contextlib import asynccontextmanager

@asynccontextmanager
async def resource():
    yield 1

async def main():
    with resource() as r:
        print(r)

asyncio.run(main())
```

An asynchronous context manager has `__aenter__` and `__aexit__`, not `__enter__` and `__exit__`, so a plain `with` cannot use it (rule r4).

## Comparison

`async for` and `async with` mirror `for` and `with`: the special methods have an `a` in front of their names, and the interpreter awaits them (rules r3 and r5). An asynchronous generator, written with `async def` and `yield`, is the short way to write an asynchronous iterator; see [[python/asynchronous-generators]].
