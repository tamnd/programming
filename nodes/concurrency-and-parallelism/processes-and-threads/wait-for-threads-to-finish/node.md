---
tag: 6C4496
kind: task
slug: wait-for-threads-to-finish
name: Wait for threads to finish
aliases: [join threads, wait for all threads, join all threads, WaitGroup example, Task.WhenAll example, Promise.all workers, collect results from threads, fork-join, pthread_join example]
area: concurrency-and-parallelism/processes-and-threads
level: intro
summary: Sum each chunk of numbers on its own thread, start all threads before waiting, wait until every thread has finished, and return the sums in the order of the chunks.
contract: contract.yaml
category: concurrency
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [start-a-thread, threads, thread-join-and-detach, futures, thread-pool, data-races]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_join.html
    title: POSIX.1-2024, pthread_join
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/thread/thread/join
    title: std::thread::join, cppreference.com
    locator: the description
  - url: https://en.cppreference.com/w/cpp/thread/jthread
    title: std::jthread, cppreference.com
    locator: the class description
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: The Python Standard Library, concurrent.futures
    locator: Executor.shutdown; Future.result
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
    title: Java SE 21 API, java.lang.Thread
    locator: join(), join(long), join(Duration)
  - url: https://pkg.go.dev/sync
    title: Go standard library, package sync
    locator: type WaitGroup; WaitGroup.Go; WaitGroup.Wait
  - url: https://doc.rust-lang.org/std/thread/fn.scope.html
    title: "The Rust Standard Library, Function std::thread::scope"
    locator: the function description
  - url: https://doc.rust-lang.org/std/thread/struct.JoinHandle.html
    title: "The Rust Standard Library, Struct std::thread::JoinHandle"
    locator: join
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.task.whenall
    title: .NET API reference, Task.WhenAll Method
    locator: the summary; Remarks of WhenAll<TResult>(IEnumerable<Task<TResult>>)
  - url: https://docs.python.org/3/library/threading.html
    title: The Python Standard Library, threading
    locator: Thread objects; join(); run()
  - url: https://gobyexample.com/waitgroups
    title: Go by Example, WaitGroups
    locator: the notes after the example
---

## Task

Write a function `parallel_sums(chunks)` that gets a list of chunks, each a list of integers. The function sums each chunk on a thread other than the caller. It starts the work for all chunks before it waits for any of them. Then it waits until every thread has finished and returns the sums, in the order of the chunks. For example, `parallel_sums([[1, 2, 3], [4, 5]])` is `[6, 9]`.

There are 0 to 32 chunks. Each chunk has 0 to 10,000 items, and each item has an absolute value of 10^9 or less, so a sum needs 64 bits. The task is about the wait: how the caller knows that all threads are done, how it gets their results, and what it sees when a thread fails. [[start-a-thread]] is about the start.

## Solutions

| Language | Start | Wait for all | Results |
|---|---|---|---|
| Python | `ThreadPoolExecutor.submit` | the end of the `with` block | `Future.result()` |
| JavaScript | `new Worker` for each chunk | `Promise.all` | the array that the promise gives |
| TypeScript | as JavaScript | as JavaScript | as JavaScript |
| Java | `Thread.ofPlatform().start` | `join()` on each thread | an array slot for each thread |
| C# | `Task.Run` | `Task.WhenAll` | the array that `WhenAll` gives |
| Go | `go` with `wg.Add(1)` and `wg.Done()` | `WaitGroup.Wait` | a slice element for each goroutine |
| Rust | `Scope::spawn` in `thread::scope` | the end of the scope, or `join()` | the value that `join()` returns |
| C | `pthread_create` | `pthread_join` on each thread | the pointer that the thread returns |
| C++ | `std::jthread` | the destructor of each `std::jthread` | a vector slot for each thread |

