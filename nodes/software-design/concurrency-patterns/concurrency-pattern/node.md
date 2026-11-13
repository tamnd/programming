---
tag: ZN3FT7
kind: concept
slug: concurrency-pattern
name: Concurrency pattern
aliases: [concurrency patterns, concurrency design pattern, patterns for concurrent objects, synchronization pattern]
area: software-design/concurrency-patterns
level: core
summary: >-
  A concurrency pattern is a design pattern for work that runs at the same time. Each one answers the same question: which code may touch which state, and when.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2000
  people: [Douglas C. Schmidt, Michael Stal, Hans Rohnert, Frank Buschmann]
  note: POSA2 (2000), the second volume of the Pattern-Oriented Software Architecture series, put 17 patterns for concurrent and networked objects into groups for event handling, synchronization and concurrency. POSA1 appeared in 1996.
see_also: [monitor-object, active-object, reactor, thread-pool, producer-consumer, thread-confinement, mutual-exclusion, data-races]
same_as:
  wikipedia: Concurrency_pattern
sources:
  - url: https://en.wikipedia.org/wiki/Concurrency_pattern
    title: Concurrency pattern, Wikipedia
    locator: Introduction; Examples
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Overview, the 17 patterns; Table of Contents, chapters 2 to 5
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Concurrency, Share by communicating
  - url: https://doc.rust-lang.org/book/ch16-00-concurrency.html
    title: The Rust Programming Language, 16 Fearless Concurrency
    locator: introduction
  - url: https://doc.rust-lang.org/book/ch16-04-extensible-concurrency-sync-and-send.html
    title: The Rust Programming Language, 16.4 Extensible Concurrency with Send and Sync
    locator: Transferring Ownership Between Threads; Accessing from Multiple Threads
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: introduction, CPython implementation detail
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/package-summary.html
    title: Package java.util.concurrent, Java SE 21 API
    locator: package description; Synchronizers; Concurrent Collections
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Execution_model
    title: JavaScript execution model, MDN Web Docs
    locator: Job queue and event loop; Run-to-completion
---

## Idea

When two pieces of work run at the same time, they can touch the same state. A read by one thread and a write by another can interleave in an order that the programmer did not expect, and the result is wrong only sometimes. Wikipedia describes a concurrency pattern as a design pattern that supports concurrent processing.[^1] Each pattern of this sub-area gives a fixed answer to one question: which code may touch which state, and when.

There are three basic answers. Guard the shared state with a lock. Give the state to one owner, and let the others send it messages. Or do not share the state at all: each piece of work makes its own result, and one place combines the results. The first and the third answer are in this program:

```python run id=idea
import threading
from concurrent.futures import ThreadPoolExecutor

total = 0
lock = threading.Lock()


def add_shared(n):  # every task writes the same variable, so a lock guards it
    global total
    for _ in range(n):
        with lock:
            total += 1


def count_own(n):  # every task returns its own result, so nothing is shared
    return sum(1 for _ in range(n))


with ThreadPoolExecutor(max_workers=4) as pool:
    list(pool.map(add_shared, [1000] * 4))
    results = list(pool.map(count_own, [1000] * 4))

print("shared variable with a lock:", total)
print("own results, combined by the caller:", sum(results))
```

```output for=idea
shared variable with a lock: 4000
own results, combined by the caller: 4000
```

## How it works

A concurrency pattern names the parts of the program that run at the same time, the state that each part may use, and the rule that keeps the use safe. The rule is usually one of the three answers above, or a mix of them.

POSA2 presents 17 interrelated patterns for concurrent and networked software, and it puts them in groups.[^2] Three of its groups are about concurrency:

| Group | Problem | Patterns |
|---|---|---|
| Event handling | one thread must react to many sources of events | [[reactor]], [[proactor]], [[asynchronous-completion-token]], [[acceptor-connector]] |
| Synchronization | shared objects must stay correct when threads call them | [[scoped-locking]], [[strategized-locking]], [[thread-safe-interface]], [[double-checked-locking]] |
| Concurrency | the program must decide which threads exist and what each one does | [[active-object]], [[monitor-object]], [[half-sync-half-async]], [[leader-followers]], [[thread-specific-storage]] |

