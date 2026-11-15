---
tag: H316J4
kind: pattern
slug: priority-queue-pattern
name: Priority Queue pattern
aliases: [Priority Queue, message priority, priority queues for messages]
area: distributed-and-cloud/messaging-patterns
level: advanced
summary: >-
  The Priority Queue pattern gives each message a priority, so that urgent messages are processed before the others, either in one queue that orders by priority or in a separate queue for each priority.
family: integration
polarity: positive
intent: Process urgent requests before less urgent ones, by a priority on each message and either one queue that delivers by priority or one queue for each priority with its own consumers.
also_known_as: [prioritized queue]
see_also: [competing-consumers, sequential-convoy, point-to-point-channel, dead-letter-channel, content-based-router, message-channel, enterprise-integration-patterns]
sources:
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/priority-queue
    title: Microsoft, Azure Architecture Center, Priority Queue pattern
    locator: context and problem; solution; problems and considerations; when to use this pattern; example
  - url: https://www.rabbitmq.com/docs/priority
    title: RabbitMQ documentation (version 4.4), Priority Queues
    locator: what is a priority queue; before adopting priority queues; quorum queues; priority queue behaviour; resource starvation; how priority queues work with consumers; interaction with other features
  - url: https://www.rabbitmq.com/docs/queues
    title: RabbitMQ documentation (version 4.4), Queues
    locator: message ordering; when messages can be reordered
---

## Intent

Let a workload process urgent requests faster than less urgent ones. The Azure page states the aim so: give priorities to the requests that go to a service, so that the workload processes the requests of high priority more quickly. The requests travel as messages in one or more queues. This helps an application that gives different service levels to different kinds of requests or customers.[^1]

## Problem

By default, most workloads process tasks in the order in which they arrive, in a first-in, first-out (FIFO) queue. This order does not show how important a task is.[^1] Some tasks need attention at once, and others can wait. If an urgent task waits behind many tasks that are not urgent, the users see a delay, and the workload can break a service-level agreement (SLA).[^1]

The forces are a fast path for urgent work, a fair chance for the other work, and simple operation.

## Solution

The producer gives each message a priority value, and the consumers process the messages by priority, not only by arrival order.[^1] The Azure page names two ways.[^1]

```text
  one queue:        producer --> [ h h n l l ] --> consumers
                                  ordered by priority

  multiple queues:  producer --+--> [ high ] --> pool of 3 consumers
                               +--> [ low  ] --> pool of 1 consumer
```

- **One queue.** All messages go to one queue, which delivers the messages of high priority first.[^1]
- **Multiple queues.** Each priority has its own queue. One pool of consumers can serve all the queues, and take from a lower queue only when the higher queues are empty. Or each queue has its own pool, and the queues of high priority get more consumers or faster ones.[^1]

The participants are the producer, which sets the priority, the queue or queues, and the consumers.[^1]

**Starvation.** With one shared pool, the higher queues always go first. Thus the messages of low priority can wait for a long time, or never be processed.[^1] RabbitMQ names this problem resource starvation.[^2] The Azure page suggests to increase the priority of old messages, so that the messages of low priority also get processed in the end.[^1] The program below has one consumer, which handles one message in each tick. For 20 ticks, a message of high priority arrives in each tick, and 5 messages of low priority arrive in the first 5 ticks.

```python run id=prio-starve
# One consumer handles one message per tick. For 20 ticks, a high message
# arrives on each tick, which uses all of the capacity. 5 low messages
# arrive in the first 5 ticks.
TICKS, AGE_LIMIT = 30, 6
arrivals = []
for t in range(20):
    arrivals.append((t, "high"))
    if t < 5:
        arrivals.append((t, "low"))


def run(pick):
    waiting, waits = [], {"high": [], "low": []}
    i = 0
    for t in range(TICKS):
        while i < len(arrivals) and arrivals[i][0] == t:
            waiting.append(arrivals[i])
            i += 1
        if waiting:
            m = pick(waiting, t)
            waiting.remove(m)
            waits[m[1]].append(t - m[0])
    return waits


def fifo(waiting, t):
    return waiting[0]


def strict(waiting, t):
    high = [m for m in waiting if m[1] == "high"]
    return high[0] if high else waiting[0]


def aging(waiting, t):
    old = [m for m in waiting if t - m[0] >= AGE_LIMIT]
    return old[0] if old else strict(waiting, t)


for name, pick in [("FIFO", fifo), ("strict priority", strict), ("priority, aging after 6 ticks", aging)]:
    w = run(pick)
    print("%-30s high: max wait %2d | low: max wait %2d" % (name + ":", max(w["high"]), max(w["low"])))
```

