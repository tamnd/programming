---
tag: EBXWXF
kind: concept
slug: futures
name: Futures and promises
aliases: [futures, promises, future, promise, deferred, task, eventual value, CompletableFuture, Promise]
area: concurrency-and-parallelism/futures-and-promises
level: core
summary: >-
  A future is a placeholder for the result of a computation that has not finished. A promise is the side that sets the result once. Code can wait for the value, or attach the next step to it.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1977
  people: [Henry Baker, Carl Hewitt]
  note: Baker and Hewitt described call-by-future, in which a separate process evaluates each argument and the parameter is bound to that future.
see_also: [async-await, threads, channels, closures, exception-handling]
same_as:
  wikidata: Q1426138
  wikipedia: Futures_and_promises
sources:
- url: https://tc39.es/ecma262/#sec-promise-objects
  title: ECMAScript Language Specification, Ecma International
  locator: 27.2 Promise Objects
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/CompletableFuture.html
  title: CompletableFuture, Java SE 25 API documentation
  locator: class description
- url: https://en.cppreference.com/w/cpp/thread/promise
  title: std::promise, cppreference.com
  locator: description
- url: https://en.cppreference.com/w/cpp/thread/future
  title: std::future, cppreference.com
  locator: description
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise
  title: Promise, MDN Web Docs
  locator: Description
- url: https://docs.python.org/3.14/library/asyncio-future.html
  title: Futures, asyncio, Python 3.14 documentation
  locator: Future Object
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/concurrent/Future.html
  title: Future, Java SE 25 API documentation
  locator: interface description
- url: https://docs.python.org/3.14/library/concurrent.futures.html
  title: concurrent.futures, Python 3.14 documentation
  locator: Future Objects
- url: https://promisesaplus.com/
  title: Promises/A+ specification
  locator: introduction
- url: https://learn.microsoft.com/en-us/dotnet/standard/asynchronous-programming-patterns/task-based-asynchronous-pattern-tap
  title: Task-based asynchronous pattern (TAP), Microsoft Learn
  locator: Task status
- url: https://doc.rust-lang.org/std/future/trait.Future.html
  title: Trait Future, Rust standard library documentation
  locator: The poll method
- url: https://doc.rust-lang.org/book/ch17-01-futures-and-syntax.html
  title: Futures and the Async Syntax, The Rust Programming Language
  locator: '17.1'
- url: https://kotlinlang.org/docs/composing-suspending-functions.html
  title: Composing suspending functions, Kotlin documentation
  locator: Concurrent using async
- url: https://en.cppreference.com/w/cpp/thread/async
  title: std::async, cppreference.com
  locator: Notes
- url: https://www.plover.com/~mjd/misc/hbaker-archive/Futures.html
  title: The Incremental Garbage Collection of Processes, H. Baker and C. Hewitt, ACM SIGPLAN Notices 12(8), 1977
  locator: Introduction
---

## Idea

A slow operation, such as a network request, a disk read or a long calculation, does not have its result at the time of the call. A future is an object that stands for that result before it exists. The caller gets the future at once and continues with other work. Later, the caller reads the value from the future, and waits only if the value is not ready yet. A promise is the other side of the same object: the code that does the work uses the promise to store the value, or an error, exactly one time.

```python run id=idea
import time
from concurrent.futures import ThreadPoolExecutor


def fetch(name):
    time.sleep(0.05)
    return f"{name} loaded"


with ThreadPoolExecutor() as pool:
    future = pool.submit(fetch, "profile")
    print("request sent, done:", future.done())
    print(future.result())
    print("done:", future.done())
```

```output for=idea
request sent, done: False
profile loaded
done: True
```

The names are not used the same way in all languages. In JavaScript, `Promise` is the name of the object that the caller reads. In C++, `std::promise` is the write side and `std::future` is the read side. Java has `Future` and `CompletableFuture`, C# has `Task`, Kotlin has `Deferred`, and Rust has the `Future` trait. All of them are a placeholder for a value that a computation will make later.

## How it works

**A placeholder with states.** The ECMAScript specification defines a promise as "an object that is used as a placeholder for the eventual results of a deferred (and possibly asynchronous) computation". A promise is pending, fulfilled or rejected, and "a promise is said to be settled if it is not pending, i.e. if it is either fulfilled or rejected."[^1] The change from pending to settled occurs one time only. The specification states: "Attempting to resolve or reject a resolved promise has no effect".[^1] Java gives the same rule for threads that race: "When two or more threads attempt to complete, completeExceptionally, or cancel a CompletableFuture, only one of them succeeds."[^2] Because the value can never change after it is set, many readers can share it without a lock.

