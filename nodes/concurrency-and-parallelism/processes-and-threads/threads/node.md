---
tag: GTTVPE
kind: concept
slug: threads
name: Thread
aliases: [thread of execution, thread of control, OS thread]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: >-
  A thread is one sequence of execution inside a process. The threads of a process share its memory, and each thread has its own stack, so several threads can run at the same time.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1966
  people: [Jerome H. Saltzer, Victor A. Vyssotsky]
  note: Saltzer's thesis defined a process as a program in execution and gave "thread", a term suggested by Vyssotsky, as an alternative name.
see_also: [data-races, mutual-exclusion, deadlock, channels, async-await, closures]
same_as:
  wikidata: Q213092
  wikipedia: Thread_(computing)
sources:
  - url: http://web.mit.edu/Saltzer/www/publications/MIT-MAC-TR-030.ocr.pdf
    title: Jerome H. Saltzer, Traffic Control in a Multiplexed Computer System, MIT Project MAC Technical Report TR-30, 1966
    locator: Section One, The concept of process, page 20, footnote 1
  - url: https://go.dev/ref/spec#Go_statements
    title: The Go Programming Language Specification
    locator: Go statements
  - url: https://docs.python.org/3.14/library/threading.html#thread-objects
    title: The Python Standard Library, threading, Python 3.14
    locator: the CPython implementation detail on the GIL; GIL and performance considerations; Thread objects (join, daemon); threading.excepthook
  - url: https://learn.microsoft.com/en-us/windows/win32/procthread/thread-stack-size
    title: Microsoft Learn, Win32 apps, Thread Stack Size
    locator: the first two paragraphs (reserved and committed memory, default size of 1 MB)
  - url: https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf
    title: ISO/IEC 9899 working draft N3220 (the C23 draft)
    locator: 5.1.2.5 Multi-threaded executions and data races, paragraph 1; 6.10.10.4 Conditional feature macros (__STDC_NO_THREADS__); 7.28.1 Introduction to threads, paragraph 2
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_create.html
    title: POSIX.1-2024, pthread_create
    locator: DESCRIPTION; CHANGE HISTORY (first released in Issue 5)
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pthread_join.html
    title: POSIX.1-2024, pthread_join
    locator: DESCRIPTION; RATIONALE
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.thread.member
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.thread.member], paragraphs 3, 4 and 8"
  - url: https://man7.org/linux/man-pages/man3/pthread_create.3.html
    title: Linux manual page, pthread_create(3)
    locator: NOTES (joinable by default; default stack size under NPTL)
  - url: https://doc.rust-lang.org/std/thread/index.html
    title: "The Rust Standard Library, Module std::thread"
    locator: The threading model; Spawning a thread; Stack size
  - url: https://timsong-cpp.github.io/cppwp/n4950/futures.async
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[futures.async], paragraph 1"
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/lang/Thread.html
    title: Java SE 25 API, java.lang.Thread
    locator: Platform threads; Virtual threads; join()
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.tasks.task.run
    title: .NET API reference, Task.Run Method
    locator: the summary of the method
  - url: https://nodejs.org/api/worker_threads.html
    title: Node.js documentation, Worker threads
    locator: the introduction; Class Worker
  - url: https://docs.python.org/3.14/library/concurrent.futures.html#threadpoolexecutor
    title: The Python Standard Library, concurrent.futures, Python 3.14
    locator: ThreadPoolExecutor (default max_workers); Future.result
  - url: https://go.dev/ref/spec#Program_execution
    title: The Go Programming Language Specification
    locator: Program execution
  - url: https://learn.microsoft.com/en-us/dotnet/standard/threading/foreground-and-background-threads
    title: .NET documentation, Foreground and background threads
    locator: the first two paragraphs
  - url: https://www.akkadia.org/drepper/nptl-design.pdf
    title: Ulrich Drepper and Ingo Molnar, The Native POSIX Thread Library for Linux, revision of 21 February 2005
    locator: The First Implementation; 1-on-1 vs. M-on-N
  - url: https://openjdk.org/jeps/444
    title: "JEP 444: Virtual Threads"
    locator: "Motivation; Description (virtual and platform threads, M:N scheduling); Do not pool virtual threads; Scheduling virtual threads; Pinning"
  - url: https://go.dev/doc/faq#goroutines
    title: Go FAQ
    locator: Why goroutines instead of threads?
  - url: https://openjdk.org/jeps/491
    title: "JEP 491: Synchronize Virtual Threads without Pinning"
    locator: Summary
  - url: https://docs.python.org/3.14/glossary.html#term-global-interpreter-lock
    title: Python glossary, Python 3.14
    locator: global interpreter lock; free threading; free-threaded build
  - url: https://peps.python.org/pep-0703/
    title: "PEP 703: Making the Global Interpreter Lock Optional in CPython"
    locator: Abstract; Reference counting (biased, immortalization, deferred); Python critical sections; mimalloc
  - url: https://docs.python.org/3.14/howto/free-threading-python.html#identifying-free-threaded-python
    title: Python HOWTOs, Python support for free threading, Python 3.14
    locator: Identifying free-threaded Python; The global interpreter lock in free-threaded Python; Single-threaded performance
  - url: https://docs.python.org/3.14/library/sys.html#sys._is_gil_enabled
    title: The Python Standard Library, sys, Python 3.14
    locator: sys._is_gil_enabled
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_09_03_02
    title: POSIX.1-2024, Shell Command Language
    locator: 2.9.3.1 Asynchronous AND-OR Lists
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/foreach-object
    title: PowerShell documentation, ForEach-Object
    locator: Description (Parallel running script block); Notes (runspace pool size, separate process threads)
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.concurrent/thread.html
    title: Kotlin standard library, kotlin.concurrent.thread
    locator: the signature and the parameters
  - url: https://doc.rust-lang.org/std/thread/fn.scope.html
    title: "The Rust Standard Library, Function std::thread::scope"
    locator: the description and the section on panics
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.jthread.cons
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.jthread.cons], paragraph 11"
  - url: https://timsong-cpp.github.io/cppwp/n4950/thread.thread.destr
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[thread.thread.destr], paragraph 1"
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.threading.thread.join
    title: .NET API reference, Thread.Join Method
    locator: Join(), Join(TimeSpan) and their return values
  - url: https://go.dev/doc/faq#history
    title: Go FAQ
    locator: What is the history of the project?
  - url: https://go.dev/doc/go1.4#runtime
    title: Go 1.4 Release Notes
    locator: Changes to the runtime
  - url: https://go.dev/doc/devel/release#go1.4
    title: Go release history
    locator: go1.4
  - url: https://openjdk.org/projects/jdk/21/
    title: OpenJDK, JDK 21
    locator: the first paragraph and the schedule
  - url: https://openjdk.org/projects/jdk/24/
    title: OpenJDK, JDK 24
    locator: the first paragraph
  - url: https://peps.python.org/pep-0779/
    title: "PEP 779: Criteria for supported status for free-threaded Python"
    locator: the header (Created, Resolution); Abstract
  - url: https://docs.python.org/3.14/whatsnew/3.14.html#whatsnew314-free-threaded-now-supported
    title: What's new in Python 3.14
    locator: Free-threaded Python is officially supported
