---
tag: 8W7TX4
kind: pattern
slug: request-reply
name: Request-Reply
aliases: [Request-Reply pattern, request-response, request/reply, RPC over messaging]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  Request-Reply is a pair of messages on two channels: a requestor sends a request with a return address, and the replier sends the answer to that address. It gives two-way talk over one-way messages.
family: integration
polarity: positive
intent: Get an answer from another program over messaging, by a request message that names a reply channel and a reply message that the replier sends on that channel.
also_known_as: [request-response, request/reply]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003), with Return Address and Correlation Identifier as its two helper patterns.
see_also: [correlation-identifier, command-message, point-to-point-channel, message-channel, event-message, dead-letter-channel, guaranteed-delivery, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/RequestReply.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Request-Reply (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 5, Message Construction, Request-Reply and Return Address
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/ReturnAddress.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Return Address (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 2.16 Request/reply; 3.4.6 JMSReplyTo
  - url: https://www.rabbitmq.com/tutorials/tutorial-six-python
    title: RabbitMQ tutorials, Remote procedure call (RPC), using the Pika Python client
    locator: A note on RPC; Callback queue; Summary
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageConstructionIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Message Construction
    locator: Returning a response
  - url: https://www.rabbitmq.com/docs/direct-reply-to
    title: RabbitMQ documentation, Direct Reply-To
    locator: Overview; Motivation
  - url: https://github.com/rabbitmq/rabbitmq-java-client/blob/v5.22.0/src/main/java/com/rabbitmq/client/RpcClient.java#L305-L327
    title: RabbitMQ Java client v5.22.0 source, RpcClient.java
    locator: method doCall, lines 305 to 327
---

## Intent

Get an answer over messaging. A message goes one way, from a sender to a receiver, but a program often needs a result. The book asks: "When an application sends a message, how can it get a response from the receiver?"[^1][^2] The answer: "Send a pair of Request-Reply messages, each on its own channel."[^1]

## Problem

A web shop must show the price of an item, and the prices are in another program that reads a queue. The web shop can put a request on the queue, but a queue only carries messages away from the sender. The price program needs a way to send the answer back, and the web shop needs a way to wait for it.

The forces are these. The price program has many clients, such as the web shop and a mobile app, and it should not need a list of them. A client must not wait for ever when the price program is down. And the answer must go to the client that asked, not to all the clients.

## Solution

The requestor sends a request message on a request channel, and the replier sends a reply message on a reply channel. The book gives the two roles: the requestor sends the request and waits for the reply, and the replier gets the request and answers it.[^1] The request channel can be point-to-point or publish-subscribe, but the reply channel is almost always a [[point-to-point-channel]], because only the requestor needs the reply.[^1]

```text
  requestor --- request {id: q1, reply_to: replies.web, body: A1} ---> [ prices ] ---> replier
      ^                                                                                  |
      +------------- reply {correlation_id: q1, body: 1999 cents} <--- [ replies.web ] <-+
```

**A return address.** The replier must know where to send the reply. The Return Address pattern answers: "The request message should contain a Return Address that indicates where to send the reply message."[^3] Then the replier needs no list of its clients, and two clients can get their replies on different channels. In Jakarta Messaging, the address is the header field `JMSReplyTo`: "It is the destination where a reply to the message should be sent."[^4] In AMQP 0-9-1, the property `reply_to` is "Commonly used to name a callback queue."[^5]

**A correlation identifier.** When a requestor has many requests in progress, the reply must name the request that it answers.[^6] The page [[correlation-identifier]] gives the details.

**Which reply channel.** Jakarta Messaging gives "temporary queues and topics that can be used as a unique destination for replies."[^4] A new queue for each request costs a lot, and RabbitMQ advises: "A better approach is to create a single reply queue per requester and reuse it across requests."[^7] RabbitMQ also has a special address: "Direct Reply-To lets you implement RPC (request/reply) patterns like those in tutorial 6 without creating a dedicated reply queue".[^7]

**How the requestor waits.** The book gives two ways.[^1] In a synchronous block, one thread sends the request and waits for the reply. It is simple, but the thread holds one request at a time. In an asynchronous callback, one thread sends requests, and another thread reads all the replies and runs the callback of each request. Many requests can then share one reply channel.

**A timeout.** A reply can be late or never come. The RabbitMQ tutorial asks: "Should a client have some kind of timeout for the RPC?"[^5] In the program below, three requests go to one replier at tick 0, and the requestor waits 3 ticks for each reply.

```python run id=timeouts-and-late-replies
TIMEOUT = 3  # ticks that the requestor waits for each reply

# Three requests are sent at tick 0. One replier works on them one at a time.
requests = [("r1", 1), ("r2", 5), ("r3", 2)]  # the request id and its work in ticks

events = []
t = 0
for rid, work in requests:
    t += work
    events.append((t, 1, rid, "reply"))
    events.append((TIMEOUT, 0, rid, "timeout"))  # at a tie, the timeout comes first

pending = {rid for rid, _ in requests}
for tick, _, rid, kind in sorted(events):
    if kind == "timeout":
        if rid in pending:
            pending.discard(rid)
            print(f"tick {tick}: {rid} timed out")
    elif rid in pending:
        pending.discard(rid)
        print(f"tick {tick}: reply for {rid}")
    else:
        print(f"tick {tick}: late reply for {rid}, dropped")
```

```output
tick 1: reply for r1
tick 3: r2 timed out
tick 3: r3 timed out
tick 6: late reply for r2, dropped
tick 8: late reply for r3, dropped
```

The slow request `r2` times out. The request `r3` is fast, but it waits behind `r2` in the queue, so it also times out. Both replies come later, and the requestor drops them, because it gave up on those requests. A timeout does not tell the requestor whether the request was lost or only slow: the replier can still do the work.

## Example

Each example file has three named channels, as on a broker: `prices` for the requests, and a reply channel for each of two requestors, `web` and `mobile`. A replier thread reads `prices` and sends each reply to the channel in the `reply_to` field of the request. Each requestor waits at most 2 seconds for its reply. The third request asks for an unknown item, and the reply carries the error, so that the requestor does not wait for nothing.

## Consequences

**Benefits.**

- **Answers over messaging.** The requestor gets a result and keeps the benefits of a channel: the request waits in a queue when the replier is busy.
- **The replier does not know its clients.** Each request carries its return address, so new clients need no change in the replier.[^3]
- **Many replies on one channel.** With callbacks and a correlation identifier, one requestor can have many requests in progress.

**Liabilities.**

- **Waiting.** A requestor that waits for a reply is coupled in time to the replier again, as with a remote call. RabbitMQ says: "When in doubt avoid RPC."[^5]
- **Timeouts are hard to choose.** Too short, and fast requests behind a slow one fail, as in the program above. Too long, and the user waits.
- **Late replies.** After a timeout, the reply can still come. The requestor must drop it, and the work may already be done, so a second try must be safe to repeat.
- **State in the requestor.** With callbacks, the requestor must keep a table of the requests in progress and remove an entry after a timeout, or the table grows.
- **Errors.** The reply must also carry failures, or the requestor waits until the timeout for each error.

## When not to use it

**When no answer is needed.** Send an [[event-message]] or a [[command-message]] and do not wait.

**When the answer is needed at once and both programs are always up.** A direct call over HTTP or gRPC is simpler than two channels and a correlation table.

**For a long chain of steps.** RabbitMQ advises: "If you can, you should use an asynchronous pipeline - instead of RPC-like blocking, results are asynchronously pushed to a next computation stage."[^5]

## Variants and related patterns

**Return Address and Correlation Identifier.** The two helper patterns that the book names for the reply.[^6]

**Messaging RPC.** The request is a [[command-message]] and the reply is a document with a result or an exception.[^6]

**Requests to many repliers.** On a publish-subscribe request channel, the request goes to all the repliers that are interested, not to one.[^1]

**A reply that may not come.** A message that is not processed in time can go to a [[dead-letter-channel]].

## Known uses

- RabbitMQ Java client: [RpcClient.doCall at v5.22.0](https://github.com/rabbitmq/rabbitmq-java-client/blob/v5.22.0/src/main/java/com/rabbitmq/client/RpcClient.java#L305-L327) sets `correlationId` and `replyTo` on each request, waits with a timeout, and on a timeout removes the entry of the request from its table, with the comment "Avoid potential leak."[^8]
- Jakarta Messaging: `JMSReplyTo`, temporary queues, and the helper classes `QueueRequestor` and `TopicRequestor`.[^4]
- RabbitMQ: `reply_to` with a callback queue, and Direct Reply-To with no reply queue.[^5][^7]

## References
