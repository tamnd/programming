---
tag: MS2MYG
kind: pattern
slug: data-mapper
name: Data Mapper
aliases: [data mapper pattern]
area: software-design/data-access-patterns
level: core
summary: >-
  A Data Mapper is a layer that moves data between domain objects and a database. The objects know nothing of the database, and the database knows nothing of the objects.
family: architectural
polarity: positive
intent: "A layer of mappers that moves data between objects and a database while keeping them independent of each other and the mapper itself."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). SQLAlchemy and Doctrine ORM are built on it.
see_also: [active-record, row-data-gateway, table-data-gateway, repository-pattern]
sources:
  - url: https://martinfowler.com/eaaCatalog/dataMapper.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Data Mapper
    locator: pattern summary; the book's Chapter 10
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 10, Data Source Architectural Patterns, Data Mapper
  - url: https://typeorm.io/docs/guides/active-record-data-mapper
    title: TypeORM documentation, Active Record vs Data Mapper
    locator: What is the Data Mapper pattern; Which one should I choose
  - url: https://martinfowler.com/eaaCatalog/unitOfWork.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Unit of Work
    locator: pattern summary
  - url: https://docs.sqlalchemy.org/en/latest/orm/session_basics.html
    title: SQLAlchemy documentation, Session Basics
    locator: What does the Session do
  - url: https://martinfowler.com/eaaCatalog/activeRecord.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Active Record
    locator: pattern summary
  - url: https://www.sqlalchemy.org/features.html
    title: SQLAlchemy, Key Features of SQLAlchemy
    locator: Unit Of Work; Separate mapping and class design
  - url: https://docs.sqlalchemy.org/en/20/orm/mapping_styles.html
    title: SQLAlchemy 2.0 documentation, ORM Mapped Class Overview
    locator: Imperative Mapping
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/decl_api.py#L1839-L1895
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/decl_api.py
    locator: registry.map_imperatively, lines 1839 to 1895
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/tutorials/getting-started.html
    title: Doctrine ORM 3.7 documentation, Getting Started with Doctrine
    locator: What is Doctrine?
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/reference/architecture.html
    title: Doctrine ORM 3.7 documentation, Architecture
    locator: Terminology, Entities
---

## Intent

"A layer of mappers that moves data between objects and a database while keeping them independent of each other and the mapper itself."[^1] The domain objects hold the data and the business rules. A separate mapper reads rows into objects and writes objects back into rows.

## Problem

A program has a rich domain model, with lists, value objects and inheritance. The database has tables, columns and foreign keys. The two do not match. Fowler's catalogue says: "Many parts of an object, such as collections and inheritance, aren't present in relational databases."[^1] When each side is designed for its own job, the object schema and the relational schema differ.

Something must still move the data between the two. If the domain objects do this work, as an Active Record does, they must know the tables and the SQL. Then "changes in one tend to ripple to the other."[^1] A change of a column changes a domain class, and a test of a business rule needs a database.

## Solution

Put all the database code in mapper classes, one for each kind of domain object. The mapper has methods such as `find`, `insert`, `update` and `delete`. It reads rows, makes domain objects and fills their fields, and it turns the fields of an object back into rows. The domain classes have no SQL and no reference to the mapper. "With Data Mapper the in-memory objects needn't know even that there's a database present".[^1][^2]

```text
  domain layer                          data source layer
  Product                               ProductMapper
      name, price, stock, tags              find(id)    SELECT products + product_tags
      sell(n)                               insert(p)   INSERT into both tables
      (no SQL, no mapper)                   update(p)   UPDATE products, rewrite tags
            ^                               delete(p)
            |  makes and reads objects          |
            +-------------------------------+   v
                                            products (id, title, price_cents, qty)
                                            product_tags (product_id, tag)
```

The participants are the **domain objects**, the **mapper**, which knows both the objects and the schema, and the **callers**, which ask the mapper for objects and give changed objects back to it.

