---
tag: NEW-mutual-exclusion
kind: concept
slug: mutual-exclusion
name: Mutual exclusion
aliases: [mutex, lock, critical section]
area: concurrency-and-parallelism/shared-memory-and-locks
level: core
summary: >-
  Mutual exclusion lets at most one thread at a time run a critical section, the code that uses a shared resource. A mutex, or lock, is the usual tool.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1965
  people: [Edsger W. Dijkstra]
  note: Dijkstra published "Solution of a problem in concurrent programming control" in Communications of the ACM, the paper that first stated and solved the problem of mutual exclusion.
see_also: [threads, data-races, deadlock, channels, async-await, immutability, exception-handling]
same_as:
  wikidata: Q1047554
  wikipedia: Mutual_exclusion
sources:
  - url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD01xx/EWD123.html
    title: Edsger W. Dijkstra, Cooperating sequential processes, EWD 123
    locator: 2.1 A Simple Example (the critical section and the conditions on a solution); 3.2 and 3.3 (the operations P and V, and a semaphore for mutual exclusion)
  - paper: doi:10.1145/365559.365617
    title: E. W. Dijkstra, Solution of a problem in concurrent programming control, Communications of the ACM 8(9), 1965
    locator: p. 569
  - paper: doi:10.1016/0020-0190(81)90106-X
    title: G. L. Peterson, Myths about the mutual exclusion problem, Information Processing Letters 12(3), 1981
    locator: pp. 115-116
  - url: https://lamport.azurewebsites.net/pubs/pubs.html
    title: Leslie Lamport, The Writings of Leslie Lamport
    locator: the entry for A New Solution of Dijkstra's Concurrent Programming Problem, Communications of the ACM 17(8), 1974, pp. 453-455
  - url: https://docs.python.org/3.14/library/threading.html#lock-objects
    title: The Python Standard Library, threading, Python 3.14
    locator: Lock objects; Lock.acquire and Lock.release
  - url: https://docs.python.org/3.14/library/threading.html#rlock-objects
    title: The Python Standard Library, threading, Python 3.14
    locator: RLock objects
  - url: https://docs.python.org/3.14/library/threading.html#using-locks-conditions-and-semaphores-in-the-with-statement
    title: The Python Standard Library, threading, Python 3.14
    locator: Using locks, conditions, and semaphores in the with statement
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.mutex.requirements.mutex.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.mutex.requirements.mutex.general], paragraphs 4 to 8 (lock) and 20 to 21 (unlock)"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.mutex.class
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.mutex.class], paragraph 4, note 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.mutex.recursive
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.mutex.recursive], paragraph 3"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.lock.guard
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.lock.guard], paragraph 6"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.lock.scoped
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.lock.scoped], paragraph 2"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.lock.unique
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.lock.unique], the constructors with defer_lock and try_to_lock"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.sharedmutex.requirements.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.sharedmutex.requirements.general], paragraph 2"
  - url: https://timsong-cpp.github.io/cppwp/n4950/atomics.flag
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[atomics.flag], paragraphs 2, 9 and 10"
  - url: https://timsong-cpp.github.io/cppwp/n4950/hardware.interference
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[hardware.interference], paragraph 1"
  - url: https://doc.rust-lang.org/std/sync/struct.Mutex.html
    title: The Rust Standard Library, std::sync::Mutex
    locator: the type description, Poisoning, and the methods lock, get_mut and into_inner
  - url: https://doc.rust-lang.org/std/sync/struct.RwLock.html
    title: The Rust Standard Library, std::sync::RwLock
    locator: the type description, Poisoning, and the method read (Panics and Errors)
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-17.html#jls-17.1
    title: The Java Language Specification, Java SE 25 Edition
    locator: 17.1 Synchronization
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/locks/ReentrantLock.html
    title: Java SE 25 API, java.util.concurrent.locks.ReentrantLock
    locator: the class description (fairness and the recommended try and finally form) and tryLock(long, TimeUnit)
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/locks/ReentrantReadWriteLock.html
    title: Java SE 25 API, java.util.concurrent.locks.ReentrantReadWriteLock
    locator: the class description, Reentrancy and Lock downgrading
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/lock
    title: C# language reference, The lock statement
    locator: the lock statement, and Guidelines
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.readerwriterlockslim
    title: .NET API, System.Threading.ReaderWriterLockSlim
    locator: Remarks, Upgrade and downgrade locks
  - url: https://pkg.go.dev/sync#Mutex
    title: Go standard library, package sync
    locator: type Mutex, Mutex.Lock, Mutex.TryLock and Mutex.Unlock
  - url: https://pkg.go.dev/sync#RWMutex
    title: Go standard library, package sync
    locator: type RWMutex
  - url: https://github.com/golang/go/blob/go1.27.3/src/internal/sync/mutex.go#L31-L63
    title: Go source, src/internal/sync/mutex.go at go1.27.3
    locator: lines 31 to 63 (Mutex fairness and the fast path of Lock) and lines 95 to 149 (lockSlow)
  - url: https://go.dev/ref/spec#Defer_statements
    title: The Go Programming Language Specification
    locator: Defer statements
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_lock.html
    title: The Open Group Base Specifications Issue 8 (IEEE Std 1003.1-2024), pthread_mutex_lock
    locator: DESCRIPTION, the table of mutex types
  - url: https://man7.org/linux/man-pages/man7/futex.7.html
    title: Linux manual page, futex(7)
    locator: DESCRIPTION
  - url: https://tc39.es/ecma262/2026/#sec-atomics-object
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 25.4.6 Atomics.compareExchange, 25.4.13 Atomics.wait and 25.4.15 Atomics.notify
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.concurrent/
    title: Kotlin standard library, package kotlin.concurrent
    locator: withLock, read and write
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/foreach-object?view=powershell-7.5
    title: PowerShell reference, ForEach-Object
    locator: the Parallel parameter, and Notes
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899 working draft N3220 (C23 with editorial changes)
    locator: 6.10.10.4 Conditional feature macros (__STDC_NO_THREADS__); 7.28.1 Introduction, paragraph 2; 7.28.4.3 The mtx_lock function
  - url: https://en.wikipedia.org/wiki/Peterson%27s_algorithm
    title: Wikipedia, Peterson's algorithm
    locator: the introduction, and Note about hardware
  - url: https://man7.org/linux/man-pages/man1/flock.1.html
    title: Linux manual page, flock(1)
    locator: DESCRIPTION and AVAILABILITY
  - url: https://en.wikipedia.org/wiki/Mutual_exclusion
    title: Wikipedia, Mutual exclusion
    locator: the introduction, and Problem description
