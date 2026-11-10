---
tag: NEW-repository-pattern
kind: pattern
slug: repository-pattern
name: Repository pattern
aliases: [Repository]
area: software-design/architecture
level: core
summary: >-
  A repository gives the domain code a collection-like interface to stored objects, such as add, get and find by criteria, and hides the database queries and the mapping behind it.
family: architectural
polarity: positive
intent: "Mediate between the domain and the data mapping layers using a collection-like interface for accessing domain objects."
also_known_as: []
history:
  year: 2002
  people: [Edward Hieatt, Rob Mee, Martin Fowler, Eric Evans]
  note: Fowler's Patterns of Enterprise Application Architecture (2002) includes Repository, written by Edward Hieatt and Rob Mee. Evans's Domain-Driven Design (2003) made it one of the building blocks of a domain model, tied to aggregates.
see_also: [dependency-injection, facade-pattern, proxy-pattern, iterator-pattern, sql-injection, unit-testing, interfaces]
sources:
  - url: https://martinfowler.com/eaaCatalog/repository.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Repository (by Edward Hieatt and Rob Mee)
    locator: pattern summary; the book's Chapter 13
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 13, Repository; Chapter 10, Data Mapper and Active Record; Chapter 11, Unit of Work
  - url: https://martinfowler.com/eaaCatalog/dataMapper.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Data Mapper
    locator: pattern summary; the book's Chapter 10
  - url: https://martinfowler.com/eaaCatalog/activeRecord.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Active Record
    locator: pattern summary; the book's Chapter 10
  - url: https://martinfowler.com/eaaCatalog/unitOfWork.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Unit of Work
    locator: pattern summary; the book's Chapter 11
  - book: isbn:9780321125217
    title: Eric Evans, Domain-Driven Design, Tackling Complexity in the Heart of Software, Addison-Wesley, 2003
    locator: Part II, The Building Blocks of a Model-Driven Design; Chapter 6, The Life Cycle of a Domain Object, Repositories
  - url: https://www.domainlanguage.com/wp-content/uploads/2016/05/DDD_Reference_2015-03.pdf
    title: Eric Evans, Domain-Driven Design Reference, Definitions and Pattern Summaries, 2015 (CC BY 4.0)
    locator: Part II, Building Blocks of a Model-Driven Design, Repositories
  - url: https://learn.microsoft.com/en-us/dotnet/api/microsoft.entityframeworkcore.dbcontext
    title: Entity Framework Core API, DbContext Class
    locator: class remarks
  - url: https://learn.microsoft.com/en-us/ef/core/testing/choosing-a-testing-strategy#repository-pattern
    title: Entity Framework Core, Choosing a testing strategy
    locator: Repository pattern
  - url: https://docs.djangoproject.com/en/5.2/topics/db/managers/#managers
    title: Django 5.2 documentation, Managers
    locator: Managers, first paragraph
  - url: https://docs.spring.io/spring-data/commons/reference/repositories/core-concepts.html
    title: Spring Data Commons reference, Core concepts
    locator: first section
  - url: https://docs.python.org/3.14/library/sqlite3.html#sqlite3-placeholders
    title: The Python Standard Library, sqlite3, Python 3.14
    locator: How to use placeholders to bind values in SQL queries
  - url: https://github.com/spring-projects/spring-data-commons/blob/3.4.0/src/main/java/org/springframework/data/repository/CrudRepository.java#L22-L66
    title: Spring Data Commons 3.4.0 source, CrudRepository.java
    locator: interface CrudRepository, lines 22 to 66
  - url: https://github.com/django/django/blob/5.2/django/db/models/manager.py#L150-L177
    title: Django 5.2 source, django/db/models/manager.py
    locator: BaseManager.get_queryset and class Manager, lines 150 to 177
---

## Intent

Mediate between the domain and the data mapping layers using a collection-like interface for accessing domain objects.[^1] The domain code asks a repository for orders as if the orders were in a collection in memory. The repository turns each request into a query, and turns the rows into objects.

## Problem

