---
tag: HKN4DH
kind: pattern
slug: worker-queue
name: Work queue
aliases: [work queue, task queue, job queue, worker queue]
area: software-design/concurrency-patterns
level: core
summary: >-
  A work queue holds tasks that several workers take and do. The queue keeps each task until a worker acknowledges it, so a task whose worker fails goes to another worker.
family: concurrency
polarity: positive
intent: Put tasks on a shared queue so that several workers can take and do them concurrently, and keep each task until a worker acknowledges that the work is done, so that no task is lost when a worker fails.
also_known_as: [Task queue, Job queue]
see_also: [producer-consumer, thread-pool, pipeline-concurrency, message-passing-concept, command-pattern, guarded-suspension, scheduler-pattern, future-pattern]
sources:
  - url: https://www.rabbitmq.com/tutorials/tutorial-two-python
    title: RabbitMQ tutorial, Work Queues (using the Pika Python client)
    locator: the introduction; Round-robin dispatching; Message acknowledgment; Fair dispatch
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/competing-consumers
    title: Competing Consumers pattern, Azure Architecture Center
    locator: the introduction; Solution; Problems and considerations; When to use this pattern; Example
  - url: https://docs.python.org/3/library/queue.html
    title: queue, A synchronized queue class, Python documentation
    locator: Queue.task_done; Queue.join
---

## Intent

The main idea behind work queues (also called task queues) is to avoid doing a resource-intensive task immediately and having to wait for it to complete.[^1] The program puts each task on a queue, and workers take the tasks and do them. The Azure Architecture Center describes the form for distributed systems as the Competing Consumers pattern: enable multiple concurrent consumers to process messages received on the same messaging channel.[^2]

## Problem

A program gets more work than one thread can do at once. Some tasks are slow, and the amount of work changes over time. The program must not wait for each task to finish before it accepts the next one. Workers can fail in the middle of a task. If the program gives a task to a worker and then forgets it, a worker that fails takes the task with it.

## Solution

Put each task on a queue, and start several workers that take tasks from it. Each task goes to one worker. A worker takes a task, does the work, and then sends an acknowledgment (an ack) to the queue. The queue keeps each task that it gave out until the ack comes.

```text
           ┌─────────── work queue ───────────┐
 producer ─┤ ready: 5 6 7 ...  unacked: 1 2 3 ├─► worker 1: take 1, work, ack 1
           │                                  ├─► worker 2: take 2, work, ack 2
           │   no ack from a lost worker:     ├─► worker 3: take 3, fails
           │   task 3 goes back to ready  ◄───┘
           └──────────────────────────────────┘
```

An ack tells the queue that a particular message has been received and processed, and that the queue is free to delete it.[^1] If a consumer dies without sending an ack, RabbitMQ understands that a message was not processed fully and re-queues it; if other consumers are online, it then quickly redelivers it to another consumer.[^1]

The queue also decides which worker gets the next task. By default, RabbitMQ sends each message to the next consumer, in sequence, which is called round-robin.[^1] With fair dispatch, the queue does not give a new message to a worker until it has processed and acknowledged the previous one.[^1] The example uses this rule: a worker asks for a task only when it is free.

## Example

The program puts ten tasks on a work queue, and three workers take them. Each delivery gets a tag, and the worker acks the tag after the work. The first worker that takes task 4 fails before it does the work and stops. The queue gets no ack for that delivery, so it puts task 4 back, and another worker does it. The queue gives out 11 deliveries for 10 tasks.

