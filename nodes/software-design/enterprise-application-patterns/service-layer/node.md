---
tag: Z57N8A
kind: pattern
slug: service-layer
name: Service layer
aliases: [Service Layer, application service layer, application layer, application services]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A service layer is the set of operations that an application offers to all its clients. Each operation controls its transaction and coordinates the domain objects.
family: architectural
polarity: positive
intent: "Define an application's boundary with a layer of services that establishes a set of available operations and coordinates the application's response in each operation."
history:
  year: 2002
  people: [Randy Stafford, Martin Fowler]
  note: Randy Stafford wrote the pattern for Fowler's Patterns of Enterprise Application Architecture (2002). Eric Evans describes the same layer as the application layer in Domain-Driven Design.
see_also: [domain-model, transaction-script, remote-facade, data-transfer-object, facade-pattern, unit-of-work, separation-of-concerns, enterprise-application]
sources:
  - url: https://martinfowler.com/eaaCatalog/serviceLayer.html
    title: Randy Stafford, Catalog of Patterns of Enterprise Application Architecture, Service Layer
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 9, Domain Logic Patterns, Service Layer
  - url: https://docs.spring.io/spring-framework/reference/data-access/transaction/declarative/annotations.html
    title: Spring Framework reference, Using @Transactional
    locator: introduction and @Transactional Settings
  - url: https://martinfowler.com/bliki/AnemicDomainModel.html
    title: Martin Fowler, Anemic Domain Model, 25 November 2003
    locator: the part on the Service Layer and the application layer
  - url: https://docs.spring.io/spring-framework/docs/current/javadoc-api/org/springframework/stereotype/Service.html
    title: Spring Framework API, Annotation Interface Service
    locator: description
---

## Intent

A service layer "Defines an application's boundary with a layer of services that establishes a set of available operations and coordinates the application's response in each operation."[^1] The clients of the application, such as the web pages, the batch loaders and the other systems, call these operations and nothing below them. Randy Stafford wrote the pattern for the book.[^1][^2]

## Problem

"Enterprise applications typically require different kinds of interfaces to the data they store and the logic they implement: data loaders, user interfaces, integration gateways, and others."[^1] These clients do different things, but they need the same operations: "Despite their different purposes, these interfaces often need common interactions with the application to access and manipulate its data and invoke its business logic."[^1]

One operation can do much work. It can start a transaction, change several domain objects, write them, and then send an email or a message. If each client does these steps itself, the steps are copied: "Encoding the logic of the interactions separately in each interface causes a lot of duplication."[^1] Later, a change goes into some of the copies only, and the clients act differently for the same operation.

## Solution

"A Service Layer defines an application's boundary and its set of available operations from the perspective of interfacing client layers."[^1] Each operation is one method of the layer. The method controls the transaction and coordinates the responses to the operation, such as the email after an order. The business rules stay in the [[domain-model]] or in a [[transaction-script]] below the layer.

The parts are:

- **The service.** A class or a module with one method for each operation of the application, such as `place_order` or `cancel_booking`.
- **The transaction control.** Each operation starts a transaction, and commits it or rolls it back. A client never sees a half-done operation.
- **The coordination.** The operation calls the domain objects for the rules, the data source for the data, and other resources such as a mailer or a message queue.
- **The clients.** The web handlers, the loaders and the integration gateways. They change their input into a call of an operation, and change the result into their output.

```text
  clients        web handler     import loader     integration gateway
                      \               |                 /
  service layer        place_order()   cancel_order()   ...
                       (transaction, coordination of the responses)
                            |                 |
  domain layer         Order, Customer, Product (the business rules)
                            |
  data source          database, mailer, message queue
```

In the following program, `transfer` is the operation. Two clients call it, and so they get the same checks and the same log.

```python run id=one-operation
class Account:
    def __init__(self, balance):
        self.balance = balance


accounts = {1: Account(100), 2: Account(0)}
log = []


# The service layer: one operation of the application that the clients call.
def transfer(source, target, amount):
    a, b = accounts[source], accounts[target]
    if a.balance < amount:
        return "refused"
    a.balance -= amount
    b.balance += amount
    log.append(f"transfer {amount} from {source} to {target}")
    return "done"


print("web client:", transfer(1, 2, 30))
print("batch client:", transfer(1, 2, 90))
print("balances:", accounts[1].balance, accounts[2].balance, "log:", log)
```

