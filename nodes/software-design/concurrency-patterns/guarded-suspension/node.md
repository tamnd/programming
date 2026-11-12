---
tag: GA9K7G
kind: pattern
slug: guarded-suspension
name: Guarded suspension
aliases: [guarded suspension, guarded wait, guarded method, wait until a condition holds]
area: software-design/concurrency-patterns
level: core
summary: >-
  Guarded suspension makes a method wait until it holds a lock and its precondition is true. The thread sleeps on a condition variable and checks the guard again in a loop each time it wakes up.
family: concurrency
polarity: positive
intent: Run an operation only when the object is in a state that allows it, and suspend the calling thread until another thread changes the state so that the precondition holds.
also_known_as: [Guarded wait]
history:
  year: 2000
  people: [Doug Lea]
  note: Wikipedia cites the second edition of "Concurrent Programming in Java" by Doug Lea (2000) for the name of the pattern.
see_also: [monitor-object, producer-consumer, read-write-lock, thread-safe-interface, scoped-locking, mutual-exclusion, deadlock, channels]
sources:
  - url: https://en.wikipedia.org/wiki/Guarded_suspension
    title: Guarded suspension, Wikipedia
    locator: lead; Usage; Implementation
  - url: https://doc.rust-lang.org/std/sync/struct.Condvar.html
    title: The Rust Standard Library, std::sync::Condvar
    locator: the type description; wait; wait_while
  - url: https://docs.python.org/3/library/threading.html#condition-objects
    title: threading, Thread-based parallelism, Python documentation
    locator: Condition Objects
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Java SE 21 API, java.lang.Object
    locator: wait(long, int)
  - url: https://pkg.go.dev/sync#Cond
    title: Go standard library, package sync
    locator: type Cond; func (*Cond) Wait
---

## Intent

Guarded suspension is a software design pattern for operations that need both a lock and a precondition before they can run.[^1] It suspends the method call and the calling thread until the precondition, which acts as a guard, is satisfied.[^1]

## Problem

A thread calls a method on a shared object, but the object is not yet in a state that allows the operation. For example, a `get` method on a queue must not run when there are no items in the queue.[^1] The thread could poll the state in a loop, but then it uses processor time and must still take the lock each time it checks.

## Solution

Take the lock and test the guard. If the guard is false, wait on a condition variable, which releases the lock while the thread sleeps. A thread that changes the state notifies the waiting threads, and each one takes the lock again and tests its guard again.[^1] A condition variable lets a thread block so that it consumes no CPU time while it waits for an event.[^2]

```text
 caller                          object (lock + condition variable)
   │ take(8)                        │
   ├──► lock ──► guard: level >= 8? │
   │             no  ──► wait ◄─────┤ (lock released while asleep)
   │                                │◄── fill(2) ──► notify all
   │             wake ──► lock ──► guard again
   │             yes ──► level -= 8 ──► unlock ──► return
```

Test the guard in a `while` loop, not in an `if`. In Python, the loop is necessary because `wait()` can return after an arbitrary long time, and the condition that caused the `notify()` call may no longer hold true.[^3] In Java, a thread can also wake up without being notified, interrupted, or timing out, a so-called spurious wakeup.[^4]

## Example

A tank starts empty. Three threads take 5, 3 and 8 units, and each one waits until the tank has enough. The main thread fills the tank with 2 units 10 times and notifies all the waiting takers after each fill. All three takers are served, the level is never negative, and 4 units are left.

```text
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

The examples for each language show the same program.

## Consequences

Benefits:

- A waiting thread does not use the processor, because it blocks on the condition variable until a notification.[^2]
- The guard and the operation run under the same lock, so no other thread can change the state between the test and the operation.
- The caller does not need to know when the state changes. In Java, the `wait()` and `notify()` methods of the `Object` class assist with guarded suspension.[^1]

Liabilities:

- The pattern blocks. If a method call is suspended for too long, the overall program slows down or stops while it waits for the precondition.[^1]
- Every guard must be checked again each time the wait returns, to protect against spurious wakeups.[^2] An `if` in place of the loop is a bug that tests may not show.
- A thread that changes the state must notify the waiting threads, or they wait forever. Each `Cond` in Go has an associated Locker, which must be held when the condition changes and when `Wait` is called.[^5]

This program tests the guard with `if`. Two takers wait for 5 units, the main thread adds 5 units and notifies both, and both take 5:

```python wrong id=if-instead-of-while
import threading

cond = threading.Condition()
level = 0
waiting = 0


def take(amount):
    global level, waiting
    with cond:
        # Wrong: "if" checks the guard only once, before the wait.
        if level < amount:
            waiting += 1
            cond.wait()
        level -= amount


takers = [threading.Thread(target=take, args=(5,)) for _ in range(2)]
for t in takers:
    t.start()
while True:  # wait until both takers wait
    with cond:
        if waiting == 2:
            level += 5  # enough for one taker only
            cond.notify_all()
            break
for t in takers:
    t.join()
print("level after both takers:", level)
```

```output for=if-instead-of-while
level after both takers: -5
```

With `while level < amount`, the second taker tests the guard again, finds a level of 0, and waits again.

## When not to use it

Use guarded suspension only when the call will be suspended for a finite and reasonable period of time.[^1] If the suspension can be indefinite or unacceptably long, the [balking pattern](balking-pattern) may be preferred.[^1] In Go, many simple uses are better served by channels than a `Cond`.[^5]

## Variants and related patterns

- **Wait with a timeout.** The guarded method gives up after a time limit and reports that it was not served. Python's `wait_for()` automates the condition check and eases the computation of timeouts.[^3]

```python run id=guard-with-timeout
import threading

cond = threading.Condition()
level = 2


def take(amount, timeout):
    """Wait at most timeout seconds for the guard. Return True if served."""
    global level
    with cond:
        if not cond.wait_for(lambda: level >= amount, timeout):
            return False
        level -= amount
        return True


print("take 5 with a level of 2, served:", take(5, timeout=0.2))
print("level:", level)
print("take 2 with a level of 2, served:", take(2, timeout=0.2))
print("level:", level)
```

```output for=guard-with-timeout
take 5 with a level of 2, served: False
level: 2
take 2 with a level of 2, served: True
level: 0
```

- **Predicate wait.** Some libraries put the loop inside one call. Rust's `wait_while` blocks the thread until the condition becomes false: it checks the condition at once, and if it is not met, it waits for the next notification and checks again.[^2]
- **Notify one or notify all.** Notify one waiting thread when one state change can be interesting for only one waiting thread, as when a producer adds one item to a buffer for one consumer.[^3] Notify all when the waiting threads have different guards, as the takers in the example do.

A queue whose `get` waits while the queue is empty is guarded suspension, and it is the consumer side of [producer-consumer](producer-consumer).[^1] A [monitor object](monitor-object) puts guarded methods behind one lock for the whole object.

## Known uses

- Java `Object.wait()` and `notify()` inside `synchronized` methods.[^1]
- Python `threading.Condition` with `wait()` and `wait_for()`.[^3]
- Go `sync.Cond` with `Wait`, `Signal` and `Broadcast`.[^5]
- Rust `std::sync::Condvar` with `wait` and `wait_while`.[^2]

## References
