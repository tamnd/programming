---
tag: H0F26W
kind: task
slug: run-tasks-on-a-thread-pool
name: Run tasks on a thread pool
aliases: [thread pool example, worker pool, ExecutorService example, newFixedThreadPool, ThreadPoolExecutor example, Go worker pool, submit tasks to a pool, Parallel.For MaxDegreeOfParallelism]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: Submit one job for each number to a pool of at most a given number of worker threads, wait until every job has finished, and return the results in the order of the numbers.
contract: contract.yaml
category: concurrency
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [thread-pool, wait-for-threads-to-finish, start-a-thread, futures, threads, data-races]
sources:
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: The Python Standard Library, concurrent.futures
    locator: ThreadPoolExecutor; ProcessPoolExecutor
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/Executors.html
    title: Java SE 21 API, java.util.concurrent.Executors
    locator: newFixedThreadPool(int)
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ExecutorService.html
    title: Java SE 21 API, java.util.concurrent.ExecutorService
    locator: shutdown(); close(); invokeAll
  - url: https://learn.microsoft.com/en-us/dotnet/standard/threading/the-managed-thread-pool
    title: Microsoft Learn, The managed thread pool
    locator: Thread pool characteristics
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.paralleloptions.maxdegreeofparallelism
    title: Microsoft Learn, ParallelOptions.MaxDegreeOfParallelism
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/standard/parallel-programming/how-to-write-a-simple-parallel-for-loop
    title: Microsoft Learn, How to write a simple Parallel.For loop
    locator: Return Value and Exception Handling
  - url: https://gobyexample.com/worker-pools
    title: Go by Example, Worker Pools
    locator: whole page
  - url: https://doc.rust-lang.org/book/ch21-02-multithreaded.html
    title: The Rust Programming Language, Turning Our Single-Threaded Server into a Multithreaded Server
    locator: Improving Throughput with a Thread Pool; Listing 21-21
  - url: https://doc.rust-lang.org/std/sync/mpsc/index.html
    title: Rust standard library, module std::sync::mpsc
    locator: Disconnection
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_mutex_lock.html
    title: POSIX.1-2024, pthread_mutex_lock
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/thread/packaged_task
    title: std::packaged_task, cppreference.com
    locator: the class description
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: new Worker(filename[, options]) example
  - url: https://docs.python.org/3/library/threading.html
    title: The Python Standard Library, threading
    locator: CPython implementation detail
  - url: https://docs.rs/rayon/latest/rayon/
    title: rayon crate documentation
    locator: crate overview
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ThreadPoolExecutor.html
    title: Java SE 21 API, java.util.concurrent.ThreadPoolExecutor
    locator: Queuing
---

## Task

Write a function `steps_on_pool(workers, numbers)` that runs one job for each number on a pool of worker threads. A job counts the steps that take its number to 1, where a step halves an even number and changes an odd number n to 3n + 1. The pool has at most `workers` threads, so at most `workers` jobs run at the same time. The function waits until every job has finished, stops the pool, and returns the counts in the order of the numbers. For example, `steps_on_pool(2, [1, 2, 3, 6])` is `[0, 1, 7, 8]`.

`workers` is 1 to 8. There are 0 to 1,000 numbers, and each number is 1 to 1,000,000. A count fits in 32 bits, but the numbers on the way to 1 can be larger than 2^31, so a job needs 64-bit arithmetic. The task is about the pool: a fixed set of threads takes jobs from a queue, many more jobs than threads go through it, and the results come back in the order of the input. [[wait-for-threads-to-finish]] starts one thread for each job instead.

## Solutions

| Language | Pool | Submit | Results | Stop |
|---|---|---|---|---|
| Python | `ThreadPoolExecutor(max_workers=workers)` | `submit` | `Future.result()` | the end of the `with` block |
| JavaScript | an array of `workers` Node.js workers | `postMessage` to a free worker | a message from the worker | `terminate()` on each worker |
| TypeScript | as JavaScript | as JavaScript | as JavaScript | as JavaScript |
| Java | `Executors.newFixedThreadPool(workers)` | `submit` | `Future.get()` | `close()` at the end of `try` |
| C# | the .NET thread pool, with `MaxDegreeOfParallelism` | `Parallel.For` | an array slot for each job | `Parallel.For` returns |
| Go | `workers` goroutines that read one channel | a send on the channel | a slice element for each job | `close` the channel, then `WaitGroup.Wait` |
| Rust | `workers` scoped threads that share one `mpsc` receiver | `send` on the channel | a second channel with `(index, count)` pairs | the end of `thread::scope` |
| C | `workers` threads from `pthread_create` | an index behind a mutex | an array slot for each job | `pthread_join` on each thread |
| C++ | a small `ThreadPool` class with `std::jthread` | `submit` returns a `std::future` | `std::future::get()` | the destructor of the pool |

