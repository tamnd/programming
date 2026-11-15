---
tag: T9VZTP
kind: pattern
slug: event-message
name: Event Message
aliases: [Event Message pattern, event, event notification, domain event message]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  An event message is a message that tells other programs that something happened, such as an order that was placed. The sender reports a fact and does not tell the receivers what to do.
family: integration
polarity: positive
intent: Tell the programs that care about a change that the change happened, by a message on a channel, so that the sender does not need to know who reacts or how.
also_known_as: [event, event notification]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003), with Command Message and Document Message as the other two kinds of message intent.
see_also: [publish-subscribe-channel, command-message, request-reply, message-channel, guaranteed-delivery, observer-pattern, message-bus, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/EventMessage.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Event Message (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 5, Message Construction, Event Message
  - url: https://docs.particular.net/nservicebus/messaging/messages-events-commands
    title: Particular Software, NServiceBus documentation, Messages, events, and commands
    locator: Events; Commands vs Events
  - url: https://masstransit.io/documentation/concepts/messages
    title: MassTransit documentation, Messages
    locator: Events
  - url: https://github.com/cloudevents/spec/blob/v1.0.2/cloudevents/spec.md
    title: CloudEvents, Version 1.0.2, specification
    locator: Event; REQUIRED Attributes, id and type; Size Limits
  - url: https://martinfowler.com/articles/201701-event-driven.html
    title: Martin Fowler, What do you mean by Event-Driven?, 2017
    locator: Event Notification; Event-Carried State Transfer; Event-Sourcing
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 3.4.6 JMSReplyTo
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageConstructionIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Message Construction
    locator: Message intent
  - url: https://github.com/cloudevents/sdk-go/blob/v2.16.2/v2/event/event.go#L14-L23
    title: CloudEvents SDK for Go v2.16.2 source, event.go
    locator: type Event, lines 14 to 23
---

## Intent

Tell other programs that something happened, by a message. The book asks: "How can messaging be used to transmit events from one application to another?"[^1][^2] The answer: "Use an Event Message for reliable, asynchronous event notification between applications."[^1] The sender reports a fact. It does not tell the receivers what to do with it.

## Problem

A shop takes orders. For each order, billing must charge the card, the warehouse must pack the items, and an email service must send a confirmation. If the shop calls each of these programs, the shop must know all of them, and it must change each time that one more program wants to know about orders. When one of them is down, the shop must wait or try again.

The forces are these. The shop knows that an order was placed, but the other programs decide what to do with that fact. A program that is added next year must get the same facts with no change to the shop. And the notice must come soon, because it is about a change that happens now.

## Solution

The sender makes a message for each change and puts it on a channel, usually a [[publish-subscribe-channel]], so that each program that cares gets its own copy. The book compares this with the [[observer-pattern]]: the subject wraps the event in a message, and messaging only makes sure that the notice gets to each observer.[^1]

```text
  shop --- OrderPlaced {order: 41} ---> [ topic: orders ] ---> billing
           a fact, in the past tense                      \--> warehouse
                                                           \-> email
```

The participants are the producer, the event message, a publish-subscribe channel and the subscribers. The producer does not know the subscribers, and each subscriber decides alone how to react.

**A name in the past tense.** NServiceBus and MassTransit give the same rule: "Events should be expressed in a noun-verb (past tense) sequence, indicating that something happened."[^3][^4] `OrderPlaced` and `CustomerAddressUpdated` are events. `ChargeCard` is a [[command-message]], because it asks for an action.

**Attributes that identify the event.** CloudEvents is a specification for the attributes of an event. It requires four: `id`, `source`, `specversion` and `type`. The producer must make `source` and `id` together unique for each distinct event, and "Consumers MAY assume that Events with identical `source` and `id` are duplicates."[^5] The `type` names the kind of event, and it "SHOULD be prefixed with a reverse-DNS name", as in `com.example.object.deleted.v2`.[^5]

**What the event carries.** Fowler gives two forms. In event notification, "An event need not carry much data on it, often just some id information and a link back to the sender that can be queried for more information."[^6] In event-carried state transfer, the event contains the data that changed, so that each receiver keeps its own copy and does not need to call the source.[^6] The program below sends the same two changes of a customer to three subscribers in the two forms.

```python run id=thin-and-fat
import json

# The customer service owns the data, and it publishes an event for each change.
customers = {"c7": {"name": "Ana", "city": "Hanoi"}}
calls_to_source = 0
bytes_sent = 0


def get_customer(cid):
    """The query API of the customer service."""
    global calls_to_source
    calls_to_source += 1
    return dict(customers[cid])


def change_city(cid, city, subscribers, style):
    global bytes_sent
    customers[cid]["city"] = city
    event = {"type": "CustomerAddressUpdated", "id": cid}
    if style == "fat":
        event["data"] = dict(customers[cid])  # the event carries the new state
    for on_event in subscribers:
        bytes_sent += len(json.dumps(event))
        on_event(event)


def make_subscriber(copy):
    def on_event(event):
        # A thin event only says what changed, so the subscriber asks the source.
        data = event.get("data") or get_customer(event["id"])
        copy[event["id"]] = data
    return on_event


for style in ("thin", "fat"):
    calls_to_source, bytes_sent = 0, 0
    copies = [{}, {}, {}]  # billing, shipping and crm keep their own copy
    subscribers = [make_subscriber(c) for c in copies]
    for city in ("Hue", "Da Nang"):
        change_city("c7", city, subscribers, style)
    print(f"{style}: {bytes_sent} bytes of events, {calls_to_source} calls back to the source,"
          f" each copy says {copies[0]['c7']['city']}")
```

```output
thin: 276 bytes of events, 6 calls back to the source, each copy says Da Nang
fat: 528 bytes of events, 0 calls back to the source, each copy says Da Nang
```

The thin events are smaller, but each subscriber calls the source for each event: 2 changes and 3 subscribers give 6 calls. The fat events are almost two times as large, and the source gets no calls. Large data has a limit: CloudEvents says that "Intermediaries MUST forward events of a size of 64 KByte or less", and that publishers should keep events compact and link to large data items.[^5]

**Timing.** The book says that the time of an event is often more important than its content. Many events have no data, and the receiver should act on an event soon. For this reason it says that [[guaranteed-delivery]] often helps little with events, and that a message expiration can make sure that an event is processed soon or not at all.[^1]

**No return address.** An event does not ask for an answer. Jakarta Messaging says: "Messages sent with a null JMSReplyTo value may be a notification of some event or they may just be some data the sender thinks is of interest."[^7]

## Example

Each example file has a topic in memory with two subscribers, billing and email. The shop publishes four event messages with the CloudEvents attributes. The third message is the second one sent again after a network error, with the same `id`, and both subscribers skip it. Email has no handler for the cancelled event and ignores it: a subscriber reacts only to the events that it cares about, and the producer does not need to know which.

## Consequences

**Benefits.**

- **Loose coupling.** The producer does not know the subscribers. A new subscriber starts to read the topic, and the producer does not change.
- **Each subscriber at its own speed.** Each subscriber has its own copy, so a slow subscriber does not stop the others.
- **One fact, many uses.** Billing, shipping and reports read the same event, and each keeps the data that it needs.

**Liabilities.**

- **The flow is hard to see.** Fowler warns about a flow that runs over many events: "The problem is that it can be hard to see such a flow as it's not explicit in any program text."[^6]
- **Commands in disguise.** "A simple example of this trap is when an event is used as a passive-aggressive command."[^6] If the producer needs one program to act, it should send a command.
- **Duplicates.** With at-least-once delivery, the same event can arrive two times. The subscriber must find the copy by its id, as in the examples.
- **A contract with unknown readers.** The producer does not know who reads its events, so a change to the data can break a subscriber that the producer has not heard of. A version in the `type`, as in the CloudEvents example `com.example.object.deleted.v2`,[^5] lets the old and new forms live side by side.
- **Thin or fat, both cost.** Thin events put load on the source. Fat events make large messages and many copies of the data.

## When not to use it

**When one program must do a specific action.** Send a [[command-message]] to the owner of the action, on a point-to-point channel.

**When the sender needs an answer.** Use [[request-reply]].

**Inside one program.** A list of callbacks, as in the [[observer-pattern]], is simpler than a channel when all the observers are in the same process and nothing must survive a crash.

## Variants and related patterns

**Command Message and Document Message.** The book gives three kinds of message intent: a command asks the receiver to run a function, a document carries data, and an event tells the receiver about a change in the sender and does not say how to react.[^8]

**Event notification and event-carried state transfer.** The thin and fat forms above.[^6]

**Event sourcing.** Fowler: "The core idea of event sourcing is that whenever we make a change to the state of a system, we record that state change as an event".[^6] The events are then the record of the state, not only notices.

**Message Bus.** Events are one part of the common set of messages of a [[message-bus]].

## Known uses

- CloudEvents: the [Event type in sdk-go at v2.16.2](https://github.com/cloudevents/sdk-go/blob/v2.16.2/v2/event/event.go#L14-L23) holds the context attributes and the data; its comment calls it "the canonical representation of a CloudEvent."[^9]
- NServiceBus: an event is "Used to communicate that an action has been performed". It "Should be published by the logical owner" and "Cannot be sent".[^3]
- MassTransit: events are published with `Publish`, and they have names in the past tense.[^4]

## References