The following program uses SQLite. The field names of `Product` differ from the column names, and its `tags` list is stored as rows of a second table. Only `ProductMapper` knows this.

```python run id=map-two-tables
import sqlite3
from dataclasses import dataclass, field


@dataclass
class Product:
    """A domain object. It has no SQL, no connection and no idea that a database exists."""

    name: str
    price: int
    stock: int
    tags: list[str] = field(default_factory=list)
    id: int | None = None

    def sell(self, quantity: int) -> int:
        if quantity > self.stock:
            raise ValueError(f"{self.name}: only {self.stock} in stock")
        self.stock -= quantity
        return quantity * self.price


class ProductMapper:
    """Moves products between objects and two tables. The tags list becomes rows of product_tags."""

    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db

    def find(self, product_id: int) -> Product | None:
        row = self._db.execute("SELECT id, title, price_cents, qty FROM products WHERE id = ?", (product_id,)).fetchone()
        if row is None:
            return None
        tags = [t for (t,) in self._db.execute("SELECT tag FROM product_tags WHERE product_id = ? ORDER BY tag", (product_id,))]
        return Product(name=row[1], price=row[2], stock=row[3], tags=tags, id=row[0])

    def insert(self, p: Product) -> None:
        p.id = self._db.execute("INSERT INTO products (title, price_cents, qty) VALUES (?, ?, ?)", (p.name, p.price, p.stock)).lastrowid
        self._write_tags(p)

    def update(self, p: Product) -> None:
        self._db.execute("UPDATE products SET title = ?, price_cents = ?, qty = ? WHERE id = ?", (p.name, p.price, p.stock, p.id))
        self._db.execute("DELETE FROM product_tags WHERE product_id = ?", (p.id,))
        self._write_tags(p)

    def _write_tags(self, p: Product) -> None:
        self._db.executemany("INSERT INTO product_tags (product_id, tag) VALUES (?, ?)", [(p.id, t) for t in p.tags])


db = sqlite3.connect(":memory:")
db.executescript("""
CREATE TABLE products (id INTEGER PRIMARY KEY, title TEXT NOT NULL, price_cents INTEGER NOT NULL, qty INTEGER NOT NULL);
CREATE TABLE product_tags (product_id INTEGER REFERENCES products(id), tag TEXT NOT NULL);
""")
mapper = ProductMapper(db)
mapper.insert(Product("pen", 120, 3, ["office"]))

pen = mapper.find(1)
print("sold:", pen.sell(2))          # domain logic, no database involved
pen.tags.append("sale")
mapper.update(pen)                   # the mapper writes both tables
print("object:", mapper.find(1))
print("products:", db.execute("SELECT * FROM products").fetchall())
print("product_tags:", db.execute("SELECT * FROM product_tags ORDER BY tag").fetchall())
```

```output
sold: 240
object: Product(name='pen', price=120, stock=1, tags=['office', 'sale'], id=1)
products: [(1, 'pen', 120, 1)]
product_tags: [(1, 'office'), (1, 'sale')]
```

`Product.sell` can be tested with no database at all. The object has a list, and the database has a second table, and neither side had to change its shape for the other.

## Example

Each example file has a plain `Product` class with the domain method `sell`, and a `ProductMapper` with `find`, `insert`, `update` and `delete`. The stored rows have other names than the fields (`title`, `price_cents`, `qty`), and the tags are rows of a second table. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the two tables in memory, because the standard libraries of these languages have no database driver. The domain class in each file has no reference to the tables.

## Consequences

**Benefits.**

- **The domain model and the schema can differ.** Each side uses its own structures: lists and value objects in memory, tables and keys in the database. Only the mapper changes when one side changes.[^1]
- **The domain logic is testable without a database.** The domain objects need no SQL interface code and no knowledge of the schema.[^1]
- **It scales to large models.** TypeORM's guide says that the Data Mapper approach "helps with maintainability, which is more effective in larger apps."[^3]

**Liabilities.**

**A change in memory is not saved by itself.** The domain object has no `save` method, so a changed object reaches the database only when some code gives it to the mapper. A function that loads an object, changes it and returns has lost the change.

