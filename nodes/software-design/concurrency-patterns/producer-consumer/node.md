---
tag: RXGQZP
kind: pattern
slug: producer-consumer
name: Producer-consumer
aliases: [producer-consumer, producer-consumer problem, bounded buffer, bounded-buffer problem]
area: software-design/concurrency-patterns
level: core
summary: >-
  Producers put items into a shared buffer and consumers take them out. A bounded buffer makes a producer wait when it is full and a consumer wait when it is empty, so neither side runs too far ahead.
family: concurrency
polarity: positive
intent: Join threads that make data to threads that use it through a buffer, so that each side runs at its own speed and a bounded buffer limits how far the producers get ahead.
also_known_as: [Bounded buffer]
history:
  year: 1965
  people: [Edsger W. Dijkstra]
  note: Dijkstra described a producer and a consumer coupled by a buffer, first with unbounded capacity and then with a buffer of N portions, in Cooperating sequential processes (EWD123).
see_also: [thread-pool, monitor-object, half-sync-half-async, active-object, channels, message-passing-concept, mutual-exclusion, deadlock]
sources:
  - url: https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
    title: Producer-consumer problem, Wikipedia
    locator: lead; Dijkstra's bounded buffer solution; Using monitors; Using channels
  - url: https://www.cs.utexas.edu/~EWD/transcriptions/EWD01xx/EWD123.html
    title: "Edsger W. Dijkstra, Cooperating sequential processes (EWD123)"
    locator: 4.1 Typical Uses of the General Semaphore; 4.3 The Bounded Buffer
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/BlockingQueue.html
    title: BlockingQueue, Java SE 21 API documentation
    locator: interface description
  - url: https://docs.python.org/3/library/queue.html
    title: queue, A synchronized queue class, Python documentation
    locator: class queue.Queue; Queue Objects
  - url: https://go.dev/doc/effective_go
    title: Effective Go
    locator: Channels
---

## Intent

Couple a producer and a consumer through a buffer. Dijkstra describes the producer as a cyclic process that produces a portion of information in each cycle, and the consumer as a cyclic process that processes the next portion in each cycle.[^1] The buffer lets the two run at different speeds.

## Problem

The producer and the consumer are separate threads, and they share the buffer. If the buffer has unbounded capacity, the producer can be arbitrarily far ahead of the consumer, but the consumer can never be ahead of the producer.[^2] A fast producer then fills memory with items that nobody uses yet. A consumer that finds the buffer empty must wait, and it must not take the same item as another consumer.

## Solution

Use a buffer of finite size, say N portions. Then the relation becomes symmetric: the consumer produces and the producer consumes empty positions in the buffer.[^2] Dijkstra's solution uses three semaphores:[^2][^1]

- "number of queuing portions" counts the filled places. It starts at 0, and a consumer waits on it when the buffer is empty.[^2][^1]
- "number of empty positions" counts the empty places. It starts at N, and a producer waits on it when the buffer is full.[^1]
- "buffer manipulation" starts at 1 and works as a mutex for the put and get operations.[^1]

```text
producer --put--> [ item | item | item | .... ]  N places --take--> consumer
          waits when full                       waits when empty
```

Modern libraries give the whole buffer as one type. A Java `BlockingQueue` supports operations that wait for the queue to become non-empty when retrieving an element, and wait for space to become available when storing an element.[^3] `BlockingQueue` implementations are designed to be used primarily for producer-consumer queues.[^3]

## Example

The example has 2 producers that each put 500 numbers into a buffer of 4 places, and 2 consumers that take them out. When the producers finish, the main thread puts one end-of-stream object for each consumer.

