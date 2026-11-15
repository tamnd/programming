---
tag: VFFEC5
kind: pattern
slug: message-bus
name: Message Bus
aliases: [Message Bus pattern, messaging bus, service bus, integration bus]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A message bus is a shared messaging infrastructure with a common data model and command set, so that many programs can work together and a program can join or leave without changes to the others.
family: integration
polarity: positive
intent: Connect many programs through one shared set of channels, message formats and commands, so that each program needs only one connection, to the bus, and programs can be added or removed without changes to the others.
also_known_as: [service bus, integration bus]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003) and compared it to the bus of a computer, which connects the CPU, the memory and the devices.
see_also: [message-channel, publish-subscribe-channel, command-message, enterprise-integration-patterns, mediator-pattern, event-aggregator, observer-pattern, event-driven-programming]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageBus.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Message Bus (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 4, Messaging Channels, Message Bus; Chapter 8, Message Transformation, Canonical Data Model
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/CanonicalDataModel.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Canonical Data Model (problem and solution under CC BY)
    locator: problem and solution statements
  - url: https://github.com/MassTransit/MassTransit/blob/v8.5.5/src/MassTransit.Abstractions/IBus.cs#L6-L20
    title: MassTransit 8.5.5 source, IBus.cs
    locator: interface IBus, lines 6 to 20
  - url: https://masstransit.io/documentation/concepts/messages
    title: MassTransit documentation, Messages
    locator: Commands; Events
  - url: https://www.freedesktop.org/wiki/Software/dbus/
    title: freedesktop.org, D-Bus
    locator: What is D-Bus?
---

## Intent

Give many programs one shared way to talk. The book asks: "What is an architecture that enables separate applications to work together, but in a decoupled fashion such that applications can be easily added or removed without affecting the others?"[^1][^2] A Message Bus is the answer: one messaging infrastructure, one data model and one set of commands that all the programs use.

## Problem

A company has a shop, a billing program, a warehouse program and a report program. Each one has its own data format. At first, the shop calls billing and the warehouse directly, with a translation for each pair. Then the report program must also see the orders, and the shop must change again.

The forces are these. With a direct connection for each pair, the number of connections grows with the square of the number of programs: 4 programs that each talk to all the others need 12 one-way translations. A change of one format breaks each program that talks to it. And the programs are often old or bought, so they cannot all change to one format.

## Solution

"Structure the connecting middleware between these applications as a Message Bus that enables them to work together using messaging."[^1] The book names the parts of the bus, by analogy with the bus of a computer:[^1]

- **A common infrastructure.** A messaging system carries the messages between the programs, often with routers or [[publish-subscribe-channel]]s.
- **Adapters.** Each program connects to the bus through an adapter that translates between the format of the program and the format of the bus.
- **A common data model.** The messages on the bus use one format that all the programs agree on.
- **A common command set.** The bus has a set of commands that all the participants understand; see [[command-message]].

```text
  shop ------- adapter --+                          +-- adapter ------- billing
  "order 42 total 19.99" |                          |   charge 1999 cents
                         +---[ bus: OrderPlaced ]---+
                         |   order=42               |
  analytics -- adapter --+   amount_cents=1999      +-- adapter ------- warehouse
  (added later)                                         SHIP|42
```

The common data model is a pattern of its own, the Canonical Data Model: "design a Canonical Data Model that is independent from any specific application. Require each application to produce and consume messages in this common format."[^3] With it, a new program needs only its translation to and from the common format, and the number of programs that are already on the bus does not matter.[^3] For 4 programs, this is 4 adapters in place of 12 translations.

The common command set lets the bus check the messages. The program below gives the bus three commands, each with its fields. The bus sends a valid command to the channel of the command, and it puts a message with an unknown command name or with the wrong fields on an invalid message channel, with the sender and the reason.

```python run id=command-set
# The bus knows a common command set: the name and the fields of each command.
COMMANDS = {
    "ReserveStock": {"order", "sku", "qty"},
    "ChargeCard": {"order", "cents"},
    "ShipOrder": {"order", "address"},
}

channels = {name: [] for name in COMMANDS}  # one channel for each command
invalid = []  # the Invalid Message Channel of the bus


def subscribe(command, handler):
    channels[command].append(handler)


def send(sender, msg):
    name = msg.get("command")
    fields = set(msg) - {"command"}
    if name not in COMMANDS:
        invalid.append((sender, msg, "unknown command"))
    elif fields != COMMANDS[name]:
        missing = ", ".join(sorted(COMMANDS[name] - fields)) or "none"
        extra = ", ".join(sorted(fields - COMMANDS[name])) or "none"
        invalid.append((sender, msg, f"missing {missing}; extra {extra}"))
    else:
        for handler in channels[name]:
            handler(msg)


subscribe("ReserveStock", lambda m: print(f"warehouse: reserve {m['qty']} x {m['sku']} for {m['order']}"))
subscribe("ChargeCard", lambda m: print(f"billing: charge {m['cents']} cents for {m['order']}"))
subscribe("ShipOrder", lambda m: print(f"shipping: ship {m['order']} to {m['address']}"))

send("shop", {"command": "ReserveStock", "order": 42, "sku": "A1", "qty": 2})
send("shop", {"command": "ChargeCard", "order": 42, "cents": 1999})
send("mobile-app", {"command": "ChargeCard", "order": 43, "amount": 5.0})
send("partner", {"command": "ReserveStok", "order": 44, "sku": "B7", "qty": 1})
send("shop", {"command": "ShipOrder", "order": 42, "address": "Hue"})

for sender, msg, why in invalid:
    print(f"invalid from {sender}: {msg['command']}: {why}")
```

```output
warehouse: reserve 2 x A1 for 42
billing: charge 1999 cents for 42
shipping: ship 42 to Hue
invalid from mobile-app: ChargeCard: missing cents; extra amount
invalid from partner: ReserveStok: unknown command
```

The command from `mobile-app` has a field `amount` in place of `cents`, and the command from `partner` has a wrong name. Without the common command set, billing would get a message that it cannot read, and the warehouse would never see the reservation.

## Example

Each example file makes a small bus in memory with one message type, `OrderPlaced`, in a common format: an order number and an amount in cents. The shop writes its orders as text, such as `order 42 total 19.99`, and its adapter translates the text to the common message. Billing and the warehouse read the common message, and the warehouse adapter writes the format of the warehouse, `SHIP|42`. Then an analytics program joins the bus. It needs only a subscription to `OrderPlaced`, and the shop, billing and the warehouse do not change.

The examples use the maps, lists and records of the standard library in place of a broker.

## Consequences

**Benefits.**

- **Few connections.** Each program has one adapter to the bus, not one connection to each other program.
- **Programs join and leave.** A new program subscribes to the messages that it needs, and the others do not change, as the examples show.
- **One place for rules.** The bus can check the messages against the common model and the command set, and it can watch and log all the traffic.

**Liabilities.**

- **The common model is hard to agree on.** All the programs must agree on the meaning of each field, and each change of the model is a change for all the adapters.
- **Two translations.** A message is translated from the sender format to the common format and again to the receiver format. Each translation costs time and can lose detail.
- **A central dependency.** When the bus is down, no program can talk to another. The bus must be as reliable as the most important program on it.
- **Logic moves into the bus.** A bus that also routes, transforms and runs business rules becomes a large program of its own, and a team that is not the owner of the programs must change it for each new need.

## When not to use it

**For two or three programs that will stay the same.** A direct channel between them is simpler than a common model and a set of adapters.

**Inside one program.** The objects of one program share a language and a process; use an [[event-aggregator]] or the [[observer-pattern]] in place of a bus.

**When the programs cannot agree on a model.** A bus with a common model that nobody follows is a set of point-to-point translations with more parts. Use a [[mediator-pattern]] or explicit translations for each pair.

## Variants and related patterns

**Canonical Data Model.** The common format of the bus.[^3]

**Message Channel.** The bus is a set of [[message-channel]]s with the same infrastructure and formats.

**Command Message.** The common commands of the bus are [[command-message]]s.

**Service bus libraries.** Some libraries use the name for a programming model over a broker. In MassTransit, the `IBus` interface is a publish endpoint and gives send endpoints,[^4] and its documentation says that a command should typically go to a single consumer, and that events are published, for example through `IBus`.[^5]

**D-Bus.** The name is also used for a bus for communication between programs on one computer. "D-Bus is a message bus system, a simple way for applications to talk to one another."[^6] It has a system daemon for events of the system, and a daemon for each user login session.[^6]

## Known uses

- MassTransit: [IBus.cs at v8.5.5](https://github.com/MassTransit/MassTransit/blob/v8.5.5/src/MassTransit.Abstractions/IBus.cs#L6-L20) defines the bus as "a logical element that includes a local endpoint and zero or more receive endpoints".[^4]
- D-Bus: a system daemon and a daemon for each user login session.[^6]

## References
