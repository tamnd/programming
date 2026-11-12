---
tag: EWN7HA
kind: pattern
slug: strategized-locking
name: Strategized Locking
aliases: [pluggable locking, parameterized locking, lock strategy]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  Strategized Locking makes the lock of a component a parameter, so one implementation serves single-threaded and multi-threaded use with the lock that fits each case.
family: concurrency
polarity: positive
intent: Make the synchronization of a reusable component a pluggable type, so the component can be tuned for each concurrency use-case without a separate copy of its code.
also_known_as: []
history:
  year: 2000
  people: [Douglas C. Schmidt]
  note: Schmidt described the pattern in a C++ Report paper with Thread-safe Interface and Scoped Locking. POSA2 (2000) has it with the synchronization patterns.
see_also: [scoped-locking, thread-safe-interface, double-checked-locking, strategy-pattern, null-object-pattern, bridge-pattern, mutual-exclusion, deadlock]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/locking-patterns.pdf
    title: "Douglas C. Schmidt, Strategized Locking, Thread-safe Interface, and Scoped Locking: Patterns and Idioms for Simplifying Multi-threaded C++ Components (C++ Report)"
    locator: 2 The Strategized Locking Pattern, sections 2.1 to 2.10
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: Lock objects; RLock objects
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 4 Synchronization Patterns
  - url: https://en.cppreference.com/w/cpp/thread/lock_guard.html
    title: std::lock_guard, cppreference.com
    locator: Template parameters
  - url: https://pkg.go.dev/sync
    title: sync package, Go standard library
    locator: type Locker
---

## Intent

The Strategized Locking pattern strategizes a component's synchronization to increase its flexibility and reusability without degrading its performance or maintainability.[^1]

## Problem

The paper's example is the file cache of a Web server. One version, File Cache ST, implements a single-threaded file cache that requires no locking. Another version, File Cache Thread Mutex, uses a mutex to serialize multiple threads that access the cache concurrently.[^1] Maintaining multiple separate implementations of similar file cache components can be tedious since future enhancements and fixes must be updated consistently in each component implementation.[^1]

A family of components whose synchronization strategies are hard-coded into their implementations fails to resolve two forces:[^1]

- **Ease of performance tuning.** If the synchronization strategy is hard-coded, it is time-consuming to modify the component to support new, more efficient synchronization strategies. For example, a completely new class must be written to use a readers-writer lock instead of a thread mutex.[^1]
- **Ease of maintenance and enhancement.** If there are multiple copies of the same basic components, version-skew is likely to occur, since changes to one component may not be applied consistently to the others.[^1]

## Solution

Strategize a component's synchronization aspects by making them "pluggable" types. Define instances of these pluggable types as data members that the component's method implementations use to perform the synchronization strategies that an application or a service configures into the component.[^1]

```text
FileCache(lock):            # the lock is a parameter
  find(path):
    guard(lock)             # Scoped Locking inside the method
    look up or insert path

FileCache(NullMutex)        # single-threaded: no cost
FileCache(ThreadMutex)      # multi-threaded: serialized
FileCache(RWLock)           # many readers
```

The paper gives two forms:[^1]

- **Parameterized types.** Add a `LOCK` template parameter to the component. The template can be instantiated with any `LOCK` type that conforms to the `acquire` and `release` signature, and it need not inherit from an abstract base class.[^1]
- **Polymorphism.** Pass a polymorphic `Lock` object to the component's initialization method. A common way to implement it is to use the Bridge pattern.[^1]

The parameterized type approach should be used when the locking strategy is known at compile-time, and the polymorphic approach when it is not known until run-time.[^1] A surprisingly useful strategy is the Null Mutex, which defines an efficient locking strategy for single-threaded applications and components.[^1] All of its methods are empty inlined functions that can be completely removed by optimizing compilers, and it is an example of the Null Object pattern.[^1]

## Example

The example files show one file cache class in each language, used with two strategies. A null lock serves one thread that looks up three paths. A real lock serves four threads that each do 1,000 lookups. The `FileCache` code does not change between the two cases. The program prints:

```text
Null_Mutex cache: 2 files, 3 lookups
Thread_Mutex cache: 3 files, 4000 lookups
```

## Consequences

Benefits:[^1]

- **Enhanced flexibility and performance tuning.** Because the synchronization aspects are strategized, it is straightforward to configure and tune a component for particular concurrency use-cases.[^1]
- **Decreased maintenance effort.** There is only one implementation, rather than a separate implementation for each concurrency use-case, which avoids version-skew.[^1]