---

## Idea

Threads that share memory can get in each other's way. A thread that adds to a shared balance reads the old value, computes the sum and writes the result. When two [[threads]] do these steps at the same time, both can read the same old value. Then one write replaces the other, and one deposit is lost. This bug is a lost update. [[data-races]] describes the wider class of bugs.

A critical section is a part of a program that uses a shared resource. Mutual exclusion is the rule that at most one thread is in its critical section at any moment.[^1] The program below forces the bad interleaving with a barrier. Both threads read the balance, both wait at the barrier until the other has read, and then both write.

```python wrong id=lost-update
import threading

balance = 100
both_have_read = threading.Barrier(2)


def deposit(amount):
    global balance
    seen = balance
    both_have_read.wait()
    balance = seen + amount


depositors = [threading.Thread(target=deposit, args=(50,)) for _ in range(2)]
for depositor in depositors:
    depositor.start()
for depositor in depositors:
    depositor.join()
print("balance:", balance)
```

```output
balance: 150
```

The balance must be 200, but the program loses one deposit of 50. Without the barrier, the bug appears only on some runs, which makes it hard to find. A mutex, or lock, gives mutual exclusion. A thread locks the mutex before its critical section and unlocks it after. While one thread holds the lock, each other thread that tries to lock it waits. In Python, the `with` statement locks the lock at the start of the block and unlocks it at the end.[^7]

```python run id=locked
import threading

balance = 100
balance_lock = threading.Lock()


def deposit(amount):
    global balance
    with balance_lock:
        seen = balance
        balance = seen + amount


depositors = [threading.Thread(target=deposit, args=(50,)) for _ in range(2)]
for depositor in depositors:
    depositor.start()
for depositor in depositors:
    depositor.join()
print("balance:", balance)
```