---

## Idea

A process is a program in execution, with its own memory.[^1] A thread is one sequence of execution inside a process, a thread of control. A process starts with one thread, and that thread can start more. All threads of a process share its address space: the heap, the global variables and the open files.[^2][^3] Each thread has its own stack and its own position in the code.[^4] The C standard says that a program can have more than one thread of execution running concurrently.[^5]

Threads have two main uses. On a computer with several cores, threads can compute parts of one job in parallel. On any computer, one thread can wait for a disk, a network or a user while other threads keep working. In Python, the module `threading` starts OS threads. Here, three threads each fill their own slot of a list. The main thread waits for all of them with `join()`, and then it reads the list.

```python run id=idea
import threading


def square(results, index):
    results[index] = index * index


results = [None] * 3
workers = [threading.Thread(target=square, args=(results, i)) for i in range(3)]
for worker in workers:
    worker.start()
print("started:", len(workers))
for worker in workers:
    worker.join()
print("results:", results)
print("alive after join:", any(w.is_alive() for w in workers))
```

```output for=idea
started: 3
results: [0, 1, 4]
alive after join: False
```

The threads run in an order that the program does not control. The main thread reads `results` only after each `join()` has returned, so the output is the same on every run. Threads that change shared data at the same time cause [[data-races]]. A lock gives [[mutual-exclusion]], and a cycle of locks can cause a [[deadlock]]. Threads can also send each other values through [[channels]] and share no data at all.

