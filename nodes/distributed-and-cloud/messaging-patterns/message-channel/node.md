---
tag: DABD4B
kind: pattern
slug: message-channel
name: Message Channel
aliases: [Message Channel pattern, messaging channel, named channel, message destination]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A message channel is a named path in a messaging system. One program writes messages to it and other programs read them, so the sender knows the name of the channel, not the receiver.
family: integration
polarity: positive
intent: Connect two programs through a named channel in a messaging system, so that one program writes messages to the channel and the other reads them from it.
also_known_as: [queue, topic, destination, exchange]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003). In the chapter on messaging systems, it is the first of the six root patterns.
see_also: [enterprise-integration-patterns, point-to-point-channel, publish-subscribe-channel, message-passing-concept, channels, producer-consumer, worker-queue, actor-model]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageChannel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Message Channel (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Messaging.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Messaging
    locator: problem and solution statements
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 4.1.2 Queue semantics; 4.1.8 Reliability; 4.2.2 Topic semantics
  - url: https://learn.microsoft.com/en-us/azure/service-bus-messaging/service-bus-queues-topics-subscriptions
    title: Azure Service Bus queues, topics, and subscriptions
    locator: Choose between queues and topics; Queues
  - url: https://www.rabbitmq.com/tutorials/tutorial-three-python
    title: RabbitMQ tutorial 3, Publish/Subscribe (Python)
    locator: Exchanges; Temporary queues; Putting it all together
  - url: https://github.com/apache/kafka/blob/4.1.0/docs/introduction.html
    title: Apache Kafka 4.1.0 documentation source, introduction.html
    locator: Main Concepts and Terminology
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingChannelsIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Messaging Channels
    locator: Message Channel Themes; Message Channel Decisions
  - url: https://go.dev/ref/spec#Channel_types
    title: The Go Programming Language Specification, Channel types
    locator: Channel types
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/channels
    title: .NET documentation, System.Threading.Channels library
    locator: Producer/consumer conceptual programming model; Unbounded creation strategies
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/concurrent/BlockingQueue.html
    title: Java SE 21 API, Interface BlockingQueue
    locator: interface description
  - url: https://docs.python.org/3.14/library/queue.html
    title: The Python Standard Library, queue, Python 3.14
    locator: module introduction
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/toc.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Table of Contents
    locator: Messaging Systems
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 3, Messaging Systems, Message Channel; Chapter 4, Messaging Channels
  - url: https://github.com/spring-projects/spring-framework/blob/v6.2.10/spring-messaging/src/main/java/org/springframework/messaging/MessageChannel.java#L19-L58
    title: Spring Framework 6.2.10 source, MessageChannel.java
    locator: interface MessageChannel, lines 19 to 58
  - url: https://docs.spring.io/spring-integration/reference/channel/implementations.html
    title: Spring Integration reference, Message Channel Implementations
    locator: PublishSubscribeChannel; QueueChannel; DirectChannel
---

## Intent

Connect the programs through a named channel, "where one application writes information to the channel and the other one reads that information from the channel".[^1] The sender knows the name of the channel. It does not know which program reads it, or when.

## Problem

A shop program takes orders, and a warehouse program packs them. The two programs run on different computers, are written by different teams, and stop and start at different times. The question that the pattern answers is: "How does one application communicate with another using messaging?"[^1]

The forces are these. The shop must not wait for the warehouse: "Sending a message does not require both systems to be up and ready at the same time."[^2] The shop must not need the address of the warehouse, because the warehouse can move, or become three programs. And the warehouse must get only the messages that it can use, not every message in the system.

A direct call, such as an HTTP request from the shop to the warehouse, fails all three. It needs both programs to run at the same time, it needs the address, and it carries no type of message other than the one that the call defines.

## Solution

The messaging system keeps a set of named channels. A sender writes each message to the channel for that kind of message. A receiver reads from the channels for the kinds of messages that it wants. The channel holds a message until a receiver takes it.

```text
  shop ----send----> [ orders.new       ] ----receive----> warehouse
  shop ----send----> [ orders.cancelled ] ----receive----> billing
                     messaging system: named channels
```

The participants are the sender, the receiver, the channel and the messaging system that keeps the channels. A broker such as RabbitMQ, a JMS provider, Azure Service Bus or Kafka is the messaging system. The names differ: a queue or a topic in JMS and Service Bus,[^3][^4] an exchange and its queues in RabbitMQ,[^5] and a topic in Kafka.[^6]

The following program shows the idea without a broker. A `deque` is one channel. The shop sends three messages while no receiver runs, and the warehouse reads its channel later. The message on the other channel stays where it is. A send to a name that does not exist fails.

```python run id=named-channels
from collections import deque


class Broker:
    """A messaging system with named channels. Each channel keeps its messages in order."""

    def __init__(self, *names):
        self.channels = {name: deque() for name in names}

    def send(self, channel, message):
        if channel not in self.channels:
            raise LookupError(f"no channel named {channel}")
        self.channels[channel].append(message)

    def receive(self, channel):
        waiting = self.channels[channel]
        return waiting.popleft() if waiting else None


broker = Broker("orders.new", "orders.cancelled")

# The shop sends. The warehouse program is not running yet.
broker.send("orders.new", {"order": 1, "sku": "A-7", "qty": 2})
broker.send("orders.new", {"order": 2, "sku": "B-1", "qty": 1})
broker.send("orders.cancelled", {"order": 1})
print("waiting:", {name: len(q) for name, q in broker.channels.items()})

# Later, the warehouse starts. It reads only the channel that it knows.
while (message := broker.receive("orders.new")) is not None:
    print("warehouse got", message)
print("waiting:", {name: len(q) for name, q in broker.channels.items()})

try:
    broker.send("order.new", {"order": 3})
except LookupError as e:
    print("send failed:", e)
```

```output
waiting: {'orders.new': 2, 'orders.cancelled': 1}
warehouse got {'order': 1, 'sku': 'A-7', 'qty': 2}
warehouse got {'order': 2, 'sku': 'B-1', 'qty': 1}
waiting: {'orders.new': 0, 'orders.cancelled': 1}
send failed: no channel named order.new
```

Hohpe and Woolf list the decisions that a set of channels must make.[^7] These are the main ones:

- **One receiver or all receivers.** A [[point-to-point-channel]] gives each message to one receiver. A [[publish-subscribe-channel]] gives a copy to each subscriber.
- **One type of data for each channel.** The receiver must know the structure of the message. If each channel carries one type of message, the name of the channel tells the receiver the type. This is the main reason why a system has many channels.[^7] The book calls it Datatype Channel.
- **Messages that a receiver cannot use, or that cannot be delivered.** A receiver puts a message that it does not understand on an Invalid Message Channel. The messaging system puts a message that it cannot deliver on a Dead Letter Channel.[^7]
- **A crash of the messaging system.** If the messages must survive a crash, the channel must keep them on disk. The book calls this Guaranteed Delivery.[^7]

A channel is in practice one-way: a program that reads the channel that it writes would read its own messages. Two programs that talk in both directions use two channels, as in Request-Reply.[^7] And the set of channels is usually fixed when the system is designed. The book gives two exceptions: a reply channel that a requester makes for its replies, and hierarchical channels, where a receiver subscribes to a parent and gets the messages of new child channels.[^7]

## Example

Each example file makes two channels, `orders` and `payments`, with the queue or channel type of its language. The shop and the bank send three messages before any receiver runs. Then a warehouse thread reads `orders` and an accounting thread reads `payments`, and each one sees only its own channel. The last line sends to the name `order`, which is not a channel, and gets an error.

The examples use the queues of the standard libraries in place of a broker, so that they run with no server. Each language has a type for the job. Go has channels in the language.[^8] .NET has `System.Threading.Channels`, which the documentation calls "an implementation of the producer/consumer conceptual programming model".[^9] Java has `BlockingQueue`, and its documentation says that implementations "are designed to be used primarily for producer-consumer queues".[^10] Python has the `queue` module.[^11] A broker client keeps the same shape: a send to a named destination and a receive from one.

## Consequences

**Benefits.**

- **The sender and the receiver do not need to know each other.** They agree on the name of the channel and the format of the messages. A new receiver can read the channel with no change to the sender.
- **The sender and the receiver do not need to run at the same time.** The channel holds the messages. Jakarta Messaging says that a queue "is always available to hold messages sent to it, whether or not the client that consumes its messages is active".[^3]
- **The name of the channel tells the receiver what it gets.** With one type of message for each channel, the receiver does not test each message to find out what it is.

**Liabilities.**

- **The channels are a design that all programs share.** A channel name is an interface. A change of a name or a format must reach all the programs that use it, as with an API.
- **A wrong name can lose messages without an error.** The program above fails on an unknown name, but a broker may not. In RabbitMQ, a producer sends to an exchange, not to a queue, and a message to an exchange with no bound queue is lost. The RabbitMQ tutorial says that "The messages will be lost if no queue is bound to the exchange yet".[^5] Declare the channels before the programs start, and watch the channels for messages that nobody reads.
- **Each channel costs something.** Jakarta Messaging says that "creating and maintaining each queue is somewhat costly", and that most queues are made by an administrator.[^3] Many short-lived channels are a load on the broker.
- **The communication is asynchronous.** The sender does not get an answer, or an error from the receiver. A reply needs a second channel, and a failure in the receiver needs a Dead Letter Channel or an alert.

## When not to use it

**When the caller needs the answer now.** A price check on a web page waits for the answer. A direct call with a timeout is simpler than a request channel, a reply channel and a correlation identifier.

**Inside one program.** Two parts of one program can call each other. A channel between them adds a copy of each message, a thread or task to read it, and an order of events that is harder to follow. A queue in memory inside one program is the [[producer-consumer]] pattern, not a messaging system: when the program stops, the messages are gone.

**One channel for all messages.** A single channel that carries every kind of message makes each receiver read, test and skip messages that it does not want. It is a sign that the design of the channels has not started.

## Variants and related patterns

**Point-to-Point Channel and Publish-Subscribe Channel.** The two kinds of channel: one receiver for each message, or a copy for each subscriber.[^7] See [[point-to-point-channel]] and [[publish-subscribe-channel]].

**Datatype Channel.** One type of message for each channel.[^7]

**Invalid Message Channel and Dead Letter Channel.** Channels for the messages that a receiver cannot use, and for the messages that the system cannot deliver.[^7]

**The other root patterns.** In the chapter on messaging systems, Message Channel comes first, and Message, Pipes and Filters, Message Router, Message Translator and Message Endpoint follow it.[^12] The book gives the full text of each one.[^13]

**Channels in a language.** Go channels, .NET channels and the [[channels]] of other languages are channels between the threads or tasks of one program, with no names and no broker. See also [[message-passing-concept]] and the [[actor-model]], where each actor has a mailbox.

**Work queues.** A channel whose messages are tasks for a pool of workers. See [[worker-queue]].

## Known uses

- Spring Framework: [MessageChannel.java at v6.2.10](https://github.com/spring-projects/spring-framework/blob/v6.2.10/spring-messaging/src/main/java/org/springframework/messaging/MessageChannel.java#L19-L58) is the interface for a channel inside a Spring application. Its comment says that it "Defines methods for sending messages", and `send` returns false when a message cannot be sent for a reason that is not fatal.[^14] Spring Integration builds its channels on it, for example `QueueChannel` and `PublishSubscribeChannel`.[^15]
- Jakarta Messaging calls a channel a destination: a `Queue` or a `Topic`.[^3]
- Kafka calls a channel a topic, and its documentation says that "Topics in Kafka are always multi-producer and multi-subscriber".[^6]

## References