```python wrong id=change-never-written
import sqlite3
from dataclasses import dataclass


@dataclass
class Product:
    name: str
    stock: int
    id: int

    def sell(self, quantity):
        self.stock -= quantity


class ProductMapper:
    def __init__(self, db):
        self.db = db

    def find(self, id):
        return Product(*self.db.execute("SELECT name, stock, id FROM products WHERE id = ?", (id,)).fetchone())

    def update(self, p):
        self.db.execute("UPDATE products SET name = ?, stock = ? WHERE id = ?", (p.name, p.stock, p.id))


def checkout(mapper, product_id, quantity):
    product = mapper.find(product_id)
    product.sell(quantity)  # bug: the change stays in memory; nothing calls mapper.update
    return product


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, stock INTEGER)")
db.execute("INSERT INTO products VALUES (1, 'pen', 3)")
mapper = ProductMapper(db)
print("in memory:", checkout(mapper, 1, 2).stock)
print("in the table:", mapper.find(1).stock)
```

```output
in memory: 1
in the table: 3
```

The fix is a call of `mapper.update`, or a Unit of Work, which "Maintains a list of objects affected by a business transaction and coordinates the writing out of changes".[^4] SQLAlchemy uses one. Its Session records each change of an attribute and writes the pending changes before the next query or the commit: "This is known as the unit of work pattern."[^5]

**More code.** Each domain class has a mapper, and the mapper repeats the field list in its SQL. ORMs remove most of this code with mapping metadata, but then the mapping is one more thing to learn. TypeORM's guide puts Active Record on the other side: it "helps keep things simple which works well in smaller apps."[^3]

**The mapper must reach private state.** The mapper sets and reads the fields of the domain objects, so the domain classes must expose their fields, or the mapper uses reflection, as ORMs do.

## When not to use it

**When the objects match the tables.** If each class is one table with the same columns, and the rules are simple, Active Record is less code. Fowler's catalogue describes Active Record as "the most obvious approach".[^6]

**When there is little domain logic.** A program that mostly reads and writes rows, for example a report or an admin screen, gains little from a separate domain model. A Table Data Gateway or plain queries are enough.

## Variants and related patterns

**Active Record.** The domain object holds its own data access. See [[active-record]].

**Row Data Gateway and Table Data Gateway.** Gateways that hold the SQL for one row or one table and return plain data, with no domain model. See [[row-data-gateway]] and [[table-data-gateway]].

**Repository.** A collection-like interface over the mappers, which the domain code uses to get and add objects. TypeORM says that with Data Mapper "you define all your query methods in separate classes called "repositories"".[^3] See [[repository-pattern]].

**Unit of Work and Identity Map.** Mappers are usually used with a Unit of Work, which tracks changed objects and writes them at the end of a business transaction,[^4] and with an Identity Map, which keeps one object for each row. The SQLAlchemy Session holds its objects "inside a structure called the identity map".[^5]

**Metadata mapping.** An ORM describes the mapping as data, for example a table object and a class, and a generic mapper reads that data. SQLAlchemy says that "the ORM considers the user-defined class, the associated table metadata, and the mapping of the two to be entirely separate."[^7]

## Known uses

- SQLAlchemy, the Python ORM, can map any plain class to a table. Its imperative mapping "originates from the very first releases of SQLAlchemy in 2006".[^8] In [decl_api.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/decl_api.py#L1839-L1895), `registry.map_imperatively` takes a class and a table and builds a `Mapper` for them. The class itself "is not scanned for any mapping information".[^9]
- Doctrine ORM, for PHP: "It uses the Data Mapper pattern at the heart, aiming for a complete separation of your domain/business logic from the persistence in a relational database management system."[^10] An entity "can be any regular PHP class" within a few restrictions.[^11]
- TypeORM supports both forms. In its Data Mapper form, "your entities are very dumb - they just define their properties and may have some "dummy" methods."[^3]

## References