## How it works

### Start, join and detach

A program starts a thread with a function to run and, often, an argument. The POSIX function `pthread_create` makes a thread that runs `start_routine` with one `void *` argument.[^6] The thread ends when the function returns. Another thread can then join it. `pthread_join` suspends the caller until the target thread ends, and it stores the value that the thread returned.[^7]

Join does two jobs. It waits, and it makes the work of the thread visible to the joiner. In C++, the end of the thread synchronizes with the return of `join()`.[^8] Every write that the thread made before it ended is visible after `join()`. This is why the examples on this page can read the results without a lock.

A thread that nobody will join is detached. Its resources go back to the system when it ends. POSIX says that an application must join or detach each joinable thread, because otherwise the system cannot reclaim the storage of the thread.[^7] On Linux, a thread is joinable by default.[^9] In C++, `detach()` lets the thread run on with no handle.[^8] In Rust, dropping the `JoinHandle` detaches the thread.[^10]

Languages return the result of a thread in different ways.

| Language | Start | Wait | Result of the thread |
|---|---|---|---|
| C | `pthread_create` | `pthread_join` | a `void *` that the thread returns[^7] |
| C++ | `std::thread`, `std::jthread` | `join()` | none; `std::async` returns a `std::future`[^11] |
| Rust | `thread::spawn` | `JoinHandle::join` | `Result<T>`, an `Err` when the thread panicked[^10] |
| Java | `Thread.ofPlatform().start`, `Thread.ofVirtual().start` | `join()` | none; `join()` returns `void`[^12] |
| Python | `Thread(target=f).start()` | `join()` | none; `join()` returns `None`[^3] |
| C# | `new Thread(f).Start()` | `Join()` | none; `Task.Run` returns a `Task<TResult>`[^13] |
| Go | `go f()` | `sync.WaitGroup` | none; the results of `f` are discarded[^2] |
| JavaScript | `new Worker` | the `exit` event | a message from `postMessage`[^14] |

When no result comes back through the join, each thread writes to its own location, such as its own element of an array. A future is a handle that holds the result. In Python, `Future.result()` waits for the call and returns its value.[^15]

### Daemon threads and the end of the program

The rules for the end of a program differ, and they decide whether a running thread finishes its work.

- In Go, the program exits when `main` returns. It does not wait for other goroutines.[^16]
- In Rust, the program shuts down when the main thread ends, even if other threads are still running.[^10]
- In Java, the shutdown sequence begins when all started non-daemon threads have ended. Virtual threads are always daemon threads.[^12]
- In Python, the interpreter waits for non-daemon threads. Daemon threads stop abruptly at shutdown, and their resources may not be released.[^3]
- In C#, a background thread does not keep the process alive. At the end of the process, the runtime stops background threads with no exception.[^17]

### One to one, many to one, many to many

A thread that the program sees must run on a thread of the operating system kernel. Three mappings exist.

