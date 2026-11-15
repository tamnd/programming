---
tag: YDEMBE
kind: pattern
slug: remote-facade
name: Remote facade
aliases: [Remote Facade, session facade, coarse-grained remote interface]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A remote facade gives remote clients a few coarse-grained methods over many fine-grained objects, so one network call does the work of many local calls.
family: architectural
polarity: positive
intent: "Provide a coarse-grained facade on fine-grained objects to improve efficiency over a network."
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on distribution patterns. The Core J2EE patterns give the same idea for Enterprise JavaBeans as the Session Facade.
see_also: [facade-pattern, data-transfer-object, service-layer, proxy-pattern, domain-model, object-serialization, enterprise-application, adapter-pattern]
sources:
  - url: https://martinfowler.com/eaaCatalog/remoteFacade.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Remote Facade
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 15, Distribution Patterns, Remote Facade
  - url: https://learn.microsoft.com/en-us/azure/architecture/antipatterns/chatty-io/
    title: Microsoft, Azure Architecture Center, Chatty I/O antipattern
    locator: Problem description and How to fix the problem
  - url: https://martinfowler.com/articles/distributed-objects-microservices.html
    title: Martin Fowler, Microservices and the First Law of Distributed Objects, 13 August 2014
    locator: the whole article
  - url: https://www.oracle.com/java/technologies/session-facade.html
    title: Core J2EE Patterns, Session Facade
    locator: Problem, Solution and Consequences
  - url: https://martinfowler.com/bliki/LocalDTO.html
    title: Martin Fowler, Local DTO, 21 October 2004
    locator: the quote from Randy Stafford
---

## Intent

A remote facade "Provides a coarse-grained facade on fine-grained objects to improve efficiency over a network."[^1] Inside the process, the design keeps many small objects with small methods. At the process boundary, a few large methods do the work of a whole screen or a whole task in one call.[^1][^2]

## Problem

Small objects with small methods are good inside one process. Fowler: "This gives you lots of opportunity for control and substitution of behavior".[^1] But the price is many calls: "there's usually a great deal of interaction between objects".[^1]

This is not a problem in one address space, but a remote call costs much more: "Data may have to be marshaled, security may need to be checked, packets may need to be routed through switches."[^1] Fowler writes that "any inter-process call is orders of magnitude more expensive than an in-process call - even if both processes are on the same machine."[^1] A client that uses the small objects over the network makes one remote call for each small method, and the time of these calls adds up. Microsoft calls this design the chatty I/O antipattern, and notes that it "often happens when developers try to follow an object-oriented paradigm, and treat remote objects as if they were local objects in memory."[^3]

The remote interface must be coarse-grained, but a coarse-grained design for all the objects loses the benefits of the small objects.

## Solution

Keep the small objects, and put a coarse-grained facade in front of them for the remote clients only. "None of the fine-grained objects have a remote interface, and the Remote Facade contains no domain logic. All the Remote Facade does is translate coarse-grained methods onto the underlying fine-grained objects."[^1]

The parts are:

- **The fine-grained objects.** The [[domain-model]] or the other objects of the application, with no remote interface.
- **The remote facade.** A few methods, each of which does one task for a remote client, such as "get the order with its lines and its address". It calls the small objects, and it has no domain logic of its own.
- **The transfer data.** Each method takes and returns data, not references to objects. The data is often a [[data-transfer-object]], which the remote connection can serialize.
- **The remote connection.** HTTP, gRPC, a message queue or another way to make calls between processes.

```text
  remote client         one call: get_order(7)
       |                       ^
  ============= network ========|================== process boundary
       v                       |
  remote facade         get_order()  change_address()   (no domain logic)
                         |    |    \
  fine-grained objects  Order  Customer  Address  Line ... (many local calls)
```

The following program has one fine-grained object and a facade with one method. The client gets the three values of the customer in one remote call, as JSON text.

```python run id=one-coarse-call
import json


class Customer:  # a fine-grained object: one small method for each value
    def __init__(self, name, street, city):
        self._name, self._street, self._city = name, street, city

    def name(self):
        return self._name

    def street(self):
        return self._street

    def city(self):
        return self._city


class CustomerFacade:  # the remote facade: one coarse-grained method
    def __init__(self, customers):
        self._customers = customers

    def get_customer(self, number):
        c = self._customers[number]
        return {"name": c.name(), "street": c.street(), "city": c.city()}


class Network:  # counts the calls, and lets only JSON text cross
    def __init__(self, server):
        self.server, self.calls = server, 0

    def call(self, method, *args):
        self.calls += 1
        return json.loads(json.dumps(getattr(self.server, method)(*args)))


net = Network(CustomerFacade({1: Customer("Ann", "1 Elm Street", "Leeds")}))
doc = net.call("get_customer", 1)
print(doc["name"], "|", doc["street"], "|", doc["city"])
print("remote calls:", net.calls)
```

```output
Ann | 1 Elm Street | Leeds
remote calls: 1
```

