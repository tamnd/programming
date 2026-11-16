---
tag: KEH1ME
kind: concept
slug: green-threads
name: Green threads
aliases: [user-level threads, user-space threads, M:N threading, lightweight threads, coroutine scheduler, goroutines, Erlang processes, libgreen]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: A green thread is a thread that a runtime or virtual machine schedules, not the operating system. Many green threads can share a few OS threads, so they are cheap to create.
applies_to: core-languages
viz: optional
history:
  year: 1997
  note: Java had green threads from version 1.1, between 1997 and 2000, and then changed to native threads. Rust removed its green threading runtime from the standard library through RFC 230, which started in 2014.
see_also: [threads, virtual-threads, process, start-a-thread, futures, message-passing-concept]
sources:
  - url: https://en.wikipedia.org/wiki/Green_thread
    title: Wikipedia, Green thread
    locator: the introduction; History; Performance; Green threads in the Java Virtual Machine; Green threads in other languages
  - url: https://raw.githubusercontent.com/rust-lang/rfcs/master/text/0230-remove-runtime.md
    title: "Rust RFC 230: Remove runtime"
    locator: Summary; Motivation, Background; Where Rust is now
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Why goroutines instead of threads?
  - url: https://pkg.go.dev/runtime
    title: Go standard library, package runtime
    locator: Environment Variables, GOMAXPROCS
  - url: https://docs.oracle.com/en/java/javase/21/core/virtual-threads.html
    title: Java SE 21 Core Libraries, Virtual Threads
    locator: What is a Virtual Thread?
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Go statements
  - url: https://openjdk.org/jeps/444
    title: "JEP 444: Virtual Threads"
    locator: Summary; Description
  - url: https://www.erlang.org/doc/system/ref_man_processes.html
    title: Erlang System Documentation, Processes
    locator: the introduction
---

## Idea

A green thread is a thread that a runtime library or a virtual machine schedules, not the operating system.[^1] To the program, a green thread looks like an ordinary [[threads|thread]]: it has its own flow of control, and the runtime switches between green threads. But the operating system does not know about it. The runtime keeps green threads in user space and runs them on one or more OS threads.[^1]

The simplest form is cooperative. Each green thread runs until it gives the processor back, and then a scheduler picks the next one. In the Python example below, each generator is a green thread, and `yield` is the point where it gives the processor back. A loop of a few lines is the scheduler. The two green threads take turns on one OS thread.

```python run id=idea
from collections import deque


def worker(name, steps):
    for i in range(steps):
        print(f"{name} step {i}")
        yield  # give the processor back to the scheduler


ready = deque([worker("a", 3), worker("b", 2)])
while ready:
    task = ready.popleft()
    try:
        next(task)
    except StopIteration:
        continue
    ready.append(task)
```

```output for=idea
a step 0
b step 0
a step 1
b step 1
a step 2
```

The scheduler runs `a` until its first `yield`, then `b`, then `a` again. A green thread that ends leaves the queue. Nothing here is parallel: one OS thread runs every step. A real runtime adds what this loop does not have: a stack for each green thread, a switch point inside each blocking call, and more than one OS thread to run on.

## How it works

### 1:1 and M:N

Rust RFC 230 names the basic choice. In the 1:1 model, each task is one OS thread. In the M:N model, a scheduler in user space maps M tasks onto N worker threads.[^2] Green threads are the M:N model, or its special case with one worker thread. The first green threads in Java shared "a single operating system thread", so they could not run in parallel on several processors.[^1]

Modern runtimes use more than one worker thread. Go multiplexes goroutines "onto a set of threads".[^3] The variable `GOMAXPROCS` "limits the number of operating system threads that can execute user-level Go code simultaneously".[^4] Java maps a large number of virtual threads to a small number of OS threads.[^5] See [[virtual-threads]].

### Stacks

A green thread needs a place for its local variables while it waits. There are two ways:[^2]

- **A stack for each green thread.** The runtime allocates a stack and switches stacks when it switches threads. A fixed stack makes each thread heavy. Rust's old green threads used pre-allocated stacks, so the RFC says that they were not "lightweight".[^2] Go instead gives each new goroutine "a few kilobytes" and grows or shrinks the stack when needed.[^3]
- **No stack.** The task keeps only the state it needs in a closure or a state machine, as the generators of the Idea section do. Such tasks can be "extremely lightweight", but each blocking operation must take the rest of the work as a closure, or the task cannot move between worker threads.[^2]

### Blocking calls

The hard part of green threads is a blocking system call. The OS blocks the OS thread, and with it every green thread that runs on that OS thread. With one OS thread, the whole process stops.[^1] There are three answers:

