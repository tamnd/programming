---
tag: GZ4XD3
kind: pattern
slug: table-data-gateway
name: Table Data Gateway
aliases: [table gateway]
area: software-design/data-access-patterns
level: core
summary: >-
  A Table Data Gateway is one object that holds all the SQL for one table or view. Its methods find, insert, update and delete rows, and they return plain data, not domain objects.
family: architectural
polarity: positive
intent: "An object that acts as a gateway to a database table. One instance handles all the rows in the table."
also_known_as: [Table Gateway]
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002), as one of the four data source architectural patterns, with Row Data Gateway, Active Record and Data Mapper.
see_also: [row-data-gateway, active-record, data-mapper, repository-pattern, facade-pattern, sql-injection]
sources:
  - url: https://martinfowler.com/eaaCatalog/tableDataGateway.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Table Data Gateway
    locator: pattern summary; the book's Chapter 10
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 10, Data Source Architectural Patterns, Table Data Gateway
  - url: https://martinfowler.com/eaaCatalog/
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture
    locator: Domain Logic Patterns; Data Source Architectural Patterns; Base Patterns
  - url: https://docs.python.org/3.14/library/sqlite3.html#sqlite3-placeholders
    title: The Python Standard Library, sqlite3, Python 3.14
    locator: Tutorial; How to use placeholders to bind values in SQL queries
  - url: https://martinfowler.com/eaaCatalog/dataMapper.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Data Mapper
    locator: pattern summary; the book's Chapter 10
  - url: https://martinfowler.com/eaaCatalog/rowDataGateway.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Row Data Gateway
    locator: pattern summary; the book's Chapter 10
  - url: https://www.oracle.com/java/technologies/dataaccessobject.html
    title: Core J2EE Patterns, Data Access Object, Oracle
    locator: Solution; Participants and Responsibilities, TransferObject
  - url: https://docs.laminas.dev/laminas-db/table-gateway/
    title: Laminas documentation, laminas-db, Table Gateways
    locator: first section
  - url: https://github.com/laminas/laminas-db/blob/ddde58a7c4124863621ee45041ce60300162e7b9/src/TableGateway/AbstractTableGateway.php#L165-L199
    title: laminas-db 2.22.0 source, src/TableGateway/AbstractTableGateway.php
    locator: select and selectWith, lines 165 to 199
---

## Intent

"An object that acts as a gateway to a database table. One instance handles all the rows in the table."[^1] The rest of the program calls the methods of the gateway, and it never writes SQL for that table itself.

## Problem

A small stock system has a products table. The order code reads the stock of a product. The restock job finds the products with low stock and adds to them. A report lists all products. In the first version, each of these places writes its own SQL. The query for "products with low stock" exists three times, with small differences. A change to a column name needs a search through the whole code base.

The forces are these. The business rules should be readable without SQL around them. The SQL should be in one place, so that a person who tunes or changes the database can find all of it. Fowler's catalogue gives the same two reasons: many developers do not write SQL well, and database administrators "need to be able to find SQL easily so they can figure out how to tune and evolve the database".[^1] And the program does not need objects with behaviour for each row: the rules are simple procedures.

## Solution

Make one class for each table or view. Give it one method for each operation that the program needs on that table: find by key, find by a criterion, insert, update and delete. Each method holds its SQL and binds the arguments as parameters. The finders return plain data, such as rows, records or a record set. The gateway has no state of its own except the connection, so one instance serves all the rows of the table.[^1][^2]

```text
  application code (transaction scripts, a table module)
      restock(products, limit, amount)
            |
            v
  ProductGateway           one instance for the whole table
      find(id)             SELECT ... WHERE id = ?
      find_low_stock(n)    SELECT ... WHERE stock < ?
      insert(...)          INSERT ...
      update_stock(id, n)  UPDATE ... WHERE id = ?
      delete(id)           DELETE ... WHERE id = ?
            |
            v
  products table           rows go out as plain data
```

The participants are the **gateway**, the **table** behind it, and the **callers**. In Fowler's catalogue, the callers are often transaction scripts, procedures that each handle one request, or a table module, a single instance that holds the business logic for all rows of a table.[^3] The gateway is a special case of the base pattern **Gateway**: "An object that encapsulates access to an external system or resource."[^3]

The following program has one gateway for a products table in SQLite. The function `restock` is the business rule. It uses the gateway and contains no SQL. Every query binds its values with `?` placeholders, as the Python documentation requires for values that come from outside the code.[^4]

```python run id=restock
import sqlite3


class ProductGateway:
    """All the SQL for the products table. It returns rows, not objects."""

    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db

    def find(self, product_id: int) -> sqlite3.Row | None:
        return self._db.execute("SELECT id, name, price, stock FROM products WHERE id = ?", (product_id,)).fetchone()

    def find_low_stock(self, limit: int) -> list[sqlite3.Row]:
        return self._db.execute("SELECT id, name, price, stock FROM products WHERE stock < ? ORDER BY id", (limit,)).fetchall()

    def insert(self, name: str, price: int, stock: int) -> int:
        return self._db.execute("INSERT INTO products (name, price, stock) VALUES (?, ?, ?)", (name, price, stock)).lastrowid

    def update_stock(self, product_id: int, stock: int) -> None:
        self._db.execute("UPDATE products SET stock = ? WHERE id = ?", (stock, product_id))

    def delete(self, product_id: int) -> None:
        self._db.execute("DELETE FROM products WHERE id = ?", (product_id,))


def restock(products: ProductGateway, limit: int, amount: int) -> list[str]:
    """A transaction script: the business rule is here, the SQL is in the gateway."""
    done = []
    for row in products.find_low_stock(limit):
        products.update_stock(row["id"], row["stock"] + amount)
        done.append(f"{row['name']} {row['stock']} -> {row['stock'] + amount}")
    return done


db = sqlite3.connect(":memory:")
db.row_factory = sqlite3.Row
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT NOT NULL, price INTEGER NOT NULL, stock INTEGER NOT NULL)")
products = ProductGateway(db)
products.insert("pen", 120, 3)
ink = products.insert("ink", 450, 40)
products.insert("pad", 300, 0)
with db:
    print("restocked:", "; ".join(restock(products, limit=5, amount=20)))
products.delete(ink)
print("rows:", [tuple(r) for r in db.execute("SELECT * FROM products ORDER BY id")])
print("find(9):", products.find(9))
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: [(1, 'pen', 120, 23), (3, 'pad', 300, 20)]
find(9): None
```

