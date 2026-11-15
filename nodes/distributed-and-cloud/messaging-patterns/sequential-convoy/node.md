---
tag: 1GN8NF
kind: pattern
slug: sequential-convoy
name: Sequential Convoy
aliases: [Sequential Convoy pattern, ordered processing per key, message sessions, message groups]
area: distributed-and-cloud/messaging-patterns
level: advanced
summary: >-
  A sequential convoy gives each message a category key, such as an order id, and processes the messages of one category in order, one at a time, while different categories run in parallel.
family: integration
polarity: positive
intent: Keep the order of related messages and still scale out, by a key on each message that puts it in a group, a lock that gives each group to one consumer at a time, and many consumers for many groups.
also_known_as: [message sessions]
see_also: [competing-consumers, point-to-point-channel, correlation-identifier, dead-letter-channel, idempotent-receiver, aggregator, splitter, enterprise-integration-patterns]
sources:
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/sequential-convoy
    title: Microsoft, Azure Architecture Center, Sequential Convoy pattern
    locator: context and problem; solution; problems and considerations; when to use this pattern; example; related resources
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/message-sessions
    title: Microsoft, Azure Service Bus documentation, Message sessions
    locator: first-in, first out (FIFO) pattern; session features
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Resequencer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Resequencer (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/resequence-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Resequence EIP
    locator: introduction; batch resequencing
---

## Intent

Process the related messages in the order in which they come, and still use many consumers. The Azure page describes the pattern so. It puts related messages into categories and processes each category in sequence, one message at a time. Different categories are processed in parallel.[^1]

## Problem

Take a system that tracks orders. A stream of operations comes in: create an order, add a transaction, change an old transaction, delete the order. The operations of one order must be applied in their order, or the state of the order becomes wrong. But the queue mixes the operations of many orders.[^1]

The two simple answers both fail.[^1]

- **One consumer.** It keeps the order, because it processes one message at a time, but it cannot scale.
- **[[competing-consumers]].** Many consumers read in parallel and scale, but two of them can take two operations of the same order, and process them at the same time or in the wrong order.

The forces are order inside a group and throughput across all groups.

## Solution

Give each message a category key that names its group, for example the order id. The broker uses the key to put the messages into logical groups. Inside a group, it keeps first-in, first-out order: a consumer that locks a group gets its messages in the order in which they were queued. Other consumers can process other groups at the same time.[^1]

```text
                                        +--> consumer 1: A1 A2 A3 A4
  producer --> [ A1 B1 A2 C1 B2 A3 ... ] +--> consumer 2: B1 B2 B3
                  one queue, key = order +--> consumer 3: C1 C2, then D1 D2
```

The participants are the producer, which sets the key on each message, the queue or broker, which groups the messages by key and locks a group for one consumer, and the consumers.[^1]

**Session locks.** In Azure Service Bus, a group is a session. A sender sets the session id on each message; at the AMQP level, this is the `group-id` property.[^2] A receiver that accepts a session has an exclusive lock on all the messages with that session id, also on those that come later. The lock ends when the receiver closes the session or when the lock expires, and the receiver can renew it.[^2] While no receiver holds the lock of a session, no messages of that session are delivered.[^2]

The program below sends the operations of 4 orders through one queue. A create takes 3 ticks, and each other operation takes 1 tick. An operation is out of order if it starts before the previous operation of its order is done.

```python run id=convoy-order
# Operations for four orders arrive interleaved on one queue.
# A "create" takes 3 ticks; any other operation takes 1 tick.
ops = [("A", "create"), ("B", "create"), ("A", "add"), ("C", "create"), ("A", "add"),
       ("B", "add"), ("D", "create"), ("C", "add"), ("A", "delete"), ("B", "modify"),
       ("D", "add"), ("C", "delete")]
COST = {"create": 3}


def cost(op):
    return COST.get(op[1], 1)


def check(runs):
    """runs: (start, end, order, index in its order). Count operations that start before the previous one ends."""
    last_end, bad = {}, 0
    for start, end, order, _ in sorted(runs, key=lambda r: (r[2], r[3])):
        if start < last_end.get(order, 0):
            bad += 1
        last_end[order] = end
    return bad, max(r[1] for r in runs)


def indexed():
    seen = {}
    for op in ops:
        seen[op[0]] = seen.get(op[0], -1) + 1
        yield op, seen[op[0]]


def competing(workers):
    free = [0] * workers
    runs = []
    for op, i in indexed():
        w = free.index(min(free))
        start = free[w]
        free[w] = start + cost(op)
        runs.append((start, free[w], op[0], i))
    return check(runs)


def convoy(workers):
    groups = {}
    for op, i in indexed():
        groups.setdefault(op[0], []).append((op, i))
    free = [0] * workers
    runs = []
    for order in groups:  # a free worker locks the next session and empties it
        w = free.index(min(free))
        for op, i in groups[order]:
            start = free[w]
            free[w] = start + cost(op)
            runs.append((start, free[w], order, i))
    return check(runs)


for label, (bad, done) in [("one consumer", competing(1)),
                           ("3 competing consumers", competing(3)),
                           ("3 consumers, one session at a time", convoy(3))]:
    print("%-35s done at tick %2d, %d out of order" % (label + ":", done, bad))
```

```output
one consumer:                       done at tick 20, 0 out of order
3 competing consumers:              done at tick  7, 2 out of order
3 consumers, one session at a time: done at tick  9, 0 out of order
```

One consumer is correct but slow. Three competing consumers are fast, but 2 operations start while the create of their order is still running. With one session at a time for each consumer, nothing is out of order, and the work ends at tick 9. The longest order takes 6 ticks, and the fourth order must wait for a free consumer.

## Example

Each example file runs 2 workers on 3 sessions. A worker locks the next free session and keeps it until the session is empty. The second message of session B cannot be processed. While it fails, session B waits, but session A and then session C go on with the other worker. After 3 deliveries, the message goes to the dead letter queue, and session B goes on. The Azure page asks for this. A message that fails again and again blocks all later messages in its session. Thus, count the deliveries, and move the message to a dead-letter queue after a limit.[^1] Service Bus does this with the maximum delivery count of the queue, which is 10 by default; after that, the receiver gets the next messages of the session.[^2]

## Consequences

**Benefits.**

- **Order inside each group.** The messages of one category are processed in sequence, with no race conditions and no workarounds for wrong order.[^1]
- **Scale across groups.** Each category is an independent unit of concurrency, so more consumers give more throughput, as long as there are enough active categories.[^1]
- **Loose coupling.** The producer does not know which consumer processes a message, or when.[^1]

**Liabilities.**

- **The choice of the key.** The key sets the unit of parallelism. A key that is too coarse, such as one customer id for all orders, limits the parallelism; a key that is too fine gives no useful order.[^1]
- **A limit for each group.** The throughput of one category is limited by the time for one message.[^1]
- **Poison messages.** A message that always fails blocks its whole session.[^1]
- **Locks.** A lock that is too short can expire and cause a message to be processed again; a lock that is too long delays the recovery from a stalled consumer.[^1]
- **Replay breaks the order.** If a message from the dead-letter queue is sent back to the original queue, it gets a new sequence number, and its place in the order of the session is lost.[^2]
- **Wrong keys.** If a producer sets a wrong key, the message goes into the wrong session and corrupts the state of that group.[^1]
- **Broker support.** Not all brokers lock a group or keep order inside a partition. Without such support, the consumers must coordinate themselves, which adds complexity and the risk of copies, lost messages or wrong order.[^1] In Service Bus, the basic tier has no sessions, and a queue with sessions takes only messages with a session id.[^2]

## When not to use it

The Azure page names two cases.[^1]

- **Very high throughput.** For millions of messages per minute, the FIFO rule limits how far the system can scale.
- **No need for order.** If each message can be processed alone, [[competing-consumers]] scale more simply and need no session locks.

If the throughput must be very high, the Azure page asks whether the strict order is necessary for the whole life of a message. It names two other ways. One is a start message and an end message around a sequence. The other is a sort by time stamp inside a batch window, after which the batch is processed in parallel.[^1]

## Variants and related patterns

**Serial in, parallel out.** In the Azure example, all transactions first go through one ledger queue and one ledger processor. The processor sets the session id of each transaction to its order id and sends it to a second queue, where order processors take one session each. The ledger stage is the main bottleneck.[^1]

**Order on arrival.** Messages can still come to the broker in the wrong order, because the network delay from each producer differs. The Azure page suggests sequence numbers inside each category, and a flag on the last message of a sequence.[^1] Service Bus leaves the start and the end of a sequence to the application, for example a label of `start`, `content` and `end`.[^2]

**Resequencer.** When the order is already lost, a resequencer puts it back. The book asks: "How can we get a stream of related but out-of-sequence messages back into the correct order?" The answer: "Use a stateful filter, a Resequencer", "to collect and re-order messages so that they can be published to the output channel in a specified order."[^3] Camel has a batch mode, which collects up to 100 messages or waits one second by default and then sorts them, and a stream mode, which finds the gaps between messages.[^4]

**With priorities.** The Azure page says a convoy can be combined with a priority queue, with one session for each priority, when the order inside each priority level must stay.[^1]

## Known uses

- Azure Service Bus: message sessions, the main Azure mechanism for this pattern. Azure Functions can process one session at a time with the Service Bus trigger, and Logic Apps with the peek-lock connector.[^1][^2]
- Apache Camel: the [Resequence EIP of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/resequence-eip.adoc), which restores an order by an expression, such as a header with a sequence number.[^4]

## References