```output
balance: 200
```

The lock does not fix the order of the two deposits. Either thread can go first. The lock makes sure that one critical section ends before the other starts, so the result is the same on each run.

## How it works

### The problem

Dijkstra stated the problem for processes that loop and enter a critical section from time to time. A solution must keep at most one process in its critical section. It must not assume anything about the relative speeds of the processes. A process that stops outside its critical section must not block the others. And the processes must not wait for each other forever when both want to enter.[^1] The last two conditions rule out a lock that is never released and two threads that each give way to the other without end.

Ordinary loads and stores can solve the problem, as the algorithms of Dekker, Dijkstra, Lamport and Peterson show (see History). These algorithms work, but on hardware that reorders memory accesses they need memory barriers to be correct.[^34] Library locks use special instructions instead.

### Atomic read-modify-write

An atomic read-modify-write (RMW) instruction reads a memory word and writes it in one step that no other thread can split. Two forms are common:

- **Test-and-set** sets a flag to true and returns the value that it had before. In C++, `std::atomic_flag::test_and_set` does this, and the operations of `atomic_flag` are always lock-free.[^15] The thread that gets `false` back has taken the lock.
- **Compare-and-swap** (CAS) writes a value only if the word still holds an expected value, and reports whether it did. A thread takes the lock when it changes the word from "unlocked" to "locked".

A spin lock is a loop around one of these instructions. This Rust spin lock uses CAS on an `AtomicBool`. The guard type unlocks in its destructor, which the next section explains.

```rust run id=spinlock
use std::cell::UnsafeCell;
use std::hint;
use std::ops::{Deref, DerefMut};
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

struct SpinLock<T> {
    locked: AtomicBool,
    value: UnsafeCell<T>,
}

// SAFETY: the flag lets only one thread at a time reach the value.
unsafe impl<T: Send> Sync for SpinLock<T> {}

struct SpinGuard<'a, T> {
    lock: &'a SpinLock<T>,
}

impl<T> SpinLock<T> {
    fn new(value: T) -> Self {
        SpinLock {
            locked: AtomicBool::new(false),
            value: UnsafeCell::new(value),
        }
    }

    fn lock(&self) -> SpinGuard<'_, T> {
        while self
            .locked
            .compare_exchange_weak(false, true, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            hint::spin_loop();
        }
        SpinGuard { lock: self }
    }
}

impl<T> Deref for SpinGuard<'_, T> {
    type Target = T;
    fn deref(&self) -> &T {
        // SAFETY: this guard holds the lock.
        unsafe { &*self.lock.value.get() }
    }
}

impl<T> DerefMut for SpinGuard<'_, T> {
    fn deref_mut(&mut self) -> &mut T {
        // SAFETY: this guard holds the lock.
        unsafe { &mut *self.lock.value.get() }
    }
}

impl<T> Drop for SpinGuard<'_, T> {
    fn drop(&mut self) {
        self.lock.locked.store(false, Ordering::Release);
    }
}

fn main() {
    let counter = SpinLock::new(0u32);
    thread::scope(|scope| {
        for _ in 0..4 {
            scope.spawn(|| {
                for _ in 0..10_000 {
                    *counter.lock() += 1;
                }
            });
        }
    });
    println!("total: {}", *counter.lock());
}
```

```output
total: 40000
```

The orderings `Acquire` and `Release` are part of the lock. A lock must also make the writes of one critical section visible to the next. The C++ standard states this for every mutex: the earlier unlock operations on a mutex synchronize with each lock operation on it.[^8] Without this rule, a thread could take the lock and still read a stale value.

### Spinning, then parking

A spin lock burns CPU time while it waits. If the thread that holds the lock is not running, the waiter can spin for its whole time slice. So a library mutex spins only for a short time. Then it asks the operating system to put the thread to sleep until the lock is free, which is called parking.

On Linux, the tool for this is the futex, a "fast user-space mutex". A futex is an aligned integer in user memory that the program changes only with atomic instructions. When there is no contention, the lock and the unlock stay entirely in user space. The kernel takes part only in the contended case, to put waiters to sleep and wake them.[^29]

