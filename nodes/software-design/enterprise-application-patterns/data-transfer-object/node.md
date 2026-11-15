---
tag: HNJB56
kind: pattern
slug: data-transfer-object
name: Data transfer object
aliases: [Data Transfer Object, DTO, Transfer Object, transfer object pattern]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A data transfer object holds all the data for one remote call, so the call can carry it in one message. An assembler copies the data between it and the domain objects.
family: architectural
polarity: positive
intent: "An object that carries data between processes in order to reduce the number of method calls."
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002), with Remote Facade, in the chapter on distribution patterns. The Core J2EE patterns name it the Transfer Object.
see_also: [remote-facade, service-layer, object-serialization, domain-model, data-mapper, enterprise-application, separation-of-concerns]
sources:
  - url: https://martinfowler.com/eaaCatalog/dataTransferObject.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Data Transfer Object
    locator: pattern summary and the note of 2013-08-11
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 15, Distribution Patterns, Data Transfer Object
  - url: https://www.oracle.com/java/technologies/transfer-object.html
    title: Core J2EE Patterns, Transfer Object
    locator: Problem and Solution
  - url: https://learn.microsoft.com/en-us/aspnet/web-api/overview/data/using-web-api-with-entity-framework/part-5
    title: Microsoft, ASP.NET Web API 2 with Entity Framework, Create Data Transfer Objects (DTOs)
    locator: the whole page
  - url: https://martinfowler.com/bliki/LocalDTO.html
    title: Martin Fowler, Local DTO, 21 October 2004
    locator: the whole post
---

## Intent

A data transfer object (DTO) is "An object that carries data between processes in order to reduce the number of method calls."[^1] It holds all the data that one remote call sends or returns, and it has no domain behavior. It is the usual partner of a [[remote-facade]].[^1][^2]

## Problem

A remote interface must be coarse-grained, because each remote call is expensive. So each call must carry more data. Fowler: "One way to do this is to use lots of parameters. However, this is often awkward to program".[^1] A method in Java can also return only one value.[^1]

The server cannot simply send its domain objects. They are often a graph of objects with references, which the client cannot use from another process. They hold data that must stay on the server, such as a password hash or a cost price. And if the client depends on the form of the domain objects, each change to them breaks the client.

## Solution

"The solution is to create a Data Transfer Object that can hold all the data for the call. It needs to be serializable to go across the connection. Usually an assembler is used on the server side to transfer data between the DTO and any domain objects."[^1]

The parts are:

- **The DTO.** A plain object with fields for the data of one call. It has no behavior, except perhaps the code that serializes it. Its shape follows the needs of the call, not the shape of the domain objects.
- **The serialization.** The DTO knows how to change itself into text or bytes for the wire and back. Fowler notes that this is a second benefit: "another advantage is to encapsulate the serialization mechanism for transferring data over the wire."[^1]
- **The assembler.** On the server, it reads the domain objects to fill a DTO, and it uses a DTO from the client to update the domain objects. The domain objects and the DTOs do not know each other.

```text
  server                                           client
  domain objects  --assembler-->  DTO  --wire-->  DTO (a copy)
  (Customer,                      (only the data   reads and changes
   Order, Line)   <--assembler--  for this call) <--wire--  its copy
```

In the following program, the product on the server has a cost price. The assembler copies only the name and the price into the DTO, and the DTO goes over the wire as JSON.

```python run id=one-dto
import json
from dataclasses import asdict, dataclass


class Product:  # a domain object, with data that stays on the server
    def __init__(self, name, price, cost_price):
        self.name, self.price, self.cost_price = name, price, cost_price


@dataclass
class ProductDTO:  # only the data for this call, in a form that can be serialized
    name: str
    price: int


def assemble(product):  # the assembler, on the server
    return ProductDTO(product.name, product.price)


wire = json.dumps(asdict(assemble(Product("lamp", 2500, 1400))))
received = ProductDTO(**json.loads(wire))
print(received)
```

```output
ProductDTO(name='lamp', price=2500)
```

## Example