Other patterns in this sub-area come from other sources. Wikipedia also lists balking, barrier, guarded suspension, the readers-writer lock, the scheduler, the thread pool and thread-local storage.[^1] [[producer-consumer]], [[thread-pool]], [[fork-join]] and [[master-worker-pattern]] divide work among threads. [[immutable-object]], [[copy-on-write]] and [[thread-confinement]] remove the sharing, so that no lock is needed.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | `threading.Lock` and `Condition`; `queue.Queue` for messages; `asyncio` for an event loop; the GIL in CPython |
| JavaScript | native | one thread for each agent; an event loop; each job runs to completion; workers that share nothing by default |
| TypeScript | native | the model of JavaScript, with types for promises and messages |
| Java | native | `synchronized` methods and blocks; `wait` and `notify`; the `java.util.concurrent` package |
| Kotlin | idiom | the threads and locks of the JVM; suspend functions with a coroutine library |
| C# | native | the `lock` statement; `async` and `await`; concurrent collections and `BlockingCollection` |
| Go | native | goroutines, channels and `select`; `sync.Mutex` and `sync.WaitGroup` |
| Rust | native | the `Send` and `Sync` traits, checked by the compiler; `Mutex`, channels and scoped threads in `std` |
| C | idiom | a thread library such as POSIX threads; mutexes and condition variables |
| C++ | idiom | `std::thread`, `std::mutex`, `std::condition_variable` and `std::atomic` in the standard library |
| Bash | idiom | background processes with `&` and `wait`; pipes between processes |
| PowerShell | idiom | thread jobs and parallel loops; the locks and collections of .NET |
| SQL | idiom | transactions, isolation levels and row locks for concurrent sessions |
| Regex | n/a | a pattern has no threads or shared state |
| HTML | n/a | markup has no threads or shared state |
| CSS | n/a | style rules have no threads or shared state |

The languages differ in which answer they make easy. Go prefers the second answer. Effective Go reduces it to a slogan: "Do not communicate by sharing memory; instead, share memory by communicating."[^3] When a value goes through a channel, only one goroutine has access to the value at any given time.[^3] The same text says that this approach can be taken too far, and that a reference count may be best done with a mutex around an integer.[^3]

Rust checks the rules when it compiles the program. Its book calls this fearless concurrency, and it covers message-passing concurrency and shared-state concurrency as two separate topics.[^4] The `Send` trait marks a type whose values can be transferred to another thread, and `Sync` marks a type that is safe to reference from several threads.[^5] The compiler rejects a program that moves a value to another thread when the type of the value is not `Send`.

In CPython, the Global Interpreter Lock lets only one thread execute Python code at once.[^6] This does not make a read followed by a write safe, as the Pitfalls section shows. Java collects its tools in `java.util.concurrent`, which it describes as "Utility classes commonly useful in concurrent programming." The package has synchronizers and concurrent collections.[^7] JavaScript avoids the problem in a different way. An agent runs one job at a time, and each job is processed completely before any other job is processed.[^8] A task that waits between a read and a write can still lose an update.

The example files show the three answers in each language: a lock around a shared counter, one owner thread that receives messages, and a result for each thread that is combined after the join.

## Variations and related ideas

- **Lock-free and atomic designs.** An atomic operation changes one value without a lock. See [[atomic-operations]]. It removes waiting, but it is hard to extend to more than one value.
- **Event-driven designs.** An event loop runs many tasks on one thread. The tasks take turns, so most state needs no lock. See [[reactor]] and [[event-loop-pattern]].
- **Message passing between processes.** When the parts do not share memory, every exchange is a message. See [[message-passing-concept]] and [[channels]].
- **Distributed patterns.** Some of the same ideas, such as a queue between producers and workers, also work between machines. Then the messages can be lost or repeated, and the patterns must also handle failure.

## Pitfalls

- **Check, then act.** A thread reads a value, and then it writes a new value that depends on the read. Another thread can write between the two steps, and one update is lost:

```python wrong id=lost-update
import threading
import time

count = 0


def add(_):
    global count
    for _ in range(100):
        value = count  # read
        time.sleep(0.001)  # another thread can run between the read and the write
        count = value + 1  # write: an update of another thread can be lost


threads = [threading.Thread(target=add, args=(i,)) for i in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("expected:", 4 * 100)
print("updates lost:", count < 400)
```

```output for=lost-update
expected: 400
updates lost: True
```

The read and the write must be one step: put both inside one lock, give the counter to one owner, or give each thread its own counter. The three answers are in the example files.

- **Locks in the wrong order.** Two threads that take two locks in different orders can wait for each other for ever. See [[deadlock]].
- **A lock that is too wide.** A lock around slow work, such as I/O, makes the other threads wait, and the program runs as if it had one thread.
- **Forgotten sharing.** A data race happens when two threads access one location, at least one access is a write, and nothing orders them. See [[data-races]].

## Cost

A lock costs time each time that a thread takes it, and more when threads must wait for each other. A message costs a copy or a move and a queue operation. A result for each thread costs memory for each thread and one step to combine the results. A design that shares less state usually needs fewer locks and scales better, but it can need more memory and more copies. The largest cost is often in the design: a pattern that is easy to check by reading the code prevents bugs that tests find only sometimes.

## History

The second volume of the Pattern-Oriented Software Architecture series, POSA2, was published by Wiley & Sons in 2000. Its authors are Douglas C. Schmidt, Michael Stal, Hans Rohnert and Frank Buschmann. POSA1 was published in 1996.[^2] POSA2 presents 17 patterns that address service access and configuration, event handling, synchronization and concurrency.[^2] The list of concurrency patterns in Wikipedia also has patterns that are not in POSA2, such as balking, the barrier and the thread pool.[^1]

## References