The finders return `sqlite3.Row` values, which the caller reads by column name. That is the normal shape of this pattern: the gateway passes data, and the caller decides what the data means.

## Example

Each example file has a gateway for a products table with the methods `find`, `findAll`, `findLowStock`, `insert`, `updateStock` and `delete`, and a transaction script `restock` that uses it. The Python file uses SQLite, as the block above does. The JavaScript, Java, Go and Rust files keep the table in memory, as a map of rows and an id counter, because the standard libraries of these languages have no database driver. In those files the gateway is still the only code that touches the table, and it returns copies of rows, so a caller cannot change the table except through a gateway method.

## Consequences

**Benefits.**

- **All the SQL of a table is in one class.** A person who changes the schema or tunes a query reads one file.[^1]
- **The pattern is simple.** It needs no mapping layer and no framework. The gateway is a thin layer over the database API, and a new developer can read it in minutes.
- **It fits procedural business logic.** A transaction script or a table module works on rows and record sets, and the gateway gives it exactly that.[^3]
- **It is a seam for tests.** A test can replace the gateway with a stub that returns fixed rows, and test the business rules without a database.

**Liabilities.**

- **The callers still know the columns.** The gateway hides the SQL, but its results are rows with the names of the columns. A change of a column name still reaches the callers that read it.
- **No behaviour lives with the data.** A rule, for example that a product with zero stock cannot be sold, goes into the procedures that use the rows. When many procedures need the same rule, it is copied. That is the point where Fowler moves to a domain model and a Data Mapper.[^5][^2]
- **The finder list grows.** Each new question about the table needs a new method, and the gateway can grow to dozens of finders.

The gateway must not accept SQL from its callers. A method that takes a condition as a string moves the SQL back into the callers, and it lets any value that is pasted into the condition change the query:

```python wrong id=sql-in-the-caller
import sqlite3


class ProductGateway:
    def __init__(self, db):
        self._db = db

    def find_where(self, condition):
        # bug: the caller passes SQL, and the gateway pastes it into the query
        return self._db.execute(f"SELECT name FROM products WHERE {condition}").fetchall()


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, owner TEXT)")
db.executemany("INSERT INTO products (name, owner) VALUES (?, ?)", [("pen", "ana"), ("ink", "bo"), ("pad", "bo")])
products = ProductGateway(db)

owner = "ana"
print(products.find_where(f"owner = '{owner}'"))
owner = "x' OR '1'='1"
print(products.find_where(f"owner = '{owner}'"))
```

```output
[('pen',)]
[('pen',), ('ink',), ('pad',)]
```

The second call returns the rows of every owner, because the value of `owner` closes the quote and adds a condition that is always true. A gateway method takes values as arguments and binds them as parameters, as in the first program.[^4] See [[sql-injection]].

## When not to use it

**With a rich domain model.** When the business rules live in objects with behaviour, the code that turns rows into objects grows around every call of the gateway. A Data Mapper does that work in one place and keeps the objects independent of the database.[^5]

**When the framework already gives you one.** An ORM or a query builder gives each table an API for find, insert, update and delete. A hand-written gateway on top of it only renames the calls. A gateway is still useful over raw SQL, or for queries that the ORM cannot express well.

**For one row with its own behaviour.** When the program works with one row at a time and wants an object for it, Row Data Gateway gives one object per row instead.[^6]

## Variants and related patterns

**Row Data Gateway.** One instance per row instead of one per table. "There is one instance per row."[^6] The finders of a Row Data Gateway design often live in a separate object that is a Table Data Gateway in all but name. See [[row-data-gateway]].

**Active Record.** A row object that also holds the domain logic. See [[active-record]].

**Data Mapper.** A layer that maps between domain objects and tables, so that neither knows the other.[^5] See [[data-mapper]].

**Repository.** A collection-like interface for domain objects, usually over a Data Mapper. A repository returns domain objects. A table gateway returns rows. See [[repository-pattern]].

**Data access object.** The Core J2EE pattern: a DAO is used "to abstract and encapsulate all access to the data source", and one DAO serves one business object.[^7] A DAO and a table gateway are often the same class with two names. The difference is in what they return: "The DataAccessObject may use a Transfer Object to return data to the client."[^7]

**Record Set.** "An in-memory representation of tabular data."[^3] A gateway can return a record set, and the record set can carry the changes back to the gateway.

**Facade.** A gateway is a facade over the database API for one table. See [[facade-pattern]].

## Known uses

- laminas-db, a PHP database library, has a Table Gateway subcomponent that "provides an object-oriented representation of a database table; its methods mirror the most common table operations".[^8] Its interface has `select`, `insert`, `update` and `delete`. In [AbstractTableGateway.php at 2.22.0](https://github.com/laminas/laminas-db/blob/ddde58a7c4124863621ee45041ce60300162e7b9/src/TableGateway/AbstractTableGateway.php#L165-L199), `select` builds a `Select` object for the table of the gateway, adds the condition of the caller, and executes it.[^9]

## References
