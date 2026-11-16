---
tag: S4PAF3
kind: feature
owner: python
slug: thread-synchronization
name: Thread synchronization
aliases: [threading.Lock, RLock, reentrant lock, Condition, condition variable, threading.Event, Semaphore, BoundedSemaphore, Barrier, threading.local, thread-local data]
area: languages/python/concurrency
level: core
summary: >-
  The threading module has Lock, RLock, Condition, Semaphore, Event and Barrier to coordinate threads, and local for data of one thread. A with statement acquires and releases a lock.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/threading.html#lock-objects", locator: "threading, Lock objects to Barrier objects"}
see_also: [python/multi-threading, python/the-queue-module, python/atomicity-and-the-memory-model, mutual-exclusion, data-races]
sources:
  - url: https://docs.python.org/3.15/library/threading.html#lock-objects
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Lock objects; RLock objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#barrier-objects
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Barrier objects; Using locks, conditions, and semaphores in the with statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#condition-objects
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Condition objects; Semaphore objects; Event objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/threading.html#thread-local-data
    title: threading, Thread-based parallelism, Python 3.15 documentation
    locator: Thread-local data
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#multi-threading
    title: The Python Tutorial, 11. Brief Tour of the Standard Library, Part II
    locator: 11.4. Multi-threading
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/faq/library.html#what-kinds-of-global-value-mutation-are-thread-safe
    title: Library and Extension FAQ, Python 3.15 documentation
    locator: What kinds of global value mutation are thread-safe?
    accessed: 2026-11-16
  - url: https://pkg.go.dev/sync#Mutex
    title: Package sync, Go standard library
    locator: type Mutex
    accessed: 2026-11-16
---

## Synopsis

```python run id=lock-counter
import threading

count = 0
lock = threading.Lock()

def add(times):
    global count
    for _ in range(times):
        with lock:
            count += 1

threads = [threading.Thread(target=add, args=(10_000,)) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(count)
```

```output for=lock-counter
40000
```

`count += 1` reads, adds and writes. The lock lets only one thread do these three steps at a time, so no update is lost.

## Semantics

**r1.** A `Lock` "is in one of two states, “locked” or “unlocked”. It is created in the unlocked state." When the lock is locked, `acquire()` "blocks until a call to release() in another thread changes it to unlocked". A lock "is not owned by a particular thread when locked", so any thread can release it.[^1]

