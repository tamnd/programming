---
tag: J5466E
kind: pattern
slug: thread-safe-interface
name: Thread-Safe Interface
aliases: [thread-safe interface, interface and implementation methods, locked public methods]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  Thread-Safe Interface lets only the public methods of a component take its lock, and they call private methods that never lock, so calls inside the component cannot self-deadlock.
family: concurrency
polarity: positive
intent: Make calls between the methods of a thread-safe component avoid self-deadlock and take the lock no more often than needed.
also_known_as: []
history:
  year: 2000
  people: [Douglas C. Schmidt]
  note: Schmidt described the pattern in a C++ Report paper with Strategized Locking and Scoped Locking. POSA2 (2000) has it with the synchronization patterns.
see_also: [scoped-locking, strategized-locking, double-checked-locking, decorator-pattern, mutual-exclusion, deadlock, data-races, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/locking-patterns.pdf
    title: "Douglas C. Schmidt, Strategized Locking, Thread-safe Interface, and Scoped Locking: Patterns and Idioms for Simplifying Multi-threaded C++ Components (C++ Report)"
    locator: 3 The Thread-safe Interface Pattern, sections 3.1 to 3.9
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: Lock objects; RLock objects
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 4 Synchronization Patterns
  - url: https://github.com/python/cpython/blob/main/Lib/queue.py
    title: Lib/queue.py, CPython source code
    locator: class Queue, __init__ and the _qsize, _put and _get methods
---

## Intent

The Thread-safe Interface pattern ensures that intracomponent method calls avoid self-deadlock and minimize locking overhead.[^1]

## Problem

Multi-threaded components typically contain multiple interface and implementation methods that perform computations on state that is encapsulated by the component.[^1] A lock protects that state, and component methods often call each other to carry out their computations.[^1] In components with poorly designed intra-component method invocation behavior, two forces are unresolved:[^1]

- **Avoid self-deadlock.** Self-deadlock will occur if one component method acquires a non-recursive component lock and calls another method that tries to reacquire the same lock.[^1]
- **Minimal locking overhead.** If a recursive component lock is selected to avoid the self-deadlock problem, additional overhead will be incurred to acquire and release the lock multiple times across intracomponent method calls.[^1]

The paper's example is a file cache whose `find` method calls its `bind` method when a file is not in the cache.[^1]

## Solution

Structure components with intra-component method invocations according to two design conventions:[^1]

- **Interface methods check.** All interface methods, such as C++ public methods, should only acquire and release locks, thereby performing the synchronization checks at the "border" of the component. After the lock is acquired, the interface method should forward to an implementation method, which performs the actual method functionality.[^1]
- **Implementation methods trust.** Implementation methods, such as C++ private and protected methods, should trust that they are called with locks held and never acquire or release locks. Implementation methods should never call interface methods, since these methods acquire locks.[^1]

```text
find(path):            # interface method
  guard(lock)
  return find_i(path)

find_i(path):          # implementation method, lock held
  if check_cache_i(path) is none:
    bind_i(path)       # never bind(): it would lock again
  return check_cache_i(path)
```

As long as these design conventions are followed, components will avoid self-deadlock and minimize locking overhead.[^1] To implement the pattern, first determine the interface methods, and for each interface method define a corresponding implementation method.[^1]

## Example

The example files show the paper's file cache in each language with a lock that is not reentrant. The public `bind` and `find` methods take the lock. The `find_i`, `check_cache_i` and `bind_i` methods do the work. The program binds one file, then four workers each call `find` 1,000 times on three paths, and a last `find` reads one entry. Each of the 4,002 interface calls takes the lock once, and no call takes it twice. The program prints:

```text
find /a.html: mapped /a.html
files cached: 4
lock acquisitions: 4002
```

## Consequences

Benefits:[^1]

- **Increased robustness.** The pattern ensures that deadlock does not occur due to intra-component method calls.[^1]
- **Enhanced performance.** The pattern ensures that no unnecessary locks are acquired or released.[^1]

Liability:[^1]

- **Additional indirection and extra methods.** Each interface method requires at least one implementation method. This increases the footprint of the component and may add an extra level of method-call indirection for each invocation. One way to minimize this overhead is to inline each interface method.[^1]

Here is the mistake that the pattern prevents. The public `find` calls the public `bind` while it holds a lock that is not reentrant. In this version, `bind` tries the lock with a timeout, so the program shows the failure instead of waiting forever:

```python wrong id=self-deadlock
import threading


class FileCache:
    def __init__(self):
        self.lock = threading.Lock()
        self.files = {}

    def bind(self, path):
        if not self.lock.acquire(timeout=0.1):
            return "self-deadlock"  # a real bind waits here forever
        try:
            self.files[path] = "mapped " + path
            return "bound"
        finally:
            self.lock.release()

    def find(self, path):
        with self.lock:
            if path not in self.files:
                return self.bind(path)  # an interface method calls an interface method
            return self.files[path]


cache = FileCache()
print("find /a.html:", cache.find("/a.html"))
print("files cached:", len(cache.files))
```

```output for=self-deadlock
find /a.html: self-deadlock
files cached: 0
```

## When not to use it

- The component has one method, or its methods never call each other. There is no intra-component call to protect, and the extra methods only add size.[^1]
- The component must take the lock for each step separately, for example to let other threads run between two steps. The pattern holds the lock for the whole interface call.
- Each step is a single atomic operation on a type that is safe for concurrent use. No component lock is needed then.

## Variants and related patterns

- **Recursive lock.** The other answer to self-deadlock is a recursive lock. A reentrant lock may be acquired multiple times by the same thread, so the call does not block, but the overhead of the second acquisition stays.[^2][^1]
- **Strategized Locking.** Components designed according to the Strategized Locking pattern should employ Thread-safe Interface, because it ensures that the component functions robustly and efficiently regardless of the type of locking strategy that is selected.[^1]
- **Scoped Locking.** The interface methods in the paper use the Scoped Locking idiom to acquire and release the lock.[^1]
- **Decorator.** The pattern is related to Decorator, which extends an object transparently by dynamically attaching additional responsibilities. The primary difference is that Decorator focuses on objects, whereas Thread-safe Interface focuses on classes.[^1]
- **Synchronization patterns.** POSA2 puts Thread-Safe Interface with Scoped Locking, Strategized Locking and Double-Checked Locking Optimization.[^3]

This program counts the lock acquisitions of both answers for the same four lookups, of which three are misses. The recursive design takes the lock again in each `bind`:

```python run id=overhead
import threading


class CountingLock:
    def __init__(self, inner):
        self.inner = inner
        self.acquires = 0

    def __enter__(self):
        self.inner.acquire()
        self.acquires += 1

    def __exit__(self, *exc):
        self.inner.release()


class RecursiveCache:
    """Every method locks; a recursive lock avoids the self-deadlock."""

    def __init__(self):
        self.lock = CountingLock(threading.RLock())
        self.files = {}

    def bind(self, path):
        with self.lock:
            self.files[path] = "mapped " + path

    def find(self, path):
        with self.lock:
            if path not in self.files:
                self.bind(path)  # locks a second time
            return self.files[path]


class InterfaceCache:
    """Thread-Safe Interface: only the public methods lock."""

    def __init__(self):
        self.lock = CountingLock(threading.Lock())
        self.files = {}

    def bind(self, path):
        with self.lock:
            self._bind_i(path)

    def find(self, path):
        with self.lock:
            if path not in self.files:
                self._bind_i(path)
            return self.files[path]

    def _bind_i(self, path):
        self.files[path] = "mapped " + path


for cache in [RecursiveCache(), InterfaceCache()]:
    for path in ["/a.html", "/b.html", "/c.html", "/a.html"]:
        cache.find(path)
    print(f"{type(cache).__name__}: 4 finds, {cache.lock.acquires} lock acquisitions")
```

```output for=overhead
RecursiveCache: 4 finds, 7 lock acquisitions
InterfaceCache: 4 finds, 4 lock acquisitions
```

## Known uses

- **ACE.** The pattern is used extensively throughout the ACE object-oriented network programming toolkit.[^1]
- **Python `queue`.** `Queue` uses a `threading.Lock` as its mutex. Its public `put` and `get` methods acquire the mutex through conditions that share it, and the methods that subclasses override, such as `_put` and `_get`, will only be called with appropriate locks held.[^4]

## References
