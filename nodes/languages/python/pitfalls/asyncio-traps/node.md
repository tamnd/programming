---
tag: BBP77R
kind: feature
owner: python
slug: asyncio-traps
name: asyncio traps
aliases: [blocking the event loop, time.sleep in async, coroutine was never awaited, missing await, lost task, fire and forget task, Task exception was never retrieved, RUF006, ASYNC251]
area: languages/python/pitfalls
level: advanced
summary: >-
  A blocking call in a coroutine stops all tasks. A call with no await makes a coroutine that never runs. A task with no reference can vanish. Use await, asyncio.to_thread and TaskGroup.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/asyncio-dev.html", locator: "Developing with asyncio"}
proposals: []
see_also: [python/asyncio, python/coroutines-async-and-await, python/async-for-and-async-with, python/concurrent-futures, python/testing-async-code, python/context-variables, python/raising-and-handling-multiple-unrelated-exceptions]
sources:
  - url: https://docs.python.org/3.15/library/asyncio-dev.html
    title: Developing with asyncio, Python 3.15 documentation
    locator: Debug Mode; Running Blocking Code; Detect never-awaited coroutines
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/asyncio-task.html#asyncio.create_task
    title: Coroutines and Tasks, Python 3.15 documentation
    locator: Creating Tasks; Task Groups; Running in Threads
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/asyncio-dangling-task/
    title: asyncio-dangling-task (RUF006), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/blocking-sleep-in-async-function/
    title: blocking-sleep-in-async-function (ASYNC251), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
---

## Synopsis

```python wrong id=blocking-sleep
import asyncio
import time

events = []


async def job(name, wait):
    events.append(f"{name} start")
    await wait(0.1)
    events.append(f"{name} end")


async def blocking(seconds):
    time.sleep(seconds)


async def main():
    await asyncio.gather(job("a", blocking), job("b", blocking))
    print(events)
    events.clear()
    await asyncio.gather(job("a", asyncio.sleep), job("b", asyncio.sleep))
    print(events)


asyncio.run(main())
```

```output for=blocking-sleep
['a start', 'a end', 'b start', 'b end']
['a start', 'b start', 'a end', 'b end']
```

With `time.sleep`, job `b` cannot start until job `a` ends, because `time.sleep` stops the thread of the event loop. With `asyncio.sleep`, both jobs wait at the same time.

## Semantics