```text
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

The examples for each language show the same program.

## Consequences

Benefits:

- Several workers do tasks at the same time. A long task does not stop the other workers, because a message that requires some long-running processing does not prevent other consumer service instances from processing other messages concurrently.[^2]
- The producer and the workers are independent. The queue functions as a buffer between application instances and consumer service instances, so it can take a sudden increase in work.[^2]
- The program can add or remove workers when the amount of work changes.[^2]
- A task is not lost when a worker fails, because the queue keeps it until the ack.

Liabilities:

- A task can be done more than once. The message queue ensures that each message is delivered at least once,[^2] not exactly once. If a worker does the work and fails before the ack, the queue gives the task to another worker. Operations that can run twice must be idempotent, which minimizes the effects when a worker processes a single message more than once.[^2]
- The order is not fixed. The order in which consumer service instances receive messages is not guaranteed and does not necessarily show the order in which the messages were created.[^2]
- A bad task can fail every worker that takes it. The system should prevent such poison messages from returning to the queue indefinitely, and store their details elsewhere instead.[^2]
- A missing ack has a cost. It is a common mistake to miss the ack: messages are redelivered when the client quits, and RabbitMQ uses more and more memory because it cannot release any unacknowledged messages.[^1]

In this program, the queue forgets each task when a worker takes it. This is like an ack before the work:

```python wrong id=ack-before-the-work
import queue
import threading

tasks = queue.Queue()
for n in range(1, 11):
    tasks.put(n)
results = {}
failed = []
lock = threading.Lock()


def worker():
    while True:
        try:
            task = tasks.get_nowait()  # the queue forgets the task here: there is no ack
        except queue.Empty:
            return
        with lock:
            if task == 4 and not failed:
                failed.append(task)
                return  # this worker fails before it does the work
            results[task] = task * task


threads = [threading.Thread(target=worker) for _ in range(3)]
for t in threads:
    t.start()
for t in threads:
    t.join()
print("tasks with a result:", len(results))
print("lost tasks:", sorted(set(range(1, 11)) - set(results)))
```

```output for=ack-before-the-work
tasks with a result: 9
lost tasks: [4]
```

RabbitMQ describes the same loss: when the queue marks a message for deletion as soon as it delivers the message, the message that a terminated worker was processing is lost.[^1]

## When not to use it

Do not use a work queue when the tasks depend on each other or must run in a fixed order. The Azure guide says that the pattern might not be suitable when there is a high degree of dependence between tasks, or when tasks must run synchronously and the application logic must wait for each task to complete before it continues.[^2] A [pipeline](pipeline-concurrency) fits work that goes through fixed steps. If the tasks run in one process and a lost task is not a problem, a [thread pool](thread-pool) is simpler, because it needs no acks.

## Variants and related patterns

- **Idempotent handler.** At-least-once delivery can give a worker the same task twice. The handler keeps the IDs of the tasks that it has done and ignores a second delivery of the same ID:

```python run id=idempotent-handler
# Delivery is at least once: the worker that first took payment 4 failed
# after it did the work and before its ack, so the queue delivered it again.
deliveries = [(1, 10), (2, 20), (3, 30), (4, 40), (5, 50), (4, 40)]

balance = 0
for payment_id, amount in deliveries:
    balance += amount
print("balance, handler that is not idempotent:", balance)

balance, done = 0, set()
for payment_id, amount in deliveries:
    if payment_id in done:
        continue  # a duplicate delivery: the effect is already applied
    balance += amount
    done.add(payment_id)
print("balance, idempotent handler:", balance)
```

```output for=idempotent-handler
balance, handler that is not idempotent: 190
balance, idempotent handler: 150
```

- **In-process work queue.** In Python, a worker calls `Queue.task_done()` after each `get()`, which tells the queue that the processing on the task is complete, and `Queue.join()` blocks until all items in the queue have been gotten and processed.[^3] This counts the tasks that are not finished, but it does not put back a task whose worker failed.
- **Dead-letter queue.** Azure Service Bus can automatically send messages to a dead-letter queue after the delivery count exceeds a configured threshold.[^2] This stops a poison message.
- **Result queue.** A worker process can pass results back to the application logic through a dedicated message reply queue.[^2]
- **[Producer-consumer](producer-consumer)** is the bounded buffer between threads that a work queue is built on. A work queue adds acks and redelivery.
- **[Command](command-pattern)** objects are a common form for the tasks.

## Known uses

- RabbitMQ: the work queues tutorial distributes time-consuming tasks among multiple workers, with manual acks and `basic_qos(prefetch_count=1)` for fair dispatch.[^1]
- Azure: Service Bus queues and Azure Functions queue triggers together directly implement the Competing Consumers pattern.[^2]
- Python: `queue.Queue` with `task_done()` and `join()` is the in-process form.[^3]

## References
