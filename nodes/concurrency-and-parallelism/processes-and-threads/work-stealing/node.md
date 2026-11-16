---
tag: G7XJGG
kind: concept
slug: work-stealing
name: Work stealing
aliases: [work stealing, work-stealing scheduler, work-stealing deque, task stealing, ForkJoinPool, child stealing, continuation stealing]
area: concurrency-and-parallelism/processes-and-threads
level: advanced
summary: Work stealing is a way to share tasks among worker threads. Each worker keeps its own deque of tasks, and a worker with no tasks takes the oldest task from the deque of another worker.
applies_to: core-languages
viz: optional
history:
  year: 1999
  note: Blumofe and Leiserson published their analysis of randomized work stealing in the Journal of the ACM in 1999.
see_also: [executors, run-tasks-on-a-thread-pool, threads, green-threads, async-await]
sources:
  - url: https://en.wikipedia.org/wiki/Work_stealing
    title: Wikipedia, Work stealing
    locator: introduction; Algorithm; Child stealing vs. continuation stealing; Efficiency; Space usage; Multiprogramming variant
  - url: https://gee.cs.oswego.edu/dl/papers/fj.pdf
    title: "A Java Fork/Join Framework (Doug Lea)"
    locator: 2.1 Work-Stealing; 3.1 Deques
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.taskscheduler
    title: Microsoft Learn, TaskScheduler class
    locator: The global queue vs. local queues; Work stealing
  - url: https://docs.rs/rayon/latest/rayon/fn.join.html
    title: Rayon documentation, function rayon::join
    locator: description
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ForkJoinPool.html
    title: Java SE 21 API, java.util.concurrent.ForkJoinPool
    locator: class description; ForkJoinPool constructor, asyncMode
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Executors.html
    title: Java SE 21 API, java.util.concurrent.Executors
    locator: newWorkStealingPool
  - url: https://raw.githubusercontent.com/golang/go/master/src/runtime/proc.go
    title: Go source, src/runtime/proc.go
    locator: Goroutine scheduler comment; stealWork; runqsteal
  - url: https://tokio.rs/blog/2019-10-scheduler
    title: "Tokio blog, Making the Tokio scheduler 10x faster (2019)"
    locator: Work-stealing scheduler; Throttle stealing
---

## Idea

Work stealing is a way to share tasks among a fixed set of worker threads. Each worker has its own queue of tasks. A worker adds the tasks that it creates to its own queue and runs them from there. In the words of Wikipedia, when a processor "runs out of work, it looks at the queues of the other processors" and steals their work items.[^1]

The main benefit is that busy workers do not coordinate at all. "as long as all processors have work to do, no scheduling overhead occurs".[^1] Coordination happens only when a worker is idle, and then the idle worker pays for it. Compare a pool with one shared queue, where every worker takes every task from the same place.

Work stealing suits work that splits itself: a task that creates smaller tasks, which create smaller tasks again. Divide-and-conquer algorithms, such as a parallel sum or a parallel sort, work like this. The example files show the rules on a small case. Two workers split the range `[0, 8)` into pieces and add the numbers. Worker w1 starts with nothing and steals its first task.

## How it works

### A deque for each worker

Each worker keeps its tasks in a double-ended queue, a deque, and uses the two ends for different things. The scheme of the Java fork/join framework, which adapts the scheme of Cilk, is typical:[^2]

- A worker pushes the tasks that it creates onto the bottom of its own deque.
- A worker takes its next task from the bottom of its own deque, in LIFO order: the newest task first.
- An idle worker picks another worker at random and takes a task from the top of that worker's deque, in FIFO order: the oldest task first.[^2]
- A worker that finds nothing to steal backs off and tries again later. When all workers are idle, they block until new work arrives.[^2]

### Why the two ends

The owner and the thief use opposite ends, and this has two effects:[^2]

- **Less contention.** Lea's paper says that this "reduces contention by having stealers operate on the opposite side of the deque as owners".[^2] Most of the time, only the owner touches its deque, and the owner's push and pop can avoid locks. The owner and a thief can only interfere "if the deque is about to become empty", and only that case needs a lock.[^2]
- **Large steals.** In a recursive split, the oldest task in a deque is the one that was split first, so it is the largest. "an older stolen task is likely to provide a larger unit of work", so one steal gives the thief a lot of work.[^2] In the example, w1 steals `[0, 4)`, half of the whole range, in its first step.

The owner's LIFO order also helps the cache: the newest task usually uses the data that the worker has just touched. The .NET thread pool takes local tasks in LIFO order "to preserve cache locality and reduce contention".[^3]

### Joining

A task often waits for the subtasks that it forked. A worker that waits by blocking would waste its thread. So in the Java fork/join framework, a worker that reaches a join "processes other tasks, if available, until the target task is noticed to have completed".[^2] Rayon's `join` works the same way. The current thread runs the first closure and makes the second available to other threads. If a thief has taken the second closure, the current thread looks for other work while it waits.[^4]

### Child stealing and continuation stealing

When a task forks a child, one of the two must wait in the deque, where a thief can take it:[^1]

- **Child stealing.** The parent continues, and the child waits in the deque. This is "easier to implement as a library, without compiler support". Threading Building Blocks, the .NET Task Parallel Library and OpenMP use it.[^1]
- **Continuation stealing.** The worker runs the child at once, as a function call, and the rest of the parent waits in the deque. Cilk Plus uses it.[^1]

