---
tag: NEW-python-atomicity-and-the-memory-model
kind: feature
owner: python
slug: atomicity-and-the-memory-model
name: Atomicity and the memory model
aliases: [atomic operations, thread safety of built-in types, race condition, data race, check-then-act, TOCTOU, read-modify-write]
area: languages/python/concurrency
level: research
summary: >-
  Python promises atomicity only where the documentation says so. A single list or dict operation is atomic, but a read, a change and a write are three steps, and threads can interleave between them.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/threadsafety.html", locator: "Thread Safety Guarantees"}
see_also: [data-races, python/thread-synchronization, python/free-threaded-python, python/the-global-interpreter-lock, mutual-exclusion]
sources:
  - url: https://docs.python.org/3.15/glossary.html
    title: Glossary, Python 3.15 documentation
    locator: atomic operation; data race; race condition; thread-safe
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/library.html#what-kinds-of-global-value-mutation-are-thread-safe
    title: Library and Extension FAQ, Python 3.15 documentation
    locator: What kinds of global value mutation are thread-safe?
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/threadsafety.html
    title: Thread Safety Guarantees, Python 3.15 documentation
    locator: Thread safety levels; Thread safety for list objects; Thread safety for dict objects
    accessed: 2026-11-16
  - url: https://go.dev/ref/mem
    title: The Go Memory Model
    locator: Memory Model; Implementation Restrictions for Programs Containing Data Races
    accessed: 2026-11-16
---

## Synopsis

```python run id=lost-update
import threading

counter = 0
barrier = threading.Barrier(2)

def add_one():
    global counter
    value = counter       # read
    barrier.wait()        # both threads have read
    counter = value + 1   # write

threads = [threading.Thread(target=add_one) for _ in range(2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(counter)
```

```output for=lost-update
1
```

Two threads each add one, but the result is 1. The barrier forces the bad order every time: both threads read 0 before either writes. Without the barrier, the same loss can happen at random.

## Semantics

**r1.** An atomic operation is "An operation that appears to execute as a single, indivisible step: no other thread can observe it half-done, and its effects become visible all at once." Python does not promise that a statement is atomic: "x += 1 performs multiple bytecode operations and is not atomic". "Atomicity is only guaranteed where explicitly documented."[^1] Example: [`lost-update`](#lost-update).

**r2.** With the GIL, "Python offers to switch among threads only between bytecode instructions". In practice, "operations on shared variables of built-in data types (ints, lists, dicts, etc) that “look atomic” really are."[^2]

**r3.** The FAQ lists `L.append(x)`, `x = L.pop()`, `D[x] = y` and `D1.update(D2)` among the atomic operations, and `i = i+1`, `L.append(L[-1])` and `D[x] = D[x] + 1` among the operations that are not.[^2]

**r4.** An atomic operation can still run other code. "Operations that replace other objects may invoke those other objects’ __del__() method when their reference count reaches zero". The FAQ ends: "When in doubt, use a mutex!"[^2]

**r5.** The Thread Safety Guarantees page gives the rules for the free-threaded build. "When the GIL is enabled, most operations are implicitly serialized."[^3]

**r6.** For a list, "Reading a single element from a list is atomic", and `lst.append(x)` and `lst.pop()` "are atomic". `lst.insert(idx, item)` and `lst.pop(idx)` "may allow lock-free operations to observe intermediate states". `sort()` "is not atomic", and "the list appears empty for the duration of the sort."[^3] Example: [`append-only`](#append-only).

**r7.** For a dict, `d[key]`, `d.get(key)`, `key in d` and `len(d)` "are lock-free and atomic". A write or a delete of one item "is safe to call from multiple threads and will not corrupt the dictionary".[^3]

**r8.** "Operations that involve multiple accesses, as well as iteration, are never atomic." A read-modify-write and a check-then-act are such operations. "To avoid time-of-check to time-of-use (TOCTOU) issues, use atomic operations or handle exceptions".[^3] Example: [`check-then-act`](#check-then-act).

**r9.** A race condition is "A condition of a program where the behavior depends on the relative timing or ordering of events". A data race is "A situation where multiple threads access the same memory location concurrently, at least one of the accesses is a write, and the threads do not use any synchronization". In Python, "data races can only happen in native code, but that native code might be exposed in a Python API."[^1]

## Examples

```python run id=append-only
import threading

items = []

def worker():
    for i in range(1_000):
        items.append(i)

threads = [threading.Thread(target=worker) for _ in range(8)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(len(items))
```

```output for=append-only
8000
```

Each `append` is one atomic operation (rule r6), so no append is lost. The order of the items depends on the threads, so the program prints only the length.

```python wrong id=check-then-act
import threading

jobs = {"job": 1}
barrier = threading.Barrier(2)
errors = []

def take():
    if "job" in jobs:     # check
        barrier.wait()    # both threads have checked
        try:
            del jobs["job"]   # act
        except KeyError:
            errors.append("KeyError")

threads = [threading.Thread(target=take) for _ in range(2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(errors)
```

```output for=check-then-act
['KeyError']
```

Both threads see the key, and then both try to delete it. The second delete fails.

```python run id=pop-default
import threading

jobs = {"job": 1}
results = []

def take():
    results.append(jobs.pop("job", None))

threads = [threading.Thread(target=take) for _ in range(2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(sorted(results, key=str))
```

```output for=pop-default
[1, None]
```

`d.pop(key, None)` checks and removes in one operation, which the guarantees page gives as the fix for check-then-delete.[^3] Exactly one thread gets the job.

## Pitfalls

A list or dict that does not get corrupted is not the same as a correct program. The lock inside a dict keeps the dict valid, but `d[key] = d[key] + 1` is still a read and a write, and an update can be lost (rule r8).

The atomic operations in the FAQ come from how CPython runs bytecode with the GIL (rule r2). They are not rules of the language. Code that depends on them makes assumptions that other implementations need not keep. Use a lock, a queue or an atomic method such as `pop` with a default.

Iteration over a list, dict or set that another thread changes can see a mix of old and new state (rule r8). Iterate over a copy, such as `d.copy().items()`, or hold a lock.[^3]

## Comparison

Go has a formal memory model. It defines a "happens before" relation, and "A data race is defined as a write to a memory location happening concurrently with another read or write to that same location, unless all the accesses involved are atomic data accesses as provided by the sync/atomic package."[^4] The Python documents cited here instead list, type by type, which operations are atomic or safe (rules r6 and r7). In both languages, the advice is the same: Go programmers "are strongly encouraged to use appropriate synchronization to avoid data races", and the Python FAQ says "When in doubt, use a mutex!"[^4][^2]
