---
tag: 8TWJAY
kind: pattern
slug: publish-subscribe-channel
name: Publish-Subscribe Channel
aliases: [Publish-Subscribe Channel pattern, pub/sub channel, pub-sub, topic, fan-out channel]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A publish-subscribe channel gives a copy of each message to each of its subscribers. The publisher sends an event once, and each program that subscribed to the channel gets it.
family: integration
polarity: positive
intent: Send the event on a channel that gives a copy of each message to each subscriber, so that all the interested programs get it and the publisher does not know them.
also_known_as: [topic, fanout exchange, event bus]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003). JMS calls the same model publish/subscribe messaging and its channels topics.
see_also: [message-channel, point-to-point-channel, enterprise-integration-patterns, observer-pattern, event-aggregator, event-driven-programming, mediator-pattern, message-passing-concept]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/PublishSubscribeChannel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Publish-Subscribe Channel (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; Example, Google Cloud Pub/Sub
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 4.2.1 Overview; 4.2.2 Topic semantics; 8.3 Subscriptions
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/service-bus-queues-topics-subscriptions
    title: Azure Service Bus queues, topics, and subscriptions
    locator: Choose between queues and topics; Topics and subscriptions
  - url: https://www.rabbitmq.com/tutorials/tutorial-three-python
    title: RabbitMQ tutorial 3, Publish/Subscribe (Python)
    locator: Exchanges; Temporary queues; Bindings; Putting it all together
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 4, Messaging Channels, Publish-Subscribe Channel; Chapter 10, Messaging Endpoints, Durable Subscriber
  - url: https://cloud.google.com/pubsub/docs/subscription-overview
    title: Google Cloud Pub/Sub documentation, Subscription overview
    locator: Default subscription properties; topic retention
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/introduction.html
    title: Apache Kafka 4.1.0 documentation source, introduction.html
    locator: Main Concepts and Terminology
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/design.html
    title: Apache Kafka 4.1.0 documentation source, design.html
    locator: The Consumer, Consumer Position
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingChannelsIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Messaging Channels
    locator: Message Channel Decisions, One-to-one or one-to-many
  - url: https://github.com/spring-projects/spring-integration/blob/v6.5.1/spring-integration-core/src/main/java/org/springframework/integration/channel/PublishSubscribeChannel.java#L29-L38
    title: Spring Integration 6.5.1 source, PublishSubscribeChannel.java
    locator: class PublishSubscribeChannel, lines 29 to 38
  - url: https://docs.spring.io/spring-integration/reference/channel/implementations.html
    title: Spring Integration reference, Message Channel Implementations
    locator: PublishSubscribeChannel
---

## Intent

Send the event on a Publish-Subscribe Channel, "which delivers a copy of a particular event to each receiver".[^1] The publisher sends once. Each program that subscribed gets its own copy.

## Problem

The shop places an order. Billing must charge the card, the email program must send a receipt, and the stock program must reserve the items. Next year, an analytics program will want the same event. The question is: "How can the sender broadcast an event to all interested receivers?"[^1]

The forces are these. The shop must not know the list of programs that react to an order, because the list changes. Each program must get every order, not a share of them. And a slow or stopped program must not stop the others.

A [[point-to-point-channel]] gives each message to one receiver, so billing would get some orders and email the others. A separate queue for each program, filled by the shop, works, but the shop then knows each program and must change for each new one.

## Solution

The channel has one input and one output for each subscriber. A message that a publisher sends to the input is copied to each output. Each output has one subscriber, which consumes its copy once.[^1]

```text
                                 +--> [ copy ] --> billing
  shop --publish--> ( orders ) --+--> [ copy ] --> email
                      topic      +--> [ copy ] --> stock
```

The participants are the publisher, the channel, the subscriptions and the subscribers. In Jakarta Messaging, the channel is a topic, and a subscription "will receive a copy of every message that is sent to the topic after the subscription is created".[^2] Each copy is a separate message: "acknowledging one does not acknowledge any other".[^2] Azure Service Bus describes a topic subscription as a virtual queue that receives copies of the messages that are sent to the topic.[^3] In RabbitMQ, the publisher sends to a fanout exchange, which "just broadcasts all the messages it receives to all the queues it knows", and each subscriber reads its own queue.[^4]

**After the subscription, not before.** A subscriber gets only the messages that are published while its subscription exists. A non-durable subscription exists only while it has an active consumer.[^2] A durable subscription "is persisted and continues to accumulate messages until explicitly deleted, even if there are no consumer objects consuming messages from it".[^2] The book calls a subscriber with a durable subscription a Durable Subscriber.[^5] The following program shows the difference. `billing` and `audit` are durable, and `email` is not. `audit` and `email` disconnect while the shop publishes order 2.

```python run id=durable-subscriptions
from collections import deque


class Topic:
    """Each subscription gets a copy of each message that is published after it is made."""

    def __init__(self):
        self.subscriptions = {}  # name -> (durable, messages)

    def subscribe(self, name, durable=False):
        self.subscriptions.setdefault(name, (durable, deque()))

    def disconnect(self, name):
        durable, _ = self.subscriptions[name]
        if not durable:
            del self.subscriptions[name]  # the copies that wait for it are gone too

    def publish(self, message):
        for _, messages in self.subscriptions.values():
            messages.append(message)
        return len(self.subscriptions)

    def receive_all(self, name):
        _, messages = self.subscriptions[name]
        out = list(messages)
        messages.clear()
        return out


topic = Topic()
topic.subscribe("billing", durable=True)
topic.subscribe("audit", durable=True)
topic.subscribe("email")
print("order 1: copies", topic.publish("order 1"))

topic.disconnect("audit")
topic.disconnect("email")
print("order 2: copies", topic.publish("order 2"))

topic.subscribe("audit", durable=True)  # the same durable subscription again
topic.subscribe("email")                # a new non-durable subscription
topic.subscribe("analytics")            # a new subscriber
print("order 3: copies", topic.publish("order 3"))

for name in ["billing", "audit", "email", "analytics"]:
    print(f"{name:9}  {topic.receive_all(name)}")
```

```output
order 1: copies 3
order 2: copies 2
order 3: copies 4
billing    ['order 1', 'order 2', 'order 3']
audit      ['order 1', 'order 2', 'order 3']
email      ['order 3']
analytics  ['order 3']
```

`audit` gets all three orders, because its durable subscription kept order 2 while it was away. `email` loses order 1, which waited for it when it disconnected, and order 2, which was published while it had no subscription. `analytics` starts with order 3, because a subscription does not see the past. With no subscription at all, a message is lost: the RabbitMQ tutorial says that "The messages will be lost if no queue is bound to the exchange yet".[^4]

## Example

Each example file makes a topic with three subscribers, `billing`, `email` and `stock`. Each subscriber has its own queue or channel and its own thread or task. The shop publishes three events, and the topic puts a copy of each event in each queue. Each subscriber gets all three events in the order of publication, and the program counts nine copies.

The examples build the topic from one standard queue for each subscriber, because that is the structure of a broker topic: a fanout exchange with a queue for each subscriber in RabbitMQ, or a topic with subscriptions in Service Bus.[^4][^3] The publisher puts the copies in the queues and does not wait for the subscribers.

## Consequences

**Benefits.**

- **The publisher does not know the subscribers.** A new subscriber subscribes, and the publisher does not change. With the topic as an intermediary, Jakarta Messaging says, "message publishers are kept independent of subscribers and vice versa".[^2]
- **Each subscriber works at its own rate.** Each copy waits in its own queue, so a slow subscriber does not slow the others.
- **Eavesdropping for debugging.** The book notes that a publish-subscribe channel lets a program watch the traffic on a channel without a change to the flow of messages.[^1]

**Liabilities.**

- **Copies cost storage.** A message with ten subscriptions is stored and delivered ten times.
- **No subscriber, no message.** A message that is published before a subscription exists is not delivered to it. Some systems keep messages for replay: Google Cloud Pub/Sub has topic retention, which lets a subscription "seek back in time and replay previously published messages".[^6] In Kafka, "events are not deleted after consumption", and a topic keeps them for the time that its configuration sets.[^7] A consumer "can deliberately rewind back to an old offset and re-consume data".[^8]
- **No reply and no single owner.** The publisher does not know who did the work, or if anybody did. An event that must be handled needs a subscriber that someone watches.
- **At-least-once and no order.** By default, "Pub/Sub offers at-least-once delivery with no ordering guarantees on all subscription types".[^6] Each subscriber must accept duplicates and out-of-order events, or the system must give an order for each key.

## When not to use it

**For a command that must run once.** A charge or a shipment is a command for one receiver. On a topic, each subscriber would run it. Use a [[point-to-point-channel]].

**When the publisher needs to know that the work was done.** A topic gives no reply. Use a request and a reply, or a process manager that waits for the replies.

**For a large audience with a long history.** A new subscriber that needs all past events gets nothing from a plain topic. Use a log with replay, such as a Kafka topic, or a store of events.

## Variants and related patterns

**Durable Subscriber.** A subscriber whose subscription keeps the messages while it is offline.[^5][^2]

**Competing consumers on a subscription.** A subscription can have many consumers that share its copies. In Jakarta Messaging, a shared subscription gives each message in the subscription to only one consumer.[^2] Kafka does the same with consumer groups: each partition of a topic "is consumed by exactly one consumer within each subscribing consumer group at any given time".[^8] Google Cloud Pub/Sub has topics for publish-subscribe and subscriptions for competing consumers.[^1]

**Message filters on subscriptions.** A subscription can take only some messages. In Jakarta Messaging, a subscription with a message selector gets only the messages whose properties match it.[^2]

**Observer, inside one program.** The [[observer-pattern]] gives the same one-to-many shape with method calls in one process, and an [[event-aggregator]] gives one place to publish events. A publish-subscribe channel adds a broker, queues and copies that survive the publisher.

**Point-to-Point Channel.** The other kind of channel.[^9] See [[point-to-point-channel]].

## Known uses

- Spring Integration: [PublishSubscribeChannel.java at v6.5.1](https://github.com/spring-projects/spring-integration/blob/v6.5.1/spring-integration-core/src/main/java/org/springframework/integration/channel/PublishSubscribeChannel.java#L29-L38) is "A channel that sends Messages to each of its subscribers".[^10] The reference says that it broadcasts any message sent to it to all of its subscribed handlers.[^11]
- RabbitMQ fanout exchanges, with a temporary queue for each subscriber.[^4]
- Azure Service Bus topics and subscriptions.[^3]
- Kafka topics, which are "always multi-producer and multi-subscriber".[^7]

## References