The difference matters for memory. With continuation stealing, the stack space is bounded by the space of a one-processor run times the number of processors. With child stealing there is no such bound: a loop that forks `n` children can fill a deque with `n` tasks.[^1]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | absent | no work-stealing scheduler in the standard library |
| JavaScript | absent | one thread for each event loop |
| TypeScript | absent | as JavaScript |
| Java | stdlib | `ForkJoinPool`, `Executors.newWorkStealingPool` |
| Kotlin | stdlib | the Java `ForkJoinPool` on the JVM |
| C# | stdlib | the .NET thread pool, with a local queue for each thread |
| Go | native | the runtime scheduler steals goroutines between Ps |
| Rust | library | Rayon, and the multi-threaded Tokio runtime |
| C | library | OpenMP tasks |
| C++ | library | Threading Building Blocks |
| Bash | absent | no scheduler for tasks |
| PowerShell | absent | no work-stealing scheduler |
| SQL | n/a | the database schedules its own work |
| Regex | n/a | a pattern has no tasks |
| HTML/CSS | n/a | markup and style rules have no tasks |

- **Java.** `ForkJoinPool` "differs from other kinds of ExecutorService mainly by virtue of employing work-stealing".[^5] A task calls `fork` to push a subtask and `join` to wait for it. `Executors.newWorkStealingPool` creates a work-stealing pool with one target thread for each processor.[^6] The Java example runs a model of the rules, and then a real sum on a `ForkJoinPool`.
- **C#.** The .NET thread pool has one global FIFO queue and a local queue for each thread. A task that another task creates goes to the local queue of its thread.[^3] An idle thread looks in its own local queue, then in the global queue, and "then in the local queues of other threads".[^3]
- **Go.** Each P, a processor in the Go runtime, has a local run queue of goroutines. `stealWork` "attempts to steal a runnable goroutine or timer from any P", and `runqsteal` takes half of the goroutines of the other P.[^7]
- **Rust.** Rayon uses "a fixed pool of worker threads" and work stealing to run the two closures of `join` in parallel only when CPUs are idle.[^4] Tokio's multi-threaded scheduler also steals tasks.[^8]
- **Other languages.** The model in the example files works in any language. A real scheduler needs threads and a deque that is safe for an owner and thieves at the same time, so most programs use a library.

## Variations and related ideas

- **Work sharing.** The opposite approach: a scheduler gives each new task to a processor when the task is created. Work stealing moves tasks between processors less, because no task moves while all processors are busy.[^1]
- **A shared queue.** A pool where all workers take from one queue, as in [[run-tasks-on-a-thread-pool]]. It is simpler, but all workers contend for the one queue.[^8]
- **Steal half.** Go and Tokio take half of the tasks of another worker in one steal, not one task.[^7][^8] This spreads a burst of new tasks over the idle workers quickly.
- **FIFO mode.** `ForkJoinPool` has an `asyncMode` for "event-style tasks that are never joined". In this mode, each worker takes its own tasks in FIFO order.[^5]
- **[[executors]].** A work-stealing pool is one kind of executor. Code submits tasks to it as to any other executor.[^5]
- **[[green-threads]].** The Go runtime uses work stealing to share goroutines among its worker threads.[^7]

## Pitfalls

- **Tasks that are too small.** Each fork has a cost, like a procedure call.[^2] If the leaves of a split are tiny, this cost is larger than the work. Split until a piece is large enough, then run it as a plain loop, as the Java example does with pieces of 10,000 numbers.
- **Tasks that are too large.** A task that never splits cannot be shared. If one worker gets all the work in one task, the other workers have nothing to steal.
- **Blocking.** A worker that blocks on I/O or a lock does no work, and it cannot run the tasks in its deque. `ForkJoinPool` tries to add threads when tasks wait to join, but "no such adjustments are guaranteed in the face of blocked I/O or other unmanaged synchronization".[^5]
- **Many idle thieves.** When a batch of work ends, many workers can become idle at the same time and all try to steal. Tokio limits the workers that search for work at the same time to about half of the processors.[^8]
- **Order.** Tasks do not run in the order of submission. Code that depends on the order must enforce it in another way.

## Cost

- **Time.** The randomized algorithm of Blumofe and Leiserson runs a computation in expected time `T1/P + O(T∞)` on `P` processors. `T1` is the time of a serial run, and `T∞` is the time with unlimited processors.[^1] So the expected time is within a constant factor of the best possible.[^1]
- **Space.** With continuation stealing, the stack space is at most `S1 × P`, where `S1` is the stack space of a serial run.[^1] Child stealing has no such bound.[^1]
- **Operations.** A push or a pop by the owner is cheap, because it does not contend with other workers in the common case.[^2] A steal is more expensive, because it must synchronize with the owner and other thieves. This is acceptable because "relatively few tasks are stolen in most fork/join programs".[^2]

## History

According to Wikipedia, the idea goes back to the Multilisp language and to work on parallel functional languages in the 1980s.[^1] Blumofe and Leiserson published an analysis of randomized work stealing in 1999, and the Cilk language uses work stealing in its scheduler.[^1] Doug Lea described a Java fork/join framework that adapts the tactics of the Cilk scheduler.[^2] `ForkJoinPool` in the Java standard library is a work-stealing pool of the same kind.[^5] .NET Framework 4 added work stealing to the thread pool.[^3] In 2019, Tokio described the rewrite of its scheduler as a work-stealing scheduler, the design that Go, Erlang and Java also use.[^8]

## References