```output
FIFO:                          high: max wait  5 | low: max wait  5
strict priority:               high: max wait  0 | low: max wait 20
priority, aging after 6 ticks: high: max wait  5 | low: max wait  6
```

FIFO makes the urgent messages wait behind the others. Strict priority gives the urgent messages no wait at all, but a message of low priority waits 20 ticks, until the urgent messages stop. With aging, a message that has waited 6 ticks goes first. The messages of low priority then wait at most 6 ticks, and the urgent ones wait at most 5 ticks, as with FIFO. Aging moves the delay; it does not remove it, because the consumer has no spare capacity.

**The broker.** In RabbitMQ, classic queues and quorum queues support priorities. A publisher sets the priority of a message, and a larger number is a higher priority.[^2] A classic queue needs the `x-max-priority` argument, in the range 1 to 255, and it keeps an internal sub-queue for each priority. RabbitMQ recommends a small number of priorities, from 2 to 4.[^2] A quorum queue has 32 priority levels, from 0 to 31.[^2]

## Example

Each example file puts 8 messages with 3 priorities in one in-memory queue with a heap. The consumer takes one message in each tick. The messages of high priority go first, and the 3 messages of low priority wait 5 or 6 ticks. Messages with the same priority must keep their arrival order. A heap does not guarantee this, so the key of each message has its arrival number after the priority. Thus l1 comes before l2.

## Consequences

**Benefits.**

- **Different service levels.** The urgent work, or the work of a premium customer, gets more speed and availability.[^1]
- **Focus.** The work for reliability and performance can go to the most critical flows.[^1]
- **Fault isolation.** With a pool of consumers for each queue, a problem in one queue does not affect the other queues.[^1]

**Liabilities.**

- **Starvation.** With one pool and strict priority, a steady stream of urgent messages can delay the other messages without limit.[^1][^2]
- **Order.** A priority changes the order in which the consumers see the messages. RabbitMQ names priorities as one of the queue features that affect this order.[^3]
- **Prefetch.** A priority has an effect only on the messages that wait in the queue. If a consumer has already received 10 messages of low priority under its prefetch limit, a new urgent message waits behind them. RabbitMQ thus recommends manual acknowledgements and a prefetch limit on the consumers.[^2]
- **Cost.** In RabbitMQ, each priority level costs memory, disk and CPU.[^2] Some queue services charge for each send, receive and query, so more queues can cost more.[^1]
- **Interaction with other features.** In a classic priority queue, a message expires only when it reaches the head of the queue. A queue with a length limit drops messages from the head, so it can drop a message of high priority to make room for one of low priority.[^2]

## When not to use it

The Azure page names two cases.[^1]

- All work items are of the same importance, and FIFO order is more important than priority.
- Tasks of different priority depend on each other's order, and a change of order gives wrong results or needs complex coordination.

RabbitMQ also asks for simpler alternatives first, because a priority queue is harder to reason about than a FIFO queue. Its example is three queues, `priority.low`, `priority.medium` and `priority.high`, which avoid head-of-line blocking and keep the normal delivery order in each queue.[^2]

## Variants and related patterns

**Queue-based load leveling.** If the producers can send faster than the consumers process, the Azure page suggests the combination with a queue that buffers the bursts.[^1]

**Competing consumers.** The Azure page lists [[competing-consumers]] as a related pattern: several consumers read the same queue in parallel, and only one of them processes each message.[^1]

**Routing by priority.** The multiple-queue form sends each message to a queue by its priority value, as a [[content-based-router]] does. In the Azure example, a filter on a topic sends each message to the subscription of its priority.[^1]

## Known uses

- Azure: the example of the Azure page has a Service Bus topic with two subscriptions, one for each value of a `Priority` property, which SQL filters select. The consumer of high priority can scale to 200 instances, and the consumer of low priority to 40.[^1]
- RabbitMQ: classic and quorum queues with message priorities, and the alternative of one queue for each priority class.[^2]

## References
