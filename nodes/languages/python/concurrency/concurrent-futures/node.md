---
tag: AA2WDW
kind: feature
owner: python
slug: concurrent-futures
name: The concurrent.futures module
aliases: [concurrent.futures, ThreadPoolExecutor, ProcessPoolExecutor, InterpreterPoolExecutor, Executor, Future, as_completed, thread pool, process pool]
area: languages/python/concurrency
level: core
summary: >-
  concurrent.futures runs callables on a pool of threads, processes or interpreters behind one Executor interface. submit returns a Future that holds the result or the exception of the call.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/concurrent.futures.html", locator: "concurrent.futures — Launching parallel tasks"}
proposals: [pep-3148]
see_also: [futures, python/multi-threading, python/multiprocessing, python/subinterpreters, python/asyncio, python/the-global-interpreter-lock]
sources:
  - url: https://docs.python.org/3.15/library/concurrent.futures.html
    title: concurrent.futures — Launching parallel tasks, Python 3.15 documentation
    locator: the introduction; Executor Objects; ThreadPoolExecutor
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/concurrent.futures.html#future-objects
    title: concurrent.futures — Launching parallel tasks, Python 3.15 documentation
    locator: Future Objects; Module Functions
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/concurrent.futures.html#processpoolexecutor
    title: concurrent.futures — Launching parallel tasks, Python 3.15 documentation
    locator: InterpreterPoolExecutor; ProcessPoolExecutor
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-3148/
    title: "PEP 3148: futures - execute computations asynchronously"
    locator: Rationale; Specification
    accessed: 2026-11-16
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/ExecutorService.html
    title: ExecutorService (Java SE 25 & JDK 25)
    locator: Interface ExecutorService
    accessed: 2026-11-16
---

## Synopsis

```python run id=submit-map
from concurrent.futures import ThreadPoolExecutor

with ThreadPoolExecutor(max_workers=4) as executor:
    future = executor.submit(pow, 2, 10)
    print(future.result())
    print(list(executor.map(len, ["a", "bb", "ccc"])))
```

```output for=submit-map
1024
[1, 2, 3]
```

## Semantics

**r1.** The module "provides a high-level interface for asynchronously executing callables." The work runs on threads with `ThreadPoolExecutor` or `InterpreterPoolExecutor`, or on processes with `ProcessPoolExecutor`. "Each implements the same interface, which is defined by the abstract Executor class."[^1]

