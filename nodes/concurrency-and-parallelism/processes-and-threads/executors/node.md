---
tag: EHE1JZ
kind: concept
slug: executors
name: Executors
aliases: [executor, Executor, ExecutorService, TaskScheduler, task scheduler, async executor, block_on, launch policy]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: An executor is an object that runs tasks. Code submits a task to it, and the executor decides when, where and on which thread the task runs.
applies_to: core-languages
viz: optional
history:
  year: 2004
  note: J2SE 5.0, released in September 2004, added the concurrency utilities in java.util.concurrent, which include the Executor interface.
see_also: [run-tasks-on-a-thread-pool, threads, futures, async-await, virtual-threads, work-stealing]
sources:
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Executor.html
    title: Java SE 21 API, java.util.concurrent.Executor
    locator: interface description; execute
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ExecutorService.html
    title: Java SE 21 API, java.util.concurrent.ExecutorService
    locator: interface description; close
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: Python documentation, concurrent.futures
    locator: Executor Objects; Future Objects; ThreadPoolExecutor
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Executors.html
    title: Java SE 21 API, java.util.concurrent.Executors
    locator: Method Summary
  - url: https://tokio.rs/blog/2019-10-scheduler
    title: "Tokio blog, Making the Tokio scheduler 10x faster (2019)"
    locator: "One queue, many processors; Many processors, each with their own run queue"
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.taskscheduler
    title: Microsoft Learn, TaskScheduler class
    locator: Remarks; The default task scheduler and the thread pool; Task inlining
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ThreadPoolExecutor.html
    title: Java SE 21 API, java.util.concurrent.ThreadPoolExecutor
    locator: Queuing; Rejected tasks
  - url: https://rust-lang.github.io/async-book/02_execution/04_executor.html
    title: Asynchronous Programming in Rust, Build an Executor
    locator: introduction
  - url: https://rust-lang.github.io/async-book/08_ecosystem/00_chapter.html
    title: Asynchronous Programming in Rust, The Async Ecosystem
    locator: Async Runtimes
  - url: https://docs.python.org/3/library/asyncio-eventloop.html
    title: Python documentation, asyncio Event Loop
    locator: loop.run_in_executor
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.concurrentexclusiveschedulerpair
    title: Microsoft Learn, ConcurrentExclusiveSchedulerPair class
    locator: class description
  - url: https://developer.mozilla.org/en-US/docs/Web/API/HTML_DOM_API/Microtask_guide
    title: MDN Web Docs, Using microtasks in JavaScript with queueMicrotask
    locator: Tasks vs. microtasks
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Why goroutines instead of threads?
  - url: https://gobyexample.com/worker-pools
    title: Go by Example, Worker Pools
    locator: whole page
  - url: https://en.cppreference.com/w/cpp/thread/async
    title: cppreference, std::async
    locator: Async invocation; Deferred invocation; Policy selection
  - url: https://en.wikipedia.org/wiki/Java_version_history
    title: Wikipedia, Java version history
    locator: J2SE 5.0
---

## Idea

An executor is an object that runs tasks. Code that has work to do gives a task to the executor, and the executor decides how the task runs: now or later, on the current thread or on another thread, alone or at the same time as other tasks. The Java documentation describes its `Executor` interface as "a way of decoupling task submission from the mechanics of how each task will be run, including details of thread use, scheduling, etc."[^1]

This split has two benefits. The code that submits tasks stays the same when the policy changes, so a program can move from one thread per task to a pool of threads by changing one line. And the policy is in one place, where it is easy to limit, measure and shut down.[^1][^2] The example files show this. One function submits the same tasks to different executors, and each executor runs them in a different way.

## How it works

### The interface

The smallest executor interface has one operation: run this task. In Java, `Executor` has one method, `execute(Runnable)`, which "Executes the given command at some time in the future".[^1] Most executor interfaces add two things:

- **A result.** `submit` returns a future, an object that holds the result of the task when it is ready. Java `ExecutorService.submit` returns a `Future` that "can be used to cancel execution and/or wait for completion".[^2] Python `Executor.submit` returns a `Future` too.[^3] If the task raises an exception, the future keeps it, and `result` raises "the same exception" when the caller asks for the value.[^3]
- **A lifecycle.** An executor that owns threads must stop them. An `ExecutorService` "can be shut down, which will cause it to reject new tasks", and "An unused ExecutorService should be shut down to allow reclamation of its resources".[^2]

