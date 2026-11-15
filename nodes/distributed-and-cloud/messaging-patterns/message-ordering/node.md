---
tag: E0WM6P
kind: concept
slug: message-ordering
name: Message ordering
aliases: [ordered delivery, message order, in-order delivery, ordering key]
area: distributed-and-cloud/messaging-patterns
level: advanced
summary: >-
  Message ordering is the promise that consumers see related messages in the order in which they were sent. Brokers keep it inside a queue, a partition or a key, and rarely across all messages.
applies_to: core-languages
viz: optional
formal: none
see_also: [sequential-convoy, competing-consumers, idempotent-receiver, correlation-identifier, aggregator, point-to-point-channel, publish-subscribe-channel, guaranteed-delivery]
sources:
  - url: https://www.rabbitmq.com/docs/queues
    title: RabbitMQ documentation (version 4.4), Queues
    locator: message ordering; when messages can be reordered; preserving message order
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/introduction.html
    title: Apache Kafka 4.1.0 documentation source, Introduction
    locator: main concepts and terminology (topics, partitions and event keys)
  - url: https://cloud.google.com/pubsub/docs/ordering
    title: Google Cloud, Pub/Sub documentation, Order messages
    locator: ordering keys; considerations when using message ordering
  - url: https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-key-terms.html
    title: Amazon Simple Queue Service Developer Guide, Amazon SQS FIFO queue key terms
    locator: message group ID
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/design.html
    title: Apache Kafka 4.1.0 documentation source, Design
    locator: load balancing (semantic partitioning); consumer position; message delivery semantics (idempotent producer); log compaction guarantees
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/message-sessions
    title: Microsoft, Azure Service Bus documentation, Message sessions
    locator: session features; maximum delivery count in sessions
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Resequencer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Resequencer (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 7, Message Routing, Resequencer
---
## Idea

Many messages depend on the messages before them. An order is created before it is paid, and paid before it is shipped. RabbitMQ says that message ordering matters when there are causal dependencies between messages.[^1] If a consumer sees "shipped" before "created", it can reject the message, or it can store a wrong state.

A messaging system can promise order at three levels.

- **No order.** The consumers can see the messages in any order.
- **Total order.** All consumers see all messages in the order in which they were sent.
- **Order inside a key.** The messages that have the same key, such as an order id or a customer id, keep their order. Messages with different keys have no fixed order between them.

Kafka, Google Cloud Pub/Sub and Amazon SQS FIFO queues give the third level.[^2][^3][^4] It keeps the order that most applications need, and many consumers can still work in parallel on different keys.

## How it works

**The queue keeps the order; the consumers break it.** In RabbitMQ, a queue is an ordered collection with FIFO semantics. Messages that are published on one channel are enqueued in the order of publication. Messages from several connections or channels are interleaved.[^1] The broker still delivers in the order of the queue, but other events change the order that the consumers see.[^1] The program below sends 12 messages with the keys A, B and C, 4 for each key, in the order A1 B1 C1 A2 and so on. Consumer 0 needs 1 tick for a message, and consumer 1 needs 3 ticks. The program counts the pairs of messages with one key that finish in the wrong order.

```python run id=order-cases
# 3 keys, 4 messages each, sent in the order A1 B1 C1 A2 B2 C2 ...
# Consumer 0 needs 1 tick for a message, consumer 1 needs 3 ticks.
sent = [(k, n) for n in range(1, 5) for k in "ABC"]
SPEED = [1, 3]


def process(assign):
    """assign(i, message) gives the consumer; each consumer works through its own list in order."""
    free = [0] * len(SPEED)
    done = []
    for i, m in enumerate(sent):
        c = assign(i, m)
        free[c] += SPEED[c]
        done.append((free[c], i, m))
    return [m for _, _, m in sorted(done)]


def report(name, order):
    per_key = sum(1 for i, a in enumerate(order) for b in order[i + 1:] if a[0] == b[0] and a[1] > b[1])
    total = "kept" if order == sent else "lost"
    print("%-26s total order %s, %d pairs out of order inside a key" % (name + ":", total, per_key))


report("one consumer", process(lambda i, m: 0))
report("2 consumers, round robin", process(lambda i, m: i % 2))
report("2 consumers, by key", process(lambda i, m: {"A": 0, "B": 1, "C": 0}[m[0]]))

# one consumer, but A2 fails once and goes back to the end of the queue
queue, order = list(sent), []
failed = False
while queue:
    m = queue.pop(0)
    if m == ("A", 2) and not failed:
        failed = True
        queue.append(m)
        continue
    order.append(m)
report("one consumer, A2 requeued", order)
```

```output
one consumer:              total order kept, 0 pairs out of order inside a key
2 consumers, round robin:  total order lost, 3 pairs out of order inside a key
2 consumers, by key:       total order lost, 0 pairs out of order inside a key
one consumer, A2 requeued: total order lost, 2 pairs out of order inside a key
```

One consumer keeps the total order. Two consumers that take turns lose the total order, and also the order inside the keys, because the slow consumer finishes its messages late. When all the messages of one key go to the same consumer, the total order is lost but each key keeps its order. A message that fails and goes back to the end of the queue breaks the order of its key, even with one consumer.

**Order inside a key.** Each broker has a name for the key.

- **Kafka.** A topic is divided into partitions. Events with the same key go to the same partition, and Kafka guarantees that any consumer of a partition reads its events in exactly the order in which they were written.[^2] The producer hashes the key to select the partition.[^5] Each partition is consumed by exactly one consumer in each consumer group at a time.[^5]
- **Amazon SQS.** In a FIFO queue, the attribute `MessageGroupId` puts messages in groups. The messages of one group are always processed one at a time, in strict order.[^4]
- **Google Cloud Pub/Sub.** Related messages have the same ordering key, of up to 1 KB. They must be published in the same region, and the subscription must have message ordering enabled. Messages with an empty ordering key are not ordered.[^3]
- **Azure Service Bus.** A session holds the messages that have one session id. A receiver that accepts a session holds an exclusive lock on all its messages, so one receiver processes them in order.[^6] The [[sequential-convoy]] page describes this pattern.
- **RabbitMQ.** A stream is an append-only log, where the offset of a message never changes. Or a queue has Single Active Consumer, so only one consumer receives messages at a time.[^1]

**Fix the order at the receiver.** If the channel cannot keep the order, the receiver can put the messages back in order. The problem statement of the Resequencer in Enterprise Integration Patterns asks: "How can we get a stream of related but out-of-sequence messages back into the correct order?"[^7] The answer: "Use a stateful filter, a Resequencer, to collect and re-order messages so that they can be published to the output channel in a specified order."[^7] Each message carries a key and a sequence number for that key. The resequencer keeps the next number that it expects for each key. It holds a message that comes too early, and it releases the held messages when the missing one arrives. The output channel after it must keep the order.[^7]

## In languages

The example files build a resequencer in memory. Eight messages with the keys A and B arrive out of order, with one copy. The resequencer holds A3 until A2 arrives, and it drops the second A3.

| Language | Verdict | Form |
|---|---|---|
| Python | idiom | a dict of the next number for each key and a dict of held messages |
| Go | idiom | maps, with `sort.Strings` for a fixed order of the keys in the output |
| Java | idiom | `TreeMap` and `HashMap`, with `computeIfAbsent` for the held messages of a key |
| C# | idiom | `SortedDictionary` with an ordinal comparer, and `Dictionary.Remove(key, out value)` |
| JavaScript | idiom | `Map` objects for the next numbers and the held messages |
| TypeScript | idiom | as JavaScript, with types for the key and the sequence number |
| Rust | idiom | `BTreeMap` and `HashMap` from the standard library |
| C | idiom | arrays indexed by key and sequence number, or a hash table written by hand |
| C++ | idiom | `std::map` and `std::unordered_map` |
| Kotlin | idiom | `sortedMapOf` and `mutableMapOf`, with `getOrPut` |
| PowerShell | idiom | hashtables, or .NET dictionaries as in C# |
| Bash | idiom | associative arrays, with `declare -A` |
| SQL | idiom | a table of held messages; a query with `ORDER BY` key and sequence releases the rows up to the first gap |
| Regex | n/a | a pattern has no messages and no state |
| HTML/CSS | n/a | markup and style rules have no messages |

No language has message order as a feature. Order is a property of the broker and of the code around it. The resequencer is the same small piece of state in each language. One map gives the next number for each key, and one map gives the messages that wait for each key.

## Variations and related ideas

**Partition by key and scale out.** Sometimes messages must be processed in parallel, but in order for each entity, such as each order id. For this case, RabbitMQ suggests the `x-modulus-hash` exchange, which partitions the messages across several queues, each with a single active consumer.[^1] Kafka partitions and SQS message groups do the same inside the broker.[^2][^4]

**Duplicates.** Order and copies are related. Kafka has an idempotent producer: the broker gives each producer an id, and it uses a sequence number that the producer sends with each message to drop copies.[^5] At the receiver, an [[idempotent-receiver]] drops copies, and the resequencer in the examples drops a copy whose number it has already delivered.

**Related patterns.** The book relates the Resequencer to the [[aggregator]], [[competing-consumers]], the message router and the [[splitter]].[^7] A [[correlation-identifier]] connects a reply to its request, but it does not give an order.

## Pitfalls

- **Redelivery.** In RabbitMQ, a message that a consumer negatively acknowledges with requeue, or that is unacknowledged when a channel closes, is delivered again, and this can change the order. The broker flags a redelivered message.[^1] In Pub/Sub, a redelivery of a message also redelivers all later messages of its key, even the acknowledged ones.[^3]
- **The dead letter queue.** In Service Bus, a message of a session can be abandoned more often than `MaxDeliveryCount`, which is 10 by default. Then it goes to the dead letter queue, and the receiver continues with the later messages. If the message is moved back to the queue, it loses its order, because it gets a new enqueue time and sequence number.[^6] Pub/Sub forwards messages to a dead letter topic on a best-effort basis, so its redelivery rule might not hold.[^3]
- **The consumer side.** RabbitMQ also says that consumers that return messages to the queue must return them in the order in which they received them. A quorum queue needs a delivery limit, and AMQP 0-9-1 consumers must not use `basic.get`.[^1]
- **Priorities.** In RabbitMQ, a message of higher priority can be delivered before a message of lower priority, so a priority queue does not keep the order of publication.[^1]
- **No order across keys.** In Pub/Sub, messages with different ordering keys have no order between them.[^3] Code that needs two keys in order needs one key for both, or a step that waits for both.
- **Hot keys.** All messages of one key go to one consumer. In Pub/Sub, a hot key occurs when a key gets more messages each second than the subscriber can process, and a backlog builds on it.[^3]

## Cost

Order limits parallel work. One key is processed by one consumer at a time, so the throughput of a key is the speed of one consumer.[^3][^4] Pub/Sub also limits the publish throughput of each ordering key to 1 MBps.[^3] Google says that ordered delivery decreases publish availability and increases end-to-end latency, because a failover needs coordination to keep the order.[^3] A resequencer holds early messages in memory until the missing ones arrive. If a message is lost, the held messages of its key wait for ever, unless the resequencer has a timeout or a limit.

## History

Enterprise Integration Patterns by Gregor Hohpe and Bobby Woolf, published in 2003, puts the Resequencer in its chapter on message routing.[^8] The book explains that a router can send related messages on different routes, so some of them pass the processing steps sooner than others and arrive out of order.[^7] Kafka builds its design on totally ordered partitions: because each partition has exactly one consumer in a group, the position of the consumer is one number, the offset of the next message. Log compaction never changes the order of the messages; it only removes some.[^5]

## References