**r2.** `release()` on an unlocked lock raises `RuntimeError`.[^1] `acquire(blocking=False)` does not block, and `acquire(timeout=t)` waits at most t seconds. Both return `False` when they do not get the lock.[^1] Example: [`try-acquire`](#try-acquire).

**r3.** "A reentrant lock is a synchronization primitive that may be acquired multiple times by the same thread." "Only the final release() (the release() of the outermost pair) resets the lock to an unlocked state". An `RLock` "must be released by the thread that acquired it."[^1] Example: [`rlock`](#rlock).

**r4.** "All of the objects provided by this module that have acquire and release methods can be used as context managers for a with statement." The block is the same as `acquire()` followed by `try` and a `release()` in `finally`.[^2]

**r5.** A `Condition` has a lock. `wait()` "releases the lock, and then blocks until another thread awakens it by calling notify() or notify_all()". Then it gets the lock again. Code must test its state in a loop, "because wait() can return after an arbitrary long time, and the condition which prompted the notify() call may no longer hold true." `wait_for(predicate)` does the loop.[^3] Example: [`condition`](#condition).

**r6.** "A semaphore manages an internal counter which is decremented by each acquire() call and incremented by each release() call. The counter can never go below zero". A `BoundedSemaphore` raises `ValueError` when a release takes the counter above its initial value.[^3] Example: [`semaphore`](#semaphore).

**r7.** An `Event` "manages a flag that can be set to true with the set() method and reset to false with the clear() method. The wait() method blocks until the flag is true. The flag is initially false."[^3]

**r8.** A `Barrier` is for "a fixed number of threads that need to wait for each other". `wait()` blocks until all the parties call it. It returns "an integer in the range 0 to parties – 1, different for each thread".[^2] Example: [`barrier`](#barrier).

**r9.** The attributes of a `threading.local` object have a separate value in each thread. "changes made in the other thread don’t affect data seen in this thread".[^4] Example: [`thread-local`](#thread-local).

**r10.** The tutorial prefers one thread that owns a resource, fed by a queue: "Applications using Queue objects for inter-thread communication and coordination are easier to design, more readable, and more reliable."[^5] See [[python/the-queue-module]].

## Examples

```python run id=try-acquire
import threading

lock = threading.Lock()
print(lock.acquire())
print(lock.acquire(blocking=False))
print(lock.acquire(timeout=0.05))
lock.release()
print(lock.locked())
```

```output for=try-acquire
True
False
False
False
```

The second and the third call fail, because the lock is already locked, even by the same thread (rule r2).

```python run id=rlock
import threading

lock = threading.RLock()

def outer():
    with lock:
        return inner() + 1

def inner():
    with lock:
        return 41

print(outer())
```

```output for=rlock
42
```

`outer` holds the lock when it calls `inner`, and `inner` takes it again. With a `Lock`, the second acquire would wait forever (rule r3).

```python run id=condition
import threading

items = []
ready = threading.Condition()

def consumer():
    with ready:
        ready.wait_for(lambda: len(items) == 3)
        print("got", items)

t = threading.Thread(target=consumer)
t.start()
for n in range(3):
    with ready:
        items.append(n)
        ready.notify()
t.join()
```

```output for=condition
got [0, 1, 2]
```

`wait_for` tests the predicate with the lock held, and it waits again until the list has three items (rule r5).

```python run id=semaphore
import threading
import time

slots = threading.BoundedSemaphore(2)
lock = threading.Lock()
active = 0
peak = 0

def use_slot():
    global active, peak
    with slots:
        with lock:
            active += 1
            peak = max(peak, active)
        time.sleep(0.05)
        with lock:
            active -= 1

threads = [threading.Thread(target=use_slot) for _ in range(6)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("peak:", peak)
```

```output for=semaphore
peak: 2
```

Six threads want a slot, but the semaphore lets at most two of them in at one time (rule r6).

```python run id=barrier
import threading

barrier = threading.Barrier(3)
lock = threading.Lock()
order = []

def phase(name):
    with lock:
        order.append("1-" + name)
    barrier.wait()
    with lock:
        order.append("2-" + name)

threads = [threading.Thread(target=phase, args=(n,)) for n in "abc"]
for t in threads:
    t.start()
for t in threads:
    t.join()
print(all(x.startswith("1-") for x in order[:3]))
print(all(x.startswith("2-") for x in order[3:]))
```

```output for=barrier
True
True
```

No thread starts phase 2 before all three threads finish phase 1 (rule r8).

```python run id=thread-local
import threading

data = threading.local()
data.name = "main"

def worker():
    data.name = "worker"
    print("in the thread:", data.name)

t = threading.Thread(target=worker)
t.start()
t.join()
print("in main:", data.name)
```

```output for=thread-local
in the thread: worker
in main: main
```

## Pitfalls

`count += 1` is not atomic. The FAQ lists `i = i+1` and `D[x] = D[x] + 1` among the operations that are not atomic, and it ends: "When in doubt, use a mutex!"[^6] Without the lock in the synopsis, threads can lose updates, and the total can be less than 40000 on some runs.

A `Lock` is not reentrant. A thread that acquires a `Lock` twice blocks itself forever:

```python wrong id=self-deadlock
import threading

lock = threading.Lock()
with lock:
    got_it = lock.acquire(timeout=0.1)
    print("second acquire:", got_it)
```

```output for=self-deadlock
second acquire: False
```

The example uses a timeout so that it ends. Without the timeout, the program hangs. Use an `RLock` when a function that holds the lock calls another function that takes it (rule r3).

`release()` on an unlocked lock is an error:

```python fails id=release-unlocked expect=exception:RuntimeError
import threading

threading.Lock().release()
```

A `with` block avoids both a missing and an extra `release()` (rule r4).

## Comparison

Go has the same kind of lock in its `sync` package: "A Mutex is a mutual exclusion lock. The zero value for a Mutex is an unlocked mutex." As with a Python `Lock`, "A locked Mutex is not associated with a particular goroutine."[^7] Go has no `with` statement, so the code calls `Lock` and `Unlock` itself.