- **C** waits with `pthread_join`, which suspends the caller "until the target thread terminates, unless the target thread has already terminated".[^1] The value that the thread returns comes back through the second argument. The solution returns a pointer to the sum in the job struct of the thread.
- **C++** has `join()`, and "The completion of the thread identified by *this synchronizes with the corresponding successful return from join()".[^2] The solution uses `std::jthread`, which has the same general behavior as `std::thread` but "automatically rejoins on destruction".[^3] The threads live in a block, and the end of the block joins all of them.
- **Python** uses a `ThreadPoolExecutor` with one worker for each chunk. Leaving the `with` block shuts the executor down and waits, "as if Executor.shutdown() were called with wait set to True".[^4] `Future.result()` returns the value of the call. "If the call raised an exception, this method will raise the same exception."[^4]
- **Java** calls `join()` on each thread. It "Waits for this thread to terminate".[^5] Each thread writes its own element of an array, and the caller reads the array after all joins.
- **Go** uses a `sync.WaitGroup`. A WaitGroup "is a counting semaphore typically used to wait for a group of goroutines or tasks to finish".[^6] The caller calls `Add(1)` before each `go` statement, each goroutine calls `Done` when it returns, and `Wait` blocks until the counter is zero.[^6]
- **Rust** uses `thread::scope`. All threads of the scope "will be automatically joined before this function returns", so they can borrow `chunks` without `'static` data.[^7] `JoinHandle::join` gives the return value of each thread, and the completion of the thread "synchronizes with this function returning".[^8]
- **C#** queues each sum with `Task.Run` and waits with `Task.WhenAll`. When no task fails, the result of `WhenAll` is "an array containing all of the results of the supplied tasks in the same order as they were provided".[^9] The tasks run on threads of the thread pool, not on new threads.
- **JavaScript and TypeScript** start one Node.js worker for each chunk and wrap its message in a promise. `Promise.all` keeps the order of the promises. The function returns a promise, because the main thread does not block.

### Alternatives

- **A countdown latch.** Java's `CountDownLatch` and C++'s `std::latch` start at the number of threads. Each thread counts down once, and the caller waits for zero. The threads do not need handles.
- **A join with a time limit.** Python's `join(timeout)` always returns `None`, so the caller must call `is_alive()` after it to see whether the time ran out.[^10] Java has `join(Duration)`, which returns a `boolean`.[^5]
- **Results through a channel.** In Go, each goroutine can send its sum on a channel, with its index. The caller receives one value for each goroutine, so it needs no WaitGroup. See [[channels]].
- **A group that cancels on error.** The Go `WaitGroup` gives no error back. Go by Example says that this approach "has no straightforward way to propagate errors from workers" and points to the `errgroup` package.[^11]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no chunks start no threads and give an empty list. Python's `ThreadPoolExecutor` needs at least one worker, so the solution returns early.
- `empty-chunk`: an empty chunk has the sum 0. The thread for it still starts and must still be joined.
- `single`: one chunk, one thread.
- `negative`: negative numbers sum as usual.
- `limit`: 3 × 10^9 does not fit in a 32-bit `int`. The solutions use 64-bit integers; JavaScript numbers hold such sums exactly.
- `many`: 32 threads run at the same time, and the sums must come back in the order of the chunks, not in the order in which the threads finish.

## Pitfalls

- **A join right after each start.** A loop that starts a thread and joins it at once runs the threads one after another. Start all threads first, then join them. Here a `Barrier` lets three threads pass only when all three are at it at the same time:

```python run id=join-after-each-start
import threading


def threads_that_met(start_all_first):
    barrier = threading.Barrier(3, timeout=1.0)
    met = []

    def work(i):
        try:
            barrier.wait()
            met.append(i)
        except threading.BrokenBarrierError:
            pass

    threads = [threading.Thread(target=work, args=(i,)) for i in range(3)]
    if start_all_first:
        for t in threads:
            t.start()
        for t in threads:
            t.join()
    else:
        for t in threads:
            t.start()
            t.join()
    return len(met)


print("start all, then join:", threads_that_met(True))
print("join after each start:", threads_that_met(False))
```

```output for=join-after-each-start
start all, then join: 3
join after each start: 0
```

  With a join after each start, the first thread waits alone at the barrier until the timeout breaks it.
- **Errors that disappear.** An exception in the function of a Python `Thread` does not reach `join()`. `threading.excepthook()` handles it, and by default it prints the exception on `sys.stderr`.[^10] `Future.result()` raises it again in the caller.[^4] In C#, the task from `WhenAll` faults when any task faults, and its exceptions hold all of them.[^9]
- **Joining the current thread.** A thread that joins itself waits forever. Python's `join()` raises `RuntimeError` instead, "as that would cause a deadlock".[^10]
- **Two joins of one thread.** POSIX says that the results of multiple simultaneous calls of `pthread_join` on the same thread are undefined.[^1] In C++, concurrent calls of `join()` on one thread object are a data race.[^2] Give each thread one owner that joins it.
- **A `WaitGroup` that counts too late.** The calls to `Add` "should execute before the statement creating the goroutine or other event to be waited for".[^6] If a goroutine calls `Add` itself, the caller can reach `Wait` first and miss it. Call `Add` in the caller, then start the goroutine.
- **Blocking where the caller must not block.** `Task.WhenAll` does not block the calling thread, but its `Result` does.[^9] In code that is already asynchronous, `await` the task in place of a blocking wait.

## Related tasks

- [[start-a-thread]] starts one thread for each item and passes it its argument.
- [[thread-join-and-detach]] explains joinable and detached threads and the end of the program.
- [[futures]] are handles that hold the result of a job that runs elsewhere.

## References
