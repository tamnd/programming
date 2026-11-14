---
tag: 413M3R
kind: concept
slug: object-relational-impedance-mismatch
name: Object-relational impedance mismatch
aliases: [impedance mismatch, O/R impedance mismatch, object/relational mismatch]
area: software-design/data-access-patterns
level: core
summary: >-
  The set of differences between objects in memory and rows in relational tables, in granularity, subtypes, identity, associations and navigation, that make the mapping between them hard.
see_also: [identity-map, unit-of-work, data-mapper, active-record, row-data-gateway, repository-pattern]
same_as:
  wikidata: Q1600611
  wikipedia: Object–relational_impedance_mismatch
sources:
  - url: https://hibernate.org/orm/what-is-an-orm/
    title: Hibernate ORM, What is Object/Relational Mapping?
    locator: The object/relational mismatch; Where are we today?
  - url: https://agiledata.org/essays/impedancemismatch.html
    title: Scott W. Ambler, Overcoming The Object-Relational Impedance Mismatch, Agile Data
    locator: introduction; sections 1 and 3
  - url: https://en.wikipedia.org/wiki/Object%E2%80%93relational_impedance_mismatch
    title: Object–relational impedance mismatch, Wikipedia
    locator: introduction; Mismatches; Solving impedance mismatch; Contention
  - url: https://martinfowler.com/bliki/OrmHate.html
    title: Martin Fowler, OrmHate, 8 May 2012
    locator: the whole post and its notes
  - url: https://docs.python.org/3/reference/datamodel.html
    title: The Python Language Reference, Data model
    locator: object.__eq__ (rich comparison methods); object.__hash__
  - url: https://docs.sqlalchemy.org/en/20/orm/session_basics.html
    title: SQLAlchemy 2.0 documentation, Session Basics
    locator: Get by Primary Key; Expiring / Refreshing
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/lang/Object.html
    title: Java SE 21 API, class java.lang.Object
    locator: method equals
  - url: https://go.dev/ref/spec
    title: The Go Programming Language Specification
    locator: Comparison operators
  - url: https://go.dev/doc/faq
    title: Go FAQ
    locator: Is Go an object-oriented language?
  - url: https://learn.microsoft.com/en-us/ef/core/change-tracking/identity-resolution
    title: Entity Framework Core documentation, Identity Resolution in EF Core
    locator: introduction
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/reference/working-with-objects.html
    title: Doctrine ORM 3.7 documentation, Working with Objects
    locator: Entities and the Identity Map
  - url: https://docs.sqlalchemy.org/en/20/glossary.html
    title: SQLAlchemy 2.0 documentation, Glossary
    locator: lazy load; N plus one problem
  - url: https://docs.python.org/3/library/stdtypes.html
    title: The Python Standard Library, Built-in Types
    locator: Numeric Types
  - url: https://www.sqlite.org/datatype3.html
    title: SQLite, Datatypes In SQLite
    locator: section 2, Storage Classes and Datatypes
---

## Idea

A program keeps its data as objects in memory. A relational database keeps its data as rows in tables. The two models do not fit together exactly, and the name for the differences is the object-relational impedance mismatch. The Hibernate documentation defines it: "The object/relational impedance mismatch refers to differences in how entities are represented in the relational data model, compared to how they’re represented in an object-oriented programming language."[^1] It continues: "The relational model represents data in a tabular format (quite similar to a spreadsheet), whereas object-oriented languages like Java represent data as an interconnected graph of objects with inheritance and polymorphism."[^1]

Scott Ambler gives the difference in access as the clearest sign: "With the object paradigm you traverse objects via their relationships whereas with the relational paradigm you join the data rows of tables."[^2] The program below shows two of the differences. It loads customer 1 two times. The table has one row with that key, but the program now has two objects, and `==` does not find them equal. Then it gets the order total of each customer in two ways. The object code walks from each customer to its orders, and the relational code uses one join.

