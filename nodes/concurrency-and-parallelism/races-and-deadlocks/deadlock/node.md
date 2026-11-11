---
tag: YJ5FP2
kind: concept
slug: deadlock
name: Deadlock
aliases: [deadly embrace, circular wait]
area: concurrency-and-parallelism/races-and-deadlocks
level: core
summary: >-
  A deadlock is a cycle of waits. Each thread in the cycle holds a resource and waits for a resource that the next thread holds, so no thread in the cycle can continue.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1968
  people: [Edsger W. Dijkstra, J. W. Havender, Edward G. Coffman Jr., M. J. Elphick, Arie Shoshani]
  note: Dijkstra described the deadly embrace and the banker's algorithm in Cooperating Sequential Processes (published 1968). Havender described lock ordering in OS/360 (1968). Coffman, Elphick and Shoshani stated the four conditions in 1971.
see_also: [threads, data-races, mutual-exclusion, channels, async-await]
same_as:
  wikidata: Q623276
  wikipedia: Deadlock_(computer_science)
sources:
  - paper: doi:10.1145/356586.356588
    title: E. G. Coffman Jr., M. J. Elphick and A. Shoshani, System Deadlocks, ACM Computing Surveys 3(2), 1971, pp. 67 to 78
    locator: 1 Introduction; 2 Characterizations of Deadlocks; 3 Prevention of Deadlock; 4 Deadlock Detection and Recovery; 5 Avoiding Deadlocks Using Information on Resource Requirements; reference 4 (Havender)
  - url: https://docs.python.org/3.14/library/threading.html#lock-objects
    title: Python 3.14 documentation, threading
    locator: Lock objects, acquire with blocking and timeout; RLock objects
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.lock.algorithm
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.lock.algorithm], paragraph 5"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.lock.scoped
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.lock.scoped], paragraph 2"
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/locks/ReentrantLock.html
    title: Java SE 25 API, Class ReentrantLock
    locator: class description (fairness); tryLock(long, TimeUnit)
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.monitor.tryenter#system-threading-monitor-tryenter(system-object-system-timespan)
    title: .NET API reference, Monitor.TryEnter
    locator: TryEnter(Object, TimeSpan), Returns
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_lock.html
    title: POSIX.1-2024, pthread_mutex_lock
    locator: DESCRIPTION (the table of mutex types); RETURN VALUE; ERRORS
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.timedmutex.requirements.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.timedmutex.requirements.general], paragraphs 4 and 7"
  - url: https://doc.rust-lang.org/std/sync/struct.Mutex.html#method.lock
    title: Rust standard library, Struct Mutex
    locator: lock, the paragraph on a lock in a thread that already holds it; try_lock, Errors
  - url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD01xx/EWD123.html
    title: Edsger W. Dijkstra, Cooperating Sequential Processes (EWD 123), in F. Genuys (ed.), Programming Languages, Academic Press, 1968
    locator: table of contents, 6 The Problem of the Deadly Embrace; 6.1 The Banker's Algorithm
  - url: https://www.cs.utexas.edu/~EWD/ewd06xx/EWD623.PDF
    title: Edsger W. Dijkstra, The mathematics behind the Banker's Algorithm (EWD 623), 1977
    locator: the whole note
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.management/java/lang/management/ThreadMXBean.html#findDeadlockedThreads()
    title: Java SE 25 API, Interface ThreadMXBean
    locator: findDeadlockedThreads()
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/jstack.html
    title: Java SE 25 tool specifications, The jstack Command
    locator: Description; Options, -l
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 17.1 Synchronization
  - url: https://github.com/golang/go/blob/go1.27.3/src/runtime/proc.go#L6414-L6519
    title: Go runtime source, src/runtime/proc.go at go1.27.3
    locator: checkdead, lines 6414 to 6519
  - url: https://www.postgresql.org/docs/18/explicit-locking.html#LOCKING-DEADLOCKS
    title: PostgreSQL 18 documentation, Explicit Locking
    locator: 13.3.4 Deadlocks
  - url: https://www.postgresql.org/docs/18/runtime-config-locks.html#GUC-DEADLOCK-TIMEOUT
    title: PostgreSQL 18 documentation, Lock Management
    locator: deadlock_timeout
  - url: https://doc.rust-lang.org/reference/behavior-not-considered-unsafe.html
    title: The Rust Reference, Behavior not considered unsafe
    locator: Deadlocks
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/lock
    title: C# language reference, The lock statement
    locator: the remarks on reentry, on await in a lock body, and Guidelines
  - url: https://docs.python.org/3.14/library/threading.html#threading.Thread.join
    title: Python 3.14 documentation, threading
    locator: Thread objects, join
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_join.html
    title: POSIX.1-2024, pthread_join
    locator: ERRORS, EDEADLK
  - url: https://man7.org/linux/man-pages/man3/pthread_join.3.html
    title: Linux manual page, pthread_join(3)
    locator: ERRORS, EDEADLK
  - url: https://go.dev/ref/spec#Channel_types
    title: The Go Programming Language Specification, language version go1.27
    locator: Channel types
  - url: https://nodejs.org/api/process.html#exit-codes
    title: Node.js documentation, Process
    locator: Exit codes, 13 Unsettled Top-Level Await
  - url: https://docs.oracle.com/javase/tutorial/essential/concurrency/starvelive.html
    title: The Java Tutorials, Starvation and Livelock
    locator: Starvation; Livelock
