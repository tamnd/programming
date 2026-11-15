---
tag: 6FW1ZD
kind: pattern
slug: database-access-layer
name: Database access layer
aliases: [data access layer, DAL, relational database access layer, persistence layer]
area: software-design/data-access-patterns
level: advanced
summary: >-
  A database access layer puts all database access of an application in one layer, with a logical part that the application uses and a physical part that knows the tables.
family: architectural
polarity: positive
intent: "Separate the application from the database with a layer that holds all database access."
also_known_as: [data access layer]
history:
  year: 1996
  people: [Wolfgang Keller, Jens Coldewey]
  note: Keller and Coldewey presented the Relational Database Access Layer pattern language at PLoP 1996. Buschmann, Henney and Schmidt include Database Access Layer in the Database Access chapter of POSA4 (2007).
see_also: [data-access-object, query-object, table-data-gateway, data-mapper, identity-map, unit-of-work, separation-of-concerns, object-relational-impedance-mismatch]
sources:
  - url: http://www.objectarchitects.de/ObjectArchitects/papers/Published/ZippedPapers/plop_relzs05.pdf
    title: Wolfgang Keller and Jens Coldewey, Relational Database Access Layers, A Pattern Language, The Key Patterns, sd&m, 1996
    locator: Pattern Relational Database Access Layer; Context; Problem; Forces; Solution; Participants; Implementation; Consequences; Variants; Known Uses
  - book: isbn:9780470059029
    title: Frank Buschmann, Kevlin Henney and Douglas C. Schmidt, Pattern-Oriented Software Architecture, Volume 4, A Pattern Language for Distributed Computing, Wiley, 2007
    locator: Chapter 21, Database Access, Database Access Layer, page 538
  - url: https://docs.sqlalchemy.org/en/20/dialects/index.html
    title: SQLAlchemy 2.0 documentation, Dialects
    locator: introduction
  - url: https://docs.djangoproject.com/en/5.2/ref/databases/
    title: Django 5.2 documentation, Databases
    locator: introduction
---

## Intent

A database access layer holds all the code of an application that talks to the database. The rest of the application, which Keller and Coldewey call the application kernel, uses the interface of the layer and has no SQL and no table names. Their pattern language starts from one question: "How do you access the relational database?"[^1] POSA4 includes the pattern as the first pattern of its chapter on database access, and the other patterns of the chapter refine it.[^2]

## Problem

Database code and business code are both hard, and they are hard in different ways. Keller and Coldewey put the first force like this: "Database programming is complex and so is application logic. Both call for clever solutions. Mixing them will add up to more than just the combined complexity."[^1]

The other forces pull against each other. The database must be tuned, and tuning changes the tables, the indexes and the access paths, many times in the life of a system. The application kernel must not change each time. Old data in legacy systems often has a structure that does not fit the new design, and a new system must still read it. A layer that hides the database costs classes and design work, and it must still be powerful enough for the queries that the application needs.[^1]

## Solution

"Use a layered architecture consisting of two layers."[^1] The **logical access layer** gives the application kernel a stable interface. The **physical access layer** knows the database: the tables, the SQL and the calls of the database interface. When the database is tuned, the physical layer changes, and the logical layer and the kernel do not.

The participants in the Keller and Coldewey form are:[^1]

- **Transaction.** The kernel starts, commits and aborts work through it.
- **View factory and view cache.** The kernel gets each logical view, such as an order with its lines, by a key. The cache keeps the views of the transaction, so a second read of the same view does not go to the database, and the commit writes the changed views.
- **Physical views.** "wraps one physical database table".[^1] A physical view can also hide a tuning change, such as a table whose columns were copied into another table, and it can be generated from the metadata of the database.
- **Database.** It wraps the connection and the administrative calls of the database system.
- **Query broker.** An optional part that connects the logical views to the physical views. Without it, the connection is written in the code.

The physical views also translate the error codes of the database into errors of the application.[^1]

```text
  application kernel          invoice(10)        no SQL, no tables
  ----------------------------------------------------------------
  logical access layer        Transaction, ViewFactory, ViewCache
                              get_order(10) -> Order
  ----------------------------------------------------------------
  physical access layer       NormalizedOrderView | DenormalizedOrderView
                              Database (connection, statements)
  ----------------------------------------------------------------
  database API                SQL on customer, orders, order_line
```

The following program has the three parts in their smallest form. Only the physical part, the set-up of the tables and `read_order`, has SQL in it.

```python run id=three-parts
import sqlite3

# Physical access layer: the only code that knows the tables and the SQL.
db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text);
create table orders (id integer primary key, customer_id integer);
insert into customer values (1, 'Acme Ltd');
insert into orders values (10, 1);
""")


def read_order(order_id):
    sql = "select c.name from orders o join customer c on c.id = o.customer_id where o.id = ?"
    return db.execute(sql, (order_id,)).fetchone()


# Logical access layer: the interface of the application kernel.
def customer_of_order(order_id):
    row = read_order(order_id)
    return row[0] if row else None


# Application kernel: no SQL, no table names.
print("order 10 is for", customer_of_order(10))
```

