---
tag: NEW-transaction-script
kind: pattern
slug: transaction-script
name: Transaction Script
aliases: [transaction script pattern, procedural domain logic]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A Transaction Script puts the business logic of each request in one procedure that reads and writes the database directly. It is simple, but the logic is copied as the rules grow.
family: architectural
polarity: positive
intent: "Organizes business logic by procedures where each procedure handles a single request from the presentation."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). In 2003 he wrote that he coined the name for a procedural style of handling a request.
see_also: [enterprise-application, domain-model, table-module, service-layer, repository-pattern, separation-of-concerns]
sources:
  - url: https://martinfowler.com/eaaCatalog/transactionScript.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Transaction Script
    locator: pattern summary; the book's Chapter 9
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 2, Organizing Domain Logic; Chapter 9, Transaction Script
  - url: https://martinfowler.com/articles/dblogic.html
    title: Martin Fowler, Domain Logic and SQL, February 2003
    locator: Transaction Script; Looking at Performance
  - url: https://martinfowler.com/bliki/AnemicDomainModel.html
    title: Martin Fowler, Anemic Domain Model, 25 November 2003
    locator: whole article
---

## Intent

Organize the business logic by procedures, where each procedure handles one request from the presentation.[^1]

## Problem

Most business applications can be seen as a series of transactions. One transaction shows some data, and another changes it. Each request between a client and a server has some logic. The logic can be as simple as showing data from the database, or it can have many steps of validation and calculation.[^1] The program needs a place for this logic that is easy to find and easy to write.

## Solution

Write one procedure for each business transaction. The procedure does all the work of the request. It calls the database directly or through a thin database wrapper.[^1] Common subtasks can be broken into subprocedures.[^1]

```text
presentation:   [place order]        [cancel order]
                      |                    |
scripts:        place_order()        cancel_order()
                      \                   /
thin wrapper:    find_customer(), insert_order(), ...
                      |
database:        customers, orders
```

There is no object model of the business. The rows of the database are data, and the scripts hold all the behaviour.

## Example

A shop has customers with a credit limit. Two requests change the data: place an order and cancel an order. Each request is one script. `place_order` checks the credit, reduces it and inserts the order. `cancel_order` marks the order as cancelled and gives the credit back, and it refuses to cancel an order twice. Both scripts use the same small finder and insert functions.

```text
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

The examples for each language show the same program.

## Consequences

Benefits:

- The pattern is simple.[^2] Most developers understand a procedure, and each request has one place to read.
- The scripts work well with a thin layer over the database, such as a gateway for each table or row.[^2] There is no mapping between objects and tables to build.
- Each script is separate from the others, so a new request is a new procedure.
- Fowler describes the trade-off: a domain model has an initial cost in the complexity of database access, and it pays off only if there is a lot of domain logic.[^3]

Liabilities:

- When the business logic grows, the scripts copy the same rules. A change to a rule must then be made in every script that has a copy.[^2]
- A change to the structure of the data can mean a change to the whole script. If many scripts use similar data, each one must be changed.[^3]
- The scripts give no structure to the logic of the business, so complex rules become hard to keep in a good design.[^2]

In this program, two scripts have their own copy of a discount rule. The rule changed, but only one copy was changed, so the price that a customer is quoted is not the price that the customer pays:

```python wrong id=copied-rule
# Two scripts each have their own copy of the discount rule.
def quote_price(quantity, unit_cents):
    total = quantity * unit_cents
    if quantity >= 10:  # 10% off for 10 or more
        total = total * 90 // 100
    return total


def place_order(quantity, unit_cents):
    total = quantity * unit_cents
    if quantity >= 5:  # the rule changed to 5 or more, but only here
        total = total * 90 // 100
    return total


quoted = quote_price(5, 1000)
charged = place_order(5, 1000)
print("quoted:", quoted, "charged:", charged)
print("same price:", quoted == charged)
```

```output for=copied-rule
quoted: 5000 charged: 4500
same price: False
```

## When not to use it

Do not use Transaction Script when the business logic is complex or is likely to grow, such as rules with many conditions that interact. A [[domain-model]] gives each rule one place and handles complex logic better. Do not use it when many requests share most of their logic: the shared part becomes a set of subprocedures with no clear owner, and the result is a domain model without the objects.

## Variants and related patterns

- **Subprocedures for common subtasks.** The scripts call one procedure for a shared step, so a rule lives in one place.[^1] This fixes the wrong program above:

```python run id=shared-subtask
# The common subtask is one procedure that every script calls.
def discounted_total(quantity, unit_cents):
    total = quantity * unit_cents
    if quantity >= 5:  # the rule lives in one place
        total = total * 90 // 100
    return total


def quote_price(quantity, unit_cents):
    return discounted_total(quantity, unit_cents)


def place_order(quantity, unit_cents):
    return discounted_total(quantity, unit_cents)


quoted = quote_price(5, 1000)
charged = place_order(5, 1000)
print("quoted:", quoted, "charged:", charged)
print("same price:", quoted == charged)
```

```output for=shared-subtask
quoted: 4500 charged: 4500
same price: True
```

- **Command objects.** Each script can be a class with one method, with the Command pattern, so that the presentation can treat all the scripts in the same way.[^2]
- **[[service-layer]].** A service layer can hold the logic of each operation as a script, or it can be a thin layer of operations over a domain model.[^2]
- **[[domain-model]].** The main alternative. Fowler notes that an anemic domain model, where service objects hold all the logic and the domain objects hold only data, ends up as transaction scripts.[^4]
- **[[table-module]].** One class for each table. In structure it is between Transaction Script and Domain Model.[^2]

## Known uses

- Fowler used a transaction script in Ruby in *Domain Logic and SQL*. The procedure reads the orders and the line items of a customer and calculates in memory which months qualify for a discount.[^3]
- PoEAA names Transaction Script as one of the three main patterns for domain logic, with Domain Model and Table Module.[^2]
- Many designs that put all the logic in service objects are transaction scripts in practice, as the article on the anemic domain model describes.[^4]

## References