```text
1:1                   M:1                    M:N
T1   T2   T3          T1   T2   T3           T1  T2  T3  T4  T5  T6
|    |    |             \   |   /              \  |  /     \  |  /
K1   K2   K3               K1                    K1          K2

T = a thread of the program        K = a kernel thread
```

In the 1:1 model, each program thread is one kernel thread. The POSIX thread libraries of Linux use it.[^18] A Rust program is a collection of native OS threads.[^10] Java platform threads are typically mapped 1:1 to kernel threads.[^12] The kernel schedules each thread, so a thread that blocks in a system call stops only itself.

In the M:1 model, a runtime library runs all program threads on one kernel thread. These are often called green threads. Early versions of Java had green threads that shared one OS thread, and platform threads later outperformed them.[^19] A pure user-level implementation cannot use more than one processor.[^18]

In the M:N model, a runtime schedules many program threads onto a smaller number of kernel threads. Java virtual threads use this model.[^19] The Go runtime multiplexes goroutines onto a set of OS threads. When a goroutine blocks in a system call, the runtime moves the other goroutines of that OS thread to another runnable thread.[^20]

M:N needs two schedulers, one in the runtime and one in the kernel. The designers of the Native POSIX Thread Library for Linux considered M:N. They wrote that the two schedulers must collaborate, for example through scheduler activations, or performance suffers. The kernel developers chose 1:1 for Linux.[^18]

### Virtual threads in Java

A virtual thread is a `java.lang.Thread` that the JDK schedules, not the OS. The JDK scheduler is a work-stealing `ForkJoinPool`, and by default its parallelism is the number of available processors. The scheduler mounts a virtual thread on a platform thread, its carrier, to run it. When the virtual thread blocks, for example in I/O, the JDK unmounts it and the carrier runs another virtual thread.[^19]

The scheduler does not use time sharing for virtual threads. It does not preempt a virtual thread that has used a fixed quantity of CPU time.[^19] A virtual thread is pinned when it cannot unmount while it blocks. Then it holds its carrier, for example during a call to native code.[^19] JEP 491 removed nearly all cases of pinning inside `synchronized` code.[^21]

### The global interpreter lock in Python

The default build of CPython has a global interpreter lock (GIL). The GIL makes sure that only one thread executes Python bytecode at a time.[^22] Python threads are still OS threads, and the GIL is always released during I/O.[^22] The `threading` documentation therefore recommends threads for I/O-bound tasks, and processes for CPU-bound work.[^3]

PEP 703 made the GIL optional.[^23] A free-threaded build of CPython is configured with `--disable-gil` before compilation.[^22] To work without the GIL, the free-threaded build uses biased reference counting, immortal objects, deferred reference counting and per-object locks. It also uses the mimalloc allocator.[^23]

A program can ask which build runs it. In a free-threaded build, the output of `python -VV` contains "free-threading build", and `sysconfig.get_config_var("Py_GIL_DISABLED")` is `1`. The function `sys._is_gil_enabled()` tells whether the GIL is active at run time.[^24][^25] A free-threaded build can enable the GIL again. The environment variable `PYTHON_GIL` and the option `-X gil` do this. The interpreter also enables the GIL, with a warning, when it imports a C extension module that is not marked as safe without it.[^24] This run uses the default build.

```python run id=gil
import sys
import sysconfig

print("free-threaded build:", sysconfig.get_config_var("Py_GIL_DISABLED") == 1)
print("GIL enabled now:", sys._is_gil_enabled())
```

```output for=gil
free-threaded build: False
GIL enabled now: True
```

## In languages