The Go `sync.Mutex` shows the three stages in its source. The fast path of `Lock` is one CAS that changes the state from 0 to locked. If the CAS fails, `lockSlow` spins a few times while `runtime_canSpin` allows it. Then it calls `runtime_SemacquireMutex`, which parks the goroutine.[^26]

### Ownership

Most mutexes have an owner, the thread that locked them. In C++, a thread must own a mutex to unlock it. A call of `lock` on a `std::mutex` that the thread already owns breaks a precondition.[^8] In Java, only one thread at a time holds the monitor of an object.[^19]

Some locks have no owner. A Python `Lock` does not belong to a thread, and any thread may release it.[^5] A locked Go `Mutex` is also not tied to a goroutine, so one goroutine can lock it and another can unlock it.[^24] A Python `RLock` has an owner, and only that thread may release it.[^6]

```python run id=ownership
import threading

plain = threading.Lock()
owned = threading.RLock()
plain.acquire()
owned.acquire()


def release_both():
    plain.release()
    print("Lock: released by another thread")
    try:
        owned.release()
    except RuntimeError as error:
        print("RLock:", error)


helper = threading.Thread(target=release_both)
helper.start()
helper.join()
```

```output
Lock: released by another thread
RLock: cannot release un-acquired lock
```

A lock without an owner can serve as a signal between threads. A lock with an owner can find more mistakes, because it knows which thread must unlock it.

### Lock guards and scope

A pair of calls to `lock` and `unlock` is fragile. An early return, a `break` or an exception between them skips the unlock, and the lock stays held. Most languages tie the unlock to the end of a scope, so it runs on each path out of the critical section.

| Language | Scoped form | What unlocks the lock |
|---|---|---|
| C++ | `std::lock_guard`, `std::scoped_lock`, `std::unique_lock` | the destructor of the guard[^11] |
| Rust | the `MutexGuard` that `Mutex::lock` returns | the drop of the guard[^17] |
| Python | `with lock:` | the end of the block, also on an exception[^7] |
| Java | a `synchronized` block or method; `lock()`, then `try` and `finally` with `unlock()` | normal or abrupt completion[^19]; the `finally` block[^20] |
| C# | the `lock` statement | a `try` and `finally` that the compiler writes[^22] |
| Go | `mu.Lock()`, then `defer mu.Unlock()` | the deferred call, at return or during a panic[^27] |
| Kotlin | `lock.withLock { }` | the end of the lambda[^31] |
| C | none | an explicit `pthread_mutex_unlock` on each path |

The three C++ guards differ. A `lock_guard` holds one mutex for its whole life. A `scoped_lock` can take several mutexes, and then its constructor calls `std::lock`, which avoids a [[deadlock]] between them.[^12] A `unique_lock` can move to another owner, start without the lock (`defer_lock`), or try once and keep the result (`try_to_lock`).[^13]

Rust goes one step further. A `Mutex<T>` owns the data that it protects, and the program can reach the data only through the guard.[^17] Code that forgets to lock does not compile, because it has no path to the data. In C, C++, Java and Go, the mutex and the data are separate. Only a comment or a naming rule says which lock protects which field. [[rust/ownership]] explains the borrow rules that make the Rust form work.

### Reentrant locks

A reentrant lock, also called a recursive lock, lets its owner lock it again. It counts the levels, and the owner must unlock once for each level. The lock becomes free when the count goes back to zero. Java monitors work this way.[^19] So do `ReentrantLock` in Java[^20], the C# `lock` statement[^22], the Python `RLock`[^6], `std::recursive_mutex` in C++[^10], and a POSIX mutex of type `PTHREAD_MUTEX_RECURSIVE`.[^28]

Other locks are not reentrant. A second lock by the owner then waits for itself. A `std::mutex` can deadlock in this case.[^9] For a Rust `Mutex`, the result is unspecified: the call can panic or deadlock, but it does not return.[^17] POSIX gives a choice: a `PTHREAD_MUTEX_NORMAL` mutex deadlocks, a `PTHREAD_MUTEX_ERRORCHECK` mutex returns an error, and for the default type the behaviour is undefined.[^28] This program uses a timeout, so the plain `Lock` gives up instead of waiting forever.[^5]

