---
tag: JFNA3W
kind: technique
slug: process-based-parallelism
name: Process-based parallelism
aliases: [multiprocessing, worker processes, process pool, prefork, pre-forking server, ProcessPoolExecutor, multiprocessing.Pool, fork and pipe]
area: concurrency-and-parallelism/processes-and-threads
level: core
summary: >-
  Run work in several worker processes instead of threads. Each process has its own memory and interpreter, so CPU-bound work runs in parallel. Data goes through pipes, queues or shared memory.
see_also: [process, threads, global-interpreter-lock, run-tasks-on-a-thread-pool, executors, run-a-subprocess, message-passing-concept, thread-interruption]
sources:
  - url: https://docs.python.org/3/library/multiprocessing.html
    title: The Python Standard Library, multiprocessing, Process-based parallelism
    locator: Introduction; Contexts and start methods; Pipes; Connection Objects; Programming guidelines
  - url: https://httpd.apache.org/docs/2.4/mod/prefork.html
    title: Apache HTTP Server 2.4 documentation, prefork
    locator: Summary; How it Works
  - url: https://docs.python.org/3/library/multiprocessing.shared_memory.html
    title: The Python Standard Library, multiprocessing.shared_memory
    locator: introduction; SharedMemory.close; SharedMemory.unlink
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/fork.html
    title: POSIX.1-2024, fork
    locator: DESCRIPTION
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: The Python Standard Library, concurrent.futures
    locator: ProcessPoolExecutor
  - url: https://pubs.opengroup.org/onlinepubs/9799919799/functions/pipe.html
    title: POSIX.1-2024, pipe
    locator: DESCRIPTION
  - url: https://www.postgresql.org/docs/current/connect-estab.html
    title: PostgreSQL documentation, How Connections Are Established
    locator: section text
  - url: https://nodejs.org/api/cluster.html
    title: Node.js documentation, Cluster
    locator: introduction; How it works
  - url: https://pkg.go.dev/os/exec
    title: Go standard library, package os/exec
    locator: Overview; func (*Cmd) StdinPipe; func (*Cmd) StdoutPipe
  - url: https://nodejs.org/api/child_process.html
    title: Node.js documentation, Child process
    locator: introduction; child_process.fork()
  - url: https://doc.rust-lang.org/std/process/index.html
    title: Rust standard library, module std::process
    locator: module description; Handling I/O
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/ProcessBuilder.html
    title: Java SE 21 API, java.lang.ProcessBuilder
    locator: class description
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.diagnostics.process
    title: Microsoft Learn, Process class
    locator: class description; Examples
---

## Purpose

Threads share one address space, and in some languages they cannot run code in parallel. CPython has a global interpreter lock, so CPU-bound Python threads take turns. Some libraries are not thread-safe at all.

Process-based parallelism runs the work in several worker processes. Each process has its own memory, and in CPython its own interpreter and its own GIL. The `multiprocessing` package works this way, "effectively side-stepping the Global Interpreter Lock by using subprocesses instead of threads", and so it can "fully leverage multiple processors on a given machine".[^1] Servers use the same idea. The Apache prefork module "implements a non-threaded, pre-forking web server", in which "Each server process may answer incoming requests".[^2]

The cost is that the processes do not share objects. Data must go between them through pipes, queues or shared memory.[^1][^3]

## Steps