**Two sides.** The C++ library shows the two roles as two types. `std::promise` "provides a facility to store a value or an exception that is later acquired asynchronously via a std::future object created by the std::promise object".[^3] `std::future` "provides a mechanism to access the result of asynchronous operations".[^4] Other libraries put both roles in one object, or hide the write side. A JavaScript `Promise` gives `resolve` and `reject` only to the function passed to its constructor.[^5] A C# `TaskCompletionSource` and a Java `CompletableFuture` expose the write side as methods. Python's `asyncio.Future` has `set_result` and `set_exception`, and the documentation states that these objects "are used to bridge low-level callback-based code with high-level async/await code."[^6]

**Read by waiting.** The first futures were read with a blocking call. The Java `Future` interface represents "the result of an asynchronous computation", and the program retrieves the result "using method get when the computation has completed, blocking if necessary until it is ready."[^7] Python's `Future.result()` is the same, and "if the call raised an exception, this method will raise the same exception."[^8] A blocking read is simple, but it holds a whole thread while it waits.

**Read by callback.** A newer style attaches the next step to the future and does not wait. Promises/A+ states: "A promise represents the eventual result of an asynchronous operation. The primary way of interacting with a promise is through its then method".[^9] `then` returns a new promise for the result of the callback, so a program can chain steps, and an error skips to the next rejection handler, as an exception skips to the next `catch`. Java `CompletableFuture` has `thenApply`, `thenCompose` and `thenCombine` for the same purpose.[^2] `async` and `await` are syntax over this model: `await` gives the rest of the function to the future as its callback.

**Eager or lazy.** In most languages, the work starts when the future is made. The .NET guidelines state that tasks "begin their life cycle in a hot state, which means that the asynchronous operations they represent are already initiated".[^10] A JavaScript promise also runs its executor function at once. Rust is different. A Rust future "is a value that might not have finished computing yet",[^11] and its `poll` method "does not block if the value is not ready. Instead, the current task is scheduled to be woken up when it's possible to make further progress".[^11] Nothing calls `poll` until a program awaits the future or gives it to an executor. The Rust book states: "futures in Rust are lazy: they don't do anything until you ask them to with the await keyword."[^12]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | stdlib | `concurrent.futures.Future`, `asyncio.Future`, `asyncio.Task` |
| JavaScript | native | `Promise`, `then`, `await` |
| TypeScript | native | `Promise<T>` |
| Java | stdlib | `Future`, `CompletableFuture` |
| Kotlin | library | `Deferred` in kotlinx.coroutines |
| C# | native | `Task<T>`, `TaskCompletionSource<T>`, `await` |
| Go | idiom | a goroutine and a channel with a buffer of one |
| Rust | native | the `Future` trait, `async`, `.await`, an executor crate |
| C | absent | a thread and a join, or callbacks on an event loop |
| C++ | stdlib | `std::promise`, `std::future`, `std::async` |
| Bash | idiom | `&`, `$!` and `wait` |
| PowerShell | native | `Start-Job`, `Receive-Job`, `Wait-Job` |
| SQL | n/a | the client library runs statements asynchronously |
| Regex, HTML, CSS | n/a | no concurrency |

**Python** has two future types. `concurrent.futures.Future` belongs to a pool of threads or processes: "The Future class encapsulates the asynchronous execution of a callable. Future instances are created by Executor.submit()".[^8] `asyncio.Future` belongs to an event loop, and a coroutine reads it with `await`.[^6]

**JavaScript** and **TypeScript** make all asynchronous APIs of the platform return a `Promise`. MDN describes it as "a proxy for a value not necessarily known when the promise is created", so that "instead of immediately returning the final value, the asynchronous method returns a promise to supply the value at some point in the future."[^5] Handlers always run as later jobs, after the current code ends, even when the promise is already settled.[^1]

**Java** has the blocking `Future` interface and `CompletableFuture`, "a Future that may be explicitly completed (setting its value and status), and may be used as a CompletionStage".[^2] **Kotlin** uses the `async` builder of kotlinx.coroutines, which returns a `Deferred`. The Kotlin documentation calls it a light-weight non-blocking future, and a coroutine reads its value with `await()`.[^13]

**C#** uses `Task<T>` for all asynchronous work, and `await` reads it without a blocked thread.[^10]

**Go** has no future type. A function starts a goroutine and returns a channel that will receive the result.

**Rust** has the `Future` trait in the standard library, but no executor. A crate such as Tokio polls futures and wakes them when I/O is ready.[^12] `std::thread::spawn` returns a `JoinHandle`, which is a blocking future for the result of a thread.

