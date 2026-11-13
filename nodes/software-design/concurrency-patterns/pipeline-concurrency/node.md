---
tag: 0M24GG
kind: pattern
slug: pipeline-concurrency
name: Concurrent pipeline
aliases: [pipeline, concurrent pipeline, pipeline pattern, staged pipeline, fan-out fan-in]
area: software-design/concurrency-patterns
level: core
summary: >-
  A concurrent pipeline is a chain of stages that run at the same time and pass values through channels. A slow stage can have several workers, and full buffers make fast stages wait.
family: concurrency
polarity: positive
intent: Split the processing of a stream into stages that run concurrently, connect each stage to the next with a channel, and let each stage start on a value as soon as the stage before it sends that value.
also_known_as: [Pipes and filters, Fan-out and fan-in]
see_also: [producer-consumer, channels, message-passing-concept, thread-pool, fork-join, barrier-pattern, future-pattern, replace-loop-with-pipeline]
sources:
  - url: https://en.wikipedia.org/wiki/Pipeline_(software)
    title: Pipeline (software), Wikipedia
    locator: lead
  - url: https://go.dev/blog/pipelines
    title: Sameer Ajmani, Go Concurrency Patterns, Pipelines and cancellation, The Go Blog (13 March 2014)
    locator: What is a pipeline?; Fan-out, fan-in; Stopping short; Explicit cancellation
  - url: https://docs.python.org/3/library/queue.html
    title: queue, A synchronized queue class, Python documentation
    locator: the module description; Queue; Queue.put; Queue.shutdown
  - url: https://doc.rust-lang.org/std/sync/mpsc/index.html
    title: The Rust Standard Library, module std::sync::mpsc
    locator: the module description; Disconnection
---

## Intent

In software engineering, a pipeline consists of a chain of processing elements, arranged so that the output of each element is the input of the next.[^1] In a concurrent pipeline, each element is a stage that runs at the same time as the others. In Go terms, a pipeline is a series of stages connected by channels, where each stage is a group of goroutines running the same function.[^2]

## Problem

A program reads a stream of values and does several steps of work on each one, for example parse, compute and store. A single loop that does all the steps for one value before it reads the next value uses one processor, and it waits for input and output one value at a time. The steps are independent for different values, so the program could work on several values at once, but threads that share the values need a safe way to pass them on.

## Solution

Give each step its own stage, and connect the stages with channels or blocking queues. In each stage, the workers receive values from upstream via inbound channels, perform some function on that data, and send values downstream via outbound channels.[^2] The first stage is sometimes called the source or producer, and the last stage the sink or consumer.[^2]

```text
 source ──► [buf 1] ──┬─► square worker 1 ─┐
  1..20               ├─► square worker 2 ─┼─► [buf 1] ──► sink: count, sum
                      └─► square worker 3 ─┘
            fan-out                        fan-in
```

A slow stage can have several workers. Multiple functions can read from the same channel until that channel is closed, which is called fan-out.[^2] A function can read from multiple inputs and proceed until all are closed, by multiplexing the input channels onto a single channel, which is called fan-in.[^2]

The end of the stream must travel through the pipeline. In Go, stages close their outbound channels when all the send operations are done, and they keep receiving values from inbound channels until those channels are closed.[^2] Usually some amount of buffering is provided between consecutive elements.[^1] A bounded buffer gives backpressure: in Python, insertion into a `Queue` with a `maxsize` will block once this size has been reached, until queue items are consumed.[^3]

## Example

A source sends the numbers 1 to 20. Three square workers read from one queue, and they send their results to one output queue. The sink counts the values and adds them up. Each queue holds one value. The source, the workers and the output queue together hold at most five values that the sink has not received, so the source cannot send its last value until the sink has started. The last line shows that the stages ran at the same time.

