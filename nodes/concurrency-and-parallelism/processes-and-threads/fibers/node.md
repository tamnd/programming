---
tag: D8F89R
kind: concept
slug: fibers
name: Fibers
aliases: [fiber, user-mode thread, stackful coroutine, Win32 fibers, Ruby Fiber, PHP Fiber, Boost.Fiber, ucontext]
area: concurrency-and-parallelism/processes-and-threads
level: advanced
summary: A fiber is a thread of execution with its own stack that the program schedules itself. A fiber runs until it switches to another fiber, so fibers on one thread never run at the same time.
applies_to: core-languages
viz: optional
history:
  year: 1996
  note: According to P1364, service pack 5 of Windows NT 3.51 added fibers to the Win32 API in 1996, and the term dates from that release.
see_also: [green-threads, virtual-threads, threads, iterators, async-await, thread-local-storage]
sources:
  - url: https://learn.microsoft.com/en-us/windows/win32/procthread/fibers
    title: Microsoft Learn, Win32 apps, Fibers
    locator: whole page; Fiber Local Storage
  - url: https://docs.ruby-lang.org/en/master/Fiber.html
    title: Ruby documentation, class Fiber
    locator: introduction; Non-blocking Fibers
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4024.pdf
    title: "N4024: Distinguishing coroutines and fibers (Goodspeed and Kowalke, 2014)"
    locator: Fibers; Coroutines; Relationship
  - url: https://www.php.net/manual/en/language.fibers.php
    title: PHP Manual, Fibers
    locator: introduction
  - url: https://www.boost.org/doc/libs/release/libs/fiber/doc/html/fiber/overview.html
    title: Boost.Fiber documentation, Overview
    locator: Overview
  - url: https://man7.org/linux/man-pages/man3/makecontext.3.html
    title: Linux manual page, makecontext(3)
    locator: DESCRIPTION; HISTORY
  - url: https://peps.python.org/pep-0380/
    title: "PEP 380: Syntax for Delegating to a Subgenerator"
    locator: Abstract
  - url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/yield*
    title: MDN Web Docs, yield*
    locator: introduction
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/iterators
    title: Microsoft Learn, C# iterators
    locator: Produce elements with yield return; Understand deferred element production
  - url: https://openjdk.org/jeps/444
    title: "JEP 444: Virtual Threads"
    locator: Description
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Why goroutines instead of threads?
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/SynchronousQueue.html
    title: Java SE 21 API, java.util.concurrent.SynchronousQueue
    locator: class description
  - url: https://doc.rust-lang.org/std/sync/mpsc/index.html
    title: Rust standard library, module std::sync::mpsc
    locator: module documentation
  - url: https://www.php.net/releases/8.1/en.php
    title: PHP 8.1 release announcement
    locator: Fibers
  - url: https://en.wikipedia.org/wiki/Fiber_(computer_science)
    title: Wikipedia, Fiber (computer science)
    locator: Advantages and disadvantages; Operating system support
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1364r0.pdf
    title: "P1364R0: Fibers under the magnifying glass (Nishanov, 2018)"
    locator: Introduction; 2.1 Memory Footprint; 2.2 Context Switching overhead; 2.3 Compatibility & Scalability; 3.1 Fiber use on Windows
---

## Idea

A fiber is a thread of execution that the program schedules itself. The Win32 documentation defines it as "a unit of execution that must be manually scheduled by the application".[^1] Each fiber has its own stack, like a thread. Unlike a thread, a fiber is never preempted: it runs until it switches to another fiber.[^1] The Ruby documentation says that fibers "are never preempted and that the scheduling must be done by the programmer and not the VM".[^2]

A fiber runs inside an OS thread, and one thread can run many fibers, one at a time.[^1] So fibers add one more level to the usual list: a computer runs processes, a process runs threads, and a thread runs fibers.[^3]

Because a fiber has its own stack, it can pause anywhere in a chain of function calls and continue later at the same point. Ruby says that each fiber "comes with a stack", which lets the fiber pause "from deeply nested function calls".[^2] PHP calls its fibers "full-stack, interruptible functions".[^4] The example files use this property. A recursive function walks a tree and pauses at each leaf, deep in the recursion, and gives the leaf to the caller.