```python run id=walk-or-join
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table customer (id integer primary key, name text);
create table orders (id integer primary key, customer_id integer, total integer);
insert into customer values (1, 'Ann'), (2, 'Bo');
insert into orders values (10, 1, 30), (11, 1, 20), (12, 2, 45);
""")
statements = []
db.set_trace_callback(statements.append)


class Customer:
    def __init__(self, id, name, orders):
        self.id, self.name, self.orders = id, name, orders


def load(id):
    (name,) = db.execute("select name from customer where id = ?", (id,)).fetchone()
    rows = db.execute("select total from orders where customer_id = ?", (id,))
    return Customer(id, name, [total for (total,) in rows])


# Identity: the table has one row with key 1, and the program has two objects.
a, b = load(1), load(1)
print("a is b:", a is b, "| a == b:", a == b, "| a.id == b.id:", a.id == b.id)

# Navigation: walk from each customer object to its orders.
statements.clear()
ids = [id for (id,) in db.execute("select id from customer order by id")]
walk = {c.name: sum(c.orders) for c in map(load, ids)}
print("walk:", walk, "| statements:", len(statements))

# The relational way: one join does the same work.
statements.clear()
join = db.execute(
    "select name, sum(total) from customer"
    " join orders on orders.customer_id = customer.id"
    " group by customer.id order by customer.id"
).fetchall()
print("join:", dict(join), "| statements:", len(statements))
```

```output
a is b: False | a == b: False | a.id == b.id: True
walk: {'Ann': 50, 'Bo': 45} | statements: 5
join: {'Ann': 50, 'Bo': 45} | statements: 1
```

The walk sends one statement for the keys and two statements for each customer. The join sends one statement for all of the customers.

## How it works

The mismatch is not one problem. The Hibernate documentation divides it into five problems that occur when a program writes or reads "a graph of objects to or from a relational database".[^1]

- **Granularity.** "Often we find that an object model has more classes than there are tables in the database."[^1] An address can be a class of its own, but in the database it is often three columns of the customer table.
- **Subtypes.** "The relational model doesn’t have inheritance as a first-class construct, but relational data modelling does have standard ways to represent subtyping."[^1] One way is a column that tells which subclass a row is, as in the examples of this page.
- **Identity.** In the relational model, the primary key is the only test of sameness. Java "defines both object identity a==b and value equality", and "it’s common to have more than one object in a given Java program representing the same row of a database table."[^1]
- **Associations.** "Associations are represented as unidirectional references in object-oriented languages, whereas relational databases represent associations via foreign keys." Also, "to represent a bidirectional relationship in Java, we must write code on both sides of the association."[^1] One foreign key becomes two references, and the program must keep both of them correct.
- **Data navigation.** "In Java, we navigate associations from one object to the next, walking the object network." For relational data, that is "an extremely inefficient way of retrieving relational data, where, instead, we must control latency by minimizing the number of SQL queries, loading related entity data using joins."[^1]

Other sources add more differences. Wikipedia lists data types, encapsulation, constraints and transactions. In the relational model, "Links are reversible (INNER JOIN is symmetric to follow foreign keys backward), forming undirected graphs".[^3] The two sides also disagree about which copy of the data is the real one: "Relational says the DBMS is authoritative and the OO program's objects are temporary copies (possibly outdated if the database is modified concurrently). OO says the objects are authoritative, and the DBMS is just for persistence."[^3]

Ambler gives the root cause. The object-oriented paradigm "is based on proven software engineering principles", and the relational paradigm "is based on proven mathematical principles".[^2] He also names a second, cultural mismatch between developers and data professionals, apart from the technical one.[^2]

Martin Fowler adds that the problem is not about objects. "By rights it should be referred to as in-memory/relational mapping problem, because it's true of mapping RDBMSs to any in-memory data structure."[^4] He gives two more causes of difficulty: "you can make changes on either side that have to be mapped to the other", and many users can change the database at the same time, but "in most cases, you can't hold transactions open while you fiddle with the data in-memory."[^4]

## In languages

The identity problem looks different in each language, because each language has its own default test of whether two objects are the same.

- **Python.** User-defined classes inherit `__eq__` from `object`, "with them, all objects compare unequal (except with themselves)".[^5] So two objects for the same row are not equal unless the class defines `__eq__`. The SQLAlchemy session keeps "a unique Python object instance maintained corresponding to a particular database identity."[^6]
- **Java.** The `equals` method of `Object` "returns true if and only if x and y refer to the same object".[^7] So two objects for the same row are not equal unless the class overrides `equals`.
- **Go.** "Two struct values are equal if their corresponding non-blank field values are equal", and "Two pointer values are equal if they point to the same variable".[^8] The program chooses value equality or identity by the choice of a struct or a pointer. Go has "no type hierarchy",[^9] so the subtypes of a table become types that implement one interface.
- **C#.** In Entity Framework Core, "A DbContext can only track one entity instance with any given primary key value."[^10]
- **PHP.** Doctrine "will only ever give you access to one instance of the Article with ID 1234, no matter how often do you retrieve it from the EntityManager".[^11]

