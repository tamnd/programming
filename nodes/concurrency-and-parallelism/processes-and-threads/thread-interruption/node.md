---
tag: CT7Y2C
kind: concept
slug: thread-interruption
name: Thread interruption
aliases: [interrupting a thread, thread cancellation, cooperative cancellation, stop a thread, Thread.interrupt, InterruptedException, CancellationToken, stop_token, request_stop, pthread_cancel, Thread.stop]
area: concurrency-and-parallelism/processes-and-threads
level: advanced
summary: Thread interruption asks a running thread to stop. The thread checks for the request and stops by itself, because killing a thread from outside can leave locks held and data half changed.
applies_to: core-languages
viz: optional
history:
  year: 2020
  note: C++20 added std::jthread, a thread that holds a stop source, so that other code can ask it to stop through a std::stop_token.
see_also: [threads, thread-join-and-detach, defer-and-finally, exception-handling, deadlock, mutual-exclusion, process-based-parallelism, async-await]
sources:
  - url: https://docs.oracle.com/javase/tutorial/essential/concurrency/interrupt.html
    title: The Java Tutorials, Interrupts
    locator: introduction; Supporting Interruption; The Interrupt Status Flag
  - url: https://learn.microsoft.com/en-us/dotnet/standard/threading/cancellation-in-managed-threads
    title: Microsoft Learn, Cancellation in managed threads
    locator: introduction; Listening by Polling; Listening by Using a Wait Handle
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/doc-files/threadPrimitiveDeprecation.html
    title: Java SE 21, Java Thread Primitive Deprecation
    locator: Why is Thread.stop deprecated; What should I use instead of Thread.stop; How do I stop a thread that waits for long periods
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
    title: Java SE 21 API, java.lang.Thread
    locator: stop; interrupt; interrupted
  - url: https://en.cppreference.com/w/cpp/thread/stop_token
    title: cppreference, std::stop_token
    locator: description; Notes
  - url: https://pkg.go.dev/context
    title: Go standard library, package context
    locator: Overview; func WithCancel
  - url: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminatethread
    title: Microsoft Learn, TerminateThread function
    locator: Remarks
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_cancel.html
    title: POSIX.1-2024, pthread_cancel
    locator: DESCRIPTION; RATIONALE; CHANGE HISTORY
  - url: https://docs.python.org/3/library/threading.html
    title: The Python Standard Library, threading
    locator: introduction; Thread objects; Event objects
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.thread.abort
    title: Microsoft Learn, Thread.Abort method
    locator: Remarks
  - url: https://doc.rust-lang.org/std/thread/index.html
    title: Rust standard library, module std::thread
    locator: module description; Functions
  - url: https://en.cppreference.com/w/cpp/thread/jthread
    title: cppreference, std::jthread
    locator: description
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: introduction; worker.terminate()
---

## Idea

Sometimes a thread must stop before its work is done. The user cancels a download, a server shuts down, or another thread already found the answer. Thread interruption is a way to ask the thread to stop. The Java tutorial calls an interrupt "an indication to a thread that it should stop what it is doing and do something else".[^1]

The important word is *ask*. In most modern APIs, the thread that wants the stop only sends a request, and the target thread decides when and how to stop. .NET calls this "cooperative cancellation": "Cancellation is cooperative and is not forced on the listener".[^2] The reason is that a thread killed at a random point can leave shared data broken. Java removed `Thread.stop` because "it was inherently unsafe".[^3]

## How it works

### A request and a check

Cooperative interruption has two parts:[^3][^2]

1. **The request.** One thread sets a flag that the target thread can see. The flag can be a plain variable, Java's interrupt status, a .NET `CancellationToken`, a Go `context.Context` or a C++ `std::stop_token`.
2. **The check.** The target thread checks the flag at points where it is safe to stop. When it sees the request, it cleans up and returns.

