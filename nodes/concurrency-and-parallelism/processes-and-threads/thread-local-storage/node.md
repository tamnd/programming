---
tag: 18V7W5
kind: concept
slug: thread-local-storage
name: Thread-local storage
aliases: [TLS, thread-local variable, thread-specific data, ThreadLocal, thread_local, threading.local, ThreadStatic, pthread_key_create, _Thread_local]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: Thread-local storage gives each thread its own copy of a variable. Code reads and writes the variable by one name, and each thread sees only its own value.
applies_to: core-languages
viz: optional
history:
  year: 1998
  note: Java added the ThreadLocal class in version 1.2, which came out in December 1998.
see_also: [threads, use-thread-local-storage, run-tasks-on-a-thread-pool, executors, fibers, virtual-threads, data-races, async-await]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/ThreadLocal.html
    title: Java SE 21 API, java.lang.ThreadLocal
    locator: class description
  - url: https://en.wikipedia.org/wiki/Thread-local_storage
    title: Wikipedia, Thread-local storage
    locator: introduction; Usage; Windows implementation; Pthreads implementation; C and C++
  - url: https://openjdk.org/jeps/506
    title: "JEP 506: Scoped Values"
    locator: Motivation; Problems with thread-local variables
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_key_create.html
    title: POSIX.1-2024, pthread_key_create
    locator: DESCRIPTION; CHANGE HISTORY
  - url: https://en.cppreference.com/w/cpp/language/storage_duration
    title: cppreference, Storage duration
    locator: Thread storage duration
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threadstaticattribute
    title: Microsoft Learn, ThreadStaticAttribute class
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.threadlocal-1
    title: Microsoft Learn, ThreadLocal<T> class
    locator: class description; Examples
  - url: https://docs.python.org/3/library/threading.html
    title: Python documentation, threading
    locator: Thread-local data
  - url: https://doc.rust-lang.org/std/macro.thread_local.html
    title: Rust standard library, macro std::thread_local
    locator: Syntax
  - url: https://doc.rust-lang.org/std/thread/struct.LocalKey.html
    title: Rust standard library, struct std::thread::LocalKey
    locator: Initialization and Destruction; Platform-specific behavior
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Why is there no goroutine ID?
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: introduction; Class Worker
  - url: https://nodejs.org/api/async_context.html
    title: Node.js documentation, Asynchronous context tracking
    locator: Introduction
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.asynclocal-1
    title: Microsoft Learn, AsyncLocal<T> class
    locator: class description
  - url: https://docs.python.org/3/library/contextvars.html
    title: Python documentation, contextvars
    locator: introduction
  - url: https://learn.microsoft.com/en-us/windows/win32/procthread/fibers
    title: Microsoft Learn, Win32 apps, Fibers
    locator: Fiber Local Storage
  - url: https://en.wikipedia.org/wiki/Java_version_history
    title: Wikipedia, Java version history
    locator: Release table
---

## Idea

A thread-local variable has a separate value for each thread. All threads use the same name for it, but a read or a write in one thread uses only the value of that thread. The Java documentation says that each thread that uses one "has its own, independently initialized copy of the variable".[^1]

Wikipedia gives the classic reason. The C library reports errors in a global variable, `errno`. With many threads, a call in one thread could overwrite the error code of a call in another thread before that thread reads it. The solution is to make `errno` look global but store it "in a per-thread memory pool, the thread-local storage".[^2]

A second use is to avoid locks. Each thread can add to its own thread-local total with no lock, and the threads combine the totals once at the end.[^2] A third use is to pass context, such as the current user or transaction, down a call chain without a parameter on every method.[^1][^3]

## How it works

### Keys and slots

The oldest form is an API with keys. A program creates a key once. Each thread can then store one value under the key and read it back:[^2][^4]

- **POSIX.** `pthread_key_create` creates "a thread-specific data key visible to all threads in the process". Each thread binds its own value to the key with `pthread_setspecific` and reads it with `pthread_getspecific`.[^4]
- **Windows.** `TlsAlloc` returns an unused slot index, and `TlsSetValue` and `TlsGetValue` write and read the value of the current thread in that slot.[^2]

A key can have a destructor. When a thread exits with a value under the key, the system calls the destructor with that value, so the thread can free memory or close connections.[^4][^2]

### Language support

Most languages hide the keys behind a variable or an object:

- **A storage class.** C++ marks a variable `thread_local`. The variable has thread storage duration: "There is a distinct object or reference per thread", and the object lasts as long as its thread.[^5] C11 has the keyword `_Thread_local`, and C23 makes `thread_local` a keyword.[^2] In .NET, a static field with `[ThreadStatic]` "is unique for each thread".[^6]
- **An object.** Java `ThreadLocal`, .NET `ThreadLocal<T>`, and Python `threading.local` are objects with a different value in each thread.[^1][^7][^8] Rust's `thread_local!` macro declares a `LocalKey`, which gives a reference to the value of the current thread inside a closure.[^9][^10]

The compiler and the runtime usually find the value of the current thread through a register. On RISC machines, "the calling convention often reserves a thread pointer register for this use".[^2]

### Initialization and cleanup

A new thread does not see the values of other threads. It starts with an initial value, or with no value:

- In Rust, "Initialization is dynamically performed" on the first use in each thread, and the value is dropped when the thread exits.[^10]
- Java `ThreadLocal.withInitial` takes a function that makes the first value in each thread.[^1]
- Python `threading.local` has no attributes in a new thread. In the documentation's example, a new thread sees an empty dictionary, and "changes made in the other thread don’t affect data seen in this thread".[^8]

