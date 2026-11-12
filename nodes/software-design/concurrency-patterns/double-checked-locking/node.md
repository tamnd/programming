---
tag: J09WED
kind: pattern
slug: double-checked-locking
name: Double-Checked Locking
aliases: [double-checked locking, DCL, double-checked locking optimization, lock hint]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  Double-Checked Locking tests a condition before it takes a lock and again after, so code that must run once takes the lock only on the first calls. The plain form needs a safe memory model.
family: concurrency
polarity: positive
intent: Reduce contention and synchronization overhead when a critical section must run just once, but must be thread-safe when it runs.
also_known_as: [Double-Checked Locking Optimization, Lock Hint]
history:
  year: 1997
  people: [Douglas C. Schmidt, Tim Harrison]
  note: The paper appeared in Pattern Languages of Program Design 3. The Double-Checked Locking is Broken declaration later showed that the plain form fails in Java, and JDK5 made it work with volatile.
see_also: [singleton-pattern, lazy-initialization, scoped-locking, strategized-locking, thread-safe-interface, data-races, atomic-operations, mutual-exclusion]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/DC-Locking.pdf
    title: "Douglas C. Schmidt and Tim Harrison, Double-Checked Locking: An Optimization Pattern for Efficiently Initializing and Accessing Thread-safe Objects (PLoPD3)"
    locator: Abstract; 1 Intent; 2 Also Known As; 3 Motivation; 7 Consequences; 9 Variations; 10 Known Uses; 11 Related Patterns
  - url: https://en.wikipedia.org/wiki/Double-checked_locking
    title: Double-checked locking, Wikipedia
    locator: lead; Motivation and original pattern; Usage in C++; Usage in C#
  - url: https://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html
    title: David Bacon and others, The "Double-Checked Locking is Broken" Declaration
    locator: introduction; The first reason it doesn't work; Under the new Java Memory Model
  - url: https://en.cppreference.com/w/cpp/thread/call_once.html
    title: std::call_once, cppreference.com
    locator: description
  - url: https://pkg.go.dev/sync
    title: sync package, Go standard library
    locator: type Once
  - url: https://doc.rust-lang.org/std/sync/struct.OnceLock.html
    title: OnceLock in std::sync, The Rust Standard Library
    locator: description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.lazy-1?view=net-8.0
    title: Lazy<T> Class, .NET API reference, Microsoft Learn
    locator: Definition; Remarks
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 4 Synchronization Patterns
---

## Intent

The Double-Checked Locking optimization pattern reduces contention and synchronization overhead whenever "critical sections" of code need to acquire locks just once, but must be thread-safe when they do acquire locks.[^1] It is also known as Lock Hint.[^1]

## Problem

The paper starts from the canonical implementation of the Singleton pattern, which does not work correctly in the presence of preemptive multi-tasking or true parallelism.[^1] If two threads call `instance` at the same time, they may both try to create the object, or one may get a reference to an incompletely initialized object.[^2] Synchronizing the whole method with a lock fixes this, but the code then performs synchronization every time the method is called.[^3] After the first call, that lock protects nothing, because the object exists and no thread changes it.

## Solution

Unnecessary locking is avoided by wrapping the call to `new` with another conditional test.[^1] The steps are:[^2]

1. Check that the variable is initialized, without obtaining the lock. If it is initialized, return it immediately.
2. Obtain the lock.
3. Double-check whether the variable has already been initialized. If another thread acquired the lock first, it may have done the initialization.
4. Otherwise, initialize and return the variable.

```text
instance():
  if instance is null:          # first check, no lock
    lock:
      if instance is null:      # second check, lock held
        instance = new Config
  return instance
```

The first check is only a hint. Andrew Birrell refers to the first check of the flag as a lock "hint."[^1] The second check decides, because it runs with the lock held.

## Example

The example files show the pattern in each language. Eight threads start together and each calls `instance` 1,000 times. The Java and C# versions use a `volatile` field, the C++ version an atomic pointer with acquire and release ordering, the Go version an atomic pointer, and the Rust version a `OnceLock` as the flag. The JavaScript and TypeScript versions use async tasks, because they have one thread. The program prints:

