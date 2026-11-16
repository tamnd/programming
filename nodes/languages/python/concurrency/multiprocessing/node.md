---
tag: Q2K7C2
kind: feature
owner: python
slug: multiprocessing
name: The multiprocessing module
aliases: [multiprocessing, Process, Pool, start method, fork, spawn, forkserver, process-based parallelism, Manager, shared memory]
area: languages/python/concurrency
level: core
summary: >-
  The multiprocessing module runs Python code in other processes. Each process has its own interpreter and its own GIL, so CPU-bound work runs in parallel. Arguments and results cross by pickling.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/multiprocessing.html", locator: "multiprocessing — Process-based parallelism"}
see_also: [python/concurrent-futures, python/multi-threading, python/the-global-interpreter-lock, python/atomicity-and-the-memory-model]
sources:
  - url: https://docs.python.org/3.15/library/multiprocessing.html
    title: multiprocessing — Process-based parallelism, Python 3.15 documentation
    locator: Introduction; The Process class; Contexts and start methods; Exchanging objects between processes; Sharing state between processes
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/multiprocessing.html#reference
    title: multiprocessing — Process-based parallelism, Python 3.15 documentation
    locator: Reference; Process and exceptions; exitcode; Process Pools
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/multiprocessing.html#programming-guidelines
    title: multiprocessing — Process-based parallelism, Python 3.15 documentation
    locator: Programming guidelines
    accessed: 2026-11-16
---

## Synopsis

```python run id=pool-map
from multiprocessing import Pool

def square(x):
    return x * x

if __name__ == "__main__":
    with Pool(2) as pool:
        print(pool.map(square, range(6)))
```

```output for=pool-map
[0, 1, 4, 9, 16, 25]
```

## Semantics

**r1.** The module "supports spawning processes using an API similar to the threading module". It sidesteps the GIL "by using subprocesses instead of threads", so it can "fully leverage multiple processors on a given machine."[^1]

**r2.** "Process objects represent activity that is run in a separate process. The Process class has equivalents of all the methods of threading.Thread." A program calls `start()` and then `join()`.[^2] Example: [`process-exitcode`](#process-exitcode).

**r3.** There are three start methods. With spawn, "The parent process starts a fresh Python interpreter process." With fork, the child "is effectively identical to the parent process", but "safely forking a multithreaded process is problematic." With forkserver, a server process forks each new process.[^1] Example: [`start-method`](#start-method).

**r4.** "The global start method can only be set once." To use another method in one part of a program, `get_context()` returns a context object that has "the same API as the multiprocessing module".[^1][^2]

**r5.** Processes do not share memory by default. A child sees its own copy of each global: with spawn and forkserver, "the value it sees (if any) may not be the same as the value in the parent process at the time that Process.start was called."[^3] Example: [`separate-memory`](#separate-memory).

**r6.** Objects cross between processes by pickling. "The arguments to Process usually need to be picklable", and "Any object put into a multiprocessing queue will be serialized."[^1] Example: [`queue-result`](#queue-result).

**r7.** For shared state, `Value` and `Array` store data "in a shared memory map", and "These shared objects will be process and thread-safe." A `Manager()` "controls a server process which holds Python objects and allows other processes to manipulate them using proxies."[^1] Example: [`shared-value`](#shared-value).

**r8.** `Pool.map` is "A parallel equivalent of the map() built-in function". It "chops the iterable into a number of chunks which it submits to the process pool as separate tasks."[^2]

**r9.** With spawn or forkserver, the main module must be safe to import: "protect the “entry point” of the program by using if __name__ == '__main__'".[^3]

**r10.** `exitcode` is 0 when `run()` returns normally, and N when the child calls `sys.exit()` with an integer N. It is 1 for an uncaught exception, and -N when signal N ends the process.[^2]

## Examples

```python run id=process-exitcode
import sys
from multiprocessing import Process

def ok():
    pass

def stop(code):
    sys.exit(code)

if __name__ == "__main__":
    procs = [Process(target=ok), Process(target=stop, args=(3,))]
    for p in procs:
        p.start()
    for p in procs:
        p.join()
    print([p.exitcode for p in procs])
```

```output for=process-exitcode
[0, 3]
```

```python run id=start-method
import multiprocessing as mp

if __name__ == "__main__":
    print(mp.get_start_method())
    print(sorted(mp.get_all_start_methods()))
```

```output for=start-method
forkserver
['fork', 'forkserver', 'spawn']
```

This is the output on Linux with Python 3.14. On Windows and macOS, the default is spawn.[^1]

```python run id=separate-memory
from multiprocessing import Process

counter = 0

def add():
    global counter
    counter += 1
    print("child:", counter)

if __name__ == "__main__":
    p = Process(target=add)
    p.start()
    p.join()
    print("parent:", counter)
```

```output for=separate-memory
child: 1
parent: 0
```

The child changes its own copy of `counter` (rule r5).

```python run id=queue-result
from multiprocessing import Process, Queue

def work(q):
    q.put([42, None, "hello"])

if __name__ == "__main__":
    q = Queue()
    p = Process(target=work, args=(q,))
    p.start()
    print(q.get())
    p.join()
```

```output for=queue-result
[42, None, 'hello']
```

The parent calls `get()` before `join()`. The guidelines show that a `join()` first can deadlock when the child has put a large item that nobody has taken.[^3]

```python run id=shared-value
from multiprocessing import Process, Value, Array

def change(n, a):
    n.value = 3.1415927
    for i in range(len(a)):
        a[i] = -a[i]

if __name__ == "__main__":
    num = Value("d", 0.0)
    arr = Array("i", range(5))
    p = Process(target=change, args=(num, arr))
    p.start()
    p.join()
    print(num.value)
    print(arr[:])
```

```output for=shared-value
3.1415927
[0, -1, -2, -3, -4]
```

## Versions

> [!since v="3.14"]
> On POSIX, the default start method changed from fork to forkserver "to retain the performance but avoid common multithreaded process incompatibilities." Fork "is no longer the default start method on any platform."[^1]

## Pitfalls

```python wrong id=lambda-target
from multiprocessing import Pool

if __name__ == "__main__":
    with Pool(2) as pool:
        try:
            print(pool.map(lambda x: x * x, range(3)))
        except Exception as e:
            print(type(e).__name__)
```

```output for=lambda-target
PicklingError
```

A lambda has no importable name, so pickle cannot send it to a worker (rule r6). Define the function at the top level of a module.

A module that starts a process at import time fails with spawn and forkserver, because each child imports the main module again. Put the start code under `if __name__ == "__main__":` (rule r9).[^3]

A program that sets no start method and depends on fork, for example on a global that the parent fills in before `start()`, can break on 3.14, where the default on POSIX is forkserver (rule r5). Pass the data as an argument, as the guidelines advise: "it is better to pass the object as an argument to the constructor for the child process."[^3]

## Comparison

`multiprocessing.Process` and `threading.Thread` have the same shape (rule r2), but a thread shares the memory of its process, and a process does not. Threads need locks for shared data; processes need pickling or shared memory. For a pool of workers with futures, [[python/concurrent-futures]] offers `ProcessPoolExecutor` on top of the same machinery. The documentation says it "offers a higher level interface to push tasks to a background process without blocking execution of the calling process."[^1]
