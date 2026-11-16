---
tag: 44BD7Y
kind: concept
slug: global-interpreter-lock
name: Global interpreter lock
aliases: [GIL, GVL, global VM lock, Python GIL, free-threaded Python, free threading, --disable-gil]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: A global interpreter lock lets only one thread of a process run interpreter code at a time. CPython and Ruby MRI have one, so CPU-bound threads take turns instead of running in parallel.
applies_to: core-languages
viz: optional
history:
  year: 2024
  note: Python 3.13, released in October 2024, added an experimental free-threaded build of CPython, in which the GIL is disabled.
see_also: [threads, process, process-based-parallelism, data-races, atomic-operations, mutual-exclusion, green-threads]
sources:
  - url: https://docs.python.org/3/glossary.html
    title: Python documentation, Glossary
    locator: global interpreter lock; global state
  - url: https://en.wikipedia.org/wiki/Global_interpreter_lock
    title: Wikipedia, Global interpreter lock
    locator: introduction; Technical background concepts; Advantages; Drawbacks; Examples
  - url: https://docs.python.org/3/library/threading.html
    title: The Python Standard Library, threading
    locator: CPython implementation detail; GIL and performance considerations
  - url: https://docs.python.org/3/library/sys.html
    title: The Python Standard Library, sys
    locator: setswitchinterval
  - url: https://docs.python.org/3/whatsnew/3.2.html
    title: What's New In Python 3.2
    locator: Multi-threading
  - url: https://peps.python.org/pep-0703/
    title: "PEP 703: Making the Global Interpreter Lock Optional in CPython"
    locator: header; Abstract; Motivation
  - url: https://docs.python.org/3/whatsnew/3.13.html
    title: What's New In Python 3.13
    locator: introduction; Free-threaded CPython
  - url: https://docs.python.org/3/howto/free-threading-python.html
    title: Python HOWTOs, Python support for free threading
    locator: introduction; The global interpreter lock in free-threaded Python; Thread safety; Single-threaded performance
  - url: https://docs.python.org/3/library/multiprocessing.html
    title: The Python Standard Library, multiprocessing
    locator: Introduction; Exchanging objects between processes; Sharing state between processes
  - url: https://docs.ruby-lang.org/en/3.4/ractor_md.html
    title: Ruby documentation, Ractor - Ruby's Actor-like concurrent abstraction
    locator: Summary
  - url: https://docs.ruby-lang.org/en/master/Ractor.html
    title: Ruby documentation, class Ractor
    locator: class description; Ractors vs threads
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: introduction; Class Worker
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Atomics
    title: MDN Web Docs, Atomics
    locator: introduction
  - url: https://docs.oracle.com/javase/specs/jls/se21/html/jls-17.html
    title: The Java Language Specification, Java SE 21, Chapter 17, Threads and Locks
    locator: introduction
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/atomic/AtomicLong.html
    title: Java SE 21 API, java.util.concurrent.atomic.AtomicLong
    locator: class description
  - url: https://learn.microsoft.com/en-us/dotnet/standard/threading/threads-and-threading
    title: Microsoft Learn, Threads and threading
    locator: Multithreading; When to use multiple threads
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.interlocked
    title: Microsoft Learn, Interlocked class
    locator: class description; Read
  - url: https://pkg.go.dev/runtime
    title: Go standard library, package runtime
    locator: Environment Variables; func GOMAXPROCS
  - url: https://doc.rust-lang.org/book/ch16-01-threads.html
    title: The Rust Programming Language, Using Threads to Run Code Simultaneously
    locator: introduction
  - url: https://doc.rust-lang.org/std/sync/atomic/index.html
    title: Rust standard library, module std::sync::atomic
    locator: module description
  - url: https://en.cppreference.com/w/cpp/atomic/atomic
    title: cppreference, std::atomic
    locator: description
---

## Idea

A global interpreter lock, or GIL, is one lock for a whole interpreter. A thread must hold the lock to run interpreter code, so only one thread runs that code at a time. The Python glossary calls the GIL "The mechanism used by the CPython interpreter to assure that only one thread executes Python bytecode at a time".[^1] Wikipedia gives the general form: only one native thread for each process "can execute basic operations", such as memory allocation and reference counting, at a time.[^2]