```python run id=reentry
import threading

plain = threading.Lock()
reentrant = threading.RLock()

plain.acquire()
print("Lock, second acquire:", plain.acquire(timeout=0.1))

reentrant.acquire()
print("RLock, second acquire:", reentrant.acquire(timeout=0.1))
```

```output
Lock, second acquire: False
RLock, second acquire: True
```

A reentrant lock lets one locked method call another locked method of the same object. It also hides a risk. The inner call can run while the outer call has broken the invariant of the data for a moment.

### Reader-writer locks

Many critical sections only read. Reads do not conflict with each other, so a reader-writer lock lets many readers in at the same time, or one writer alone.

| Language | Type | Read form | Write form |
|---|---|---|---|
| Rust | `std::sync::RwLock`[^18] | `read()` | `write()` |
| C++ | `std::shared_mutex`[^14] | `std::shared_lock` | `std::unique_lock` |
| Java, Kotlin | `ReentrantReadWriteLock`[^21] | `readLock()`; Kotlin `read { }`[^31] | `writeLock()`; Kotlin `write { }` |
| C# | `ReaderWriterLockSlim`[^23] | `EnterReadLock()` | `EnterWriteLock()` |
| Go | `sync.RWMutex`[^25] | `RLock()` | `Lock()` |

In this Rust program, two read guards exist at the same time. A writer cannot enter while they exist.

```rust run id=rwlock
use std::sync::RwLock;

fn main() {
    let settings = RwLock::new(String::from("mode=quiet"));
    {
        let first = settings.read().unwrap();
        let second = settings.read().unwrap();
        println!("two readers: {} and {}", *first, *second);
        println!("writer can enter: {}", settings.try_write().is_ok());
    }
    settings.write().unwrap().push_str(",log=on");
    println!("after the write: {}", settings.read().unwrap());
}
```

```output
two readers: mode=quiet and mode=quiet
writer can enter: false
after the write: mode=quiet,log=on
```

The policy for a waiting writer differs. In Go, a blocked `Lock` call stops further readers from entering, so a stream of readers cannot starve the writer.[^25] `ReaderWriterLockSlim` also blocks further readers while a writer waits.[^23] The Rust `RwLock` takes its policy from the operating system.[^18] Because of these policies, a thread that takes a read lock twice can deadlock: a writer that waits between the two calls blocks the second one.[^18][^25]

A read lock cannot become a write lock in Java. A thread that holds the read lock and asks for the write lock never succeeds. A writer can take the read lock and then release the write lock, which is a downgrade.[^21] C# offers an upgradeable mode instead. Only one thread at a time can be in it, so its upgrade cannot deadlock with another upgrade.[^23]

## In languages

The example files show a counter that several threads increase under a lock, in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `threading.Lock` and `RLock` with `with` |
| JavaScript | idiom | a lock built from `Atomics` on a `SharedArrayBuffer`, between workers |
| TypeScript | idiom | as JavaScript; an async mutex for critical sections that contain `await` |
| Java | native | `synchronized`; `ReentrantLock` and `ReentrantReadWriteLock` |
| C | stdlib | POSIX `pthread_mutex_t`; the optional C11 `mtx_t` |
| C++ | stdlib | `std::mutex` with `std::lock_guard` or `std::scoped_lock`; `std::shared_mutex` |
| C# | native | the `lock` statement; `ReaderWriterLockSlim` |
| Go | stdlib | `sync.Mutex` and `sync.RWMutex` with `defer` |
| Rust | stdlib | `std::sync::Mutex<T>` and `RwLock<T>` with guards |
| Kotlin | stdlib | `withLock`, `read` and `write` over the JDK locks |
| PowerShell | idiom | `[System.Threading.Monitor]` from .NET in `ForEach-Object -Parallel` |
| Bash | absent | no threads; `flock(1)` locks a file between processes |
| SQL | n/a | the database system controls concurrent transactions |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