**r2.** `submit(fn, /, *args, **kwargs)` "Schedules the callable, fn, to be executed as fn(*args, **kwargs) and returns a Future object representing the execution of the callable."[^1] Example: [`submit-map`](#submit-map).

**r3.** `map(fn, *iterables)` is like the built-in `map`, but "fn is executed asynchronously and several calls to fn may be made concurrently." The results come back in the order of the inputs. "If a fn call raises an exception, then that exception will be raised when its value is retrieved from the iterator."[^1]

**r4.** `result(timeout=None)` waits for the call. "If the call raised an exception, this method will raise the same exception." `exception()` returns that exception, or `None` when the call did not raise.[^2] Example: [`future-exception`](#future-exception).

**r5.** `cancel()` stops a call that has not started. "If the call is currently being executed or finished running and cannot be cancelled then the method will return False".[^2] Example: [`cancel-pending`](#cancel-pending).

**r6.** `as_completed(fs)` "yields futures as they complete (finished or cancelled futures)". `wait(fs, return_when=...)` returns two sets, `done` and `not_done`.[^2] Example: [`as-completed`](#as-completed).

**r7.** A `with` block calls `shutdown(wait=True)` at its end, so the block waits for all pending futures. After shutdown, `submit()` and `map()` "will raise RuntimeError."[^1]

**r8.** `ProcessPoolExecutor` uses the multiprocessing module, which lets it "side-step the Global Interpreter Lock but also means that only picklable objects can be executed and returned." "The __main__ module must be importable by worker subprocesses."[^3] Example: [`process-pool`](#process-pool).

**r9.** In an `InterpreterPoolExecutor`, "Each thread runs tasks in its own interpreter." "Each interpreter has its own Global Interpreter Lock, which means code run with this executor has true multi-core parallelism." The callable, the arguments and the result cross by pickle.[^3] Example: [`interpreter-pool`](#interpreter-pool).

## Examples

```python run id=future-exception
from concurrent.futures import ThreadPoolExecutor

def divide(a, b):
    return a / b

with ThreadPoolExecutor() as executor:
    bad = executor.submit(divide, 1, 0)
    print(repr(bad.exception()))
    try:
        bad.result()
    except ZeroDivisionError as e:
        print("result() raised:", e)
```

```output for=future-exception
ZeroDivisionError('division by zero')
result() raised: division by zero
```

The exception moves from the worker thread to the thread that asks for the result (rule r4).

```python run id=cancel-pending
import threading
from concurrent.futures import ThreadPoolExecutor

started = threading.Event()
gate = threading.Event()

def blocker():
    started.set()
    gate.wait()
    return "done"

with ThreadPoolExecutor(max_workers=1) as executor:
    first = executor.submit(blocker)
    second = executor.submit(print, "never printed")
    started.wait()
    print(second.cancel())
    print(first.cancel())
    gate.set()
print(first.result(), second.cancelled())
```

```output for=cancel-pending
True
False
done True
```

The single worker is busy with `first`, so `second` waits in the queue and can be cancelled. `first` is running, so it cannot (rule r5).

```python run id=as-completed
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

def nap(name, seconds):
    time.sleep(seconds)
    return name

with ThreadPoolExecutor(max_workers=3) as executor:
    futures = [executor.submit(nap, "slow", 0.8),
               executor.submit(nap, "fast", 0.0),
               executor.submit(nap, "medium", 0.4)]
    print([f.result() for f in as_completed(futures)])
    print([f.result() for f in futures])
```

```output for=as-completed
['fast', 'medium', 'slow']
['slow', 'fast', 'medium']
```

`as_completed` gives the futures in the order they finish. The list itself keeps the order of submission.

```python run id=process-pool
from concurrent.futures import ProcessPoolExecutor

def count_primes(limit):
    return sum(all(n % d for d in range(2, int(n ** 0.5) + 1)) for n in range(2, limit))

if __name__ == "__main__":
    with ProcessPoolExecutor(max_workers=2) as executor:
        print(list(executor.map(count_primes, [10, 100, 1000])))
```

```output for=process-pool
[4, 25, 168]
```

```python run id=interpreter-pool
from concurrent.futures import InterpreterPoolExecutor

if __name__ == "__main__":
    with InterpreterPoolExecutor(max_workers=2) as executor:
        print(list(executor.map(pow, [2, 3, 4], [10, 10, 10])))
```

```output for=interpreter-pool
[1024, 59049, 1048576]
```

## Versions

> [!since v="3.14"]
> `InterpreterPoolExecutor` is new. `Executor.map` takes a `buffersize` argument "to limit the number of submitted tasks whose results have not yet been yielded".[^1][^3] The default start method of `ProcessPoolExecutor` "changed away from fork".[^3]

## Pitfalls

```python wrong id=wait-inside
from concurrent.futures import ThreadPoolExecutor, TimeoutError

def outer():
    inner = executor.submit(pow, 5, 2)
    try:
        return inner.result(timeout=0.5)
    except TimeoutError:
        return "TimeoutError: the only worker is busy with outer"

executor = ThreadPoolExecutor(max_workers=1)
print(executor.submit(outer).result())
executor.shutdown()
```

```output for=wait-inside
TimeoutError: the only worker is busy with outer
```

A task that waits for another task of the same pool can deadlock, because the task it waits for needs a worker that is busy waiting. The documentation shows this case with one worker.[^1] Without the timeout, this program would never end.

For a `ProcessPoolExecutor`, "A function defined in a REPL or a lambda should not be expected to work", because the function must be pickled (rule r8). "Calling Executor or Future methods from a callable submitted to a ProcessPoolExecutor will result in deadlock."[^3]

A future that raised keeps its exception until someone asks for the result (rule r4). A program that never calls `result()` or `exception()` never sees the error.

## Comparison

PEP 3148 says: "The proposed design of this module was heavily influenced by the Java java.util.concurrent package". "Futures are created by concrete implementations of the Executor class (called ExecutorService in Java)."[^4] In Java, an `ExecutorService` is "An Executor that provides methods to manage termination and methods that can produce a Future for tracking progress of one or more asynchronous tasks."[^5] A Python executor in a `with` block shuts down at the end of the block (rule r7). The `asyncio` module has its own `Future` class; the documentation says that `concurrent.futures.Future` "must not be confused with asyncio.Future".[^1]
