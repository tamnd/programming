---
tag: 01FS9B
kind: pattern
slug: row-data-gateway
name: Row Data Gateway
aliases: [row gateway]
area: software-design/data-access-patterns
level: core
summary: >-
  A Row Data Gateway is an object for one row of a table. It has one field for each column and methods to insert, update and delete its row, and it has no business logic.
family: architectural
polarity: positive
intent: "An object that acts as a gateway to a single record in a data source. There is one instance per row."
also_known_as: [Row Gateway]
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002), as one of the four data source architectural patterns.
see_also: [table-data-gateway, active-record, data-mapper, repository-pattern, proxy-pattern]
sources:
  - url: https://martinfowler.com/eaaCatalog/rowDataGateway.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Row Data Gateway
    locator: pattern summary; the book's Chapter 10
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 10, Data Source Architectural Patterns, Row Data Gateway
  - url: https://martinfowler.com/eaaCatalog/identityMap.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Identity Map
    locator: pattern summary; the book's Chapter 11
  - url: https://martinfowler.com/eaaCatalog/activeRecord.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Active Record
    locator: pattern summary; the book's Chapter 10
  - url: https://docs.laminas.dev/laminas-db/row-gateway/
    title: Laminas documentation, laminas-db, Row Gateways
    locator: first section; ActiveRecord Style Objects
  - url: https://github.com/laminas/laminas-db/blob/ddde58a7c4124863621ee45041ce60300162e7b9/src/RowGateway/AbstractRowGateway.php#L110-L180
    title: laminas-db 2.22.0 source, src/RowGateway/AbstractRowGateway.php
    locator: save, lines 110 to 180
---

## Intent

"An object that acts as a gateway to a single record in a data source. There is one instance per row."[^1] The object looks like the row: it has one field for each column. It also knows how to write itself back to the table.

## Problem

A program works with one product at a time: it reads the product, changes the stock, and saves it. With a Table Data Gateway, the program passes ids and column values to methods of one object for the whole table. The code reads like a list of SQL calls with the SQL removed. The program would be clearer if a product were an object with fields, and if a changed product could save itself.

The forces are these. The program wants the data of a row in the normal form of the language: fields that it reads and writes. The database code should be in one place and not in the business rules. Fowler's catalogue names the cost of the other way, where database code sits inside objects that also have business logic: the objects become more complex, and tests become slower, because the objects need a database.[^1] So the row object holds the data and the database access, and nothing else.

## Solution

Make one class whose fields are the columns of the table. Give it the methods `insert`, `update` and `delete`, each with the SQL for one row. Put the finders, which read rows and make one gateway object for each row, in a separate finder class or in static methods. The business logic stays outside, in transaction scripts that read and set the fields.[^1][^2]

```text
  application code (transaction scripts)
      for p in finder.find_low_stock(5): p.stock += 20; p.update()
            |                           |
            v                           v
  ProductFinder                  ProductRow (one object per row)
      find(id)                       id, name, price, stock
      find_low_stock(n)              insert()   INSERT ...
      makes ProductRow objects       update()   UPDATE ... WHERE id = ?
            |                        delete()   DELETE ... WHERE id = ?
            v                           |
  products table  <---------------------+
```

The participants are the **row gateway**, with fields and the write methods, the **finder**, which holds the read queries and creates gateways, and the **callers**. A row gateway "gives you objects that look exactly like the record in your record structure but can be accessed with the regular mechanisms of your programming language".[^1]

The following program uses SQLite. `ProductRow` is the gateway, `ProductFinder` holds the queries that read rows, and `restock` is the business rule.

```python run id=restock-rows
import sqlite3


class ProductRow:
    """A gateway to one row of the products table. It has the columns as fields, and no business logic."""

    def __init__(self, db: sqlite3.Connection, name: str, price: int, stock: int, id: int | None = None) -> None:
        self._db = db
        self.id, self.name, self.price, self.stock = id, name, price, stock

    def insert(self) -> None:
        cur = self._db.execute("INSERT INTO products (name, price, stock) VALUES (?, ?, ?)", (self.name, self.price, self.stock))
        self.id = cur.lastrowid

    def update(self) -> None:
        self._db.execute("UPDATE products SET name = ?, price = ?, stock = ? WHERE id = ?", (self.name, self.price, self.stock, self.id))

    def delete(self) -> None:
        self._db.execute("DELETE FROM products WHERE id = ?", (self.id,))


class ProductFinder:
    """The finders are static in spirit: they make one ProductRow for each row that they read."""

    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db

    def _load(self, row: tuple) -> ProductRow:
        id, name, price, stock = row
        return ProductRow(self._db, name, price, stock, id)

    def find(self, product_id: int) -> ProductRow | None:
        row = self._db.execute("SELECT id, name, price, stock FROM products WHERE id = ?", (product_id,)).fetchone()
        return self._load(row) if row else None

    def find_low_stock(self, limit: int) -> list[ProductRow]:
        rows = self._db.execute("SELECT id, name, price, stock FROM products WHERE stock < ? ORDER BY id", (limit,))
        return [self._load(row) for row in rows]


def restock(finder: ProductFinder, limit: int, amount: int) -> list[str]:
    """A transaction script. It changes the fields of each row object and tells the row to save itself."""
    done = []
    for product in finder.find_low_stock(limit):
        before = product.stock
        product.stock += amount
        product.update()
        done.append(f"{product.name} {before} -> {product.stock}")
    return done


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT NOT NULL, price INTEGER NOT NULL, stock INTEGER NOT NULL)")
for name, price, stock in [("pen", 120, 3), ("ink", 450, 40), ("pad", 300, 0)]:
    ProductRow(db, name, price, stock).insert()
finder = ProductFinder(db)
with db:
    print("restocked:", "; ".join(restock(finder, limit=5, amount=20)))
finder.find(2).delete()
print("rows:", db.execute("SELECT * FROM products ORDER BY id").fetchall())
print("find(9):", finder.find(9))
```