---

## Idea

A deadlock is a state in which a group of threads wait for each other in a cycle. Thread `t1` holds lock `A` and waits for lock `B`. Thread `t2` holds lock `B` and waits for lock `A`. Each thread releases its lock only after it gets the other lock, so neither wait can end. The program does not crash and prints no error. It stops making progress.

The term is wider than threads and locks. Coffman, Elphick and Shoshani apply it to devices, storage, programs and data, and they quote Dijkstra's name for it, the "deadly embrace".[^1] A database transaction, a process or an async task can be part of a deadlock too.

In Python, two threads take two locks in opposite orders. A barrier makes sure that each thread has its first lock before either thread asks for its second lock. A real deadlock waits forever. Here the `timeout` argument of `acquire` ends each wait after 0.2 s, and `acquire` returns `False`.[^2]

```python run id=idea
import threading

a = threading.Lock()
b = threading.Lock()
both_hold = threading.Barrier(2)
both_tried = threading.Barrier(2)
results = {}


def transfer(name, first, second):
    with first:
        both_hold.wait()
        got = second.acquire(timeout=0.2)
        if got:
            second.release()
        both_tried.wait()
    results[name] = got


t1 = threading.Thread(target=transfer, args=("t1", a, b))
t2 = threading.Thread(target=transfer, args=("t2", b, a))
t1.start()
t2.start()
t1.join()
t2.join()
for name in sorted(results):
    print(name, "got its second lock:", results[name])
```

```output for=idea
t1 got its second lock: False
t2 got its second lock: False
```

The second barrier keeps each thread on its first lock until both threads have tried. Without it, one thread could time out, release its lock, and let the other thread through. The output would then change from run to run. The fix is in How it works: take the locks in one global order.

## How it works

### The four conditions

Coffman, Elphick and Shoshani list four conditions that are all in effect when a deadlock exists.[^1]

1. **Mutual exclusion.** A task claims exclusive control of the resources it needs. A lock is the usual example: one thread at a time holds it.
2. **Hold and wait.** A task holds the resources that it already has while it waits for more. The paper calls this the "wait for" condition.
3. **No preemption.** Nothing can take a resource away from the task that holds it. The task gives it back only when it is done.
4. **Circular wait.** A circular chain of tasks exists, and each task holds a resource that the next task in the chain requests.

The first three conditions are properties of the resources and of the rules for their use. Most locks have all three. The fourth condition is a property of one moment in one run. A program can run correctly a thousand times and deadlock on the next run, when the threads reach their locks in an unlucky order. The paper says that the four conditions together "effectively" define a deadlock state.[^1]

### The wait-for graph

A graph shows the cycle. Coffman et al. use a state graph with one node for each resource. An arc goes from resource `Rj` to resource `Rk` when some task holds `Rj` and requests `Rk`. Take the case of one resource of each type. Then a deadlock exists at a time t if and only if the graph has a circuit at t.[^1] When there are several identical resources of a type, a circuit is necessary for a deadlock but not sufficient.

