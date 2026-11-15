---
tag: GK1510
kind: pattern
slug: domain-model
name: Domain model
aliases: [Domain Model, domain model pattern, rich domain model, object model of the domain]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A domain model is a set of objects for the concepts of the business, where each object holds its data and the rules that use the data.
family: architectural
polarity: positive
intent: "An object model of the domain that incorporates both behavior and data."
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). The book Domain-Driven Design by Eric Evans is built on the same idea.
see_also: [transaction-script, table-module, enterprise-application, data-mapper, active-record, replace-conditional-with-polymorphism, strategy-pattern, unit-of-work]
sources:
  - url: https://martinfowler.com/eaaCatalog/domainModel.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Domain Model
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 9, Domain Logic Patterns, Domain Model
  - url: https://docs.djangoproject.com/en/5.2/topics/db/models/
    title: Django 5.2 documentation, Models
    locator: introduction; Model methods
  - url: https://martinfowler.com/bliki/AnemicDomainModel.html
    title: Martin Fowler, Anemic Domain Model, 25 November 2003
    locator: the whole post
  - url: https://martinfowler.com/eaaCatalog/tableModule.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Table Module
    locator: pattern summary
  - url: https://guides.rubyonrails.org/active_record_basics.html
    title: Ruby on Rails Guides, Active Record Basics
    locator: 1. What is Active Record?
  - url: https://jakarta.ee/specifications/persistence/3.2/jakarta-persistence-spec-3.2
    title: Jakarta Persistence 3.2 specification
    locator: 2. Entities
---

## Intent

A domain model is "An object model of the domain that incorporates both behavior and data."[^1] There is a class for each concept of the business, such as customer, order and product. Each object holds its data and the rules that use the data. The rest of the application asks the objects to do the work. It is one of the three patterns for domain logic in Fowler's book, with [[transaction-script]] and [[table-module]].[^2]

## Problem

Fowler: "At its worst business logic can be very complex. Rules and logic describe many different cases and slants of behavior, and it's this complexity that objects were designed to work with."[^1]

When the rules are in procedures, as in a [[transaction-script]] design, each procedure has conditions on the kind of customer, the state of the order and the type of product. A new kind of customer means a change in each procedure that has such a condition. The rules about one concept are spread across many procedures, and a rule that two procedures need is often copied.

## Solution

"A Domain Model creates a web of interconnected objects, where each object represents some meaningful individual, whether as large as a corporation or as small as a single line on an order form."[^1]

The parts are:

- **Domain objects.** A class for each concept. The data of an object is private, and its methods are the operations of the business, such as add a line, place an order or calculate a total.
- **Associations.** References between the objects, such as from an order to its customer and its lines.
- **Polymorphism.** Each variant of a concept is a subclass or an implementation of an interface. A gold customer and a staff customer each know their own discount, so the order does not test the kind of customer.
- **Invariants.** The objects check their own rules. A placed order refuses a new line, whichever code asks.
- **A mapping to the database.** The objects do not fit the tables directly, so a domain model needs a mapping. For a simple model, each object can save itself, as in [[active-record]]. For a complex model, a [[data-mapper]] keeps the objects independent of the database.

```text
  Order ----------------> Customer (interface: discount)
    | add(), place(),         ^          ^          ^
    | total()                 |          |          |
    v                     Regular      Gold       Staff
  OrderLine ---> Product
    amount()
```

The following program has the smallest domain object. The account holds its balance and the rule for a withdrawal.

```python run id=data-and-rule
class Account:
    # The data and the rules that use it are in one object.
    def __init__(self, balance, overdraft=0):
        self.balance = balance
        self.overdraft = overdraft

    def withdraw(self, amount):
        if amount > self.balance + self.overdraft:
            raise ValueError("refused: over the limit")
        self.balance -= amount


acct = Account(100, overdraft=50)
acct.withdraw(120)
print("balance", acct.balance)
try:
    acct.withdraw(40)
except ValueError as e:
    print(e)
```

```output
balance -20
refused: over the limit
```

## Example