The Python, Java and Go examples of this page show all five problems with one customer table and one orders table.

## Variations and related ideas

- **Mapping layers.** The data access patterns each solve a part of the problem. An [[identity-map|Identity map]] keeps one object for each row. A [[unit-of-work|Unit of work]] collects the changes and writes them in one transaction. A [[data-mapper|Data mapper]] keeps the objects and the tables independent, while an [[active-record|Active record]] puts the mapping into the object. Lazy loading delays the load of an association until the program uses it.[^12]
- **A more relational model in memory.** Fowler: "you either have to make your in-memory model more relational, or you complicate your mapping code." In many cases he suggests "Row Data Gateways that are just the relational tables wrapped in simple objects."[^4] See [[row-data-gateway|Row data gateway]].
- **Avoiding the problem.** "Either you use the relational model in memory, or you don't use it in the database."[^4] A NoSQL database helps "If you have an application problem that maps well to a NoSQL data model - such as aggregates or graphs", but "it only works when the fit between the application model and the NoSQL data model is good."[^4]
- **One-way mapping.** "ORMs are complex because they have to handle a bi-directional mapping. A uni-directional problem is much easier to work with".[^4] Fowler gives this as one of the arguments for CQRS.[^4]
- **Functional-relational mapping.** "Comprehensions in functional programming languages are isomorphic with relational queries."[^3] Some functional languages use this to avoid the object mapping.[^3]
- **The relational view.** Christopher J. Date "says a true relational DBMS overcomes the problem", because in his view domains and classes are equivalent.[^3]

## Pitfalls

- **The N plus one problem.** Navigation by objects with lazy loading sends one query for the parents and one more for each parent: "for a result set of N parent objects, there will be N + 1 SELECT statements emitted."[^12] The Idea example sends 5 statements for 2 customers. The join sends 1.
- **One side of an association.** If the program changes the reference on one object only, the objects disagree. In the examples, an order moves to another customer, but the old customer still lists it.
- **Two objects for one row.** Two loads of one row give two objects. A change to one of them does not show in the other, and the second save can write old values back. See [[identity-map|Identity map]].
- **Types that do not match.** Wikipedia notes that "Scalar types differ between them, impeding mapping."[^3] A Python int has no size limit: "Integers have unlimited precision."[^13] A SQLite INTEGER "is a signed integer, stored in 0, 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value."[^14] A value that fits in the object does not fit in the column.

```python wrong id=int-too-large
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table counter (id integer primary key, hits integer)")
hits = 2**63  # A Python int has no fixed size.
try:
    db.execute("insert into counter values (1, ?)", (hits,))
    print("stored", hits)
except OverflowError as e:
    print("OverflowError:", e)
```

```output
OverflowError: Python int too large to convert to SQLite INTEGER
```

- **Inflated expectations.** Fowler: "the ORM can handle about 80-90% of the mapping problems, but that last chunk always needs careful work by somebody who really understands how a relational database works."[^4]

## Cost

The mapping costs code and round trips. Fowler: "Mapping to a relational database involves lots of repetitive, boiler-plate code."[^4] A tool removes most of that code, but adds complexity, and "often systems using an ORM perform badly - often due to naive interactions with the underlying database."[^4] Navigation by objects costs one round trip to the database for each step, and a join costs one round trip for the full result. The examples count these statements.

## History

Ambler writes: "In the early 1990s, the differences between the two approaches were labelled the “object-relational impedance mismatch”, or simply “impedance mismatch” for short."[^2] The name comes from electrical engineering: "The term impedance mismatch comes from impedance matching in electrical engineering ."[^3]

Fowler remembers the 1990s: "For much of the 90's I saw project after project deal with the object/relational mapping problem by writing their own framework - it was always much tougher than people imagined."[^4] In 2006, Ted Neward published the essay The Vietnam of Computer Science about the problem.[^3] Fowler wrote that he sympathizes "greatly with Ted Neward's famous quote that object-relational mapping is the Vietnam of Computer Science".[^4] Open source tools changed the work: "The widely available open source ORMs (such as iBatis, Hibernate, and Active Record) did a great deal to remove this problem".[^4] The Hibernate documentation now says that the mapping "is a difficult and subtle problem, and used to be a major source of anguish for enterprise developers."[^1]

## References