The Java documentation describes the simplest form. Code "simply modifies some variable to indicate that the target thread should stop running".[^3] The target thread "should check this variable regularly, and return from its run method in an orderly fashion if the variable indicates that it is to stop running".[^3] A long calculation checks the flag in its loop. In Java, it can "periodically invoke Thread.interrupted" and return when the result is true.[^1]

### Waking a thread that waits

A thread that sleeps or waits does not check a flag. So a good interruption API also wakes the thread:

- **Java.** If a thread is blocked in `sleep`, `wait` or `join`, `interrupt()` clears its "interrupt status" and the thread "will receive an InterruptedException".[^4] Many methods that throw this exception "are designed to cancel their current operation and return immediately when an interrupt is received".[^1]
- **.NET.** A thread can wait on `CancellationToken.WaitHandle` together with another event, so a cancellation ends the wait.[^2]
- **C++.** A `std::stop_token` can be passed "to the interruptible waiting functions of std::condition_variable_any", which ends the wait when a stop is requested.[^5]
- **Go.** A goroutine waits in a `select` on `ctx.Done()` and on its other channels. The `Done` channel closes when the cancel function is called.[^6]

The examples show this. Each worker starts a wait of 10 seconds, and the main thread then asks it to stop. The worker wakes, prints a message and ends in well under 1 second.

### Forced stops

Some systems can also stop a thread from outside, without its help. This is almost always unsafe:

- **Locks and broken data.** Java's `Thread.stop` made the thread unlock all of its monitors. If an object that a monitor protected was half changed, "other threads may have viewed these objects in an inconsistent state".[^3] The thread could be stopped "almost anywhere", so code could not clean up reliably.[^3]
- **Locks that stay locked.** Windows `TerminateThread` does the opposite. If the target thread owns a critical section, "the critical section will not be released", and if it is allocating memory, "the heap lock will not be released".[^7]
- **No defense.** "A thread cannot protect itself against TerminateThread", other than by controlling access to its handles.[^7]

POSIX `pthread_cancel` is between the two kinds. It sends a request, and "The target thread's cancelability state and type determines when the cancellation takes effect". Before the thread ends, its cleanup handlers and the destructors of its thread-specific data run.[^8]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | no interrupt; a stop flag such as `threading.Event` |
| JavaScript | stdlib | `worker.terminate()`; a flag in a `SharedArrayBuffer` |
| TypeScript | stdlib | as JavaScript |
| Java | stdlib | `Thread.interrupt`, `InterruptedException` |
| Kotlin | stdlib | the Java `Thread.interrupt` on the JVM |
| C# | stdlib | `CancellationTokenSource`, `CancellationToken` |
| Go | stdlib | `context.WithCancel`, `ctx.Done()` |
| Rust | absent | a stop flag such as `AtomicBool`; `park` and `unpark` |
| C | stdlib | `pthread_cancel` in POSIX |
| C++ | stdlib | `std::jthread`, `std::stop_source`, `std::stop_token` |
| Bash | n/a | one thread for each process |
| PowerShell | stdlib | the .NET `CancellationToken` |
| SQL | n/a | a query has no threads |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

- **Python.** Threads "cannot be destroyed, stopped, suspended, resumed, or interrupted".[^9] To stop a thread "gracefully", the documentation says to use "a suitable signalling mechanism such as an Event".[^9]
- **Java.** `interrupt()` sets the interrupt status of the thread, or wakes it from a blocking call with an `InterruptedException`.[^4] `Thread.stop` is deprecated for removal, and in Java 21 it throws `UnsupportedOperationException` always.[^4]
- **C#.** The code that starts the work creates a `CancellationTokenSource` and passes its token to each thread or task. A listener can notice a request "by polling, callback registration, or waiting on wait handles".[^2] `Thread.Abort` "throws a PlatformNotSupportedException at run time on .NET 5 and later and .NET Core".[^10]
- **Go.** A `Context` carries "cancellation signals", and "When a Context is canceled, all Contexts derived from it are also canceled".[^6] So one cancel can stop a whole tree of goroutines.
- **Rust.** The `std::thread` module can spawn, sleep, park and join threads, but it has no function to stop another thread.[^11] The example uses an `AtomicBool` and `park_timeout`.
- **C++.** `std::jthread` "can be cancelled/stopped in certain situations". It holds a `std::stop_source` and passes a `std::stop_token` to the thread function.[^12] Its destructor asks the thread to stop and joins it.[^12]
- **JavaScript.** Node.js `worker.terminate()` will "Stop all JavaScript execution in the worker thread as soon as possible".[^13] A worker shares memory only "by transferring ArrayBuffer instances or sharing SharedArrayBuffer instances", so a forced stop can leave only that shared memory half changed.[^13]

