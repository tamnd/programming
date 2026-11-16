---
tag: 0VNQXC
kind: task
slug: start-a-thread
name: Start a thread
aliases: [create a thread, spawn a thread, launch a thread, run a function on a new thread, pthread_create example, std::thread example, start a goroutine, Thread.start]
area: concurrency-and-parallelism/processes-and-threads
level: intro
summary: Start one thread for each name, pass the name to it as an argument, let each thread build a greeting in its own slot, and wait for all threads before returning the greetings.
contract: contract.yaml
category: concurrency
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [threads, wait-for-threads-to-finish, thread-join-and-detach, process, data-races, closures]
sources:
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_create.html
    title: POSIX.1-2024, pthread_create
    locator: DESCRIPTION
  - url: https://en.cppreference.com/w/cpp/thread/thread
    title: std::thread, cppreference.com
    locator: the class description
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Thread.html
    title: Java SE 21 API, java.lang.Thread
    locator: the class description; Thread.Builder examples
  - url: https://docs.python.org/3/library/threading.html
    title: The Python Standard Library, threading
    locator: Thread objects; start(); run()
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Go statements; For statements with for clause; Program execution
  - url: https://pkg.go.dev/sync
    title: Go standard library, package sync
    locator: type WaitGroup; WaitGroup.Go
  - url: https://doc.rust-lang.org/std/thread/fn.spawn.html
    title: "The Rust Standard Library, Function std::thread::spawn"
    locator: the function description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.thread
    title: .NET API reference, Thread Class
    locator: Remarks (Start a thread; Pass data to a thread)
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: the introduction; new Worker(filename[, options]), the option workerData
  - url: https://en.cppreference.com/w/cpp/thread/thread/~thread
    title: std::thread::~thread, cppreference.com
    locator: the description
---

## Task

Write a function `thread_greetings(names)` that starts one new thread for each name. Pass the name to the thread as its argument. The thread builds the string `"hello from " + name` and stores it in its own slot of a result list. After it has started all the threads, the function waits for each of them and returns the list, in the order of the names. For example, `thread_greetings(["Ada", "Linus"])` is `["hello from Ada", "hello from Linus"]`.

The list has 0 to 64 names, and each name has 0 to 50 characters. The task is about the start of a thread: the function to run, the argument, and the place where the thread puts its result. [[wait-for-threads-to-finish]] is about the wait. The plan of the task asks for two threads that print. Two threads that print at the same time print in an order that changes from run to run, so the threads here build strings and the caller checks them.

## Solutions

| Language | Start a thread | Pass the argument | Get the result back |
|---|---|---|---|
| Python | `threading.Thread(target=f, args=(...)).start()` | `args` | a slot of a list, read after `join()` |
| JavaScript | `new Worker(source, { workerData })` | `workerData`, a copy | `postMessage`, wrapped in a promise |
| TypeScript | as JavaScript | as JavaScript | as JavaScript |
| Java | `Thread.ofPlatform().start(runnable)` | a lambda captures a local | an array slot, read after `join()` |
| C# | `new Thread(lambda).Start()` | a lambda captures a local | an array slot, read after `Join()` |
| Go | `go f()` with a `sync.WaitGroup` | a closure | a slice element, read after `Wait()` |
| Rust | `std::thread::spawn(move \|\| ...)` | the closure owns a copy | the return value, from `join()` |
| C | `pthread_create(&id, NULL, f, arg)` | one `void *` | a field of the argument struct |
| C++ | `std::thread(lambda)` | a lambda captures a copy | a vector slot, read after `join()` |