The example files show four threads that each sum a quarter of the numbers 0 to 999, and a join of all four. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | stdlib | `threading.Thread`; one thread at a time runs bytecode in the default build |
| JavaScript | stdlib | `node:worker_threads` in Node.js; each worker has its own event loop |
| TypeScript | stdlib | as JavaScript |
| Java | stdlib | `java.lang.Thread`: platform threads (1:1) and virtual threads (M:N) |
| C | stdlib | POSIX threads; ISO C `<threads.h>` is optional |
| C++ | stdlib | `std::thread`, `std::jthread` |
| C# | stdlib | `System.Threading.Thread`; the thread pool through `Task.Run` |
| Go | native | the `go` statement; goroutines (M:N) |
| Rust | stdlib | `std::thread::spawn`, `std::thread::scope` |
| Kotlin | stdlib | `kotlin.concurrent.thread` over `java.lang.Thread` |
| PowerShell | stdlib | `ForEach-Object -Parallel` on a pool of runspaces |
| Bash | absent | background processes with `&` and `wait` |
| SQL | n/a | the database engine decides how many threads run a query |
| Regex | n/a | a pattern has no threads of control |
| HTML/CSS | n/a | markup and style rules have no threads of control |

ISO C defines threads in `<threads.h>`, but the header is optional. An implementation that does not provide it defines the macro `__STDC_NO_THREADS__`.[^5] The C library of macOS has no `<threads.h>`, so the C example uses POSIX threads.

JavaScript has no threads that share a heap. A Node.js worker is an independent JavaScript thread with its own event loop, and it exchanges messages with the main thread.[^14] Workers help with CPU-bound work. For I/O, [[async-await]] on one thread is the usual form.

In Bash, a command that ends with `&` runs asynchronously in a subshell, and the shell does not wait for it.[^26] The subshell is a separate process, not a thread. PowerShell runs each script block of `ForEach-Object -Parallel` on its own thread, in a runspace from a pool of five by default.[^27] The Kotlin function `thread` makes and starts a `java.lang.Thread`, with named arguments for the name and the daemon flag.[^28]

## Variations and related ideas

- **Processes.** A process has its own address space, so its threads do not share memory with other processes. Python's `multiprocessing` module and `ProcessPoolExecutor` use separate processes to run CPU-bound Python code in parallel in the default build.[^3] Bash runs background work as processes.[^26]
- **Thread pools.** A pool keeps a set of threads and gives them tasks from a queue. It saves the cost of a fresh thread for each task. Python's `ThreadPoolExecutor` uses `min(32, os.cpu_count() + 4)` workers by default.[^15] C# `Task.Run` queues work to the thread pool.[^13] Java virtual threads should not be pooled. JEP 444 recommends a semaphore to limit concurrency instead.[^19]
- **Scoped threads.** A scope joins all of its threads before it returns, so no thread outlives the data that it uses. Rust's `thread::scope` lets the threads borrow local variables, and a panic in a thread makes the scope panic.[^29] The destructor of a C++ `std::jthread` asks the thread to stop and then joins it.[^30]
- **Coroutines and async functions.** An async function suspends without a thread of its own, and many of them share one thread. [[async-await]] describes them. Java virtual threads keep the blocking thread-per-request style and still scale to large numbers of threads.[^19]
- **Workers with no shared memory.** JavaScript workers exchange messages, and a `SharedArrayBuffer` lets them share memory.[^14] Message passing in this style is the subject of [[channels]].
- **Closures as thread bodies.** Most languages start a thread with a closure. The closure captures the data of the thread, and the capture rules of [[closures]] decide what the thread may touch. In Rust, a `move` closure gives its data to the thread (see the Rust example).

## Pitfalls

### The program ends before the thread does

When `main` returns, a Go program exits at once, and the remaining goroutines stop.[^16] Here the goroutine needs 100 ms, and `main` does not wait for it.

```go wrong id=no-wait
package main

import (
	"fmt"
	"time"
)

func main() {
	go func() {
		time.Sleep(100 * time.Millisecond)
		fmt.Println("worker done")
	}()
	fmt.Println("main done")
}
```

```output for=no-wait
main done
```

The fix is to wait. A `sync.WaitGroup` counts the goroutines, and `Wait` returns after each has called `Done`.

```go run id=wait
package main

import (
	"fmt"
	"sync"
	"time"
)

func main() {
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		time.Sleep(100 * time.Millisecond)
		fmt.Println("worker done")
	}()
	wg.Wait()
	fmt.Println("main done")
}
```

