---
tag: KH2W6K
kind: pattern
slug: thread-pool
name: Thread pool
aliases: [thread pool, worker pool, worker-crew model, replicated workers, executor]
area: software-design/concurrency-patterns
level: core
summary: >-
  A thread pool keeps a fixed set of worker threads that take tasks from a queue. The program pays for thread creation once and limits how many tasks run at the same time.
family: concurrency
polarity: positive
intent: Keep a set of threads that wait for tasks, so that a program runs many short tasks concurrently without the cost of creating and destroying a thread for each task.
also_known_as: [Worker pool, Worker-crew model, Replicated workers]
see_also: [object-pool-pattern, half-sync-half-async, leader-followers, active-object, futures, threads, channels, deadlock]
sources:
  - url: https://en.wikipedia.org/wiki/Thread_pool
    title: Thread pool, Wikipedia
    locator: lead; Performance; In languages
  - url: https://doc.rust-lang.org/book/ch21-02-multithreaded.html
    title: The Rust Programming Language, 21.2 From Single-Threaded to Multithreaded Server
    locator: Improving Throughput with a Thread Pool
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/ThreadPoolExecutor.html
    title: ThreadPoolExecutor, Java SE 21 API documentation
    locator: class description; Core and maximum pool sizes; Queuing
  - url: https://docs.python.org/3/library/concurrent.futures.html
    title: concurrent.futures, Launching parallel tasks, Python documentation
    locator: Executor Objects; ThreadPoolExecutor
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Channels
  - url: https://docs.libuv.org/en/v1.x/design.html
    title: Design overview, libuv documentation
    locator: The I/O loop; File I/O
---

## Intent

A thread pool maintains multiple threads that wait for tasks to be allocated for concurrent execution by the supervising program.[^1] The pool avoids the latency of frequent creation and destruction of threads for short-lived tasks, and it can limit the load on the system.[^1]

## Problem

A server that creates a new thread for each request pays for thread creation and destruction on every request. Creating and destroying a thread and its associated resources can be an expensive process in terms of time.[^1] The number of threads also has no upper limit. The Rust book gives the example of a server that someone sends 10 million requests: with a thread for each request, the requests could use up all the server's resources.[^2]

## Solution

Create a fixed number of worker threads once. Requests go to the pool, and the pool keeps a queue of them.[^2] Each worker takes a request from the queue, handles it, and then asks the queue for the next request.[^2] With N workers, up to N requests run at the same time.[^2]

The Java `ThreadPoolExecutor` names the parts of a general pool:[^3]

- The **core pool size** and the **maximum pool size** set the bounds of the number of threads. If they are the same, the pool has a fixed size.[^3]
- The **work queue** is a `BlockingQueue` that holds submitted tasks.[^3]
- When fewer than the core number of threads run, the pool adds a thread for a new task. When the core number or more run, the pool prefers to put the task in the queue.[^3]

```text
submit(task) --> [ task | task | task ]   work queue
                     |      |      |
                 worker1 worker2 worker3   fixed set of threads
                     |      |      |
                   result result result    (for example in a future)
```

In Python, `ThreadPoolExecutor.submit` schedules a callable and returns a `Future` that represents its execution.[^4]

## Example

The example submits 100 tasks to a pool of 4 workers. Each task returns the square of its number and records which worker ran it and how many tasks run at that moment.

```text
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- Thread creation and destruction overhead is restricted to the initial creation of the pool, which may result in better performance and better system stability.[^1]
- The pool bounds and manages the resources, including threads, that a collection of tasks uses.[^3]
- A pool can make it simple to queue work and to control concurrency at a higher level than manual management of threads.[^1]

Liabilities:

- The size must be tuned. An excessive number of threads in reserve wastes memory, and context switches between runnable threads cost performance.[^1]
- An unbounded queue admits the possibility of unbounded work queue growth when tasks arrive on average faster than the pool processes them.[^3]
- A task that waits for another task in the same pool can deadlock. The Python documentation shows a pool with one worker that runs a function which submits a second task and waits for its result: the second task never runs.[^4]

```python wrong id=pool-self-wait
from concurrent.futures import ThreadPoolExecutor, TimeoutError

pool = ThreadPoolExecutor(max_workers=1)


def inner():
    return "inner done"


def outer():
    f = pool.submit(inner)  # queued behind outer: no free worker
    try:
        return f.result(timeout=0.5)
    except TimeoutError:
        return "outer timed out: the only worker waits for itself"


print(pool.submit(outer).result())
pool.shutdown()
```

```output for=pool-self-wait
outer timed out: the only worker waits for itself
```

The example uses a timeout so that it ends. Without the timeout, the program never ends.

## When not to use it

A pool saves work when the program runs many short-lived tasks.[^1] A few threads that live as long as the program gain little from it. Do not put tasks that wait for each other into a small pool. The Java documentation recommends a direct hand-off queue, which creates a new thread when no thread is free, for sets of requests that might have internal dependencies.[^3]

## Variants and related patterns

- **Fixed, cached and single pools.** Java's `Executors` class has factory methods for an unbounded pool with automatic thread reclamation, a fixed-size pool and a single background thread.[^3]
- **Queue strategies.** A `ThreadPoolExecutor` can use a direct hand-off, an unbounded queue or a bounded queue. A bounded queue helps prevent resource exhaustion, but it can be more difficult to tune.[^3]
- **Worker goroutines.** In Go, a server can start a fixed number of goroutines that all read from the request channel. The number of goroutines limits the number of simultaneous calls.[^5]
- **Dynamic size.** A `ThreadPoolExecutor` adjusts its size between the core and maximum bounds. Threads above the core size end when they are idle for longer than the keep-alive time.[^3]

A bounded submission makes the caller wait when the pool has too many tasks. The example limits the pool to 4 tasks that wait or run:

```python run id=bounded-submit
import threading
import time
from concurrent.futures import ThreadPoolExecutor


class BoundedExecutor:
    def __init__(self, workers, limit):
        self.pool = ThreadPoolExecutor(max_workers=workers)
        self.slots = threading.Semaphore(limit)

    def submit(self, fn, *args):
        self.slots.acquire()  # the caller waits when the queue is full
        f = self.pool.submit(fn, *args)
        f.add_done_callback(lambda _: self.slots.release())
        return f


outstanding = 0
most = 0


def task(n):
    time.sleep(0.01)
    return n


ex = BoundedExecutor(workers=2, limit=4)
futures = []
for n in range(20):
    f = ex.submit(task, n)
    outstanding = sum(not g.done() for g in futures) + 1
    most = max(most, outstanding)
    futures.append(f)
ex.pool.shutdown()
print("tasks done:", sum(f.done() for f in futures))
print("tasks queued or running at once, at most 4:", most <= 4)
```

```output for=bounded-submit
tasks done: 20
tasks queued or running at once, at most 4: True
```

[Half-Sync/Half-Async](half-sync-half-async) uses a pool as its synchronous layer. [Leader/Followers](leader-followers) is a pool in which the threads take turns to wait for events, without a queue between them. An [Object Pool](object-pool-pattern) applies the same idea to objects other than threads.

## Known uses

- Java: `ThreadPoolExecutor` and `ScheduledThreadPoolExecutor` provide tunable, flexible thread pools.[^3]
- Python: `concurrent.futures.ThreadPoolExecutor`. The default number of workers is `min(32, (os.process_cpu_count() or 1) + 4)`.[^4]
- libuv, a library that was originally written for Node.js, runs blocking file I/O operations in a thread pool, while network I/O stays on the loop's thread.[^6]
- The Rust book builds a `ThreadPool` type for its web server project.[^2]

## References
