---
tag: AVTYES
kind: pattern
slug: active-record
name: Active Record
aliases: [active record pattern]
area: software-design/data-access-patterns
level: core
summary: >-
  An Active Record is an object that wraps one row of a table. It holds the data of the row, the code that reads and writes the row, and the domain logic on that data.
family: architectural
polarity: positive
intent: "An object that wraps a row in a database table or view, encapsulates the database access, and adds domain logic on that data."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). The Rails ORM and the Django models describe themselves with it.
see_also: [row-data-gateway, data-mapper, table-data-gateway, repository-pattern]
sources:
  - url: https://martinfowler.com/eaaCatalog/activeRecord.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Active Record
    locator: pattern summary; the book's Chapter 10
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 10, Data Source Architectural Patterns, Active Record
  - url: https://guides.rubyonrails.org/active_record_basics.html
    title: Rails Guides, Active Record Basics
    locator: 1.1 The Active Record Pattern; 5 Validations
  - url: https://typeorm.io/docs/guides/active-record-data-mapper
    title: TypeORM documentation, Active Record vs Data Mapper
    locator: What is the Active Record pattern; Which one should I choose
  - url: https://guides.rubyonrails.org/active_record_validations.html
    title: Rails Guides, Active Record Validations
    locator: Skipping Validations
  - url: https://docs.djangoproject.com/en/6.1/topics/db/queries/
    title: Django documentation, Making queries
    locator: Updating multiple objects at once
  - url: https://martinfowler.com/eaaCatalog/dataMapper.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Data Mapper
    locator: pattern summary
  - url: https://martinfowler.com/eaaCatalog/rowDataGateway.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Row Data Gateway
    locator: pattern summary
  - url: https://github.com/rails/rails/blob/3f290b759c8e2ffe7c257951b1255740751355e4/activerecord/lib/active_record/persistence.rb#L923-L928
    title: Rails 8.1.4.1 source, activerecord/lib/active_record/persistence.rb
    locator: create_or_update, lines 923 to 928
  - url: https://docs.djangoproject.com/en/6.1/misc/design-philosophies/
    title: Django documentation, Design philosophies
    locator: Models, Include all relevant domain logic
  - url: https://github.com/django/django/blob/9e7cc2b628fe8fd3895986af9b7fc9525034c1b0/django/db/models/base.py#L1081-L1175
    title: Django 5.2 source, django/db/models/base.py
    locator: Model._save_table, lines 1081 to 1175
---

## Intent

"An object that wraps a row in a database table or view, encapsulates the database access, and adds domain logic on that data."[^1] One object stands for one row. It knows its data, it knows how to load and save itself, and it knows the business rules of that data.

## Problem

A program has domain objects such as products and orders. Each object has data and behaviour, and most of the data must be stored in a database. The rules are simple and close to the data: a product cannot be sold when it has no stock, and the stock must never be negative. The tables match the objects one to one.

In this case a separate layer between the objects and the tables adds classes and code with little gain. Fowler's catalogue states the simple answer: "Active Record uses the most obvious approach, putting data access logic in the domain object."[^1] Then each developer knows where the code that reads and writes the data is.

## Solution

Make one class for each table. The fields of the class are the columns. The class has finders, usually static methods, that read rows and make one object for each row. Each object has `save` and `delete` methods, and the domain methods that work on its fields.[^1][^2] A framework usually puts the finders and `save` in a base class, so a model class only names its table, its columns and its rules.

```text
  application code
      p = Product.find(1); p.sell(2); p.save()
            |
            v
  Record (base class)                 Product (one object per row)
      find(id), all()   static            name, price, stock
      save()            INSERT or UPDATE   validate()   stock >= 0
      delete()          DELETE             sell(n), needs_restock()
            |
            v
  products table
```

The participants are the **active record class**, which holds the finders, the **active record object**, which holds the row, the save and delete methods and the domain logic, and the **callers**. The Rails guide quotes Fowler and adds that "Active Record classes match very closely to the record structure of the underlying database."[^3]