## Variations and related ideas

- **Deadlines and timeouts.** A Go context can also have a deadline: "A Context with a deadline is canceled after the deadline passes".[^6] The same check then handles both a cancel and a timeout.
- **Cancellation trees.** Go contexts and .NET linked tokens join requests. A child context is canceled with its parent, and a .NET listener can "listen to multiple tokens simultaneously by joining them into one linked token".[^6][^2]
- **Callbacks.** Instead of polling, a .NET listener can register a callback, and a C++ `std::stop_callback` runs when a stop is requested.[^2][^5]
- **Processes instead of threads.** Work that may need a forced stop can run in a separate process. See [[process-based-parallelism]].
- **Async cancellation.** In [[async-await]] code, the same tokens and contexts cancel tasks instead of threads. The .NET model covers "asynchronous or long-running synchronous operations".[^2]

## Pitfalls

- **A request that nobody checks.** An interrupt only sets a flag. "For the interrupt mechanism to work correctly, the interrupted thread must support its own interruption".[^1] A loop that never checks the flag never stops.
- **Swallowing the interrupt.** In Java, the `InterruptedException` clears the interrupt status.[^4] Code that catches the exception and does nothing loses the request. Code that cannot rethrow it should call `Thread.currentThread().interrupt()`, so the thread will "reraise the InterruptedException as soon as it is able".[^3]
- **A flag that other threads cannot see.** A plain variable may not be visible to the other thread at once. In Java, "the variable must be volatile (or access to the variable must be synchronized)".[^3] Use `Event`, an atomic type or a token, as the examples do.
- **Killing threads.** Do not use `Thread.stop`, `Thread.Abort` or `TerminateThread` to stop normal work. `TerminateThread` "is a dangerous function that should only be used in the most extreme cases".[^7]
- **Forgetting to cancel.** In Go, "Failing to call the CancelFunc leaks the child and its children until the parent is canceled".[^6]
- **Daemon threads at exit.** In Python, daemon threads "are abruptly stopped at shutdown", and their resources "may not be released properly".[^9] In Rust, when the main thread ends, "any destructors on the remaining threads’ stacks may not be executed".[^11]

## Cost

- **Checks.** A check of a flag is cheap. .NET says that "Polling itself does not significantly impact performance".[^2] The developer chooses how often to check, which sets how fast the thread responds.[^2]
- **Latency.** A thread stops only at its next check or at the end of its current wait. A long step with no checks delays the stop.[^1]
- **Code.** Every long loop and every wait must handle the request, and the thread must clean up on the way out.[^3]

## History

- **POSIX.** `pthread_cancel` was "First released in Issue 5", for alignment with the POSIX Threads Extension.[^8]
- **Java 1.2.** Java deprecated `Thread.stop` in version 1.2.[^4] In Java 21, it always throws `UnsupportedOperationException`.[^4]
- **.NET 5.** `Thread.Abort` throws `PlatformNotSupportedException` on .NET 5 and later and on .NET Core.[^10]
- **C++20.** `std::jthread` and `std::stop_token` came with C++20.[^12][^5]

## References
