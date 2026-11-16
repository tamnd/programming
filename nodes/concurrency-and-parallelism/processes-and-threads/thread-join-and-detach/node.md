---
tag: GW7RFF
kind: concept
slug: thread-join-and-detach
name: Join and detach
aliases: [thread join, join a thread, detach a thread, detached thread, joinable thread, zombie thread, daemon thread, background thread, pthread_join, pthread_detach, jthread]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: To join a thread is to wait until it ends and take its result. To detach it is to let it run on its own. A program should do one of the two for each thread that it starts.
applies_to: core-languages
viz: optional
history:
  year: 2011
  note: C++11 gave std::thread the members join and detach, after the POSIX functions pthread_join and pthread_detach. C++20 added std::jthread, which joins in its destructor.
see_also: [threads, start-a-thread, wait-for-threads-to-finish, futures, process, data-races]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_detach.html
    title: POSIX.1-2024, pthread_detach
    locator: DESCRIPTION; RATIONALE
  - url: https://docs.python.org/3/library/threading.html
    title: Python documentation, threading, Thread objects
    locator: Thread objects; join
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html
    title: POSIX.1-2024, Base Definitions, chapter 3, Definitions
    locator: 3.183 Joinable Thread; 3.427 Zombie Thread
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_join.html
    title: POSIX.1-2024, pthread_join
    locator: DESCRIPTION; CHANGE HISTORY
  - url: https://en.cppreference.com/w/cpp/thread/thread/join
    title: std::thread::join, cppreference.com
    locator: the description
  - url: https://doc.rust-lang.org/std/thread/fn.spawn.html
    title: The Rust Standard Library, function std::thread::spawn
    locator: the description of the join handle
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
    title: Java SE 21 API, java.lang.Thread
    locator: Platform threads; join(Duration); setDaemon
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Program execution
  - url: https://doc.rust-lang.org/std/thread/index.html
    title: The Rust Standard Library, module std::thread
    locator: The threading model
  - url: https://learn.microsoft.com/en-us/dotnet/standard/threading/foreground-and-background-threads
    title: Microsoft Learn, .NET, Foreground and background threads
    locator: the whole article
  - url: https://en.cppreference.com/w/cpp/thread/thread/~thread
    title: std::thread::~thread, cppreference.com
    locator: the description; Notes
  - url: https://en.cppreference.com/w/cpp/thread/thread/detach
    title: std::thread::detach, cppreference.com
    locator: the description; Postconditions
  - url: https://en.cppreference.com/w/cpp/thread/jthread
    title: std::jthread, cppreference.com
    locator: the description
  - url: https://doc.rust-lang.org/std/thread/fn.scope.html
    title: The Rust Standard Library, function std::thread::scope
    locator: the description
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: worker.unref()
  - url: https://doc.rust-lang.org/std/thread/struct.JoinHandle.html
    title: The Rust Standard Library, struct std::thread::JoinHandle
    locator: is_finished
---

## Idea

A program that starts a [[threads|thread]] must decide what happens when the thread ends. There are two choices. To **join** a thread is to wait until it ends. The caller then knows that the work of the thread is complete, and it can take the result. To **detach** a thread is to give up the right to wait for it. The thread runs on its own, and the system frees its storage when it ends. POSIX says that one of the two "should eventually be called for every thread that is created so that storage associated with the thread may be reclaimed".[^1]

Some runtimes add a third choice that is close to detach: a daemon thread, or background thread, does not keep the program alive. In Python, "the entire Python program exits when only daemon threads are left".[^2] The example below starts a child Python program twice. In each child, the main thread starts a worker that sleeps for 0.2 seconds and then prints a line. The main thread does not join the worker.

```python run id=idea
import subprocess
import sys

CHILD = """
import threading
import time


def work():
    time.sleep(0.2)
    print("worker finished", flush=True)


threading.Thread(target=work, daemon=DAEMON).start()
print("main returns", flush=True)
"""

for daemon in (False, True):
    code = CHILD.replace("DAEMON", str(daemon))
    result = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True, check=True)
    print(f"daemon={daemon}:", result.stdout.splitlines())
```

```output for=idea
daemon=False: ['main returns', 'worker finished']
daemon=True: ['main returns']
```

