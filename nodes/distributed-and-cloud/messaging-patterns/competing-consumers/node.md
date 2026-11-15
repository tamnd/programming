---
tag: 0FHWAR
kind: pattern
slug: competing-consumers
name: Competing Consumers
aliases: [Competing Consumers pattern, competing consumer, work queue consumers, worker pool on a queue]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  Competing consumers are many consumers on one point-to-point channel; the messaging system gives each message to one of them, so the work is shared and processed in parallel.
family: integration
polarity: positive
intent: Process messages in parallel and share the load, by many consumers that receive from one point-to-point channel, where each message goes to only one consumer.
also_known_as: [competing consumer]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on messaging endpoints. The Azure Architecture Center describes it as one of its cloud design patterns.
see_also: [point-to-point-channel, publish-subscribe-channel, dead-letter-channel, idempotent-receiver, polling-consumer, worker-queue, thread-pool, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/CompetingConsumers.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Competing Consumers (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; example, Apache Kafka
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 10, Messaging Endpoints, Competing Consumers
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/competing-consumers
    title: Microsoft, Azure Architecture Center, Competing Consumers pattern
    locator: context and problem; solution; problems and considerations; when to use this pattern; example
  - url: https://www.rabbitmq.com/tutorials/tutorial-two-python
    title: RabbitMQ tutorials, Work Queues (Python)
    locator: round-robin dispatching; message acknowledgment; fair dispatch
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingEndpointsIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Messaging Endpoints
    locator: message consumer patterns; message endpoint themes
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageDispatcher.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Message Dispatcher (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/competing-consumers.adoc
    title: Apache Camel 4.14.0 documentation source, Competing Consumers
    locator: competing consumers with JMS; competing consumers with thread pool
---

## Intent

Let many consumers share the work of one queue, so that the messages are processed in parallel. The book asks: "How can a messaging client process multiple messages concurrently?"[^1][^2] The answer: "Create multiple Competing Consumers on a single channel so that the consumers can process multiple messages concurrently."[^1]

## Problem

An application puts work on a queue faster than one consumer can process it. The load also changes. The Azure page gives the example of a system that must process many hundreds of requests per second at peak hours, and few at other times.[^3] One consumer becomes the bottleneck, and the queue grows.

More threads in one consumer help only up to the limits of one process, and one process is a single point of failure.

The forces are these. Each message must be processed by one consumer only. No consumer must be idle while another has a backlog. And a consumer that stops must not lose the message that it holds.

## Solution

Start many consumers on one [[point-to-point-channel]]. Each consumer receives from the channel, and the messaging system gives each message to one of them. The consumers compete, and the messaging system, not the consumers, decides which one gets a message.[^1]

```text
                                    +--> [ consumer 1 ]
  producers ---> [ work queue ] ----+--> [ consumer 2 ]     each message goes to one consumer
                       |            +--> [ consumer 3 ]
                       +--> [ dead letters ] (a message that fails too often)
```

The participants are the producers, the queue, the consumers and often a [[dead-letter-channel]]. The pattern needs a point-to-point channel. On a [[publish-subscribe-channel]], each consumer gets its own copy, so more consumers make more copies, not less work for each one.[^1][^3]

**Acknowledgements.** A consumer tells the broker when it has processed a message. In the RabbitMQ tutorial, when a consumer stops before its ack, RabbitMQ puts the message back on the queue and gives it to another consumer.[^4] Azure Service Bus does the same with PeekLock: the message stays in the queue but is hidden from the other consumers, and it becomes visible again if the consumer abandons it.[^3] So a message can come more than once, and the consumers must be safe for this; see [[idempotent-receiver]].

**How the broker chooses.** By default, RabbitMQ gives each message to the next consumer in turn, and it does not look at how many unacknowledged messages a consumer has. With `basic_qos(prefetch_count=1)`, it gives a consumer a new message only after the consumer has acknowledged the last one.[^4] The program below simulates two workers and six messages. The odd messages take 5 ticks and the even ones 1 tick.

```python run id=dispatch
# Two workers share one queue. Odd messages take 5 ticks, even ones 1 tick.
costs = {1: 5, 2: 1, 3: 5, 4: 1, 5: 5, 6: 1}


def round_robin(workers):
    # The broker gives message n to worker n mod k when it arrives.
    busy = [0] * workers
    done = []
    for m, cost in costs.items():
        w = (m - 1) % workers
        busy[w] += cost
        done.append((busy[w], m, w))
    return busy, sorted(done)


def prefetch_one(workers):
    # The broker gives the next message to a worker only when it is free.
    free_at = [0] * workers
    busy = [0] * workers
    done = []
    for m, cost in costs.items():
        w = min(range(workers), key=lambda i: (free_at[i], i))
        free_at[w] += cost
        busy[w] += cost
        done.append((free_at[w], m, w))
    return busy, sorted(done)


for name, plan in [("round robin", round_robin), ("prefetch 1", prefetch_one)]:
    busy, done = plan(2)
    print(f"{name}: worker 1 busy {busy[0]} ticks, worker 2 busy {busy[1]} ticks,"
          f" all done at tick {max(t for t, _, _ in done)}")
    print("  order of completion:", " ".join(f"m{m}" for _, m, _ in done))
```

```output
round robin: worker 1 busy 15 ticks, worker 2 busy 3 ticks, all done at tick 15
  order of completion: m2 m4 m6 m1 m3 m5
prefetch 1: worker 1 busy 11 ticks, worker 2 busy 7 ticks, all done at tick 11
  order of completion: m2 m1 m3 m4 m6 m5
```

With round robin, worker 1 gets all the heavy messages and is busy for 15 ticks, while worker 2 is idle after 3 ticks. With a prefetch of 1, the work is shared and all is done at tick 11. In both runs, the messages complete in an order that is not the order in which they were sent.

## Example

Each example file starts three workers on one in-memory queue with eight jobs. Job 4 fails on its first delivery, as with a short outage of a database, and goes back on the queue. Job 7 always fails. After three deliveries it goes to the dead letter channel, so that it does not come back for ever. Which worker gets which job changes from run to run, so the programs sort the events by job number before they print them.

## Consequences

**Benefits.**

- **Throughput.** The consumers work in parallel, and the number of consumers can change with the length of the queue. Azure Functions adds consumer instances as the queue grows, and can scale down to zero when it is empty.[^3]
- **Availability.** If one consumer fails, the others go on, and a message with no ack goes to another consumer.[^3][^4]
- **Simple producers.** The producers send to one queue and do not know how many consumers there are.
- **Throttling.** Each consumer takes one message at a time, so the number of consumers limits the rate at which messages are consumed.[^5]

**Liabilities.**

- **No order.** The Azure page says that the order in which the consumers receive messages is not guaranteed.[^3] In the program above, m5 completes after m6.
- **Duplicates.** A redelivery after a failure means that a message can be processed twice.
- **Poison messages.** A message that always fails comes back again and again. Service Bus moves a message to a dead-letter queue when its delivery count is more than `MaxDeliveryCount`.[^3]
- **Transactions.** The book warns that competing consumers that control their transactions from outside the messaging system can have serious problems.[^5]
- **Fixed assignment in Kafka.** The EIP site describes a Kafka consumer group: each partition is a channel of its own, and each consumer reads its partitions. So the choice of the consumer is made in advance, a consumer with an empty partition cannot help the others, and a group cannot have more active consumers than the topic has partitions.[^1]

## When not to use it

**When the order matters.** If the messages must be processed in sequence, use one consumer, or a feature that sends a group of related messages to one consumer, such as message sessions in Service Bus.[^3]

**When the tasks depend on each other.** The Azure page names a workload that cannot be split into separate tasks, and tasks that must run synchronously, one after the other.[^3]

**When each consumer needs each message.** That is a publish-subscribe channel, not a work queue.

## Variants and related patterns

**Message Dispatcher.** The book asks: "How can multiple consumers on a single channel coordinate their message processing?" The answer: "Create a Message Dispatcher on a channel that will consume messages from a channel and distribute them to performers."[^6] Here the application chooses the performer for a message, for example by its content, where competing consumers leave the choice to the messaging system.

**Polling or event-driven.** Each competing consumer can be a [[polling-consumer]] or an event-driven consumer, and it can also be a transactional client.[^5]

**A thread pool in one process.** When a component does not allow many consumers, Camel can hand the messages of one consumer to a [[thread-pool]] with the Threads EIP. Its documentation calls the result "quasi like" competing consumers.[^7] In one program, the same shape is a [[worker-queue]].

## Known uses

- RabbitMQ: the work queues tutorial runs two workers on one queue, with round-robin dispatch, manual acks and `prefetch_count=1`.[^4]
- Apache Camel: the JMS component starts five consumers on one queue with `concurrentConsumers=5`, as the [Competing Consumers page of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/competing-consumers.adoc) shows.[^7]
- Azure: Service Bus queues with Azure Functions triggers. The runtime receives in PeekLock mode, calls Complete on success, may call Abandon on a failure, and renews the lock while a function runs.[^3]
- Apache Kafka: the consumers of a consumer group share the partitions of a topic.[^1]

## References