Liabilities:[^1]

- **Obtrusive locking.** If templates are used to parameterize locking aspects, this exposes the locking strategies to application code. One way to avoid this problem is to apply the polymorphic approach.[^1]
- **Strategy-dependent self-deadlock.** A component whose `find` method calls its `bind` method works well only with a lock with recursive mutex semantics or a Null Mutex. With a non-recursive mutex, the code will self-deadlock, because `bind` reacquires the lock already held by `find`.[^1]

The second liability is easy to miss, because the same code works with one strategy and hangs with another. In this version, `bind` tries the lock with a timeout, so the program shows the failure instead of waiting forever. A reentrant lock may be acquired multiple times by the same thread, and a plain lock may not.[^2]

```python wrong id=self-deadlock
import threading


class FileCache:
    def __init__(self, lock):
        self.lock = lock
        self.files = {}

    def bind(self, path):
        if not self.lock.acquire(timeout=0.1):
            return False  # a real bind waits here forever
        self.files[path] = "mapped " + path
        self.lock.release()
        return True

    def find(self, path):
        with self.lock:
            if path not in self.files:
                return self.bind(path)  # takes the lock a second time
            return True


print("RLock strategy, find binds the file:", FileCache(threading.RLock()).find("/a.html"))
print("Lock strategy, find binds the file:", FileCache(threading.Lock()).find("/a.html"))
```

```output for=self-deadlock
RLock strategy, find binds the file: True
Lock strategy, find binds the file: False
```

The Thread-Safe Interface pattern removes this liability, because its implementation methods never take the lock.[^1]

## When not to use it

- The component has one concurrency use-case, and it is not a reusable component. A hard-coded lock is then simpler, and there is no family of versions to keep in step.
- Application code must not see the lock type, and the language has only the template form. The obtrusive locking liability applies then.[^1]
- The component calls its own locking methods, and it does not follow Thread-Safe Interface. Some strategies then self-deadlock.[^1]

## Variants and related patterns

- **Scoped Locking.** The Scoped Locking idiom uses Strategized Locking to parameterize various synchronization strategies into its guard class.[^1]
- **Thread-Safe Interface.** Components designed according to Strategized Locking should employ Thread-safe Interface, because it ensures that the component functions robustly and efficiently regardless of the type of locking strategy that is selected.[^1]
- **Wrapper Facade.** If the synchronization components that exist have incompatible interfaces, use the Wrapper Facade pattern to adapt them to the signatures that the component expects.[^1]
- **Synchronization patterns.** POSA2 puts Strategized Locking with Scoped Locking, Thread-Safe Interface and Double-Checked Locking Optimization.[^3]
- **Standard library forms.** The type parameter of `std::lock_guard` must meet the BasicLockable requirements, so a null mutex type can be the parameter.[^4] In Go, a `Locker` represents an object that can be locked and unlocked, and a component can hold that interface.[^5]

A strategy can also measure. This version plugs a counting wrapper around a real lock to see how often the component locks. The `FileCache` class does not change.

```python run id=counting
import threading


class CountingLock:
    def __init__(self, inner):
        self.inner = inner
        self.acquires = 0

    def acquire(self):
        self.inner.acquire()
        self.acquires += 1

    def release(self):
        self.inner.release()


class FileCache:
    def __init__(self, lock):
        self.lock = lock
        self.files = {}

    def find(self, path):
        self.lock.acquire()
        try:
            return self.files.setdefault(path, "mapped " + path)
        finally:
            self.lock.release()


lock = CountingLock(threading.Lock())
cache = FileCache(lock)
for path in ["/a.html", "/b.html", "/a.html"]:
    cache.find(path)
print("files:", len(cache.files))
print("lock acquired:", lock.acquires, "times")
```

```output for=counting
files: 2
lock acquired: 3 times
```

## Known uses

- **Booch Components.** They were one of the first C++ class libraries to parameterize locking strategies with templates.[^1]
- **ACE.** The pattern is used extensively throughout the ACE object-oriented network programming toolkit.[^1]
- **Aspect-Oriented Programming.** The paper names AOP as a general methodology for systematically strategizing aspects that vary in applications and components.[^1]
- **C++ and Go standard libraries.** `std::lock_guard` takes the lock type as a template parameter, and Go code can depend on `sync.Locker` instead of a concrete mutex.[^4][^5]

## References
