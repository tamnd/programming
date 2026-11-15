---
tag: B9TJ6Z
kind: pattern
slug: idempotent-receiver
name: Idempotent Receiver
aliases: [Idempotent Receiver pattern, idempotent consumer, de-duplicating receiver, message de-duplication]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  An idempotent receiver can get the same message more than once with the same result as once, because it finds duplicates by an id or because its messages have an effect that a repeat does not change.
family: integration
polarity: positive
intent: Make duplicate messages safe, by a receiver that removes duplicates by their message id, or by messages whose effect is the same when they are applied once or many times.
also_known_as: [idempotent consumer]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on messaging endpoints. Apache Camel calls its implementation the Idempotent Consumer.
see_also: [guaranteed-delivery, competing-consumers, dead-letter-channel, correlation-identifier, retry-with-backoff, command-message, event-message, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/IdempotentReceiver.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Idempotent Receiver (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 10, Messaging Endpoints, Idempotent Receiver
  - url: https://www.rabbitmq.com/tutorials/tutorial-two-python
    title: RabbitMQ tutorials, Work Queues (Python)
    locator: message acknowledgment
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingEndpointsIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Messaging Endpoints
    locator: message consumer patterns
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/idempotentConsumer-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Idempotent Consumer EIP
    locator: introduction; Idempotent Consumer implementations
  - url: https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/FIFO-queues-exactly-once-processing.html
    title: Amazon Simple Queue Service Developer Guide, Exactly-once processing in Amazon SQS
    locator: whole page
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/handler-advice/idempotent-receiver.adoc
    title: Spring Integration 7.1.1 reference source, Idempotent Receiver Enterprise Integration Pattern
    locator: introduction; MetadataStoreSelector
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/competing-consumers
    title: Microsoft, Azure Architecture Center, Competing Consumers pattern
    locator: problems and considerations
---

## Intent

Let a receiver get the same message twice, or more often, with the same result as once. The book asks: "How can a message receiver deal with duplicate messages?"[^1][^2] The answer: "Design a receiver to be an Idempotent Receiver", "one that can safely receive the same message multiple times."[^1]

## Problem

A message can arrive more than once even when the sender sends it once.[^1] In the RabbitMQ tutorial, when a consumer stops before it sends its ack, RabbitMQ delivers all its unacknowledged messages again.[^3] So a consumer that has done the work but crashes before the ack gets the message a second time. A sender that sends again after a timeout can also make a copy.

The book notes that receivers often assume that each message comes exactly once.[^4] A receiver that adds money to an account, sends an e-mail or ships goods then does it twice.

The forces are these. Delivery with no loss means delivery with possible copies. The receiver must find a copy at a low cost. Its record of processed messages must not grow for ever. And after a crash, the record and the effect must agree.

## Solution

The book gives two ways: remove the duplicates explicitly, or define the meaning of the messages so that a repeat has no further effect.[^1]

```text
  [ queue ] ---> receiver: id in the store? --yes--> ack, and ignore it
                                 |
                                 no --> apply the effect and add the id (one transaction) --> ack
```

**De-duplication.** Each message carries a unique id. The receiver keeps the ids of the messages that it has processed. For each message, it looks up the id; if the id is there, it acknowledges the message and does nothing else.

**Idempotent meaning.** *Set the status of order 17 to paid* gives the same state when it comes twice. *Add 10 to the balance* does not. A message with an absolute value, or an update keyed by the id of the message, needs no store of ids.

**The order of the steps.** Apache Camel adds the message id to its repository eagerly, before the processing ends, so that it also finds a copy of a message that is still in progress. When the processing fails, Camel removes the id again.[^5] The program below shows why the order matters when the process stops between the two steps. The balance and the ids are in a store that survives the crash, and the broker then delivers m1 again.

```python run id=crash-order
# m1 is delivered twice: the broker got no ack for the first delivery.
stream = [("m1", 10), ("m2", 5), ("m1", 10)]


def naive():
    balance = 0
    for _, amount in stream:
        balance += amount
    return balance


def dedup():
    balance, seen = 0, set()
    for mid, amount in stream:
        if mid in seen:
            continue
        balance += amount
        seen.add(mid)
    return balance


print("no check:", naive())
print("de-dup by message id:", dedup())


# Now the process crashes once, after the first step for the first delivery of m1.
# The balance and the set of seen ids are both in a store that survives the crash.
def crash_run(steps):
    db = {"balance": 0, "seen": set()}
    crashed = False
    for mid, amount in stream:
        if mid in db["seen"]:
            continue
        staged = []
        for i, step in enumerate(steps):
            if i == 1 and mid == "m1" and not crashed:
                crashed = True  # the process stops; the broker delivers m1 again
                break
            if step == "mark":
                db["seen"].add(mid)
            elif step == "apply":
                db["balance"] += amount
            elif step == "stage both":
                staged = [amount, mid]
            elif step == "commit":
                db["balance"] += staged[0]
                db["seen"].add(staged[1])
    return db["balance"]


print("mark, then apply:", crash_run(["mark", "apply"]))
print("apply, then mark:", crash_run(["apply", "mark"]))
print("one transaction for both:", crash_run(["stage both", "commit"]))
```

```output
no check: 25
de-dup by message id: 15
mark, then apply: 5
apply, then mark: 25
one transaction for both: 15
```

With no check, m1 counts twice. With the id marked first, the crash loses m1, because its second delivery looks like a copy. With the effect applied first, the crash gives a double effect. Only when the effect and the id are written in one transaction is the result 15.

## Example

Each example file receives payments for orders and keeps the ids of the last three messages. The copy of pay-1 that comes at once is ignored. The copy of pay-2 comes after three other messages, when its id has left the window, so the payment counts twice. The window must be longer than the longest time in which a copy can come. Amazon SQS FIFO queues, for example, use a deduplication interval of 5 minutes.[^6]

## Consequences

**Benefits.**

- **Safe redelivery.** The broker and the senders can retry, and each message still has its effect once.
- **Simple recovery.** After a crash, the messages can be delivered again with no manual repair.
- **One place for the rule.** The check is in the receiver, not in each sender.

**Liabilities.**

- **A store of ids.** It must survive a restart, and the competing consumers of one queue must share it. Camel has many repositories, from one in memory to Cassandra, JDBC, Kafka, MongoDB and Redis.[^5]
- **A limit on the store.** A store with no limit grows for ever. A store with a limit lets a late copy through, as the example shows.
- **A stable id.** The sender must keep the id when it sends again. SQS can make the id from the content: it uses a SHA-256 hash of the body of the message, but not its attributes.[^6]
- **More work for each message.** Each message needs a lookup and a write.
- **Application logic.** Spring Integration calls it "a functional pattern" and says that "the whole idempotency logic should be implemented in the application."[^7]

## When not to use it

**When the effect is already idempotent.** A message that sets a value or deletes a record by its key can come twice with no harm, and a store of ids only adds cost.[^1]

**When a copy does no harm.** A message that only refreshes a cache or a display can be processed again.

**When the broker removes the copies that matter.** SQS FIFO queues do not add a copy when a sender sends again within the 5-minute deduplication interval.[^6] Check which copies the feature covers before you remove the check from the receiver.

## Variants and related patterns

**Message filter.** Camel describes its Idempotent Consumer as a message filter that removes duplicates.[^5] Spring Integration can instead mark a copy with the header `duplicateMessage` set to `true`, so that the handler decides, or send it to a discard channel.[^7]

**A high-water mark.** In place of a set of ids, Spring Integration can keep one value for each key, such as the last line number of a file, and accept a message only when its value is newer.[^7]

**Guaranteed delivery and retries.** [[guaranteed-delivery]] and a sender with [[retry-with-backoff]] make copies, and an idempotent receiver makes them safe. The Azure page on [[competing-consumers]] asks for idempotent processing for the same reason.[^8]

## Known uses

- Apache Camel: the Idempotent Consumer EIP with a pluggable `IdempotentRepository`, as the [Idempotent Consumer page of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/idempotentConsumer-eip.adoc) lists.[^5]
- Spring Integration: the `IdempotentReceiverInterceptor` with a `MetadataStoreSelector`, applied to an existing endpoint.[^7]
- Amazon SQS FIFO queues: a message deduplication ID from the sender, or content-based deduplication.[^6]

## References
