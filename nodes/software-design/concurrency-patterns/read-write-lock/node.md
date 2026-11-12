---
tag: 9D9V70
kind: pattern
slug: read-write-lock
name: Read-write lock
aliases: [read-write lock, readers-writer lock, readers–writer lock, RW lock, shared-exclusive lock, MRSW lock]
area: software-design/concurrency-patterns
level: core
summary: >-
  A read-write lock lets many threads read shared data at the same time but gives a writer exclusive access. Its priority policy decides if waiting readers or a waiting writer go first.
family: concurrency
polarity: positive
intent: Let threads that only read shared data run in parallel, and still give a thread that changes the data exclusive access, so that no reader sees a half-done write.
also_known_as: [Readers-writer lock, Single-writer lock, Multi-reader lock, Push lock, MRSW lock]
history:
  year: 1971
  people: [P. J. Courtois, F. Heymans, D. L. Parnas]
  note: Courtois, Heymans and Parnas first formulated and solved the readers-writers problem, in "Concurrent Control with Readers and Writers" in Communications of the ACM.
see_also: [mutual-exclusion, monitor-object, immutable-object, thread-confinement, scoped-locking, strategized-locking, deadlock, data-races]
sources:
  - url: https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock
    title: Readers–writer lock, Wikipedia
    locator: lead; Upgradable RW lock; Priority policies; Implementation; Programming language support; Alternatives
  - url: https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem
    title: Readers–writers problem, Wikipedia
    locator: lead; First readers–writers problem; Second readers–writers problem
  - url: https://pkg.go.dev/sync#RWMutex
    title: Go standard library, package sync
    locator: type RWMutex
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/locks/ReentrantReadWriteLock.html
    title: Java SE 21 API, java.util.concurrent.locks.ReentrantReadWriteLock
    locator: Acquisition order; Reentrancy; Lock downgrading
  - url: https://doc.rust-lang.org/std/sync/struct.RwLock.html
    title: The Rust Standard Library, std::sync::RwLock
    locator: the type description
---

## Intent

A read-write lock allows concurrent access for read-only operations, whereas write operations require exclusive access.[^1] Multiple threads can read the data in parallel, but a thread needs an exclusive lock to write or modify the data.[^1]

## Problem

Many threads read a shared data structure and a few threads change it. The structure cannot be updated atomically, and it is invalid until the update is complete, so no thread may read it during a write.[^1] A plain mutex protects the data, but then no two threads can access the data at the same time.[^2] That is sub-optimal, because a reader R2 waits for a reader R1 although two reads do not interfere.[^2]

## Solution

Give the lock two modes. A thread takes the read lock to read and the write lock to write. The lock can be held by an arbitrary number of readers or a single writer.[^3] When a writer is writing the data, all other writers and readers are blocked until the writer is finished writing.[^1]

```text
            read lock                     write lock
 reader 1 ──►┐                  writer ──►┐
 reader 2 ──►├─► shared data              ├─► shared data
 reader 3 ──►┘    (all at once)           ┘    (alone; readers and
                                                 other writers wait)
```

Read-write locks are usually constructed on top of mutexes and condition variables, or on top of semaphores.[^1] The lock also needs a priority policy. A read-preferring lock gives priority to readers, and a write-preferring lock gives priority to writers.[^1] The Python example is write-preferring: a reader that arrives while a writer waits must wait too.

## Example

The example first starts two readers. Each takes the read lock and then waits until the other one is also inside, which shows that the two hold the read lock at the same time. Then 4 readers each read a pair of values 1000 times, and a writer changes the pair 100 times. The writer sets the first value, yields, and then sets the second value. Each reader reads the first value, yields, and then reads the second value. The lock makes sure that no reader sees a pair with two different values, and that no reader is inside while the writer is.

```text
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

The examples for each language show the same program.

## Consequences

Benefits:

- Readers do not block each other, so read-mostly data gets more concurrency than with a mutex.[^1]
- A read-preferring lock allows maximum concurrency.[^1]
- A write-preferring lock avoids writer starvation, because no new reader can acquire the lock while a writer is queued and waiting for it.[^1]

Liabilities:

- A read-preferring lock can lead to write-starvation if contention is high, because a writer cannot acquire the lock as long as at least one reading thread holds it.[^1]
- A write-preferring lock allows less concurrency in the presence of writer threads, and each operation is more complex.[^1]
- Upgrading a lock from read-mode to write-mode is prone to deadlocks. If two threads that hold reader locks both try to upgrade to writer locks, there is a deadlock.[^1] In Java, if a reader tries to acquire the write lock it will never succeed.[^4]
- A thread that takes the read lock again can block. In Go, if a goroutine calls `Lock` while readers hold the lock, new calls to `RLock` block until the writer has acquired and released the lock, and this prohibits recursive read-locking.[^3] The Rust documentation shows a similar potential deadlock, because a writer which waits for the lock might or might not block concurrent calls to `read`.[^5]

This program asks for the write lock while it still holds the read lock. The write lock waits until no thread holds the read lock, so the thread waits for itself:

```python wrong id=read-to-write-upgrade
import threading


