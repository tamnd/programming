---
tag: R4Y8QZ
kind: concept
slug: virtual-threads
name: Virtual threads
aliases: [Java virtual threads, Project Loom threads, JEP 444, carrier thread, pinned virtual thread, Thread.ofVirtual, newVirtualThreadPerTaskExecutor, user-mode threads]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: A virtual thread is a Java thread that the JDK schedules onto a small pool of OS threads. When it blocks, it gives its OS thread to another virtual thread, so a program can run millions.
applies_to: core-languages
viz: optional
history:
  year: 2023
  note: JEP 444, created in 2023, made virtual threads final in JDK 21 after previews in JDK 19 and JDK 20. JEP 491 in JDK 24 stopped synchronized code from pinning a virtual thread to its carrier.
see_also: [green-threads, threads, start-a-thread, thread-pool, futures, process]
sources:
  - url: https://openjdk.org/jeps/444
    title: "JEP 444: Virtual Threads"
    locator: Summary; History; Goals; Motivation; Description; Do not pool virtual threads
  - url: https://docs.oracle.com/en/java/javase/21/core/virtual-threads.html
    title: Java SE 21 Core Libraries, Virtual Threads
    locator: What is a Platform Thread?; What is a Virtual Thread?
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
    title: Java SE 21 API, java.lang.Thread
    locator: Virtual threads
  - url: https://openjdk.org/jeps/491
    title: "JEP 491: Synchronize Virtual Threads without Pinning"
    locator: Summary; Description
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Why goroutines instead of threads?
  - url: https://en.wikipedia.org/wiki/Green_thread
    title: Wikipedia, Green thread
    locator: Differences to virtual threads in the Java Virtual Machine
---

## Idea

A virtual thread is a Java thread that the JDK implements, not the operating system. JEP 444 calls virtual threads "a lightweight implementation of threads that is provided by the JDK rather than the OS".[^1] The classic Java thread, which is now called a platform thread, is "a thin wrapper around an operating system (OS) thread", and it keeps that OS thread for its whole life.[^2] A virtual thread runs on an OS thread only while it has work to do. When it blocks, for example on I/O, the JDK suspends it, and the OS thread is free to run other virtual threads.[^2]

Both kinds are instances of `java.lang.Thread`, so code that uses threads works with both. In the example below, the same method runs on a platform thread and on a virtual thread.

```java run id=idea
public class Main {
    public static void main(String[] args) throws InterruptedException {
        Thread platform = Thread.ofPlatform().name("platform-1").start(Main::describe);
        platform.join();
        Thread virtual = Thread.ofVirtual().name("virtual-1").start(Main::describe);
        virtual.join();
    }

    static void describe() {
        Thread current = Thread.currentThread();
        System.out.println(current.getName() + ": isVirtual=" + current.isVirtual()
            + ", isDaemon=" + current.isDaemon());
    }
}
```

```output for=idea
platform-1: isVirtual=false, isDaemon=false
virtual-1: isVirtual=true, isDaemon=true
```

The builder `Thread.ofVirtual()` makes a virtual thread, and `join` waits for it as for any thread. A virtual thread is always a daemon thread, so it does not keep the JVM running.[^3] The purpose of virtual threads is scale. A program can have "a great many active virtual threads, even millions, running in the same Java process".[^2]

## How it works

### Carriers, mounting and unmounting

The JDK has its own scheduler for virtual threads. It is a work-stealing `ForkJoinPool` that runs in FIFO mode, and by default it has as many platform threads as there are available processors.[^1] To run a virtual thread, the scheduler mounts it on one of these platform threads, which becomes its carrier. When the virtual thread blocks, it unmounts, and the carrier is free to run another virtual thread. When the blocking operation is ready, the virtual thread goes back to the scheduler, which mounts it again, maybe on a different carrier.[^1]

```text
  virtual threads:  V1   V2   V3   V4   ... (many)
                     \    |    |    /
                      scheduler (ForkJoinPool)
                     /         \
  carriers:      platform-A   platform-B   (about one per processor)
                     |             |
  OS threads:      OS-1          OS-2
```

This is the M:N model of [[green-threads]]: many virtual threads on few OS threads.[^1] The Oracle guide compares it to virtual memory, which maps a large address space to a limited amount of RAM.[^2] From the point of view of Java code, the carrier is invisible: `Thread.currentThread()` always gives the virtual thread itself.[^1]

### Blocking calls

The JDK changes its blocking operations so that they unmount the virtual thread instead of blocking the carrier. A virtual thread will typically unmount "when it blocks on I/O or some other blocking operation in the JDK".[^1] So code can use the plain blocking style. Each request can have its own thread, as in the thread-per-request style, which JEP 444 calls "easy to understand, easy to program".[^1]

### Pinning

