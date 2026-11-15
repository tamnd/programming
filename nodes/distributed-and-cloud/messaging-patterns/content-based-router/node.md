---
tag: KC8BRN
kind: pattern
slug: content-based-router
name: Content-Based Router
aliases: [Content-Based Router pattern, content-based routing, CBR, choice router]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A content-based router reads each message on one channel and sends it to another channel that it chooses from the content, such as the type of an item. The sender then needs to know only one channel.
family: integration
polarity: positive
intent: Send each message to the right receiver for its content, by a router that reads one input channel, tests rules on each message, and puts the message on the channel of the first rule that matches.
also_known_as: [content-based routing, CBR]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) as the first of the simple routers; Apache Camel calls it the Choice EIP.
see_also: [message-channel, point-to-point-channel, publish-subscribe-channel, dead-letter-channel, event-message, message-bus, chain-of-responsibility-pattern, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/ContentBasedRouter.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Content-Based Router (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; Example, Apache Camel
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 7, Message Routing, Content-Based Router
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageRoutingIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Message Routing
    locator: Simple Routers
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/choice-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Choice EIP
    locator: introduction; Example
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/ChoiceProcessor.java#L60-L100
    title: Apache Camel 4.14.0 source, ChoiceProcessor.java
    locator: method process, lines 60 to 100
  - url: https://docs.aws.amazon.com/sns/latest/dg/sns-message-filtering.html
    title: Amazon SNS Developer Guide, Amazon SNS message filtering
    locator: introduction
---

## Intent

Send each message to the receiver that can handle its content. The book asks: "How do we handle a situation where the implementation of a single logical function (e.g., inventory check) is spread across multiple physical systems?"[^1][^2] The answer: "Use a Content-Based Router to route each message to the correct recipient based on message content."[^1]

## Problem

A shop sells widgets and gadgets. One warehouse program keeps the stock of widgets, and another one keeps the stock of gadgets. For the shop, an inventory check is one function, but the work is in two systems. If the shop chooses the system for each order, it must know all the systems and change when a third one comes.

The forces are these. The sender should send to one channel and not know the systems behind it. Each message must go to exactly one system, the one that owns its item. And a message that no system can handle must not be lost.

## Solution

Put a router between the sender and the systems. The router reads each message on its input channel, looks at the data in the message, and puts the message on the output channel of the right system.[^1] The book says that the router lets the producer send all its messages to one channel, and so the producer is not coupled to the channels of the destinations.[^3]

```text
                                              +--> [ inventory.widgets ] ---> widget system
  shop ---> [ orders ] ---> content-based ---+--> [ inventory.gadgets ] ---> gadget system
                              router          +--> [ invalid ]           ---> a person looks
```

The participants are the input channel, the router with its rules, the output channels and the receivers. A rule can test that a field exists or that it has a given value.[^1]

**One rule for each channel, in order.** Apache Camel calls the pattern the Choice EIP, which "allows you to route messages to the correct destination based on the contents of the message exchanges."[^4] A route lists `when` clauses with a predicate and a destination, and an `otherwise` clause.[^4] The class `ChoiceProcessor` tests the predicates in order and sends the message to the first one that matches.[^5] So the order of the rules is a part of the routing, as the program below shows. It routes the same four orders with two orders of the rules.

```python run id=rule-order
orders = [
    {"id": 1, "shipping": "express", "total": 5000},
    {"id": 2, "shipping": "normal", "total": 5000},
    {"id": 3, "shipping": "normal", "total": 20},
    {"id": 4, "total": 20},  # a message with no shipping field
]

express = ("orders.express", lambda o: o["shipping"] == "express")
large = ("orders.large", lambda o: o["total"] >= 1000)
normal = ("orders.normal", lambda o: True)


def route(order, rules):
    """Return the channel of the first rule that matches."""
    try:
        for channel, test in rules:
            if test(order):
                return channel
    except KeyError as missing:
        return f"orders.invalid (no field {missing})"
    return "orders.invalid (no rule matched)"


for name, rules in [("express first", [express, large, normal]), ("large first", [large, express, normal])]:
    print(name + ":")
    for order in orders:
        print(f"  order {order['id']} -> {route(order, rules)}")
```

```output
express first:
  order 1 -> orders.express
  order 2 -> orders.large
  order 3 -> orders.normal
  order 4 -> orders.invalid (no field 'shipping')
large first:
  order 1 -> orders.large
  order 2 -> orders.large
  order 3 -> orders.normal
  order 4 -> orders.invalid (no field 'shipping')
```

Order 1 is an express order and also a large order. With the express rule first, it goes to `orders.express`. With the large rule first, it goes to `orders.large`. Order 4 has no `shipping` field, and the router sends it to an invalid channel, not to a normal channel by accident.

**A default.** A message that matches no rule needs a place. In Camel, when no `when` matches and there is no `otherwise`, the comment in the code says: "when no filter matches and there is no otherwise, then just continue".[^5] The message goes on with no routing, so an unknown type can pass with no error. A default rule to an invalid channel or a [[dead-letter-channel]] makes the case visible.

**Maintenance.** The book warns that the router can become a place of frequent change, because it must know all the destinations. In larger systems, the rules can be in a configurable rules engine.[^1]

## Example

Each example file routes four orders by the `type` field. Widgets go to `inventory.widgets`, gadgets go to `inventory.gadgets`, and the order for a `gizmo` goes to `invalid`, because no system owns it. Python and Go use a map from the type to the channel, Java uses a list of rules with a predicate and the first match wins, and C# uses a `switch` expression with a default arm.

## Consequences

**Benefits.**

- **One channel for the sender.** The sender does not know how many systems there are, or which one owns which items.[^3]
- **Each message goes once.** The router chooses one channel, so no system gets a message that it must ignore.
- **Rules in one place.** A change of the owner of an item type is a change of one rule.

**Liabilities.**

- **The router knows all destinations.** A new system means a change to the router.[^3]
- **The router reads the content.** It must parse the message, so it depends on the format of the data. A change of a field name can send messages to the wrong place.
- **Order of the rules.** When two rules can match one message, the first one wins, and a change of the order changes the routing.
- **One more step.** Each message goes through the router, which adds a hop and a part that can fail.

## When not to use it

**When each receiver can decide alone.** The book gives an alternative: a [[publish-subscribe-channel]] with one message filter for each receiver. The router routes predictively and depends on all the destinations; the filters filter reactively, and no part knows all the destinations.[^3] Amazon SNS uses this form: by default, "an Amazon SNS topic subscriber receives every message that's published to the topic", and a filter policy on a subscription selects a subset.[^6]

**When the sender already knows the receiver.** If the sender has the information and the destinations do not change, it can send to the right channel itself.

**When a message must go to many receivers.** A content-based router chooses one channel. The book uses a recipient list for more than one.[^3]

## Variants and related patterns

**Message Filter.** A special form of the router with one output: a message that does not match is discarded.[^3]

**Dynamic Router.** A router whose rules change at run time, by control messages.[^3]

**Recipient List.** A router that can send one message to more than one channel.[^3]

**Chain of Responsibility.** In object-oriented design, the [[chain-of-responsibility-pattern]] gives a request to a list of handlers in order; the rules of a router also run in order, but the router, not the handler, chooses the destination.

## Known uses

- Apache Camel: the [process method of ChoiceProcessor at 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/ChoiceProcessor.java#L60-L100) finds "the first matching filter", runs its processor, and else runs `otherwise`.[^5] The EIP site shows the same router in the Camel Java DSL, with `choice`, `when` and `otherwise`.[^1]
- Amazon SNS: filter policies on the subscriptions act on the message attributes or on the body.[^6]

## References