With `daemon=False`, Python waits for the worker before the program exits, so the worker prints its line. With `daemon=True`, the program exits when the main thread returns, and the worker is stopped before it prints. In both cases the main thread does not know when the worker ends. Only a join gives that knowledge.

## How it works

### Joinable, detached and zombie threads

POSIX gives a name to each state. A new thread is joinable, unless it was created detached. It stays joinable until a call to `pthread_join` or `pthread_detach` for it succeeds.[^3] The two calls do different things:

- **`pthread_join`** makes the caller wait: it "shall suspend execution of the calling thread until the target thread terminates, unless the target thread has already terminated". The value that the thread gives to `pthread_exit` becomes available to the caller.[^4]
- **`pthread_detach`** changes the thread "from joinable to detached", which tells the system that the storage of the thread "can be reclaimed when the thread terminates". The call does not stop the thread.[^1]

A joinable thread that has ended, but that nobody has joined or detached, is a zombie thread. POSIX defines it as the remains of a joinable thread "after it terminates" and "before it has been joined".[^3] A zombie holds storage until a join or a detach frees it.

```text
                      join returns
  joinable (running) -------------------------------> gone
       |          \
       | detach    \ thread ends
       v            v
  detached       zombie --- join or detach ---> gone
       |
       | thread ends
       v
     gone
```

### What a join gives

A join is a point of synchronization. When it returns, the thread has ended, and its writes to memory are visible to the caller. The C++ reference says that the end of the thread synchronizes with the return from `join`.[^5] So the caller can read the results that the thread wrote without a lock. Most languages also give a way to return a value through the join. In Rust, the join handle gives the return value of the closure, and "If the spawned thread panics, join will return an Err".[^6]

A join can also have a time limit. Python's `join` takes a timeout, but it always returns `None`, so "you must call is_alive" after it "to decide whether a timeout happened".[^2] Java's `join(Duration)` returns a `boolean` instead.[^7]

### What a detach gives up

After a detach, the program cannot wait for the thread, cannot take its result and cannot see its errors. A detached thread also does not keep the program alive. In Go, every goroutine is like a detached thread: when the `main` function returns, "the program exits", and the other goroutines do not finish first.[^8] In Rust, "When the main thread of a Rust program terminates, the entire program shuts down, even if other threads are still running".[^9]

### Daemon and background threads

Java, Python and .NET decide at the start of a thread whether the runtime waits for it at exit:

- **Java.** "The shutdown sequence begins when all started non-daemon threads have terminated."[^7]
- **Python.** A daemon thread does not keep the program alive, and "Daemon threads are abruptly stopped at shutdown".[^2]
- **.NET.** A background thread is like a foreground thread with one exception: "a background thread does not keep the managed execution environment running".[^10]

A daemon thread is still joinable in these languages. The flag only changes what happens at exit.

### Handles that must be joined

In C++ and Rust, a thread object owns the thread, and the language decides what happens when the object goes away:

- **`std::thread`.** If the object still has a thread when it is destroyed, the destructor calls `std::terminate`. After `join` or `detach`, the object has no thread and is safe to destroy.[^11] After `detach`, the object "no longer owns any thread".[^12]
- **`std::jthread`.** It "automatically rejoins on destruction". It also holds a stop source, and its destructor asks the thread to stop before it joins.[^13]
- **Rust `JoinHandle`.** If the handle is dropped, "the spawned thread will implicitly be detached".[^6] Inside `thread::scope`, the threads can borrow local data, because "the scope guarantees all threads will be joined at the end of the scope".[^14]

## In languages

