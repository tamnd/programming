---
tag: W7GEKN
kind: pattern
slug: lazy-load
name: Lazy Load
aliases: [lazy load pattern, lazy loading, deferred loading]
area: software-design/data-access-patterns
level: core
summary: >-
  A Lazy Load is an object that does not hold all of its data yet but knows how to get it, so a related object is read from the database only when the program first uses it.
family: architectural
polarity: positive
intent: "An object that doesn't contain all of the data you need but knows how to get it."
also_known_as: [deferred loading]
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern and its four varieties (lazy initialization, virtual proxy, value holder and ghost) in Patterns of Enterprise Application Architecture (2002).
see_also: [identity-map, unit-of-work, data-mapper, proxy-pattern, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/lazyLoad.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Lazy Load
    locator: pattern summary and the note on the four varieties
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 11, Object-Relational Behavioral Patterns, Lazy Load
  - url: https://docs.sqlalchemy.org/en/20/orm/queryguide/relationships.html
    title: SQLAlchemy 2.0 documentation, Relationship Loading Techniques
    locator: Summary of Relationship Loading Styles; Preventing unwanted lazy loads using raiseload
  - url: https://docs.sqlalchemy.org/en/20/glossary.html
    title: SQLAlchemy 2.0 documentation, Glossary
    locator: lazy load; N plus one problem
  - url: https://learn.microsoft.com/en-us/ef/core/querying/related-data/lazy
    title: Entity Framework Core documentation, Lazy Loading of Related Data
    locator: Lazy loading with proxies; Lazy loading without proxies
  - url: https://en.wikipedia.org/wiki/Lazy_loading
    title: Lazy loading, Wikipedia
    locator: Methods
  - url: https://docs.djangoproject.com/en/5.2/ref/models/querysets/
    title: Django 5.2 documentation, QuerySet API reference
    locator: select_related(); prefetch_related()
  - url: https://docs.python.org/3/library/functools.html
    title: The Python Standard Library, functools
    locator: functools.cached_property
  - url: https://pkg.go.dev/sync
    title: Go standard library, package sync
    locator: func OnceValue
  - url: https://doc.rust-lang.org/std/cell/struct.OnceCell.html
    title: Rust standard library, struct std::cell::OnceCell
    locator: the type description
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/strategies.py#L878-L995
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/strategies.py
    locator: LazyLoader._load_for_state, lines 878 to 995
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related_descriptors.py#L226-L275
    title: Django 5.2.8 source, django/db/models/fields/related_descriptors.py
    locator: ForwardManyToOneDescriptor.__get__, lines 226 to 275
---

## Intent

A Lazy Load is "An object that doesn't contain all of the data you need but knows how to get it."[^1] The program loads an object without its related objects. The first time the program asks for a related object, the Lazy Load reads it from the database.

## Problem

An order refers to a customer, the customer refers to an address and to other orders, and each of these refers to more objects. It is convenient to load the related objects together with the object of interest, because then the caller does not have to load each one explicitly. But Fowler warns that "if you take this to its logical conclusion, you reach the point where loading one object can have the effect of loading a huge number of related objects - something that hurts performance when only a few of the objects are actually needed."[^1]

The program must decide, for each association, when to read it. Loading everything at once wastes queries and memory. Making each caller load the related objects by hand puts database code into the domain logic.

## Solution

"A Lazy Load interrupts this loading process for the moment, leaving a marker in the object structure so that if the data is needed it can be loaded only when it is used."[^1] The object keeps the key of the related row, and a field or an object that stands for the related object. On first use, that stand-in runs the query and keeps the result.

```text
  client                 Order 10 (loaded)                  database
  order.total    ----->  total = 30, no SELECT
  order.customer ----->  _customer is None (the marker)
                           SELECT ... FROM customer WHERE id = 1  --->  (1, Ann, Oslo)
                           _customer = Customer(1, Ann, Oslo)
  order.customer ----->  _customer is set: return it, no SELECT
```

The participants are the **owner**, the object that the program loaded, such as an order; the **marker** or stand-in for the related object; and the **loader**, the code that knows the key and the query.

Fowler names four varieties:[^1][^2]

- **Lazy initialization.** It "uses a special marker value (usually null) to indicate a field isn't loaded. Every access to the field checks the field for the marker value and if unloaded, loads it."[^1]
- **Virtual proxy.** It "is an object with the same interface as the real object. The first time one of its methods are called it loads the real object and then delegates."[^1] See [[proxy-pattern]].
- **Value holder.** It "is an object with a getValue method. Clients call getValue to get the real object, the first call triggers the load."[^1]
- **Ghost.** It "is the real object without any data. The first time you call a method the ghost loads the full data into its fields."[^1]

The following program uses lazy initialization on SQLite. It traces each SQL statement. Finding the order runs one `SELECT`. Reading `order.total` runs none. The first read of `order.customer` runs one more, and the second read runs none.

```python run id=load-on-first-use
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text, city text);
create table orders (id integer primary key, customer_id integer, total integer);
insert into customer values (1, 'Ann', 'Oslo'), (2, 'Bo', 'Rome');
insert into orders values (10, 1, 30), (11, 2, 45), (12, 1, 20);
""")
selects = []
db.set_trace_callback(selects.append)


class Customer:
    def __init__(self, id, name, city):
        self.id, self.name, self.city = id, name, city


class Order:
    def __init__(self, id, customer_id, total):
        self.id, self.customer_id, self.total = id, customer_id, total
        self._customer = None  # The marker: the customer is not loaded yet.

    @property
    def customer(self):
        if self._customer is None:
            sql = "select id, name, city from customer where id = ?"
            self._customer = Customer(*db.execute(sql, (self.customer_id,)).fetchone())
        return self._customer


def find_order(id):
    sql = "select id, customer_id, total from orders where id = ?"
    return Order(*db.execute(sql, (id,)).fetchone())


order = find_order(10)
print("find_order(10) | selects:", len(selects))
print("total:", order.total, "| selects:", len(selects))
print("customer:", order.customer.name, "| selects:", len(selects))
print("again:", order.customer.city, "| selects:", len(selects))
```

```output
find_order(10) | selects: 1
total: 30 | selects: 1
customer: Ann | selects: 2
again: Oslo | selects: 2
```

## Example

Each example file loads the customer of an order in the four ways that Fowler names: lazy initialization, a virtual proxy, a value holder and a ghost. Each line prints the number of `SELECT` statements before and after the first use, and after a second use. The proxy and the ghost answer the key without a query, because they hold it from the start. The last two lines show the cost of a Lazy Load in a loop: three orders and their customers take four statements with lazy loading and one statement with a join. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the tables in memory and count each call that stands for a `SELECT`.

## Consequences

**Benefits.**

- **Less work for data that is not used.** As Fowler puts it, "if you're lazy about doing things you'll win when it turns out you don't need to do them at all."[^1] A screen that shows only orders never reads their customers.
- **The domain code stays simple.** The caller writes `order.customer` and does not know whether the customer is in memory. In SQLAlchemy, "Lazy loading is the default loading style for all relationship() constructs that don’t otherwise indicate the relationship."[^3]
- **The load can be delayed or cut off.** Lazy loading lets an ORM reduce "the complexity and time spent within object fetches", because "attributes for related tables don’t need to be addressed immediately."[^4]

**Liabilities.**

**The N plus one problem.** A loop over N objects that uses a lazy association on each one runs one query for the list and one query for each object: "for any N objects loaded, accessing their lazy-loaded attributes means there will be N+1 SELECT statements emitted."[^3] Entity Framework Core warns that "Lazy loading can cause unneeded extra database roundtrips to occur (the so-called N+1 problem), and care should be taken to avoid this."[^5]

```python wrong id=n-plus-one
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text);
create table orders (id integer primary key, customer_id integer);
insert into customer values (1, 'Ann'), (2, 'Bo');
insert into orders values (10, 1), (11, 2), (12, 1);
""")
selects = []
db.set_trace_callback(selects.append)


class Order:
    def __init__(self, id, customer_id):
        self.id, self.customer_id, self._name = id, customer_id, None

    @property
    def customer_name(self):  # A lazy load: one SELECT on first use.
        if self._name is None:
            sql = "select name from customer where id = ?"
            (self._name,) = db.execute(sql, (self.customer_id,)).fetchone()
        return self._name


orders = [Order(*row) for row in db.execute("select id, customer_id from orders")]
print([o.customer_name for o in orders], "| selects:", len(selects))
```

```output
['Ann', 'Bo', 'Ann'] | selects: 4
```

**Loading after the session is closed.** The loader needs a database connection. In SQLAlchemy, a lazy load uses the Session of the object; "If the object is in the detached state and not associated with any Session , this is considered to be an error state and an informative exception is raised."[^4] An object that leaves its session, for example to be rendered after the request ends, cannot load its associations.

**A marker that is also a value.** Lazy initialization uses null as the marker. Wikipedia notes that "if null is a legitimate return value, it may be necessary to use a placeholder object to signal that it has not been initialized."[^6] A customer that is really absent then looks unloaded, and each access runs the query again. The value holder in the examples keeps a separate `loaded` flag for this reason.

**Threads.** "If this method is used in a multithreaded application , synchronization must be used to avoid race conditions ."[^6] Two threads that find the marker at the same time both run the query, and each may keep a different object.

**Identity.** A proxy is a different object from the real one, so a test such as `proxy is customer` is false even for the same row. A Lazy Load that does not go through the [[identity-map]] can also load a second copy of a row that is already in memory.

## When not to use it

**When the caller always needs the related data.** If every use of an order reads its customer, the Lazy Load only adds a second query. Load both with a join. Django's `select_related` "works by creating an SQL join and including the fields of the related object in the SELECT statement."[^7]

**In loops over many objects.** Load the association for the whole list in one or two queries. SQLAlchemy's select IN loading "assembles the primary key identifiers of the parent objects into an IN clause, so that all members of related collections / scalar references are loaded at once by primary key."[^3] Django's `prefetch_related` retrieves the related objects "in a single batch".[^7]

**When the object leaves its session.** Data that is sent to a template, a cache or another thread after the database work ends must be loaded before that point.

## How to fix it

To find unplanned lazy loads, make them fail. SQLAlchemy's raise loading "is triggered at the same time a lazy load would normally occur, except it raises an ORM exception in order to guard against the application making unwanted lazy loads."[^3] Then load what each use case needs with a join or a batch query.

## Variants and related patterns

**Eager loading.** The opposite strategy loads the related objects with the owner. SQLAlchemy's joined loading "applies a JOIN to the given SELECT statement so that related rows are loaded in the same result set."[^3] "Eager loading is the opposite of lazy loading ."[^4]

**Proxies made by the framework.** Entity Framework Core can generate virtual proxies. With `UseLazyLoadingProxies`, it enables "lazy loading for any navigation property that can be overridden--that is, it must be virtual and on a class that can be inherited from." Without proxies, an entity can receive an `ILazyLoader` service, or a delegate, and call it in the getter of the navigation property.[^5]

**Lazy values in the standard libraries.** Some languages have a lazy initialization helper. Python's `functools.cached_property` turns a method into "a property whose value is computed once and then cached as a normal attribute for the life of the instance."[^8] Go's `sync.OnceValue` "returns a function that invokes f only once and returns the value returned by f."[^9] Rust's `OnceCell` is "A cell which can nominally be written to only once."[^10] These helpers give the marker and the check, and the program supplies the query.

**Identity Map.** A Lazy Load of a single object by its key can look in the identity map first and skip the query. See [[identity-map]].

**Proxy.** The virtual proxy is the Proxy pattern used to delay the creation of an expensive object. See [[proxy-pattern]].

## Known uses

- SQLAlchemy: in [LazyLoader._load_for_state in strategies.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/strategies.py#L878-L995), a load by primary key first calls `session._identity_lookup`; the comment says it will "check the identity map without generating a Query at all". Only if the object is not there does it call `_emit_lazyload`.[^11]
- Django: [ForwardManyToOneDescriptor.__get__ in related_descriptors.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related_descriptors.py#L226-L275) implements `child.parent`. The comment says: "The related instance is loaded from the database and then cached by the field on the model instance state."[^12] This is lazy initialization with the field cache as the marker.
- Entity Framework Core, for .NET: lazy loading with proxies, or with an injected `ILazyLoader`.[^5]

## References
