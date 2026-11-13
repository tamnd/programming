---
tag: 1PF1PG
kind: pattern
slug: scheduler-pattern
name: Scheduler
aliases: [scheduler pattern, scheduler, access scheduler, scheduling policy object]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  A scheduler object decides which waiting thread uses a shared resource next. Threads call enter and done, and one policy, such as priority or first come first served, sets the order.
family: concurrency
polarity: positive
intent: Control the order in which threads that wait for a shared resource get it, and keep the ordering policy in one object, separate from the threads and from the resource.
also_known_as: [Scheduling policy]
see_also: [active-object, guarded-suspension, monitor-object, mutual-exclusion, read-write-lock, thread-pool, strategy-pattern, producer-consumer]
sources:
  - url: https://en.wikipedia.org/wiki/Scheduling_(computing)
    title: Scheduling (computing), Wikipedia
    locator: Scheduling disciplines; First come, first served; Fixed-priority pre-emptive scheduling
  - url: https://docs.python.org/3/library/threading.html#lock-objects
    title: threading, Thread-based parallelism, Python documentation
    locator: Lock Objects
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/locks/ReentrantLock.html
    title: Java SE 21 API, java.util.concurrent.locks.ReentrantLock
    locator: the class description
  - url: https://en.wikipedia.org/wiki/Active_object
    title: Active object, Wikipedia
    locator: lead; the list of elements
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Semaphore.html
    title: Java SE 21 API, java.util.concurrent.Semaphore
    locator: the class description
---

## Intent

Scheduling deals with the problem of deciding which of the outstanding requests is to be allocated resources.[^1] The Scheduler pattern puts that decision in one object. Threads ask the scheduler for a shared resource, and the scheduler decides which waiting thread gets it next.

## Problem

Several threads wait for one resource, for example a printer. A plain lock does not say which waiting thread gets the resource when it becomes free. In Python, when more than one thread is blocked in `acquire()`, only one thread proceeds when a `release()` call unlocks the lock, and which one of the waiting threads proceeds is not defined, and may vary across implementations.[^2] A Java `ReentrantLock` without its fairness setting does not guarantee any particular access order.[^3]

Some programs need an order: urgent jobs first, or each job in its turn. A lock alone cannot give that order, and code that sorts the threads in each caller is hard to change.

## Solution

Put a scheduler object between the threads and the resource. A thread calls `enter` before it uses the resource and `done` after it. If the resource is free and nobody waits, `enter` returns at once. If not, the scheduler adds the thread to its waiting list and the thread sleeps. When the thread that holds the resource calls `done`, the scheduler applies its policy to the waiting list, chooses one thread, and wakes it. The resource stays busy and passes directly to the chosen thread, so no other thread can take it in between.

```text
 job threads                scheduler                      printer
 report(2) ── enter ──►  waiting: report, invoice,
 invoice(1) ─ enter ──►           photo, label
 photo(3) ─── enter ──►           │
 label(1) ─── enter ──►           │
 first ────── done ───►  policy: lowest number, then first to arrive
                          └─► wake invoice ─────────────►  invoice
 invoice ──── done ───►  └─► wake label ───────────────►  label
```

The policy is the only part that knows the order. In the [Active Object](active-object) pattern, one of the elements is a scheduler, which decides which request to execute next.[^4]

## Example

The first job holds the printer. Four jobs then arrive in a known order: report with priority 2, invoice with priority 1, photo with priority 3 and label with priority 1. The policy takes the lowest priority number first, and the first to arrive among jobs with the same priority. Invoice and label share priority 1, so invoice prints before label because it arrived first. Only one job uses the printer at a time.