class ReadWriteLock:
    def __init__(self):
        self._cond = threading.Condition()
        self._readers = 0
        self._writer = False

    def acquire_read(self):
        with self._cond:
            self._cond.wait_for(lambda: not self._writer)
            self._readers += 1

    def release_read(self):
        with self._cond:
            self._readers -= 1
            self._cond.notify_all()

    def acquire_write(self, timeout):
        with self._cond:
            free = lambda: not self._writer and self._readers == 0
            if not self._cond.wait_for(free, timeout):
                return False
            self._writer = True
            return True

    def release_write(self):
        with self._cond:
            self._writer = False
            self._cond.notify_all()


lock = ReadWriteLock()
cache = {}


def get_or_load(key):
    lock.acquire_read()
    try:
        if key not in cache:
            # Wrong: ask for the write lock while this thread holds the read lock.
            if not lock.acquire_write(timeout=1):
                return "upgrade timed out: the thread's own read lock blocks the write lock"
            cache[key] = key.upper()
            lock.release_write()
        return cache[key]
    finally:
        lock.release_read()


print(get_or_load("a"))
```

```output for=read-to-write-upgrade
upgrade timed out: the thread's own read lock blocks the write lock
```

To fix the program, release the read lock, take the write lock, and check the condition again, because another thread can change the data between the two locks.

## When not to use it

Do not use a read-write lock when most operations write, or when each read is very short. Each operation on a write-preferring lock is more complex than an operation on one mutex.[^1] Do not use it when the data can be immutable: an [immutable object](immutable-object) needs no lock for reads.

## Variants and related patterns

- **Priority policies.** A lock can be read-preferring, write-preferring, or unspecified with regard to priority.[^1] An unspecified priority can be preferable if it allows a more efficient implementation.[^1] The Rust `RwLock` uses the policy of the operating system and does not guarantee a particular policy.[^5]

The next program puts a writer and a second reader in the queue while a first reader holds the lock, and prints the order in which they enter:

```python run id=read-or-write-preference
import threading
import time


class ReadWriteLock:
    def __init__(self, prefer_writers):
        self.prefer_writers = prefer_writers
        self.cond = threading.Condition()
        self.readers = 0
        self.writer = False
        self.waiting_readers = 0
        self.waiting_writers = 0

    def acquire_read(self):
        with self.cond:
            self.waiting_readers += 1
            while self.writer or (self.prefer_writers and self.waiting_writers):
                self.cond.wait()
            self.waiting_readers -= 1
            self.readers += 1

    def release_read(self):
        with self.cond:
            self.readers -= 1
            self.cond.notify_all()

    def acquire_write(self):
        with self.cond:
            self.waiting_writers += 1
            while self.writer or self.readers:
                self.cond.wait()
            self.waiting_writers -= 1
            self.writer = True

    def release_write(self):
        with self.cond:
            self.writer = False
            self.cond.notify_all()


def wait_until(test):
    while not test():
        time.sleep(0.001)


def entry_order(prefer_writers):
    lock = ReadWriteLock(prefer_writers)
    entered = []

    def writer():
        lock.acquire_write()
        entered.append("writer")
        lock.release_write()

    def second_reader():
        lock.acquire_read()
        entered.append("reader 2")
        lock.release_read()

    lock.acquire_read()  # reader 1 holds the read lock
    w = threading.Thread(target=writer)
    w.start()
    wait_until(lambda: lock.waiting_writers == 1)
    r = threading.Thread(target=second_reader)
    r.start()
    wait_until(lambda: entered or lock.waiting_readers == 1)
    lock.release_read()  # reader 1 leaves
    w.join()
    r.join()
    return entered


print("read-preferring:", entry_order(prefer_writers=False))
print("write-preferring:", entry_order(prefer_writers=True))
```

```output for=read-or-write-preference
read-preferring: ['reader 2', 'writer']
write-preferring: ['writer', 'reader 2']
```

- **Fair mode.** Java's `ReentrantReadWriteLock` does not impose a reader or writer preference ordering, but supports an optional fairness policy.[^4] In fair mode, threads contend for entry using an approximately arrival-order policy.[^4]
- **Upgradable lock.** Some read-write locks allow the lock to be atomically upgraded from read-mode to write-mode. To avoid the deadlock, only one thread at a time can hold the lock in "read-mode with intent to upgrade to write".[^1]
- **Downgrading.** Java allows downgrading from the write lock to a read lock: acquire the write lock, then the read lock, and then release the write lock. Upgrading from a read lock to the write lock is not possible.[^4] In Go, an `RLock` cannot be upgraded into a `Lock`, nor can a `Lock` be downgraded into an `RLock`.[^3]
- **Alternatives.** The read-copy-update (RCU) algorithm is one solution to the readers-writers problem, and it is wait-free for readers. The Linux kernel uses a seqlock for few writers.[^1]

A [scoped locking](scoped-locking) guard releases the read lock or the write lock when the block ends, as the C++ and Rust examples do.

## Known uses

- POSIX `pthread_rwlock_t`.[^1]
- Java `ReadWriteLock` and `ReentrantReadWriteLock`.[^1]
- .NET `System.Threading.ReaderWriterLockSlim`.[^1]
- C++ `std::shared_mutex` and Boost `shared_mutex` and `upgrade_mutex`.[^1]
- Go `sync.RWMutex` and Rust `std::sync::RwLock`.[^1]
- `SRWLock` in the Windows API and `rw_semaphore` in the Linux kernel.[^1]

## References