```output
web client: done
batch client: refused
balances: 70 30 log: ['transfer 30 from 1 to 2']
```

## Example

Each example file is a small shop. The domain class `Order` knows its lines and its discount rule: a gold customer gets 10 percent off an order of 100.00 or more. `OrderService.place_order` is the operation. It starts a transaction, finds the customer and the products, asks the order to add the lines, and commits. It sends the email only after the commit, so a failed order gets no email. Two clients use the service: a web handler and a loader that imports orders from a file. The second import line names a product that does not exist, so the service rolls back the order, and the last lines show that the store has 2 orders and that 2 emails went out.

## Consequences

**Benefits.**

- **One place for each operation.** All the clients use the same code for an operation, so a change to the transaction or to the responses goes into one place.
- **A clear boundary.** The layer is the list of things that the application can do. This list is the place to check the permissions and the input, and a good start for a [[remote-facade]].
- **Clear transactions.** Each operation is one transaction, so the limits of the transactions are easy to see. In Spring, the `@Transactional` annotation "specifies that an interface, class, or method must have transactional semantics",[^3] and it is often put on the service classes.
- **Clients that are easy to change.** A new client, such as a command line tool, needs only to call the operations.

**Liabilities.**

**Without the layer, the clients drift apart.** In the following program, each client places the order itself. The web client sends the email, but the import client was written later, and its author did not know about the email.

```python wrong id=logic-in-each-client
orders, emails = [], []


# Wrong: no service layer. Each client places the order and sends the email itself.
def web_place_order(customer, total):
    orders.append((customer, total))
    emails.append(customer)


def import_place_order(customer, total):
    orders.append((customer, total))
    # The import client was written later, and its author did not know about the email.


web_place_order("ann@example.com", 112)
import_place_order("bob@example.com", 40)
print("orders:", len(orders), "emails:", len(emails))
print("no email for:", [c for c, _ in orders if c not in emails])
```

```output
orders: 2 emails: 1
no email for: ['bob@example.com']
```

Move the steps into one operation of the service layer, and let the two clients call it.

**A layer that is too thick.** The layer can take the business rules from the domain objects. Fowler writes that "the more behavior you find in the services, the more likely you are to be robbing yourself of the benefits of a domain model."[^4] Keep the rules in the domain objects, and keep the service for the transaction and the coordination.

**One more layer.** The layer adds classes and calls. An application with one client and simple operations may not need it.

## When not to use it

**When the application has one client.** If only one user interface uses the logic, and its operations do not need transactions across many resources, the extra layer gives little. The calls can go from the client to the domain objects.

**When the operations are only reads and writes of one table.** A layer that only passes each call to a [[table-data-gateway]] or a repository adds code and gives nothing.

## Variants and related patterns

**Domain facade.** The service is a thin set of methods that pass each call to the domain objects. The rules stay in the [[domain-model]].[^2]

**Operation script.** The service holds the logic of each operation, as a set of [[transaction-script]] procedures, and calls the domain objects only for the simple rules.[^2]

**The application layer.** Eric Evans names the same layer the application layer. In the words of Evans, quoted by Fowler, "This layer is kept thin. It does not contain business rules or knowledge, but only coordinates tasks and delegates work to collaborations of domain objects in the next layer down."[^4]

**Related patterns.** A service layer is a [[facade-pattern]] for the whole application. A [[remote-facade]] puts a coarse-grained remote interface on the layer, and a [[data-transfer-object]] carries the data of each call. An operation often uses a [[unit-of-work]] to write all its changes in one transaction. The layer is a way to keep [[separation-of-concerns]] between the clients and the domain logic.

## Known uses

- Spring applications. The `@Service` annotation marks a service class, and the Javadoc says that the annotation "is a general-purpose stereotype and individual teams may narrow their semantics and use as appropriate."[^5] The service classes often have `@Transactional`, as in the example class `DefaultFooService` of the Spring reference.[^3]
- Applications that follow Domain-Driven Design, where the application layer coordinates the domain objects for each task.[^4]

## References