Sometimes a virtual thread cannot unmount. It is then pinned to its carrier, and a blocking operation blocks the carrier and its OS thread too.[^1] In JDK 21, a virtual thread was pinned in two cases: in a `synchronized` block or method, and in a native method or a foreign function.[^1] JEP 491 in JDK 24 removed the first case for nearly all code. It lets a virtual thread that blocks in `synchronized` code release its platform thread.[^4] A virtual thread is still pinned when it calls native code that calls back into Java code that blocks.[^4] Pinning does not make a program wrong, but it "might hinder its scalability".[^1]

## In languages

Virtual threads are a feature of the Java platform. Other languages solve the same problem, many tasks that spend most of their time waiting, with goroutines, `async` tasks or a pool of OS threads. The example files run 1,000 tasks that each wait 10 ms, and allow at most 10 of them to run at the same time. The table gives the verdict for each tier 1 language.

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | `asyncio` tasks instead |
| JavaScript | absent | `async` functions on the event loop instead |
| TypeScript | absent | as JavaScript |
| Java | stdlib | `Thread.ofVirtual()`, `Executors.newVirtualThreadPerTaskExecutor()` (Java 21) |
| Kotlin | stdlib | the Java virtual threads on the JVM |
| C# | absent | `async` methods and tasks instead |
| Go | native | goroutines are M:N threads of the Go runtime |
| Rust | absent | a fixed set of OS threads, or `async` tasks with a library executor |
| C | absent | a pool of OS threads |
| C++ | absent | a pool of OS threads |
| Bash | absent | background processes |
| PowerShell | absent | thread jobs on OS threads |
| SQL | n/a | a query does not start threads |
| Regex | n/a | a pattern has no threads |
| HTML/CSS | n/a | markup and style rules have no threads |

- **Java.** `Thread.ofVirtual()` returns a builder for virtual threads, and `Thread.startVirtualThread(Runnable)` creates and starts one.[^1] Virtual threads are "not intended for long running CPU intensive operations".[^3]
- **Go.** JEP 444 names goroutines in Go and processes in Erlang as user-mode threads that "have been successful in other multithreaded languages".[^1] The Go FAQ says that it is "practical to create hundreds of thousands of goroutines in the same address space".[^5]
- **C#, Python and JavaScript.** These languages have no virtual threads. An `async` function gives its thread back at each `await`, so many waiting tasks need few threads. Unlike a virtual thread, an `async` function must mark each point where it can wait.

## Variations and related ideas

- **[[green-threads]].** The green threads of early Java shared one OS thread, and a blocking call blocked them all. Virtual threads differ: they live next to platform threads, and "sleeping on a virtual thread does not block the underlying carrier thread".[^6]
- **[[thread-pool]].** A pool shares a few expensive platform threads among many tasks. Virtual threads are cheap, so "there is never a need to pool them".[^1] The JEP recommends a new virtual thread for each task.[^1]
- **Semaphores to limit concurrency.** Programs sometimes use a pool of fixed size to limit how many tasks use a resource at the same time. With virtual threads, the JEP recommends constructs made for this purpose, "such as semaphores".[^1] The example files use this approach.
- **[[futures]] and `async` code.** An asynchronous API returns at once and completes a future later. Virtual threads give the same scale with blocking code, so a stack trace and a debugger show one task on one thread.[^1]

## Pitfalls

- **CPU-bound work.** Virtual threads "do not run code any faster than platform threads". They give throughput when there are many tasks that wait, not lower latency for one task.[^1] A long computation keeps its carrier busy.[^3]
- **Pooling virtual threads.** A pool of virtual threads brings back the limits that virtual threads remove. Create a new virtual thread for each task.[^1]
- **Pinning on old JDKs.** On JDK 21 to 23, a virtual thread that blocks inside `synchronized` code holds its carrier.[^1][^4] Frequent, long pinning "can harm the scalability of an application by capturing carriers".[^1] The system property `jdk.tracePinnedThreads` prints a stack trace when a thread blocks while pinned.[^1]
- **Thread-local variables.** Virtual threads support them, but a single JVM can have millions of virtual threads, so a large value in each thread-local variable adds up.[^2]

## Cost

- **Memory.** A virtual thread typically has a shallow call stack and needs few resources.[^2][^3] A platform thread has a large stack and other resources that the operating system keeps.[^2]
- **Speed.** A virtual thread runs code at the same speed as a platform thread.[^1] The gain is in the number of threads, not in the speed of each.
- **Scheduling.** Mounting and unmounting happen often and do not block platform threads.[^4]

## History

Java first had threads in user mode: the green threads of early Java.[^1] Later versions changed to OS threads. Virtual threads came as a preview in JDK 19 (JEP 425) and JDK 20 (JEP 436), and JEP 444, created in 2023, made them final in JDK 21.[^1] JEP 491, delivered in JDK 24, removed pinning in `synchronized` code for nearly all cases.[^4]

## References
