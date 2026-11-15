---
tag: MB6M4W
kind: pattern
slug: messaging-bridge
name: Messaging Bridge
aliases: [Messaging Bridge pattern, message bridge, queue bridge, shovel]
area: distributed-and-cloud/messaging-patterns
level: advanced
summary: >-
  A messaging bridge connects two messaging systems; it reads each message on a channel of one system and sends it to the matching channel of the other, and the applications on each side need no change.
family: integration
polarity: positive
intent: Make the messages of one messaging system available on another, by a pair of channel adapters that read from a channel on one side, map the channel and the format, and send to the matching channel on the other side.
also_known_as: [message bridge]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on messaging channels. The Azure Architecture Center describes it as one of its cloud design patterns.
see_also: [message-channel, message-bus, message-translator, competing-consumers, idempotent-receiver, guaranteed-delivery, dead-letter-channel, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingBridge.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Messaging Bridge (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 4, Messaging Channels, Messaging Bridge
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/messaging-bridge
    title: Microsoft, Azure Architecture Center, Messaging Bridge pattern
    locator: context and problem; solution; issues and considerations; when to use this pattern; example; next steps
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/ChannelAdapter.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Channel Adapter (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://www.rabbitmq.com/docs/shovel
    title: RabbitMQ documentation, Shovel plugin
    locator: overview; why use Shovel; what does a shovel do; consumption; shovels and queue federation
---

## Intent

Connect two messaging systems at the level of their channels. The book asks: "How can multiple messaging systems be connected so that messages available on one are also available on the others?"[^1][^2] The answer: "Use a Messaging Bridge", "a connection between messaging systems, to replicate messages between systems."[^1]

## Problem

One enterprise often has more than one messaging system. The Azure page names MSMQ, RabbitMQ, Azure Service Bus and Amazon SQS, and gives the causes: a merger, an acquisition, or new parts of a system that run in the cloud while the old parts stay on premises.[^3] An application on one system cannot read the channels of the other.[^1]

One fix is to change both sides to call each other over HTTP. The Azure page lists the costs: an HTTP client on one side and a handler on the other, a new test and deployment of both systems, endpoints that must be secure and highly available, and retry code for the network faults.[^3]

## Solution

Put a component between the two systems that is a client of both. It reads the messages from a channel on one side and sends them to the matching channel on the other.[^1][^3] The book says there is usually no practical way to connect two complete messaging systems, so the bridge connects pairs of channels.[^1] The bridge is a set of channel adapters. For each adapter, the application on the other side is in fact the other messaging system.[^1][^4]

```text
  system A                                  system B
  sender ---> [ orders ]   ---> +--------+ ---> [ sb-orders ]  ---> receiver
                                | bridge |
  receiver <- [ replies ]  <--- +--------+ <--- [ sb-replies ] <--- sender
```

The participants are the channels on each side, the bridge with its map from the channels of one side to the channels of the other, and the applications, which do not know that the bridge exists.[^3] The book says the bridge also changes the message format of one system to the format of the other.[^1] The Azure page says the bridge does not change the payload.[^3] In the examples, the body goes through as it is, and only the name of a header changes.

**Topology.** The Azure page gives two forms. In the first, each side has one bridging queue, and a sender sends to it the message types for the other side. In the second, each destination queue has a shadow queue on the other side, and a sender does not know that the bridge exists.[^3]

**When the bridge acks.** The bridge reads from one broker and writes to another, and the two steps are not one transaction. A RabbitMQ shovel acks a message on the source in one of three ways: on receipt, after it publishes the message again, or after the destination confirms the publish.[^5] The program below sends one message through a bridge with each of the three modes, and one failure each time.

```python run id=bridge-acks
# A bridge moves message m3 from a source broker to a destination broker.
# When does it ack m3 on the source, and what does one failure do?
MODES = ["on receipt", "on publish", "on confirm"]
FAILURES = ["crash after receive", "crash before the ack", "publish lost on the way"]


def copies_at_destination(mode, failure, dedup):
    source, dest, seen, failed = ["m3"], [], set(), False
    while source:
        msg = source[0]          # received, still on the source until the ack
        acked = False
        if mode == "on receipt":
            source.pop(0)
            acked = True
        if failure == "crash after receive" and not failed:
            failed = True
            continue             # the bridge restarts; an unacked message comes again
        lost = failure == "publish lost on the way" and not failed
        if lost:
            failed = True
        elif not (dedup and msg in seen):
            dest.append(msg)
            seen.add(msg)
        if mode == "on publish" and not acked:
            if failure == "crash before the ack" and not failed:
                failed = True
                continue
            source.pop(0)
        if mode == "on confirm" and not acked:
            if lost:
                continue         # no confirm from the destination, so no ack; send again
            if failure == "crash before the ack" and not failed:
                failed = True
                continue
            source.pop(0)
    return len(dest)


NAMES = {0: "lost", 1: "ok", 2: "duplicate"}
for dedup in (False, True):
    print("de-dup by id at the bridge:", "yes" if dedup else "no")
    print("  %-24s %-11s %-11s %s" % ("failure", *MODES))
    for f in FAILURES:
        row = [NAMES[copies_at_destination(m, f, dedup)] for m in MODES]
        print("  %-24s %-11s %-11s %s" % (f, *row))
```

```output
de-dup by id at the bridge: no
  failure                  on receipt  on publish  on confirm
  crash after receive      lost        ok          ok
  crash before the ack     ok          duplicate   duplicate
  publish lost on the way  lost        lost        ok
de-dup by id at the bridge: yes
  failure                  on receipt  on publish  on confirm
  crash after receive      lost        ok          ok
  crash before the ack     ok          ok          ok
  publish lost on the way  lost        lost        ok
```

An ack on receipt loses the message if the bridge stops before it publishes. An ack after the publish loses the message when the publish does not arrive. Only an ack after the confirm loses nothing, but then a stop before the ack gives a copy. A store of the ids that the bridge has already sent removes the copies. The Azure page asks for such a de-duplication in the bridge when one of the systems depends on distributed transactions for its correctness.[^3]

## Example

Each example file moves 5 messages from two source channels to the matching channels of a destination with a limit of 64 KiB. The Azure page gives this kind of difference as an example: a message can be up to 4 MB in MSMQ, but only up to 64 KB in Azure Storage queues.[^3] The bridge maps the channel names and the header name, and keeps the body. It acks a message only after the publish, and it stops once before the ack of `m3`. The source delivers `m3` again, and the store of sent ids prevents a copy. A message that is too large goes to an error queue of the bridge.

## Consequences

**Benefits.**

- **No change to the systems.** In the best case, the endpoints do not know that their messages cross a bridge.[^3]
- **Reliable delivery.** The Azure page says the bridge is more reliable than the HTTP alternative, because of the at-least-once delivery of the messaging systems.[^3]
- **A step-by-step move.** Endpoints can move from one messaging system to the other one at a time, as the schedule permits.[^3]
- **WAN links.** A shovel uses acks and publisher confirms to keep the data safe when a connection or a node fails, so it can connect brokers in different places.[^5]

**Liabilities.**

- **The common subset.** A feature of one system may not be available on the bridged route, and the route has the limits of both systems.[^3]
- **Copies.** A bridge that acks after the confirm can send a message twice, as the program shows, so the receiver must be an [[idempotent-receiver]] or the bridge must remove copies.[^3]
- **Poison messages.** The usual retry limit can mark a message as poison when the real cause is a fault in the infrastructure. The bridge may need a different retry policy, or a circuit breaker to stop the forwarding for a time.[^3]
- **One more component to run.** To meet its availability goals, the bridge may need several instances as [[competing-consumers]].[^3]
- **Messages in flight.** When endpoints move one at a time, messages that wait on the old system, also in an error queue for a retry, must still reach the endpoint after it moves. The bridge must stay until those messages are delivered.[^3]

## When not to use it

The Azure page lists these cases.[^3]

- **A feature on one side only.** One of the systems depends on a feature that the other messaging system does not have.
- **A synchronous call.** The system that starts the call needs an immediate response.
- **Special requirements.** The integration has specific requirements, for example for security or privacy.
- **Too much data.** The volume is more than the messaging systems can take, or messaging is too expensive for it.

## Variants and related patterns

**Channel Adapter.** A bridge is a pair of channel adapters. The book asks: "How can you connect an application to the messaging system so that it can send and receive messages?" The answer: "Use a Channel Adapter that can access the application's API or data and publish messages on a channel based on this data, and that likewise can receive messages and invoke functionality inside the application."[^4]

**A queue table.** If a system has no messaging system and cannot change, the Azure page suggests a queue table in SQL Server, filled by change data capture. The bridge reads the table and forwards the messages to the real messaging system.[^3]

**Federation.** RabbitMQ queue federation also moves messages from an upstream queue to a local queue, but only when the upstream queue has no local consumers. A shovel moves the messages in all cases, and always in one direction.[^5]

## Known uses

- RabbitMQ: the Shovel plugin, which consumes from a queue, publishes to a destination in the same cluster or another one, and can move messages between AMQP 0-9-1 and AMQP 1.0 brokers.[^5]
- NServiceBus: the Messaging Bridge, a .NET bridge from queue to queue that supports MSMQ, Service Bus and Azure Queue Storage, among others. The Azure page uses it in its sample that connects MSMQ and Service Bus.[^3]
- Apache Qpid: its bridge can connect messaging systems that use AMQP, as the Azure page notes.[^3]

## References