The C examples use POSIX threads. The C standard library has `mtx_t` and `mtx_lock` in `<threads.h>`, but this header is optional. An implementation that defines `__STDC_NO_THREADS__` need not provide it.[^33] The C library of macOS does not provide it.

JavaScript code on one thread needs no mutex. Only one piece of code runs at a time, and nothing else runs between two statements. Workers do run in parallel, and they can share a `SharedArrayBuffer`. The language gives no lock type, but `Atomics.compareExchange`, `Atomics.wait` and `Atomics.notify` are the parts of one.[^30] The JavaScript example builds a lock from them. A second case appears on one thread: a critical section that contains `await` lets other tasks run in the middle of it. The TypeScript example shows this case and an async mutex that fixes it.

Bash has no threads. A background job is a separate process, and processes share no variables. They can share files. The `flock` command manages `flock(2)` locks from shell scripts, but it is part of the util-linux package, not of Bash.[^35]

SQL statements have no threads and no shared variables. The database system decides how concurrent transactions see each other's changes. Regex patterns, HTML and CSS have no threads.

## Variations and related ideas

- **Semaphores.** Dijkstra's semaphore is a counter with the operations P (wait until the count is positive, then decrease it) and V (increase it). A semaphore that starts at one gives mutual exclusion.[^1] A larger start value lets that many threads in at once.
- **Try-lock and timed lock.** `tryLock` in Java[^20], `acquire(timeout=...)` in Python[^5] and `TryLock` in Go return at once or after a time limit. They let a thread do other work instead of waiting. The Go documentation warns that the use of `TryLock` is often a sign of a deeper problem.[^24]
- **Fair locks.** A fair lock grants the lock in the order of the requests. Java `ReentrantLock` takes an optional fairness setting. The documentation says that many threads on a fair lock can show lower overall throughput.[^20]
- **Monitors.** In Java, each object has a monitor, which is the lock that `synchronized` uses.[^19] Condition variables, which let a thread wait for a state of the data, are a separate topic.
- **Locks across `await`.** A thread lock held across an `await` stays held for the whole wait. The code after the `await` can also resume on a thread that does not own the lock. C# forbids `await` inside a `lock` statement (see Pitfalls). [[async-await]] explains the model. An async mutex suspends the task instead of the thread.
- **No shared mutable state.** Data that never changes needs no lock, see [[immutability]]. Threads that send messages instead of sharing memory need no lock around the data, see [[channels]].
- **Lock ordering.** Code that takes two locks can deadlock when two threads take them in different orders. [[deadlock]] covers lock ordering and the other remedies.

## Pitfalls

### A lock around each access is not enough

The read and the write of a read-modify-write must be in one critical section. Here each access has its own lock, and the barrier makes both threads read before either writes.

```python wrong id=split-sections
import threading

balance = 100
balance_lock = threading.Lock()
both_have_read = threading.Barrier(2)


def deposit(amount):
    global balance
    with balance_lock:
        seen = balance
    both_have_read.wait()
    with balance_lock:
        balance = seen + amount


depositors = [threading.Thread(target=deposit, args=(50,)) for _ in range(2)]
for depositor in depositors:
    depositor.start()
for depositor in depositors:
    depositor.join()
print("balance:", balance)
```

```output
balance: 150
```

No access is unprotected, but the update is still lost. The fix is one critical section around the read, the computation and the write, as in Idea. The same rule holds for check-then-act code, such as "if the key is absent, insert it".

### An exception skips the unlock

When a lock and its unlock are separate calls, an exception between them leaves the lock held. Here `withdraw` throws before it reaches `unlock`, and another thread then waits 100 milliseconds for the lock and gives up.[^20]

```java wrong id=no-finally
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    private static final ReentrantLock balanceLock = new ReentrantLock();
    private static int balance = 100;

    static void withdraw(int amount) {
        balanceLock.lock();
        if (amount > balance) {
            throw new IllegalArgumentException("not enough money");
        }
        balance -= amount;
        balanceLock.unlock();
    }

    public static void main(String[] args) throws InterruptedException {
        try {
            withdraw(500);
        } catch (IllegalArgumentException error) {
            System.out.println("withdraw failed: " + error.getMessage());
        }
        Thread auditor = new Thread(() -> {
            try {
                boolean acquired = balanceLock.tryLock(100, TimeUnit.MILLISECONDS);
                System.out.println("auditor got the lock: " + acquired);
                if (acquired) {
                    balanceLock.unlock();
                }
            } catch (InterruptedException error) {
                Thread.currentThread().interrupt();
            }
        });
        auditor.start();
        auditor.join();
    }
}
```