An order service ships orders, lists the orders of a customer, and cancels orders that are not paid. The first version writes SQL in each method of the service. After a year, the query for "the open orders of a customer" exists in six places, each with small differences. The service cannot be tested without a database. And a change to the tables touches the business rules.

The forces are these. The business rules should read like the business: get an order, ship it, store it. The queries should be in one place, because each query encodes a decision about the schema. The code that applies the rules should be testable without a database. And the domain objects should not know how they are stored.

Fowler's catalog describes the setting: a system with a complex domain model benefits from a layer, such as Data Mapper, that keeps the domain objects separate from the database access code. In such a system, it can be worth adding another layer of abstraction over the mapping layer, where the query construction code is concentrated.[^1][^3]

## Solution

Define, for each kind of object that the domain code must find, an interface that looks like a collection: add an object, remove it, get it by identity, find the objects that match a criterion. Write one implementation over the database and, when tests need one, another in memory. The domain code depends on the interface only.

```text
  domain / application code
      ship_order(orders, 1)
            |
            v
  OrderRepository (interface, in the language of the domain)
      add(order)   get(id)   by_customer(name)   update(order)
            ^                         ^
            |                         |
  SqliteOrderRepository       InMemoryOrderRepository
  SQL, rows -> Order          dict of copies
            |
            v
         database
```

Evans states the pattern for domain-driven design. For each type of aggregate that needs global access, create a service that can provide "the illusion of an in-memory collection of all objects of that aggregate's root type".[^7] It has methods to add and remove objects, and methods that select objects by criteria that mean something to domain experts. Evans limits repositories to the roots of aggregates that need direct access: an order line is reached through its order, so there is no repository for order lines.[^7][^6]

The repository is not the mapper. In Fowler's catalog, Repository sits over a Data Mapper, the layer that moves data between objects and tables.[^1][^3] An ORM usually provides the mapper. The repository adds the vocabulary of the domain: `by_customer`, `overdue`, `awaiting_payment`.

The following program runs one function, `ship_order`, against two implementations of the same interface. One stores rows in SQLite. The other keeps copies in a dictionary. The SQL uses placeholders, never string formatting, so values from users cannot change the query.[^12] See [[sql-injection]].

```python run id=two-stores
import sqlite3
from dataclasses import dataclass, replace
from typing import Protocol


@dataclass
class Order:
    id: int
    customer: str
    total: int
    status: str = "open"

    def ship(self) -> None:
        if self.status != "open":
            raise ValueError(f"order {self.id} is {self.status}")
        self.status = "shipped"


class OrderRepository(Protocol):
    def add(self, order: Order) -> None: ...
    def get(self, order_id: int) -> Order | None: ...
    def by_customer(self, customer: str) -> list[Order]: ...
    def update(self, order: Order) -> None: ...


class SqliteOrderRepository:
    def __init__(self, db: sqlite3.Connection) -> None:
        self._db = db
        db.execute("CREATE TABLE orders (id INTEGER PRIMARY KEY, customer TEXT, total INTEGER, status TEXT)")

    def add(self, order: Order) -> None:
        self._db.execute("INSERT INTO orders VALUES (?, ?, ?, ?)", (order.id, order.customer, order.total, order.status))

    def get(self, order_id: int) -> Order | None:
        row = self._db.execute("SELECT id, customer, total, status FROM orders WHERE id = ?", (order_id,)).fetchone()
        return Order(*row) if row else None

    def by_customer(self, customer: str) -> list[Order]:
        rows = self._db.execute("SELECT id, customer, total, status FROM orders WHERE customer = ? ORDER BY id", (customer,))
        return [Order(*row) for row in rows]

    def update(self, order: Order) -> None:
        self._db.execute("UPDATE orders SET customer = ?, total = ?, status = ? WHERE id = ?", (order.customer, order.total, order.status, order.id))


class InMemoryOrderRepository:
    def __init__(self) -> None:
        self._rows: dict[int, Order] = {}

    def add(self, order: Order) -> None:
        self._rows[order.id] = replace(order)

    def get(self, order_id: int) -> Order | None:
        row = self._rows.get(order_id)
        return replace(row) if row else None

    def by_customer(self, customer: str) -> list[Order]:
        return [replace(o) for _, o in sorted(self._rows.items()) if o.customer == customer]

    def update(self, order: Order) -> None:
        self._rows[order.id] = replace(order)


def ship_order(orders: OrderRepository, order_id: int) -> str:
    order = orders.get(order_id)
    if order is None:
        return f"order {order_id}: not found"
    order.ship()
    orders.update(order)
    return f"order {order_id} is {order.status}"


for name, repo in [("sqlite", SqliteOrderRepository(sqlite3.connect(":memory:"))), ("memory", InMemoryOrderRepository())]:
    for order in [Order(1, "ana", 30), Order(2, "bo", 12), Order(3, "ana", 5)]:
        repo.add(order)
    shipped = ship_order(repo, 1)
    ana = [(o.id, o.status) for o in repo.by_customer("ana")]
    print(f"{name}: {shipped}; ana: {ana}; {ship_order(repo, 9)}")
```

