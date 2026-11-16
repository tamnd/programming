---
tag: BRRXA9
kind: feature
owner: python
slug: the-queue-module
name: The queue module
aliases: [queue.Queue, LifoQueue, PriorityQueue, SimpleQueue, task_done, producer consumer threads, thread-safe queue, queue.ShutDown]
area: languages/python/concurrency
level: core
summary: >-
  The queue module has thread-safe FIFO, LIFO and priority queues. Producer threads put items, consumer threads get them, and task_done() with join() waits until each item is handled.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/queue.html", locator: "queue, A synchronized queue class"}
see_also: [python/multi-threading, python/thread-synchronization, producer-consumer]
sources:
  - url: https://docs.python.org/3.15/library/queue.html
    title: queue, A synchronized queue class, Python 3.15 documentation
    locator: the module introduction; the classes Queue, LifoQueue, PriorityQueue and SimpleQueue
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/queue.html#queue-objects
    title: queue, A synchronized queue class, Python 3.15 documentation
    locator: Queue Objects; Waiting for task completion
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/queue.html#terminating-queues
    title: queue, A synchronized queue class, Python 3.15 documentation
    locator: Terminating queues
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/stdlib2.html#multi-threading
    title: The Python Tutorial, 11. Brief Tour of the Standard Library, Part II
    locator: 11.4. Multi-threading
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Channel_types
    title: The Go Programming Language Specification
    locator: Channel types
    accessed: 2026-11-16
---

## Synopsis

```python run id=producer-consumer
import queue
import threading

tasks = queue.Queue()
results = queue.Queue()

def worker():
    while True:
        n = tasks.get()
        results.put(n * n)
        tasks.task_done()

for _ in range(3):
    threading.Thread(target=worker, daemon=True).start()

for n in range(10):
    tasks.put(n)
tasks.join()

print(sorted(results.get() for _ in range(10)))
```

```output for=producer-consumer
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```

The main thread puts ten numbers. Three worker threads take them, and `tasks.join()` returns when each number is done.

## Semantics

**r1.** "The queue module implements multi-producer, multi-consumer queues." "The Queue class in this module implements all the required locking semantics."[^1] Threads can share one queue with no lock of their own.

**r2.** The three queue classes differ only in the order of retrieval. "In a FIFO queue, the first tasks added are the first retrieved. In a LIFO queue, the most recently added entry is the first retrieved (operating like a stack). With a priority queue, the entries are kept sorted (using the heapq module) and the lowest valued entry is retrieved first."[^1] Example: [`orders`](#orders).

**r3.** `maxsize` "sets the upperbound limit on the number of items that can be placed in the queue. Insertion will block once this size has been reached, until queue items are consumed." A `maxsize` of zero or less gives a queue of infinite size.[^1]

**r4.** `get()` and `put()` block by default. With `block=False`, or when a timeout ends, `get()` raises `queue.Empty` and `put()` raises `queue.Full`.[^2] Example: [`empty-full`](#empty-full).

**r5.** "For each get() used to fetch a task, a subsequent call to task_done() tells the queue that the processing on the task is complete." `join()` "Blocks until all items in the queue have been gotten and processed."[^2]

**r6.** `qsize()`, `empty()` and `full()` give a snapshot only. "qsize() > 0 doesn’t guarantee that a subsequent get() will not block".[^2]

**r7.** `shutdown()` puts a queue "into a shutdown mode". Then `put()` raises `ShutDown`. With `immediate=False`, consumers can still get the items in the queue, and "Once the queue is empty, future calls to get() will raise ShutDown."[^3] Example: [`shutdown`](#shutdown).

**r8.** `SimpleQueue` is "an unbounded FIFO queue". "Simple queues lack advanced functionality such as task tracking."[^1]

**r9.** The tutorial calls this the preferred design: "concentrate all access to a resource in a single thread and then use the queue module to feed that thread with requests from other threads."[^4]

## Examples

```python run id=orders
import queue

for cls in (queue.Queue, queue.LifoQueue, queue.PriorityQueue):
    q = cls()
    for item in (3, 1, 2):
        q.put(item)
    print(cls.__name__, [q.get() for _ in range(3)])
```

```output for=orders
Queue [3, 1, 2]
LifoQueue [2, 1, 3]
PriorityQueue [1, 2, 3]
```

The same three puts give three orders (rule r2). A priority queue entry is often a tuple `(priority, data)`.[^1]

```python run id=empty-full
import queue

q = queue.Queue(maxsize=1)
q.put("a")
try:
    q.put("b", block=False)
except queue.Full:
    print("full")
print(q.get())
try:
    q.get(timeout=0.05)
except queue.Empty:
    print("empty")
```

```output for=empty-full
full
a
empty
```

```python run id=shutdown
import queue
import threading

q = queue.Queue()

def consumer():
    while True:
        try:
            item = q.get()
        except queue.ShutDown:
            print("consumer stops")
            return
        print("got", item)

t = threading.Thread(target=consumer)
for item in ("x", "y"):
    q.put(item)
q.shutdown()
t.start()
t.join()
```

```output for=shutdown
got x
got y
consumer stops
```

The consumer gets the two items that were in the queue before the shutdown. The next `get()` raises `ShutDown`, and the loop ends (rule r7).

## Versions

> [!since v="3.13"]
> `Queue.shutdown()` and the exception `queue.ShutDown` were added.[^3]

## Pitfalls

`empty()` is not a safe test before `get()`. Another thread can take the item between the test and the call (rule r6). Call `get()` with a timeout, or with `block=False` and a handler for `queue.Empty`.

`task_done()` once too many is an error:

```python fails id=extra-task-done expect=exception:ValueError
import queue

q = queue.Queue()
q.put(1)
q.get()
q.task_done()
q.task_done()
```

`task_done()` "Raises a ValueError if called more times than there were items placed in the queue."[^2] A consumer that forgets to call it makes `join()` wait forever.

## Comparison

A Go channel has the same role: "A channel provides a mechanism for concurrently executing functions to communicate by sending and receiving values of a specified element type."[^5] The capacity of a buffered channel is like `maxsize`. An unbuffered channel has no Python match: there, "communication succeeds only when both a sender and receiver are ready."[^5] Go has no `task_done()` or `join()` on a channel.