- **Python** uses `ThreadPoolExecutor`, "An Executor subclass that uses a pool of at most max_workers threads to execute calls asynchronously".[^1] `submit` returns a `Future` at once. Leaving the `with` block waits for all jobs, and `result()` gives the value of each one.[^1]
- **Java** uses `Executors.newFixedThreadPool`, which "reuses a fixed number of threads operating off a shared unbounded queue". A job that comes when all threads are busy waits "in the queue until a thread is available".[^2] `ExecutorService` has a `close()` method, which "waits until all tasks have completed execution and the executor has terminated", so a `try` block stops the pool.[^3]
- **C#** does not make its own threads. The .NET thread pool gives "a pool of worker threads that are managed by the system".[^4] `Parallel.For` runs the jobs on it, and `MaxDegreeOfParallelism` "limits the number of concurrent operations to the set value".[^5] `Parallel.For` returns "when all threads have completed".[^6]
- **Go** follows Go by Example: the workers "receive work on the jobs channel and send the corresponding results".[^7] Here each worker writes the result to its own element of a slice, so the solution needs no results channel. Closing the jobs channel ends the `range` loop in each worker, and a `WaitGroup` waits for them.
- **Rust** follows the thread pool of the Rust book: the workers share one receiver of an `mpsc` channel behind a `Mutex`.[^8] The workers run in `thread::scope`, so that they can borrow `numbers`. When the sender is dropped, the channel has "hung up", and `recv` returns an error once the queue is empty.[^9] Then the worker stops.
- **C** keeps the queue as an index into the array, behind a `pthread_mutex_t`. A worker locks the mutex, takes the next index, and unlocks it before it runs the job. If another worker holds the mutex, the call "shall block until the mutex becomes available".[^10]
- **C++** has no thread pool in the standard library, so the solution has a small one: a queue of `std::packaged_task`, a mutex, a condition variable and a vector of `std::jthread`. A `packaged_task` stores the return value "or exception thrown" in a shared state that a `std::future` reads.[^11] The destructor lets the threads finish the queue and joins them.
- **JavaScript and TypeScript** have no pool of workers in Node.js, but the Node.js documentation advises "use a pool of Workers" when there are many jobs, because otherwise "the overhead of creating Workers would likely exceed their benefit".[^12] The solution starts `workers` workers and gives each one a new job when it sends back a result.

### Alternatives

- **A process pool in Python.** In CPython, "only one thread can execute Python code at once", so a thread pool does not make CPU-bound Python code faster.[^13] `ProcessPoolExecutor` has the same interface and can "side-step the Global Interpreter Lock", but only with picklable jobs and results.[^1]
- **`invokeAll` in Java.** It takes a collection of `Callable` jobs and returns "a list of Futures holding their status and results when all complete".[^3]
- **Data parallelism in Rust.** The rayon crate has `par_iter`, which "provides parallel implementations of many iterative functions" such as `map`, and a global thread pool that `ThreadPoolBuilder` can customize.[^14] With rayon, the solution is `numbers.par_iter().map(...).collect()`.
- **A configurable pool in Java.** `ThreadPoolExecutor` takes a core size, a maximum size and a queue. With a bounded queue, a job that cannot be queued and cannot get a new thread "will be rejected".[^15]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no numbers give an empty list. The pool can start and stop with no jobs.
- `one-worker`: with one worker, the jobs run one after another, but still not on the caller's thread.
- `more-workers-than-jobs`: 8 workers and 2 jobs. Some workers get no job, and the pool must still stop. The JavaScript solution starts only as many workers as there are jobs.
- `duplicates`: equal numbers are separate jobs with separate results.
- `wide-intermediate`: the sequence from 113,383 reaches 2,482,111,348, and the sequence from 837,799 reaches 2,974,984,576. Both are larger than 2^31 − 1, so 32-bit arithmetic overflows. JavaScript numbers hold these values exactly.
- `many`: 200 jobs on 4 workers. The jobs finish in an order that changes from run to run, and the results must still come back in the order of the numbers.

## Pitfalls

- **A job that waits for another job of the same pool.** If all threads of a pool run jobs that wait for jobs that are still in the queue, no thread is free to run those, and the pool deadlocks. The Python documentation shows a job that submits a second job to a pool of one worker and waits for it. The second job never runs, because "there is only one worker thread", and that thread runs the first job.[^1] Do not wait on a future of the same pool inside a job, or use a pool that is large enough.
- **Holding the lock while the job runs.** In Rust, `while let Ok(job) = receiver.lock().unwrap().recv()` holds the lock until the end of the loop body. The Rust book explains that `while let` "does not drop temporary values until the end of the associated block", so other workers "cannot receive jobs".[^8] A `let` statement drops the guard at its end, as in the solution. The same rule holds in C: unlock the mutex before the job runs.
- **An unbounded queue.** The queue of `newFixedThreadPool` has no limit.[^2] If jobs come faster than the workers finish them, the queue grows without limit. Use a `ThreadPoolExecutor` with a bounded queue, or limit the producer.[^15]
- **A pool that is never stopped.** The threads of a fixed pool "will exist until it is explicitly shutdown".[^2] Python joins the threads of a `ThreadPoolExecutor` before the interpreter exits.[^1] In both cases, a pool that is never shut down can keep the program alive or hold its threads after the work is done. Close the pool when its work is finished.
- **Results in the wrong order.** Jobs on a pool finish in any order. Store each result at the index of its job, or keep the futures in the order of the input, as the solutions do.

## Related tasks

- [[wait-for-threads-to-finish]] starts one thread for each job and joins all of them.
- [[start-a-thread]] starts one thread and gives it its argument.
- [[thread-pool]] explains the design pattern behind this task.
- [[futures]] are the handles that `submit` returns.

## References
