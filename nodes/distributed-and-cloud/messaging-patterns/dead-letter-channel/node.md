---
tag: XMPVT7
kind: pattern
slug: dead-letter-channel
name: Dead Letter Channel
aliases: [Dead Letter Channel pattern, dead letter queue, dead-letter queue, DLQ, dead message queue, dead-letter topic]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A dead letter channel holds the messages that the messaging system cannot or should not deliver, so that one bad message does not block the others and a person can look at it later.
family: integration
polarity: positive
intent: Move a message that cannot be delivered or processed to a separate channel, with the reason, so that the other messages go on and the failed message is kept for a later decision.
also_known_as: [dead letter queue, DLQ, dead message queue, dead-letter topic]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003). Brokers of that time already had dead message queues, and the cloud queues of today have dead-letter queues and topics.
see_also: [point-to-point-channel, message-channel, guaranteed-delivery, enterprise-integration-patterns, retry-with-backoff, publish-subscribe-channel, worker-queue]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/DeadLetterChannel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Dead Letter Channel (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; examples for Amazon SQS and Kafka Connect
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 4, Messaging Channels, Dead Letter Channel and Invalid Message Channel
  - url: https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-dead-letter-queues.html
    title: Amazon SQS Developer Guide, Using dead-letter queues in Amazon SQS
    locator: Using policies for dead-letter queues; Understanding message retention periods for dead-letter queues
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/service-bus-dead-letter-queues
    title: Azure Service Bus dead-letter queues
    locator: Maximum delivery count; Time to live; Application-level dead-lettering
  - url: https://cloud.google.com/pubsub/docs/handling-failures
    title: Google Cloud Pub/Sub documentation, Dead-letter topics
    locator: How dead-letter topics work; How maximum delivery attempts are calculated
  - url: https://www.rabbitmq.com/docs/dlx
    title: RabbitMQ documentation, Dead Letter Exchanges
    locator: What is a Dead Letter Exchange; Dead-letter cycle; Delayed Retries; Safety
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/InvalidMessageChannel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Invalid Message Channel (problem and solution under CC BY)
    locator: problem and solution statements
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/dead-letter-channel.adoc
    title: Apache Camel 4.14.0 documentation source, Dead Letter Channel
    locator: Redelivery; Redelivery default values
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/errorhandler/DeadLetterChannel.java#L29-L33
    title: Apache Camel 4.14.0 source, DeadLetterChannel.java
    locator: class DeadLetterChannel, lines 29 to 33
---

## Intent

Give the messaging system a place for the messages that it cannot deliver. The book asks: "What will the messaging system do with a message it cannot deliver?"[^1][^2] A Dead Letter Channel keeps such a message out of the way of the others, with the reason that it failed, until a person or a program decides what to do with it.

## Problem

A payment program reads a queue of payments. One message has an amount that the program cannot read. The program throws an exception, does not acknowledge the message, and the broker gives the message to it again. The message fails again, and again.

The forces are these. The broker must not lose a message only because a receiver failed once, because the failure can be temporary: a database that is down for a minute, or a network error. But a message that always fails must not stay in the queue for ever. It uses the work of the receivers, it can delay the messages after it, and it hides the error from the people who can fix it. And the broker cannot know if a failure is temporary or permanent: only a count of the attempts, or a time limit, can tell it to stop.

## Solution

"When a messaging system determines that it cannot or should not deliver a message, it may elect to move the message to a Dead Letter Channel".[^1] The messaging system also records information with the message, such as the channel where the message was to go.[^1]

```text
  sender --> [ payments ] --> receiver
                  |   ^         | fails, no ack
                  |   +---------+ deliver again (count + 1)
                  |
                  +-- count = limit --> [ payments.dead ] --> operator, alert, redrive
                                         reason, source, count
```

The participants are the sender, the source channel, the receiver, the dead letter channel and the program or person that reads the dead letters. The messaging system usually decides when a message is dead. These are the usual rules:

- **Too many deliveries.** In Amazon SQS, "The maxReceiveCount is the number of times a consumer can receive a message from a source queue before it is moved to a dead-letter queue."[^3] Azure Service Bus counts the deliveries under a peek lock, and when the count is more than the limit, which is 10 by default, it moves the message and sets the reason `MaxDeliveryCountExceeded`.[^4] In Google Cloud Pub/Sub, the maximum number of delivery attempts is 5 by default, and it can be from 5 to 100.[^5]
- **A rejection.** RabbitMQ dead-letters a message when a consumer rejects it and does not ask to requeue it. It also dead-letters a message that expires because of a per-message TTL, a message that is dropped because the queue is over its length limit, and a message that is returned to a quorum queue more times than the delivery limit.[^6]
- **Expiry.** When dead-lettering is on in Service Bus, "all expiring messages are moved to the DLQ", with the reason `TTLExpiredException`.[^4]
- **A decision of the receiver.** In Service Bus, "applications can use the DLQ to explicitly reject unacceptable messages."[^4] The book gives a separate pattern for this case, the Invalid Message Channel: "The receiver should move the improper message to an Invalid Message Channel", which is "a special channel for messages that could not be processed by their receivers".[^7]

The program below follows a message through a dead letter channel and back. The receiver cannot read the amount `12,50`. After 3 deliveries, the queue moves the message to `orders.dead` with the reason and the source. An operator reads the reason, the handler is fixed, and the dead letter is moved back to its source queue. Amazon SQS calls this last step a dead-letter queue redrive.[^3]

```python run id=redrive
from collections import deque

MAX_DELIVERIES = 3


class Queue:
    def __init__(self, name, dead_letter=None):
        self.name = name
        self.items = deque()
        self.dead_letter = dead_letter

    def send(self, body):
        self.items.append({"body": body, "deliveries": 0, "headers": {}})


def consume(queue, handler):
    while queue.items:
        msg = queue.items.popleft()
        msg["deliveries"] += 1
        try:
            handler(msg["body"])
        except ValueError as e:
            print(f"failed {msg['body']['id']}, delivery {msg['deliveries']}")
            if msg["deliveries"] < MAX_DELIVERIES:
                queue.items.append(msg)
            else:
                msg["headers"] = {"reason": str(e), "source": queue.name}
                queue.dead_letter.items.append(msg)


def handle_v1(order):
    total = float(order["total"])
    print(f"handled {order['id']} total {total:.2f}")


def handle_v2(order):
    total = float(order["total"].replace(",", "."))
    print(f"handled {order['id']} total {total:.2f}")


dead = Queue("orders.dead")
orders = Queue("orders", dead_letter=dead)
orders.send({"id": "order-1", "total": "10.00"})
orders.send({"id": "order-2", "total": "12,50"})
orders.send({"id": "order-3", "total": "7.25"})
consume(orders, handle_v1)

# An operator reads the dead letters and finds the cause.
for msg in dead.items:
    h = msg["headers"]
    print(f"dead letter {msg['body']['id']} from {h['source']} "
          f"after {msg['deliveries']} deliveries: {h['reason']}")

# The handler is fixed. Redrive: move the dead letters back to their source.
moved = 0
while dead.items:
    msg = dead.items.popleft()
    orders.send(msg["body"])
    moved += 1
print(f"redrive {moved} message to orders")
consume(orders, handle_v2)
print(f"dead letters left: {len(dead.items)}")
```

```output
handled order-1 total 10.00
failed order-2, delivery 1
handled order-3 total 7.25
failed order-2, delivery 2
failed order-2, delivery 3
dead letter order-2 from orders after 3 deliveries: could not convert string to float: '12,50'
redrive 1 message to orders
handled order-2 total 12.50
dead letters left: 0
```

**Dead letters must be read.** A dead letter channel only moves a failure to another place. In Service Bus, "There's no automatic cleanup of the DLQ."[^4] A message there stays until a program receives it and completes it.[^4] A team that sends dead letters to a queue that nobody reads loses the messages as surely as a broker that drops them, only more slowly. Give each dead letter channel an owner, an alert on its length, and a way to redrive or delete its messages.

## Example

Each example file runs a queue in memory with a limit of 3 deliveries. The handler fails for the message `pay-2`, because its amount is `x`. The queue puts the message back at the end after each failure, and after the third failure it moves the message to a list of dead letters with the reason and the source. The other two messages are processed, and they do not wait for the bad one.

The examples use the standard library only, in place of a broker. With a broker, the delivery count, the limit and the move are usually the work of the broker, and the receiver only fails or rejects the message.[^3][^6][^4]

## Consequences

**Benefits.**

- **The queue goes on.** A message that always fails leaves the queue after a limit, so it stops using the receivers.
- **No message is lost because of a bad message.** The message and its reason are kept for a person or a program.
- **A place to watch.** The length of the dead letter channel is a simple signal that something is wrong.

**Liabilities.**

- **The order changes.** A message that leaves the queue is no longer in its place in the order. Amazon SQS says: "Don't use a dead-letter queue with a FIFO queue if you don't want to break the exact order of messages or operations."[^3]
- **Temporary failures can end in the channel.** A limit that is too low moves messages that a retry would process. SQS advises to set the `maxReceiveCount` "high enough to allow for sufficient retries".[^3]
- **The messages can expire there.** For SQS standard queues, the expiry of a message is based on the time it was first put in the source queue, so "it is a best practice to always set the retention period of a dead-letter queue to be longer than the retention period of the original queue."[^3]
- **The move can fail.** RabbitMQ says that dead-lettering is a form of publishing, and "using DLX in a clustered RabbitMQ environment is not guaranteed to be safe."[^6] Pub/Sub counts delivery attempts on a best-effort basis, so a subscriber can get a message more times than the limit.[^5]
- **Cycles.** A dead letter channel that sends the message back to its source can make a loop. RabbitMQ finds such a cycle and drops the message "if there was no rejection in the entire cycle".[^6]

## When not to use it

**As the retry mechanism.** A dead letter channel is for the messages that cannot be processed. For a temporary failure, use a retry with a delay first; see [[retry-with-backoff]]. RabbitMQ compares the two and says that "Dead lettering remains the right tool for messages that cannot be processed at all".[^6]

**When a lost message is acceptable and nobody will read the dead letters.** For data such as a periodic measurement, the next message replaces the last one. Discard the bad message and count it.

**When the order of all messages is a requirement.** A message that leaves the queue breaks the order. Stop the queue and alert a person, or use a channel for each key so that only one key stops.

## Variants and related patterns

**Invalid Message Channel.** The receiver, not the messaging system, decides that a message is bad and moves it. The book asks: "How can a messaging receiver gracefully handle receiving a message that makes no sense?"[^7] Some brokers use one channel for both cases, and the reason tells them apart: Service Bus sets its own reasons and lets applications define their own codes.[^4]

**Dead-letter exchange and dead-letter topic.** RabbitMQ publishes a dead letter to an exchange, which routes it to one or more queues, and adds an `x-death` header with the history of the message.[^6] Pub/Sub forwards a dead letter to a topic, and it wraps the original message in a new one with attributes that identify the source subscription.[^5]

**Redelivery before the dead letter channel.** In Apache Camel, the Dead Letter Channel is an error handler that tries again with a redelivery policy, and "Only when all redelivery attempts have failed, then the message is moved to the dead letter queue."[^8] The default is no redelivery: `maximumRedeliveries` is 0.[^8]

**Guaranteed Delivery.** A dead letter channel keeps a failed message, and [[guaranteed-delivery]] keeps the messages that are on the way. Together they make sure that a message is either delivered or kept.

**Point-to-Point Channel.** The redelivery that comes before a dead letter is the at-least-once delivery of a [[point-to-point-channel]].

## Known uses

- Apache Camel: [DeadLetterChannel.java at camel-4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/errorhandler/DeadLetterChannel.java#L29-L33) is the error handler that moves a message to a dead letter endpoint after it tries to redeliver it with a `RedeliveryPolicy`.[^9]
- Amazon SQS dead-letter queues, with a redrive policy and a redrive back to the source.[^3] The book site gives SQS as an example of the pattern.[^1]
- RabbitMQ dead-letter exchanges.[^6]
- Azure Service Bus: a dead-letter subqueue for each queue and each topic subscription.[^4]
- Google Cloud Pub/Sub dead-letter topics.[^5]

## References
