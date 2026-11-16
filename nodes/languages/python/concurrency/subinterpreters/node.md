---
tag: DQ1DRQ
kind: feature
owner: python
slug: subinterpreters
name: Subinterpreters
aliases: [concurrent.interpreters, multiple interpreters, per-interpreter GIL, PEP 734, PEP 684, PEP 554, create_queue, call_in_thread]
area: languages/python/concurrency
level: advanced
summary: >-
  The concurrent.interpreters module runs code in other interpreters of the same process. Each one is isolated and has its own GIL, so threads in different interpreters run Python code in parallel.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/concurrent.interpreters.html", locator: "concurrent.interpreters — Multiple interpreters in the same process"}
proposals: [pep-734, pep-684, pep-554]
since: "3.14"
see_also: [python/concurrent-futures, python/multiprocessing, python/the-global-interpreter-lock, python/multi-threading]
sources:
  - url: https://docs.python.org/3.15/library/concurrent.interpreters.html
    title: concurrent.interpreters — Multiple interpreters in the same process, Python 3.15 documentation
    locator: the introduction; Key details; Multiple Interpreters and Isolation; Running in an Interpreter; Concurrency and Parallelism; Communication Between Interpreters; “Sharing” Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/concurrent.interpreters.html#reference
    title: concurrent.interpreters — Multiple interpreters in the same process, Python 3.15 documentation
    locator: Reference; Interpreter objects; Exceptions; Communicating Between Interpreters
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0734/
    title: "PEP 734: Multiple Interpreters in the Stdlib"
    locator: Abstract
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0684/
    title: "PEP 684: A Per-Interpreter GIL"
    locator: Abstract
    accessed: 2026-11-16
---

## Synopsis

```python run id=create-call
from concurrent import interpreters

def double(x):
    return 2 * x

if __name__ == "__main__":
    interp = interpreters.create()
    print(interp.call(double, 21))
    print(interpreters.get_current().id, interp.id)
    interp.close()
```

```output for=create-call
42
0 1
```

## Semantics

**r1.** An interpreter "is effectively the execution context of the Python runtime. It contains all of the state the runtime needs to execute a program." CPython can run several in one process, and "Each interpreter is effectively isolated from the others".[^1]

**r2.** `create()` makes "a new (idle) Python interpreter". `call(callable, /, *args, **kwargs)` returns the result of the function run in that interpreter "in the current thread". `exec(code, /)` runs source code in it. `close()` finalizes and destroys it.[^2] Example: [`create-call`](#create-call).

**r3.** Code that `exec()` runs uses "the interpreter’s __main__ module as the “globals” namespace". The names of the calling interpreter are not there. `prepare_main(ns=None, **kwargs)` binds objects in that `__main__` module.[^1][^2] Example: [`isolated-main`](#isolated-main).

**r4.** An uncaught exception in the other interpreter raises `ExecutionFailed` in the caller. Its `excinfo` attribute is "A basic snapshot of the exception raised in the other interpreter."[^2]

**r5.** Objects do not move between interpreters; they are copied. "By default, most objects are copied with pickle when they are passed to another interpreter." Immutable built-in objects such as `None`, `int`, `str` and tuples of them are shared or copied cheaply. `memoryview` and `Queue` "actually share mutable data between interpreters".[^1] Example: [`copied-list`](#copied-list).

**r6.** An object that cannot be sent raises `NotShareableError`, "a subclass of TypeError".[^2]

**r7.** "interpreters do not provide any concurrency on their own." Parallel work needs threads: `call_in_thread()` runs the function "in a new thread". Because interpreters "do not share the GIL", threads in different interpreters get "full multi-core parallelism".[^1][^2] Example: [`queue-threads`](#queue-threads).

**r8.** `create_queue()` makes a cross-interpreter queue. It "implements all the methods of queue.Queue except for task_done(), join() and shutdown()."[^2]

## Examples

```python run id=isolated-main
from concurrent import interpreters

if __name__ == "__main__":
    interp = interpreters.create()
    x = 10
    try:
        interp.exec("y = x * 2")
    except interpreters.ExecutionFailed as e:
        print("ExecutionFailed:", e.excinfo.type.__name__)
    interp.prepare_main(x=10)
    interp.exec("y = x * 2")
    print(interp.call(eval, "y"))
    interp.close()
```

```output for=isolated-main
ExecutionFailed: NameError
20
```

The global `x` of the main interpreter does not exist in the new one until `prepare_main` binds it. `eval` called in the interpreter reads its `__main__` namespace (rule r3).

```python run id=copied-list
from concurrent import interpreters

def grow(items):
    items.append(4)
    return items

if __name__ == "__main__":
    interp = interpreters.create()
    items = [1, 2, 3]
    print(interp.call(grow, items))
    print(items)
    interp.close()
```

```output for=copied-list
[1, 2, 3, 4]
[1, 2, 3]
```

The other interpreter changed its own copy of the list, and the result came back as another copy (rule r5).

```python run id=queue-threads
from concurrent import interpreters

def total(queue, numbers):
    queue.put(sum(numbers))

if __name__ == "__main__":
    queue = interpreters.create_queue()
    workers = [interpreters.create() for _ in range(2)]
    threads = [w.call_in_thread(total, queue, range(n))
               for w, n in zip(workers, (10, 100))]
    for t in threads:
        t.join()
    print(sorted([queue.get(), queue.get()]))
    for w in workers:
        w.close()
```

```output for=queue-threads
[45, 4950]
```

Each thread runs in its own interpreter with its own GIL, and the queue carries the results back (rules r7 and r8).

## Versions

> [!since v="3.14"]
> The `concurrent.interpreters` module is new, from PEP 734, which proposed "a new module, interpreters, to support inspecting, creating, and running code in multiple interpreters in the current process."[^3] The per-interpreter GIL came earlier, in 3.12, through the C API (PEP 684).[^4][^1]

## Pitfalls

```python wrong id=not-shareable
import threading
from concurrent import interpreters

if __name__ == "__main__":
    interp = interpreters.create()
    try:
        interp.prepare_main(lock=threading.Lock())
    except TypeError as e:
        print(type(e).__name__)
    interp.close()
```

```output for=not-shareable
NotShareableError
```

A lock cannot cross to another interpreter (rule r6). Interpreters coordinate with a queue, not with shared locks.

A copy of a mutable object does not stay in sync. The documentation warns: "a copy must be created, which means mutable objects won’t stay in sync."[^1]

`exec()` and `call()` run in the current thread and block it. They give isolation, not speed (rule r7).

The documentation notes that "not all PyPI packages support use in multiple interpreters yet". Also, interpreters in one process are not a security boundary: "do not use multiple interpreters in security-sensitive situations, where they shouldn’t have access to each other’s data."[^1]

## Comparison

The documentation places interpreters between processes and threads: "That isolation makes them similar to processes, but they still enjoy in-process efficiency, like threads." It calls their model "a similar concurrency model to CSP or the actor model": each interpreter keeps its own state, and messages pass through queues.[^1] [[python/multiprocessing]] gives stronger isolation, with separate processes. [[python/concurrent-futures]] wraps a pool of interpreters as `InterpreterPoolExecutor`.