## How it works

### Switching between fibers

A fiber stores only a small amount of state: its stack, some registers, and the data that it got when it was created.[^1] To switch fibers, the running fiber saves its registers and its stack pointer, and loads those of the next fiber.[^5] The kernel takes no part in the switch, so the switch is faster than a thread switch.[^3] The Boost.Fiber documentation compares the costs: a thread switch "usually costs thousands of CPU cycles on x86", and a fiber switch costs "less than a hundred cycles".[^5]

On Windows, the API has four main calls:[^1]

- `ConvertThreadToFiber` makes the current thread into a fiber, so that it can switch to other fibers.
- `CreateFiber` makes a new fiber with a stack size, a start function and a data value.
- `SwitchToFiber` saves the current fiber and runs another one.
- `DeleteFiber` frees the stack and the other data of a fiber.

On Linux, the C library has the same tools in `<ucontext.h>`. `makecontext` prepares a context that runs a function on a stack that the caller allocates, and `swapcontext` "saves the current context" and activates another one.[^6] The C++ example uses these functions.

### Fibers and their thread

For the operating system, a fiber does not exist. The work that a fiber does belongs to the thread that runs it. For example, a fiber that reads thread-local storage reads the storage "of the thread that is running it".[^1] When the OS preempts the thread, it also stops the fiber that the thread is running, and that fiber continues when the thread runs again.[^1] For values that belong to one fiber, Windows has fiber-local storage, with the calls `FlsAlloc`, `FlsGetValue` and `FlsSetValue`.[^1]

### Who chooses the next fiber

There are two styles:

- **Direct switching.** The running fiber names the fiber that runs next. Win32 `SwitchToFiber`, Ruby `Fiber#resume` and `Fiber.yield`, and PHP `Fiber::resume` and `Fiber::suspend` work like this.[^1][^2][^4]
- **A scheduler.** A fiber that waits gives control to a scheduler, and the scheduler picks a fiber that is ready. The N4024 paper describes this model: "a fiber blocks" and the scheduler dispatches another fiber, as an OS does with threads.[^3] Boost.Fiber works like this, and so do Ruby fibers with a scheduler: a non-blocking fiber that reaches an operation that would block gives control to other fibers.[^5][^2]

## In languages

Few of the tier 1 languages have fibers. Most of them have a stackless alternative, such as generators or iterators, where only one function can pause. The example files walk the tree `[[1, 2], [3, [4]]]` and print its leaves. Then they compare the leaves of two trees in step, which is the classic "same fringe" problem.

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | generators with `yield from`, which are stackless |
| JavaScript | absent | generator functions with `yield*`, which are stackless |
| TypeScript | absent | as JavaScript |
| Java | absent | a virtual thread and a `SynchronousQueue` |
| Kotlin | absent | the Java alternatives on the JVM |
| C# | absent | iterators with `yield return`, which are stackless |
| Go | absent | a goroutine and an unbuffered channel |
| Rust | absent | an OS thread and a rendezvous channel, `sync_channel(0)` |
| C | library | `<ucontext.h>` in glibc, or the Win32 fiber API |
| C++ | library | `<ucontext.h>`, Boost.Fiber, or the Win32 fiber API |
| Bash | absent | no fibers |
| PowerShell | absent | no fibers |
| SQL | n/a | a query does not run threads of execution |
| Regex | n/a | a pattern has no threads of execution |
| HTML/CSS | n/a | markup and style rules have no threads of execution |

- **Python, JavaScript and C#.** A generator or an iterator can pause, but only in its own body. To pause inside a recursion, each level must be a generator. PEP 380 added `yield from` so that a generator can "delegate part of its operations to another generator".[^7] JavaScript has `yield*` for the same purpose.[^8] C# has no such operator, so a recursive iterator loops over the inner iterator and gives each value again with `yield return`.[^9]
- **Java and Go.** A virtual thread or a goroutine has its own stack, so it can pause at any depth. But a scheduler in the runtime, not the program, decides when it runs.[^10][^11] A `SynchronousQueue` or an unbuffered channel makes the two sides take turns: in a `SynchronousQueue`, "each insert operation must wait for a corresponding remove operation by another thread".[^12]
- **Rust.** A channel with bound 0 is a "rendezvous" channel, where each sender hands off a message directly to a receiver.[^13] With an OS thread on the sending side, this gives the same behavior as a fiber, but each walk uses a full thread.
- **C and C++.** The `<ucontext.h>` functions give real fibers. Boost.Fiber adds a scheduler, and fiber versions of mutexes, condition variables and futures.[^3][^5]
- **Ruby and PHP.** Outside the tier 1 languages, Ruby has the class `Fiber`,[^2] and PHP added the class `Fiber` in PHP 8.1.[^14]

