---
tag: NEW-async-await
kind: concept
slug: async-await
name: Async/await
aliases: [async functions, async and await, coroutines with await]
area: concurrency-and-parallelism/async-and-event-loops
level: core
summary: >-
  An async function can pause at each await until a result is ready, and a scheduler runs other tasks in the meantime. Code that waits for many things reads like sequential code.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2007
  people: [Don Syme, Tomas Petricek, Dmitry Lomov]
  note: F# asynchronous workflows came first. C# 5.0 added the async and await keywords in 2012, and Python, JavaScript, Kotlin, Rust and C++ followed.
see_also: [closures]
same_as:
  wikidata: Q16245951
  wikipedia: Async/await
sources:
  - url: https://peps.python.org/pep-0492/
    title: "PEP 492: Coroutines with async and await syntax"
    locator: Specification; Why StopAsyncIteration?
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#coroutine-function-definition
    title: The Python Language Reference, Compound statements, Python 3.14
    locator: Coroutine function definition
  - url: https://docs.python.org/3.14/reference/expressions.html#await
    title: The Python Language Reference, Expressions, Python 3.14
    locator: Await expression
  - url: https://docs.python.org/3.14/library/asyncio-task.html#asyncio.gather
    title: Python 3.14 documentation, Coroutines and Tasks
    locator: Coroutines; Running Tasks Concurrently, asyncio.gather
  - url: https://docs.python.org/3.14/library/asyncio-task.html#asyncio.create_task
    title: Python 3.14 documentation, Coroutines and Tasks
    locator: Creating Tasks, asyncio.create_task, the note on references
  - url: https://docs.python.org/3.14/library/asyncio-dev.html#running-blocking-code
    title: Python 3.14 documentation, Developing with asyncio
    locator: Running Blocking Code
  - url: https://docs.python.org/3.14/library/asyncio-dev.html#detect-never-awaited-coroutines
    title: Python 3.14 documentation, Developing with asyncio
    locator: Detect never-awaited coroutines
  - url: https://tc39.es/ecma262/2026/#sec-async-function-definitions
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 15.8 Async Function Definitions
  - url: https://tc39.es/ecma262/2026/#await
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.7.5.3 Await
  - url: https://tc39.es/ecma262/2026/#sec-jobs
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 9.5 Jobs and Host Operations to Enqueue Jobs
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/async
    title: C# language reference, async
    locator: the paragraph on how an async method runs until its first await
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/asynchronous-programming/
    title: C# documentation, Asynchronous programming with async and await
    locator: Start tasks concurrently; Await tasks efficiently
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.valuetask-1
    title: .NET API reference, ValueTask<TResult> Struct
    locator: Remarks
  - url: https://doc.rust-lang.org/reference/expressions/await-expr.html
    title: The Rust Reference
    locator: Await expressions
  - url: https://doc.rust-lang.org/std/future/trait.Future.html
    title: Rust standard library, Trait Future
    locator: Runtime characteristics
  - url: https://timsong-cpp.github.io/cppwp/n4950/dcl.fct.def.coroutine
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[dcl.fct.def.coroutine], paragraphs 1 and 9"
  - url: https://timsong-cpp.github.io/cppwp/n4950/expr.await
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[expr.await]"
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3552r3.html
    title: "P3552R3: Add a Coroutine Task Type"
    locator: Proposal
  - url: https://kotlinlang.org/docs/coroutines-basics.html
    title: Kotlin documentation, Coroutines basics
    locator: the introduction to suspending functions and the kotlinx.coroutines library
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.coroutines/
    title: Kotlin standard library API, package kotlin.coroutines
    locator: Continuation, startCoroutine, suspendCoroutine
  - url: https://openjdk.org/jeps/444
    title: "JEP 444: Virtual Threads"
    locator: Summary; Goals
  - url: https://go.dev/ref/spec#Go_statements
    title: The Go Programming Language Specification
    locator: Go statements
  - paper: doi:10.1145/872734.806932
    title: Henry G. Baker Jr. and Carl Hewitt, The incremental garbage collection of processes, ACM SIGPLAN Notices 12(8), 1977
    locator: the definition of futures
  - paper: doi:10.1007/978-3-642-18378-2_15
    title: Don Syme, Tomas Petricek and Dmitry Lomov, The F# Asynchronous Programming Model, PADL 2011
    locator: Introduction (asynchronous workflows in F#)
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/whats-new/csharp-version-history#c-version-50
    title: The history of C#
    locator: C# version 5.0
  - url: https://github.com/tc39/proposals/blob/main/finished-proposals.md
    title: TC39, Finished Proposals
    locator: Async functions, expected publication year 2017
  - url: https://kotlinlang.org/docs/whatsnew13.html#coroutines-release
    title: What's new in Kotlin 1.3
    locator: Coroutines release
  - url: https://blog.rust-lang.org/2019/11/07/Async-await-stable.html
    title: Async-await on stable Rust! (Rust blog, 2019-11-07)
    locator: the announcement
  - url: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0912r5.html
    title: "P0912R5: Merge Coroutines TS into C++20 working draft"
    locator: Proposal
  - url: https://journal.stuffwithstuff.com/2015/02/01/what-color-is-your-function/
    title: Bob Nystrom, What Color is Your Function? (2015)
    locator: the whole essay
  - url: https://docs.python.org/3.14/library/asyncio-task.html#asyncio.TaskGroup
    title: Python 3.14 documentation, Coroutines and Tasks
    locator: Task Groups
  - url: https://docs.python.org/3.14/library/asyncio-task.html#asyncio.to_thread
    title: Python 3.14 documentation, Coroutines and Tasks
    locator: Running in Threads, asyncio.to_thread
  - url: https://go.dev/doc/faq#goroutines
    title: Go FAQ
    locator: Why goroutines instead of threads?
---

## Idea

A program that talks to a network or a disk spends most of its time in waits. One thread per wait is costly when there are thousands of waits. Callbacks avoid the threads, but they break the code into many small functions. Async/await keeps one function per job. An `async` function runs until it reaches an `await`. There it gives control back to a scheduler, which runs other tasks. When the awaited result is ready, the scheduler resumes the function at the same place, with its local variables intact.

In Python, `asyncio.gather` runs two loads at the same time on one thread. The slow load starts first, but the fast load ends first. The total time is about 200 ms, not 300 ms.[^4]

```python run id=idea
import asyncio


async def load(name, delay):
    print("start", name)
    await asyncio.sleep(delay)
    print("end", name)
    return name


async def main():
    results = await asyncio.gather(load("slow", 0.2), load("fast", 0.1))
    print(results)


asyncio.run(main())
```

```output for=idea
start slow
start fast
end fast
end slow
['slow', 'fast']
```

`async def` makes a coroutine function. `await` is allowed only inside it.[^2][^3] `asyncio.run` starts the event loop and runs `main` to its end.

## How it works

The compiler turns an async function into a state machine. Each `await` is a suspension point. The state records which suspension point the function reached and the local variables that are still needed after it. A call of the async function makes this state and returns an object that stands for the result that will come: a coroutine in Python, a `Promise` in JavaScript, a `Task` in C#, a `Future` in Rust. Python coroutines use the generator machinery of the interpreter for this.[^1]

A scheduler, usually an event loop, drives the state machines. It keeps a queue of tasks that can run, and a list of things that tasks wait for: timers, sockets, and other tasks. One turn of the loop works as follows.

1. Take a task that can run, and resume it.
2. The task runs until it awaits something that is not ready. It registers itself as the continuation of that thing and returns to the loop.
3. When no task can run, wait for the next timer or for the operating system to report a ready socket.
4. Mark the tasks that wait on the ready things as runnable, and go to step 1.

The timeline of the example in Idea:

```text
time    event loop                              task slow            task fast
0 ms    resume slow                             print "start slow"
0 ms                                            await sleep(0.2)
0 ms    resume fast                                                  print "start fast"
0 ms                                                                 await sleep(0.1)
0 ms    nothing can run: wait for a timer
100 ms  timer of fast fires, resume fast                             print "end fast", return
200 ms  timer of slow fires, resume slow        print "end slow", return
200 ms  both results ready, resume main         main prints the list
```

The scheduling is cooperative. A task gives up control only at an `await`. With one thread, two tasks never run at the same instant, so the code between two awaits needs no lock. The other side of the same rule: a task that computes for a long time, or that calls a blocking function, stops every other task (see Pitfalls).

Languages differ in when the body of an async function starts. In JavaScript, the call runs the body at once, up to the first `await`. The rest runs later, from the job queue, even when the awaited value is already there.[^8][^9][^10]

```javascript run id=order
async function load() {
  console.log("1. load starts");
  await null;
  console.log("3. load resumes");
}

load();
console.log("2. caller continues");
```

```output for=order
1. load starts
2. caller continues
3. load resumes
```

| Language | A call of the async function gives | The body starts | The scheduler |
|---|---|---|---|
| Python | a coroutine object | when it is awaited or wrapped in a task[^4] | the `asyncio` event loop of the standard library |
| JavaScript, TypeScript | a `Promise` | at once, up to the first `await`[^8] | the host's event loop (browser, Node.js, Deno) and the job queue |
| C# | a `Task`, `Task<T>` or `ValueTask<T>` | at once, up to the first `await` of an incomplete task[^11] | the thread pool, or the synchronization context of the caller |
| Rust | a value that implements `Future` | when an executor first polls it[^15] | none in the standard library; an executor from a library |
| C++ | the coroutine type that the program or a library defines | as the promise type says, with `initial_suspend`[^16] | none in C++23; the C++26 draft adds `std::execution::task`[^18] |
| Kotlin | the result itself, to a caller that is also `suspend` | at once, in the coroutine of the caller | the `kotlinx.coroutines` library; the standard library has only the primitives[^19][^20] |

## In languages

The example files show the same two loads in each language where async/await exists. The Rust, C++ and Kotlin examples use only the standard library, so each one contains a small scheduler. Real programs use a library executor or `kotlinx.coroutines` instead.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `async def`, `await`, the `asyncio` package |
| JavaScript | native | `async function`, `await`, `Promise` |
| TypeScript | native | as JavaScript, with the type `Promise<T>` |
| Java | absent | virtual threads, or `CompletableFuture` with callbacks |
| C | absent | callbacks with an event library, or threads |
| C++ | native | `co_await` in a coroutine; the task type comes from a library |
| C# | native | `async`, `await`, `Task` |
| Go | absent | goroutines and channels |
| Rust | native | `async fn`, `.await`, the `Future` trait; the executor comes from a library |
| Kotlin | native | `suspend fun`; `async` and `await` are library functions of `kotlinx.coroutines` |
| PowerShell | absent | thread jobs and `ForEach-Object -Parallel` |
| Bash | absent | background processes with `&` and `wait` |
| SQL | n/a | a query does not control its own scheduling |
| Regex | n/a | a pattern has no control flow over time |
| HTML/CSS | n/a | markup and style have no control flow |

Java has no `async` or `await`. Virtual threads make a blocking call cheap: when a virtual thread blocks on I/O, the runtime unmounts it and runs another virtual thread on the same carrier thread.[^21] Plain blocking code then scales to many waits, which is the goal of async/await. `CompletableFuture` gives a callback style for code that wants futures.

Go has no `async` or `await` either. The `go` statement starts a function call in a separate goroutine.[^22] The runtime multiplexes goroutines onto a set of operating system threads. When a goroutine blocks in a system call, the runtime moves the other goroutines to another thread, so they keep running.[^33] Go code is therefore written in the blocking style.

C has no coroutines in the standard. Programs use callbacks with an event library, or one thread for each blocking job. PowerShell and Bash run work in parallel with jobs and background processes, but they have no suspension inside a function.

## Variations and related ideas

- **Stackless and stackful coroutines.** Async/await in Python, JavaScript, C#, Rust, C++ and Kotlin is stackless: only the async function becomes a state machine, so each caller on the path must also be async and must await. Go goroutines and Java virtual threads are stackful: the runtime saves the whole stack, so any function can block, and no function needs a mark.
- **Function colour.** Because only async functions can await, a code base splits into async and sync functions, and a sync function cannot call an async one in the normal way. Bob Nystrom named this the problem of function colour.[^30]
- **Futures and promises with callbacks.** A future is a placeholder for a result that a computation will give later.[^23] Before async/await, code attached callbacks to it (`then` in JavaScript, `ContinueWith` in C#). Async/await is syntax over the same objects.
- **Lazy and eager futures.** A Rust future does nothing until it is polled.[^15] A Python coroutine does not run until it is awaited or scheduled.[^4] A JavaScript promise and a C# task are already running when the call returns.
- **Structured concurrency.** `asyncio.TaskGroup` in Python and `coroutineScope` in `kotlinx.coroutines` tie the life of child tasks to a block. The block waits for all children, and an error in one child cancels the others.[^31][^19]
- **Async iteration and cleanup.** Python has `async for`, which awaits the next item of an asynchronous iterator, and `async with`, which awaits the enter and exit steps of a context manager.[^1]

## Pitfalls

### A call without await

A call of an async function does not wait for its result. In Python, the coroutine does not even start, so the save never happens.

```python wrong id=no-await
import asyncio


async def save(record):
    print("saved", record)


async def main():
    save("order 1")
    print("done")


asyncio.run(main())
```

```output for=no-await
done
```

Python prints `RuntimeWarning: coroutine 'save' was never awaited` on standard error when the coroutine object is destroyed.[^7] The fix is `await save("order 1")`. In JavaScript, the body runs, but the caller gets a `Promise`, not the value:

```javascript wrong id=promise-plus-one
async function loadTotal() {
  return 41;
}

const total = loadTotal();
console.log(total + 1);
```

```output for=promise-plus-one
[object Promise]1
```

The fix is `const total = await loadTotal();` inside an async function. With type checking, TypeScript rejects this code with error TS2365, because `+` cannot take a `Promise<number>`. The Rust compiler warns that the future is unused (see the Rust example).

### A blocking call stops every task

`time.sleep` blocks the thread, and the event loop runs on that thread. No other task can run until it returns, so the two loads run one after the other.[^6]

```python wrong id=blocking
import asyncio
import time


async def load(name, delay):
    print("start", name)
    time.sleep(delay)
    print("end", name)
    return name


async def main():
    results = await asyncio.gather(load("slow", 0.2), load("fast", 0.1))
    print(results)


asyncio.run(main())
```

```output for=blocking
start slow
end slow
start fast
end fast
['slow', 'fast']
```

The fix is the async form of the call, here `await asyncio.sleep(delay)`. For a blocking function that has no async form, `await asyncio.to_thread(function, ...)` runs it in another thread.[^32] Long computations need the same care, because the loop cannot stop a task that does not await.

### Awaits in sequence where concurrency was meant

Each `await` waits for its result before the next line runs. Two awaits in a row run the loads one after the other and take the sum of the times, 300 ms here.

```javascript wrong id=sequential
const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms));

async function load(name, ms) {
  console.log("start", name);
  await sleep(ms);
  console.log("end", name);
  return name;
}

async function main() {
  const slow = await load("slow", 200);
  const fast = await load("fast", 100);
  console.log([slow, fast].join(", "));
}

main();
```

```output for=sequential
start slow
end slow
start fast
end fast
slow, fast
```

The fix is to start both and then await both: `Promise.all` in JavaScript (see the JavaScript example), `asyncio.gather` in Python, `Task.WhenAll` in C#.[^12]

### Await outside an async function

`await` is a syntax error in a function that is not async.[^3]

```python fails expect=compile id=await-in-def
import asyncio


def report():
    value = await asyncio.sleep(0.1, result=42)
    print(value)
```

Python stops with `SyntaxError: 'await' outside async function`. The fix is to make the caller async, and to start the outermost coroutine with `asyncio.run`. This is the colour problem of Variations: the change goes up the whole chain of callers.

### A task with no reference can disappear

The `asyncio` event loop keeps only weak references to tasks. A task from `asyncio.create_task` that nothing else refers to may be garbage collected before it is done.[^5] Keep each task in a variable or a set until it ends, or start it in an `asyncio.TaskGroup`.

## Cost

- **State per task.** A suspended async function keeps only the variables that it needs after the suspension point, not a whole thread stack. In Rust, the size of the future is known at compile time. A buffer that is still alive at an `await` is part of the future:

```rust run id=future-size
use std::future::{Future, poll_fn};
use std::task::Poll;

// A future that is pending once, so an async fn can await it.
fn yield_once() -> impl Future<Output = ()> {
    let mut polled = false;
    poll_fn(move |_| {
        if polled {
            Poll::Ready(())
        } else {
            polled = true;
            Poll::Pending
        }
    })
}

async fn sum_before_await() -> u32 {
    let total = {
        let buffer = [1u8; 1024];
        buffer.iter().map(|&b| u32::from(b)).sum()
    };
    yield_once().await;
    total
}

async fn sum_after_await() -> u32 {
    let buffer = [1u8; 1024];
    yield_once().await;
    buffer.iter().map(|&b| u32::from(b)).sum()
}

fn main() {
    println!(
        "buffer dropped before await: {} bytes",
        size_of_val(&sum_before_await())
    );
    println!(
        "buffer kept across await: {} bytes",
        size_of_val(&sum_after_await())
    );
}
```

```output for=future-size
buffer dropped before await: 8 bytes
buffer kept across await: 1026 bytes
```

- **Allocation.** A Rust future is a plain value. It goes on the heap only when the program boxes it or gives it to an executor that stores it. A C++ coroutine gets its state from an allocation function, unless the compiler can elide it.[^16] A C# async method allocates a `Task` for its result. `ValueTask<T>` avoids the allocation when the method often completes without a suspension.[^13]
- **Resumption.** In JavaScript, each `await` resumes through the job queue, even when the value is already there (see How it works).[^9] An `await` is cheap, but it is not free, so a tight loop that awaits on each pass pays it on each pass.
- **Threads.** Async/await does not add parallelism. Python and JavaScript run the tasks of one loop on one thread. CPU-heavy work needs threads, processes or workers. C# and Rust executors can run tasks on a pool of threads, and then the code between awaits needs the same care with shared data as threaded code.

## History

- **1977.** Henry Baker and Carl Hewitt described futures: a value that stands for the result of a computation that runs on its own.[^23]
- **2007.** F# added asynchronous workflows, which write asynchronous code with sequential syntax and resume after each wait. The 2011 paper by Syme, Petricek and Lomov describes the model.[^24]
- **2012.** C# 5.0 added the `async` and `await` keywords.[^25]
- **2015.** PEP 492 added `async def` and `await` to Python. Before this, `asyncio` code used generators and `yield from`.[^1]
- **2017.** Async functions became part of ECMAScript 2017.[^26]
- **2018 to 2020.** Kotlin 1.3 made coroutines stable (2018).[^27] Rust stabilized `async` and `.await` in 2019.[^28] C++20 took coroutines from the Coroutines Technical Specification.[^29]

## References
