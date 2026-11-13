---
tag: 02BA2X
kind: pattern
slug: barrier-pattern
name: Barrier synchronization
aliases: [barrier, barrier pattern, cyclic barrier, rendezvous of threads, phase barrier]
area: software-design/concurrency-patterns
level: advanced
summary: >-
  A barrier makes each thread of a group stop at one point until all the threads of the group arrive. Then all continue, so the work runs in phases and each phase sees the results of the last one.
family: concurrency
polarity: positive
intent: Split a parallel computation into phases, and make sure that no thread starts the next phase until every thread has finished the current one.
also_known_as: [Barrier, Rendezvous]
see_also: [fork-join, guarded-suspension, monitor-object, threads, data-races, future-pattern, producer-consumer, thread-pool]
sources:
  - url: https://en.wikipedia.org/wiki/Barrier_(computer_science)
    title: Barrier (computer science), Wikipedia
    locator: lead
  - url: https://docs.python.org/3/library/threading.html#barrier-objects
    title: threading, Thread-based parallelism, Python documentation
    locator: Barrier Objects
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/CyclicBarrier.html
    title: Java SE 21 API, java.util.concurrent.CyclicBarrier
    locator: the class description
  - url: https://en.wikipedia.org/wiki/Bulk_synchronous_parallel
    title: Bulk synchronous parallel, Wikipedia
    locator: The model; History
  - url: https://doc.rust-lang.org/std/sync/struct.Barrier.html
    title: The Rust Standard Library, std::sync::Barrier
    locator: the type description; wait
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Phaser.html
    title: Java SE 21 API, java.util.concurrent.Phaser
    locator: the class description
---

## Intent

A barrier for a group of threads or processes means that all of them stop at that point and do not proceed until all the others reach the barrier.[^1] A program puts barriers between the phases of a parallel computation, so that each phase starts only when the phase before it is complete.

## Problem

Some parallel algorithms work in steps. In each step, every thread reads values that the other threads wrote in the step before, and then writes new values. If one thread writes before another thread has read, the reader sees a value from the wrong step. If one thread starts the next step early, it reads values that are not yet written. A lock on each value does not help, because the problem is not two writes at once but the order of the steps.

## Solution

Give the group one barrier object that knows the number of parties. Each thread calls `wait` on the barrier at the end of a phase. The call blocks until all the threads have made their `wait()` calls, and then the threads are released simultaneously.[^2] The barrier can be used again for the next phase. A Java `CyclicBarrier` is called cyclic because it can be re-used after the waiting threads are released.[^3]

```text
 worker 0   worker 1   ...  worker 7
   read       read            read        phase 1: read the last step's values
 ───┴──────────┴───── barrier ──┴───      all 8 arrive, then all continue
   write      write           write       phase 2: write the new values
 ───┴──────────┴───── barrier ──┴───
   read       read            read        next step
```

A barrier can also run an action once per trip. A `CyclicBarrier` action runs after the last thread in the party arrives, but before any threads are released, which is useful for updating shared state before any of the parties continue.[^3]

This structure is the base of the bulk synchronous parallel model. There, a computation proceeds in a series of global supersteps, and each superstep ends with a barrier synchronization: when a process reaches the barrier, it waits until all other processes have reached the same barrier.[^4]

## Example

Eight workers compute the prefix sums of the numbers 1 to 8. Worker `i` owns value `i`. In each of three steps, with the distances 1, 2 and 4, each worker reads the value that is that distance to its left, and then adds it to its own value. One barrier separates the reads from the writes, and a second barrier separates the writes from the next reads. The barrier action counts the trips: two per step.

```text
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- The phases are clear in the code. Each phase can read the results of the last phase without a lock, because no thread writes while the others read.
- The threads stay alive across the phases, so the program does not start new threads for each step.
- One thread can do the work that comes between phases. The return value of Python's `wait()` is an integer in the range 0 to parties – 1, different for each thread, which can be used to select a thread to do some special housekeeping.[^2]

Liabilities:

- Every phase is as slow as its slowest thread, because all the other threads wait at the barrier for it.
- The number of parties must be right. If one thread does not arrive, the others wait. Java's `CyclicBarrier` uses an all-or-none breakage model: if a thread leaves a barrier point because of interruption, failure, or timeout, all other threads waiting at that barrier point also leave abnormally.[^3]
- The fixed number of parties does not suit work whose size changes. Python's barrier is for use by a fixed number of threads that need to wait for each other.[^2]

In this program, worker 3 fails before it reaches the barrier. The other three workers wait for a party that will not come:

```python wrong id=a-party-never-arrives
import threading

