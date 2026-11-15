---
tag: G6J38R
kind: pattern
slug: wire-tap
name: Wire Tap
aliases: [Wire Tap pattern, message tap, message tracing, firehose]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A wire tap is a fixed recipient list with two outputs; it sends each message on a channel to its normal receiver and a copy to a second channel, where a tool can inspect, log or store it.
family: integration
polarity: positive
intent: Inspect the messages on a channel, for testing, monitoring or audit, by a step that sends each message on to the main channel and a copy of it to a secondary channel, with no change to the sender or the receiver.
also_known_as: [message tap]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on system management.
see_also: [point-to-point-channel, publish-subscribe-channel, message-channel, content-based-router, idempotent-receiver, structured-logging, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/WireTap.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Wire Tap (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; related patterns
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 11, System Management, Wire Tap
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/wireTap-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Wire Tap EIP
    locator: introduction; Wire Tap; using Wire Tap
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/channel/configuration.adoc
    title: Spring Integration 7.1.1 reference documentation source, Configuring Message Channels
    locator: Wire Tap
  - url: https://www.rabbitmq.com/docs/firehose
    title: RabbitMQ documentation, Firehose Tracing
    locator: Firehose Tracer overview; turning on firehose; firehose event message format
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Detour.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Detour (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageStore.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Message Store (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
---


## Intent

See each message on a channel without a change to the flow. The book asks: "How do you inspect messages that travel on a point-to-point channel?"[^1][^2] The answer: "Insert a simple Recipient List into the channel that publishes each incoming message to the main channel and a secondary channel."[^1]

## Problem

A [[point-to-point-channel]] gives each message to exactly one consumer. This is the reason to use it for a document that only one receiver must process.[^1] But for a test, for monitoring or for a fault search, you often want to see all the messages that go through the channel.[^1]

A second consumer on the same channel does not help, because it takes messages away from the real receiver. A change to the sender or the receiver to log each message puts monitoring code into business code, and you must change it again to turn the logging off.

## Solution

Put a small step into the channel. The step takes each message from the input channel and sends the message, with no change, to two output channels: the main channel, from which the receiver now reads, and a secondary channel for the tool that inspects it.[^1] It is a recipient list that never changes and always has the same two outputs.[^1]

```text
                           +--> [ main ] --> receiver
  sender --> [ in ] --> wire tap
                           +--> [ tap ] --> monitor, log or message store
```

The participants are the input channel, the wire tap, the main channel with the real receiver, and the secondary channel with its consumer. Because the logic that inspects the messages is in a separate component, you can put a generic wire tap into any channel, and the main flow does not change. The book names two results: more reuse, and less risk when you add instruments to a system that already runs.[^1]

**On the same thread or not.** The tools do this in different ways. Camel sends the tapped copy as a fire-and-forget message in a separate thread, so it runs in parallel with the original.[^3] Spring Integration does not make the tap asynchronous by default; the type of the output channel of the tap decides. A direct channel runs the handler on the thread of the sender, and a pollable channel or an executor channel moves it to another thread.[^4] The Spring documentation says that an asynchronous output channel for the tap is the usual choice.[^4]

**A copy, but how deep.** Camel copies the `Exchange`, but it does not clone the objects in it, unless you give an `onPrepare` processor that does. So the copies can share objects with the original.[^3]

The program below models both questions. Ten messages come, one at each tick. The main consumer takes 1 tick for each, and the handler on the tap takes 3 ticks to write an audit record.

```python run id=tap-costs
# Ten messages, one each tick; the main consumer takes 1 tick for each.
# The handler on the tap channel writes an audit record and takes 3 ticks for each.
N, WORK, TAP = 10, 1, 3


def sync_tap():
    t = 0
    for _ in range(N):
        t += TAP   # the tap channel calls its handler on the same thread
        t += WORK
    return t


def async_tap(capacity):
    queue, free_at, logged, dropped = [], 0, 0, 0
    for i in range(N):
        now = i * WORK
        while queue and free_at <= now:
            queue.pop(0)
            free_at += TAP
            logged += 1
        if free_at <= now and not queue:
            free_at = now + TAP
            logged += 1
        elif len(queue) < capacity:
            queue.append(now)
        else:
            dropped += 1
    tap_done = free_at + TAP * len(queue)
    return N * WORK, logged + len(queue), dropped, tap_done


print("sync tap: main done at tick %d, %d logged, 0 dropped" % (sync_tap(), N))
for cap in (4, 10):
    main, logged, dropped, done = async_tap(cap)
    print("async tap, buffer %d: main done at tick %d, %d logged, %d dropped, tap done at tick %d"
          % (cap, main, logged, dropped, done))

# A shallow copy shares the objects inside the message.
original = {"order": "o1", "card": {"number": "4111111111111111"}}
for name, copy in [("shallow", dict(original)),
                   ("deep", {"order": original["order"], "card": dict(original["card"])})]:
    copy["card"]["number"] = "*" * 12 + copy["card"]["number"][-4:]  # the tap masks the card
    print("%s copy: the main flow sees card %s" % (name, original["card"]["number"]))
    original["card"]["number"] = "4111111111111111"
```

```output
sync tap: main done at tick 40, 10 logged, 0 dropped
async tap, buffer 4: main done at tick 10, 8 logged, 2 dropped, tap done at tick 24
async tap, buffer 10: main done at tick 10, 10 logged, 0 dropped, tap done at tick 30
shallow copy: the main flow sees card ************1111
deep copy: the main flow sees card 4111111111111111
```

With a synchronous tap, the slow audit handler makes the main flow four times slower. With an asynchronous tap, the main flow ends at tick 10, but the tap needs a buffer. With a buffer of 4 messages, 2 audit records are lost; with a buffer of 10, none are lost, and the tap ends at tick 30. The last two lines show the copy: when the tap masks the card number in a shallow copy, it also changes the message of the main flow.

## Example

Each example file sends 4 orders through a wire tap. The tap puts a deep copy on the audit channel, and the audit consumer masks the card number and adds a header to its copy. After the third order, an operator turns the tap off. The main receiver gets all 4 orders, and none of the changes of the audit consumer.

## Consequences

**Benefits.**

- **No change to the sender or the receiver.** You add a channel and change where the receiver reads from.[^1]
- **One component for many channels.** A generic tap can go into any channel.[^1]
- **Easy to turn on and off.** In Spring Integration, the tap is an interceptor in the definition of a channel, so you can turn it on or off without a change to the flow.[^4]

**Liabilities.**

- **One more channel and one more hop.** The receiver must read from the new main channel.[^1]
- **Load.** The tap makes one more message for each message. RabbitMQ says that its firehose has no effect on performance when it is off, but that performance drops somewhat when it is on.[^5]
- **A slow tap.** On the same thread, the tap slows the main flow. On another thread, it needs a buffer and a rule for a full buffer, as the program shows.
- **Shared objects.** A shallow copy lets the consumer of the tap change the message of the main flow.[^3]
- **All of the data.** The copy has all the data of the message, also personal or payment data, so the consumer of the tap needs the same protection as the receiver. The examples mask the card number for this reason.

## When not to use it

**When the channel is publish-subscribe.** A [[publish-subscribe-channel]] already gives a copy of each message to each subscriber, so a monitor can be one more subscriber. The wire tap is for a point-to-point channel.[^1]

**When you want to change the route.** A wire tap only inspects; it does not change where the message goes. The Detour pattern is for this case. The book asks: "How can you route a message through intermediate steps to perform validation, testing or debugging functions?" The answer: "Construct a Detour with a context-based router controlled via the Control Bus".[^6] In one state, the router sends the messages through the extra steps, and in the other state, it sends them directly to the destination.[^6]

**When each copy must be kept.** A tap with a full buffer loses records, and a tap on the thread of the sender slows it. When an audit must have each message, a [[guaranteed-delivery]] channel to a Message Store is a better fit. The book asks: "How can we report against message information without disturbing the loosely coupled and transient nature of a messaging system?" The answer: "Use a Message Store to capture information about each message in a central location."[^7]

## Variants and related patterns

**Message Store.** The store gets its data by a duplicate of each message on a special channel, which is the work of a wire tap.[^7]

**Firehose.** A broker can tap all its channels at once. The RabbitMQ firehose sends a copy of each message that is published and each message that is delivered to the topic exchange `amq.rabbitmq.trace`, with the routing key `publish.` or `deliver.` and the name of the exchange or queue.[^5] It is off at the start of the server, and an administrator turns it on for one node and one virtual host.[^5]

**Logging channel adapter.** In Spring Integration, a wire tap with a logging channel adapter as its output writes each message to the log, which is useful for a fault search.[^4]

**Related patterns.** The EIP page lists Control Bus, Detour, Message Store, Point-to-Point Channel, Publish-Subscribe Channel and Recipient List, among others.[^1]

## Known uses

- Apache Camel: the `wireTap` step in a route, as the [Wire Tap page of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/wireTap-eip.adoc) shows.[^3]
- Spring Integration: the `int:wire-tap` interceptor on a channel, as the [channel configuration page of 7.1.1](https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/channel/configuration.adoc) shows.[^4]
- RabbitMQ: the firehose tracer, which an administrator turns on with `rabbitmqctl trace_on`.[^5]

## References