```output
restocked: pen 3 -> 23; pad 0 -> 20
rows: [(1, 'pen', 120, 23), (3, 'pad', 300, 20)]
find(9): None
```

The rule changes a field and calls `update`. It does not know the SQL or the column list.

## Example

Each example file has a `ProductRow` gateway with `insert`, `update` and `delete`, a `ProductFinder` with `find`, `findAll` and `findLowStock`, and the transaction script `restock`. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the table in memory, as a map from id to the stored columns, because the standard libraries of these languages have no database driver. The gateway copies its fields into the table on `update`, so a changed field reaches the table only through that call, as with a database.

## Consequences

**Benefits.**

- **The data has the form of the language.** A caller reads `product.stock` and sets it, and the type system of the language can check the fields.
- **The SQL of a row is in one class.** The insert, update and delete statements exist once.[^2]
- **The pattern is simple.** A code generator or a small base class can make the gateways from the schema, because each class follows the columns of one table.

**Liabilities.**

**Two objects for one row.** Each call of a finder makes a new object. Two parts of a program can load the same row, change different fields, and both save. The last save wins, and it writes back the old values of the fields that it did not change. Fowler's catalogue describes this risk: "If you aren't careful you can load the data from the same database record into two different objects."[^3]

```python wrong id=two-objects-one-row
import sqlite3


class ProductRow:
    def __init__(self, db, id, name, price, stock):
        self._db = db
        self.id, self.name, self.price, self.stock = id, name, price, stock

    def update(self):
        self._db.execute("UPDATE products SET name = ?, price = ?, stock = ? WHERE id = ?", (self.name, self.price, self.stock, self.id))


def find(db, product_id):
    return ProductRow(db, *db.execute("SELECT id, name, price, stock FROM products WHERE id = ?", (product_id,)).fetchone())


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, price INTEGER, stock INTEGER)")
db.execute("INSERT INTO products VALUES (1, 'pen', 120, 3)")

stock_job = find(db, 1)   # one part of the program loads row 1
price_job = find(db, 1)   # another part loads row 1 again: a second object
stock_job.stock += 20
stock_job.update()
price_job.price = 150
price_job.update()        # bug: writes back the old stock, 3
print(db.execute("SELECT * FROM products").fetchone())
```

```output
(1, 'pen', 150, 3)
```

The stock change is lost. The fixes are an Identity Map, which returns the same object for the same row within one business transaction,[^3] an update that writes only the changed columns, or an optimistic lock that refuses a save when the row changed after it was read.

**The business logic has nowhere to go.** The rules sit in scripts around the gateways. When a rule is about one row, for example the price of a product with a discount, it is natural to put it on the gateway. Then the gateway has become an Active Record.[^4]

**The objects follow the tables.** A gateway class has the columns of its table, so a change of the schema changes the class and its callers.

## When not to use it

**When the rows carry domain logic.** Use Active Record, which is the same object with the business rules added, or a Data Mapper with a separate domain model.[^4]

**When the program works on sets of rows.** A report or a batch job that reads many rows and computes totals does not need one object per row. A Table Data Gateway that returns rows or a record set is simpler and faster.

**When an ORM is in use.** An ORM gives each table a class, often in the Active Record or Data Mapper form, and a hand-written row gateway on top of it adds a second class for the same row.

## Variants and related patterns

**Table Data Gateway.** One object for the whole table, which returns plain data. The finder of a Row Data Gateway design is a table gateway that returns row objects.[^2] See [[table-data-gateway]].

**Active Record.** A row gateway with domain logic. Fowler's catalogue describes Active Record as an object that "wraps a row in a database table or view, encapsulates the database access, and adds domain logic on that data".[^4] See [[active-record]].

**Data Mapper.** The domain objects know nothing of the database, and a separate mapper loads and saves them. See [[data-mapper]].

**Identity Map.** Keeps one object for each row that a business transaction has loaded, which removes the lost update of the example above.[^3]

**Generated gateways.** A tool reads the schema and writes one gateway class for each table. The gateways have no hand-written logic, so a regenerated class loses nothing.

## Known uses

- laminas-db, a PHP database library, has a RowGateway subcomponent that "implements the Row Data Gateway pattern described in the book Patterns of Enterprise Application Architecture".[^5] Its interface has two methods, `save` and `delete`. In [AbstractRowGateway.php at 2.22.0](https://github.com/laminas/laminas-db/blob/ddde58a7c4124863621ee45041ce60300162e7b9/src/RowGateway/AbstractRowGateway.php#L110-L180), `save` runs an UPDATE with the primary key in the WHERE clause when the row came from the database, and an INSERT otherwise. Then it reads the row again, so the fields hold what the database stored.[^6] The documentation shows how a custom class with business behaviour turns the row gateway into objects in the Active Record style.[^5]

## References