Each example file is a small shop. `Customer` has three kinds: a regular customer has no discount, a gold customer gets 10 percent off an order of 100.00 or more, and staff get 20 percent off each order. `Order` keeps its lines, calculates its subtotal and total, and asks its customer for the discount. It refuses a new line after it is placed, and it refuses to be placed with no lines. `OrderLine` refuses a quantity that is not positive. The program builds three orders, prints their totals, and then breaks each rule once.

## Consequences

**Benefits.**

- **Complex logic stays organized.** Each rule is in the object that it is about. A new kind of customer is a new class, and the order does not change.
- **Rules are in one place.** The Django documentation recommends methods on the model for the same reason: "This is a valuable technique for keeping business logic in one place – the model."[^3]
- **The objects protect their rules.** In the examples, no caller can add a line to a placed order, because only the order can change its lines.
- **The language of the business.** The names of the classes and methods are the words of the business, so the code and the experts use the same terms.

**Liabilities.**

**The mapping to the database costs effort.** Fowler: "The primary cost is the awkwardness of mapping to a database, which typically results in a whole layer of O/R mapping."[^4] See [[object-relational-impedance-mismatch]].

**An anemic model has the costs and none of the benefits.** In an anemic domain model, the objects have data and no behavior, and "a set of service objects which capture all the domain logic" changes them.[^4] Fowler: "In essence the problem with anemic domain models is that they incur all of the costs of a domain model, without yielding any of the benefits."[^4] In the following program, the order is a data class. A service calculates the total, and nothing stops other code from changing a placed order after the invoice.

```python wrong id=anemic-order
from dataclasses import dataclass, field


# Wrong: an anemic order, with data and no behavior. The rules are in a service.
@dataclass
class Order:
    tier: str
    lines: list = field(default_factory=list)
    placed: bool = False


def total(order):
    subtotal = sum(order.lines)
    return subtotal - subtotal // 10 if order.tier == "gold" and subtotal >= 100 else subtotal


order = Order("gold", [50, 75])
order.placed = True
print("invoiced:", total(order))
# Nothing stops another piece of code from changing a placed order.
order.lines.append(25)
print("total after a late change:", total(order), "placed:", order.placed)
```

```output
invoiced: 113
total after a late change: 135 placed: True
```

Put the rules in the objects, as in the examples, and keep the services thin.

**It needs skill in object design.** A team that does not know object design can make a domain model that is harder to read than procedures.

## When not to use it

**When the logic is simple.** For a few rules, a [[transaction-script]] costs less: there is no mapping layer and no class design. Fowler writes that domain models "aren't always the best tool."[^4]

**When the logic fits the tables.** A domain model needs code to move the data between the objects and the rows. Fowler calls this "considerable programmatic gymnastics".[^5] If the logic works well on the rows of each table, a [[table-module]] avoids this work.

## Variants and related patterns

**Simple domain model.** About one class for each table, where each object saves itself with [[active-record]].

**Rich domain model.** Inheritance, strategies and many small objects, which do not match the tables. It needs a [[data-mapper]], usually with a [[unit-of-work]] and an [[identity-map]].

**Domain-driven design.** Eric Evans's method adds rules for how to find and group the objects, with [[entity|entities]], [[value-object|value objects]] and aggregates. It keeps a thin application layer over the model, as Fowler's post describes.[^4]

**With a service layer.** A [[service-layer]] on top of the model gives the application its operations and controls the transactions. The service layer is thin, and the rules stay in the model.[^4]

**Related patterns.** [[transaction-script]] and [[table-module]] are the other two ways to organize domain logic. [[replace-conditional-with-polymorphism]] is the refactoring that moves a design from conditions to the subclasses of a domain model. A [[strategy-pattern]] lets a domain object change a rule at run time.

## Known uses

- Django models. "A model is the single, definitive source of information about your data. It contains the essential fields and behaviors of the data you’re storing."[^3] Model methods "act on a particular model instance".[^3]
- Ruby on Rails models. "Active Record is part of the M in MVC - the model - which is the layer of the system responsible for representing data and business logic."[^6] Each model also saves itself, so it is an [[active-record]].
- Jakarta Persistence entities. "An entity is a lightweight persistent domain object."[^7] JPA maps a model of entity classes to the tables, in the style of a [[data-mapper]].

## References
