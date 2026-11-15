---
tag: T6NQ7W
kind: pattern
slug: correlation-identifier
name: Correlation Identifier
aliases: [Correlation Identifier pattern, correlation ID, correlation id, CorrelationId]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A correlation identifier is a value in a reply message that names the request that the reply answers. The replier copies it from the request id, so a requestor with many requests can match each reply.
family: integration
polarity: positive
intent: Let a requestor match each reply to its request, by an id that the requestor puts in each request and that the replier copies into the reply.
also_known_as: [correlation ID]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) as a helper of Request-Reply; JMS already had the header field JMSCorrelationID.
see_also: [request-reply, command-message, event-message, point-to-point-channel, message-channel, guaranteed-delivery, dead-letter-channel, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/CorrelationIdentifier.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Correlation Identifier (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 5, Message Construction, Correlation Identifier
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/RequestReply.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Request-Reply
    locator: Asynchronous Callback
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 3.4.5 JMSCorrelationID
  - url: https://www.rabbitmq.com/tutorials/tutorial-six-python
    title: RabbitMQ tutorials, Remote procedure call (RPC), using the Pika Python client
    locator: Message properties; Correlation id
  - url: https://masstransit.io/documentation/concepts/messages
    title: MassTransit documentation, Messages
    locator: Message Headers; Message Correlation
  - url: https://github.com/open-telemetry/semantic-conventions/blob/v1.37.0/docs/messaging/messaging-spans.md
    title: OpenTelemetry semantic conventions v1.37.0, Semantic conventions for messaging spans
    locator: Conversations; Context propagation; messaging.message.conversation_id
  - url: https://github.com/rabbitmq/rabbitmq-java-client/blob/v5.22.0/src/main/java/com/rabbitmq/client/RpcClient.java#L272-L288
    title: RabbitMQ Java client v5.22.0 source, RpcClient.java
    locator: method handleDelivery of the reply consumer, lines 272 to 288
---

## Intent

Match each reply to its request. The book asks: "How does a requestor that has received a reply know which request this is the reply for?"[^1][^2] The answer: "Each reply message should contain a Correlation Identifier", which is "a unique identifier that indicates which request message this reply is for."[^1]

## Problem

A web shop asks a price program for three prices. It uses one reply channel for all its requests, as the [[request-reply]] pattern advises, so that it does not need a new channel for each request. The price program does not answer in the order of the requests: a short request can finish before a long one. Three replies come back on one channel, and each one says only a price.

The forces are these. The shop needs to know which price belongs to which item. It cannot use the order of the replies. And some replies on the channel are not for any request that the shop is still waiting for: a reply that came after a timeout, or a second reply after the replier ran a request two times.

## Solution

The requestor gives each request a unique id. The replier copies that id into the reply, as the correlation id. The requestor keeps a table of the requests that wait for a reply, and it finds the request of each reply by its correlation id.

```text
  requestor                                    replier
    pending {q1: A1, q2: B2}
    request {id: q1, body: A1} ---------------->
    request {id: q2, body: B2} ---------------->
    <---------------- reply {correlation_id: q2, body: 500 cents}
    <---------------- reply {correlation_id: q1, body: 1999 cents}
    pending {}
```

The book names six parts: the requestor, the replier, the request, the reply, the request id in the request, and the correlation id in the reply, which has the same value as the request id.[^1] With this id, many requests can share one reply channel, and one reply thread can serve many request threads.[^3]

**In the brokers.** Jakarta Messaging has the header field `JMSCorrelationID`: "A client can use the JMSCorrelationID header field to link one message with another. A typical use is to link a response message with its request message."[^4] The value can be the message id that the provider gave to the request, or a value of the application.[^4] AMQP 0-9-1 has the property `correlation_id`, "Useful to correlate RPC responses with requests."[^5] RabbitMQ sets it "to a unique value for every request."[^5]

**Unknown ids.** A reply can have an id that is not in the table. RabbitMQ says: "If we see an unknown correlation_id value, we may safely discard the message - it doesn't belong to our requests."[^5] One reason is a replier that stops after it sends the reply and before it acknowledges the request: after a restart, the replier processes the request again, and the requestor gets a second reply.[^5]

**Unique across restarts.** The id must not repeat while an old reply can still arrive. A counter that starts at 1 in each run of the requestor breaks this rule. In the program below, the requestor asks for A1, stops before it reads the reply, and starts again. Then it asks for B2, and the reply queue still holds the old reply.

```python run id=ids-across-restarts
import itertools

PRICES = {"A1": 1999, "B2": 500}


def replier(request):
    return {"correlation_id": request["id"], "body": PRICES[request["sku"]]}


def run(make_ids):
    reply_queue = []  # a durable reply queue: it keeps its messages when the requestor stops
    # First run: the requestor asks for A1 and stops before it reads the reply.
    ids = make_ids("run1")
    reply_queue.append(replier({"id": next(ids), "sku": "A1"}))
    # Second run: the requestor starts again and asks for B2.
    ids = make_ids("run2")
    pending = {}
    rid = next(ids)
    pending[rid] = "B2"
    reply_queue.append(replier({"id": rid, "sku": "B2"}))
    for reply in reply_queue:
        sku = pending.pop(reply["correlation_id"], None)
        if sku is None:
            print(f"  no request for {reply['correlation_id']}, reply dropped")
        else:
            print(f"  {sku} costs {reply['body']} cents (reply {reply['correlation_id']})")


print("a counter that starts again at 1:")
run(lambda run_id: map(str, itertools.count(1)))
print("an id that is unique across runs:")
run(lambda run_id: (f"{run_id}-{n}" for n in itertools.count(1)))
```

```output
a counter that starts again at 1:
  B2 costs 1999 cents (reply 1)
  no request for 1, reply dropped
an id that is unique across runs:
  no request for run1-1, reply dropped
  B2 costs 500 cents (reply run2-1)
```

With the counter, the old reply for A1 has the same id `1` as the new request, so the requestor shows the price of A1 for B2, and it drops the right reply. With an id that includes the run, the old reply matches no request and is dropped. A random UUID for each request gives the same safety.

**Conversations and traces.** An id can also link more than one pair of messages. MassTransit has the headers `ConversationId`, `CorrelationId` and `InitiatorId`, and outbound messages from a consumer keep the `ConversationId` of the message that it consumed.[^6] OpenTelemetry calls such a group of messages a conversation, and it records the id as `messaging.message.conversation_id`.[^7] Trace context is a different link: it connects the spans of a producer and of its consumers, and OpenTelemetry says that "A producer SHOULD attach a message creation context to each message."[^7] A correlation id says which request a reply answers. A trace says where a message came from, through all the steps.

## Example

Each example file sends three requests with the ids `q1`, `q2` and `q3`, and keeps a table of them. The replier answers in the opposite order, and the reply channel also holds an old reply `q0` from a request that timed out. The requestor drops `q0`, matches the other replies by their correlation ids, and at the end has no request that still waits.

## Consequences

**Benefits.**

- **One reply channel for many requests.** The requestor does not need a channel for each request.
- **Any order.** Replies can come in any order, from many copies of the replier.
- **Safe handling of strays.** Late and repeated replies match no request and can be dropped.

**Liabilities.**

- **A table in the requestor.** The requestor must keep the requests that wait, and remove each entry after its reply or its timeout. The RabbitMQ Java client removes the entry on a timeout to avoid a leak.[^8]
- **Ids must be unique.** A repeated id gives a wrong match with no error, as in the program above.
- **The table is lost on a crash.** After a restart, replies for the old requests have no entry. If the requests matter, the requestor must keep the table in durable storage.
- **One more field to copy.** Each replier must copy the id. A replier that forgets it gives replies that nobody can use.

## When not to use it

**When each request has its own reply channel.** With a temporary queue for each request, the channel already says which request the reply answers.

**When the requestor sends one request at a time and blocks.** One request waits at a time, but a check of the id still protects against a late reply from an earlier request.

**For tracing.** To follow a message through many services, use trace context, not a correlation id of one request and its reply.[^7]

## Variants and related patterns

**Request-Reply.** The pattern that needs the id.[^3]

**Message id as correlation id.** Jakarta Messaging links messages by the message id of the request, which starts with `ID:`.[^4]

**Conversation id.** One id for a whole conversation, not only one pair.[^6][^7]

## Known uses

- RabbitMQ Java client: the reply consumer in [RpcClient at v5.22.0](https://github.com/rabbitmq/rabbitmq-java-client/blob/v5.22.0/src/main/java/com/rabbitmq/client/RpcClient.java#L272-L288) reads the correlation id of each reply, removes the waiting request from its table, and logs "No outstanding request for correlation ID {}" when no request matches.[^8]
- Jakarta Messaging: `JMSCorrelationID` on a reply, with the message id of the request.[^4]
- MassTransit: `RequestId` is set by the request client and copied into the response, and `CorrelationId` identifies an operation or an event.[^6]

## References
