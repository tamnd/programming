---
tag: 78MAN6
kind: pattern
slug: scoped-locking
name: Scoped Locking
aliases: [scoped lock, lock guard, guard, synchronized block]
area: software-design/concurrency-patterns
level: core
summary: >-
  Scoped Locking takes a lock when control enters a scope and releases it when control leaves the scope by any path, so no return or exception can leave the lock held.
family: concurrency
polarity: positive
intent: Tie a lock to a scope, so that the lock is acquired at the start of a critical section and always released at its end.
also_known_as: [Synchronized Block, Object-Construction-is-Resource-Acquisition, Guard]
history:
  year: 2000
  people: [Douglas C. Schmidt]
  note: POSA2 (2000) has the idiom in its synchronization patterns. It is a specialization of Stroustrup's resource acquisition idiom, applied to locks.
see_also: [resource-acquisition-is-initialization, execute-around, mutual-exclusion, strategized-locking, thread-safe-interface, double-checked-locking, deadlock, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/ScopedLocking.pdf
    title: Douglas C. Schmidt, Scoped Locking (POSA2 chapter draft)
    locator: Intent; Also Known As; Example; Solution; Variants; Known Uses; Consequences; See Also
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/lock
    title: The lock statement, C# language reference, Microsoft Learn
    locator: introduction
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: Lock objects; RLock objects
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Table of Contents, 4 Synchronization Patterns
  - url: https://en.cppreference.com/w/cpp/thread/lock_guard.html
    title: std::lock_guard, cppreference.com
    locator: description
  - url: https://doc.rust-lang.org/std/sync/struct.Mutex.html
    title: Mutex in std::sync, The Rust Standard Library
    locator: description
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Defer
---

## Intent

The Scoped Locking C++ idiom ensures that a lock is acquired when control enters a scope and the lock is released automatically when control leaves the scope.[^1] It is also known as Synchronized Block, Object-Construction-is-Resource-Acquisition and Guard.[^1]

## Problem

The paper's example is the hit counter of a Web server. Its public methods must be serialized to prevent threads from corrupting the state of its internal table as hit counts are updated concurrently.[^1] When each method calls `acquire` and `release` itself, maintenance programmers may forget to release the lock on all return paths out of the method.[^1] If a later version throws an exception, the lock is not released either.[^1] If the lock is not released, the Web server process will hang when other threads block indefinitely trying to acquire the lock.[^1]

## Solution

Define a guard class whose constructor automatically acquires a lock when control enters a scope and whose destructor automatically releases the lock when control leaves the scope.[^1] Instantiate instances of the guard class to acquire and release locks in method or block scopes that define critical sections.[^1]

```text
increment(path):
  guard = Guard(lock)        # constructor: lock.acquire()
  if path is bad: return -1  # leaves the scope: lock.release()
  table[path] += 1
  return 0                   # leaves the scope: lock.release()
```

Languages without destructors get the same effect from a statement. Java defines a programming feature called a synchronized block that implements the Scoped Locking idiom in the language.[^1] The C# `lock` statement acquires the mutual-exclusion lock for a given object, executes a statement block, and then releases the lock.[^2] Python locks also support the context management protocol, so a `with` statement holds the lock for its block.[^3]

## Example

The example files show one program in each language. Four workers each add 1,000 hits to one table entry under a scoped lock. Then a call with an empty path takes the early return. The program checks that the lock is free after that return, and that the counter still works. The program prints:

```text
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

## Consequences

Benefits:[^1]

- **Increased robustness.** Locks will be acquired and released automatically when control enters and leaves critical sections defined by method and block scopes.[^1]
- **Decreased maintenance effort.** A change to the method, such as a new return path, cannot forget the release.[^1]

Liabilities:[^1]

- **Potential for deadlock when used recursively.** If a method that uses the Scoped Locking idiom calls itself recursively, self-deadlock will occur if the lock is not a recursive mutex.[^1]
- **Limitations with language-specific semantics.** Locks may not be released automatically when threads or processes abort or exit inside of a guarded critical section.[^1]

Here is the mistake that the idiom prevents. The early return skips the explicit release:

```python wrong id=early-return
import threading

lock = threading.Lock()
table = {}


def increment(path):
    lock.acquire()
    if not path:
        return -1  # returns with the lock held
    table[path] = table.get(path, 0) + 1
    lock.release()
    return 0


increment("")
print("lock still held:", lock.locked())
print("next caller gets the lock:", lock.acquire(timeout=0.1))
```

```output for=early-return
lock still held: True
next caller gets the lock: False
```

## When not to use it

- The lock must be held across scopes, for example taken in one callback and released in another. A guard cannot express that; the Variants section shows explicit accessors.[^1]
- The data is confined to one thread, or it is immutable. No lock is needed then.
- A recursive call path takes the same lock, and the lock is not reentrant.[^1]

## Variants and related patterns

- **Explicit accessors.** A variant of the idiom can be defined to provide explicit accessors to the underlying lock, for code that must release the lock before the scope ends.[^1]
- **Strategized Scoped Locking.** A common variant is to apply the parameterized type or polymorphic version of the Strategized Locking pattern, so one guard works for each kind of lock.[^1]
- **Thread-Safe Interface.** It ensures that only interface methods apply the Scoped Locking idiom, whereas implementation methods do not, which avoids self-deadlock.[^1]
- **Synchronization patterns.** POSA2 puts Scoped Locking with Strategized Locking, Thread-Safe Interface and Double-Checked Locking Optimization.[^4]
- **RAII.** The idiom is a special case of a more general C++ idiom where a constructor acquires a resource and a destructor releases the resource.[^1]

A reentrant lock is a synchronization primitive that may be acquired multiple times by the same thread.[^3] This shows the recursive case with both kinds of lock:

```python run id=recursive
import threading


def nested(lock):
    with lock:  # the outer scope holds the lock
        got = lock.acquire(timeout=0.1)  # a recursive call tries again
        if got:
            lock.release()
        return got


print("Lock, recursive call gets the lock:", nested(threading.Lock()))
print("RLock, recursive call gets the lock:", nested(threading.RLock()))
```

```output for=recursive
Lock, recursive call gets the lock: False
RLock, recursive call gets the lock: True
```

## Known uses

- **Booch Components.** They were one of the first C++ class libraries to use the Scoped Locking idiom for multithreaded C++ programs.[^1]
- **ACE.** The idiom is used extensively throughout the Adaptive Communication Environment.[^1]
- **C++.** `std::lock_guard` is a mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block.[^5]
- **Rust.** The data of a `Mutex` can only be accessed through the RAII guards returned from `lock` and `try_lock`.[^6]
- **Go.** The canonical examples of `defer` are unlocking a mutex or closing a file.[^7]
- **Java and C#.** The `synchronized` block and the `lock` statement are the idiom in the language.[^1][^2]

## References
