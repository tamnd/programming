---
tag: P2W06C
kind: pattern
slug: thread-specific-storage
name: Thread-Specific Storage
aliases: [thread-specific storage, thread-local storage, tls, thread-local variable]
area: software-design/concurrency-patterns
level: core
summary: >-
  Thread-Specific Storage gives each thread its own copy of an object behind one global access point. The code reads and writes the object with no lock, because no other thread can reach that copy.
family: concurrency
polarity: positive
intent: Allow multiple threads to use one logically global access point to retrieve thread-specific data without incurring locking overhead for each access.
also_known_as: [Thread-Local Storage]
history:
  year: 1997
  people: [Douglas C. Schmidt, Timothy H. Harrison, Nat Pryce]
  note: The paper was written for the 4th Pattern Languages of Programming conference (PLoP '97) in September 1997. POSA2 (2000) has the pattern with the concurrency patterns.
see_also: [singleton-pattern, proxy-pattern, double-checked-locking, active-object, leader-followers, data-races, mutual-exclusion, threads]
sources:
  - url: https://www.dre.vanderbilt.edu/~schmidt/PDF/TSS-pattern.pdf
    title: "Douglas C. Schmidt, Timothy H. Harrison, and Nat Pryce, Thread-Specific Storage for C/C++: An Object Behavioral Pattern for Accessing per-Thread State Efficiently"
    locator: Abstract; 1 Intent; 2 Motivation; 3 Applicability; 4 Structure and Participants; 6 Consequences; 10 Known Uses; 11 Related Patterns
  - url: https://en.wikipedia.org/wiki/Thread-local_storage
    title: Thread-local storage, Wikipedia
    locator: lead; C and C++; Java; .NET languages; Python; Rust
  - url: https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/
    title: Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects (POSA2), overview page
    locator: Overview, the 17 patterns
  - url: https://docs.python.org/3/library/threading.html
    title: threading, Thread-based parallelism, Python documentation
    locator: Thread-local data
---

## Intent

The Thread-Specific Storage pattern allows multiple threads to use one logically global access point to retrieve thread-specific data without incurring locking overhead for each access.[^1] Wikipedia calls the mechanism thread-local storage: memory that is static or global but local to a thread, so data appears to be global in a system with separate threads.[^2]

## Problem

The paper's example is `errno`. When a system call fails, the OS sets `errno` and returns a failure status, and the application then checks `errno` to find the type of error.[^1] With one global `errno` per process, a value set by a method in one thread can be interpreted erroneously by other threads.[^1] Thread T1's `recv` sets `errno` to `EWOULDBLOCK`, T1 is preempted, and T2's `recv` sets `errno` to `EINTR`. Then T1 falsely assumes its `recv` call was interrupted.[^1]

A mutex around `errno` does not solve the race, because the call sets the value and the application tests it in two steps.[^1] A locking protocol in which the application releases the lock after the test is also undesirable: applications can forget to release the lock, and the extra locking overhead degrades performance even when multiple threads are not used.[^1]

## Solution

Use one global access point that is logically shared by all threads but physically unique for each thread.[^1] The participants are:[^1]

- The **Application Thread** uses the access point, such as the `errno` macro.[^1]
- The **TS Object Proxy** defines the interface of a TS Object and gives each thread access to its own object through `get` and `set` methods. It hides keys and TS Object Collections from the application threads.[^1]
- The **TS Object** is a particular thread's instance of a thread-specific object, such as a thread's `errno` of type `int`.[^1]
- The **TS Object Collection** contains the thread-specific objects of one thread and maps keys to them. Every thread has a unique collection.[^1]

```text
thread T1                         thread T2
errno = EWOULDBLOCK               errno = EINTR
  proxy -> T1's collection          proxy -> T2's collection
        -> key -> T1's errno              -> key -> T2's errno
if errno == EWOULDBLOCK: true     if errno == EINTR: true
```

Once the proxy finds the object, the thread works on it with ordinary method calls. No locking is necessary, because only the calling thread accesses the object.[^1] On Solaris, `errno` is a macro that calls a function, which returns a pointer to the thread-specific value.[^1]

## Example

The example files keep a last-error value in thread-specific storage. Four threads each make 1000 calls to a function that stores an error code for its thread and then reads the code back, with a yield to the scheduler between the two steps. The program counts the reads that see another thread's code. The program prints:

```text
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

## Consequences

Benefits:[^1]

- **Efficiency.** The pattern can be implemented so that no locking is needed for thread-specific data. Each thread can set and test the completion status of methods within that thread without complex synchronization protocols.[^1]
- **Ease of use.** System developers can make thread-specific storage completely transparent at the source-code level via data abstraction or macros.[^1]

Liabilities:[^1]

- **It encourages the use of (thread-safe) global variables.** Many applications do not need a common access point to thread-specific data. Then the data should be stored so that only the thread owning the data can access it, for example by passing a Logger as a parameter to all functions of an Active Object.[^1]
- **It hides the structure of the system.** It hides the relationships between objects in an application, potentially making the application harder to understand.[^1]

The block below shows the race from the paper with one global value. Events force the order of the threads, so the race happens on each run:

```python wrong id=global-errno
import threading

errno = 0  # one global errno per process
EWOULDBLOCK, EINTR = 35, 4
t1_set = threading.Event()
t2_set = threading.Event()
seen = {}


def t1():
    global errno
    errno = EWOULDBLOCK  # T1's recv sets errno
    t1_set.set()
    t2_set.wait()  # T1 is preempted here
    seen["T1"] = "interrupted" if errno == EINTR else "would block"


def t2():
    global errno
    t1_set.wait()
    errno = EINTR  # T2's recv sets errno
    t2_set.set()


threads = [threading.Thread(target=t1), threading.Thread(target=t2)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("T1's recv would block, T1 reads:", seen["T1"])
```

```output for=global-errno
T1's recv would block, T1 reads: interrupted
```

## When not to use it

- Multiple threads collaborate on a single task that requires concurrent access to shared data, such as an in-memory database. If thread-specific storage held the database, the threads could not share the data.[^1]
- It is more intuitive and efficient to maintain both a physical and logical separation of data, for example by passing the data explicitly as parameters to all methods.[^1]
- Tasks of a thread pool must not see each other's data. A pool thread runs many tasks, so a value that one task leaves in thread-specific storage is still there for the next task, as the block below shows.

## Variants and related patterns

- **Per-thread Singleton.** Objects in thread-specific storage are often used as per-thread Singletons; `errno` is one. A thread can also have multiple instances of a type in thread-specific storage.[^1]
- **Proxy.** The `TSS` template class of the paper serves as a Proxy that shields applications from the implementation of thread-specific storage in OS thread libraries.[^1]
- **Double-Checked Locking Optimization.** Applications that use the pattern commonly use it to avoid constraining the order of initialization for thread-specific storage keys.[^1]
- **Language features.** C11 has the `_Thread_local` keyword, and C++11 introduces `thread_local`. Java has the `ThreadLocal` class, C# has the `ThreadStatic` attribute and the `ThreadLocal<T>` class, Python has `threading.local`, and Rust has the `thread_local!` macro.[^2] POSA2 lists the pattern among its 17 patterns.[^3]

In Python, a `threading.local` object holds data whose values are thread specific, and changes made in another thread do not affect the data seen in this thread.[^4] With a thread pool, one thread runs each task in turn:

```python run id=pool-leak
import threading
from concurrent.futures import ThreadPoolExecutor

context = threading.local()


def handle(user):
    previous = getattr(context, "user", None)
    context.user = user
    return previous


with ThreadPoolExecutor(max_workers=1) as pool:
    print("task 1 saw:", pool.submit(handle, "alice").result())
    print("task 2 saw:", pool.submit(handle, "bob").result())
print("main thread sees:", getattr(context, "user", None))
```

```output for=pool-leak
task 1 saw: None
task 2 saw: alice
main thread sees: None
```

## Known uses

- **errno.** The `errno` mechanism on platforms that support the POSIX and Solaris threading APIs is a widely used example. The Win32 C runtime library supports a thread-specific `errno`, and the Win32 `GetLastError` and `SetLastError` functions also implement the pattern.[^1]
- **Win32 windows.** Windows are owned by threads, and each thread that owns a window has a private message queue in thread-specific storage.[^1]
- **OpenGL.** On Win32, the OpenGL library keeps a unique set of state variables in thread-specific storage for each thread that uses the library.[^1]
- **ACE.** The ACE network programming toolkit uses thread-specific storage for its error handling scheme and implements type-safe thread-specific storage template wrappers.[^1]

## References