- **Non-blocking I/O.** The runtime gives its own versions of the blocking calls. They start the operation, switch to another green thread, and come back when the operation is ready.[^1][^2]
- **More OS threads.** When a goroutine blocks in a system call, the Go runtime moves the other goroutines of that OS thread "to a different, runnable thread", so that they are not blocked.[^3] Threads that are blocked in system calls do not count against `GOMAXPROCS`.[^4]
- **Nothing.** A loop that never yields, or a page fault, still blocks a worker thread. The RFC notes that task-aware blocking calls help "only with explicit blocking".[^2]

## In languages

The example files show what each language has in place of OS threads for many small tasks. The Python, Rust and C++ examples build a small cooperative scheduler by hand, with the same output as the Idea section. The Go, Java, C# and JavaScript examples use the scheduler of their runtime. The table gives the verdict for each tier 1 language.

| Language | Verdict | Mechanism |
|---|---|---|
| Python | library | `greenlet` and `gevent`; generators and `asyncio` coroutines in the standard library |
| JavaScript | absent | one thread for each event loop; `async` functions instead |
| TypeScript | absent | as JavaScript |
| Java | stdlib | virtual threads (Java 21) |
| Kotlin | library | coroutines in `kotlinx.coroutines`; Java virtual threads on the JVM |
| C# | absent | `async` methods and tasks on the thread pool instead |
| Go | native | goroutines, started with the `go` statement |
| Rust | absent | removed in 2014; `async` tasks with a library executor instead |
| C | idiom | `makecontext` and `swapcontext`, or a library |
| C++ | idiom | the C calls, or C++20 coroutines |
| Bash | absent | no threads; background processes instead |
| PowerShell | absent | thread jobs and runspaces are OS threads |
| SQL | n/a | a query does not start threads |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

- **Go.** A `go` statement starts "an independent concurrent thread of control, or goroutine, within the same address space".[^6] It is "practical to create hundreds of thousands of goroutines in the same address space".[^3]
- **Java.** Java 1.1 used green threads, at least on Solaris, and later versions dropped them for native threads.[^1] Java 21 brought back the M:N model as virtual threads.[^7]
- **Rust.** Rust first had only green threads, then added native threads, and native threads became the default.[^2] RFC 230 then removed the runtime from the standard library and moved `libgreen` out of it.[^2] Today `std::thread` is 1:1, and `async` tasks take the place of green threads.
- **Python.** The standard library has no green threads. The libraries `greenlet`, `eventlet` and `gevent` give them to CPython.[^1]
- **C.** On POSIX systems, `makecontext` gives lightweight cooperative threads, but POSIX.1-2008 does not include it.[^1] glibc still provides it, as the C++ example shows.

## Variations and related ideas

- **[[virtual-threads]].** Java's virtual threads are green threads that do not block their OS thread when they sleep or wait for I/O.[^1][^7]
- **Erlang processes.** The Erlang runtime schedules its own processes. They share no memory, and they are "lightweight (grow and shrink dynamically) with a small memory footprint, fast to create and terminate".[^8] See [[message-passing-concept]].
- **Coroutines and async functions.** A coroutine that yields at fixed points is a green thread without a stack of its own. The RFC calls this style continuation-passing style, in which "each blocking operation takes a closure representing the work left to do".[^2] The Rust and C++ examples on this page work this way.

## Pitfalls

- **A blocking call that the runtime does not know.** A call into C code or a plain blocking system call can stop every green thread on the same OS thread.[^1] With libgreen in Rust, any native code that called blocking I/O could "block the worker thread running the green scheduler".[^2]
- **A loop that never yields.** In a cooperative scheduler, a green thread that computes for a long time without a switch point keeps all the other green threads waiting.[^2]
- **No parallel speedup with one OS thread.** Green threads on one OS thread do not use more than one processor.[^1] CPU-bound work needs a runtime with several worker threads, or OS threads.
- **Green and native threads in one program.** In Rust, before RFC 230, `std::io` objects "created on a native task cannot safely be used within a green task".[^2] A program that mixes the two models must know which calls work in both.

## Cost

- **Memory.** A goroutine needs little more than the memory of its stack, which starts at a few kilobytes. The Go FAQ says that if goroutines were OS threads, "system resources would run out at a much smaller number".[^3]
- **Start and synchronization.** Green threads are managed in user space. Benchmarks on Linux 2.2 showed that green threads were much faster than native threads to start and to synchronize, and slightly slower for I/O and context switches.[^1]
- **Runtime.** The program carries a scheduler, and often its own I/O layer. In Rust, this layer added a cost to binary size and to task-local storage in every program, and that cost was one reason to remove it.[^2]

## History

The name comes from Java. Green threads were the original thread library of Java, designed by the Green Team at Sun Microsystems. They were available in Java between 1997 and 2000, and later versions changed to native threads.[^1] Rust also began with green threads only. Its RFC 230, which started in 2014, removed the runtime from the standard library and tied `std::io` to native threads.[^2] The M:N model came back in newer runtimes: Go multiplexes its goroutines onto OS threads,[^3] and Java 21 made virtual threads final.[^7]

## References
