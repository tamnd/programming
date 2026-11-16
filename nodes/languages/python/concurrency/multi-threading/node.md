---
tag: FHK4YS
kind: feature
owner: python
slug: multi-threading
name: Multi-threading
aliases: [threading module, threading.Thread, start a thread, join a thread, background thread, daemon thread, Python threads]
area: languages/python/concurrency
level: core
summary: >-
  The threading module runs functions in threads of one process. start() begins a thread, join() waits for it, and a daemon thread does not keep the program alive.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/threading.html#thread-objects", locator: "threading, Thread objects"}
see_also: [python/thread-synchronization, python/the-queue-module, python/the-global-interpreter-lock, python/concurrent-futures, python/multiprocessing, threads]
sources:
  - url: https://docs.python.org/3.15/library/threading.html#gil-and-performance-considerations
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Introduction; GIL and performance considerations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#thread-objects
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Thread objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#threading.excepthook
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: threading.excepthook
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#multi-threading
    title: The Python Tutorial, 11. Brief Tour of the Standard Library, Part II
    locator: 11.4. Multi-threading
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Go_statements
    title: The Go Programming Language Specification
    locator: Go statements; Program execution
    accessed: 2026-11-16
---

## Synopsis

```python run id=start-join
import threading

squares = {}

def square(n):
    squares[n] = n * n

threads = [threading.Thread(target=square, args=(n,)) for n in range(5)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(sorted(squares.items()))
```

```output for=start-join
[(0, 0), (1, 1), (2, 4), (3, 9), (4, 16)]
```

Each `Thread` calls `square` with its own argument. The program starts all the threads first, and then it waits for each one with `join()`.

## Semantics

**r1.** "The threading module provides a way to run multiple threads (smaller units of a process) concurrently within a single process."[^1] The threads share the memory of the process, so all of them see the same objects.

**r2.** "Once a thread object is created, its activity must be started by calling the thread’s start() method. This invokes the run() method in a separate thread of control."[^2] The standard `run()` calls the `target` callable with `args` and `kwargs`. A subclass can override `run()` instead. Example: [`subclass`](#subclass).

**r3.** `start()` "must be called at most once per thread object." A second call raises `RuntimeError`.[^2] Example: [`start-twice`](#start-twice).

**r4.** `join()` "blocks the calling thread until the thread whose join() method is called terminates – either normally or through an unhandled exception – or until the optional timeout occurs." Because "join() always returns None, you must call is_alive() after join() to decide whether a timeout happened".[^2] Example: [`join-timeout`](#join-timeout).

**r5.** A daemon thread does not keep the program alive: "The entire Python program exits when no alive non-daemon threads are left." The flag "must be set before start() is called", and its initial value comes from the creating thread.[^2] Example: [`daemon`](#daemon).

**r6.** An exception that leaves `run()` does not reach the thread that calls `join()`. `threading.excepthook()` handles it. The default hook prints it on `sys.stderr`, and it ignores `SystemExit`.[^3] Example: [`excepthook`](#excepthook).

**r7.** By default, the name of a thread has the form "Thread-N", or "Thread-N (target)" when the constructor gets a target.[^2] The name has no meaning for the program.

**r8.** In CPython with the global interpreter lock, "only one thread can execute Python code at once". Threads are thus a good fit for I/O-bound work, and processes are the tool for CPU-bound work.[^1]

## Examples

```python run id=subclass
import threading

class Summer(threading.Thread):
    def __init__(self, n):
        super().__init__(name=f"summer-{n}")
        self.n = n
        self.total = 0

    def run(self):
        self.total = sum(range(self.n))

t = Summer(1000)
t.start()
t.join()
print(t.name, t.total, t.is_alive())
```

```output for=subclass
summer-1000 499500 False
```

The subclass calls the base constructor first, as the documentation asks.[^2] The thread stores its result in an attribute, and the main thread reads it after `join()` (rule r2). The tutorial uses the same shape for a thread that writes a zip file in the background.[^4]

```python run id=join-timeout
import threading

release = threading.Event()
t = threading.Thread(target=release.wait)
t.start()

t.join(timeout=0.1)
print("alive after the timeout:", t.is_alive())

release.set()
t.join()
print("alive after the event:", t.is_alive())
```

```output for=join-timeout
alive after the timeout: True
alive after the event: False
```

The first `join()` returns after 0.1 seconds, and the thread still runs. Only `is_alive()` tells the two cases apart (rule r4).

```python run id=daemon
import threading
import time

def forever():
    while True:
        time.sleep(1)

t = threading.Thread(target=forever, daemon=True)
t.start()
print(t.daemon, threading.main_thread().daemon)
print("the main thread ends here")
```

```output for=daemon
True False
the main thread ends here
```

The loop never ends, but the program ends with the main thread, because the only other thread is a daemon (rule r5).

```python run id=excepthook
import threading

def fail():
    raise ValueError("bad input")

def report(args):
    print("caught in", args.thread.name, repr(args.exc_value))

threading.excepthook = report
t = threading.Thread(target=fail, name="worker")
t.start()
t.join()
print("join returned normally")
```

```output for=excepthook
caught in worker ValueError('bad input')
join returned normally
```

The hook gets the exception, and `join()` returns as usual (rule r6).

```python run id=default-names
import threading

def work():
    pass

a = threading.Thread(target=work)
b = threading.Thread(target=work, name="loader")
print(a.name.endswith("(work)"), b.name)
```

```output for=default-names
True loader
```

The number N in "Thread-N" depends on how many threads the program made before, so the example tests only the end of the name (rule r7).

## Versions

> [!since v="3.14"]
> The `Thread` constructor has a `context` parameter, the `contextvars.Context` that the new thread starts with. `start()` also sets the name of the operating system thread.[^2]

## Pitfalls

`run()` is not `start()`. A call to `run()` runs the target in the calling thread:

```python wrong id=run-not-start
import threading

def where():
    print(threading.current_thread().name)

t = threading.Thread(target=where, name="worker")
t.run()
```

```output for=run-not-start
MainThread
```

Call `start()`, and the target runs in a new thread named "worker" (rule r2).

A thread object starts only once:

```python fails id=start-twice expect=exception:RuntimeError
import threading

t = threading.Thread(target=print, args=("once",))
t.start()
t.join()
t.start()
```

To run the work again, make a new `Thread` (rule r3).

An exception in a thread is not raised again by `join()` (rule r6). The program goes on as if the work succeeded. To get the result or the exception of the work, use a [[python/concurrent-futures]] executor, whose `Future.result()` raises the exception of the call.

## Comparison

A Go program starts a goroutine with a `go` statement, which "starts the execution of a function call as an independent concurrent thread of control, or goroutine, within the same address space."[^5] Go has no `join()`: a program waits with a channel or a `sync.WaitGroup`. When `main` returns, "the program exits. It does not wait for other (non-main) goroutines to complete."[^5] So each goroutine acts like a Python daemon thread, and a Python non-daemon thread keeps the program alive until it ends.