The following program uses SQLite. `Product` is the Active Record. The rule that the stock cannot be negative is in `save`, so each write through the object checks it.

```python run id=sell-and-save
import sqlite3


class Product:
    """An Active Record: one object per row, with the data access and the domain logic together."""

    db: sqlite3.Connection

    def __init__(self, name: str, price: int, stock: int = 0, id: int | None = None) -> None:
        self.id, self.name, self.price, self.stock = id, name, price, stock

    # data access: finders are class methods, saving is an instance method
    @classmethod
    def find(cls, product_id: int) -> "Product | None":
        row = cls.db.execute("SELECT name, price, stock, id FROM products WHERE id = ?", (product_id,)).fetchone()
        return cls(*row) if row else None

    @classmethod
    def low_stock(cls, limit: int) -> list["Product"]:
        rows = cls.db.execute("SELECT name, price, stock, id FROM products WHERE stock < ? ORDER BY id", (limit,))
        return [cls(*row) for row in rows]

    def save(self) -> None:
        if self.stock < 0:
            raise ValueError(f"{self.name}: stock cannot be negative")
        if self.id is None:
            self.id = self.db.execute("INSERT INTO products (name, price, stock) VALUES (?, ?, ?)", (self.name, self.price, self.stock)).lastrowid
        else:
            self.db.execute("UPDATE products SET name = ?, price = ?, stock = ? WHERE id = ?", (self.name, self.price, self.stock, self.id))

    # domain logic on the same object
    def sell(self, quantity: int) -> int:
        if quantity > self.stock:
            raise ValueError(f"{self.name}: only {self.stock} in stock")
        self.stock -= quantity
        return quantity * self.price

    def restock(self, amount: int) -> None:
        self.stock += amount


Product.db = sqlite3.connect(":memory:")
Product.db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT NOT NULL, price INTEGER NOT NULL, stock INTEGER NOT NULL)")
for name, price, stock in [("pen", 120, 3), ("ink", 450, 40), ("pad", 300, 0)]:
    Product(name, price, stock).save()

pen = Product.find(1)
print("sold 2 pens for", pen.sell(2))
pen.save()
for product in Product.low_stock(5):
    product.restock(20)
    product.save()
print([(p.id, p.name, p.stock) for p in map(Product.find, [1, 2, 3])])
try:
    Product.find(3).sell(50)
except ValueError as e:
    print("refused:", e)
```

```output
sold 2 pens for 240
[(1, 'pen', 21), (2, 'ink', 40), (3, 'pad', 20)]
refused: pad: only 20 in stock
```

The caller does not see SQL. It finds a product, calls a domain method, and saves the product.

## Example

Each example file has a `Product` Active Record with the finders `find` and `all`, a `save` method that runs `validate` first, and the domain methods `sell` and `needsRestock`. The Python file has a small `Record` base class, which holds the finders and `save` for any table, as a framework does. It uses SQLite. The JavaScript, Java, Go and Rust files keep the table in memory, as a map from id to the stored columns, because the standard libraries of these languages have no database driver. A change of a field reaches the table only through `save`, as with a database.

## Consequences

**Benefits.**

- **It is simple.** Each table has one class, and each developer knows where the data access is.[^1] TypeORM's guide says that the Active Record approach "helps keep things simple which works well in smaller apps."[^4]
- **The rules sit next to the data.** `sell` and the stock check are methods of `Product`, so a reader finds the data and its rules in one class.
- **Frameworks support it well.** A base class gives each model the finders, `save` and `delete`, so a model class can be a few lines long.[^3][^4]

**Liabilities.**

**The rules in save do not cover every write.** A rule that is checked in `save` protects only the writes that go through `save`. A bulk update is one SQL statement, and it makes no objects. The Rails guide lists methods such as `update_all`, `update_column` and `insert_all`, and states: "The following methods skip validations, and will save the object to the database regardless of its validity."[^5] Django says the same of the `update()` method of a query set: "It doesn’t run any save() methods on your models".[^6]

