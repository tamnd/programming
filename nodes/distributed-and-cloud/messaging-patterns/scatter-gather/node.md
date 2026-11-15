---
tag: CDA0NF
kind: pattern
slug: scatter-gather
name: Scatter-Gather
aliases: [Scatter-Gather pattern, scatter gather, fan-out fan-in, fan out and fan in, best quote]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  Scatter-gather sends one request to many receivers, such as a request for a price to all suppliers, and then collects their replies into one message, for example the best price.
family: integration
polarity: positive
intent: Get one answer from many receivers, by sending a request to all of them through a recipient list or a publish-subscribe channel, and by an aggregator that combines the replies under a completion rule.
also_known_as: [fan-out fan-in, best quote]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on message routing, with the variants distribution and auction; the page on the EIP site has the file name BroadcastAggregate.
see_also: [aggregator, splitter, publish-subscribe-channel, request-reply, correlation-identifier, futures, fork-join, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/BroadcastAggregate.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Scatter-Gather (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; Examples
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 7, Message Routing, Scatter-Gather
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/RecipientList.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Recipient List (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Aggregator.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Aggregator (problem and solution under CC BY)
    locator: pattern text, completeness strategies
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/scatter-gather.adoc#L4-L13
    title: Spring Integration 7.1.1 reference source, Scatter-Gather
    locator: introduction, lines 4 to 13; Functionality; Configuring a Scatter-Gather Endpoint
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/recipientList-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Recipient List EIP
    locator: Using Recipient List; Using parallel processing; Using timeout
---

## Intent

Ask many receivers the same question and get one answer from their replies. The book asks: "How do you maintain the overall message flow when a message needs to be sent to multiple recipients, each of which may send a reply?"[^1][^2] The answer: "Use a Scatter-Gather that broadcasts a message to multiple recipients and re-aggregates the responses back into a single message".[^1]

## Problem

An item of an order is not in stock, and many outside suppliers can deliver it. Each supplier can have the item or not, with a different price and a different date. To get the best terms, the shop must ask all the suppliers and then choose.[^1]

One [[request-reply]] for each supplier, one after the other, makes the total time the sum of all the waits. And the caller must then keep the state of all the requests itself.

The forces are these. The requests must go out in parallel. The caller wants one result, not many replies. Some suppliers reply slowly or never, and the caller must not wait for ever.

## Solution

Make one component that sends the request to all receivers, and an [[aggregator]] that collects the replies into one message. The book describes it as a router for the request and an aggregator for the responses.[^1]

```text
                        +--> [ alpha ] --+
  request ---> scatter -+--> [ beta  ] --+--> [ replies ] ---> gather ---> best quote
                        +--> [ gamma ] --+        (correlation id, timeout)
```

The participants are the requester, the scatter step, the receivers, the channel of the replies and the gather step, which is an aggregator. Each reply carries a [[correlation-identifier]], so that the gatherer can find the request of each reply.

**Two ways to scatter.** The book gives two variants. In the distribution variant, a recipient list sends the request to the channel of each known receiver; the scatter-gather controls the list, but it must know the channel of each receiver. In the auction variant, a [[publish-subscribe-channel]] sends the request to any receiver that listens; there is one channel, but the scatter-gather does not control who answers.[^1] The book's question for the recipient list is: "How do we route a message to a list of dynamically specified recipients?"[^3]

**When to stop gathering.** The gatherer uses the completeness strategies of the aggregator.[^4] With the auction variant, the gatherer does not know how many replies to expect, so it needs a timeout. The program below gathers the same four replies with five strategies. Delta is down and never replies.

```python run id=gather-strategies
# The time in ms after which each supplier replies, and its price in cents. Delta is down.
replies = {"alpha": (30, 1250), "beta": (80, 1190), "gamma": (400, 1100), "delta": (None, None)}
arrived = sorted((t, name, price) for name, (t, price) in replies.items() if t is not None)


def gather(name, done, deadline=None):
    """Collect replies in the order of arrival until done() is true or the deadline comes."""
    got = []
    for t, supplier, price in arrived:
        if deadline is not None and t > deadline:
            ready = deadline
            break
        got.append((price, supplier))
        if done(got):
            ready = t
            break
    else:
        if deadline is None:
            print(f"{name:30}: no result, it waits for delta for ever")
            return
        ready = deadline
    price, supplier = min(got)
    print(f"{name:30}: {supplier} {price}, ready at {ready} ms, {len(got)} of {len(replies)} replies")


gather("wait for all", lambda got: len(got) == len(replies))
gather("wait for all, timeout 500 ms", lambda got: len(got) == len(replies), deadline=500)
gather("timeout 200 ms", lambda got: False, deadline=200)
gather("first reply", lambda got: len(got) == 1)
gather("200 ms, or 1200 or less", lambda got: any(p <= 1200 for p, _ in got), deadline=200)
```

```output
wait for all                  : no result, it waits for delta for ever
wait for all, timeout 500 ms  : gamma 1100, ready at 500 ms, 3 of 4 replies
timeout 200 ms                : beta 1190, ready at 200 ms, 2 of 4 replies
first reply                   : alpha 1250, ready at 30 ms, 1 of 4 replies
200 ms, or 1200 or less       : beta 1190, ready at 80 ms, 2 of 4 replies
```

To wait for all the replies with no timeout gives no result at all. A timeout of 500 ms finds the lowest price, from gamma, but the caller waits 500 ms. A timeout of 200 ms is faster and misses gamma. The first reply is fastest and the most expensive. A rule that stops at a price of 1200 or less, or at 200 ms, gets beta at 80 ms.

## Example

Each example file asks three suppliers for a price, with real concurrency: Python uses a thread pool and `wait` with a timeout, Go uses goroutines, a buffered channel and `select` on a timer, Java uses `CompletableFuture` with `completeOnTimeout`, and C# uses `Task.WhenAny` on all the tasks and a delay. Each supplier waits for a fixed time before it replies. Gamma takes 1 second, so with a limit of 400 ms, the result has only alpha and beta, and beta has the best price.

## Consequences

**Benefits.**

- **Parallel requests.** The total time is about the time of the slowest reply that the gatherer waits for, not the sum.
- **One result for the caller.** Spring Integration describes its scatter-gather as "a compound endpoint for which the goal is to send a message to the recipients and aggregate the results."[^5]
- **Free choice of the receivers.** In the auction variant, a new receiver subscribes and takes part with no change to the requester.[^1]

**Liabilities.**

- **Slow and lost replies.** Without a limit, one slow receiver holds the whole result. Spring Integration waits 30 seconds by default, and returns null on a timeout.[^5]
- **Work that goes on after the timeout.** In Camel, a timeout ends the wait, but "certain tasks for which it is challenging for Camel to shut down in a graceful manner may continue to run."[^6] The examples show the same: gamma still runs after the gatherer is done.
- **More load.** Each request becomes many requests, and each receiver does the work even if its reply is not used.
- **Partial results.** The caller must handle a result with fewer replies, or with none.

## When not to use it

**When one receiver is enough.** If one known receiver can answer, use a [[request-reply]].

**When the parts of the request are different.** If each receiver must get a different part of a message, a [[splitter]] and an aggregator, the Composed Message Processor of the book, fit better.

**When all the replies must be correct together.** If the result must include all receivers or fail, the timeout and the partial result do not fit; the caller needs *Wait for all* and an error path for a missing reply.

## Variants and related patterns

**Distribution and auction.** The two ways to scatter, by a recipient list or by a publish-subscribe channel.[^1]

**Fork-join.** In one program, [[fork-join]] starts tasks in parallel and waits for them all; scatter-gather does the same with messages between systems, and it can stop before all replies come.

**Futures.** The examples use [[futures]]: one future for each request, and a wait on all of them with a limit.

## Known uses

- Spring Integration: the `ScatterGatherHandler` combines a `PublishSubscribeChannel` or a `RecipientListRouter` with an `AggregatingMessageHandler`, as the [Scatter-Gather page at 7.1.1](https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/scatter-gather.adoc#L4-L13) says.[^5]
- Apache Camel: the Recipient List EIP sends the same message to many endpoints, in parallel with `parallelProcessing`, with a total `timeout`, and with an `AggregationStrategy` that makes one reply.[^6]
- Azure Durable Functions: the EIP site shows the fan-out and fan-in form, and names `Task.WhenAll` for wait for all, `Task.WhenAny` for first best, and `Task.WaitAll` for a fixed timeout.[^1]

## References