### Policies

The executor decides how each task runs. The Java documentation gives three examples:[^1]

- **Direct.** The executor runs the task "immediately in the caller's thread".[^1] This executor adds no concurrency.
- **A thread for each task.** The executor "spawns a new thread for each task".[^1] `Executors.newThreadPerTaskExecutor` gives this policy, and `newVirtualThreadPerTaskExecutor` gives the same policy with virtual threads.[^4]
- **Serial.** A composite executor passes tasks to another executor one at a time, so they never overlap.[^1]

The most common policy is a pool: a fixed set of worker threads takes tasks from a queue. `Executors.newFixedThreadPool` "reuses a fixed number of threads operating off a shared unbounded queue".[^4] A pool limits the number of threads, so it limits the parallel work and the memory for stacks. See [[run-tasks-on-a-thread-pool]].

### Queues and scheduling

Inside most executors, there is a queue of tasks that wait and one or more workers that take tasks from it. Tokio describes a scheduler in the same terms: a "run queue and a processor that drains the queue".[^5] The design choices are in the queue:

- **One queue or many.** With one shared queue, all workers take from the same place, and the queue must support many producers and many consumers at the same time.[^5] With a queue for each worker, workers contend less, but a worker with no work must take tasks from the queue of another worker. This is [[work-stealing]]. The .NET default scheduler uses a global FIFO queue and a local queue for each thread of the pool, and "provides work-stealing for load-balancing".[^6]
- **Bounded or unbounded.** An unbounded queue accepts every task, but it can grow without limit "when commands continue to arrive on average faster than they can be processed".[^7] A bounded queue must say what happens to a task when it is full. Java calls this a rejection policy. The default throws `RejectedExecutionException`, and `CallerRunsPolicy` runs the task "directly in the calling thread".[^7]

### Executors for async code

Async code needs an executor too, because a coroutine or a future does not run by itself. In Rust, futures are "lazy: they won’t do anything unless actively driven to completion".[^8] An async executor takes futures and runs them "by calling poll whenever the Future can make progress".[^8] When a future cannot continue, it saves a waker. When the event that it waits for happens, the waker puts the future back in the queue, and the executor polls it again.[^8] The Rust example builds a minimal executor of this kind.

Rust has no async executor in its standard library: executors "are not yet provided in the standard library", and libraries such as Tokio supply them.[^9] An async runtime usually combines one or more executors with a reactor, which gives notice of events such as I/O and timers.[^9]

## In languages

All the tier 1 languages that have threads can use an executor. The difference is whether the standard library defines an executor interface, or whether the runtime has one fixed executor and no interface.

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `concurrent.futures.Executor`, with `ThreadPoolExecutor` and `ProcessPoolExecutor` |
| JavaScript | absent | the event loop and its task and microtask queues |
| TypeScript | absent | as JavaScript |
| Java | stdlib | `Executor`, `ExecutorService` and the `Executors` factory methods |
| Kotlin | stdlib | the Java executors on the JVM |
| C# | stdlib | `TaskScheduler`, with the thread pool as the default |
| Go | absent | the `go` statement and the runtime scheduler |
| Rust | library | the `Future` trait in std; executors in crates such as Tokio |
| C | absent | threads from `pthread_create` or `thrd_create` |
| C++ | absent | `std::async` with a launch policy |
| Bash | absent | background jobs with `&` |
| PowerShell | absent | background jobs with `Start-Job` |
| SQL | n/a | the database runs the query |
| Regex | n/a | a pattern has no tasks |
| HTML/CSS | n/a | markup and style rules have no tasks |

