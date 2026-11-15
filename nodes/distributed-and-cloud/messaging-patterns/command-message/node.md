---
tag: 3P6MZR
kind: pattern
slug: command-message
name: Command Message
aliases: [Command Message pattern, command, command message]
area: distributed-and-cloud/messaging-patterns
level: core
summary: >-
  A command message is a message that asks one receiver to do something, such as to reserve stock or to charge a card. It is a normal message whose body names an operation and gives its arguments.
family: integration
polarity: positive
intent: Ask another program to run an operation by sending a message that names the operation and carries its arguments, so that the call gets the benefits of messaging in place of a remote procedure call.
also_known_as: [command]
history:
  year: 2003
  people: [Gregor Hohpe, Bobby Woolf]
  note: Hohpe and Woolf named the pattern in Enterprise Integration Patterns (2003), with Document Message and Event Message as the other two kinds of message intent.
see_also: [point-to-point-channel, message-bus, message-channel, publish-subscribe-channel, guaranteed-delivery, dead-letter-channel, enterprise-integration-patterns, message-passing-concept]
sources:
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/CommandMessage.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Command Message (problem and solution under CC BY)
    locator: problem and solution statements; pattern text
  - book: isbn:9780321200686
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Designing, Building, and Deploying Messaging Solutions, Addison-Wesley, 2003
    locator: Chapter 5, Message Construction, Command Message and Request-Reply
  - url: https://jakarta.ee/specifications/messaging/3.1/jakarta-messaging-spec-3.1
    title: Jakarta Messaging 3.1 specification
    locator: 3.11 Jakarta Messaging message body
  - url: https://docs.particular.net/nservicebus/messaging/messages-events-commands
    title: Particular Software, NServiceBus documentation, Messages, events, and commands
    locator: Commands; Commands vs Events
  - url: https://masstransit.io/documentation/concepts/messages
    title: MassTransit documentation, Messages
    locator: Message Names; Commands
  - url: https://www.enterpriseintegrationpatterns.com/patterns/messaging/MessageConstructionIntro.html
    title: Gregor Hohpe and Bobby Woolf, Enterprise Integration Patterns, Introduction to Message Construction
    locator: Message intent; Request-Reply
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/io/ObjectInputStream.html
    title: Java SE 21 API, Class ObjectInputStream
    locator: class description, the warning on untrusted data
  - url: https://learn.microsoft.com/en-us/dotnet/standard/serialization/system-text-json/polymorphism
    title: .NET documentation, How to serialize properties of derived classes with System.Text.Json
    locator: Polymorphic type discriminators
  - url: https://github.com/Particular/NServiceBus.MessageInterfaces/blob/1.0.0/src/NServiceBus.MessageInterfaces/ICommand.cs#L3-L8
    title: NServiceBus.MessageInterfaces 1.0.0 source, ICommand.cs
    locator: interface ICommand, lines 3 to 8
---

## Intent

Use a message to call an operation in another program. The book asks: "How can messaging be used to invoke a procedure in another application?"[^1][^2] The answer: "Use a Command Message to reliably invoke a procedure in another application."[^1] The message names the operation and carries its arguments, and the receiver runs the operation.

## Problem

A shop must reserve stock in the warehouse program for each order. The easy way is a remote procedure call: the shop calls `reserveStock` over the network and waits. But then the shop stops when the warehouse is down or slow, and the call is lost when the network fails.

The forces are these. The shop needs a specific action, not only a notice: the warehouse must reserve the stock. The shop and the warehouse should not need to run at the same time. And the request must not be lost, also when the warehouse is down for an hour.

## Solution

Put the request in a message. The book says that there is no special message type for commands: a Command Message is a normal message that contains a command.[^1] In Jakarta Messaging, it can be any message type, for example an `ObjectMessage`, "a message that contains a serializable Java object",[^3] or a text message with the command in XML.[^1]

```text
  shop --- ReserveStock {sku: A1, qty: 2} ---> [ stock.commands ] ---> warehouse
           a name and the arguments              point-to-point         runs reserve_stock
```

The participants are the sender, the command message, a channel and the receiver that owns the operation. A command message has three usual parts:

- **The name of the command.** It tells the receiver which operation to run. NServiceBus and MassTransit advise a verb and a noun, in the "tell" style, such as `SubmitOrder`.[^4][^5]
- **The arguments.** The data that the operation needs.
- **Headers for the conversation.** A message id, and, if the sender wants an answer, a reply channel.

**One owner.** A command asks for an action, so one receiver must do it. NServiceBus says: "Typically, a command should only be consumed by a single consumer."[^4] It also says that a command should be sent to its logical owner, and it throws an exception when a program tries to publish a command.[^4] A command on a [[publish-subscribe-channel]] runs once for each subscriber, so a card is charged once for each copy. Send commands on a [[point-to-point-channel]].

**Command, document and event.** The book gives three kinds of message intent. A Command Message asks the receiver to run a function, a Document Message carries a data structure, and an Event Message tells the receivers that something changed in the sender.[^6] NServiceBus puts the difference between commands and events in its table: a command is "Used to make a request to perform an action", and an event is "Used to communicate that an action has been performed".[^4]

**A reply.** A command can need an answer. In the Request-Reply pattern, the request is usually a Command Message, and the reply is a Document Message with a result or an exception; the request gives the channel for the reply.[^6] The program below sends three commands to a stock service on a queue, each with an id and a reply channel. A thread of the service runs the commands, and it puts a reply with a result or an error on the reply channel.