Each example file has a customer on the server, with an email, a password hash and two orders, and each line of an order has a cost price. `CustomerDTO` holds only the name, the email, and the number and the total of each order. The assembler fills the DTO from the customer, and later updates the customer from a DTO that the client sends back; it copies back only the name and the email. The program shows that the password hash is not on the wire, that a change on the client changes only the copy, and that the server changes only when the client sends the DTO back.

## Consequences

**Benefits.**

- **One call for all the data.** The Core J2EE patterns: "A single method call is used to send and retrieve the Transfer Object." After that, "all calls to the Transfer Object instance are local calls instead of remote method invocations."[^3]
- **Control of what leaves the server.** The DTO has only the fields that the client must see. Microsoft gives as a reason to use DTOs: "Hide particular properties that clients are not supposed to view."[^4]
- **A stable contract.** The DTO is the shape of the data on the wire. The domain objects can change while the DTO stays the same, and the assembler takes up the difference.
- **One place for the serialization.** The DTOs "keep this logic out of the rest of the code and also provide a clear point to change serialization should you wish."[^1]

**Liabilities.**

**Without a DTO, the domain object leaks.** In the following program, the server serializes the customer object itself. All its fields go over the wire, the password hash too.

```python wrong id=send-the-domain-object
import json


class Customer:
    def __init__(self, name, email, password_hash):
        self.name, self.email, self.password_hash = name, email, password_hash


ann = Customer("Ann", "ann@example.com", "pbkdf2:9f86d081")

# Wrong: the server sends the domain object itself, with all its fields.
wire = json.dumps(vars(ann))
received = json.loads(wire)
print("fields sent:", ", ".join(received))
print("password hash on the wire:", received["password_hash"])
```

```output
fields sent: name, email, password_hash
password hash on the wire: pbkdf2:9f86d081
```

Put only the fields that the client needs into a DTO, and let an assembler fill it.

**The mapping is work.** Each DTO needs an assembler, and the code that copies the data grows with each screen. Randy Stafford, quoted by Fowler, says of this cost: "It's significant, and it's painful - perhaps second only to the cost and pain of object-relational mapping".[^5] Libraries can do the copy: Microsoft names "a library like AutoMapper that handles the conversion automatically."[^4]

**Two models to keep in step.** A change to the data that a screen shows needs a change to the DTO, the assembler and the client.

## When not to use it

**Inside one process.** "DTOs are called Data Transfer Objects because their whole purpose is to shift data in expensive remote calls."[^5] In a local context, Fowler writes, "they are actually harmful both because a coarse-grained API is more difficult to use and because you have to do all the work moving data from your domain or data source layer into the DTOs."[^5]

**Only to guard the clients of a local service layer.** Some people use DTOs in a local [[service-layer]] API so that its clients do not depend on the [[domain-model]]. Fowler: "While that may be handy, I don't think it's worth the cost of all of that data mapping."[^5]

## Variants and related patterns

**Presentation DTO.** Fowler names one local use that is worth the cost: "One case where it is useful to use something like a DTO is when you have a significant mismatch between the model in your presentation layer and the underlying domain model."[^5]

**Messages between isolated parts.** When an application has areas with one thread each, the areas can "use a message queue with DTOs as messages to transfer the information."[^5]

**Transfer Object.** The Core J2EE patterns give the same pattern for Enterprise JavaBeans: "Use a Transfer Object to encapsulate the business data."[^3] Fowler notes that the Java community first used the term value object for this pattern, which caused confusion with a [[value-object]], and later changed to the name Transfer Object.[^1]

**Related patterns.** A [[remote-facade]] takes and returns DTOs. The assembler is a kind of mapper, like the [[data-mapper]] between objects and tables. The DTO is a unit of [[object-serialization]].

## Known uses

- Enterprise JavaBeans applications, where an enterprise bean "can construct the Transfer Object, populate it with its attribute values, and pass it by value to the client."[^3]
- ASP.NET Web API, where the tutorial adds `BookDto` and `BookDetailDto` classes and changes the controller to return them, because "A DTO is an object that defines how the data will be sent over the network."[^4]
- The request and response types of HTTP and RPC APIs in general, which are DTOs for the calls of a [[remote-facade]].

## References