```text
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- The producer and the consumer do not call each other. They share only the buffer.
- A bounded buffer stops a fast producer. The insertion blocks once the size has been reached, until queue items are consumed.[^4]
- A `BlockingQueue` can safely be used with multiple producers and multiple consumers.[^3]

Liabilities:

- The buffer needs a way to end. A `BlockingQueue` does not intrinsically support any kind of "close" or "shutdown" operation. A common tactic is for producers to insert special end-of-stream or poison objects.[^3]
- The size of the buffer is a test of the moment only. If `empty()` returns `True`, the Python documentation does not guarantee that a subsequent call to `put()` will not block, and a `False` result does not guarantee that `get()` will not block.[^4]

A consumer that stops when the buffer is empty misses the items of a slow producer:

```python wrong id=empty-is-not-done
import queue
import threading

buffer = queue.Queue()
consumed = []
consumer_checked = threading.Event()


def consumer():
    while not buffer.empty():  # wrong: an empty buffer is not the end
        consumed.append(buffer.get())
    consumer_checked.set()


def producer():
    consumer_checked.wait()  # the producer is slow to start
    for item in range(10):
        buffer.put(item)


threads = [threading.Thread(target=consumer), threading.Thread(target=producer)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("items produced:", 10)
print("items consumed:", len(consumed))
```

```output for=empty-is-not-done
items produced: 10
items consumed: 0
```

## When not to use it

If the consumer must answer the producer, such as a request that waits for its result, a one-way buffer is not enough. Use a future or a reply channel for each item instead. If the producer and the consumer run in one thread, call the consumer directly. A buffer between them adds work and no concurrency.

## Variants and related patterns

- **Semaphores.** Dijkstra's original solution uses the two counting semaphores and a mutex.[^2] The example follows it:

```python run id=dijkstra-semaphores
import threading
from collections import deque

N = 4
queuing_portions = threading.Semaphore(0)  # counts the filled places
empty_positions = threading.Semaphore(N)  # counts the empty places
buffer_manipulation = threading.Lock()  # the mutex for the buffer
buffer = deque()
consumed = []


def producer():
    for portion in range(1000):
        empty_positions.acquire()  # P(number of empty positions)
        with buffer_manipulation:
            buffer.append(portion)
        queuing_portions.release()  # V(number of queuing portions)


def consumer():
    for _ in range(1000):
        queuing_portions.acquire()  # P(number of queuing portions)
        with buffer_manipulation:
            portion = buffer.popleft()
        empty_positions.release()  # V(number of empty positions)
        consumed.append(portion)


threads = [threading.Thread(target=producer), threading.Thread(target=consumer)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("portions consumed:", len(consumed))
print("in order:", consumed == list(range(1000)))
```

```output for=dijkstra-semaphores
portions consumed: 1000
in order: True
```

- **Monitor.** A bounded buffer monitor contains the buffer, the count and the condition variables `nonempty` and `nonfull`, and the methods `append` and `remove`.[^1] [Monitor Object](monitor-object) states this form as a pattern.
- **Channels.** A Go channel with a buffer is a bounded buffer. If the buffer is full, the sender waits until some receiver has retrieved a value.[^5] If the channel is unbuffered, the sender blocks until the receiver has received the value.[^5]
- **Shutdown.** The Python `Queue.shutdown` method puts a queue into a shutdown mode. Future calls to `put()` raise `ShutDown`, and once the queue is empty, future calls to `get()` raise `ShutDown`.[^4]

A [Thread pool](thread-pool) is a producer-consumer arrangement in which the consumers are the workers and the items are tasks. [Half-Sync/Half-Async](half-sync-half-async) puts a queue of this kind between its two layers.

## Known uses

- Java: `ArrayBlockingQueue`, `LinkedBlockingQueue` and the other `BlockingQueue` implementations.[^3]
- Python: `queue.Queue`, `queue.LifoQueue` and `queue.PriorityQueue`, where `maxsize` sets the upper bound of items.[^4]
- Go: channels with a buffer.[^5]
- Dijkstra gives the Electrologica X1 and X8 computers as the first use: two counting semaphores controlled the transport of information between store and peripheral.[^1]

## References
