---
tag: KK2VXS
kind: feature
owner: python
slug: coroutines-async-and-await
name: Coroutines with async and await
aliases: [async def, await, coroutine, coroutine function, awaitable, __await__, native coroutine, PEP 492]
area: languages/python/concurrency
level: core
summary: >-
  async def defines a coroutine function. A call makes a coroutine object and runs nothing; await suspends the coroutine until an awaitable is done. An event loop such as asyncio drives coroutines.
grammar: |
  async_funcdef: [decorators] "async" "def" funcname "(" [parameter_list] ")" ["->" expression] ":" suite
  await_expr: "await" primary
spec: {standard: null, url: "https://docs.python.org/3.15/reference/compound_stmts.html#coroutine-function-definition", locator: "8.9.1 Coroutine function definition"}
proposals: [pep-492]
see_also: [async-await, python/asyncio, python/generators, python/async-for-and-async-with, python/asynchronous-generators]
sources:
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#coroutine-function-definition
    title: The Python Language Reference, Python 3.15
    locator: 8.9 Coroutines; 8.9.1 Coroutine function definition
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/asyncio-task.html#coroutines
    title: Coroutines and Tasks, Python 3.15 documentation
    locator: Coroutines
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/expressions.html#await-expression
    title: The Python Language Reference, Python 3.15
    locator: 6.4 Await expression
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0492/
    title: "PEP 492: Coroutines with async and await syntax"
    locator: Specification; Why “async” and “await” keywords
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#coroutines
    title: The Python Language Reference, Python 3.15
    locator: 3.4.1 Awaitable Objects; 3.4.2 Coroutine Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/asyncio-dev.html#detect-never-awaited-coroutines
    title: Developing with asyncio, Python 3.15 documentation
    locator: Detect never-awaited coroutines
    accessed: 2026-11-16
---

## Synopsis

```python run id=run-coroutine
import asyncio

async def greet(name):
    await asyncio.sleep(0)
    return f"hello, {name}"

print(asyncio.run(greet("Ada")))
```

```output for=run-coroutine
hello, Ada
```

## Semantics

**r1.** "Functions defined with async def syntax are always coroutine functions, even if they do not contain await or async keywords."[^1] Example: [`call-makes-object`](#call-makes-object).

**r2.** Calling a coroutine function returns a coroutine object and runs none of its body. "Note that simply calling a coroutine will not schedule it to be executed". `asyncio.run()`, an `await`, or a task runs it.[^2]

**r3.** `await` will "Suspend the execution of coroutine on an awaitable object. Can only be used inside a coroutine function."[^3] "It is a SyntaxError to use await outside of an async def function".[^4] "It is a SyntaxError to use a yield from expression inside the body of a coroutine function."[^1] Example: [`await-outside`](#await-outside).

**r4.** An awaitable "generally implements an __await__() method" that "Must return an iterator". "Coroutine objects returned from async def functions are awaitable."[^5] Example: [`custom-awaitable`](#custom-awaitable).

**r5.** A coroutine runs as its `__await__()` iterator is advanced. "When the coroutine has finished executing and returns, the iterator raises StopIteration, and the exception’s value attribute holds the return value."[^5]

**r6.** `coroutine.send(value)` "Starts or resumes execution of the coroutine." `throw()` raises an exception at the point where it is suspended, and `close()` makes it "clean itself up and exit".[^5]

**r7.** "unlike generators, coroutines do not directly support iteration." "It is a RuntimeError to await on a coroutine more than once."[^5] Example: [`await-twice`](#await-twice).

**r8.** "The language doesn’t place any restriction on the type or value of the objects yielded by the iterator returned by __await__". Their meaning belongs to the framework that drives the coroutines, such as asyncio.[^5]

## Examples

```python run id=call-makes-object
import inspect

async def fetch():
    print("running")
    return 1

print(inspect.iscoroutinefunction(fetch))
coro = fetch()
print(type(coro).__name__)
coro.close()
```

```output for=call-makes-object
True
coroutine
```

The call made a coroutine object, but "running" is never printed (rule r2). `close()` marks the coroutine as finished, so Python gives no warning about it.

```python run id=custom-awaitable
class Ticket:
    def __await__(self):
        value = yield "waiting for a value"
        return value

async def use():
    got = await Ticket()
    return got * 2

coro = use()
print(coro.send(None))
try:
    coro.send(21)
except StopIteration as stop:
    print("returned", stop.value)
```

```output for=custom-awaitable
waiting for a value
returned 42
```

This program drives a coroutine by hand, as an event loop does. The first `send(None)` runs `use` up to the `yield` inside `Ticket.__await__`, and the yielded value comes out. The second `send` resumes the coroutine with 21, and the return value arrives in `StopIteration.value` (rules r4 to r6).

```python run id=sequential-awaits
import asyncio

async def step(name):
    print("start", name)
    await asyncio.sleep(0)
    print("end", name)

async def main():
    await step("a")
    await step("b")

asyncio.run(main())
```

```output for=sequential-awaits
start a
end a
start b
end b
```

Each `await` waits until its coroutine is done, so two awaits in a row run one after the other. To run coroutines at the same time, make tasks; see [[python/asyncio]].

## Pitfalls

```python fails id=await-twice expect=exception:RuntimeError
import asyncio

async def value():
    return 1

async def main():
    coro = value()
    print(await coro)
    print(await coro)

asyncio.run(main())
```

A coroutine object runs once (rule r7). To run the work again, call the coroutine function again.

```python fails id=await-outside expect=exception:SyntaxError
import asyncio

def main():
    await asyncio.sleep(0)
```

`await` is allowed only in a coroutine function (rule r3). The fix is `async def main():`.

```python wrong id=forgot-await
import asyncio

async def total():
    return 42

async def main():
    result = total()          # no await
    print(type(result).__name__)
    result.close()

asyncio.run(main())
```

```output for=forgot-await
coroutine
```

Without `await`, `result` is a coroutine object, not 42. If such an object is never awaited, asyncio warns: "RuntimeWarning: coroutine 'test' was never awaited", for a coroutine function named `test`. "The usual fix is to either await the coroutine or call the asyncio.create_task() function".[^6]

## Comparison

Coroutines have `send()`, `throw()` and `close()`, which are "analogous to those of generators", but a coroutine is not an iterator (rule r7).[^5] PEP 492 chose the words `async` and `await` because "async/await is not a new concept in programming languages". It names C#, a proposal for ECMAScript 7, Hack, Dart and Scala, and says that the shared syntax "makes working with many languages in one project easier".[^4] In Python, the language gives only the syntax and the protocol; the event loop comes from a library (rule r8).
