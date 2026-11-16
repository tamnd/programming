---
tag: QEF7A4
kind: task
slug: use-thread-local-storage
name: Use thread-local storage
aliases: [per-thread counter, ThreadLocal example, threading.local example, thread_local example, ThreadStatic example, _Thread_local example, thread-specific data example]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: Start one thread for each count. Each thread adds 1 to its own thread-local counter as many times as its count says, with no lock, and returns the final value of its counter.
contract: contract.yaml
category: concurrency
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust]
see_also: [thread-local-storage, thread-specific-storage, thread-confinement, start-a-thread, wait-for-threads-to-finish, run-tasks-on-a-thread-pool, data-races]
sources:
  - url: https://docs.python.org/3/library/threading.html
    title: The Python Standard Library, threading
    locator: Thread-local data
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/ThreadLocal.html
    title: Java SE 21 API, java.lang.ThreadLocal
    locator: class description; withInitial; remove()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threadstaticattribute
    title: Microsoft Learn, ThreadStaticAttribute class
    locator: Remarks
  - url: https://doc.rust-lang.org/std/macro.thread_local.html
    title: Rust standard library, macro std::thread_local
    locator: Syntax
  - url: https://doc.rust-lang.org/std/thread/struct.LocalKey.html
    title: Rust standard library, struct std::thread::LocalKey
    locator: Initialization and Destruction; impl LocalKey<Cell<T>>
  - url: https://en.cppreference.com/w/cpp/language/storage_duration
    title: cppreference, Storage duration
    locator: Thread storage duration
  - url: https://en.wikipedia.org/wiki/Thread-local_storage
    title: Wikipedia, Thread-local storage
    locator: C and C++
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Why is there no goroutine ID?
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: worker_threads.isMainThread; Class Worker
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.threadlocal-1
    title: Microsoft Learn, ThreadLocal<T> class
    locator: class description; Values
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_key_create.html
    title: POSIX.1-2024, pthread_key_create
    locator: DESCRIPTION
  - url: https://nodejs.org/api/async_context.html
    title: Node.js documentation, Asynchronous context tracking
    locator: Introduction
  - url: https://docs.python.org/3/library/contextvars.html
    title: Python documentation, contextvars
    locator: introduction
  - url: https://openjdk.org/jeps/506
    title: "JEP 506: Scoped Values"
    locator: Problems with thread-local variables
---

## Task

Write a function `count_per_thread(counts)` that starts one thread for each count. Thread i calls a function `bump()` `counts[i]` times. `bump` has no parameters: it adds 1 to the counter of the thread that calls it and returns the new value. The counter is a thread-local variable, so each thread has its own counter, which starts at 0. When thread i has finished, it reads its counter, and the function returns these values in the order of the counts. For example, `count_per_thread([1, 2, 3])` is `[1, 2, 3]`.

There are 0 to 16 counts, and each count is 0 to 100,000. The result is always equal to `counts`. That is the point of the task: the threads use one name for the counter, but no thread sees the counts of another thread, and the counter needs no lock. A second function `current()` returns the counter of the calling thread. The tests call `bump()` 3 times on one thread, then call `count_per_thread`, and check that `current()` on that thread is still 3. [[thread-local-storage]] explains the idea.

## Solutions

| Language | Thread-local counter | Threads | Results |
|---|---|---|---|
| Python | an attribute of a `threading.local()` object | `threading.Thread`, then `join` | a list slot for each thread |
| JavaScript | a module variable; each worker loads the module again | `new Worker(new URL(import.meta.url))` | a message from each worker |
| TypeScript | as JavaScript | as JavaScript | as JavaScript |
| Java | `ThreadLocal.withInitial(() -> 0)` | `Thread.ofPlatform().start`, then `join` | an array slot for each thread |
| C# | a `static int` field with `[ThreadStatic]` | `new Thread`, `Start`, then `Join` | an array slot for each thread |
| Go | none; each goroutine owns a `Counter` value | `go` statements and a `sync.WaitGroup` | a slice element for each goroutine |
| Rust | `thread_local!` with a `Cell<u32>` | `thread::scope` and `spawn` | the return value of `join` |
| C | `static _Thread_local int32_t` | `pthread_create`, then `pthread_join` | a struct for each thread |
| C++ | a `thread_local std::int32_t` | `std::jthread` | a vector element for each thread |

