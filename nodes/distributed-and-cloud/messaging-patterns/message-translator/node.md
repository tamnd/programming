---
tag: V6N3XH
kind: pattern
slug: message-translator
name: Message Translator
aliases: [Message Translator pattern, message transformer, transformer, translator, data mapping]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A message translator is a step between two systems that changes a message from the data format of one system into the format of the other, so that neither system must know the format of the other.
family: integration
polarity: positive
intent: Let systems with different data formats exchange messages, by a filter between them that reads a message in one format and writes the same data in the other format.
also_known_as: [message transformer, transformer]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) as one of the root patterns of messaging systems. The chapter on message transformation gives its variants, and the book compares it to the Adapter pattern of the Gang of Four.
see_also: [adapter-pattern, data-mapper, object-serialization, content-based-router, message-channel, message-bus, splitter, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageTranslator.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Message Translator (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 3, Messaging Systems, Message Translator; Chapter 8, Message Transformation
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageTransformationIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Message Transformation
    locator: introduction
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/CanonicalDataModel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Canonical Data Model (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/transformer.adoc
    title: Spring Integration 7.1.1 reference source, Transformer
    locator: introduction
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Normalizer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Normalizer (problem and solution under CC BY)
    locator: problem and solution statements
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/message-translator.adoc
    title: Apache Camel 4.14.0 documentation source, Message Translator
    locator: introduction
  - url: https://github.com/apache/kafka/blob/4.1.0/connect/api/src/main/java/org/apache/kafka/connect/transforms/Transformation.java#L26-L55
    title: Apache Kafka 4.1.0 source, Transformation.java
    locator: interface comment and method apply, lines 26 to 55
---

## Intent

Let two systems exchange messages when each one keeps its data in its own format. The book asks: "How can systems using different data formats communicate with each other using messaging?"[^1][^2] The answer: "Use a special filter, a Message Translator", which goes "between other filters or applications to translate one data format into another."[^1]

## Problem

Most systems that a company connects were not made to work together. Each one has its own data model. The book gives the example of a customer: the accounting system wants the tax number, and the customer relationship system keeps the phone numbers and the addresses. Each system expects messages in the form of its own data. Outside partners can also require a standard format of an industry body.[^1]

A [[message-channel]] removes the need for the sender to know where the receiver is. But if the sender must write each message in the format of the receiver, it still depends on the receiver. A change of the receiver, or a new receiver, still means a change of the sender.[^3]

The forces are these. The systems often cannot be changed. The formats differ in names, in units, in structure and in the encoding. And a new system must not mean a change to all the systems that already talk.

## Solution

Put a translator on the path of the message. It reads the message in the format of the source and writes a new message with the same data in the format of the target.[^1]

```text
  partner ---> [ partner.orders ] ---> message    ---> [ shop.orders ] ---> shop
                 id;name;12.50;dd/mm   translator       id, cents, ISO date
                                          |
                                          +---> [ invalid ] (a line it cannot read)
```

The participants are the source channel, the translator, the target channel and the two systems. Neither system knows the format of the other. The book compares the translator to the Adapter of the Gang of Four, which changes the interface of a component so that it can work in a different context.[^1] See [[adapter-pattern]].

**A canonical model.** With direct translators, each pair of systems needs its own translator. The book gives a Canonical Data Model: "How can you minimize dependencies when integrating applications that use different data formats?" The answer: "Therefore, design a Canonical Data Model that is independent from any specific application. Require each application to produce and consume messages in this common format."[^4] Each system then needs one translator to the common format and one from it. The book counts the translators: 2 systems need 2 direct translators or 4 with a canonical model, 3 systems need 6 with either approach, and 6 systems need 30 direct translators or 12 with the model.[^4] The program below translates one customer through a canonical format and prints the same counts.

```python run id=canonical-count
# Three systems keep a customer in three formats.
def crm_to_canonical(m):
    given, family = m["FullName"].split(" ", 1)
    return {"given": given, "family": family, "phone": m["Phone"]}


def canonical_to_billing(c):
    # Billing wants a tax id, but the CRM has none to give.
    return {"name": c["family"].upper() + ", " + c["given"], "tax_id": c.get("tax_id", "")}


def canonical_to_shop(c):
    return {"display": c["given"], "phone": c["phone"].replace(" ", "")}


crm_msg = {"FullName": "Lan Pham", "Phone": "+84 236 555 0101"}
c = crm_to_canonical(crm_msg)
print("crm      :", crm_msg)
print("canonical:", c)
print("billing  :", canonical_to_billing(c))
print("shop     :", canonical_to_shop(c))
print()
print("systems  direct translators  canonical translators")
for n in [2, 3, 6, 10]:
    print(f"{n:7}  {n * (n - 1):18}  {2 * n:21}")
```

```output
crm      : {'FullName': 'Lan Pham', 'Phone': '+84 236 555 0101'}
canonical: {'given': 'Lan', 'family': 'Pham', 'phone': '+84 236 555 0101'}
billing  : {'name': 'PHAM, Lan', 'tax_id': ''}
shop     : {'display': 'Lan', 'phone': '+842365550101'}

systems  direct translators  canonical translators
      2                   2                      4
      3                   6                      6
      6                  30                     12
     10                  90                     20
```

The counts are `n * (n - 1)` for direct translators and `2 * n` for the canonical model, when each system sends to each other system. The output also shows a limit: billing wants a tax id, but the customer system has none. A translator changes the form of data. It cannot make data that is not in the message.

## Example

Each example file translates lines from a partner, in the form `id;customer;amount;day/month/year`, into an order of the shop with an integer id, the total in cents and an ISO 8601 date. The line with the amount `7,25` has a comma as the decimal mark, and the translator rejects it with a reason, as it does not know which value the partner means.

## Consequences

**Benefits.**

- **Loose coupling of the formats.** The sender writes its own format, and the receiver reads its own format. Spring Integration says that transformers have "a very important role in enabling the loose-coupling of message producers and message consumers."[^5]
- **No change to the systems.** The translation is in the integration, so systems that cannot change can still take part.[^3]
- **One place for the mapping.** A change of a format is a change of one translator.

**Liabilities.**

- **Many translators.** With direct translation, the number grows with the square of the number of systems.[^4]
- **Loss of data.** When the target format has fewer fields, or a different precision, data is lost. A translator in the other direction cannot get it back.
- **Hidden rules.** A translator often contains business rules, such as a default value or a change of units, that nobody documents.
- **Cost on each message.** Each message is parsed and written again.

## When not to use it

**When both sides already use one format.** A shared schema or a canonical model with no difference to the format of a system needs no translator for that system.

**When the target needs data that the source does not have.** That is a content enricher, which gets the missing data from another place.[^3]

**When the only difference is the transport.** The book uses an envelope wrapper when the message must only be put into the form that the messaging infrastructure needs.[^3]

## Variants and related patterns

**Envelope Wrapper, Content Enricher, Content Filter and Claim Check.** The book calls these variants of the translator. They add a wrapper, add data, remove data, or put data in a store for later.[^3]

**Normalizer.** "How do you process messages that are semantically equivalent, but arrive in a different format?" The answer: "Use a Normalizer to route each message type through a custom Message Translator so that the resulting messages match a common format."[^6] It is a [[content-based-router]] with one translator for each format.

**Data Mapper.** In one program, a [[data-mapper]] moves data between objects and a database. The translator does the same work between two message formats.

## Known uses

- Apache Camel: the documentation of 4.14.0 lists four ways to translate, by the Transform or Set Body EIP, by a processor or a bean, by a template component, and by a data format to "marshal and unmarshal messages in different encodings".[^7]
- Apache Kafka Connect: the [Transformation interface at 4.1.0](https://github.com/apache/kafka/blob/4.1.0/connect/api/src/main/java/org/apache/kafka/connect/transforms/Transformation.java#L26-L55) is a "Single message transformation for Kafka Connect record types." It can "make lightweight message-at-a-time modifications." The method `apply` returns a new record, or null to drop the record.[^8]
- Spring Integration: the transformer is an endpoint for this pattern. It can call a method of a plain Java object that takes the payload of the message and returns the new payload.[^5]

## References
