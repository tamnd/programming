---
tag: 9YBAAK
kind: feature
owner: python
slug: asyncio
name: The asyncio module
aliases: [asyncio, event loop, asyncio.run, Task, create_task, TaskGroup, gather, asyncio.timeout, CancelledError, to_thread]
area: languages/python/concurrency
level: core
summary: >-
  asyncio runs many coroutines on one thread with an event loop. A task switches only at an await, so the loop serves I/O well but stops when a coroutine blocks.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/asyncio.html", locator: "asyncio — Asynchronous I/O"}
proposals: [pep-3156]
see_also: [python/coroutines-async-and-await, async-await, python/async-for-and-async-with, python/context-variables, python/concurrent-futures, python/multi-threading, futures]
sources:
  - url: https://docs.python.org/3.15/library/asyncio.html
    title: asyncio — Asynchronous I/O, Python 3.15 documentation
    locator: the introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/asyncio-runner.html
    title: Runners, Python 3.15 documentation
    locator: Running an asyncio Program
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/asyncio-dev.html
    title: Developing with asyncio, Python 3.15 documentation
    locator: Concurrency and Multithreading; Running Blocking Code
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/asyncio-task.html
    title: Coroutines and Tasks, Python 3.15 documentation
    locator: Creating tasks; Task cancellation; Task groups; Running Tasks Concurrently; Timeouts; Running in Threads
    accessed: 2026-11-16
---

## Synopsis

```python run id=task-group
import asyncio

async def fetch(name, seconds):
    await asyncio.sleep(seconds)
    print(name, "done")
    return name.upper()

async def main():
    async with asyncio.TaskGroup() as tg:
        slow = tg.create_task(fetch("slow", 0.3))
        fast = tg.create_task(fetch("fast", 0.1))
    print(slow.result(), fast.result())

asyncio.run(main())
```

```output for=task-group
fast done
slow done
SLOW FAST
```

The two tasks wait at the same time, so the program takes about 0.3 seconds, not 0.4.

## Semantics

**r1.** "asyncio is a library to write concurrent code using the async/await syntax." It "is often a perfect fit for IO-bound and high-level structured network code."[^1]

**r2.** `asyncio.run(coro)` will "Execute coro in an asyncio event loop and return the result." It "cannot be called when another asyncio event loop is running in the same thread", and it "should be used as a main entry point for asyncio programs".[^2]

**r3.** "An event loop runs in a thread (typically the main thread) and executes all callbacks and Tasks in its thread. While a Task is running in the event loop, no other Tasks can run in the same thread. When a Task executes an await expression, the running Task gets suspended, and the event loop executes the next Task."[^3]

**r4.** `asyncio.create_task(coro)` will "Wrap the coro coroutine into a Task and schedule its execution." "The event loop only keeps weak references to tasks", so a program must keep a reference to each task.[^4]