**C** has no future type. **C++** has `std::promise`, `std::future`, `std::shared_future` and `std::async`. A `std::future` is "a unique reference to the result in a shared state", so only one reader can call `get`.[^4]

**Bash** starts a command in the background with `&`, and `wait` with the process ID returns its exit status.

## Variations and related ideas

**Implicit futures.** In Multilisp and some later languages, a future is a transparent value. An operation that needs the real value waits for it with no explicit read, and code that only passes the value on does not wait.

**Promise pipelining.** In a distributed system, a program can send a message to the result of a remote call before the result arrives. The remote side forwards the message when the value is ready, so that a chain of calls takes one network round trip and not many.

**Dataflow variables.** A single-assignment variable, which a thread can read before another thread binds it, is a future without a separate object. Oz and some parallel languages use this model.

**Futures and threads.** A future is a value. A thread is a unit of execution. A join on a thread gives the result of the thread, so a thread handle with a result is one kind of future. See [threads](/threads/).

**Futures and channels.** A future carries one value and keeps it. A channel carries a sequence of values, and each receive removes one. A channel with a buffer of one, used one time, acts as a future. See [channels](/channels/).

**Streams of values.** An async iterator or an observable is the many-values form of a future: the consumer receives a sequence of results over time.

**Async and await.** `async` functions return a future, and `await` suspends the function until the future settles. See [async/await](/async-await/).

## Pitfalls

**A lost error.** A future stores an exception until a program reads it. If no code reads the future, the error is lost and the program continues as if the work succeeded.[^8] JavaScript runtimes report a rejection with no handler, but only as a warning or a process exit, at a time after the error.

**A floating promise.** A call to an asynchronous function without `await` starts the work and drops the future. In JavaScript, `forEach` with an `async` callback has this problem: the loop ends before the work ends. Linters can find promises that nothing awaits or handles.

**A blocked event loop.** A blocking read of a future on the thread that must complete the future causes a deadlock. In C#, `.Result` or `.Wait()` on a UI thread can wait for a continuation that needs that same thread.

**A future that blocks in its destructor.** In C++, "if the std::future obtained from std::async is not moved from or bound to a reference, the destructor of the std::future will block at the end of the full expression until the asynchronous operation completes".[^14] A call to `std::async` that ignores the result is therefore synchronous.

**Sequential waits.** A loop that awaits each step in turn runs the steps one after another. If the steps are independent, start all of them first, and then wait for all of them with `Promise.all`, `Task.WhenAll` or `asyncio.gather`.

**No cancellation.** A JavaScript promise cannot be cancelled. If a program stops waiting, for example after a timeout with `Promise.race`, the work continues. Cancellation must come from a separate object, such as an `AbortSignal` or a `CancellationToken`. A future that nobody needs any more still uses resources until it ends. Baker and Hewitt described this problem as early as 1977: "futures which were thought to be relevant when they were created become irrelevant through being ignored in the body of the expression where they were bound."[^15]

**Lazy futures that never run.** In Rust, a future that no code awaits or spawns never runs. The compiler warns because futures are `#[must_use]`.

## Cost

A future is usually a small object on the heap, with a state, a value slot and a list of callbacks. Each callback is a closure, and each `then` makes a new future. A blocking read holds a thread until the value is ready. A callback or `await` holds no thread, but adds one scheduling step per continuation. A Rust `async fn` compiles to a state machine. That state machine is a value that does not need a heap allocation, unless the program boxes it or spawns it on an executor. Futures do not make the work faster. They let independent work overlap, so the total time can drop to the time of the slowest task.

## History

Henry Baker and Carl Hewitt described futures in 1977 in "The Incremental Garbage Collection of Processes". In their call-by-future, "each formal parameter of a function is bound to a separate process (called a "future") dedicated to the evaluation of the corresponding argument. This mechanism allows the fully parallel evaluation of arguments to a function".[^15] Daniel Friedman and David Wise had used the word promise in 1976 for a similar construct. Robert Halstead made `future` a central construct of Multilisp, a parallel Lisp, in 1985. Barbara Liskov and Liuba Shrira, in 1988, used promises for asynchronous remote procedure calls. The E language and its predecessors used promises with pipelining for distributed objects. Futures came to mainstream libraries later: `java.util.concurrent.Future` in Java 5, `std::future` in C++11, `Task` in .NET 4, and `concurrent.futures` in Python 3.2. In JavaScript, libraries and the Promises/A+ community specification led to the `Promise` type of ECMAScript 2015.[^9] C#, JavaScript, Python, Rust and other languages then added `async` and `await` as syntax over their future types.

## References
