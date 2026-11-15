---
tag: A019X3
kind: pattern
slug: splitter
name: Splitter
aliases: [Splitter pattern, message splitter, split EIP, debatching]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A splitter takes one message that holds a list of items, such as the lines of an order, and sends one new message for each item, so that each item can go its own way.
family: integration
polarity: positive
intent: Process each element of a composite message on its own, by a step that reads the message and publishes one message for each element, with headers that link the parts to the whole.
also_known_as: [message splitter, debatching]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) in the chapter on message routing; the page on the EIP site has the file name Sequencer.
see_also: [aggregator, content-based-router, correlation-identifier, scatter-gather, message-channel, iterator-pattern, composite-pattern, enterprise-integration-patterns]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Sequencer.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Splitter (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 7, Message Routing, Splitter
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/Aggregator.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Aggregator (problem and solution under CC BY)
    locator: pattern text, completeness condition
  - url: https://github.com/spring-projects/spring-integration/blob/v7.1.1/src/reference/antora/modules/ROOT/pages/splitter.adoc
    title: Spring Integration 7.1.1 reference source, Splitter
    locator: introduction; Programming Model
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-engine/src/main/docs/modules/eips/pages/split-eip.adoc
    title: Apache Camel 4.14.0 documentation source, Split EIP
    locator: Splitting with parallel processing; What is returned from Split EIP when its complete; Splitting modes; Stop processing in case of exception
  - url: https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/Splitter.java#L290-L303
    title: Apache Camel 4.14.0 source, Splitter.java
    locator: method updateNewExchange, lines 290 to 303
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/DistributionAggregate.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Composed Message Processor (problem and solution under CC BY)
    locator: problem and solution statements
  - url: https://docs.aws.amazon.com/step-functions/latest/dg/state-map.html
    title: AWS Step Functions Developer Guide, Map workflow state
    locator: introduction; Map state processing modes
---

## Intent

Handle each item of a large message as a message of its own. The book asks: "How can we process a message if it contains multiple elements, each of which may have to be processed in a different way?"[^1][^2] The answer: "Use a Splitter to break out the composite message into a series of individual messages, each containing data related to one item."[^1]

## Problem

An order from a customer has more than one line. Each line can need a different system: widgets are in one warehouse and gadgets in another. The shop wants to process the whole order, but it must check each line in the right system.[^1]

A [[content-based-router]] sends a whole message to one channel. It cannot send the first line of an order to one warehouse and the second line to another. A receiver that gets the whole order must know about all the lines and all the systems.

The forces are these. Each item must be processed alone, maybe in parallel. The parts must keep a link to the whole, so that a later step can put the results together. And a very large message must not fill the memory.

## Solution

Put a splitter after the channel of the composite messages. For each message, it finds the repeated elements and publishes one new message for each element, or for each group of elements.[^1] Each new message carries the data of its item and, as needed, data from the whole, such as the order number.

```text
                                       +--> [ A7 1/3 widget x2 ] --+--> inventory.widgets
  [ orders ] ---> splitter ---> items -+--> [ A7 2/3 gadget x1 ] --+--> inventory.gadgets
                                       +--> [ A7 3/3 widget x5 ] --+--> inventory.widgets
```

The participants are the input channel, the splitter, the output channel and the receivers of the items. Often a router follows the splitter, and an [[aggregator]] collects the results.

**Headers for the way back.** The book's Aggregator can know how many parts to expect when each part carries the total count.[^3] Spring Integration puts three headers on each part: `CORRELATION_ID`, `SEQUENCE_SIZE` and `SEQUENCE_NUMBER`.[^4] The [[correlation-identifier]] is the id of the whole, and the number and the size tell the aggregator which part this is and when it has all of them.

**Two modes.** Apache Camel splits in a default mode and in a streaming mode. In the default mode, it can know the total size, but it keeps all the sub messages in memory. In the streaming mode, it splits on demand and does not keep them in memory, but "the total size cannot be known ahead of time."[^5] The program below shows the difference with a generator that reads the lines of an order one at a time.

```python run id=split-modes
def lines():
    """The lines of an order, read one at a time, as from a file or a socket."""
    for n, text in enumerate(["widget x2", "gadget x1", "widget x5"], 1):
        print(f"  read line {n}")
        yield text


def split_list(order_id, source):
    items = list(source)  # all items are in memory before the first message
    for i, item in enumerate(items):
        yield {"order": order_id, "index": i, "size": len(items), "item": item}


def split_stream(order_id, source):
    it = iter(source)
    item, i = next(it, None), 0
    while item is not None:
        following = next(it, None)  # look ahead one item to know if this is the last
        msg = {"order": order_id, "index": i, "item": item, "complete": following is None}
        msg["size"] = i + 1 if following is None else "?"
        yield msg
        item, i = following, i + 1


for name, split in [("list mode", split_list), ("streaming mode", split_stream)]:
    print(name + ":")
    for m in split("A7", lines()):
        print(f"  send {m['order']} item {m['index'] + 1} of {m['size']}: {m['item']}")
```

```output
list mode:
  read line 1
  read line 2
  read line 3
  send A7 item 1 of 3: widget x2
  send A7 item 2 of 3: gadget x1
  send A7 item 3 of 3: widget x5
streaming mode:
  read line 1
  read line 2
  send A7 item 1 of ?: widget x2
  read line 3
  send A7 item 2 of ?: gadget x1
  send A7 item 3 of 3: widget x5
```

In list mode, all three lines are read before the first message goes out. In streaming mode, the splitter reads one line ahead, so it knows when an item is the last. Camel does the same: in `updateNewExchange`, a split from a collection sets the size at once, and in streaming mode the size is set on the last part, to `index + 1`.[^6]

## Example

Each example file splits two orders. Order A7 has three lines, and each line becomes a message with the order id, the sequence number and the size, and goes to the channel of its warehouse. Order B9 has no lines. A splitter that sends no message for it leaves a later aggregator with nothing to complete, so the examples send one message with size 0 to an `orders.empty` channel. This is a choice of the design, not a rule of the pattern.

## Consequences

**Benefits.**

- **Simple receivers.** Each receiver gets one item and does not need to know the structure of the whole message.
- **Parallel work.** The parts can go to different systems or threads. Camel can process each split message on its own thread.[^5]
- **Fit with other patterns.** Splitter, router and aggregator together make the Composed Message Processor of the book, which splits a message, routes the parts and puts the replies together again.[^7]

**Liabilities.**

- **The parts can fail alone.** By default, the Camel splitter goes on with the other parts when one part fails. Its example has an error on row 17 of 1000 rows, and Camel processes the other 983.[^5] An option `stopOnException` changes this.
- **The size can be unknown.** In streaming mode, or when the splitter returns an iterator, the size is not known in advance. In Spring Integration, `SEQUENCE_SIZE` is then 0, and the default release strategy of an aggregator does not release the group.[^4]
- **More messages.** One message becomes many, each with its own headers and its own cost on the broker.
- **Order.** The parts can arrive in a different order from the order of the items.

## When not to use it

**When all items go to the same place and the receiver can take the whole message.** One message is cheaper than many.

**When the items must be changed as one unit.** If the order must be accepted or refused as a whole, a split needs an aggregator and a rule for a partial failure. Keep the message whole if there is no need to split it.

**When the receiver wants batches.** A splitter can publish one message for a subset of the elements, not only for each element.[^1]

## Variants and related patterns

**Aggregator.** The reverse step: it collects related messages into one. Spring Integration says that splitters are very often "upstream producers in a pipeline that includes an aggregator".[^4] See [[aggregator]].

**Composed Message Processor.** A splitter, a router and an aggregator in one unit, for a message with many elements, each of which needs different processing.[^7]

**Scatter-Gather.** It sends one request to many receivers, where the splitter sends a different part to each receiver. See [[scatter-gather]].

**Iterator.** In one program, the [[iterator-pattern]] gives the elements of a collection one at a time; the streaming mode of a splitter uses an iterator to do the same for a message.[^5]

## Known uses

- Apache Camel: the Split EIP, with the default and the streaming modes; the [method updateNewExchange of Splitter.java at 4.14.0](https://github.com/apache/camel/blob/camel-4.14.0/core/camel-core-processor/src/main/java/org/apache/camel/processor/Splitter.java#L290-L303) sets the split index, the split size and a flag for the last part.[^6] By default, the result of the Split EIP is the original input message.[^5]
- Spring Integration: `AbstractMessageSplitter` fills in the correlation, the sequence number and the sequence size on each part.[^4]
- AWS Step Functions: the Map state runs "a set of workflow steps for each item in a dataset." Its inline mode allows up to 40 concurrent iterations, and its distributed mode up to 10,000 parallel child workflow executions.[^8]

## References