**r5.** A `TaskGroup` is "An asynchronous context manager holding a group of tasks." "All tasks are awaited when the context manager exits." When a task fails, "the remaining tasks in the group are cancelled", and the exceptions "are combined in an ExceptionGroup or BaseExceptionGroup".[^4] Example: [`group-failure`](#group-failure).

**r6.** `gather(*aws)` runs the awaitables concurrently. "The order of result values corresponds to the order of awaitables in aws." By default, the first exception goes to the caller, and the "Other awaitables in the aws sequence won’t be cancelled and will continue to run."[^4] Example: [`gather-order`](#gather-order).

**r7.** "When a task is cancelled, asyncio.CancelledError will be raised in the task at the next opportunity." Code should clean up in `try/finally`. `CancelledError` "directly subclasses BaseException".[^4] Example: [`cancel-task`](#cancel-task).

**r8.** `asyncio.timeout(delay)` limits a wait. When the time runs out, the context manager cancels the task and transforms the `CancelledError` "into a TimeoutError which can be caught and handled."[^4] Example: [`timeout`](#timeout).

**r9.** "Blocking (CPU-bound) code should not be called directly." `asyncio.to_thread(func, /, *args, **kwargs)` will "Asynchronously run function func in a separate thread."[^3][^4] Example: [`to-thread`](#to-thread).

## Examples

```python run id=gather-order
import asyncio

async def square(x, seconds):
    await asyncio.sleep(seconds)
    return x * x

async def main():
    print(await asyncio.gather(square(2, 0.2), square(3, 0.0), square(4, 0.1)))

asyncio.run(main())
```

```output for=gather-order
[4, 9, 16]
```

The results keep the order of the arguments, not the order in which the tasks finished (rule r6).

```python run id=group-failure
import asyncio

async def worker(name):
    try:
        await asyncio.sleep(1)
        print(name, "finished")
    except asyncio.CancelledError:
        print(name, "cancelled")
        raise

async def broken():
    await asyncio.sleep(0.1)
    raise ValueError("bad input")

async def main():
    try:
        async with asyncio.TaskGroup() as tg:
            tg.create_task(worker("w1"))
            tg.create_task(broken())
    except* ValueError as group:
        print("caught:", group.exceptions)

asyncio.run(main())
```

```output for=group-failure
w1 cancelled
caught: (ValueError('bad input'),)
```

One failed task cancels the other, and the error comes out in an exception group (rule r5).

```python run id=cancel-task
import asyncio

async def job():
    try:
        await asyncio.sleep(10)
    finally:
        print("clean up")

async def main():
    task = asyncio.create_task(job())
    await asyncio.sleep(0.1)
    task.cancel()
    try:
        await task
    except asyncio.CancelledError:
        print("cancelled:", task.cancelled())

asyncio.run(main())
```

```output for=cancel-task
clean up
cancelled: True
```

```python run id=timeout
import asyncio

async def main():
    try:
        async with asyncio.timeout(0.1):
            await asyncio.sleep(10)
    except TimeoutError:
        print("timed out")

asyncio.run(main())
```

```output for=timeout
timed out
```

```python run id=to-thread
import asyncio
import time

ticks = 0

async def ticker():
    global ticks
    while True:
        await asyncio.sleep(0.05)
        ticks += 1

def blocking_io():
    time.sleep(0.5)
    return "data"

async def main():
    tick_task = asyncio.create_task(ticker())
    print(await asyncio.to_thread(blocking_io))
    print("the loop ran meanwhile:", ticks >= 3)
    tick_task.cancel()

asyncio.run(main())
```

```output for=to-thread
data
the loop ran meanwhile: True
```

The blocking call runs in another thread, so the ticker task goes on (rule r9).

## Versions

> [!since v="3.15"]
> `TaskGroup.cancel()` ends a task group early, with no `CancelledError` out of the group.[^4]

## Pitfalls

```python wrong id=blocking-call
import asyncio
import time

ticks = 0

async def ticker():
    global ticks
    while True:
        await asyncio.sleep(0.05)
        ticks += 1

async def main():
    tick_task = asyncio.create_task(ticker())
    await asyncio.sleep(0)
    time.sleep(0.5)            # blocks the event loop
    print("ticks while blocked:", ticks)
    tick_task.cancel()

asyncio.run(main())
```

```output for=blocking-call
ticks while blocked: 0
```

`time.sleep` does not await, so no other task runs for half a second (rule r3). Use `await asyncio.sleep(...)`, or move the blocking call to a thread with `asyncio.to_thread` (rule r9).

A task with no reference "may get garbage collected at any time, even before it’s done." Keep the tasks in a collection, or use a `TaskGroup`, which "keeps a strong reference to each task, awaits them and propagates their exceptions".[^4]

A coroutine that catches `CancelledError` and does not raise it again breaks `TaskGroup` and `timeout()`, which "are implemented using cancellation internally and might misbehave if a coroutine swallows asyncio.CancelledError."[^4]

## Comparison

The interpreter can switch threads between bytecode instructions (see [[python/the-global-interpreter-lock]]). An asyncio task gives up control only at an `await` (rule r3). So no other task of the loop runs between two awaits, but one blocking call stops every task. For CPU-bound work, asyncio offers executors: `loop.run_in_executor()` can use a `ThreadPoolExecutor` or an `InterpreterPoolExecutor` "to execute blocking code in a different OS thread without blocking the OS thread that the event loop runs in."[^3] See [[python/concurrent-futures]].