- **C** gives the thread one argument. `pthread_create` creates a thread that executes `start_routine` "with arg as its sole argument".[^1] The solution passes a pointer to a struct that holds the name and a field for the greeting. Each thread gets its own struct, so no two threads write the same memory.
- **C++** starts the thread when the `std::thread` object is made: "Threads begin execution immediately upon construction of the associated thread object".[^2] The return value of the function is ignored, so the thread writes its result to its own element of a vector.
- **Java** has the `Thread.Builder` API. `Thread.ofPlatform().start(runnable)` makes an operating system thread and starts it. The new thread "executes concurrently with the thread that caused it to start".[^3]
- **Python** gives the function to run as `target` and its arguments as `args`. `start()` arranges for the `run()` method of the object "to be invoked in a separate thread of control".[^4]
- **Go** starts a goroutine with the `go` statement. The statement "starts the execution of a function call as an independent concurrent thread of control, or goroutine, within the same address space".[^5] The results of the function are discarded, so the goroutine writes to its own element of a slice. The caller calls `wg.Add(1)` before each `go` statement, and the goroutine calls `wg.Done()` when it returns, so that `Wait()` can wait for it. The calls to `Add` "should execute before the statement creating the goroutine".[^6]
- **Rust** starts a thread with `thread::spawn`, which returns a `JoinHandle`.[^7] The closure must be `'static` and `Send`, because the thread can outlive the function that started it. The solution gives each closure its own `String` with `move`. The thread returns the greeting, and `join()` hands it to the caller.
- **C#** starts a `Thread` with a delegate. A method with an argument can use a `ParameterizedThreadStart` delegate, which takes an `object`.[^8] A lambda that captures typed locals is simpler, and the solution uses it.
- **JavaScript and TypeScript** have no threads that share memory. In Node.js, `node:worker_threads` "enables the use of threads that execute JavaScript in parallel".[^9] The value in `workerData` is cloned with the structured clone algorithm, so the worker gets a copy of the name.[^9] The worker sends the greeting back with `postMessage`. The function returns a promise, because the main thread cannot block to wait for a worker.

### Alternatives

- **A thread pool.** For many small jobs, a pool such as Java's `ExecutorService` or Python's `ThreadPoolExecutor` reuses a few threads. A new thread for each job costs more than the job here.
- **Scoped threads.** Rust's `thread::scope` and C++'s `std::jthread` join their threads at the end of a block. Then a thread can borrow local data, and the caller cannot forget the join. [[wait-for-threads-to-finish]] uses them.
- **A future.** C++'s `std::async`, C#'s `Task.Run` and Java's `ExecutorService.submit` return a handle that holds the result of the function. The caller does not need a result slot.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no names start no threads and give an empty list. In C, the code must not call `calloc` with a size of 0 and then treat the result as an error.
- `single`: one thread.
- `empty-name`: the empty name gives `"hello from "`, with the space.
- `duplicate`: two equal names still start two threads and give two greetings.
- `unicode`: names with characters outside ASCII pass through unchanged. In C the strings are UTF-8 bytes.
- `many`: 32 names start 32 threads that run at the same time. Each thread must write only its own slot.

## Pitfalls

- **Calling `run()` in place of `start()`.** In Python and Java, `run()` is a plain method call. It runs the function on the calling thread, and no new thread starts:

```python run id=run-is-not-start
import threading


def where():
    print("ran on", threading.current_thread().name)


plain = threading.Thread(target=where, name="greeter-1")
plain.run()
started = threading.Thread(target=where, name="greeter-2")
started.start()
started.join()
```

```output for=run-is-not-start
ran on MainThread
ran on greeter-2
```

  Python's `start()` must be called at most once for each thread object, and a second call raises `RuntimeError`.[^4]
- **One shared variable for all threads.** In C, passing `&i` of a loop counter to every thread gives all threads the same address, and they read whatever value it has when they run. Give each thread its own struct. In Go before version 1.22, all iterations of a `for` loop shared one loop variable. Now "Each iteration has its own separate declared variable".[^5] In C#, a lambda in a `for` loop captures the one variable `i`, so the solution copies it to `index` first.
- **Reading the result before the wait.** The thread may not have run yet. Read a result only after `join()`, `Wait()` or the promise. The join also makes the writes of the thread visible to the caller.
- **A program that ends first.** In Go, when `main` returns, the program exits. "It does not wait for other (non-main) goroutines to complete."[^5]
- **Destroying a running `std::thread`.** If a `std::thread` is still joinable when it is destroyed, the destructor calls `std::terminate`.[^10] The C++ solution joins the threads that it started, also when a later start throws.
- **Workers for small jobs.** A Node.js worker starts a new JavaScript engine instance. Workers "are useful for performing CPU-intensive JavaScript operations", and "They do not help much with I/O-intensive work."[^9]

## Related tasks

- [[wait-for-threads-to-finish]] waits for a group of threads and collects their results.
- [[run-a-subprocess]] starts a separate program in a new [[process]].
- [[thread-join-and-detach]] explains what happens to a thread that nobody joins.

## References
