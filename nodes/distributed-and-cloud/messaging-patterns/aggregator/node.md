---
tag: M5QTGV
kind: pattern
slug: aggregator
name: Aggregator
aliases: [Aggregator pattern, message aggregator, aggregate EIP, gatherer]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  An aggregator collects related messages, such as the replies for the parts of one order, until the set is complete or a timeout comes, and then publishes one message made from them.
family: integration
polarity: positive
intent: Combine related messages into one, by a stateful filter that groups the messages by a correlation key, tests a completion condition, and then publishes one message from the group.
also_known_as: [message aggregator]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on message routing, as the stateful counterpart of the Splitter.
see_also: [splitter, scatter-gather, correlation-identifier, dead-letter-channel, guaranteed-delivery, event-message, request-reply, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Aggregator.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Aggregator (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; Example, Serverless Loan Broker on AWS
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 7, Message Routing, Aggregator
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/aggregator.adoc
    title: Spring Integration 7.1.1 reference source, Aggregator
    locator: introduction; Functionality; the release of groups; Expiring Groups
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/aggregate-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Aggregate EIP
    locator: introduction; Aggregate with persistent repository; Completion
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/aggregate/AggregateProcessor.java#L1525-L1546
    title: Apache Camel 4.14.0 source, AggregateProcessor.java, the check of the completion options
    locator: lines 1525 to 1546
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/aggregate/AggregateProcessor.java#L353-L358
    title: Apache Camel 4.14.0 source, AggregateProcessor.java, the check of the closed correlation keys
    locator: lines 353 to 358
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Resequencer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Resequencer (problem and solution under CC BY)
    locator: problem and solution statements
---

## Intent

Put the results of related messages together, so that a later step can work on them as one. The book asks: "How do we combine the results of individual, but related messages so that they can be processed as a whole?"[^1][^2] The answer: "Use a stateful filter, an Aggregator", whose task is "to collect and store individual messages until a complete set of related messages has been received. Then, the Aggregator publishes a single message distilled from the individual messages."[^1]

## Problem

A [[splitter]] sends each line of an order to its warehouse, and each warehouse replies. Or a request goes to many suppliers, and each one sends a quote. The next step needs all the replies: the shop can bill an order only when all its items are taken from the warehouses, and it can choose the best quote only when it has the quotes.[^1]

The replies come as separate messages, maybe in a different order, maybe with replies for other orders between them, and some can be late or never come.

The forces are these. The aggregator must know which messages belong together. It must know when a group is ready. And it must not wait for ever for a message that is lost.

## Solution

Put an aggregator on the channel of the replies. It keeps one group for each correlation key. When a message comes, the aggregator adds it to its group and tests the completion condition. When the condition is true, it makes one message from the group and publishes it.[^1]

```text
  [ A 2/3 ] [ B 1/2 ] [ A 1/3 ] [ B 2/2 ] [ A 3/3 ]
        \        |        |        |        /
         +--> aggregator: group A {1, 2, 3}  ---> [ A complete ]
              (store)     group B {1, 2}     ---> [ B complete ]
```

The participants are the input channel, the aggregator with its store of groups, and the output channel. The book names three decisions for each aggregator: the correlation, which decides which messages belong together; the completeness condition, which decides when to publish; and the aggregation algorithm, which decides how to make one message from the group.[^1]

**Correlation.** The key is often a [[correlation-identifier]]. Spring Integration groups messages by the `CORRELATION_ID` header by default, and a `CorrelationStrategy` can change this.[^3] In Apache Camel, the messages with the same correlation key go into one bucket.[^4]

**Completeness.** The book lists strategies. *Wait for all* waits for all the replies, so it has the most data for the choice, but one missing message stops the whole group. *Time Out* waits for a fixed time and then uses the replies that came. *First Best* uses the first reply and ignores the others. *Time Out with Override* stops early when a very good reply comes. *External Event* waits for a business event, such as the end of the trading day.[^1] Spring Integration releases a group by default when all messages of a sequence are present, by the `SEQUENCE_SIZE` header.[^3] Camel has `completionSize`, `completionTimeout`, `completionInterval` and `completionPredicate`, and "It's basically the first that triggers that wins."[^4]

**Timeouts and late messages.** The program below runs three aggregators on the same messages. Part 2 of order A comes late, at tick 12, and part 2 of order B never comes.

```python run id=timeouts-and-late-parts
# Each order has 3 parts. Part 2 of A comes late, at tick 12. Part 2 of B never comes.
arrivals = {1: ("A", 1), 2: ("B", 1), 3: ("A", 3), 4: ("B", 3), 12: ("A", 2)}
SIZE, END = 3, 20


def run(name, timeout, keep_closed):
    print(name + ":")
    groups = {}  # correlation key -> [parts, tick of the last part]
    closed = set()
    for tick in range(1, END + 1):
        if tick in arrivals:
            key, part = arrivals[tick]
            if key in closed:
                print(f"  tick {tick:2}: part {part} of {key} is late, discard it")
            else:
                g = groups.setdefault(key, [[], tick])
                g[0].append(part)
                g[1] = tick
                if len(g[0]) == SIZE:
                    print(f"  tick {tick:2}: {key} complete {sorted(g[0])}")
                    del groups[key]
        for key, (parts, last) in list(groups.items()):
            if timeout and tick - last >= timeout:
                print(f"  tick {tick:2}: {key} timed out, publish {len(parts)} of {SIZE} {sorted(parts)}")
                del groups[key]
                if keep_closed:
                    closed.add(key)
    for key, (parts, _) in groups.items():
        print(f"  tick {END}: {key} still waits with {len(parts)} of {SIZE}")


run("no timeout", 0, False)
run("timeout 5, forget the key", 5, False)
run("timeout 5, keep the key closed", 5, True)
```

```output
no timeout:
  tick 12: A complete [1, 2, 3]
  tick 20: B still waits with 2 of 3
timeout 5, forget the key:
  tick  8: A timed out, publish 2 of 3 [1, 3]
  tick  9: B timed out, publish 2 of 3 [1, 3]
  tick 17: A timed out, publish 1 of 3 [2]
timeout 5, keep the key closed:
  tick  8: A timed out, publish 2 of 3 [1, 3]
  tick  9: B timed out, publish 2 of 3 [1, 3]
  tick 12: part 2 of A is late, discard it
```

With no timeout, A completes when the late part comes, but B waits for ever, and its group stays in memory. With a timeout that forgets the key, the late part of A starts a new group, which times out and publishes a second, wrong result for A. With the key kept closed, the late part is found and discarded. Camel does this when the option closeCorrelationKeyOnCompletion is set: it gives a `ClosedCorrelationKeyException` to a message for a closed key.[^5][^6] In Spring Integration, a completed group stays as an empty group, and late messages go to the discard channel.[^3] So a timeout needs a rule for late messages too.

## Example

Each example file aggregates seven messages from a splitter, for orders A and B, in a mixed order. A group is complete when it has all the sequence numbers up to its size, and the items are joined in the order of the sequence numbers. A second copy of part 2 of A, as at-least-once delivery can give, is ignored. A copy of a part of B that comes after B is complete is discarded, because the key of B is closed.

## Consequences

**Benefits.**

- **One message for the next step.** The next step gets a whole order or the best quote, and does not need to know about the parts.
- **Parallel work before it.** The parts can be processed in parallel, and the aggregator puts the results together.
- **A place for the rules.** The rules for missing and late parts are in one component.

**Liabilities.**

- **State.** The aggregator is stateful. Spring Integration says that it "must hold the messages to be aggregated" in a message store.[^3] If the process stops, an in-memory store loses the groups. Camel has a pluggable `AggregationRepository` for a persistent store.[^4]
- **A group that never completes.** With *Wait for all* and no timeout, one lost message keeps a group open for ever. Camel refuses to start an aggregator with no completion option: the message is "At least one of the completions options" and the list of the options "must be set".[^5]
- **Memory for closed keys.** To discard late messages, the aggregator must keep data about a group after the release. Spring Integration warns: "This can eventually cause out-of-memory conditions."[^3] Camel can keep the closed keys in an LRU cache with a fixed capacity.[^5]
- **Partial results.** After a timeout, the next step gets a group with missing parts and must handle it.

## When not to use it

**When each reply is useful alone.** If each part can be processed by itself, the aggregator only adds state and delay.

**When the parts come from one request and one reply.** A [[request-reply]] with one reply does not need a group.

**When the whole result must be all or nothing over many systems.** A timeout gives a partial group; a business process that must undo the work of the other parts needs a process manager or a saga, not only an aggregator.

## Variants and related patterns

**Splitter.** The step before it, which makes the parts and their sequence headers. See [[splitter]].

**Scatter-Gather.** A request goes to many receivers, and an aggregator collects their replies. See [[scatter-gather]].

**Resequencer.** Also a stateful filter that keeps messages. It answers the question "How can we get a stream of related but out-of-sequence messages back into the correct order?" and it publishes the messages in order, usually with no change to their content.[^7]

## Known uses

- Apache Camel: the Aggregate EIP, with an `AggregationStrategy` and the completion options; the [check at the start of AggregateProcessor in 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/aggregate/AggregateProcessor.java#L1525-L1546) makes a completion option mandatory.[^5]
- Spring Integration: `AggregatingMessageHandler` with a `CorrelationStrategy`, a `ReleaseStrategy` and a `MessageStore`.[^3]
- The Serverless Loan Broker of the EIP site: on AWS, a Lambda function with DynamoDB is the aggregator, with a completeness condition of a minimum number of answers, and AWS Step Functions adds the timeout.[^1]

## References
