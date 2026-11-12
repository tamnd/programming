---
tag: 7XKYEB
kind: pattern
slug: balking-pattern
name: Balking
aliases: [balking, balking pattern, balk]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  A balking method runs its action only when the object is in the right state. In any other state it returns at once, or throws, and does not wait for the state to change.
family: concurrency
polarity: positive
intent: Make a method refuse a request at once when the object is not in a state that allows it, so that the caller does not block and the action runs at most when it makes sense.
also_known_as: [Balk]
see_also: [guarded-suspension, read-write-lock, monitor-object, thread-safe-interface, scoped-locking, mutual-exclusion, data-races, null-object-pattern]
sources:
  - url: https://en.wikipedia.org/wiki/Balking_pattern
    title: Balking pattern, Wikipedia
    locator: lead; Usage; Implementation
  - url: https://en.wikipedia.org/wiki/Guarded_suspension
    title: Guarded suspension, Wikipedia
    locator: Usage
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/IllegalStateException.html
    title: Java SE 21 API, java.lang.IllegalStateException
    locator: class description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.invalidoperationexception
    title: InvalidOperationException Class, .NET API reference
    locator: class description
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: Lock.acquire; Thread.start
  - url: https://pkg.go.dev/sync#Mutex.TryLock
    title: Go standard library, package sync
    locator: func (*Mutex) TryLock
---

## Intent

The balking pattern only executes an action on an object when the object is in a particular state.[^1] If an object reads ZIP files and a caller invokes a get method when the ZIP file is not open, the object "balks" at the request.[^1]

## Problem

Several threads call the same method, but the action only makes sense in some states. A save of a document should not start while another save is in progress, and it is not needed when nothing has changed. A caller that waits for the right state, as in [guarded suspension](guarded-suspension), can wait for an unknown time, and the work it would do is often not needed any more.

## Solution

Under a lock, test the state. If the state is wrong, return at once. If it is right, change the state to show that the action is in progress, release the lock, and run the action. When the action ends, take the lock again and set the state back.

```text
 save() ──► lock ──► saving or not changed? ──yes──► unlock ──► return False (balk)
                       │ no
                       ▼
                saving = True, changed = False ──► unlock
                       ▼
                write the file (no lock held)
                       ▼
                lock ──► saving = False ──► unlock ──► return True
```

In the Java example on Wikipedia, the `synchronized` block makes sure that if there are multiple calls to the job method, only one proceeds, and the other calls return with nothing.[^1] The method that clears the flag is also synchronized, because the only way to guarantee that another thread sees a change to a field is to synchronize all access to it. A `volatile` Boolean would also work for that one field.[^1]

## Example

A document is edited, and a first save starts and stays in progress. While it runs, 7 other threads call `save`, and all of them balk. After the first save ends, a save with no changes balks too, and a save after a new edit runs.

```text
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

The examples for each language show the same program.

## Consequences

Benefits:

- The caller never blocks for the state to change, so a duplicate request costs only one test under a lock.
- The action runs at most once at a time, and only when it is useful.
- The method tells the caller what happened. It can return a value, or throw an exception: an `IllegalStateException` in Java, or an `InvalidOperationException` in C#.[^1]

Liabilities:

- The method does not always do what its name says, so each caller must check the result.
- The caller must handle the refusal. A balked request is lost unless the caller tries again later.
- The test and the change of state must be one atomic step. If they are not, two threads can both pass the test.

This program tests the flag and sets it in two separate steps without a lock. A barrier makes the second thread run its test between the test and the change of the first thread:

```python wrong id=check-then-set
import threading

saving = False
running = 0
most_running = 0
both_checked = threading.Barrier(2)
counter_lock = threading.Lock()


def save():
    global saving, running, most_running
    # Wrong: the check and the change of state are not one atomic step.
    if saving:
        return False
    both_checked.wait()  # here a second thread runs the same check
    saving = True
    with counter_lock:
        running += 1
        most_running = max(most_running, running)
    both_checked.wait()  # stay in the save until both are inside
    with counter_lock:
        running -= 1
    saving = False
    return True


threads = [threading.Thread(target=save) for _ in range(2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("saves running at once:", most_running)
```

```output for=check-then-set
saves running at once: 2
```

## When not to use it

Do not balk when the caller needs the action to happen. Then it must wait, or queue the request. Guarded suspension is used when the wait for the right state is finite and reasonable. If the wait would be indefinite or unacceptably long, the balking pattern may be preferred.[^2]

## Variants and related patterns

- **Throw instead of return.** Java's `IllegalStateException` signals that a method has been invoked at an illegal or inappropriate time.[^3] .NET's `InvalidOperationException` is thrown when a method call is invalid for the object's current state.[^4] Python's `Thread.start()` raises a `RuntimeError` if it is called more than once on the same thread object.[^5]
- **Try-lock.** A lock that balks: Python's `Lock.acquire(blocking=False)` does not block. If a call with blocking set to True would block, it returns False immediately.[^5]

```python run id=try-lock
import threading

lock = threading.Lock()
print("first acquire:", lock.acquire(blocking=False))
print("second acquire while held:", lock.acquire(blocking=False))
lock.release()
print("acquire after release:", lock.acquire(blocking=False))
lock.release()
```

```output for=try-lock
first acquire: True
second acquire while held: False
acquire after release: True
```

- Go's `TryLock` tries to lock a mutex and reports whether it succeeded. The Go documentation notes that correct uses of `TryLock` are rare, and its use is often a sign of a deeper problem.[^6]

[Guarded suspension](guarded-suspension) is the alternative: it waits for the state instead of refusing the call.[^1]

## Known uses

- Java code that throws `IllegalStateException` when the application is not in an appropriate state for the requested operation.[^3]
- .NET methods that throw `InvalidOperationException` for the current state of an object.[^4]
- Python `Thread.start()` and `Lock.acquire(blocking=False)`.[^5]
- Go `Mutex.TryLock`.[^6]

## References
