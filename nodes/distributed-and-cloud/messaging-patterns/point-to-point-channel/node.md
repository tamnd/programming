---
tag: M7J197
kind: pattern
slug: point-to-point-channel
name: Point-to-Point Channel
aliases: [Point-to-Point Channel pattern, point-to-point messaging, PTP channel, message queue]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A point-to-point channel gives each message to exactly one of its receivers. Many receivers can read the channel to share the work, but no two of them get the same message at the same time.
family: integration
polarity: positive
intent: Send the message on a channel that gives each message to only one receiver, so that a command runs once and a document is processed once.
also_known_as: [queue, work queue, task queue]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003). JMS calls the same model point-to-point messaging and its channels queues.
see_also: [message-channel, publish-subscribe-channel, enterprise-integration-patterns, worker-queue, producer-consumer, channels, message-passing-concept, retry-with-backoff]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/PointToPointChannel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Point-to-Point Channel (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 4, Messaging Channels, Point-to-Point Channel; Chapter 10, Messaging Endpoints, Competing Consumers
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 4.1.2 Queue semantics
  - url: https://www.rabbitmq.com/tutorials/tutorial-two-python
    title: RabbitMQ tutorial 2, Work Queues (Python)
    locator: Round-robin dispatching; Message acknowledgment; Fair dispatch
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/service-bus-queues-topics-subscriptions
    title: Azure Service Bus queues, topics, and subscriptions
    locator: Choose between queues and topics; Queues; Receive modes
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/channels
    title: .NET documentation, System.Threading.Channels library
    locator: Producer/consumer conceptual programming model
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/BlockingQueue.html
    title: Java SE 21 API, Interface BlockingQueue
    locator: interface description
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/service-bus-dead-letter-queues
    title: Azure Service Bus dead-letter queues
    locator: Maximum delivery count
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/design.html
    title: Apache Kafka 4.1.0 documentation source, design.html
    locator: The Consumer, Consumer Position; Share groups
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingChannelsIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Messaging Channels
    locator: Message Channel Decisions, One-to-one or one-to-many
  - url: https://docs.spring.io/spring-integration/reference/channel/implementations.html
    title: Spring Integration reference, Message Channel Implementations
    locator: QueueChannel; DirectChannel
  - url: https://github.com/spring-projects/spring-integration/blob/v6.5.1/spring-integration-core/src/main/java/org/springframework/integration/channel/QueueChannel.java#L34-L46
    title: Spring Integration 6.5.1 source, QueueChannel.java
    locator: class QueueChannel, lines 34 to 46
---

## Intent

Send the message on a Point-to-Point Channel, "which ensures that only one receiver will receive a particular message".[^1] Many receivers can read the channel, and they share the messages between them.

## Problem

A shop sends a command message to charge a card for each order. Three copies of the payment program read the commands, so that the work goes on when one copy stops. The question is: "How can the caller be sure that exactly one receiver will receive the document or perform the call?"[^1]

The forces are these. A command must run once: a card that is charged by two receivers is charged twice. The receivers must not need to talk to each other to decide who takes a message, because that makes them depend on each other. And the system must be able to add receivers when the load goes up.

A [[publish-subscribe-channel]] fails the first force: it gives a copy to each subscriber. A list of receivers in the sender fails the second and the third: the sender must know each receiver and choose one.

## Solution

The channel keeps one queue of messages. Each message in the queue goes to one receiver. If two receivers ask for a message at the same time, the channel gives it to one of them, so "the receivers do not have to coordinate with each other".[^1]

```text
                                +--> payment w1   (charge-1, charge-4)
  shop --send--> [ charges ] ---+--> payment w2   (charge-2, charge-5)
                 one queue      +--> payment w3   (charge-3)
                 each message goes to one receiver
```

The participants are the sender, the channel and one or more receivers. The receivers that read one channel are Competing Consumers in the book.[^2] Jakarta Messaging states the rule for its queues: "A queue may have more than one consumer." and "Each message in the queue is delivered to only one consumer."[^3] It does not define how a provider shares the messages between the consumers.[^3] RabbitMQ sends each message to the next consumer by default, and the tutorial calls this round-robin.[^4] Azure Service Bus lists the queue as point-to-point, with a single consumer for each message.[^5]

**One receiver at a time, not exactly once.** A broker cannot know that a receiver has done the work until the receiver sends an acknowledgement. RabbitMQ re-queues a message when a consumer dies without an ack, and it can then "quickly redeliver it to another consumer".[^4] Service Bus, in peek lock mode, locks a message for one receiver and unlocks it when the lock times out, and its documentation calls this at-least once processing.[^5] The following program shows the result. Receiver `w2` charges a card and loses its connection before the ack, and the queue gives the same command to `w3`.

```python run id=redelivery
from collections import Counter, deque


class PointToPointQueue:
    """Each message goes to one receiver at a time, and stays unacked until the receiver acks it."""

    def __init__(self):
        self.ready = deque()
        self.unacked = {}  # delivery tag -> (receiver, message)
        self.next_tag = 1

    def send(self, message):
        self.ready.append(message)

    def deliver(self, receiver):
        if not self.ready:
            return None
        tag, self.next_tag = self.next_tag, self.next_tag + 1
        message = self.ready.popleft()
        self.unacked[tag] = (receiver, message)
        return tag, message

    def ack(self, tag):
        del self.unacked[tag]

    def connection_lost(self, receiver):
        # As in RabbitMQ: a message with no ack goes back to the queue.
        for tag, (who, message) in list(self.unacked.items()):
            if who == receiver:
                del self.unacked[tag]
                self.ready.appendleft(message)


q = PointToPointQueue()
for n in range(1, 6):
    q.send(f"charge-{n}")

handled = Counter()
receivers = ["w1", "w2", "w3"]
crashed = set()
turn = 0
while q.ready:
    who = receivers[turn % len(receivers)]
    turn += 1
    if who in crashed:
        continue
    tag, message = q.deliver(who)
    handled[message] += 1  # the side effect: a card is charged
    if message == "charge-2" and who == "w2":
        print(f"{who} handled {message}, then lost its connection before the ack")
        crashed.add(who)
        q.connection_lost(who)
        continue
    q.ack(tag)
    print(f"{who} handled {message}")

print("times handled:", dict(sorted(handled.items())))
```

```output
w1 handled charge-1
w2 handled charge-2, then lost its connection before the ack
w3 handled charge-2
w1 handled charge-3
w3 handled charge-4
w1 handled charge-5
times handled: {'charge-1': 1, 'charge-2': 2, 'charge-3': 1, 'charge-4': 1, 'charge-5': 1}
```

The channel did its job: no two receivers had the message at the same time. But the card was charged twice. A receiver of a point-to-point channel that gives at-least-once delivery must be idempotent, or it must find the duplicates. Service Bus says: "If your scenario can't tolerate duplicate processing, add extra logic in your application to detect duplicates."[^5] The other mode, receive and delete, marks the message as consumed when it is delivered. A message is then lost if the receiver fails, and Service Bus calls this at-most once processing.[^5]

## Example

Each example file starts three workers on one queue, sends nine jobs, and stops the workers. Each worker records the jobs that it handled. The program then shows that the nine jobs were handled, each one by one worker. Which worker got which job changes from run to run, so the programs print only the sorted list of jobs.

The examples use the queue or channel of the standard library in place of a broker. A Go channel, a .NET channel and a Java `BlockingQueue` each give a value to one receiver, so a pool of goroutines, tasks or threads that read one of them is a point-to-point channel inside a process.[^6][^7] The examples have no acknowledgements, because a value that a thread takes from a queue in memory cannot be lost by the network.

## Consequences

**Benefits.**

- **The work is shared without coordination.** Receivers can join and leave, and the channel spreads the messages over the receivers that are there.
- **The sender does not know the receivers.** It sends to the name of the queue.
- **Load leveling.** The queue holds the messages when the receivers are slow, and the receivers take them at their own rate.

**Liabilities.**

- **The order is not kept across receivers.** With one receiver, a queue gives the messages in order. With many, a later message can finish before an earlier one, because each receiver works at its own rate. Jakarta Messaging adds that a consumer may not receive all the messages on the queue when other consumers share it.[^3]
- **Duplicates under at-least-once delivery.** As the program above shows, a redelivered message runs again.
- **Unfair dispatch.** Round-robin dispatch gives each consumer the same number of messages, not the same amount of work. RabbitMQ uses a prefetch limit, `prefetch_count=1`, so that a busy worker gets no new message until it acks the last one.[^4]
- **A message that always fails.** A message that makes every receiver fail goes back to the queue again and again. Service Bus counts the deliveries, and when the count exceeds the limit, which is 10 by default, it moves the message to the dead-letter queue.[^8] With a broker that does not do this, the receivers must count and move the message.

## When not to use it

**When each receiver must see each message.** An event that many programs react to, such as an order that was placed, needs a [[publish-subscribe-channel]]. On a queue, each program would get only some of the events.

**When the order of all messages matters.** A queue with competing receivers does not keep a global order. Use one receiver, or a channel that keeps an order for each key: a Kafka topic gives each partition to exactly one consumer in a consumer group at a time.[^9]

**For a request that needs an answer now.** A queue adds a delay and needs a reply channel for the answer.

## Variants and related patterns

**Competing Consumers.** The receivers of one point-to-point channel, which share its messages.[^2] See also [[worker-queue]] and [[producer-consumer]], the same structure inside one program.

**Publish-Subscribe Channel.** The other kind of channel: each subscriber gets a copy.[^10] A topic subscription with many consumers combines the two. In Jakarta Messaging, a shared subscription gives each message in the subscription to only one consumer.[^3]

**Kafka consumer groups.** Kafka divides a topic into partitions, and each partition "is consumed by exactly one consumer within each subscribing consumer group at any given time".[^9] Each group is then a point-to-point reader of the topic, with the order kept for each partition. Kafka 4.1 adds share groups, where the consumers of a group "cooperatively consume records, and partitions may be assigned to multiple consumers".[^9]

**Spring Integration channels.** `QueueChannel` wraps a queue and has point-to-point semantics. `DirectChannel` is also point-to-point, but it calls one subscribed handler in the thread of the sender.[^11]

**Retries.** A receiver that fails can return the message to the queue at once, or after a delay. See [[retry-with-backoff]].

## Known uses

- Spring Integration: [QueueChannel.java at v6.5.1](https://github.com/spring-projects/spring-integration/blob/v6.5.1/spring-integration-core/src/main/java/org/springframework/integration/channel/QueueChannel.java#L34-L46) puts each message in a `BlockingQueue`, and each `receive` takes a message out, so only one receiver gets it.[^12]
- RabbitMQ work queues: a queue that many workers consume, with acks and a prefetch limit.[^4]
- Azure Service Bus queues: a single consumer for each message, with peek lock or receive and delete.[^5]
- Jakarta Messaging queues, with any number of consumers for each queue.[^3]

## References