## Example

Each example file has small domain objects: an order, its lines, a customer and an address. The remote facade has two methods. `get_order` returns all the data of one order, with the lines, the total and the address, in one reply. `change_address` takes the new address in one call and passes it to the address object. A `Network` object stands for the remote connection and counts the calls. The client shows the order, changes the address and reads the order again, with 3 remote calls in total. It also changes its own copy of the order, and the last lines show that this copy is not the object on the server.

## Consequences

**Benefits.**

- **Fewer remote calls.** One call does the work of many. Fowler's First Law of Distributed Object Design is "don't distribute your objects",[^4] and the remote facade is the way to have a remote interface without distributed objects.
- **Good design inside the process.** The small objects keep their small methods, their clear names and their polymorphism. Only the facade is coarse-grained.
- **One place for the concerns of the boundary.** Security checks and transactions are easier to manage on a few coarse-grained methods. The Core J2EE patterns say that the facade "offers a central place for managing and defining transaction control in a coarse-grained fashion."[^5]

**Liabilities.**

**A fine-grained remote interface makes many calls.** In the following program, the client uses the small objects of the order over the network. To read three lines, it makes 13 remote calls. If each call takes 20 ms, the read takes 260 ms, where one coarse-grained call takes 20 ms.

```python wrong id=chatty-remote-objects
class Line:
    def __init__(self, item, qty, price):
        self._item, self._qty, self._price = item, qty, price

    def item(self):
        return self._item

    def qty(self):
        return self._qty

    def price(self):
        return self._price


class Order:
    def __init__(self, lines):
        self._lines = lines

    def line_count(self):
        return len(self._lines)

    def line(self, i):
        return self._lines[i]


class Remote:  # a proxy that makes each method call a remote call
    calls = 0

    def __init__(self, target):
        self._target = target

    def __getattr__(self, name):
        method = getattr(self._target, name)

        def remote_call(*args):
            Remote.calls += 1
            result = method(*args)
            return Remote(result) if isinstance(result, (Order, Line)) else result

        return remote_call


# Wrong: the client uses the fine-grained objects over the network.
order = Remote(Order([Line("lamp", 2, 2500), Line("desk", 1, 7500), Line("pen", 4, 500)]))
total, items = 0, []
for i in range(order.line_count()):
    line = order.line(i)
    total += line.qty() * line.price()
    items.append(line.item())
print("items:", ", ".join(items), "total:", total)
print("remote calls:", Remote.calls)
print("at 20 ms for each call:", Remote.calls * 20, "ms")
```

```output
items: lamp, desk, pen total: 14500
remote calls: 13
at 20 ms for each call: 260 ms
```

Add a remote facade with one method that returns the whole order, and do not give the small objects a remote interface.

**Extra code at the boundary.** Each method of the facade must copy the data from the objects into a form for the wire, and back. This copy code grows with the number of screens and tasks.

**Coarse-grained methods are harder to use.** "A coarser-grained API is more awkward than a fine-grained one."[^4] A method that returns too much data wastes the network, and a method that returns too little sends the client back for more. Microsoft notes the tradeoff: in its example, if clients "often need just the user name", a separate call for the name can be better.[^3]

## When not to use it

**When there is no remote call.** Inside one process, a coarse-grained facade only adds code. Randy Stafford, quoted by Fowler, advises to "start with a locally invocable Service Layer whose method signatures deal in domain objects."[^6]

**When distribution is only a guess about the future.** Fowler warns against a "speculative distribution boundary", because "Adding remote boundaries adds complexity."[^6] Add the facade when a remote client needs it.

## Variants and related patterns

**On top of a service layer.** Stafford advises to "Add remotability when you need it (if ever) by putting Remote Facades on your Service Layer or having your Service Layer objects implement remote interfaces."[^6] The facade then passes each remote call to an operation of the [[service-layer]].

**Session Facade.** The Core J2EE patterns give the same idea for Enterprise JavaBeans: "Use a session bean as a facade to encapsulate the complexity of interactions between the business objects participating in a workflow."[^5]

**A facade for each kind of client.** Different clients, such as a mobile app and a web page, can need different shapes of data. Each kind of client can then have its own facade over the same objects.

**Related patterns.** A remote facade is a [[facade-pattern]] for a process boundary. "A Remote Facade is a coarse-grained facade [Gang of Four] over a web of fine-grained objects."[^1] Its methods take and return a [[data-transfer-object]]. On the client side, a [[proxy-pattern]] can give the remote facade the look of a local object, but the calls stay coarse-grained.

## Known uses

- Session beans in Enterprise JavaBeans applications, as described in the Core J2EE patterns as the Session Facade.[^5]
- Web APIs that return a whole resource in one response. Microsoft gives the fix for chatty I/O as "a single GET method that returns the User" in place of a separate method for each property.[^3]
- Microservices that offer "coarse-grained interactions with documents over HTTP or lightweight messaging", as Fowler writes of their advocates.[^4]

## References
