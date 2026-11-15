---
tag: 83HZ5D
kind: pattern
slug: claim-check
name: Claim Check
aliases: [Claim Check pattern, reference-based messaging, large message offload, store in library]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A claim check stores a large or sensitive payload in an external store and sends only a key to it through the messaging system; a later component uses the key to get the payload back.
family: integration
polarity: positive
intent: Keep messages small, by a step that puts the payload in a persistent store and sends a reference to it in its place, so that the components that need the data can get it back by the reference.
also_known_as: [reference-based messaging]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on message transformation; the page on the EIP site has the file name StoreInLibrary. The Azure Architecture Center describes it as one of its cloud design patterns.
see_also: [message-translator, splitter, aggregator, idempotent-receiver, guaranteed-delivery, message-channel, compute-a-sha-256-hash, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/StoreInLibrary.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Claim Check (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 8, Message Transformation, Claim Check
  - url: https://learn.microsoft.com/en-us/azure/architecture/patterns/claim-check
    title: Microsoft, Azure Architecture Center, Claim Check pattern
    locator: context and problem; solution; problems and considerations; when to use this pattern; alternatives; examples
  - url: https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/quotas-messages.html
    title: Amazon Simple Queue Service Developer Guide, Amazon SQS message quotas
    locator: message size; message retention
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/claimCheck-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Claim Check EIP
    locator: introduction; Claim Check operation; merging data using get or pop operation
---

## Intent

Send a small reference through the messaging system in place of a large payload, and get the payload back where it is needed. The book asks: "How can we reduce the data volume of message sent across the system without sacrificing information content?"[^1][^2] The answer: "Store message data in a persistent store and pass a Claim Check to subsequent components. These components can use the Claim Check to retrieve the stored information."[^1]

## Problem

Messaging systems are made for many small messages, and they often limit the size of a message.[^3] An Amazon SQS message can have at most 1 MiB.[^4] A scanned document, a video or a large report does not fit, or it fits but slows the broker.

The Azure page names four problems of large messages.[^3] The broker rejects a message over its limit. Large payloads use memory and disk in the broker that other messages need. Each step takes longer to serialize and transfer them. Some services charge by the size of the message. The book adds that the steps in the middle of a flow often do not need all the data, and that a large message is also harder to debug.[^1]

The forces are these. The data must not be lost. The message must stay small. And the component at the end must still get all of the data.

## Solution

Before the message goes on the channel, put the payload in a persistent store, such as a file, a database or a blob store. Put the key of the stored data in the message in place of the data. A later component uses the key to get the data back.[^1] The book compares this to a luggage check at an airport: you get a ticket with a number, and you use it to get your bag at the end of the trip.[^1]

```text
            (1) put payload            (5) get payload by key
  sender --------------------> [ store ] <------------------- receiver
     |                                                            ^
     +--(2) [ key, type, size ] --> [ channel ] --> router --(4)--+
```

The participants are the sender, or a check luggage step, the store, the channel with the small message, and the receiver, or a content enricher step that gets the data back.[^1] The steps in the middle see only the key.

**The order of the writes.** The write to the store and the publish of the message are in two systems and are not one transaction. The Azure page says to publish the claim check only after the write of the payload succeeds; a failure between the two steps can leave a payload that no message points to.[^3]

**For all messages, or only for large ones.** A sender can make a claim check for each message, or only for a message over a size. If the choice is made for each message, the format of the message must show whether the payload is inline or in the store.[^3] The program below sends three payloads through a broker with a limit of 256 KiB and three steps in the route, and then shows a problem of the lifecycle of the payload.

```python run id=claim-sizes
import json

LIMIT = 256 * 1024  # the largest message that the broker in this program takes
HOPS = 3            # router, translator, consumer: each hop reads and writes the message
payloads = {"invoice": 2 * 1024, "scan": 300 * 1024, "video": 5 * 1024 * 1024}


def message(kind, size, threshold):
    if size <= threshold:
        return size  # the payload goes inline
    check = {"type": kind, "claim": f"store/{kind}", "size": size}
    return len(json.dumps(check))


for name, threshold in [("inline", float("inf")), ("claim check for all", 0),
                        ("claim check over 64 KiB", 64 * 1024)]:
    sizes = {k: message(k, s, threshold) for k, s in payloads.items()}
    rejected = [k for k, s in sizes.items() if s > LIMIT]
    moved = sum(s for k, s in sizes.items() if k not in rejected) * HOPS
    print(f"{name}: rejected {', '.join(rejected) or 'none'}, {moved} bytes through the broker")

# Who deletes the payload? Here the consumer deletes it when it reads it,
# then crashes before the ack, and the broker delivers the claim check again.
store = {"store/scan": b"..."}
for delivery in (1, 2):
    data = store.pop("store/scan", None)
    print(f"delivery {delivery}:", "payload read" if data else "claim store/scan: payload not found")
```

```output
inline: rejected scan, video, 6144 bytes through the broker
claim check for all: rejected none, 516 bytes through the broker
claim check over 64 KiB: rejected none, 6483 bytes through the broker
delivery 1: payload read
delivery 2: claim store/scan: payload not found
```

Inline, the broker rejects two of the three messages. With a claim check for each message, the broker moves 516 bytes in place of more than 5 MiB. With a claim check only over 64 KiB, the small invoice stays inline, and no step must go to the store for it. The last two lines show a payload that the consumer deleted when it read it: when the broker delivers the message again, the claim check points to nothing.

## Example

Each example file sends a short invoice inline and a report of 276,000 bytes as a claim check. The key is the SHA-256 hash of the payload. Thus the receiver can check that the data it gets back is the data that was sent. The Azure page suggests a content hash for this.[^3] Then the store loses the payload, as a lifecycle rule can do, and the same message gives a claim check that cannot be resolved, which goes to the dead letter channel.

## Consequences

**Benefits.**

- **Small messages.** The broker moves only the key, so a message stays under the limit and the broker stays fast.[^3]
- **Sensitive data off the path.** The data stays in a store with its own access control, hidden from the steps in the middle and from tools that read the queue.[^3] Camel names this use: to hide the sensitive parts of a message from a party that you cannot trust.[^5]
- **Less work in the middle.** Routers and translators read only a small message.[^3]

**Liabilities.**

- **One more system.** The store must be available as long as a receiver can need the payload; if it is down, the processing stops.[^3]
- **Who deletes the payload.** A component must own the delete. A payload deleted too early breaks a valid claim check, as in the program; a payload never deleted fills the store.[^3] The store must keep a payload at least as long as the broker can keep the message; SQS keeps a message for 4 days by default and up to 14 days.[^4]
- **More latency.** The receiver makes an extra call to the store before it can start.[^3]
- **Copies.** A claim check can be delivered more than once, so the receiver must be an [[idempotent-receiver]].[^3]

## When not to use it

**When the messages fit.** If the payload is under the limit of the transport and does not slow it, inline delivery is simpler and faster. Azure Service Bus Premium, for example, takes AMQP messages of up to 100 MB.[^3]

**When latency comes first.** The calls to the store add network trips between the producer and the consumer.[^3]

**When the payload can be made smaller or split.** Compression or a more compact format can be enough. A [[splitter]] and an [[aggregator]] can send a large payload as a sequence of small messages, at the cost of order, correlation and reassembly.[^3]

## Variants and related patterns

**Content Filter and Content Enricher.** The book places the claim check between the two: it removes data from the message for a time, and a content enricher puts it back later.[^1] The book puts it in the chapter on message transformation, with the [[message-translator]] and its other variants.[^2]

**A stack of claim checks.** Camel has the operations `Set`, `Get`, `GetAndRemove`, `Push` and `Pop`. `Push` and `Pop` use a stack and no key, and the data that comes back is merged into the current message by a filter such as `body` or `header:foo*`.[^5]

**An event from the store.** In three of the Azure samples, the sender only uploads the payload to Blob Storage, and the `BlobCreated` event from Event Grid, with the URL of the blob, is the claim check.[^3]

## Known uses

- Apache Camel: the Claim Check EIP, which keeps the data in an internal memory store, as the [Claim Check page of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/claimCheck-eip.adoc) notes.[^5]
- Amazon SQS: the Extended Client Libraries for Java and Python put a payload in Amazon S3 and send a reference to it; the payload can have up to 2 GB.[^4]
- Azure: samples with Blob Storage and Queue Storage, Event Hubs or Service Bus; NServiceBus has a DataBus feature for the same task.[^3]

## References