1. **Split the work into tasks that need little shared data.** The Python guidelines say: "As far as possible one should try to avoid shifting large amounts of data between processes".[^1] Good tasks take a small input, calculate for a long time and return a small result.
2. **Choose how to start the workers.** On POSIX, `fork` makes a child that "shall be an exact copy of the calling process", except for the details that the standard lists.[^4] Python has three start methods. With `spawn`, "The parent process starts a fresh Python interpreter process". With `fork`, the child "is effectively identical to the parent process". With `forkserver`, a server process forks each new worker.[^1] `spawn` is "rather slow compared to using fork or forkserver", but the child inherits only the resources that it needs.[^1]
3. **Start a pool of workers.** A pool starts the processes once and gives them many tasks. Python's `Pool` offers "a convenient means of parallelizing the execution of a function across multiple input values, distributing the input data across processes".[^1] `ProcessPoolExecutor` "uses a pool of processes to execute calls asynchronously".[^5] A server keeps spare workers ready, so that "clients do not need to wait for a new child processes to be forked before their requests can be served".[^2]
4. **Send the inputs and get the results as messages.** A POSIX pipe has two ends. Data written to one end, `fildes[1]`, can be read from the other end, `fildes[0]`, "on a first-in-first-out basis".[^6] Python's `Pipe()` returns two connection objects, and they send and receive "picklable objects or strings".[^1] The guidelines suggest "queues or pipes for communication between processes rather than using the lower level synchronization primitives".[^1]
5. **Put large data in shared memory, if copies cost too much.** Processes "are conventionally limited to only have access to their own process memory space", but shared memory "permits the sharing of data between processes".[^3] Give each worker its own part of the block, or protect the block with a lock.
6. **Wait for the workers and clean up.** Join each process and check its exit code. Close the pool. Free shared memory: `close()` ends the access of one process, and `unlink()` deletes the block.[^3]

## Example

In Python. The program does the three common things with worker processes. A pool of 4 workers runs a CPU-bound function on 4 inputs, and the workers have process IDs that are not the ID of the parent. A child process sends words to the parent through a pipe. Two children write the two halves of a shared memory block, and the parent adds up the bytes.

```python run id=python-process-workers
import multiprocessing as mp
import os
from multiprocessing import shared_memory


def sum_of_squares(n):
    # A CPU-bound task. Each call runs in a worker process.
    return sum(i * i for i in range(n))


def worker_pid(_):
    return os.getpid()


def send_words(conn):
    for word in ["alpha", "beta", "gamma"]:
        conn.send(word)
    conn.send(None)  # tells the reader that no more words come
    conn.close()


def fill(name, start, stop):
    # Attach to the block by name. Each worker writes only its own part.
    shm = shared_memory.SharedMemory(name=name)
    for i in range(start, stop):
        shm.buf[i] = i % 10
    shm.close()


if __name__ == "__main__":
    # 1. A pool of 4 worker processes runs a function on many inputs.
    with mp.Pool(4) as pool:
        print("pool results:", pool.map(sum_of_squares, [10, 100, 1000, 10000]))
        pids = set(pool.map(worker_pid, range(20)))
    print("the workers are other processes:", os.getpid() not in pids)

    # 2. A pipe carries pickled objects from a child process to the parent.
    reader, writer = mp.Pipe(duplex=False)
    child = mp.Process(target=send_words, args=(writer,))
    child.start()
    writer.close()  # the parent keeps only the read end
    words = []
    while (word := reader.recv()) is not None:
        words.append(word)
    child.join()
    print("from the pipe:", words, "exit code:", child.exitcode)

    # 3. Two processes write into one block of shared memory, with no copy.
    shm = shared_memory.SharedMemory(create=True, size=1000)
    try:
        halves = [mp.Process(target=fill, args=(shm.name, 0, 500)),
                  mp.Process(target=fill, args=(shm.name, 500, 1000))]
        for p in halves:
            p.start()
        for p in halves:
            p.join()
        print("sum of the shared bytes:", sum(shm.buf))
    finally:
        shm.close()
        shm.unlink()  # frees the block; only one process calls this
```

```output
pool results: [285, 328350, 332833500, 333283335000]
the workers are other processes: True
from the pipe: ['alpha', 'beta', 'gamma'] exit code: 0
sum of the shared bytes: 4500
```

The parent closes its copy of the write end of the pipe. Then only the child holds the write end. The child sends `None` at the end, so the loop in the parent knows when to stop.

A global variable is not shared. Each child changes its own copy, so the parent still sees 0:

```python wrong id=python-process-global
import multiprocessing as mp

counter = 0


def add_one():
    global counter
    counter += 1  # changes the copy in the child process only


if __name__ == "__main__":
    workers = [mp.Process(target=add_one) for _ in range(4)]
    for w in workers:
        w.start()
    for w in workers:
        w.join()
    print("counter in the parent:", counter)
```

```output
counter in the parent: 0
```

## When to use

