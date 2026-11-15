---
tag: TAJHC9
kind: pattern
slug: data-access-object
name: Data access object
aliases: [DAO, data access object pattern, DAO pattern]
area: software-design/data-access-patterns
level: core
summary: >-
  A data access object puts all access to one kind of stored data behind an interface of persistence operations, so that business code does not depend on the storage.
family: architectural
polarity: positive
intent: "Abstract and encapsulate all access to the data source behind an interface."
also_known_as: [DAO]
history:
  year: 2001
  people: [Deepak Alur, John Crupi, Dan Malks]
  note: Alur, Crupi and Malks described the pattern in Core J2EE Patterns (2001), for Java enterprise applications that used JDBC, entity beans and other data sources. The name is still common in Java, Spring and Android code.
see_also: [repository-pattern, table-data-gateway, data-mapper, abstract-factory-pattern, factory-method-pattern, program-to-an-interface, dependency-injection, separation-of-concerns]
sources:
  - url: https://www.oracle.com/java/technologies/dataaccessobject.html
    title: Core J2EE Patterns, Data Access Object
    locator: Context; Problem; Forces; Solution; Participants and Responsibilities; Strategies; Consequences; Related Patterns
  - url: https://docs.spring.io/spring-framework/reference/data-access/dao.html
    title: Spring Framework reference, DAO Support
    locator: introduction; Consistent Exception Hierarchy
  - url: https://developer.android.com/training/data-storage/room/accessing-data
    title: Android Developers, Access data using Room DAOs
    locator: introduction; Anatomy of a DAO
  - url: https://martinfowler.com/eaaCatalog/repository.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Repository
    locator: pattern summary
  - url: https://github.com/spring-projects/spring-framework/blob/e354390837e62c77a7ac386960df33fb357724b8/spring-context/src/main/java/org/springframework/stereotype/Repository.java#L27-L36
    title: Spring Framework 6.2.12 source, spring-context/src/main/java/org/springframework/stereotype/Repository.java
    locator: Javadoc of the Repository annotation, lines 27 to 36
---

## Intent

The Core J2EE pattern states the solution in one line: "Use a Data Access Object (DAO) to abstract and encapsulate all access to the data source. The DAO manages the connection with the data source to obtain and store data."[^1] There is usually one DAO for each kind of stored data, such as players or orders. Its interface has the persistence operations, such as insert, find, update and delete, and the business code uses only that interface.

## Problem

"Access to data varies depending on the source of the data."[^1] A relational database, a flat file, a directory service and a remote system each have a different interface, and two database products differ in their SQL. When the business code calls the storage interface directly, the business rules and the details of the storage are in the same functions. The pattern names the cost: "Portability of the components is directly affected when specific access mechanisms and APIs are included in the components."[^1] A change of storage, or a change of the table layout, then means a change in each place of the business code that reads or writes the data.

## Solution

The participants are:[^1]

- **Business object.** The client that needs the data. It knows only the DAO interface.
- **Data access object.** The interface and its implementation. The implementation holds all the code for one data source: the connection, the queries, and the conversion between the stored form and objects.
- **Data source.** The storage: a database, a file, a directory or another system.
- **Transfer object.** "This represents a Transfer Object used as a data carrier."[^1] The DAO returns transfer objects and takes them as arguments, so the client never sees the rows, the result sets or the file format.

```text
  PlayerService ----uses----> PlayerDao (interface)
   (business object)            insert, find, find_by_country, update, delete
                                   ^                     ^
                                   |                     |
                          SqlitePlayerDao          TextPlayerDao
                                   |                     |
                               database             flat file
         both return Player objects (transfer objects)
```

The Core J2EE text adds a strategy for the creation of DAOs: a factory method gives the DAO for each kind of data, and an abstract factory gives a family of DAOs for each kind of storage.[^1] Then one line of configuration chooses the storage. This is the [[factory-method-pattern]] and the [[abstract-factory-pattern]]; in current code, [[dependency-injection]] often does the same work.

The following program has a DAO for SQLite. All the SQL is in the DAO, and the caller gets `Player` objects back.

```python run id=sqlite-dao
import sqlite3
from dataclasses import dataclass


@dataclass
class Player:
    id: int
    name: str
    country: str


class SqlitePlayerDao:
    # All the SQL for players is here. The caller gets Player objects back.
    def __init__(self, db):
        self.db = db
        db.execute("create table player (id integer primary key, name text, nation text)")

    def insert(self, p):
        self.db.execute("insert into player values (?, ?, ?)", (p.id, p.name, p.country))

    def find_by_country(self, country):
        rows = self.db.execute("select id, name, nation from player where nation = ? order by id", (country,))
        return [Player(*r) for r in rows]


dao = SqlitePlayerDao(sqlite3.connect(":memory:"))
dao.insert(Player(1, "Ann", "NZ"))
dao.insert(Player(2, "Bo", "AU"))
print(dao.find_by_country("NZ"))
```

```output
[Player(id=1, name='Ann', country='NZ')]
```

## Example