```text
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

How many calls take the lock depends on the timing of the threads. Each thread takes it at most once, because after its own locked call the first check always finds the instance.

## Consequences

Benefits:[^1]

- **Minimized locking.** By performing two flag checks, the pattern optimizes for the common case. Once the flag is set, the first check ensures that subsequent accesses require no locking.[^1]
- **Prevents race conditions.** The second check of the flag ensures that the event is performed only once.[^1]

Liabilities:

- **Potential for subtle portability bugs.** If the write to memory that results from the call to `new` is not atomic, other threads may try to read an invalid pointer.[^1] An aggressive compiler may also keep the flag in a register or remove the second check, and the paper solves this with `volatile`.[^1]
- **Broken under weak memory models.** The writes that initialize the object and the write to the field can be done or perceived out of order. A thread can then see a non-null reference but the default values for the fields of the object.[^3] The original form has data races, depending on the memory model in use, and it is hard to get right. Some consider it to be an anti-pattern.[^2]
- **Hard to test.** Even a naive implementation will appear to work most of the time, and failures may only occur intermittently.[^2]

The second check is not optional. In this version, each thread that passed the first check while another thread was creating the object takes the lock and creates its own copy:

```python wrong id=one-check
import threading
import time


class Config:
    created = 0

    def __init__(self):
        time.sleep(0.01)  # slow set-up widens the window
        Config.created += 1


_instance = None
_lock = threading.Lock()


def instance():
    global _instance
    if _instance is None:  # only one check
        with _lock:
            _instance = Config()  # no second check under the lock
    return _instance


start = threading.Barrier(8)


def worker():
    start.wait()
    instance()


threads = [threading.Thread(target=worker) for _ in range(8)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("instances created more than once:", Config.created > 1)
```

```output for=one-check
instances created more than once: True
```

## When not to use it

- The language or library has a once primitive. `std::call_once` executes a callable exactly once, even if called concurrently from several threads.[^4] A Go `Once` is an object that will perform exactly one action.[^5] A Rust `OnceLock` is a synchronization primitive which can nominally be written to only once.[^6] In .NET, `Lazy<T>` provides support for lazy initialization.[^7]
- The object is a C++ static local. If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.[^2]
- The object is a static Java singleton. Define it as a static field in a separate class, which is not initialized until the field is referenced.[^3]
- The language has no memory model that makes the plain form safe, and no atomic or `volatile` field is used. Without such a field, there is no way to make the plain form work in Java without requiring each thread that accesses the object to perform synchronization.[^3]

## Variants and related patterns

- **Volatile field in Java.** JDK5 and later extends the semantics of `volatile`, and with this change the idiom can be made to work by declaring the field to be `volatile`.[^3]
- **Atomics in C++.** A manual version needs at least atomic operations with acquire and release memory ordering.[^2] Explicit memory barriers can also be used to make it work in C++.[^3]
- **Separate flag.** On hardware without atomic pointer assignment, it may be necessary to use a separate, word-aligned integral flag instead of the instance pointer.[^1]
- **First-Time-In idiom.** Double-Checked Locking is a thread-safe variant of the First-Time-In idiom, which is often used in languages like C that lack constructors.[^1]
- **Library forms.** In .NET Framework 4.0, `System.Lazy<T>` was introduced, and it internally uses double-checked locking by default in its `ExecutionAndPublication` mode.[^2]
- **Synchronization patterns.** POSA2 puts Double-Checked Locking Optimization with Scoped Locking, Strategized Locking and Thread-Safe Interface.[^8]

The paper's performance tests compare a Mutex Singleton, which acquires a mutex lock for every call, with a Double-Checked Singleton.[^1] This program counts the locked calls of both for one thread:

```python run id=lock-count
import threading


class Config:
    pass


lock = threading.Lock()


def make_mutex_singleton():
    state = {"instance": None, "locked": 0}

    def instance():
        with lock:  # every call takes the lock
            state["locked"] += 1
            if state["instance"] is None:
                state["instance"] = Config()
            return state["instance"]

    return instance, state


def make_double_checked():
    state = {"instance": None, "locked": 0}

    def instance():
        if state["instance"] is None:
            with lock:
                state["locked"] += 1
                if state["instance"] is None:
                    state["instance"] = Config()
        return state["instance"]

    return instance, state


for name, make in [("Mutex Singleton", make_mutex_singleton), ("Double-Checked", make_double_checked)]:
    instance, state = make()
    for _ in range(1000):
        instance()
    print(f"{name}: 1000 calls, {state['locked']} took the lock")
```

```output for=lock-count
Mutex Singleton: 1000 calls, 1000 took the lock
Double-Checked: 1000 calls, 1 took the lock
```

## Known uses

- **ACE.** The pattern is used extensively throughout the ACE object-oriented network programming toolkit. `ACE_Singleton` is a template that uses it, and the ACE Token Manager is created on demand through it.[^1]
- **Sequent Dynix/PTX.** The pattern is a special case of a very widely used pattern in this operating system.[^1]
- **POSIX once variables.** The pattern can be used to implement them.[^1]
- **.NET.** `System.Lazy<T>` uses double-checked locking by default.[^2]

## References