Use worker processes for CPU-bound work in a language whose threads cannot run in parallel, such as Python with the GIL.[^1][^5] Use them also when the work must be isolated. Apache says that prefork "is appropriate for sites that need to avoid threading for compatibility with non-thread-safe libraries", and that it is "the best MPM for isolating each request, so that a problem with a single request will not affect any other".[^2] PostgreSQL uses a process for each connection: a supervisor process "spawns a new backend process every time a connection is requested", and the backends share data through "semaphores and shared memory".[^7]

Do not use processes when threads can do the job and the tasks share much data. Node.js gives this advice for its cluster module: "When process isolation is not needed, use the worker_threads module instead".[^8] Each process also costs memory. Apache says that `MaxRequestWorkers` must be "small enough to assure that there is enough physical RAM for all processes".[^2]

## Pitfalls

- **Global variables.** A child does not share the globals of the parent. The value that a child sees "may not be the same as the value in the parent process at the time that Process.start was called".[^1] The second example shows this. Return results as messages instead.
- **Objects that cannot be pickled.** Python sends arguments and results between processes as pickles, so `ProcessPoolExecutor` can use "only picklable objects".[^5]
- **The main module.** With `spawn` and `forkserver`, each worker imports the main module again. Protect the entry point with `if __name__ == '__main__':`, or the program can fail with a `RuntimeError`.[^1] For the same reason, `ProcessPoolExecutor` "will not work in the interactive interpreter".[^5]
- **Fork in a program with threads.** The child of `fork` has only one thread, "a replica of the calling thread", but it has a copy of the whole address space, "possibly including the states of mutexes".[^4] A lock that another thread held stays locked in the child. So "the application shall ensure that the child process only executes async-signal-safe operations" until it calls `exec`.[^4] Python says that "safely forking a multithreaded process is problematic".[^1]
- **A new default start method.** Python 3.14 made `forkserver` the default start method on POSIX platforms, and `fork` is "no longer the default start method on any platform". Code that needs `fork` "must explicitly specify that via get_context() or set_start_method()".[^1]
- **Shared memory that is never freed.** "Depending on operating system, the underlying memory may or may not be freed even if all handles to it have been closed". Call `unlink()` once, as the example does in `finally`.[^3]
- **Pipes that fill up.** A child that writes much output blocks when the pipe is full, and a parent that waits for the child before it reads the pipe then waits forever. Go says: "It is thus incorrect to call Wait before all reads from the pipe have completed".[^9]

## Tools that do it

| Language | Start a worker process | A pool or a group of workers | Talk to the workers |
|---|---|---|---|
| Python | `multiprocessing.Process` | `multiprocessing.Pool`, `ProcessPoolExecutor` | `Pipe`, `Queue`, `shared_memory`[^1][^5][^3] |
| C (POSIX) | `fork`, then `exec` | none | `pipe`[^4][^6] |
| Node.js | `child_process.fork()` | the `cluster` module | an IPC channel with `subprocess.send()`[^10][^8] |
| Go | `exec.Command` | none | `StdinPipe`, `StdoutPipe`[^9] |
| Rust | `std::process::Command` | none | standard streams with `Stdio`[^11] |
| Java | `ProcessBuilder.start()` | none | the process streams; `startPipeline`[^12] |
| C# | `Process.Start` | none | redirected standard streams[^13] |

- **Node.js.** `child_process.fork()` is "a special case of child_process.spawn() used specifically to spawn new Node.js processes", with a built-in channel "that allows messages to be passed back and forth between the parent and child".[^10] The cluster module spawns its workers with `child_process.fork()`, "so that they can communicate with the parent via IPC and pass server handles back and forth".[^8]
- **Go.** Package `os/exec` "runs external commands". It "intentionally does not invoke the system shell".[^9]
- **Rust.** The `std::process` module "is mostly concerned with spawning and interacting with child processes".[^11]
- **Java.** `ProcessBuilder` "is used to create operating system processes". `startPipeline` makes "a pipeline of new processes that send the output of each process directly to the next process".[^12]
- **C#.** The `Process` class "Provides access to local and remote processes and enables you to start and stop local system processes".[^13]

## References
