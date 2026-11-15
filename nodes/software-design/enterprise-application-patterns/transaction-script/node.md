---
tag: AS1XQ1
kind: pattern
slug: transaction-script
name: Transaction script
aliases: [Transaction Script, transaction script pattern, script per request]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A transaction script puts the business logic of one request in one procedure, which reads the database, makes the checks and calculations, and writes the result.
family: architectural
polarity: positive
intent: "Organize business logic by procedures where each procedure handles a single request from the presentation."
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002), as the simplest of the three ways to organize domain logic. The style is older than the name, in COBOL programs and stored procedures.
see_also: [domain-model, table-module, enterprise-application, table-data-gateway, row-data-gateway, dont-repeat-yourself, command-pattern, keep-it-simple]
sources:
  - url: https://martinfowler.com/eaaCatalog/transactionScript.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Transaction Script
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 9, Domain Logic Patterns, Transaction Script
  - url: https://www.postgresql.org/docs/17/plpgsql-overview.html
    title: PostgreSQL 17 documentation, Overview of PL/pgSQL
    locator: Advantages of Using PL/pgSQL
  - url: https://martinfowler.com/bliki/AnemicDomainModel.html
    title: Martin Fowler, Anemic Domain Model, 25 November 2003
    locator: the whole post
  - url: https://docs.djangoproject.com/en/5.2/topics/http/views/
    title: Django 5.2 documentation, Writing views
    locator: introduction
---

## Intent

A transaction script "Organizes business logic by procedures where each procedure handles a single request from the presentation."[^1] There is one script for each thing that a user can do, such as place an order, cancel a booking or show a report. Fowler gives the pattern as the simplest of three ways to organize the domain logic of an [[enterprise-application]]. The other two are [[domain-model]] and [[table-module]].[^1][^2]

## Problem

An application gets requests from its users. Fowler: "Most business applications can be thought of as a series of transactions."[^1] Each request needs some logic, and the amount varies: "In some cases this can be as simple as displaying information in the database. In others it may involve many steps of validations and calculations."[^1] The question is where to put that logic.

For a small application with simple rules, a design with many classes costs more than it gives. The developers want code that is easy to read from top to bottom, that maps directly to the requests, and that needs no mapping layer between objects and tables.

## Solution

"A Transaction Script organizes all this logic primarily as a single procedure, making calls directly to the database or through a thin database wrapper. Each transaction will have its own Transaction Script, although common subtasks can be broken into subprocedures."[^1]

The parts are:

- **The script.** One procedure for one request. It reads the data, checks it, calculates, and writes the result.
- **The database transaction.** The script usually runs in one database transaction, so the request succeeds or fails as a whole.
- **Shared subprocedures.** Helpers for the steps that many scripts need, such as a format for money or a check of a customer.
- **A thin data wrapper.** Optional. A [[table-data-gateway]] or a [[row-data-gateway]] can hold the SQL, so the scripts do not repeat it.

```text
  presentation      place order      show total      cancel order
                         |                |                |
  scripts          place_order()    order_total()   cancel_order()
                         |                |                |
  data access       SQL, or a thin gateway over the tables
```

The following program has one script. It transfers money between two accounts in one transaction and refuses a transfer that the balance does not cover.

```python run id=one-script
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table account (id integer primary key, balance integer);
insert into account values (1, 100), (2, 0);
""")


# One procedure for one request: it checks, calculates and writes the database.
def transfer(source, target, amount):
    with db:  # the script runs in one database transaction
        (balance,) = db.execute("select balance from account where id = ?", (source,)).fetchone()
        if balance < amount:
            return "refused: balance too low"
        db.execute("update account set balance = balance - ? where id = ?", (amount, source))
        db.execute("update account set balance = balance + ? where id = ?", (amount, target))
    return "done"


print(transfer(1, 2, 30), transfer(1, 2, 90))
print(db.execute("select balance from account order by id").fetchall())
```

```output
done refused: balance too low
[(70,), (30,)]
```

## Example

