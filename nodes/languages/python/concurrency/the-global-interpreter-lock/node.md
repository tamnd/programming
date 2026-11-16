---
tag: NEW-python-the-global-interpreter-lock
kind: feature
owner: python
slug: the-global-interpreter-lock
name: The global interpreter lock
aliases: [GIL, global interpreter lock, sys.setswitchinterval, switch interval, CPU-bound threads, I/O-bound threads, Py_BEGIN_ALLOW_THREADS]
area: languages/python/concurrency
level: advanced
summary: >-
  In the default CPython build, the global interpreter lock lets only one thread run Python bytecode at a time. Threads still overlap while they wait for I/O or run C code that releases the lock.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/glossary.html#term-global-interpreter-lock", locator: "Glossary, global interpreter lock"}
see_also: [python/multi-threading, python/free-threaded-python, python/atomicity-and-the-memory-model, python/multiprocessing, threads]
sources:
  - url: https://docs.python.org/3.15/glossary.html#term-global-interpreter-lock
    title: Glossary, Python 3.15 documentation
    locator: global interpreter lock
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/c-api/threads.html
    title: Thread states and the global interpreter lock, Python/C API reference manual, Python 3.15
    locator: Thread states and the global interpreter lock; Detaching the thread state from extension code
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/sys.html#sys.setswitchinterval
    title: sys, System-specific parameters and functions, Python 3.15 documentation
    locator: sys.setswitchinterval; sys._is_gil_enabled
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#gil-and-performance-considerations
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Introduction; GIL and performance considerations
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/library.html#what-kinds-of-global-value-mutation-are-thread-safe
    title: Library and Extension FAQ, Python 3.15 documentation
    locator: What kinds of global value mutation are thread-safe?; Can’t we get rid of the Global Interpreter Lock?
    accessed: 2026-11-16
  - url: https://pkg.go.dev/runtime#hdr-Environment_Variables
    title: Package runtime, Go standard library
    locator: Environment Variables, GOMAXPROCS
    accessed: 2026-11-16
---

## Synopsis

```python run id=gil-state
import sys

print(sys._is_gil_enabled())
print(sys.getswitchinterval())
```

```output for=gil-state
True
0.005
```

On the default build of CPython the GIL is on. The interpreter offers to switch to another thread after each switch interval, here 5 milliseconds.

## Semantics

**r1.** The global interpreter lock is "The mechanism used by the CPython interpreter to assure that only one thread executes Python bytecode at a time." It makes "the object model (including critical built-in types such as dict) implicitly safe against concurrent access", "at the expense of much of the parallelism afforded by multi-processor machines."[^1]

**r2.** "only a thread that holds the GIL may operate on Python objects or invoke Python’s C API." Without the lock, two threads that increment the reference count of one object at the same time could increment it "only once instead of twice."[^2]

**r3.** "the interpreter regularly tries to switch threads between bytecode instructions (see sys.setswitchinterval()). This is why locks are also necessary for thread-safety in pure-Python code."[^2] The GIL protects the interpreter, not the invariants of a program.

**r4.** `sys.setswitchinterval()` sets "the ideal duration of the “timeslices” allocated to concurrently running Python threads". "which thread becomes scheduled at the end of the interval is the operating system’s decision. The interpreter doesn’t have its own scheduler."[^3] Example: [`switch-interval`](#switch-interval).

**r5.** "the GIL is always released when doing I/O." Some extension modules "release the GIL when doing computationally intensive tasks such as compression or hashing."[^1] In C, the macros `Py_BEGIN_ALLOW_THREADS` and `Py_END_ALLOW_THREADS` wrap such code. The standard `zlib` and `hashlib` modules release the lock "when compressing or hashing data."[^2] Example: [`io-overlap`](#io-overlap).

**r6.** Thus threads speed up I/O-bound work, but not CPU-bound pure Python code: "the GIL limits the performance gains of threading when it comes to CPU-bound tasks, as only one thread can execute Python bytecode at a time."[^4] For CPU-bound work the documentation points to `multiprocessing` or `concurrent.futures.ProcessPoolExecutor`.[^4] Example: [`cpu-bound`](#cpu-bound).

**r7.** `sys._is_gil_enabled()` returns "True if the GIL is enabled and False if it is disabled."[^3] It is a CPython detail. A free-threaded build can run with the GIL off; see [[python/free-threaded-python]].

## Examples

```python run id=switch-interval
import sys

old = sys.getswitchinterval()
sys.setswitchinterval(0.001)
print(sys.getswitchinterval())
sys.setswitchinterval(old)
print(sys.getswitchinterval() == old)
```

```output for=switch-interval
0.001
True
```

A shorter interval makes the interpreter offer the lock to other threads more often (rule r4). It does not make Python code run in parallel.

```python run id=io-overlap
import threading
import time

def wait():
    time.sleep(0.2)

start = time.perf_counter()
threads = [threading.Thread(target=wait) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
elapsed = time.perf_counter() - start
print("four waits of 0.2 s took less than 0.6 s:", elapsed < 0.6)
```

```output for=io-overlap
four waits of 0.2 s took less than 0.6 s: True
```

A waiting thread does not hold the lock, so the four waits overlap (rule r5).

```python run id=cpu-bound
import sys
import threading
import time

def count(n):
    while n:
        n -= 1

def best_of_three(run):
    times = []
    for _ in range(3):
        start = time.perf_counter()
        run()
        times.append(time.perf_counter() - start)
    return min(times)

N = 400_000

def serial():
    for _ in range(4):
        count(N)

def threaded():
    threads = [threading.Thread(target=count, args=(N,)) for _ in range(4)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()

s = best_of_three(serial)
t = best_of_three(threaded)
print("GIL on:", sys._is_gil_enabled())
print("threads at least 60% of the serial time:", t > 0.6 * s)
```

```output for=cpu-bound
GIL on: True
threads at least 60% of the serial time: True
```

With the GIL on, four threads that count down take about as long as one thread that does the four counts (rule r6).

## Versions

> [!since v="3.13"]
> CPython has an optional free-threaded build that can run with the GIL off. The FAQ says that it "is officially supported" from 3.14, "but is not the default build."[^5]

## Pitfalls

The GIL does not make a read, an update and a write atomic. The FAQ lists `i = i+1` and `D[x] = D[x] + 1` as not atomic.[^5] Use a lock for such updates (see [[python/thread-synchronization]]).

Code that depends on the GIL for its safety can break on a free-threaded build, where threads run Python code in parallel. Write the locks that the program needs, and do not rely on the switch interval.

Threads do not help a CPU-bound pure Python loop (rule r6). Use processes, or a C extension that releases the lock.

## Comparison

Go has no global lock. "The GOMAXPROCS variable limits the number of operating system threads that can execute user-level Go code simultaneously." Threads that are blocked in system calls do not count against that limit.[^6] Thus Go code runs in parallel on as many threads as GOMAXPROCS allows, while CPython with the GIL runs Python code on one thread at a time.