```output
order 10 is for Acme Ltd
```

## Example

Each example file has an application kernel, `invoice`, that prints the customer and the lines of an order. It uses only the logical layer, `AccessLayer`, which has the start and the commit of a transaction, a cache of the orders read, and the error `AccessError`. Under it are two physical views of the same orders. `NormalizedOrderView` reads three tables: `customer`, `orders` and `order_line`. `DenormalizedOrderView` reads two, because a tuning step copied the customer name into `orders`. The program runs the same kernel on each layout. It prints the invoice twice, and the count of physical reads shows that the second came from the cache. It changes a quantity, commits, reads the order in a new transaction, and asks for an order that does not exist. The output is the same for the two layouts except for the list of tables. The Python file runs SQL in SQLite; the other files keep the tables in memory.

## Consequences

**Benefits.**

- **Separation of concerns.** "The access layer forms a well-encapsulated subsystem for transactions, database access and caching."[^1]
- **The database can be tuned.** Keller and Coldewey note that the application code stays the same while the physical database is changed for tuning.[^1] In the examples, the change to the tables changes one class.
- **Legacy data can be moved in steps.** The layer is put in first, and then the database and the application are rewritten in separate projects.[^1]
- **Fewer reads.** The cache of the transaction reads each view once.

**Liabilities.**

**Writes that go around the layer make the cache wrong.** Keller and Coldewey warn that mass updates, of the form "update .. where", are "hard to integrate" with the view cache, and that stored procedures and triggers that change data "may cause cache consistency problems".[^1] In the following program, a mass update goes to the database directly, and the layer still gives the old value from its cache.

```python wrong id=update-around-the-layer
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table order_line (order_id integer, item text, qty integer);
insert into order_line values (10, 'bolt', 2);
""")

cache = {}


def get_qty(order_id, item):
    # The logical access layer keeps each line that it read in a cache.
    if (order_id, item) not in cache:
        sql = "select qty from order_line where order_id = ? and item = ?"
        cache[(order_id, item)] = db.execute(sql, (order_id, item)).fetchone()[0]
    return cache[(order_id, item)]


print("before:", get_qty(10, "bolt"))
# Wrong: a mass update goes to the database directly, not through the layer.
db.execute("update order_line set qty = qty + 1 where item = 'bolt'")
print("in the database:", db.execute("select qty from order_line").fetchone()[0])
print("from the layer:", get_qty(10, "bolt"))
```

```output
before: 2
in the database: 3
from the layer: 2
```

**Effort.** Keller and Coldewey give a range of "0,5 to 35 person years" to build such a layer, depending on the features: generators and hard-wired connections are cheap, and a query broker with tools is expensive.[^1]

**The kernel sees relational data.** The relational form of the layer is not an object model. "The access layer does not transform the relational model to an object-oriented view."[^1] For a rich domain model, an object-relational layer with a [[data-mapper]] fits better.

**One more level of calls.** Each access goes through two layers. Keller and Coldewey call the run-time cost minor, compared with the time of the database.[^1]

## When not to use it

**When a framework already gives the layer.** An ORM or a database toolkit is a database access layer: it has a logical part, the session and the mapped classes, and a physical part for each database product. Write the application against it, and do not add a second layer of the same kind on top.

**For a small program with one database and no tuning.** A few functions with SQL, or a [[table-data-gateway]], are enough.

## Variants and related patterns

**Without the cache.** For simple dialogs that change one record in each transaction, the view cache can be left out.[^1]

**Over non-relational storage.** The physical access layer can also hide databases and file formats that are not relational, such as IMS-DB, CODASYL or VSAM, and so give access to legacy data.[^1]

**With an object view.** The layer can give the kernel objects in place of relational views. Then the logical layer holds the object-relational mapping, with a [[data-mapper]], an [[identity-map]] and a [[unit-of-work]].

**Related patterns.** "The pattern is an application of Layers".[^1] The [[data-access-object]] objects of an application together form a database access layer, and a [[query-object]] is one form of the query broker. In POSA4, Data Mapper, Row Data Gateway, Table Data Gateway and Active Record are ways to design the layer.[^2] The view cache of a transaction is close to an [[identity-map]], and its commit is close to a [[unit-of-work]].

## Known uses

- Keller and Coldewey list the VAA Data Manager specification of the German insurance industry, and projects at sd&m for Thyssen, Deutsche Bahn and HYPO Bank.[^1]
- SQLAlchemy: the ORM and the SQL expression language are the logical part, and each dialect is a physical part. "The dialect is the system SQLAlchemy uses to communicate with various types of DBAPI implementations and databases."[^3]
- Django: the ORM is the same for each database, and a backend for each database product does the physical work. "Django attempts to support as many features as possible on all database backends."[^4]

## References