```output
withdraw failed: not enough money
auditor got the lock: false
```

The fix is the form that the Java documentation recommends: `lock()`, then a `try` block, with `unlock()` in `finally`.[^20] The scoped forms in How it works do the same in the other languages.

```java run id=with-finally
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    private static final ReentrantLock balanceLock = new ReentrantLock();
    private static int balance = 100;

    static void withdraw(int amount) {
        balanceLock.lock();
        try {
            if (amount > balance) {
                throw new IllegalArgumentException("not enough money");
            }
            balance -= amount;
        } finally {
            balanceLock.unlock();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        try {
            withdraw(500);
        } catch (IllegalArgumentException error) {
            System.out.println("withdraw failed: " + error.getMessage());
        }
        Thread auditor = new Thread(() -> {
            try {
                boolean acquired = balanceLock.tryLock(100, TimeUnit.MILLISECONDS);
                System.out.println("auditor got the lock: " + acquired);
                if (acquired) {
                    balanceLock.unlock();
                }
            } catch (InterruptedException error) {
                Thread.currentThread().interrupt();
            }
        });
        auditor.start();
        auditor.join();
    }
}
```

```output
withdraw failed: not enough money
auditor got the lock: true
```

[[exception-handling]] explains `finally` and the other cleanup forms.

### An unlock of a lock that is not held

An unlock must match a lock. A second unlock is a bug, and most libraries stop the program. This Go method mixes `defer` with an explicit unlock, so the early return unlocks twice.

```go fails expect=exit:2 id=double-unlock
package main

import (
	"fmt"
	"sync"
)

type Account struct {
	mu      sync.Mutex
	balance int
}

func (a *Account) Withdraw(amount int) bool {
	a.mu.Lock()
	defer a.mu.Unlock()
	if amount > a.balance {
		a.mu.Unlock()
		return false
	}
	a.balance -= amount
	return true
}

func main() {
	account := &Account{balance: 100}
	fmt.Println(account.Withdraw(500))
}
```

The Go runtime stops with `fatal error: sync: unlock of unlocked mutex`. The documentation calls this a run-time error.[^24] The fix is to remove the explicit `Unlock` and let `defer` do the one unlock. In Python, the release of an unlocked `Lock` raises `RuntimeError`.[^5]

```python fails expect=exception:RuntimeError id=release-unlocked
import threading

balance_lock = threading.Lock()
balance_lock.release()
```

### A panic poisons a Rust mutex

When a thread panics while it holds a Rust `Mutex`, the mutex becomes poisoned. Each later `lock` returns an error, because the data may be in a half-changed state.[^17] Code that calls `unwrap` on the result then panics too.

```rust fails expect=panic id=poisoned
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let balance = Arc::new(Mutex::new(100));
    let worker_balance = Arc::clone(&balance);
    let outcome = thread::spawn(move || {
        let mut guard = worker_balance.lock().unwrap();
        *guard -= 30;
        panic!("the transfer stopped halfway");
    })
    .join();
    println!("worker failed: {}", outcome.is_err());
    let guard = balance.lock().unwrap();
    println!("balance: {}", *guard);
}
```

The second `unwrap` panics with a `PoisonError`. Poisoning is advisory. The program can take the guard from the error with `PoisonError::into_inner`, check the data, and go on, as in the Rust example.[^17] An `RwLock` is poisoned only by a panic in write mode.[^18]

### A lock held across an `await`

A critical section that waits for I/O holds the lock for the whole wait. With a thread lock and `await`, the code after the `await` can also resume on another thread. C# rejects an `await` in the body of a `lock` statement.[^22]

```csharp fails expect=compile id=await-in-lock
var balanceLock = new Lock();
int balance = 100;

lock (balanceLock)
{
    await Task.Delay(10);
    balance += 50;
}
Console.WriteLine(balance);
```

