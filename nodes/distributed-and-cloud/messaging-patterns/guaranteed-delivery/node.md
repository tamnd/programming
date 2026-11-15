---
tag: DBNWZH
kind: pattern
slug: guaranteed-delivery
name: Guaranteed Delivery
aliases: [Guaranteed Delivery pattern, guaranteed messaging, persistent messaging, durable messaging, reliable messaging, store and forward]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  Guaranteed delivery makes a message persistent: the messaging system writes it to disk before it accepts it, and keeps it there until the receiver acknowledges it, so a crash does not lose it.
family: integration
polarity: positive
intent: Make each message persistent, so that a message that the messaging system accepted is not lost when a process or a computer of the messaging system crashes.
also_known_as: [persistent delivery, durable messaging, store and forward]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003). The JMS specification and its successor, Jakarta Messaging, give the same choice as the delivery mode PERSISTENT.
see_also: [dead-letter-channel, point-to-point-channel, publish-subscribe-channel, message-channel, enterprise-integration-patterns, retry-with-backoff, producer-consumer]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/GuaranteedMessaging.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Guaranteed Delivery (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 4, Messaging Channels, Guaranteed Delivery
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 7.7 Message delivery mode; Table 4-1 Pub/sub reliability
  - url: https://www.rabbitmq.com/docs/confirms
    title: RabbitMQ documentation, Consumer Acknowledgements and Publisher Confirms
    locator: When Will Published Messages Be Confirmed by the Broker; Publisher Confirms and Guaranteed Delivery
  - url: https://www.rabbitmq.com/docs/reliability
    title: RabbitMQ documentation, Reliability Guide
    locator: Data Safety on the RabbitMQ Side; Data Safety on the Publisher Side
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/design.html
    title: Apache Kafka 4.1.0 documentation source, design.html
    locator: Message Delivery Semantics; Replication
  - url: https://github.com/apache/kafka/blob/4.1.0/clients/src/main/java/org/apache/kafka/clients/producer/ProducerConfig.java#L124-L138
    title: Apache Kafka 4.1.0 source, ProducerConfig.java
    locator: ACKS_DOC, lines 124 to 138; ACKS_CONFIG default, lines 383 to 386; ENABLE_IDEMPOTENCE_DOC, lines 336 to 342
---

## Intent

Make a message survive a crash of the messaging system. The book asks: "How can the sender make sure that a message will be delivered, even if the messaging system fails?"[^1][^2] The answer is to write the message to a store that outlives the process, before the messaging system tells the sender that it has the message.

## Problem

A shop sends an order to a warehouse on a queue. The broker keeps its queues in memory. The broker process stops after it accepts the order and before the warehouse reads it. When the broker starts again, its queues are empty. The shop thinks that the order was sent, and the warehouse never sees it.

The forces are these. Memory is fast, and a disk is slow: each write that must reach the disk adds a delay to each send. The sender must know when it can forget a message, so the messaging system must tell it when the message is safe. And a message can be lost at each step: in the sender, on the network, in the broker, and in the receiver before it has done the work.

## Solution

"Use Guaranteed Delivery to make messages persistent so that they are not lost even if the messaging system crashes."[^1] The messaging system has a data store on each computer where it runs. A send is complete only when the message is in the store of the sender. The message stays in a store until the next store has it, and it leaves the last store only when the receiver acknowledges it.[^1]

```text
  sender                   messaging system                     receiver
    | send m1 ---------> write m1 to disk, fsync
    | <--------- confirm (m1 is safe)
    |                       deliver m1 ---------------------->  do the work
    |                       delete m1 from disk <------------- ack m1
```

The participants are the sender, the persistent store of the messaging system, and the receiver. Two signals make the chain: a confirm from the store to the sender, and an acknowledgement from the receiver to the store.

**Jakarta Messaging.** Jakarta Messaging gives the choice as a delivery mode. "The NON_PERSISTENT mode is the lowest overhead delivery mode because it does not require that the message be logged to stable storage."[^3] Thus "A Jakarta Messaging provider failure can cause a NON_PERSISTENT message to be lost."[^3] For the PERSISTENT mode, a provider "must deliver a PERSISTENT message once-and-only-once", and the specification adds: "This means a Jakarta Messaging provider failure must not cause it to be lost, and it must not deliver it twice."[^3] On a topic, the mode is not sufficient: a subscriber that is not durable misses the messages that are sent when it is not active, also when they are PERSISTENT.[^3]

**RabbitMQ.** A RabbitMQ publisher puts its channel in confirm mode, and the broker confirms each message. "For persistent messages routed to durable queues, this means persisting to disk".[^4] The queue must be durable and the message must be persistent; the reliability guide calls this critically important for important data.[^5] The disk write is not at once: the message store writes to disk "in batches after an interval (a few hundred milliseconds) to minimise the number of fsync(2) calls", and "A RabbitMQ node can lose persistent messages if it fails before said messages are written to disk."[^4] So the publisher must keep a message until the confirm comes.

**Kafka.** Kafka puts the guarantee in copies on many brokers, not in a disk write on one. "All data is immediately written to a persistent log on the filesystem without necessarily flushing to disk."[^6] "A message is considered committed only when all replicas in the in-sync replicas (ISR) for that partition have applied it to their log."[^6] A committed message is not lost while one broker that has a copy of its partition stays alive.[^6] The producer setting `acks=all` waits for the full set of in-sync replicas, and the source code says of it: "This is the strongest available guarantee."[^7] In Kafka 4.1, `all` is the default.[^7]

**At least once, not exactly once.** The store keeps a message until the ack, so a receiver that does the work and stops before the ack gets the message again. The program below shows this. A durable queue keeps a log on disk with one line for each send and each ack, and it calls `fsync` after each line. The receiver ships parcel `m2` and stops before it can ack it. When the receiver starts again, it reads the log, and `m2` is still pending.

```python run id=crash-before-ack
import os
import tempfile


class DurableQueue:
    """A queue whose state is a log on disk: one line for each send and each ack."""

    def __init__(self, path):
        self.path = path
        self.pending = {}
        if os.path.exists(path):
            with open(path) as f:
                for line in f:
                    op, mid, *body = line.rstrip("\n").split(" ", 2)
                    if op == "send":
                        self.pending[mid] = body[0]
                    else:
                        self.pending.pop(mid, None)

    def _log(self, line):
        with open(self.path, "a") as f:
            f.write(line + "\n")
            f.flush()
            os.fsync(f.fileno())

    def send(self, mid, body):
        self._log(f"send {mid} {body}")  # returns only when the line is on disk
        self.pending[mid] = body

    def ack(self, mid):
        self._log(f"ack {mid}")
        del self.pending[mid]


shipped = []


def ship(mid, body):
    shipped.append(mid)
    print(f"ship {mid}: {body}")


path = os.path.join(tempfile.mkdtemp(), "parcels.log")
q = DurableQueue(path)
q.send("m1", "parcel to Hanoi")
q.send("m2", "parcel to Hue")
q.send("m3", "parcel to Da Nang")

mid, body = next(iter(q.pending.items()))
ship(mid, body)
q.ack(mid)
mid, body = next(iter(q.pending.items()))
ship(mid, body)
print("crash after the work, before the ack")
del q

q = DurableQueue(path)  # the receiver starts again and reads the log
print("pending after restart:", " ".join(q.pending))
for mid, body in list(q.pending.items()):
    ship(mid, body)
    q.ack(mid)
print("pending:", len(q.pending))
print("m2 shipped", shipped.count("m2"), "times")
os.remove(path)
```

```output
ship m1: parcel to Hanoi
ship m2: parcel to Hue
crash after the work, before the ack
pending after restart: m2 m3
ship m2: parcel to Hue
ship m3: parcel to Da Nang
pending: 0
m2 shipped 2 times
```

No message was lost, but `m2` was shipped twice. The same can happen on the side of the sender. RabbitMQ says that a producer that recovers from a failure "should retransmit any messages for which an acknowledgement has not been received from the broker", and that this can make duplicates, "because the broker might have sent a confirmation that never reached the producer".[^5] A receiver of a guaranteed channel must be idempotent, or it must find the duplicates. Kafka gives one answer for the producer side: with idempotence on, "the producer will ensure that exactly one copy of each message is written in the stream", and "Idempotence is enabled by default if no conflicting configurations are set."[^7]

## Example

Each example file keeps two queues: one in memory and one with a log on disk. It sends three orders to each, then simulates a crash: it makes new objects, and the durable queue reads its log again. The queue in memory has lost the three orders, and the durable queue still has them. The receiver then takes one order and acks it, and after a second crash the durable queue has the other two.

Each example forces the data to the disk after each line: `os.fsync` in Python, `File.Sync` in Go, `FileDescriptor.sync` in Java and `FileStream.Flush(true)` in C#. A write without it can stay in the cache of the operating system, and a crash of the computer can lose it. A crash of the process alone does not lose such a write.

## Consequences

**Benefits.**

- **A crash does not lose accepted messages.** The sender can forget a message when the confirm comes.
- **The sender and the receiver need not run at the same time.** A message waits on disk until the receiver is there.
- **A clear contract.** The confirm and the ack tell each side when its part is done.

**Liabilities.**

- **Speed.** Each persistent message costs a disk write, or a write on many brokers. Brokers write in batches to reduce the number of `fsync` calls, and this adds a delay to each confirm.[^4]
- **Disk space.** Messages for a receiver that is down stay on disk until it comes back. A durable subscription for a program that never comes back holds its messages until somebody deletes the subscription.
- **Duplicates.** A crash between the work and the ack, or between the confirm and its arrival at the sender, gives the same message twice, as the program shows.
- **The guarantee has limits.** A persistent message in a queue that is not durable, a broker that fails before its batch write, or a store on a disk that fails can still lose messages.[^4][^5] Replication across computers covers the last case.[^6]

## When not to use it

**For data that is soon out of date.** A position of a vehicle or a price that comes each second is replaced by the next message. Jakarta Messaging calls the NON_PERSISTENT mode the lowest overhead mode;[^3] use it, or a channel in memory, for such data.

**When the sender can produce the message again.** If the data is in a database of the sender, a lost message can be sent again from it, and a fast channel in memory can be sufficient.

**As the only protection.** Guaranteed delivery keeps the message in the messaging system. It does not make the receiver do the work. Add a [[dead-letter-channel]] for the messages that always fail, and idempotent receivers for the duplicates.

## Variants and related patterns

**Durable subscriber.** On a [[publish-subscribe-channel]], a persistent message reaches a subscriber that was not active only if the subscription is durable.[^3]

**Store and forward.** The chain of stores in the solution is store and forward: each computer keeps the message until the next one has it.[^1]

**Transactions.** In standard AMQP 0-9-1, the only way to make sure that a message is not lost is a transactional channel. RabbitMQ says that transactions "are unnecessarily heavyweight and decrease throughput by a factor of 250", and it gives publisher confirms for the same need.[^4]

**Dead Letter Channel.** A [[dead-letter-channel]] keeps the persistent messages that cannot be delivered.

**Retries.** A sender that has no confirm sends again, with a delay; see [[retry-with-backoff]].

## Known uses

- Apache Kafka: [ProducerConfig.java at 4.1.0](https://github.com/apache/kafka/blob/4.1.0/clients/src/main/java/org/apache/kafka/clients/producer/ProducerConfig.java#L124-L138) gives the meaning of `acks=0`, `acks=1` and `acks=all`; the same file sets `all` as the default.[^7]
- Jakarta Messaging: the delivery modes PERSISTENT and NON_PERSISTENT.[^3]
- RabbitMQ: durable queues, persistent messages and publisher confirms.[^4][^5]

## References
