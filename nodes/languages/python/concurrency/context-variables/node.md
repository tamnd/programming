---
tag: 9DM5K0
kind: feature
owner: python
slug: context-variables
name: Context variables
aliases: [contextvars, ContextVar, copy_context, Context.run, context-local state, Token, PEP 567]
area: languages/python/concurrency
level: advanced
summary: >-
  A contextvars.ContextVar holds a value per context. Each asyncio task runs in a copy of the context that created it, so a value set in one task does not leak into another task in the same thread.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/contextvars.html", locator: "contextvars — Context Variables"}
proposals: [pep-567]
see_also: [python/asyncio, python/multi-threading, python/coroutines-async-and-await]
sources:
  - url: https://docs.python.org/3.15/library/contextvars.html
    title: contextvars — Context Variables, Python 3.15 documentation
    locator: Context Variables; Manual Context Management; asyncio support
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/asyncio-task.html#asyncio.Task
    title: Coroutines and Tasks, Python 3.15 documentation
    locator: Task; to_thread
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#threading.Thread
    title: threading — Thread-based parallelism, Python 3.15 documentation
    locator: Thread
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0567/
    title: "PEP 567: Context Variables"
    locator: Rationale; Specification
    accessed: 2026-11-16
---

## Synopsis

```python run id=per-task
import asyncio
from contextvars import ContextVar

request_id = ContextVar("request_id", default="none")

def log(message):
    print(f"[{request_id.get()}] {message}")

async def handle(rid):
    request_id.set(rid)
    await asyncio.sleep(0)
    log("done")

async def main():
    async with asyncio.TaskGroup() as tg:
        tg.create_task(handle("a"))
        tg.create_task(handle("b"))
    log("main")

asyncio.run(main())
```

```output for=per-task
[a] done
[b] done
[none] main
```

## Semantics

**r1.** `ContextVar(name, default=...)` declares a context variable. `get()` returns the value in the current context. If there is no value, it returns the `default` argument of `get()`, or else the default of the variable, or else it raises `LookupError`.[^1] Example: [`no-default`](#no-default).

**r2.** `set(value)` "Returns a Token object that can be used to restore the variable to its previous value via the ContextVar.reset() method."[^1] The token is also a context manager that calls `reset()` at the end of the block. Example: [`token`](#token).

**r3.** "Any changes to ContextVar values via the ContextVar.set() method are recorded in the current context." `Context.run(callable)` "Enters the Context, executes callable(*args, **kwargs), then exits the Context."[^1] Example: [`run-in-copy`](#run-in-copy).

**r4.** "If no context is provided, the Task copies the current context and later runs its coroutine in the copied context."[^2] So a `set()` in a task does not change the value in the code that created the task. Example: [`per-task`](#per-task).

**r5.** "Each thread has its own effective stack of Context objects."[^1] A new `threading.Thread` starts with a copy of the context of the caller of `start()` only if `sys.flags.thread_inherit_context` is true. "The flag defaults true on free-threaded builds and false otherwise."[^3] Pass `context=` to choose. Example: [`thread-context`](#thread-context).

**r6.** `asyncio.to_thread()` is different: "the current contextvars.Context is propagated, allowing context variables from the event loop thread to be accessed in the separate thread."[^2]

**r7.** "Context Variables should be created at the top module level and never in closures."[^1] A context holds strong references to its variables, so a variable made in a function is not freed.

## Examples

```python run id=token
from contextvars import ContextVar

level = ContextVar("level", default=0)

with level.set(1):
    print(level.get())
    token = level.set(2)
    print(level.get())
    level.reset(token)
    print(level.get())
print(level.get())
```

```output for=token
1
2
1
0
```

```python run id=run-in-copy
from contextvars import ContextVar, copy_context

user = ContextVar("user")
user.set("ann")
ctx = copy_context()

def change():
    user.set("bob")
    return user.get()

print(ctx.run(change))
print(user.get())
print(ctx[user])
```

```output for=run-in-copy
bob
ann
bob
```

The change happened in `ctx`, not in the current context (rule r3).

```python run id=thread-context
import threading
from contextvars import ContextVar, Context, copy_context

color = ContextVar("color", default="none")
color.set("red")

def show(label):
    print(label, color.get())

t = threading.Thread(target=show, args=("copy:",), context=copy_context())
t.start(); t.join()
t = threading.Thread(target=show, args=("empty:",), context=Context())
t.start(); t.join()
```

```output for=thread-context
copy: red
empty: none
```

With an explicit `context=`, the result is the same on both builds (rule r5).

## Versions

> [!since v="3.7"]
> The `contextvars` module was added by PEP 567, and asyncio tasks run in a copy of the context.[^1][^2]

> [!since v="3.14"]
> A token works as a context manager, and `threading.Thread` takes a `context` argument.[^1][^3]

## Pitfalls

```python fails id=no-default expect=exception:LookupError
from contextvars import ContextVar

user = ContextVar("user")
print(user.get())
```

The variable has no default and no value, so `get()` raises `LookupError` (rule r1).

```python wrong id=set-in-task
import asyncio
from contextvars import ContextVar

total = ContextVar("total", default=0)

async def add(n):
    total.set(total.get() + n)

async def main():
    await asyncio.create_task(add(5))
    print(total.get())

asyncio.run(main())
```

```output for=set-in-task
0
```

The task changed its own copy of the context, not the context of `main()` (rule r4). Return the value from the task instead.

## Comparison

`threading.local()` gives one value per thread. A context variable gives one value per context, so it also separates asyncio tasks that share one thread. PEP 567 says: "Thread-local variables are insufficient for asynchronous tasks that execute concurrently in the same OS thread."[^4] See [[python/multi-threading]] and [[python/asyncio]].