```python run id=command-with-reply
import json
import queue
import threading

channels = {"stock.commands": queue.Queue(), "shop.replies": queue.Queue()}
stock = {"A1": 5}


def reserve_stock(sku, qty):
    if sku not in stock:
        raise KeyError(f"unknown sku {sku}")
    if stock[sku] < qty:
        raise ValueError(f"not enough stock, {stock[sku]} left")
    stock[sku] -= qty
    return {"sku": sku, "left": stock[sku]}


HANDLERS = {"ReserveStock": reserve_stock}


def receiver():
    while (raw := channels["stock.commands"].get()) is not None:
        cmd = json.loads(raw)
        reply = {"in_reply_to": cmd["id"]}
        try:
            reply["result"] = HANDLERS[cmd["command"]](**cmd["args"])
        except (KeyError, ValueError) as e:
            reply["error"] = f"{type(e).__name__}: {e.args[0]}"
        channels[cmd["reply_to"]].put(json.dumps(reply))


t = threading.Thread(target=receiver)
t.start()
commands = [
    {"id": "c1", "command": "ReserveStock", "args": {"sku": "A1", "qty": 2}},
    {"id": "c2", "command": "ReserveStock", "args": {"sku": "Z9", "qty": 1}},
    {"id": "c3", "command": "ReserveStock", "args": {"sku": "A1", "qty": 9}},
]
for cmd in commands:
    cmd["reply_to"] = "shop.replies"
    channels["stock.commands"].put(json.dumps(cmd))
    print("sent", cmd["id"], cmd["command"], cmd["args"])
channels["stock.commands"].put(None)
t.join()

while not channels["shop.replies"].empty():
    reply = json.loads(channels["shop.replies"].get())
    print("reply to", reply["in_reply_to"], reply.get("result") or reply.get("error"))
```

```output
sent c1 ReserveStock {'sku': 'A1', 'qty': 2}
sent c2 ReserveStock {'sku': 'Z9', 'qty': 1}
sent c3 ReserveStock {'sku': 'A1', 'qty': 9}
reply to c1 {'sku': 'A1', 'left': 3}
reply to c2 KeyError: unknown sku Z9
reply to c3 ValueError: not enough stock, 3 left
```

The sender does not wait for each reply. It sends the three commands, and it matches the replies to the commands by the field `in_reply_to`.

## Example

Each example file sends three command messages on a channel in memory: two `ReserveStock` commands and one `ChargeCard` command. Each message is only bytes on the channel. The receiver reads the bytes, finds the name of the command, and runs the handler for it. The second reservation asks for more stock than there is, and the handler answers so.

The examples use the serialization of the standard library: `json` in Python, `encoding/json` in Go, `System.Text.Json` in C#, and the Java serialization in Java, which is the form that the book names for an `ObjectMessage`.[^1] With Java serialization, the receiver must check the classes that it reads: the Java documentation warns that "Deserialization of untrusted data is inherently dangerous and should be avoided."[^7] The Java example sets a filter that accepts only its own classes. In C#, the attribute `JsonPolymorphic` and one `JsonDerivedType` for each command read the name of the command from a property of the JSON; the documentation calls this property a type discriminator.[^8]

## Consequences

**Benefits.**

- **The sender does not wait for the receiver.** The command waits in the channel when the receiver is down or busy.
- **Reliable calls.** With [[guaranteed-delivery]], a command is not lost when a program or the network fails.
- **Load is shared.** Many copies of the receiver can read one queue of commands, and each command goes to one of them.

**Liabilities.**

- **No answer at once.** A sender that needs the result must use a reply channel and match the replies, as in the program above.
- **Duplicates.** With at-least-once delivery, a command can arrive twice. A command such as `ChargeCard` must carry an id, and the receiver must find the second copy.
- **The sender knows the receiver.** A command names an operation of one service, so the sender depends on that service and its contract. An event does not have this dependency.
- **Old commands.** A command that waits a long time in a queue can be out of date when it runs. A message expiration, or a check of the state before the work, prevents an old command from doing harm.
- **Commands that always fail.** A command that the receiver cannot run goes back to the queue again and again; send it to a [[dead-letter-channel]] after a limit.

## When not to use it

**When many programs must react.** If the sender only reports a fact, such as an order that was placed, send an Event Message on a [[publish-subscribe-channel]]. The sender then does not need to know who reacts.

**When the sender must have the answer before it can go on.** A user who waits for a price needs it in a fraction of a second. A direct call is simpler than a command and a reply channel.

**For data that the receiver only stores.** A message that only carries a data structure, with no operation, is a Document Message.[^6]

## Variants and related patterns

**Document Message and Event Message.** The two other kinds of message intent.[^6]

**Request-Reply.** A command as the request and a document as the reply.[^6]

**Message Bus.** The common command set of a [[message-bus]] is a set of command messages that all the programs understand.

**The Command pattern.** In object-oriented design, the [[command-pattern]] makes a request into an object. A command message is a similar idea between programs, but the request is data in a message, not an object in memory, so it can wait in a queue and cross a network.

## Known uses

- NServiceBus: [ICommand.cs at 1.0.0 of NServiceBus.MessageInterfaces](https://github.com/Particular/NServiceBus.MessageInterfaces/blob/1.0.0/src/NServiceBus.MessageInterfaces/ICommand.cs#L3-L8) is the "Marker interface to indicate that a class is a command message."[^9] NServiceBus uses it to route commands and to refuse to publish them.[^4]
- MassTransit: commands that are sent to a queue and consumed by one consumer.[^5]
- Jakarta Messaging: any message type can carry a command, for example an `ObjectMessage` or a `TextMessage`.[^1][^3]

## References