```output for=wait
worker done
main done
```

The same trap exists for daemon threads in Python and Java and for background threads in C#. These threads stop at the end of the program, in the middle of their work.[^3][^12][^17] Join a thread that writes a file or sends a message.

### A joinable `std::thread` is destroyed

If a `std::thread` is still joinable when its destructor runs, the destructor calls `std::terminate`.[^31] Here the function returns, and the thread object goes out of scope without `join()` or `detach()`.

```cpp fails expect=runtime id=terminate
#include <print>
#include <thread>

void start_worker() {
    std::thread worker([] { std::println("working"); });
}

int main() {
    start_worker();
    std::println("after start_worker");
}
```

The C++ runtime aborts the program, and no line after the call runs. The fix is `std::jthread`, whose destructor joins the thread.[^30] A call of `join()` before the end of the scope also works.

```cpp run id=jthread
#include <print>
#include <thread>

void start_worker() {
    std::jthread worker([] { std::println("working"); });
}

int main() {
    start_worker();
    std::println("after start_worker");
}
```

```output for=jthread
working
after start_worker
```

### An exception in a thread does not reach the caller

In Python, an exception that escapes the function of a `Thread` goes to `threading.excepthook`. The default hook prints it on `sys.stderr`.[^3] The caller of `join()` sees no error, and the result of that thread is missing.

```python wrong id=lost-error
import threading

results = {}


def parse(text):
    results[text] = int(text)


threading.excepthook = lambda args: None
workers = [threading.Thread(target=parse, args=(t,)) for t in ["1", "x", "3"]]
for worker in workers:
    worker.start()
for worker in workers:
    worker.join()
print(sorted(results.values()))
```

```output for=lost-error
[1, 3]
```

The program silences the hook here, so the effect is clear: the run ends normally with one value missing. With the default hook, the traceback goes to stderr, and the program still ends normally. The fix is a future. `Future.result()` raises the same exception in the thread that asks for the result.[^15]

```python run id=future-error
from concurrent.futures import ThreadPoolExecutor

with ThreadPoolExecutor(max_workers=3) as pool:
    futures = [pool.submit(int, t) for t in ["1", "x", "3"]]
    for future in futures:
        try:
            print(future.result())
        except ValueError as error:
            print("error:", error)
```

```output for=future-error
1
error: invalid literal for int() with base 10: 'x'
3
```

In Rust, `join()` returns an `Err` when the thread panicked.[^10]

### A join with a timeout does not stop the thread

A join with a timeout only stops the wait. The thread keeps running. In Python, `join(timeout)` always returns `None`, so the caller must call `is_alive()` to know whether the thread ended.[^3]

```python wrong id=timeout
import threading
import time

worker = threading.Thread(target=time.sleep, args=(0.5,))
worker.start()
result = worker.join(timeout=0.05)
print("join returned:", result)
print("finished:", result is None)
worker.join()
```

```output for=timeout
join returned: None
finished: True
```

The code treats `None` as success, but the thread is still asleep. The fix is to ask the thread.

```python run id=timeout-fix
import threading
import time

worker = threading.Thread(target=time.sleep, args=(0.5,))
worker.start()
worker.join(timeout=0.05)
print("finished:", not worker.is_alive())
worker.join()
print("finished:", not worker.is_alive())
```

```output for=timeout-fix
finished: False
finished: True
```

In C#, `Join(TimeSpan)` returns `false` when the time runs out (see the C# example).[^32] To stop a thread, the program must tell it to stop, for example with a flag or a `threading.Event` that the thread checks.

### The thread uses data that can disappear

A spawned thread may outlive the thread that spawned it.[^10] Data on the stack of the spawning function can disappear while the spawned thread still uses it. Rust rejects such a program. Here the closure borrows `numbers`, and the compiler stops with error E0373.