```output
sqlite: order 1 is shipped; ana: [(1, 'shipped'), (3, 'open')]; order 9: not found
memory: order 1 is shipped; ana: [(1, 'shipped'), (3, 'open')]; order 9: not found
```

## Example

Each example file defines an `Order` with a `ship` method, an order repository with `add`, `get`, `by_customer` and `update`, and an in-memory implementation. The function `ship_order` gets an order, ships it, and stores it, and it knows only the interface. Each implementation returns copies, or immutable values, so a change reaches storage only through `update`, as it does with a database. The Python, JavaScript, Go, Rust and C++ files use a mutable order. The Java and C# files use an immutable record whose `ship` returns a changed copy.

The examples stay in memory because the standard libraries of most of these languages have no database. The Python block above shows the same interface over SQLite.

## Consequences

**Benefits.**

- **The queries are in one place.** The query for "the orders of a customer" exists once, in the repository, and has a name.[^1]
- **The domain code reads like the domain.** `orders.by_customer("ana")` states the intent. The SQL is behind it.
- **The domain does not depend on storage.** The dependency runs one way: the mapping layer knows the domain, and the domain knows only the interface.[^1] With [[dependency-injection]], the application receives a repository and does not choose it.
- **A test can use a fake.** The application logic runs against an in-memory implementation, fast and with no database.

**Liabilities.**

**A fake is not the database.** An in-memory repository does not run the constraints, the transactions or the collation of the real database. The Entity Framework Core documentation recommends a repository layer for a reliable test double. In the same section, it says that the layer has a significant cost to build and maintain, and that tests against the real database are still likely to be needed for the queries in the repository.[^9] It also warns that SQLite and other substitutes behave differently from the production database, for example in case sensitivity.[^9]

A subtler gap is object identity. An in-memory fake that returns the stored object, not a copy, makes a missing `update` call invisible:

```python wrong id=live-objects
import sqlite3
from dataclasses import dataclass


@dataclass
class Order:
    id: int
    status: str = "open"


class FakeOrders:
    def __init__(self):
        self.rows = {}

    def add(self, order):
        self.rows[order.id] = order

    def get(self, order_id):
        return self.rows[order_id]  # the stored object itself

    def update(self, order):
        self.rows[order.id] = order


class SqliteOrders:
    def __init__(self):
        self.db = sqlite3.connect(":memory:")
        self.db.execute("CREATE TABLE orders (id INTEGER PRIMARY KEY, status TEXT)")

    def add(self, order):
        self.db.execute("INSERT INTO orders VALUES (?, ?)", (order.id, order.status))

    def get(self, order_id):
        return Order(*self.db.execute("SELECT id, status FROM orders WHERE id = ?", (order_id,)).fetchone())

    def update(self, order):
        self.db.execute("UPDATE orders SET status = ? WHERE id = ?", (order.status, order.id))


def ship_order(orders, order_id):
    order = orders.get(order_id)
    order.status = "shipped"
    # bug: no orders.update(order)


for repo in [FakeOrders(), SqliteOrders()]:
    repo.add(Order(1))
    ship_order(repo, 1)
    print(f"{type(repo).__name__}: {repo.get(1).status}")
```

