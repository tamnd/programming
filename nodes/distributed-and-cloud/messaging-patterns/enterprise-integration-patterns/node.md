---
tag: 97M5AX
kind: work
slug: enterprise-integration-patterns
name: Enterprise Integration Patterns
aliases: [EIP, EIPs, Enterprise Integration Patterns book, Hohpe and Woolf, Designing, Building, and Deploying Messaging Solutions]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A 2003 book by Gregor Hohpe and Bobby Woolf with 65 patterns for messaging between programs: channels, messages, routing, transformation, endpoints and system management, with an icon for each.
authors: [Gregor Hohpe, Bobby Woolf]
year: 2003
url: https://www.enterpriseintegrationpatterns.com/
see_also: [message-channel, point-to-point-channel, publish-subscribe-channel, message-passing-concept, event-driven-programming, observer-pattern, worker-queue, actor-model]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/books1.html
    title: Gregor Hohpe, Enterprise Integration Patterns site, Books
    locator: Messaging Patterns, Enterprise Integration Patterns
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: the whole book
  - url: https://www.enterpriseintegrationpatterns.com/
    title: Gregor Hohpe, Enterprise Integration Patterns site, Home
    locator: Patterns and Best Practices for Asynchronous Messaging; Messaging Patterns in Today's World; Work-in-progress, Conversation Patterns
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/toc.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Table of Contents
    locator: the whole table
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Messaging.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Messaging
    locator: problem and solution statements; pattern text
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageChannel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Message Channel
    locator: licence note at the foot of the page
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/enterprise-integration-patterns.adoc#L1-L8
    title: Apache Camel 4.14.0 source, enterprise-integration-patterns.adoc
    locator: lines 1 to 8, and the tables of patterns
  - url: https://docs.spring.io/spring-integration/reference/overview.html
    title: Spring Integration reference, Overview
    locator: Background; Goals and Principles
---

## Summary

*Enterprise Integration Patterns* is a book by Gregor Hohpe and Bobby Woolf, published by Addison-Wesley in 2003.[^1][^2] Its subtitle is *Designing, Building, and Deploying Messaging Solutions*. It is a pattern language for the integration of separate programs, most of all through asynchronous messaging. The site of the book says that the language has 65 patterns, and that they give "a technology-independent vocabulary and visual notation".[^3][^4]

The book starts from four styles of integration: File Transfer, Shared Database, Remote Procedure Invocation and Messaging. It then gives the rest of its patterns to the last one.[^4] The Messaging pattern states the reason: "Sending a message does not require both systems to be up and ready at the same time."[^5]

Each pattern has a name, an icon, a context, a problem stated as a question, a solution in one or two sentences, a sketch, and a longer discussion with examples.[^6] The site gives a short form of each pattern. Its licence note says that the pattern icon, the pattern name, the problem and solution statements, and the sketch can be reused under the Creative Commons Attribution licence, and that the other parts are protected by copyright.[^6] The pages on this site quote only those parts.

This page summarizes the book. It does not reproduce it. Read the book for the full text of the patterns.

### Contents

The table of contents puts the 65 patterns in eight groups.[^4]

| Group | Patterns | Count |
|---|---|---|
| Integration Styles | File Transfer, Shared Database, Remote Procedure Invocation, Messaging | 4 |
| Messaging Systems | Message Channel, Message, Pipes and Filters, Message Router, Message Translator, Message Endpoint | 6 |
| Messaging Channels | Point-to-Point Channel, Publish-Subscribe Channel, Datatype Channel, Invalid Message Channel, Dead Letter Channel, Guaranteed Delivery, Channel Adapter, Messaging Bridge, Message Bus | 9 |
| Message Construction | Command Message, Document Message, Event Message, Request-Reply, Return Address, Correlation Identifier, Message Sequence, Message Expiration, Format Indicator | 9 |
| Message Routing | Content-Based Router, Message Filter, Dynamic Router, Recipient List, Splitter, Aggregator, Resequencer, Composed Message Processor, Scatter-Gather, Routing Slip, Process Manager, Message Broker | 12 |
| Message Transformation | Envelope Wrapper, Content Enricher, Content Filter, Claim Check, Normalizer, Canonical Data Model | 6 |
| Messaging Endpoints | Messaging Gateway, Messaging Mapper, Transactional Client, Polling Consumer, Event-Driven Consumer, Competing Consumers, Message Dispatcher, Selective Consumer, Durable Subscriber, Idempotent Receiver, Service Activator | 11 |
| System Management | Control Bus, Detour, Wire Tap, Message History, Message Store, Smart Proxy, Test Message, Channel Purger | 8 |

The six patterns of Messaging Systems are the root of the language. A message travels on a channel, a router chooses the next channel, a translator changes the format, a pipe and filter chain connects the steps, and an endpoint connects a program to a channel. The other groups refine these six. Between the groups, the book has interludes with longer examples, for example request and reply in JMS and .NET, and one composed example built three times: with web services, with MSMQ and with TIBCO ActiveEnterprise. Near the end, a case study describes a bond trading system.[^4]

### The ideas that the book teaches

**Asynchronous messaging accepts the problems of distribution.** A remote program can be slow, stopped or gone. A message waits in a channel until the receiver takes it, so the sender does not wait. The Messaging pattern asks for messages that move "frequently, immediately, reliably, and asynchronously, using customizable formats".[^5]

**The channel is the contract.** The programs agree on channels and message formats, not on each other. See [[message-channel]], [[point-to-point-channel]] and [[publish-subscribe-channel]].

**Routing and transformation belong between the programs.** Routers, splitters, aggregators and translators sit on the channels. The programs at the ends do not change when the flow changes.

**Failure is part of the design.** Dead Letter Channel, Invalid Message Channel, Guaranteed Delivery, Idempotent Receiver and the patterns of system management are there because messages get lost, arrive twice, or arrive with a format that the receiver does not understand.

### Influence

The site says that the patterns spurred a generation of open source Enterprise Service Bus products, and it names Apache Camel, Mule, WSO2, Oracle Service Bus, Open ESB, SonicESB and Fuse ServiceMix.[^3] The documentation of Apache Camel says that "Camel supports most of the" patterns of the book, and it lists them with the icons and the problem statements.[^7] Spring Integration says that its design is inspired by the patterns, and that its concepts are discussed at length in the book.[^8] Its overview gives the book as published by Addison Wesley in 2004, but the site of the book gives 2003.[^8][^1]

The site also says that the same patterns appear in microservices, service meshes, serverless orchestrators and event buses, and it has examples for cloud services.[^3] Hohpe has started a set of Conversation Patterns, for exchanges with state between programs, as the start of a second volume.[^3]

### Pages on this site

The pages for the patterns of the book are in the area of distributed and cloud programming. These pages are written: [[message-channel]], [[point-to-point-channel]] and [[publish-subscribe-channel]]. The pages for the other core patterns follow.

## References
