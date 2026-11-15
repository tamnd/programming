---
tag: B853HS
kind: pattern
slug: table-module
name: Table module
aliases: [Table Module, table module pattern, module per table]
area: software-design/enterprise-application-patterns
level: advanced
summary: >-
  A table module is one object that holds the business logic for all the rows of one database table, and works on the rows of a record set.
family: architectural
polarity: positive
intent: "A single instance that handles the business logic for all rows in a database table or view."
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002) as the third way to organize domain logic, for tools that work with record sets.
see_also: [transaction-script, domain-model, enterprise-application, table-data-gateway, row-data-gateway, active-record, query-object, replace-conditional-with-polymorphism]
sources:
  - url: https://martinfowler.com/eaaCatalog/tableModule.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Table Module
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 9, Domain Logic Patterns, Table Module
  - url: https://learn.microsoft.com/en-us/dotnet/framework/data/adonet/dataset-datatable-dataview/
    title: Microsoft, ADO.NET, DataSets, DataTables, and DataViews
    locator: introduction
  - url: https://docs.djangoproject.com/en/5.2/topics/db/managers/
    title: Django 5.2 documentation, Managers
    locator: introduction; Adding extra manager methods
---

## Intent

A table module is "A single instance that handles the business logic for all rows in a database table or view."[^1] There is one class for each table, such as orders or customers, and one instance of it works on all the rows. A method gets the id of the row as an argument. Fowler gives it as the third way to organize domain logic, between a [[transaction-script]] and a [[domain-model]].[^2]

## Problem

"One of the key messages of object orientation is bundling the data with the behavior that uses it."[^1] A [[domain-model]] does this with one object for each row. But a domain model does not fit the relational database well. Fowler: "As a result you often need considerable programmatic gymnastics to pull data in and out of the database, transforming between two different representations of the data."[^1]

Some environments are built around tables. Their tools load the result of a query into a record set, show it in a grid, and write the changes back. A design with an object for each row works against these tools. A [[transaction-script]] design fits the tools, but it spreads the rules about one table across many scripts.

## Solution

"A Table Module organizes domain logic with one class per table in the data-base, and a single instance of a class contains the various procedures that will act on the data."[^1] The difference from a domain model is simple: "if you have many orders, a Domain Model will have one order object per order while a Table Module will have one object to handle all orders."[^1]

The parts are:

- **The record set.** An in-memory copy of the rows that a query returned, often of several tables. An ADO.NET `DataSet` is the classic form: "a memory-resident representation of data that provides a consistent relational programming model regardless of the source of the data it contains."[^3]
- **One module for each table.** The module gets the record set when it is made. Its methods take the id of a row, and they can also work on all the rows at once.
- **A gateway.** Often a [[table-data-gateway]] runs the queries that fill the record set and writes the changes back.

```text
  presentation (a grid of rows)
          |
  OrderModule(record set)      CustomerModule(record set)
    total(order_id)              row(customer_id)
    count_over(limit)            names_with_tier(tier)
          |                            |
  record set: customer, orders, order_line rows
          |
  gateway: select ... from each table
```

The following program has one table module over the rows of a product table. One instance finds the price of one row and also answers a question about all the rows.

```python run id=module-over-rows
# The record set: the rows of the product table, as a query returns them.
products = [
    {"id": 1, "name": "lamp", "price_cents": 2500, "stock": 4},
    {"id": 2, "name": "desk", "price_cents": 7500, "stock": 0},
]


# One instance of the table module handles all the rows of the table.
class ProductModule:
    def __init__(self, rows):
        self.rows = rows

    def price(self, product_id):
        return next(r["price_cents"] for r in self.rows if r["id"] == product_id)

    def out_of_stock(self):
        return [r["name"] for r in self.rows if r["stock"] == 0]


module = ProductModule(products)
print("price of product 1:", module.price(1))
print("out of stock:", module.out_of_stock())
```