```output
FakeOrders: shipped
SqliteOrders: open
```

The test with the fake passes, and production loses the change. The fakes in the example files return copies for this reason. The other answer is a **Unit of Work**, which tracks the objects that a business transaction has read and changed, and writes the changes at the end.[^5] With a unit of work, the repository can behave like a real collection with no `update` method, and both the fake and the real store must then track changes.

**Collection semantics leak.** A repository looks like a collection, but each call can be a round trip to a database. A loop that calls `get` for each of a thousand ids makes a thousand queries. The interface must offer the queries that the domain needs in bulk.

**The interface can become a second query language.** A generic repository with a `find(predicate)` method, or one that returns a query object that callers can extend, moves the queries back into the callers. The EF Core documentation says that a repository used as a test double must return `IEnumerable`, not `IQueryable`, because the query operators that a caller adds to an `IQueryable` cannot be stubbed out.[^9]

## When not to use it

**Over an ORM that already gives the collection.** EF Core describes `DbContext` as a combination of the Unit of Work and Repository patterns.[^8] Django gives each model a `Manager`, "the interface through which database query operations are provided to Django models".[^10] Spring Data generates repository implementations from interfaces.[^11] A class `OrderRepository` whose methods each call one method of `DbSet<Order>` adds a layer, a test double and a place for bugs, and it hides nothing. Write your own repository over an ORM when it adds something: names from the domain for the queries, a boundary that keeps ORM types out of the domain layer, or a seam for tests that need no database.

**With Active Record.** In Active Record, an object wraps a row of a table and carries both the data access and the domain logic.[^4] The class is its own repository: `Order.find(1)`. A repository on top of it fights the design of the framework. Choose one or the other.

**In a small application with a thin domain.** An application that reads a form and writes a row has little domain logic to protect. A query in the request handler is easier to follow than three layers.

**As a generic repository for every type.** An interface `Repository<T>` with `get`, `add`, `find(predicate)` for every table gives no names from the domain and lets callers build any query. Evans's version has one repository for each aggregate root, with methods for the queries that the domain needs.[^7]

## Variants and related patterns

**Data Mapper.** The layer of mappers that moves data between objects and the database while it keeps them independent of each other.[^3] A repository sits on top of it and gives it a collection-like interface.[^1]

**Unit of Work.** Tracks the changed objects of a business transaction and writes them together.[^5] A repository gives access to objects. A unit of work decides when the changes reach the database. `DbContext` combines the two.[^8]

**Active Record.** The alternative design, where the domain object loads and saves itself.[^4]

**Facade.** A repository is a facade over the mapper, the connection and the query language, with an interface in the terms of the domain. See [[facade-pattern]].

**Proxy.** Evans allows a repository to return proxies that give the illusion of fully loaded aggregates and load them lazily.[^7] See [[proxy-pattern]].

**Dependency injection.** The application receives a repository through its constructor, so a test can pass the in-memory implementation. See [[dependency-injection]].

**Iterator.** A query that returns many objects can return an iterator that fetches rows in batches, instead of a list. See [[iterator-pattern]].

## Known uses

- Spring Data: [CrudRepository.java at 3.4.0](https://github.com/spring-projects/spring-data-commons/blob/3.4.0/src/main/java/org/springframework/data/repository/CrudRepository.java#L22-L66) is the generic interface for create, read, update and delete operations on a repository for one type, with `save` and `findById`. Applications extend it with their own interface, and the library generates the implementation.[^13][^11]
- Django: `BaseManager.get_queryset` in [manager.py at 5.2](https://github.com/django/django/blob/5.2/django/db/models/manager.py#L150-L177) returns a `QuerySet` for the model, and the docstring says that subclasses can override it to change the behaviour of the manager. A custom manager with named query methods is the Django form of a repository.[^14][^10]

## References
