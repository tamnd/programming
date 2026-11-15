---
tag: CRGF9V
kind: pattern
slug: content-enricher
name: Content Enricher
aliases: [Content Enricher pattern, data enricher, message enrichment, enrich EIP]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A content enricher is a translator step that uses a key in a message to get missing data from another source, such as a database or a service, and adds that data to the message before it goes on.
family: integration
polarity: positive
intent: Give the receiver all the data it needs, by a step that reads a key from the message, gets the missing data by computation, from the environment or from another system, and adds it to the message.
also_known_as: [data enricher]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on message transformation; the page on the EIP site has the file name DataEnricher.
see_also: [message-translator, claim-check, content-based-router, request-reply, polling-consumer, aggregator, dead-letter-channel, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/DataEnricher.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Content Enricher (problem and solution under CC BY)
    locator: problem and solution statements; pattern text; example with Amazon EventBridge Pipes
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 8, Message Transformation, Content Enricher
  - url: https://docs.aws.amazon.com/eventbridge/latest/userguide/pipes-enrichment.html
    title: Amazon EventBridge User Guide, Event enrichment in Amazon EventBridge Pipes
    locator: enrichment step; filtering events using enrichment; invoking enrichments
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/enrich-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Enrich EIP
    locator: introduction; content enrichment using Enrich EIP; aggregation strategy is optional; using dynamic uris
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/ContentFilter.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Content Filter (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/pollEnrich-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Poll Enrich EIP
    locator: poll enrich with timeout
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/content-enrichment.adoc
    title: Spring Integration 7.1.1 reference documentation source, Content Enricher
    locator: header enricher; payload enricher
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/StoreInLibrary.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Claim Check (problem and solution under CC BY)
    locator: pattern text
---

## Intent

Add the data that a receiver needs and the sender does not have. The book asks: "How do we communicate with another system if the message originator does not have all the required data items available?"[^1][^2] The answer: "Use a specialized transformer, a Content Enricher", "to access an external data source in order to augment a message with missing information."[^1]

## Problem

Two systems seldom keep the same data. The book gives examples. An address message has only a ZIP code, but the receiver needs a state code too. A system sends a customer id, but the receiver needs the name and the address of the customer. An order message has an order number, but the next system needs the id of the customer of that order.[^1]

The sender cannot always add the data. It does not have the data, or the data is not part of its domain. And the receiver must not change to look up the data itself, because then it depends on one more system.

## Solution

Put a step between the sender and the receiver. The step reads a key from the message, such as a customer id, uses it to get the missing data, and adds the data to the message.[^1] The fields of the original message can stay in the new message or go, as the receiver needs.[^1]

```text
                         key            data
                      +------> [ other system ] --+
                      |                           |
  sender --> [ in ] --> content enricher <--------+ --> [ out ] --> receiver
```

The participants are the incoming message with its key fields, the enricher, the resource that has the data, and the enriched message.

**Where the data comes from.** The book names three sources.[^1]

- **Computation.** The enricher calculates the data, for example a state code from a ZIP code, or the size of the message. This form needs no outside resource and is close to a plain [[message-translator]].[^1]
- **The environment.** The enricher gets the data from the operating system, for example the current time for a time stamp.[^1]
- **Another system.** This is the most common source: a database, a file, an LDAP directory, an application, or even a person who types the data.[^1]

**A lookup for each message.** When the data comes from another system, each message costs one call. A cache in front of that system saves calls, but a cached value can be old. The program below enriches 8 orders of 3 customers, and one customer moves after the fourth order.

```python run id=enrich-cache
# A content enricher gets the customer from another system for each order.
# The customer c1 moves after the fourth order. How many calls, and how many stale messages?
orders = [("o1", "c1"), ("o2", "c2"), ("o3", "c1"), ("o4", "c3"),
          ("o5", "c1"), ("o6", "c2"), ("o7", "c3"), ("o8", "c1")]


def run(cache_mode):
    directory = {"c1": "Leeds", "c2": "Hue", "c3": "Oslo"}
    cache, calls, stale = {}, 0, 0
    for i, (order, cid) in enumerate(orders, 1):
        if cache_mode == "none" or cid not in cache:
            calls += 1
            cache[cid] = directory[cid]
        city = cache[cid]
        if city != directory[cid]:
            stale += 1
        if i == 4:
            directory["c1"] = "York"
            if cache_mode == "event":
                cache.pop("c1", None)
    return calls, stale


for mode, label in [("none", "lookup for each message"),
                    ("keep", "cache, never cleared"),
                    ("event", "cache, cleared by a change event")]:
    calls, stale = run(mode)
    print("%-33s %d calls, %d stale" % (label + ":", calls, stale))
```

```output
lookup for each message:          8 calls, 0 stale
cache, never cleared:             3 calls, 2 stale
cache, cleared by a change event: 4 calls, 0 stale
```

Without a cache, each order makes a call, and each address is current. A cache that is never cleared needs only 3 calls, but 2 orders get the old city. When the customer system sends an event at the change and the enricher removes that entry from its cache, 4 calls are enough and no order is stale.

## Example

Each example file enriches 4 orders with the three sources: the state comes from the first three digits of the ZIP code, the time comes from a clock, and the name of the customer comes from a directory. The clock has a fixed start, so that the output is the same on each run. The fourth order has a customer that the directory does not know. The enricher does not send a message with a hole in it; it reports the order as invalid, so that a router can send it to an invalid message channel or a [[dead-letter-channel]].

## Consequences

**Benefits.**

- **The sender and the receiver stay as they are.** Neither one must know the other system that has the data.
- **One place for a lookup.** Many flows can use the same enricher in front of the same resource.
- **The receiver gets a complete message.** It can work with no extra calls of its own.

**Liabilities.**

- **One more dependency in the path.** If the resource is slow or down, the flow waits or stops. EventBridge Pipes calls an enrichment synchronously and waits for its response before it calls the target.[^3]
- **Old data.** A cache saves calls but can give a value that has changed, as the program shows.
- **Larger messages.** Each enricher makes the message bigger. EventBridge Pipes limits the response of an enrichment to 6 MB.[^3]
- **Lost fields.** In Camel, the `enrich` step needs an `AggregationStrategy` to merge the original message and the reply. With no strategy, the reply becomes the result, and it replaces the original message.[^4]

## When not to use it

**When the receiver needs less, not more.** If the message has too much data for the receiver, use a Content Filter. The book asks: "How do you simplify dealing with a large message, when you are interested only in a few data items?" The answer: "Use a Content Filter to remove unimportant data items from a message leaving only important items."[^5]

**When the data can be computed.** If no outside resource is needed, a [[message-translator]] does the same work.[^1]

**When the call costs too much for each message.** A synchronous call for each message adds its latency to the whole flow.[^3] A receiver that needs the data only for a few messages can get it itself.

## Variants and related patterns

**Request-reply or polling.** Camel has two enrichers. `enrich` uses a producer and is usually a [[request-reply]] call, for example to a web service. `pollEnrich` uses a [[polling-consumer]] and usually reads one message from a resource, such as a file or an FTP server.[^4] The Camel page for `pollEnrich` says: "It is good practice to use timeout value."[^6] By default, the poll waits until a message comes, and it can block for ever; with a timeout of 0, it returns at once.[^6]

**Headers or the payload.** Spring Integration has a header enricher, which adds headers with fixed or computed values, and a payload enricher. The payload enricher sends the message to a request channel, waits for the reply like a gateway, and copies values from the reply into the payload with SpEL expressions.[^7] Its documentation says: "Enrichers are a variant of transformers."[^7]

**A dynamic resource.** Camel can compute the address of the resource from the message, for example an HTTP path with the order id in it.[^4]

**Claim Check.** A [[claim-check]] takes data out of a message for a time. Later, another component uses a content enricher to get the data back with the claim check.[^8]

**Enrich and filter.** An EventBridge Pipes enrichment that returns an empty response stops the event, so the step can also act as a filter.[^3]

## Known uses

- Apache Camel: the [Enrich EIP of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/enrich-eip.adoc) and the [Poll Enrich EIP of 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/pollEnrich-eip.adoc), with an `AggregationStrategy` to merge the data.[^4][^6]
- Spring Integration: the `<header-enricher>` and `<enricher>` elements, as the [Content Enricher page of 7.1.1](https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/content-enrichment.adoc) describes.[^7]
- Amazon EventBridge Pipes: the enrichment step, which can call a Lambda function, an API Gateway API, an API destination or an Express workflow of Step Functions.[^3] The EIP site shows a pipe in which a Lambda function gets the data from a DynamoDB table by a key in the message.[^1]

## References