```output
price of product 1: 2500
out of stock: ['desk']
```

## Example

Each example file loads a record set with the rows of three tables: customers, orders and order lines. A `CustomerModule` and an `OrderModule` each take the record set once. `OrderModule.total` adds the lines of an order and applies the discount of the customer's tier: 10 percent off for a gold customer on 100.00 or more, and 20 percent off for staff. The program prints the totals of the three orders, then two answers about many rows at once, and then the error for an order that is not in the record set. The Python file loads the record set from SQLite; the other files write the rows in the code.

## Consequences

**Benefits.**

- **It fits record sets.** The logic works on the same rows that the database returns and that the user interface shows. There is no mapping between objects and rows.
- **The rules of a table are in one place.** All the logic for orders is in the order module, not spread across scripts.
- **Questions about many rows are easy.** A method can work on all the rows of the table, as `count_over` does. Django gives the same split. Its documentation calls extra manager methods "the preferred way to add" functions for the whole table, and it puts the functions for one row in model methods.[^4]

**Liabilities.**

**The module sees only the record set.** The module does not query the database. It works on the rows that it was given. In the following program, the record set holds the lines of one month only. The module finds no lines for an older order and gives a subtotal of 0 with no error.

```python wrong id=row-not-in-record-set
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table order_line (order_id integer, month text, qty integer, price_cents integer);
insert into order_line values (3, '2026-11', 1, 2500), (4, '2026-10', 2, 500), (4, '2026-10', 1, 900);
""")


class OrderModule:
    def __init__(self, rows):
        self.rows = rows

    def subtotal(self, order_id):
        # Wrong: no check that the record set holds the order.
        return sum(qty * price for oid, qty, price in self.rows if oid == order_id)


# The record set was loaded for one month only.
rows = db.execute("select order_id, qty, price_cents from order_line where month = '2026-11'").fetchall()
module = OrderModule(rows)
print("order 4 in the database:", db.execute("select sum(qty * price_cents) from order_line where order_id = 4").fetchone()[0])
print("order 4 from the module:", module.subtotal(4))
```

```output
order 4 in the database: 1900
order 4 from the module: 0
```

Check that the row is in the record set before the calculation, as the `row` method of the examples does, and load the record set with the rows that each use needs.

**No object for each row.** There is no customer object, so the rules cannot use polymorphism. Each rule that depends on the tier is a chain of conditions, as in `total`. As these chains grow, the design has the problem that [[replace-conditional-with-polymorphism]] solves in a [[domain-model]].

**No identity.** Each call needs the id of the row. A module cannot hold a reference to one order, and two record sets can hold two different copies of one row.

## When not to use it

**When the logic is complex.** Rules with many cases and relations need the objects and the polymorphism of a [[domain-model]].

**When the tools do not use record sets.** In an environment with a good object-relational mapper and no grids of rows, a table module gives little. A [[transaction-script]] is simpler, and a [[domain-model]] is more powerful.

## Variants and related patterns

**Static methods.** The module can be a class with static methods that take the record set as an argument. This makes it close to a set of [[transaction-script|transaction scripts]] grouped by table.

**Instance for a query or a view.** A module can work on the rows of a view or a query, not only a table. The pattern definition says "a database table or view".[^1]

**Related patterns.** A [[table-data-gateway]] is the data source pattern that matches a table module: one object for each table that runs the SQL. A [[row-data-gateway]] or an [[active-record]] has one object for each row instead. The [[record-set]] is the data structure that the module works on.

## Known uses

- .NET applications built on ADO.NET, where a `DataSet` holds "the tables that contain, order, and constrain the data, as well as the relationships between the tables",[^3] and a `DataAdapter` fills it and writes the changes back.[^3]
- Django managers. "A Manager is the interface through which database query operations are provided to Django models."[^4] Extra manager methods hold the logic for the whole table, while model methods hold the logic for one row.[^4]

## References