An equivalent graph puts the threads at the nodes. An arc goes from thread `t1` to thread `t2` when `t1` waits for a resource that `t2` holds. This is the wait-for graph, and a cycle in it is the deadlock. The run in Idea gives this graph:

```text
t1 --waits for B, which t2 holds--> t2
t2 --waits for A, which t1 holds--> t1
```

The order of events that leads to it:

```text
step   thread t1                    thread t2
1      lock A: granted
2                                   lock B: granted
3      lock B: wait (t2 holds B)
4                                   lock A: wait (t1 holds A)
       t1 waits for t2 and t2 waits for t1: a cycle
```

The steps can also run as 1, 3, 2, 4. Then `t1` gets both locks at step 3, and no deadlock occurs. The bug is in the program in both cases, but only some schedules show it.

### Three ways to deal with deadlock

Coffman et al. sort the treatments into three groups.[^1]

- **Prevention.** Design the program so that one of the four conditions can never hold.
- **Detection and recovery.** Let the deadlock happen, find the cycle, and break it, for example by aborting one of the tasks.
- **Avoidance.** Use advance information about the resources that each task will need, and refuse a request that could lead to a deadlock.

The sections below show each group, with the tools that languages and databases give for it.

### Prevention: lock ordering

The paper gives three approaches from Havender, and each one denies one condition. Mutual exclusion stays, because a lock exists to give exclusive access.[^1]

The third approach of Havender is a linear order of resources. A task that holds a resource may then ask only for resources that come later in the order. With this rule, the state graph cannot have a circuit, so circular wait is impossible.[^1] In code, every thread takes its locks in one global order. The key can be an account number, an address, or a rank in a lock hierarchy.

```python run id=order
import threading


class Account:
    def __init__(self, number, balance):
        self.number = number
        self.balance = balance
        self.lock = threading.Lock()


def transfer(source, target, amount):
    first, second = sorted((source, target), key=lambda acc: acc.number)
    with first.lock, second.lock:
        source.balance -= amount
        target.balance += amount


a = Account(1, 1000)
b = Account(2, 1000)


def many(source, target, amount):
    for _ in range(500):
        transfer(source, target, amount)


threads = [
    threading.Thread(target=many, args=(a, b, 1)),
    threading.Thread(target=many, args=(b, a, 2)),
]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("a:", a.balance, "b:", b.balance, "total:", a.balance + b.balance)
```

```output for=order
a: 1500 b: 500 total: 2000
```

The call `transfer(b, a, 2)` names the accounts in the opposite order, but it still locks account 1 first. Havender used this method in OS/360: the system acquired storage, devices and data sets for each job step in a fixed order.[^1] The order must be total and every code path must follow it. One function that locks in another order brings the deadlock back.

### Prevention: take all the locks at once

Havender's first approach denies hold and wait. A task requests all its resources at once and waits until it gets all of them.[^1] C++ has this in its library. `std::lock(m1, m2, ...)` locks all its arguments and "does not result in deadlock". The standard leaves its algorithm unspecified.[^3] `std::scoped_lock` calls `std::lock` in its constructor and unlocks all the mutexes in its destructor.[^4] Two threads can then name the same two mutexes in opposite orders, and the program still does not deadlock (see the C++ example).

The cost is that a task must know all its resources before it starts. The paper also notes that resources can stay allocated and unused for long periods.[^1]

### Prevention: try, back off and retry

Havender's second approach denies no preemption. A task that cannot get a further resource releases what it holds, then asks for all of it again.[^1] With locks, the program uses a try-lock or a timed lock:

| Language | Try or timed lock | Result when the lock is not free |
|---|---|---|
| Python | `lock.acquire(timeout=0.2)` | returns `False`[^2] |
| Java, Kotlin | `ReentrantLock.tryLock(200, MILLISECONDS)` | returns `false`[^5] |
| C# | `Monitor.TryEnter(obj, TimeSpan)` | returns `false`[^6] |
| C, POSIX | `pthread_mutex_trylock` | returns `EBUSY`[^7] |
| C++ | `std::timed_mutex::try_lock_for` | returns `false`[^8] |
| Rust | `Mutex::try_lock` | returns `Err(TryLockError::WouldBlock)`[^9] |