```text
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- The stages run at the same time on different values. Go's concurrency primitives make it easy to construct streaming data pipelines that make efficient use of I/O and multiple CPUs.[^2]
- Each stage is small and has one job. Connecting elements into a pipeline is analogous to function composition.[^1]
- Fan-out gives a slow stage more workers without a change to the other stages, as a way to distribute work amongst a group of workers to parallelize CPU use and I/O.[^2]
- Bounded buffers limit the memory that the pipeline uses, because a fast stage waits for a slow one.

Liabilities:

- The pipeline is as fast as its slowest stage, unless that stage has enough workers.
- Fan-out changes the order of the values. In the example, the sink adds the values, so the order does not matter. A sink that needs the order must sort the values again.
- A stage that stops early can leave the stages before it blocked. In Go, this is a resource leak: goroutines consume memory and runtime resources, and goroutines are not garbage collected; they must exit on their own.[^2]

In this program, the sink stops after three values and does not tell the source:

```python wrong id=sink-stops-early
import queue
import threading

numbers = queue.Queue(maxsize=1)
stuck = []


def source():
    for n in range(1, 21):
        try:
            numbers.put(n, timeout=0.5)
        except queue.Full:
            stuck.append(n)  # nobody will ever receive this value
            return


producer = threading.Thread(target=source)
producer.start()
# Wrong: the sink stops after 3 values and does not tell the source.
used = [numbers.get() for _ in range(3)]
producer.join()
print("values the sink used:", used)
print("source stuck on a send that nobody receives:", len(stuck) == 1)
```

```output for=sink-stops-early
values the sink used: [1, 2, 3]
source stuck on a send that nobody receives: True
```

The source ends here only because its `put` has a timeout. Without a timeout, the source thread would wait forever for a free slot.

## When not to use it

Do not use a pipeline when the work is one step. A [thread pool](thread-pool) or a [producer-consumer](producer-consumer) queue is simpler. Do not split cheap steps into stages, because each pass through a channel costs a lock and a thread switch. Do not use a pipeline for work that is not a stream, such as one large computation that splits into parts and joins them. [Fork-join](fork-join) fits that case.

## Variants and related patterns

- **Explicit cancellation.** When the sink decides to stop early, it must tell the upstream stages to abandon the values they are trying to send.[^2] In Go, closing a `done` channel is effectively a broadcast signal to the senders.[^2] Python's `Queue.shutdown()` puts a queue into a shutdown mode, and currently blocked callers of `put()` will be unblocked and will raise `ShutDown`.[^3] This program uses an event as the `done` signal:

```python run id=cancel-the-source
import queue
import threading

numbers = queue.Queue(maxsize=1)
done = threading.Event()  # the sink sets it to cancel the upstream stage
sent = []


def source():
    for n in range(1, 21):
        while not done.is_set():
            try:
                numbers.put(n, timeout=0.05)
                sent.append(n)
                break
            except queue.Full:
                pass  # check done again, then try again
        if done.is_set():
            return


producer = threading.Thread(target=source)
producer.start()
used = [numbers.get() for _ in range(3)]
done.set()  # tell the source to stop
producer.join(timeout=5)
print("values the sink used:", used)
print("source finished:", not producer.is_alive())
print("source stopped before it sent all 20:", len(sent) < 20)
```

```output for=cancel-the-source
values the sink used: [1, 2, 3]
source finished: True
source stopped before it sent all 20: True
```

- **Close by dropping the sender.** In Rust, an unsuccessful send or receive is normally indicative of the other half of a channel having hung up by being dropped.[^4] A stage ends when its sender is dropped, and the Rust example uses this as the close signal.
- **Bounded or unbounded links.** A Rust `sync_channel` has a pre-allocated buffer of a fixed size, and all sends block until there is buffer space available, while a `channel` conceptually has an infinite buffer.[^4]

Each link of the pipeline is a [producer-consumer](producer-consumer) queue. The stages communicate by [message passing](message-passing-concept) over [channels](channels), not by shared variables. The [Replace loop with pipeline](replace-loop-with-pipeline) refactoring builds a pipeline of collection operations in one thread. This page is about stages that run concurrently.

## Known uses

- Go pipelines of goroutines and channels, with fan-out, fan-in and a `done` channel for cancellation.[^2]
- Python `queue.Queue` with a `maxsize` between threads.[^3]
- Rust `std::sync::mpsc` channels, which are multi-producer, single-consumer FIFO queues.[^4]
- The pipe(s) and filters design pattern, in which the elements of a pipeline may be called filters.[^1]

## References