The examples show this. Each thread counts on its own thread-local counter. The main thread counts to 3, and the threads count to 1 and 2, with no lock and no effect on each other.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `threading.local` |
| JavaScript | absent | no shared variables between threads; `AsyncLocalStorage` in Node.js |
| TypeScript | absent | as JavaScript |
| Java | stdlib | `ThreadLocal`, `InheritableThreadLocal` |
| Kotlin | stdlib | the Java `ThreadLocal` on the JVM |
| C# | stdlib | `[ThreadStatic]`, `ThreadLocal<T>` |
| Go | absent | values passed to each goroutine; `context.Context` |
| Rust | stdlib | the `thread_local!` macro and `LocalKey` |
| C | native | `_Thread_local`, or `thread_local` in C23; `tss_` functions; `pthread_key_create` |
| C++ | native | the `thread_local` storage class |
| Bash | n/a | one thread for each process |
| PowerShell | absent | no thread-local variables in the language |
| SQL | n/a | a session has its own settings, but a query has no threads |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

- **Java.** `ThreadLocal` instances "are typically private static fields in classes that wish to associate state with a thread".[^1] A thread keeps its copy "as long as the thread is alive", and after the thread ends, its copies can be garbage-collected.[^1]
- **C#.** A field with `[ThreadStatic]` must also be `static`. Do not give it an initial value in the declaration: that code runs once, "and therefore affects only one thread".[^6] `ThreadLocal<T>` takes a factory that makes the value for each thread.[^7]
- **Python.** `threading.local()` returns an object whose attributes are "thread specific".[^8] A subclass can set defaults and methods.[^8]
- **Rust.** A `LocalKey` gives a reference to the value only inside `with`, and that reference "cannot outlive the current thread or escape the given closure".[^10] To change the value, store it in a `Cell` or a `RefCell`.[^9]
- **Go.** Goroutines "expose no unique identifier, name, or data structure to the programmer", so there is nothing to attach a goroutine-local value to.[^11] The FAQ gives the reason: if a package tied state to one goroutine, its users could not use more goroutines for one request.[^11] Go code passes state as values, and passes request values in a `context.Context`.
- **JavaScript.** A worker is "an independent JavaScript execution thread", and workers share memory only through `ArrayBuffer` transfers and `SharedArrayBuffer`.[^12] So every global variable is already local to its thread. Node.js also has `AsyncLocalStorage`, which keeps a value through callbacks and promise chains. Node.js says that it "is similar to thread-local storage in other languages".[^13]

## Variations and related ideas

- **Async-local storage.** In async code, one task can move between threads at each `await`, and one thread runs many tasks. A thread-local value is then wrong in both directions. .NET `AsyncLocal<T>` holds data "local to a given asynchronous control flow".[^14] Python asks code with state to use `contextvars` "instead of threading.local() to prevent their state from bleeding to other code unexpectedly".[^15]
- **Scoped values.** Java scoped values share data one way, from a method to the methods that it calls, for a bounded time. JEP 506 presents them as an answer to three problems of thread-local variables: "Unconstrained mutability", "Unbounded lifetime" and "Expensive inheritance".[^3]
- **Inheritance.** Java `InheritableThreadLocal` copies values from a parent thread to the child threads that it creates. Each child must allocate storage for every inherited variable, which costs memory with many threads.[^3]
- **[[fibers]].** A fiber that reads thread-local storage reads the storage "of the thread that is running it". Windows has fiber-local storage for values that belong to one fiber.[^16]
- **[[virtual-threads]].** JEP 506 says that the problems of thread-local variables "have become more pressing with the availability of virtual threads", because a program can have millions of them.[^3]

## Pitfalls

- **Leaks in thread pools.** A pool reuses its threads, so a value that one task sets stays for the next task on that thread. JEP 506 warns that "the value of a thread-local variable set in one task could, if not properly cleared, accidentally leak into an unrelated task".[^3] The examples show this with a pool of one thread. Remove the value at the end of each task, for example in a `finally` block.
- **Memory that is never freed.** A value stays "for the lifetime of the thread, or until code in the thread calls the remove method".[^3] The threads of a pool can live as long as the program, so their values can too.[^3]
- **Values that cross threads.** A value read from a thread-local variable belongs to the thread that read it. Python warns that you "generally don’t want to save these values across threads".[^8]
- **Hidden inputs.** A method that reads a thread-local variable has an input that is not in its parameters. Any code that can read the variable can also change it, which can make the data flow hard to follow.[^3]
- **Async code.** After an `await`, the code can continue on another thread, with other thread-local values. Use async-local storage for values that must follow the task.[^14][^15]
- **Destructors.** Rust does not promise that destructors run for every thread-local value on every platform.[^10] Do not use a thread-local destructor for work that must happen.

## Cost

- **Access.** A thread-local access must find the storage of the current thread first. With compiler support, this uses a register that points to the thread's data.[^2] A key-based API makes a function call for each read and write.[^4]
- **Memory.** Each thread that uses a variable has its own copy, so the memory grows with the number of threads.[^3] Rust's `const` initializer can "avoid lazy initialization", which makes access cheaper.[^9]
- **No locks.** A value that only one thread uses needs no lock. This is often the reason to use thread-local storage: per-thread totals replace a shared total with a lock.[^2]

## History

POSIX thread-specific data came with the POSIX threads extension. The `pthread_key_create` page says that it was "Included for alignment with the POSIX Threads Extension".[^4] Java added `ThreadLocal` in version 1.2, released in December 1998.[^3][^17] C++11 added the `thread_local` keyword, and C11 added `_Thread_local`.[^2] Later, async code led to async-local storage, such as `contextvars` in Python 3.7, `AsyncLocal<T>` in .NET, and `AsyncLocalStorage` in Node.js.[^15][^14][^13] Java scoped values followed, for the very large numbers of threads that virtual threads allow.[^3]

## References