The lock makes the interpreter simpler. Its own data, such as the reference counts of objects, is "implicitly safe against concurrent access".[^1] The price is parallelism. The lock is held "at the expense of much of the parallelism afforded by multi-processor machines".[^1] Threads still help with I/O, but threads that only calculate take turns.[^3]

Some popular interpreters that have a GIL are CPython and Ruby MRI.[^2] In Ruby, the lock is called the Global VM Lock, or GVL.[^2]

## How it works

### Taking turns

There is always one GIL for each interpreter process.[^2] A thread that runs interpreter code holds it. The other threads wait until the holder releases it:

- **At blocking calls.** In CPython, "the GIL is always released when doing I/O".[^1] A thread that waits for a file, a socket or a sleep lets other threads run.
- **In extension code.** Some extension modules release the GIL "when doing computationally intensive tasks such as compression or hashing".[^1] Then their C code can run in parallel with Python code.
- **After a time slice.** CPython asks a running thread to give up the lock after a switch interval. `sys.setswitchinterval` sets "the ideal duration of the “timeslices” allocated to concurrently running Python threads".[^4] The default is 5 milliseconds.[^5] The interpreter does not choose the next thread: it "doesn’t have its own scheduler", and the operating system decides.[^4]

### What it means for programs

The Python documentation sums it up: "the GIL limits the performance gains of threading when it comes to CPU-bound tasks, as only one thread can execute Python bytecode at a time".[^3] For CPU-bound work on many cores, it advises "multiprocessing or concurrent.futures.ProcessPoolExecutor". For I/O, "threading is still an appropriate model if you want to run multiple I/O-bound tasks simultaneously".[^3]

The Python example shows both cases. Four threads that count down take at least twice as long as one thread, because they take turns. Four threads that sleep for 0.2 seconds finish in under 0.4 seconds, because a sleeping thread releases the lock.

### Why interpreters use one

Wikipedia gives three reasons for a GIL:[^2]

- **Speed with one thread.** The interpreter does not need to take and release a lock for each data structure.
- **C libraries.** It is easy to use C libraries that are not thread-safe.
- **Simple code.** One lock is much simpler to write than a lock-free interpreter or one with fine-grained locks.

### Free-threaded CPython

PEP 703 says that the GIL "is an obstacle to using multi-core CPUs from Python efficiently". It proposed a build option, `--disable-gil`, to run Python code without the lock.[^6] CPython 3.13 added this build as an experiment.[^7] The documentation calls it free threading: a build "where the global interpreter lock (GIL) is disabled".[^8] The executable of the free-threaded build is "usually called python3.13t".[^7]

In the free-threaded build:

- Threads can run Python code in parallel on all cores.[^8]
- `sys._is_gil_enabled()` tells whether the GIL is on in the running process.[^7]
- The GIL can come back. Some extension modules "may not be ready for use in a free-threaded build, and will re-enable the GIL".[^8] The `PYTHON_GIL` environment variable and the `-X gil` option also turn it on.[^8]
- Built-in types such as `dict`, `list` and `set` use internal locks that "behave similarly to the GIL".[^8]
- Code with one thread runs a little slower. On the pyperformance benchmarks, the average overhead is "from about 1% on macOS aarch64 to 8% on x86-64 Linux systems".[^8]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | the GIL in CPython; a free-threaded build without it |
| JavaScript | absent | one thread for each engine instance; workers in parallel |
| TypeScript | absent | as JavaScript |
| Java | absent | JVM threads run in parallel |
| Kotlin | absent | JVM threads run in parallel |
| C# | absent | .NET threads run in parallel |
| Go | absent | goroutines on up to `GOMAXPROCS` threads |
| Rust | absent | operating system threads |
| C | absent | operating system threads |
| C++ | absent | operating system threads |
| Bash | n/a | one thread for each process |
| PowerShell | absent | .NET threads; runspaces in parallel |
| SQL | n/a | a query has no threads |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