The thread that fails releases its first lock, waits a short time, and starts again. The Kotlin example does this. The wait matters: two threads that retry at the same rhythm can collide again and again (see livelock in Variations).

### Avoidance: the banker's algorithm

Avoidance needs advance information about the resource use of each task.[^1] Dijkstra described the banker's algorithm in section 6.1 of "Cooperating Sequential Processes", in the chapter on the deadly embrace.[^10] Coffman et al. describe a model of the same kind from Habermann. Each task declares the maximum number of resources of each type that it can need. A state is safe when some order exists in which all the running tasks can get their maximum and finish. The system grants a request only when the state after the grant is still safe.[^1]

The method needs the maximum needs of each task in advance, and a program that locks objects often does not know them. Dijkstra's EWD 623 gives the mathematics of the algorithm.[^11]

### Detection

Detection builds the wait-for graph and looks for a cycle. With one resource of each type, a detector updates the graph at each request, grant and release. Then it checks the graph for a circuit.[^1] Recovery then breaks the cycle, for example by aborting the tasks in the cycle one at a time until the rest can continue.[^1]

- **Java.** `ThreadMXBean.findDeadlockedThreads()` finds cycles of platform threads that wait for object monitors or for ownable synchronizers such as `ReentrantLock`. It returns the IDs of the threads, or `null`. Cycles that contain virtual threads are not found. The method is for troubleshooting, not for synchronization control, and it "might be an expensive operation".[^12] The `jstack` tool prints the stack traces of the threads of a running JVM, and `-l` adds information about locks.[^13] The Java language itself "neither prevents nor requires detection of deadlock conditions".[^14]
- **Go.** The runtime counts the threads that run Go code. When the count is zero and no timer is pending, it stops the program with `fatal error: all goroutines are asleep - deadlock!`.[^15] The check sees only the whole program. Two goroutines that wait for each other while a third goroutine still runs are not found (see the Go example).
- **Databases.** PostgreSQL detects a deadlock between transactions and aborts one of them. Which one is "difficult to predict". The documentation tells programs to acquire locks in a consistent order and to retry a transaction that was aborted.[^16] The check is "relatively expensive", so the server waits for `deadlock_timeout`, 1 s by default, before it runs it.[^17]

The Java detector finds the cycle from the Idea, with the threads still blocked. The example stops by itself because the two threads are daemon threads.

```java run id=detect
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.Arrays;
import java.util.concurrent.CountDownLatch;

public class Main {
    static Thread worker(String name, Object first, Object second, CountDownLatch ready) {
        Thread t = new Thread(() -> {
            synchronized (first) {
                ready.countDown();
                try {
                    ready.await();
                } catch (InterruptedException e) {
                    return;
                }
                synchronized (second) {
                    System.out.println(name + " has both locks");
                }
            }
        }, name);
        t.setDaemon(true);
        return t;
    }

    public static void main(String[] args) throws InterruptedException {
        Object a = new Object();
        Object b = new Object();
        CountDownLatch ready = new CountDownLatch(2);
        worker("t1", a, b, ready).start();
        worker("t2", b, a, ready).start();

        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        long[] ids = null;
        for (int i = 0; i < 100 && ids == null; i++) {
            Thread.sleep(10);
            ids = bean.findDeadlockedThreads();
        }
        String[] names = Arrays.stream(bean.getThreadInfo(ids))
                .map(info -> info.getThreadName())
                .sorted()
                .toArray(String[]::new);
        System.out.println("deadlocked: " + String.join(", ", names));
    }
}
```

```output for=detect
deadlocked: t1, t2
```

## In languages