Each example file has a transfer object `Player`, a DAO interface with six operations, and two implementations of it. `MemoryPlayerDao` keeps a map from key to record. `TextPlayerDao` keeps a list of lines in the form `id;name;country`, as a flat file would. A factory function gives the DAO for a name, and a business object, `PlayerService`, moves a player to another country with only the interface. The program runs the same steps with each DAO: it inserts three players, finds one, moves one, finds the players of two countries, and deletes one. The output is the same for both DAOs. At the end, the program prints the lines in the text store, a format that the business code never saw.

## Consequences

**Benefits.**

- **The storage is hidden.** "Access is transparent because the implementation details are hidden inside the DAO."[^1]
- **The storage can change.** A new database, or a change of the tables, changes the DAO and not the business code. With a factory, the new DAO is one new class and one line of configuration.
- **The business code is simpler.** It has no SQL and no conversion of rows, and the Core J2EE text counts this as better readability and productivity.[^1]
- **Tests can use a fake DAO.** A test gives the business object a DAO in memory, so it runs with no database.
- **All data access is in one layer.** The DAOs of an application together form a [[database-access-layer]].

**Liabilities.**

**A DAO that returns its rows does not hide the storage.** In the following program, the SQLite DAO returns the database row, a tuple in column order, and the business code takes the third value. A second DAO returns a dictionary, so the same business code fails. A DAO must return transfer objects, which have the same form for each data source.

```python wrong id=dao-returns-rows
import sqlite3


class SqlitePlayerDao:
    def __init__(self):
        self.db = sqlite3.connect(":memory:")
        self.db.execute("create table player (id integer primary key, name text, nation text)")
        self.db.execute("insert into player values (1, 'Ann', 'NZ')")

    # Wrong: the DAO returns the database row, a tuple in column order,
    # and not a transfer object.
    def find(self, id):
        return self.db.execute("select * from player where id = ?", (id,)).fetchone()


class MemoryPlayerDao:
    def __init__(self):
        self.rows = {1: {"id": 1, "name": "Ann", "country": "NZ"}}

    def find(self, id):
        return self.rows[id]


def country_of(dao, id):
    # The business code knows that the country is the third column.
    return dao.find(id)[2]


print(country_of(SqlitePlayerDao(), 1))
try:
    print(country_of(MemoryPlayerDao(), 1))
except KeyError as e:
    print("KeyError:", e)
```

```output
NZ
KeyError: 2
```

**The errors can leak too.** A DAO that lets `sqlite3.Error` or `SQLException` out ties the caller to the storage again. Spring translates them: "Spring provides a convenient translation from technology-specific exceptions, such as SQLException to its own exception class hierarchy, which has DataAccessException as the root exception."[^2]

**It is one more layer.** "The DAOs create an additional layer of objects between the data client and the data source that need to be designed and implemented to leverage the benefits of this pattern."[^1] With the factory strategy, there are also two class hierarchies to design.

**A DAO for each table makes some queries hard.** A query over two tables does not belong to one DAO. Either the business code calls two DAOs and joins the results in memory, which is slow, or the DAOs get a finder method for each screen. A [[query-object]] or a DAO for each use case is the usual answer.

## When not to use it

**When the persistence framework already does the work.** The Core J2EE text says this for container-managed entity beans: "Applications using container-managed entity beans do not need a DAO layer, since the application server transparently provides this functionality."[^1] An ORM session or an `EntityManager` is in the same place today. A DAO whose methods only call the ORM adds code and no isolation.

**For small programs and scripts.** A program with one table and no plan to change it does not gain from an interface and two classes.

## Variants and related patterns

**Generated DAOs.** The Core J2EE text describes an automatic code generation strategy, with the code made from the relations between business objects and tables.[^1] Android Room generates the DAO class from an annotated interface.[^3]

**Generic DAO.** One DAO class, with a type parameter for the entity and the key, gives the same operations for all entities, and a subclass adds the special finders.

**DAO versus repository.** The two are often mixed up. A DAO is close to the storage: one data source, operations in the terms of the storage, and transfer objects. A [[repository-pattern]] is close to the domain. Fowler's Repository "Mediates between the domain and data mapping layers using a collection-like interface for accessing domain objects."[^4] A repository often uses a DAO or a [[data-mapper]] inside. The Javadoc of Spring's `@Repository` annotation lets DAO classes use it, but warns that "care should be taken to understand the distinction between Data Access Object and DDD-style repositories before doing so."[^5]

**Related patterns.** Fowler's [[table-data-gateway]] is close to a DAO: an object with the SQL for one table. A DAO that returns domain objects and maps them is a [[data-mapper]]. The Core J2EE text relates the DAO to the Broker pattern, "which describes approaches for decoupling clients and servers in distributed systems."[^1] The DAO interface is an application of [[program-to-an-interface]] and of [[separation-of-concerns]].

## Known uses

- Spring Framework: "The Data Access Object (DAO) support in Spring is aimed at making it easy to work with data access technologies (such as JDBC, Hibernate, or JPA) in a consistent way."[^2] A class with the `@Repository` annotation gets the translation of exceptions to `DataAccessException`.[^5]
- Android Room: "Each DAO includes functions that offer abstract access to your app's database. At compile time, Room automatically generates implementations of the DAOs that you define."[^3] Each DAO is an interface or an abstract class with the `@Dao` annotation.
- Java EE applications: the Core J2EE catalog shows DAOs over JDBC, with a factory of DAOs for each kind of storage.[^1]

## References