The example files join one thread and leave another thread running at the end of the program. They show the result of the join and the way each language lets the program exit without the second thread. The table gives the verdict for each tier 1 language.

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `Thread.join(timeout)`; `daemon=True` in place of detach |
| JavaScript | stdlib | Node.js `Worker`: the `'exit'` event; `unref()` |
| TypeScript | stdlib | as JavaScript |
| Java | stdlib | `Thread.join`, `join(Duration)`; daemon threads |
| Kotlin | stdlib | the Java `Thread` methods on the JVM |
| C# | stdlib | `Thread.Join`, `Join(int)`; `IsBackground` |
| Go | idiom | no thread handle; wait with a channel or a `sync.WaitGroup` |
| Rust | stdlib | `JoinHandle::join`; drop the handle to detach; `thread::scope` |
| C | stdlib | `pthread_join`, `pthread_detach`; C11 `thrd_join`, `thrd_detach` |
| C++ | stdlib | `std::thread::join`, `detach`; `std::jthread` |
| Bash | native | `wait` for background jobs |
| PowerShell | stdlib | `Wait-Job` and `Receive-Job` for background jobs |
| SQL | n/a | a query does not start threads |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

- **C.** POSIX has `pthread_join` and `pthread_detach`, and the C11 thread functions `thrd_join` and `thrd_detach` follow the same rules.[^3]
- **Go.** The `go` statement gives no handle, so a program cannot join a goroutine directly. The goroutine signals its end itself, through a channel or a `sync.WaitGroup`. See [[wait-for-threads-to-finish]].
- **JavaScript.** A Node.js worker keeps the process alive while it runs. Calling `unref()` "allows the thread to exit if this is the only active handle in the event system".[^15]
- **Rust.** `JoinHandle::is_finished` tells whether the thread has ended, and "This function does not block".[^16] It lets a program join only a thread that has already ended.

## Variations and related ideas

- **Scoped threads.** A scope joins all of its threads before it returns, so no thread can outlive the data that it borrows. Rust's `thread::scope` works this way.[^14] `std::jthread` gives the same result for one thread, because its destructor joins.[^13]
- **Join with a timeout.** The caller waits for a limited time and then does other work. The thread is still joinable after a timeout.
- **Ask, then join.** A thread cannot be stopped safely from outside, so the caller sets a flag or an event that the thread checks, and then joins it. The stop token of `std::jthread` is one form of this.[^13] Python recommends that threads which must stop gracefully be non-daemonic and use "a suitable signalling mechanism such as an Event".[^2]
- **[[futures]].** A future joins on a result, not on a thread. The thread can return to a pool and run other tasks.
- **[[process]] exit codes.** A parent process waits for a child process in the same way, and a child that has ended but has not been waited for is a zombie process.

## Pitfalls

- **Neither join nor detach.** The thread's storage stays allocated after it ends. In Rust, "It is the responsibility of the program to either eventually join threads it creates or detach them; otherwise, a resource leak will result."[^6] For a `std::thread`, the result is worse: the destructor of a joinable object calls `std::terminate`.[^11]
- **A join on the wrong thread.** In POSIX, a join on the calling thread itself is undefined, and so are several joins on the same thread at the same time.[^4] Python raises `RuntimeError` for a join on the current thread, "as that would cause a deadlock".[^2]
- **A timeout that is not checked.** After `join(timeout)` in Python, the thread can still be alive. Call `is_alive()` before the program uses the results of the thread.[^2]
- **A detached thread that uses data that is gone.** A detached thread can still run after the function that started it has returned. If it uses a local variable of that function, the variable no longer exists. Rust prevents this: `thread::spawn` takes only data that lives long enough, and `thread::scope` allows borrowed data only because it joins.[^14]
- **Daemon threads that hold resources.** A daemon thread can stop in the middle of its work. In Python, its resources "may not be released properly".[^2] In .NET, the runtime stops a background thread at exit without an exception in the thread.[^10] Do not give a file write or a transaction to a daemon thread.

## Cost

- **Storage.** Each thread that has ended but has not been joined holds storage until the program joins or detaches it.[^1]
- **Waiting.** A join blocks the caller. A thread that joins workers one at a time cannot do other work in the meantime.
- **Detach.** A detach costs almost nothing, but the program then needs another way to know that the work is done, for example a channel or an event.

## History

The names come from POSIX threads. The functions `pthread_join` and `pthread_detach` first appeared in Issue 5 of the specification, "for alignment with the POSIX Threads Extension".[^4] C++11 added `std::thread` with `join` and `detach`.[^12] C++20 added `std::jthread`, which joins in its destructor and can ask its thread to stop.[^13] Rust's `thread::scope`, which joins all threads at the end of a scope, became stable in Rust 1.63.0.[^14]

## References