Every language with threads and locks can deadlock, and so can every language with a blocking wait of some other kind. The verdict says where the program meets deadlock and which tools the language gives against it.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `threading.Lock`, `RLock`; `acquire(timeout=...)`; `Thread.join` refuses a self-join |
| JavaScript | native | no shared-memory threads, but async functions can await each other in a cycle |
| TypeScript | native | as JavaScript |
| Java | native | `synchronized` monitors and `java.util.concurrent.locks`; `ThreadMXBean` finds cycles |
| C | stdlib | POSIX mutexes; an error-checking mutex reports a relock with `EDEADLK` |
| C++ | stdlib | `std::mutex`; `std::lock` and `std::scoped_lock` take several mutexes without deadlock |
| C# | native | the `lock` statement and `Monitor.TryEnter` with a timeout |
| Go | native | `sync.Mutex` and channels; the runtime stops a program in which all goroutines are asleep |
| Rust | stdlib | `std::sync::Mutex` with `try_lock`; safe code can deadlock |
| Kotlin | stdlib | the JDK locks, such as `ReentrantLock.tryLock` with a timeout |
| PowerShell | stdlib | .NET locks in parallel script blocks |
| Bash | absent | no threads or locks; processes that wait for each other can still deadlock |
| SQL | native | transactions that lock rows in opposite orders; the server detects the cycle |
| Regex | n/a | a pattern has no threads and no waits |
| HTML/CSS | n/a | markup and style have no threads and no waits |

The C examples use POSIX threads, because the C toolchain of the examples has no `<threads.h>`.

Rust does not prevent deadlocks. The Rust Reference lists deadlocks among the behaviours that are not considered unsafe, so safe code can deadlock.[^18] Data races are a different bug (see [[data-races]]).

## Variations and related ideas

- **Self-deadlock.** One thread is enough. A thread that holds a non-reentrant lock and asks for it again waits for itself. Python's `Lock` blocks, and its `RLock` counts the acquisitions of the owner instead.[^2] A Rust `Mutex` leaves a relock in the same thread unspecified. The second call does not return, and it might panic or deadlock.[^9] POSIX gives a mutex type for each choice: a `PTHREAD_MUTEX_NORMAL` relock deadlocks, `PTHREAD_MUTEX_ERRORCHECK` returns `EDEADLK`, and `PTHREAD_MUTEX_RECURSIVE` counts.[^7] Java monitors and C# `lock` are reentrant.[^14][^19] See the first pitfall.
- **Joins.** A join is a wait for a thread to end, so it can be part of a cycle. A thread that joins itself waits forever. Python raises `RuntimeError` instead.[^20] POSIX lets `pthread_join` fail with `EDEADLK` when it detects a deadlock, and on Linux this includes a thread that names itself.[^21][^22] See [[threads]].
- **Channels.** A send on an unbuffered channel succeeds only when a receiver is ready.[^23] A send with no receiver blocks forever, and two goroutines that each wait to receive from the other form a cycle. See [[channels]] and the fourth pitfall.
- **Async tasks.** An async function that awaits a lock or a promise that another task holds can be part of a cycle. No thread blocks, so the program uses no CPU and shows no error. In Node.js, an ES module whose top-level `await` never settles exits with code 13.[^24] C# does not allow `await` in the body of a `lock` statement.[^19] See [[async-await]] and the JavaScript example.
- **Livelock.** The threads are not blocked, but they keep reacting to each other and do no useful work. The Java tutorial compares it to two people in a corridor who keep stepping into each other's way, each one trying to let the other pass.[^25] A random wait before each retry makes a repeat of the same collision less likely.
- **Starvation.** One thread cannot get access to a shared resource for a long time, because other threads take it again and again.[^25] The other threads make progress, so it is not a deadlock. A fair lock favours the thread that has waited longest. Java's `ReentrantLock` with fairness on guarantees the lack of starvation, at a cost in throughput.[^5]
- **Mutual exclusion in general.** Locks, semaphores and monitors are the usual resources in a deadlock. See [[mutual-exclusion]].

## Pitfalls

### Relock of a non-reentrant lock

A function that holds a lock calls a second function that takes the same lock. With Python's `Lock`, the second `acquire` waits for the thread itself.[^2] The timeout makes the program end.