**r1.** The event loop runs one coroutine at a time, in one thread. A coroutine gives control back only at an `await`. "Blocking (CPU-bound) code should not be called directly. For example, if a function performs a CPU-intensive calculation for 1 second, all concurrent asyncio Tasks and IO operations would be delayed by 1 second."[^1] A blocking call such as `time.sleep`, a file read or a request with a blocking library has the same effect. Example: [`blocking-sleep`](#blocking-sleep).

**r2.** Fix for blocking calls: use the async version, such as `asyncio.sleep`, or move the call to a thread. `asyncio.to_thread` will "Asynchronously run function func in a separate thread."[^2] Example: [`to-thread`](#to-thread).

**r3.** A call of a coroutine function only makes a coroutine object. "When a coroutine function is called, but not awaited (e.g. coro() instead of await coro()) or the coroutine is not scheduled with asyncio.create_task(), asyncio will emit a RuntimeWarning".[^1] The body never runs, and the result is a coroutine object, not the value. Examples: [`never-awaited`](#never-awaited), [`coroutine-not-value`](#coroutine-not-value).

**r4.** A task needs a reference. "Save a reference to the result of this function, to avoid a task disappearing mid-execution. The event loop only keeps weak references to tasks. A task that isn’t referenced elsewhere may get garbage collected at any time, even before it’s done."[^2] Example: [`keep-reference`](#keep-reference).

**r5.** An error in a task that nobody awaits is lost until the task is collected: "if a task fails, its exception is never retrieved and asyncio logs a “Task exception was never retrieved” message when the task is garbage collected. To avoid this, use asyncio.TaskGroup which keeps a strong reference to each task, awaits them and propagates their exceptions".[^2] Example: [`task-group`](#task-group).

**r6.** Debug mode finds some of these traps. With debug mode on, "Callbacks taking longer than 100 milliseconds are logged."[^1] Turn it on with `asyncio.run(main(), debug=True)` or with `-X dev`.

**r7.** The linters:

- Ruff RUF006 "Checks for asyncio.create_task and asyncio.ensure_future calls that do not store a reference to the returned result."[^3]
- Ruff ASYNC251 "Checks that async functions do not call time.sleep."[^4] Its reason: "Blocking an async function via a time.sleep call will block the entire event loop, preventing it from executing other tasks while waiting for the time.sleep, negating the benefits of asynchronous programming."[^4]

## Examples

```python run id=to-thread
import asyncio
import time

events = []


def slow_read():
    time.sleep(0.3)
    return "data"


async def ticker():
    await asyncio.sleep(0.1)
    events.append("tick")


async def main():
    data, _ = await asyncio.gather(asyncio.to_thread(slow_read), ticker())
    events.append(data)
    print(events)


asyncio.run(main())
```

```output for=to-thread
['tick', 'data']
```

The blocking function runs in a thread, so the ticker runs while it waits (rule r2).

```python run id=task-group
import asyncio


async def fetch(n):
    await asyncio.sleep(0.05 if n == 2 else 0.01)
    if n == 2:
        raise ValueError(f"bad item {n}")
    return n * 10


async def main():
    try:
        async with asyncio.TaskGroup() as tg:
            tasks = [tg.create_task(fetch(n)) for n in [1, 2, 3]]
    except* ValueError as group:
        print([str(e) for e in group.exceptions])
    print([t.result() for t in tasks if t.exception() is None])


asyncio.run(main())
```

```output for=task-group
['bad item 2']
[10, 30]
```

Tasks 1 and 3 end first. The task group holds each task and awaits all of them. The error reaches `main` as an `ExceptionGroup`, and the code handles it with `except*` (rule r5).

```python run id=keep-reference
import asyncio

background = set()
done = []


async def log(n):
    await asyncio.sleep(0.01)
    done.append(n)


async def main():
    for n in range(3):
        task = asyncio.create_task(log(n))
        background.add(task)
        task.add_done_callback(background.discard)
    await asyncio.sleep(0.1)
    print(sorted(done), len(background))


asyncio.run(main())
```

```output for=keep-reference
[0, 1, 2] 0
```

The set keeps each task alive, and the callback removes the task when it ends. This is the pattern in the documentation (rule r4).

## Versions

> [!since v="3.11"]
> `asyncio.TaskGroup` is new. It "is a new alternative leveraging structural concurrency; it allows for waiting for a group of related tasks with strong safety guarantees."[^2]

## Pitfalls

```python run id=never-awaited
import asyncio
import warnings

saved = []


async def save():
    saved.append("row")


async def main():
    with warnings.catch_warnings(record=True) as caught:
        warnings.simplefilter("always")
        save()
    print(saved)
    print([str(w.message) for w in caught])
    await save()
    print(saved)


asyncio.run(main())
```

```output for=never-awaited
[]
["coroutine 'save' was never awaited"]
['row']
```

The first call made a coroutine and dropped it, so the row was not saved, and Python emitted a `RuntimeWarning` (rule r3). The call with `await` runs the body.

```python fails id=coroutine-not-value expect=exception:TypeError
import asyncio


async def price():
    return 10


async def main():
    total = price() + 5
    print(total)


asyncio.run(main())
```

`price()` with no `await` is a coroutine object, not 10, so the addition raises `TypeError` (rule r3). Write `await price() + 5`.

## Comparison

In Python, `async` code runs in one thread, and the code itself decides where a task can be paused, at each `await` (rule r1). So one call that does not cooperate blocks all the tasks. For the basics, see [[python/coroutines-async-and-await]]. For the event loop and tasks, see [[python/asyncio]]. For tests of async code, see [[python/testing-async-code]].