```text
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

The examples for each language show the same program.

## Consequences

Benefits:

- The order is set in one place. A different policy changes the order for every thread, and the threads do not change. The policy can be a [strategy](strategy-pattern) object.
- The scheduler can give a guarantee that a plain lock does not give. With first in, first out, there is no starvation, because each process gets a chance to be executed after a definite time.[^1]
- The main purposes of scheduling algorithms are to minimize resource starvation and to ensure fairness amongst the parties utilizing the resources.[^1] A scheduler object lets a program choose its own balance of these goals.

Liabilities:

- A policy can starve a thread. With fixed priorities, processes in lower-priority queues are selected only when all of the higher-priority queues are empty.[^1]
- Fairness costs speed. Programs using fair locks accessed by many threads may display lower overall throughput than those using the default setting, but have smaller variances in times to obtain locks and guarantee lack of starvation.[^3]
- The scheduler controls only access to the resource. Fairness of locks does not guarantee fairness of thread scheduling.[^3]
- Every thread must call `done`. A thread that fails before `done` keeps the resource busy, and every waiting thread sleeps forever.

This program applies a strict priority policy. One photo job waits with priority 3, and each time a job ends, a new invoice job with priority 1 arrives:

```python wrong id=priority-starves-a-job
# A strict priority policy. Each time a job ends, a new urgent job arrives.
waiting = [(1, "invoice 1"), (3, "photo")]
run = []
for n in range(2, 12):
    job = min(waiting)  # always the lowest priority number
    waiting.remove(job)
    run.append(job[1])
    waiting.append((1, f"invoice {n}"))  # a new urgent job arrives

print("jobs run:", len(run))
print("photo job run:", "photo" in run)
print("jobs still waiting:", [name for _, name in sorted(waiting)])
```

```output for=priority-starves-a-job
jobs run: 10
photo job run: False
jobs still waiting: ['invoice 11', 'photo']
```

The photo job never runs while urgent jobs keep arriving. Add a limit to the wait, raise the priority of a job that waits long, or use first in, first out.

## When not to use it

Do not use a scheduler when the order does not matter. A plain lock is simpler and does not keep a waiting list. If the policy that you need is first in, first out, a fair lock or semaphore from the library can be enough: a Java `Semaphore` with fairness set to true guarantees that threads invoking any of the acquire methods are selected to obtain permits in the order in which their invocation of those methods was processed.[^5] If the work can wait in a queue as data instead of as sleeping threads, a priority queue in front of a [thread pool](thread-pool) can be simpler.

## Variants and related patterns

- **First in, first out.** FIFO, also known as first come, first served (FCFS), is the simplest scheduling algorithm, and it queues processes in the order that they arrive in the ready queue.[^1] Its cost is that long processes can make short processes wait for a long time, which is known as the convoy effect.[^1] This program gives the same arrivals as the wrong program to a FIFO policy:

```python run id=first-in-first-out
from collections import deque

# The same arrivals with a first-in, first-out policy.
waiting = deque(["invoice 1", "photo"])
run = []
for n in range(2, 12):
    run.append(waiting.popleft())  # always the job that waited longest
    waiting.append(f"invoice {n}")

print("jobs run:", len(run))
print("photo job run, as job number:", run.index("photo") + 1)
print("jobs still waiting:", list(waiting))
```

```output for=first-in-first-out
jobs run: 10
photo job run, as job number: 2
jobs still waiting: ['invoice 10', 'invoice 11']
```

- **Priority levels.** If the number of priority rankings is limited, fixed-priority scheduling can be characterized as a collection of FIFO queues, one for each priority ranking.[^1] The example uses this policy.
- **Fair locks and semaphores.** The library puts a FIFO scheduler inside the lock. When the fairness parameter of a Java `ReentrantLock` is true, the lock favors granting access to the longest-waiting thread under contention.[^3] When the fairness of a Java `Semaphore` is false, barging is permitted, so a thread that calls `acquire()` can get a permit ahead of a thread that has been waiting.[^5]
- **Readers and writers.** A [read-write lock](read-write-lock) is a scheduler with a policy for two kinds of request, and its preference for readers or for writers is a scheduling decision.

Each waiting thread in `enter` uses [guarded suspension](guarded-suspension): its guard is that the scheduler chose it. A [monitor object](monitor-object) gives the lock and the condition variable that the scheduler uses.

## Known uses

- The scheduler element of the [Active Object](active-object) pattern.[^4]
- Java `ReentrantLock` and `Semaphore` in fair mode.[^3][^5]
- Scheduling disciplines in operating systems for threads and processes, and in disk drives and printers.[^1]

## References