The compiler stops with error CS1996, "Cannot await in the body of a lock statement". The fix is to do the slow work outside the lock and keep only the update inside. When the critical section must contain an `await`, use a lock that suspends the task, not the thread. The TypeScript example has one.

### Data outside the lock

In C and C++, nothing ties a mutex to the data that it protects. One function that forgets the lock is enough for a data race, which is undefined behaviour. ThreadSanitizer finds the race in this program, where `reset` writes the field without the lock.

```cpp fails expect=sanitizer:thread flags="-fsanitize=thread" id=unguarded
#include <mutex>
#include <print>
#include <thread>

class Counter {
    std::mutex mutex_;
    int value_ = 0;

  public:
    void add() {
        std::lock_guard guard(mutex_);
        ++value_;
    }
    void reset() { value_ = 0; }
    int get() {
        std::lock_guard guard(mutex_);
        return value_;
    }
};

int main() {
    Counter counter;
    std::thread adder([&] {
        for (int i = 0; i < 1000; i++) counter.add();
    });
    std::thread resetter([&] { counter.reset(); });
    adder.join();
    resetter.join();
    std::println("{}", counter.get() >= 0);
}
```

The fix is a `lock_guard` in `reset` too. A Rust `Mutex<T>` prevents this mistake, because the field is inside the mutex.

## Cost

- **The uncontended case.** A lock that nobody else holds costs one atomic RMW to lock and one atomic store or RMW to unlock. No system call happens. The Go fast path is one CAS, and a futex stays in user space when there is no contention.[^26][^29] This cost is small, but it is larger than a plain load and store.
- **The contended case.** A thread that finds the lock taken spins for a while, then parks. Each park and each wake is a system call and a context switch. So the time per critical section can grow many times over when many threads want the same lock.
- **Convoys.** When the holder of a lock is slow or not scheduled, the other threads queue behind it. After the release, they wake one at a time, and each pays the cost of a wake. A lock that is held for a long time turns parallel threads into a queue. The C# guidelines say to hold a lock for as short a time as possible.[^22]
- **Fairness against throughput.** A fair lock hands the lock to the oldest waiter, which may still be asleep. An unfair lock lets a running thread take it again at once, which gives more throughput but can starve a waiter. In its normal mode, a Go mutex lets a running goroutine compete with a woken waiter. A waiter that fails to get the mutex for more than 1 millisecond switches it to starvation mode. Then the unlock hands the mutex directly to the first waiter.[^26] The Java documentation says that a fair lock can show lower throughput.[^20]
- **False sharing.** A processor moves memory between cores in cache lines. Two variables on one cache line contend even when each thread uses only its own variable, and even when no lock protects them. A lock word next to hot data has the same effect. C++ gives `std::hardware_destructive_interference_size`, the minimum offset between two objects that avoids this contention.[^16] Padding or `alignas` with this value puts each counter or lock on its own line.
- **Reader-writer locks.** A reader still writes to the shared lock word to record its entry. So readers on many cores contend on that word, and a reader-writer lock can be slower than a plain mutex for short reads. It helps when reads are long or writes are rare.
- **Lock granularity.** One lock for a whole structure keeps the rules clear, but all threads wait for it. Many small locks allow more parallel work, but they cost more memory and make [[deadlock]] more likely.

## History

- **1965.** Dijkstra published "Solution of a problem in concurrent programming control" in Communications of the ACM.[^2] The requirement of mutual exclusion was first stated and solved in this paper.[^36] In his manuscript EWD 123, Cooperating Sequential Processes, Dijkstra used the term critical section and credited Th. J. Dekker with the first correct solution for two processes. The same text introduced the semaphore operations P and V.[^1]
- **1974.** Leslie Lamport published the bakery algorithm in "A New Solution of Dijkstra's Concurrent Programming Problem". It gives mutual exclusion without any lower-level mutual exclusion, even when a read overlaps a write to the same memory.[^4]
- **1981.** Gary L. Peterson published "Myths about the mutual exclusion problem" in Information Processing Letters.[^3] It contains a short algorithm for two processes, which is called Peterson's algorithm.[^34]

## References