## Variations and related ideas

- **Fibers and coroutines.** The words overlap. Wikipedia says that fibers are "sometimes called stackful coroutines".[^15] N4024 makes a different distinction. In that paper, a coroutine passes control directly to its caller, and a fiber has a scheduler and the synchronization tools of a thread.[^3] By that definition, Ruby and PHP fibers are closer to coroutines: `Fiber.yield` in Ruby gives control back to the caller of `Fiber#resume`.[^2]
- **Stackful and stackless.** A stackless coroutine is a state machine that the compiler makes. It keeps only the local variables that are live across a pause, and it runs on the stack of the thread that resumes it.[^16] A fiber can pause in any function that it calls, but it needs a stack large enough for all of them.[^16]
- **[[green-threads]] and [[virtual-threads]].** These are user-mode threads with a scheduler in the language runtime. A virtual thread can block in ordinary code, and the JDK runs another virtual thread on its carrier.[^10] Fibers in the strict sense have no scheduler unless a library adds one.
- **[[async-await]].** An `async` function is a stackless coroutine. The points where a stackless coroutine can pause are all "statically known to the compiler".[^16]
- **Continuations.** Boost.Fiber is built on call/cc, call with current continuation, from the Boost.Context library.[^5]

## Pitfalls

- **Blocking calls.** A fiber that calls a function that blocks its thread also stops all other fibers on that thread.[^3] Libraries that use fibers must use asynchronous I/O, or fiber-aware versions of blocking calls.[^16]
- **Long computations.** No one preempts a fiber. A long CPU-bound loop must switch to other fibers from time to time, or the other fibers on its thread do not run.[^3]
- **Thread-local storage.** Code on a fiber sees the thread-local storage of the current thread.[^1] If a fiber moves to a different thread, a cached address of a thread-local variable can point to the storage of the old thread. P1364 shows how this corrupts memory or reads freed memory.[^16]
- **Stack size.** A fiber gets a fixed stack when it is created, and it must be large enough for every function that the fiber calls. A stack that is too small can corrupt memory.[^16]
- **Deleting a running fiber.** If a fiber in one thread deletes the selected fiber of another thread, that other thread "is likely to terminate abnormally" because its stack is gone.[^1] On Windows, if the fiber function returns, the thread that runs it exits.[^1]

## Cost

- **Memory.** P1364 estimates that a fiber needs nearly as much memory as a thread: both have a user stack, typically 1 MiB of virtual address space. A fiber needs no kernel context and no kernel stack, which saves about 1%.[^16]
- **Switch.** A fiber switch does not enter the kernel, so it is cheaper than a thread switch.[^3] It is still more expensive than a function call or a stackless resume. P1364 measures a fiber switch on x86-64 System V at 23 instructions that move 64 bytes.[^16]
- **Parallelism.** The fibers of one thread never run at the same time, so they cannot use more than one core.[^3][^15] To use many cores, a program runs fibers on several threads.

## History

The first idea is the coroutine, which P1364 dates to 1958.[^16] According to P1364, service pack 5 of Windows NT 3.51 added fibers to the Win32 API in 1996, at the request of the SQL team and others.[^16] SQL Server 7.0 added an optional fiber mode in 1998, and later advice was not to turn it on, because fiber mode rarely made the server faster and some components failed with it.[^16] In 2014, N4024 described a planned proposal to add fibers to the C++ standard library, with Boost.Fiber as the reference implementation.[^3] In 2018, P1364 recommended threads, or asynchronous code with state machines, in place of fibers.[^16] PHP 8.1 added fibers as a core feature.[^14] Ruby 3.0 added non-blocking fibers, which work with a scheduler.[^2]

## References