```python wrong id=relock
import threading

lock = threading.Lock()
items = []


def add_item(item):
    # A plain "with lock:" here would wait forever.
    if not lock.acquire(timeout=0.2):
        print("add_item: timeout, the lock is held by this thread")
        return
    items.append(item)
    lock.release()


def add_two(first, second):
    with lock:
        add_item(first)
        add_item(second)


add_two("x", "y")
print("items:", items)
```

```output for=relock
add_item: timeout, the lock is held by this thread
add_item: timeout, the lock is held by this thread
items: []
```

Each call of `add_item` times out, and no item is added. One fix is `threading.RLock`, which the owner can acquire again.[^2] A cleaner fix splits the code: a public function takes the lock, and a private function assumes that the caller holds it.

### A thread that joins itself

A self-join waits for the end of the thread that waits. Python detects it.[^20]

```python fails expect=exception:RuntimeError id=self-join
import threading

threading.current_thread().join()
```

Python raises `RuntimeError: cannot join current thread`. The fix is to join each worker from the thread that started it. The C example shows `pthread_join` with the calling thread.

### A lock held across a join

The main thread holds a lock and joins a worker. The worker needs the same lock to finish. Each one waits for the other: a cycle of a lock and a join.

```python wrong id=lock-join
import threading

lock = threading.Lock()
done = []


def worker():
    with lock:
        done.append("worker")


with lock:
    t = threading.Thread(target=worker)
    t.start()
    t.join(timeout=0.2)
    print("worker still running:", t.is_alive())
t.join()
print("after release:", done)
```

```output for=lock-join
worker still running: True
after release: ['worker']
```

The timeout lets the program continue. Without it, the program would stop at the first `join`. The fix is to release the lock before the join. Callbacks, joins and calls to code that you do not control are all dangerous while you hold a lock.

### A send on an unbuffered channel with no receiver

An unbuffered channel needs a sender and a receiver at the same time.[^23] Here the only goroutine sends, so no receiver ever comes.

```go fails expect=exit:2 id=no-receiver
package main

import "fmt"

func main() {
	ch := make(chan int)
	ch <- 1
	fmt.Println(<-ch)
}
```

No goroutine can run, so the Go runtime stops the program with `fatal error: all goroutines are asleep - deadlock!` and exit code 2.[^15] The fix is a receiver in another goroutine, or a buffer: `make(chan int, 1)`.

## Cost

- **Lock ordering** costs almost nothing at run time: a comparison of two keys. The cost is in the design. Every lock needs a place in the order, and the code must know the second lock before it takes the first one.
- **All at once.** `std::lock` must find an order that does not deadlock, and the standard leaves the method to the implementation.[^3] Measure it on a hot path. A task that takes all its resources at the start can keep some of them unused for a long time.[^1]
- **Try, back off and retry** wastes the work that the failed attempt did. Under heavy contention the retries can repeat, which is a livelock. The code must also handle the failure path, which is a source of bugs.
- **Timeouts** turn a hang into an error. The program still has to decide what to do: retry, report or abort. A short timeout can fail under normal load. A long one makes a real deadlock slow to see.
- **Detection** has a run time cost. The Java documentation warns that `findDeadlockedThreads` might be expensive.[^12] PostgreSQL waits for `deadlock_timeout` before it checks, because the check is relatively expensive.[^17] Recovery throws away the work of the aborted task, so the program must be able to retry it.[^16]
- **Avoidance** checks the safety of the state at each request, and it needs the maximum needs of each task in advance.[^1]

## History

- **1968.** Dijkstra's "Cooperating Sequential Processes" (EWD 123) was published in *Programming Languages*, edited by F. Genuys. Its section 6 is "The Problem of the Deadly Embrace", and section 6.1 is "The Banker's Algorithm".[^10]
- **1968.** J. W. Havender described in the IBM Systems Journal how OS/360 avoids some deadlocks. The system took the resources for each job step in a fixed order.[^1]
- **1971.** E. G. Coffman Jr., M. J. Elphick and A. Shoshani published "System Deadlocks" in ACM Computing Surveys. It states the four conditions and sorts the treatments into prevention, detection and recovery, and avoidance.[^1]
- **1977.** Dijkstra wrote "The mathematics behind the Banker's Algorithm" (EWD 623).[^11]

## References