```python wrong id=bulk-update-skips-rule
import sqlite3


class Product:
    db = sqlite3.connect(":memory:")

    def __init__(self, name, stock, id=None):
        self.id, self.name, self.stock = id, name, stock

    def save(self):
        if self.stock < 0:  # the rule lives in save
            raise ValueError(f"{self.name}: stock cannot be negative")
        if self.id is None:
            self.id = self.db.execute("INSERT INTO products (name, stock) VALUES (?, ?)", (self.name, self.stock)).lastrowid
        else:
            self.db.execute("UPDATE products SET stock = ? WHERE id = ?", (self.stock, self.id))

    @classmethod
    def update_all(cls, change):
        # a fast bulk update: one statement, no objects, so save() never runs
        cls.db.execute("UPDATE products SET stock = stock + ?", (change,))


Product.db.execute("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, stock INTEGER)")
Product("pen", 3).save()
Product("ink", 40).save()
Product.update_all(-5)  # bug: the rule in save() is skipped
print(Product.db.execute("SELECT name, stock FROM products").fetchall())
```

```output
[('pen', -2), ('ink', 35)]
```

The stock of the pen is now negative. The fixes are a database constraint, for example `CHECK (stock >= 0)`, which holds for each write from any code, or a loop that loads each object and calls `save`, which is slower but runs the rule. Django recommends the loop when each `save` must run.[^6]

**The objects follow the tables.** The fields of the class are the columns, so the class has the shape of the schema. A domain model with inheritance, collections and objects that do not match tables one to one is hard to build in this way.[^7]

**Tests need the database.** The domain logic and the database access are in one object, so a test of a rule must load and save rows. Fowler's catalogue names this cost of database code in domain objects: "tests are slower to run because of all the database access."[^8]

## When not to use it

**When the domain logic is complex.** When the object model and the schema differ, use a Data Mapper, which keeps the domain objects free of database code.[^7] TypeORM's guide says that the Data Mapper approach "helps with maintainability, which is more effective in larger apps."[^4]

**When the code has no domain logic.** Objects that only hold columns and save them are a Row Data Gateway. A Table Data Gateway or plain queries are simpler for reports and batch work on many rows.

**When the important rules must hold for every write.** Put them in the database as constraints, and keep the checks in the record as a second line, because bulk writes skip the record.[^5]

## Variants and related patterns

**Row Data Gateway.** The same object without the domain logic. A row gateway that gets business methods becomes an Active Record.[^8] See [[row-data-gateway]].

**Data Mapper.** The domain objects know nothing of the database, and a separate mapper loads and saves them. With Data Mapper "the in-memory objects needn't know even that there's a database present".[^7] See [[data-mapper]].

**Table Data Gateway.** One object for the whole table, which returns plain rows. See [[table-data-gateway]].

**Repository.** A collection-like interface for domain objects. TypeORM offers both forms: an entity that extends `BaseEntity` is an Active Record, and its Data Mapper form uses repositories.[^4] See [[repository-pattern]].

## Known uses

- Ruby on Rails names its ORM Active Record. Its guide defines the pattern with Fowler's words, and its models get `save`, the finders and validations from the base class.[^3] In [persistence.rb at 8.1.4.1](https://github.com/rails/rails/blob/3f290b759c8e2ffe7c257951b1255740751355e4/activerecord/lib/active_record/persistence.rb#L923-L928), `create_or_update` calls `_create_record` for a new record and `_update_record` for a stored one.[^9]
- Django models follow the pattern. Its design philosophy says: "Models should encapsulate every aspect of an “object,” following Martin Fowler’s Active Record design pattern."[^10] In [base.py at 5.2](https://github.com/django/django/blob/9e7cc2b628fe8fd3895986af9b7fc9525034c1b0/django/db/models/base.py#L1081-L1175), `Model._save_table` tries an UPDATE of the row first when the object has a primary key, and runs an INSERT when the update changed no row.[^11]
- TypeORM, an ORM for TypeScript, supports Active Record entities: "Using the Active Record approach, you define all your query methods inside the model itself, and you save, remove, and load objects using model methods."[^4]

## References