Each example file is a small shop with two scripts. `place_order` checks the customer, the products and the quantities, and then writes the order and its lines. `order_total` adds the lines and applies the discount rule: a gold customer gets 10 percent off an order of 100.00 or more. The program places two orders, shows their totals, and then tries two orders that are not correct. The last line shows that the failed scripts wrote nothing. The Python file uses SQLite and a database transaction; the other files keep the tables in memory.

## Consequences

**Benefits.**

- **Simple.** The pattern needs no knowledge of object design. A reader can follow a request from the start of its script to the end.
- **A direct map to the requests.** Each script is one request, so it is easy to find the code for a request, and the transaction has clear limits.
- **Little mapping.** The scripts work with rows. There is no layer that maps objects to tables, so the [[object-relational-impedance-mismatch]] does not occur.
- **It works with the tools.** Stored procedures are transaction scripts in the database. The PostgreSQL documentation says that PL/pgSQL can "group a block of computation and a series of queries inside the database server", and so saves "client/server communication overhead".[^3]

**Liabilities.**

**Rules are copied, and the copies drift apart.** When two scripts need the same rule, the easy step is to copy it. Later, a change goes into one copy only. In the following program, the discount rule changed in the script for the order screen, but the script that prints the invoice still has the old rule, so the two give different totals for one order.

```python wrong id=copied-rule
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table orders (id integer primary key, tier text, subtotal integer);
insert into orders values (1, 'gold', 80);
""")


def order_total(order_id):
    tier, subtotal = db.execute("select tier, subtotal from orders where id = ?", (order_id,)).fetchone()
    # The rule changed: gold customers now get 10% off from 50, not from 100.
    if tier == "gold" and subtotal >= 50:
        return subtotal - subtotal // 10
    return subtotal


def print_invoice(order_id):
    tier, subtotal = db.execute("select tier, subtotal from orders where id = ?", (order_id,)).fetchone()
    # Wrong: a copy of the old rule, which nobody remembered to change.
    total = subtotal - subtotal // 10 if tier == "gold" and subtotal >= 100 else subtotal
    return f"invoice 1: pay {total}"


print("order screen: total", order_total(1))
print(print_invoice(1))
```

```output
order screen: total 72
invoice 1: pay 80
```

Move each shared rule into one subprocedure, as the [[dont-repeat-yourself]] principle asks. When there are many rules of this kind, move to a [[domain-model]].

**It does not scale with the complexity of the logic.** The scripts are simple while the rules are simple. When the rules interact, each script gets long, and the logic of one concept, such as a customer type, is spread across many scripts.

## When not to use it

**When the domain logic is complex.** Rules that depend on many cases are easier to keep in a [[domain-model]], where polymorphism replaces the conditions. Fowler writes that a design that puts all the behavior into services ends up with transaction scripts, and so loses the advantages that a domain model can bring.[^4]

**When the same data has many rules.** If many scripts work on the same tables with related rules, a [[table-module]] keeps these rules in one class for each table.

## Variants and related patterns

**Script per command.** Each script is a class with one method, which makes it a [[command-pattern]]. The presentation can then queue, log or undo the requests.

**Several scripts in one class.** Related scripts, such as all the scripts for orders, are methods of one class. This is a common form in Java and C#.

**Stored procedures.** The script runs in the database, in a language such as PL/pgSQL. This removes "Extra round trips between client and server",[^3] but it ties the logic to one database product.

**Related patterns.** [[domain-model]] and [[table-module]] are the other two ways to organize domain logic. A [[service-layer]] is often a set of transaction scripts over a domain model. An anemic domain model, with data and no behavior, is in fact a transaction script design.[^4]

## Known uses

- Django view functions. "A view function, or view for short, is a Python function that takes a web request and returns a web response."[^5] When a view does the checks and the database calls itself, it is a transaction script for that request.
- Stored procedures in PL/pgSQL, T-SQL and PL/SQL, which put the logic of one business operation in one procedure in the database.[^3]
- Batch jobs and CGI programs, where each program handles one kind of request from start to end.

## References