- **Java.** `Executor` is the base interface. `ExecutorService` adds futures, bulk operations such as `invokeAll`, and shutdown.[^2] It extends `AutoCloseable`, so a try-with-resources block shuts the executor down and waits for its tasks.[^2]
- **Python.** `concurrent.futures` defines `Executor` as an abstract class that "should not be used directly, but through its concrete subclasses".[^3] Its thread and process executors share one interface, so a program can move CPU-bound work to processes by changing the class.[^3] In `asyncio`, `loop.run_in_executor` runs a blocking function in a `concurrent.futures.Executor` and gives the event loop a future for the result.[^10]
- **C#.** `TaskScheduler` "handles the low-level work of queuing tasks onto threads".[^6] Most code uses the default scheduler through `Task.Run` and `await`. A custom scheduler can limit concurrency or run tasks on one thread. `ConcurrentExclusiveSchedulerPair` gives two schedulers where "exclusive tasks never" run at the same time as other tasks.[^11]
- **JavaScript.** There is one thread and one event loop. A program chooses the queue, not the executor. A callback from `queueMicrotask` runs "only if the JavaScript execution stack is empty", before the event loop takes the next task, such as a `setTimeout` callback.[^12]
- **Go.** The runtime multiplexes goroutines "onto a set of threads", so a `go` statement is already a submission to an executor with a scheduler.[^13] To limit the work in progress, Go programs start a fixed number of goroutines that receive jobs from a channel.[^14]
- **C++.** `std::async` takes a launch policy. With `std::launch::async`, the function runs "as if in a new thread of execution". With `std::launch::deferred`, the first wait on the future runs the function "in the thread that called the waiting function".[^15]

## Variations and related ideas

- **Thread pool.** A thread pool is the most common executor. [[run-tasks-on-a-thread-pool]] shows one in each language.
- **[[work-stealing]].** A scheduler design with a queue for each worker. An idle worker takes tasks from the queues of busy workers.[^6][^5]
- **Scheduled executors.** Some executors run tasks after a delay or at a fixed rate. Java has `newScheduledThreadPool` in `Executors`.[^4]
- **[[futures]].** The object that `submit` returns. An executor runs the task; the future gives the result to the caller.[^2][^3]
- **[[virtual-threads]].** Java can run each task on its own virtual thread with `newVirtualThreadPerTaskExecutor`.[^4]
- **[[async-await]].** Async functions need an executor to run. In JavaScript the event loop is the executor; in Rust the program chooses one.[^8][^12]

## Pitfalls

- **Tasks that wait for other tasks.** If a task waits for the result of another task on the same executor, and all workers are busy with tasks that wait, no task can continue. Python warns: "Deadlocks can occur when the callable associated with a Future waits on the results of another Future".[^3] The [[run-tasks-on-a-thread-pool]] page shows this with a single worker.
- **Lost exceptions.** An exception in a task goes into its future. If no code asks the future for its result, no one sees the exception.[^3]
- **No shutdown.** An executor that is not shut down keeps its threads and other resources.[^2] Use the lifecycle tools of the language, such as `with` in Python or try-with-resources in Java.
- **Unbounded queues.** A fixed pool with an unbounded queue accepts tasks faster than it runs them, and the queue grows.[^7] Use a bounded queue with a rejection policy when tasks can arrive faster than workers finish them.
- **Thread affinity.** An executor can run a task on any of its threads. Code that must run on one thread, such as UI code, needs a scheduler for that thread. In .NET, `TaskScheduler.FromCurrentSynchronizationContext` gives one, because UI access is often "restricted to code that is running on the same thread on which the UI object was created".[^6]
- **Blocking in async code.** A blocking call in a task of an async executor also stops the other tasks of that worker. In Python, `run_in_executor` runs such calls in a thread or process executor instead.[^10]

## Cost

- **Submission.** A submission puts a task in a queue, and a worker takes it later. A shared queue has contention when many threads use it at the same time; a queue for each worker reduces this.[^5][^6]
- **Threads.** A pool keeps its threads after a task ends, so later tasks do not pay to create a thread.[^4] A thread for each task pays that cost each time, unless the threads are virtual threads.
- **Direct execution.** A direct executor costs only a function call, and it adds no parallelism.[^1]

## History

J2SE 5.0 came out in September 2004 with the concurrency utilities in `java.util.concurrent`.[^16] The `Executor` interface is part of that package, and its documentation lists 1.5 as its first version.[^1] Rust took a different path: its standard library defines futures but leaves executors to libraries such as Tokio.[^9] In 2019 the Tokio project rewrote its scheduler and described the choices: one shared run queue, or a run queue for each processor with work stealing, which Go, Erlang and Java also use.[^5]

## References