```rust fails expect=compile id=spawn-borrow
use std::thread;

fn main() {
    let numbers = vec![1, 2, 3];
    let handle = thread::spawn(|| numbers.iter().sum::<i32>());
    println!("{}", handle.join().unwrap());
}
```

There are two fixes. A `move` closure gives the vector to the thread. A scoped thread may borrow it, because `thread::scope` joins the thread before `numbers` goes away.[^29]

```rust run id=scope
use std::thread;

fn main() {
    let numbers = vec![1, 2, 3];
    let total = thread::scope(|s| s.spawn(|| numbers.iter().sum::<i32>()).join().unwrap());
    println!("{total} from {numbers:?}");
}
```

```output for=scope
6 from [1, 2, 3]
```

In C, the same mistake compiles with no error. The argument of `pthread_create` is often a pointer to a local variable. That memory must live until the thread no longer uses it (see the C example).

## Cost

- **Stack memory.** Each OS thread gets its own stack. On Windows, the default stack reserves 1 MB of address space, and the committed pages use no physical memory until the program touches them.[^4] On Linux with NPTL, the default stack size is the soft limit for the stack resource. If that limit is unlimited, the size is 2 MB on most architectures.[^9] The Rust documentation gives 2 MiB as the default for spawned threads, and `Builder::stack_size` changes it.[^10] A goroutine starts with a stack of a few kilobytes, which grows and shrinks as needed.[^20]
- **Creation and count.** OS threads are costly, so a program cannot have very many of them. JEP 444 calls platform threads heavyweight and expensive. It says that a program that tries to make 10,000 platform threads at once might crash, depending on the machine and the OS.[^19] The Java example runs 10,000 virtual threads that each sleep, and the JDK runs them on a small number of carriers.
- **Blocking.** A blocked platform thread holds its OS thread. A blocked virtual thread unmounts and frees its carrier, unless it is pinned.[^19] For many tasks that wait on I/O, virtual threads, goroutines or async functions use fewer OS threads.
- **Virtual threads are not faster.** A virtual thread runs code at the same speed as a platform thread. Virtual threads give scale (more threads), not speed.[^19]
- **The GIL.** In the default build of CPython, CPU-bound threads do not run Python code in parallel.[^3] The free-threaded build removes this limit, but single-threaded code runs slower. On the pyperformance suite, the average overhead is about 1% on macOS on ARM64 and 8% on x86-64 Linux.[^24]
- **Pools.** A pool pays the cost of creation once for each worker, not once for each task. A task that waits for the result of another task in the same pool can deadlock, for example when all workers wait.[^15] [[deadlock]] describes the general case.

## History

- **1966.** Jerome Saltzer's thesis on traffic control in a multiplexed computer defined a process as a program in execution. A footnote gives "thread" as an alternative term, suggested by Victor Vyssotsky.[^1]
- **1996.** LinuxThreads, the first POSIX thread library for Linux, was written. It handled each user-level thread with one kernel thread.[^18]
- **2005.** Ulrich Drepper and Ingo Molnar revised their design paper of the Native POSIX Thread Library on 21 February 2005. The paper explains the 1:1 model of Linux threads.[^18]
- **2009.** Go became a public open source project on 10 November 2009.[^33] Its `go` statement starts a goroutine, an independent thread of control in the same address space.[^2]
- **2014.** Go 1.4, released on 10 December 2014, reduced the starting stack of a goroutine from 8192 bytes to 2048 bytes.[^34][^35]
- **2023.** JDK 21 reached general availability on 19 September 2023 with virtual threads (JEP 444).[^36][^19] PEP 703, by Sam Gross, proposed a build of CPython without the GIL.[^23]
- **2025.** JDK 24, generally available on 18 March 2025, stopped `synchronized` from pinning virtual threads in nearly all cases (JEP 491).[^37][^21] PEP 779, accepted on 16 June 2025, made the free-threaded build a supported build of Python 3.14.[^38][^39]

## References