- **Python** has `threading.local`. "Thread-local data is data whose values are thread specific": the program makes one `local` object and uses its attributes.[^1] A new thread sees no attribute, so `bump` reads the counter with `getattr` and the default 0.
- **Java** uses one `ThreadLocal`. Each thread that uses it "has its own, independently initialized copy of the variable".[^2] `withInitial` takes a `Supplier` that gives the first value in each thread.[^2] The field is `private static final`, as the documentation suggests: `ThreadLocal` instances "are typically private static fields in classes that wish to associate state with a thread".[^2]
- **C#** marks a static field with `[ThreadStatic]`. Such a field "is not shared between threads", and each thread has "a separate instance of the field".[^3] The field has no initializer, so it starts at its default value, 0, in every thread.[^3]
- **Rust** declares the counter with `thread_local!`, which declares "a new thread local storage key of type std::thread::LocalKey".[^4] The value is a `Cell`, so `bump` can change it through the shared reference that `with` gives. The initializer is a `const` block, which "can avoid lazy initialization".[^4] Without it, "Initialization is dynamically performed" on the first use in each thread.[^5] The threads run in `thread::scope`, and each one returns its counter as the result of its closure.
- **C++** declares the counter `thread_local`. For such a variable, the storage "lasts for the duration of the thread in which they are created", and each thread has its own object.[^6] The `std::jthread` objects are in a block, and their destructors join the threads at the end of the block.
- **C** declares the counter `_Thread_local`. C11 added this keyword, and C23 makes `thread_local` a keyword too.[^7] The threads come from `pthread_create`, and each one writes its result to its own struct.
- **Go** has no thread-local storage. Goroutines "expose no unique identifier, name, or data structure to the programmer", so there is nothing to attach a goroutine-local value to.[^8] The solution does what Go code does instead: each goroutine makes its own `Counter` and calls its methods. Code that needs the counter gets it as a value, so the call chain shows where the state goes.
- **JavaScript and TypeScript** have no thread-local variables, because threads share no variables. A `Worker` is "an independent JavaScript execution thread".[^9] The solution starts each worker on the file of its own module, as the Node.js documentation does, where the call "re-loads the current file inside a Worker instance".[^9] So each worker has its own copy of the module and of its `counter` variable. `isMainThread` tells the module whether it runs in a worker.[^9]

### Alternatives

- **`ThreadLocal<T>` in .NET.** It "Provides thread-local storage of data", and its constructor can take a factory for the first value of each thread.[^10] With `trackAllValues`, its `Values` property "Gets a list containing the values stored by all threads that have accessed this instance".[^10] Then the threads do not have to report their counters: the caller reads all of them.
- **Keys in POSIX.** `pthread_key_create` creates "a thread-specific data key visible to all threads in the process".[^11] Each thread stores a pointer under the key with `pthread_setspecific` and reads it with `pthread_getspecific`. A key can have a destructor, which frees the value of a thread when that thread exits.[^11]
- **Cell methods in Rust.** A `LocalKey<Cell<T>>` has `get` and `set` methods. `get` "Returns a copy of the contained value", so `COUNTER.set(COUNTER.get() + 1)` works without `with`.[^5]
- **Async-local storage.** Code that uses `async` and `await` should keep per-task values in `AsyncLocalStorage` in Node.js, which "is similar to thread-local storage in other languages",[^12] or in `contextvars` in Python, which code with state should use "instead of threading.local()".[^13]

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no counts start no threads and give an empty list.
- `zero`: a thread that never calls `bump` returns 0. Its counter starts at 0. It does not start at the value of the caller's counter.
- `one-thread`: one thread gives one counter.
- `equal-counts`: 4 threads count to 100 each. With one shared counter, the threads would see each other's counts, and the results could be as large as 400.
- `many-threads`: 16 threads count from 1 to 16, one count for each thread.
- `large`: 2 threads count to 100,000 at the same time. Each thread changes only its own counter, so the counts do not need a lock or an atomic operation. A shared counter with no lock would be a [[data-races|data race]].

## Pitfalls

- **Thread pools keep the values.** A pool reuses its threads, so a value that one task leaves in a thread-local variable is still there for the next task on that thread. JEP 506 warns that such a value "could, if not properly cleared, accidentally leak into an unrelated task".[^14] The solutions start a new thread for each count, so each counter starts at 0. On a pool, reset the value at the start of each task, or call Java `remove()`, which "Removes the current thread's value for this thread-local variable".[^2]

In Python, a pool of one thread shows the leak. Each task reads the counter and adds 1, and each task after the first sees the count of the tasks before it:

```python run id=pool-keeps-the-value
import threading
from concurrent.futures import ThreadPoolExecutor

local = threading.local()


def task(name):
    seen = getattr(local, "counter", 0)
    local.counter = seen + 1
    return f"{name} starts at {seen}"


with ThreadPoolExecutor(max_workers=1) as pool:
    for line in pool.map(task, ["task 1", "task 2", "task 3"]):
        print(line)
```

```output for=pool-keeps-the-value
task 1 starts at 0
task 2 starts at 1
task 3 starts at 2
```

- **An initializer on `[ThreadStatic]`.** Do not give a `[ThreadStatic]` field an initial value, as in `[ThreadStatic] static int counter = 1;`. That initialization "occurs only once, when the class constructor executes, and therefore affects only one thread".[^3] Use `ThreadLocal<T>` with a factory for a value that is not the default.[^10]
- **Reading the value after the thread ends.** The value belongs to its thread. In C++, its storage lasts only as long as the thread,[^6] and in Java, "after a thread goes away, all of its copies of thread-local instances are subject to garbage collection".[^2] Copy the value to a result while the thread runs, as the solutions do.
- **Reading the value on the wrong thread.** `current()` on the caller returns the counter of the caller, not of a thread that it started. The caller gets the counters of the other threads only through the results.
- **Async code.** One thread can run many async tasks, and one task can continue on another thread after an `await`. Use async-local storage for values that must follow a task.[^12][^13]

## Related tasks

- [[thread-local-storage]] explains the idea, with examples in more languages.
- [[thread-specific-storage]] is the design pattern behind this task.
- [[thread-confinement]] keeps data on one thread, so it needs no lock.
- [[start-a-thread]] and [[wait-for-threads-to-finish]] start and join the threads.
- [[run-tasks-on-a-thread-pool]] runs jobs on threads that a pool reuses, where thread-local values outlive a job.

## References