barrier = threading.Barrier(4, timeout=0.5)
lock = threading.Lock()
broken = 0


def worker(i):
    global broken
    try:
        if i == 3:
            raise ValueError("bad input")  # this worker fails before the barrier
        barrier.wait()
    except ValueError:
        print("worker 3 failed before the barrier")
        # Wrong: the worker leaves, and the barrier still waits for 4 parties.
    except threading.BrokenBarrierError:
        with lock:
            broken += 1


threads = [threading.Thread(target=worker, args=(i,)) for i in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("workers that waited until the timeout broke the barrier:", broken)
```

```output for=a-party-never-arrives
worker 3 failed before the barrier
workers that waited until the timeout broke the barrier: 3
```

The three workers end only because the barrier has a timeout. A worker that cannot continue should call `abort()`, which puts the barrier into a broken state so that waiting and future calls to `wait()` fail with `BrokenBarrierError`.[^2] Python's documentation describes this as a way to avoid deadlocking the application.[^2]

## When not to use it

Do not use a barrier when the threads do not depend on each other's results between steps. A [thread pool](thread-pool) or one [fork-join](fork-join) step is simpler. Do not use a barrier to wait for one result. A [future](future-pattern) does that. Do not use a barrier when the number of threads changes during the computation, unless the barrier supports registration, as the Phaser below does.

## Variants and related patterns

- **Leader of a phase.** One thread does the work between phases. Rust's `Barrier::wait` gives a single, arbitrary thread a result that returns true from `is_leader()`, and all other threads get false.[^5] Python gives each thread an index:

```python run id=one-thread-per-phase
import threading

barrier = threading.Barrier(4, timeout=5)
lock = threading.Lock()
chosen = [0, 0, 0]  # how many threads got index 0 in each phase


def worker():
    for phase in range(3):
        i = barrier.wait()  # an index from 0 to 3, different for each thread
        if i == 0:  # one thread per phase does the housekeeping
            with lock:
                chosen[phase] += 1


threads = [threading.Thread(target=worker) for _ in range(4)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("phases:", len(chosen))
print("threads that got index 0 in each phase:", chosen)
```

```output for=one-thread-per-phase
phases: 3
threads that got index 0 in each phase: [1, 1, 1]
```

- **Latch.** The term latch is sometimes used for a barrier that starts in the raised state and cannot be re-raised once it is in the lowered state.[^1] A count-down latch is a latch that is automatically lowered once a predetermined number of threads or processes have arrived.[^1] A latch is used once. A cyclic barrier is used for many phases.
- **Phaser.** A Java `Phaser` is a reusable synchronization barrier, similar to `CyclicBarrier` and `CountDownLatch` but supporting more flexible usage.[^6] Unlike other barriers, the number of parties registered to synchronize on a phaser may vary over time.[^6] Phasers may be tiered, constructed in tree structures, to reduce contention.[^6]
- **Implicit barriers.** Many collective routines and directive-based parallel languages impose implicit barriers, and in message passing, any global communication, such as reduction or scatter, may imply a barrier.[^1]

Each barrier `wait` is a form of [guarded suspension](guarded-suspension): the guard is that all parties have arrived. The Go example builds a barrier from a [monitor object](monitor-object) with a mutex, a condition variable and a generation counter.

## Known uses

- Python `threading.Barrier`, with an optional action and timeout.[^2]
- Java `CyclicBarrier` and `Phaser`.[^3][^6]
- Rust `std::sync::Barrier`.[^5]
- The barrier synchronization at the end of each superstep of the bulk synchronous parallel model, which Leslie Valiant developed at Harvard University during the 1980s.[^4]

## References
