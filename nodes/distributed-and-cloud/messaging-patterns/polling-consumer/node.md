---
tag: BR46VV
kind: pattern
slug: polling-consumer
name: Polling Consumer
aliases: [Polling Consumer pattern, synchronous receiver, pull consumer, polling receiver]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A polling consumer asks the channel for a message when it is ready for one, so the application controls when and how fast it consumes; an event-driven consumer is called when a message comes.
family: integration
polarity: positive
intent: Let an application consume messages at its own pace, by a consumer that makes an explicit call to receive the next message, with no wait, a fixed wait or a wait until a message comes.
also_known_as: [synchronous receiver]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on messaging endpoints, as the pair of the Event-Driven Consumer.
see_also: [competing-consumers, message-channel, point-to-point-channel, event-driven-programming, producer-consumer, guarded-suspension, idempotent-receiver, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/PollingConsumer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Polling Consumer (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 10, Messaging Endpoints, Polling Consumer
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessagingEndpointsIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Messaging Endpoints
    locator: message consumer patterns; message endpoint themes
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/polling-consumer.adoc
    title: Apache Camel 4.14.0 documentation source, Polling Consumer
    locator: PollingConsumer API; two kinds of Polling Consumer implementations
  - url: https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/sqs-short-and-long-polling.html
    title: Amazon Simple Queue Service Developer Guide, Amazon SQS short and long polling
    locator: whole page
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/EventDrivenConsumer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Event-Driven Consumer (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; example, RabbitMQ consumer
---

## Intent

Let an application take the next message when it is ready for it, not when the message comes. The book asks: "How can an application consume a message when the application is ready?"[^1][^2] The answer: "The application should use a Polling Consumer", "one that explicitly makes a call when it wants to receive a message."[^1]

## Problem

Messages come at the pace of the senders. A receiver that is called for each message as it comes must keep up with that pace, or it gets more work than it can do. The book's chapter on endpoints says that consumers that are event-driven tend to process messages as fast as they come, which can overload a server.[^3]

Some receivers also have their own rhythm. A batch job runs once an hour. A worker takes the next job only when it has finished the last one.

The forces are these. The receiver wants control of when and how many messages it takes. But each call costs a request, and a call that finds nothing is wasted. And a long time between calls leaves messages waiting in the queue.

## Solution

Give the application a consumer object with a receive call. When the application is ready, it calls receive, processes the message, and then calls receive again.[^1] Between the calls, new messages wait in the channel.

```text
  [ queue ] <--- receive(wait) --- consumer <--- next, please ---------- application
            ---- message or none -->          ---- message --------------->
```

The participants are the channel, the consumer and the application. The book calls this a synchronous receiver, because the thread of the receiver blocks until a message comes. How the consumer gets the message from the messaging system is a detail of the implementation; the application only knows that it gets no message until it asks.[^1]

**Three ways to wait.** Messaging APIs usually have a receive that blocks until a message comes, and calls that return at once when there is none.[^1] Camel's `PollingConsumer` has `receive()`, which "Waits until a message is available and then returns it; potentially blocking forever", `receive(long)`, which waits up to a timeout, and `receiveNoWait()`, which returns at once.[^4]

**Short and long polling.** Each call is a request to the broker. With short polling, Amazon SQS answers at once, even when it finds no message. With long polling, it waits until at least one message is there, or until the wait time ends; the longest wait is 20 seconds. Long polling has fewer empty responses, and this can lower the cost.[^5] The program below compares three consumers on the same minute with four messages.

```python run id=poll-intervals
# Four messages arrive in one minute. Three consumers poll the same queue.
arrivals = [3.2, 3.4, 15.5, 41.7]
END = 60.0


def short_poll(interval):
    requests, empty, delays = 0, 0, []
    queue = list(arrivals)
    t = 0.0
    while t < END:
        requests += 1
        got = [a for a in queue if a <= t]
        queue = [a for a in queue if a > t]
        if not got:
            empty += 1
        delays += [t - a for a in got]
        t += interval
    return requests, empty, delays


def long_poll(wait):
    requests, empty, delays = 0, 0, []
    queue = list(arrivals)
    t = 0.0
    while t < END:
        requests += 1
        nxt = [a for a in queue if a <= t + wait]
        if not nxt:
            empty += 1
            t += wait
            continue
        first = min(nxt)
        t = max(t, first)  # the reply goes out when a message is there
        got = [a for a in queue if a <= t]
        queue = [a for a in queue if a > t]
        delays += [t - a for a in got]
    return requests, empty, delays


for name, (requests, empty, delays) in [
    ("short poll every 1 s", short_poll(1.0)),
    ("short poll every 10 s", short_poll(10.0)),
    ("long poll, wait up to 20 s", long_poll(20.0)),
]:
    print(f"{name}: {requests} requests, {empty} empty, "
          f"{len(delays)} messages, longest delay {max(delays):.1f} s")
```

```output
short poll every 1 s: 60 requests, 57 empty, 4 messages, longest delay 0.8 s
short poll every 10 s: 6 requests, 3 empty, 4 messages, longest delay 8.3 s
long poll, wait up to 20 s: 6 requests, 2 empty, 4 messages, longest delay 0.0 s
```

A short poll every second gets each message within a second, but 57 of its 60 requests are empty. A short poll every 10 seconds makes 6 requests, but a message waits up to 8.3 seconds. A long poll makes 6 requests too, with 2 empty ones, and takes each message as soon as it comes.

## Example

Each example file polls an in-memory queue. A poll waits up to 300 ms for the first message and then takes at most two, so the consumer decides the size of each batch. The first poll uses no wait and returns at once. Poll 4 starts on an empty queue, and a producer adds one order 50 ms later; the poll returns it at once. Poll 5 finds nothing in 300 ms, and the consumer stops.

## Consequences

**Benefits.**

- **Throttling.** The book says that polling gives the best throttling: when the server is busy, it does not call receive, so the messages stay in the queue.[^3]
- **Control of the batch.** The consumer chooses how many messages it takes in one call.
- **Transactions.** The book names one polling consumer as the safest way to use a transactional client.[^3]

**Liabilities.**

- **Wasted calls.** A poll that finds no message costs a request. In the program, 57 of 60 short polls are empty.
- **Delay.** A message waits until the next poll; a longer interval means a longer wait.
- **A thread for each consumer.** A blocking receive holds its thread while it waits.
- **False empty responses.** An SQS short poll asks only a subset of the servers, so a poll can return no message while the queue has messages. With fewer than 1,000 messages in the queue, a later request returns them.[^5]

## When not to use it

**When each message must be handled as soon as it comes.** An event-driven consumer is called by the messaging system for each message. The book asks: "How can an application automatically consume messages as they become available?" The answer: "The application should use an Event-Driven Consumer", which the messaging system hands each message as it is delivered.[^6]

**When there are many channels with few messages.** A blocked thread for each channel, or a poll loop for each one, costs more than one callback for each message.

## Variants and related patterns

**Event-Driven Consumer.** The other choice for a receiver. It has no thread of its own until the messaging system calls it with a message. RabbitMQ consumers are event-driven by default, and the EIP site uses one as its example.[^6] The book lists the choice between the two as a main decision for each message endpoint.[^3]

**Event-driven inside, polling outside.** Camel's default `EventDrivenPollingConsumer` receives the messages by events into an internal queue, of 1000 messages by default, and the application polls that queue.[^4] The book also says that the consumer may or may not poll the messaging system; the application only polls the consumer.[^1]

**Competing consumers.** A group of [[competing-consumers]] can be polling consumers, so that each one takes a message only when it is free.[^3]

**Guarded suspension.** In one program, a blocking receive on a queue is a [[guarded-suspension]]: the call waits until the condition, a message in the queue, is true.

## Known uses

- Apache Camel: `PollingConsumer` from `Endpoint.createPollingConsumer()`, the `ConsumerTemplate`, and the Poll Enrich EIP, as the [Polling Consumer page of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/polling-consumer.adoc) lists.[^4]
- Amazon SQS: each consumer calls `ReceiveMessage`; `WaitTimeSeconds` greater than 0 makes the call a long poll.[^5]
- JMS-style APIs: a `receive` that blocks, and `receiveNoWait()`, which returns at once if no message is there.[^1]

## References