- **Python.** CPython has the GIL. In CPython, "only one thread can execute Python code at once (even though certain performance-oriented libraries might overcome this limitation)".[^3] Other implementations differ: Jython, on the JVM, does "not use global interpreter locks".[^2] The `multiprocessing` package runs work in subprocesses, "effectively side-stepping the Global Interpreter Lock".[^9]
- **Ruby.** Threads in one ractor share "a Ractor-wide global lock like GIL (GVL in MRI terminology), so they can’t run in parallel".[^10] Ractors are Ruby's way to run code in parallel: "the Global Virtual Machine Lock (GVL) is held per ractor, so ractors can run in parallel".[^11] JRuby has no global interpreter lock.[^2]
- **JavaScript.** An engine runs the code of one instance on one thread, so it needs no GIL. For parallel work, Node.js has workers. A worker is "an independent JavaScript execution thread", and workers "are useful for performing CPU-intensive JavaScript operations".[^12] Workers can share a `SharedArrayBuffer`, and the `Atomics` object has "static methods for carrying out atomic operations" on it.[^13]
- **Java.** Threads may run on "many hardware processors", by "time-slicing a single hardware processor, or by time-slicing many hardware processors".[^14] Shared data needs locks or atomic types, such as `AtomicLong`, "A long value that may be updated atomically".[^15]
- **C#.** A program can "create additional threads to execute code in parallel or concurrently with the primary thread".[^16] The `Interlocked` class "Provides atomic operations for variables that are shared by multiple threads".[^17]
- **Go.** The runtime has no GIL. The `GOMAXPROCS` variable "limits the number of operating system threads that can execute user-level Go code simultaneously".[^18]
- **Rust, C and C++.** Threads are operating system threads that "run simultaneously".[^19] Shared data needs locks or atomic types. Rust atomic types "provide primitive shared-memory communication between threads".[^20] In C++, "If one thread writes to an atomic object while another thread reads from it, the behavior is well-defined".[^21]

## Variations and related ideas

- **[[process-based-parallelism]].** Each process has its own interpreter and its own GIL, so processes run in parallel. This is the usual answer to the GIL for CPU-bound work.[^3][^9]
- **One lock for each ractor.** Ruby holds one GVL for each ractor, so code in different ractors runs in parallel, and objects move between ractors as messages.[^11]
- **Giant locks and [[green-threads]].** Wikipedia lists both as ideas related to the GIL.[^2]

## Pitfalls

- **Thinking that the GIL makes code thread-safe.** The GIL protects the data of the interpreter, not the data of your program. A step such as `total += 1` reads the value, adds 1 and writes the result, and another thread can run between these steps. The glossary says that global state shared between threads "typically requires synchronization to avoid race conditions and data races".[^1] The Python example uses a `Lock` for its total. The free-threading guide also recommends "the threading.Lock or other synchronization primitives instead of relying on the internal locks of built-in types".[^8]
- **Threads for CPU-bound work.** More threads do not make pure Python calculations faster under the GIL. Wikipedia says that there is "likely to be very little increase in speed" on a multiprocessor machine, and that a CPU-bound thread can cause "a significant slowdown, even on single processors".[^2] Use processes, or a library that releases the GIL.[^3][^1]
- **Assuming that the GIL is always there.** On a free-threaded build, code that worked only because of the GIL can fail. Test threaded code on both builds.[^8]
- **Extensions that turn the GIL back on.** In the free-threaded build, importing an extension module that does not support free threading can re-enable the GIL, and a warning is printed.[^8]

## Cost

- **With one thread.** A GIL makes code with one thread fast, because the interpreter takes one lock instead of many small ones.[^2] The free-threaded build of CPython gives up some of this speed.[^8]
- **With many threads.** CPU-bound Python threads under the GIL run no faster than one thread, and switching between them adds overhead.[^3][^2] I/O-bound threads lose little, because they release the GIL while they wait.[^1]
- **Processes instead.** Processes avoid the GIL, but data must go between them through queues or pipes, or be put in shared memory.[^9]

## History

- **2011.** Python 3.2, released on 20 February 2011, rewrote the GIL. A switch interval in seconds replaced the old "check interval", and its default became 5 milliseconds.[^5]
- **2023.** PEP 703, "Making the Global Interpreter Lock Optional in CPython", was accepted on 24 October 2023.[^6]
- **2024.** Python 3.13, released in October 2024, added "experimental support for running in a free-threaded mode, with the global interpreter lock (GIL) disabled".[^7]

## References
