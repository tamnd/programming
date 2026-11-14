---
tag: NWMKVH
kind: pattern
slug: identity-map
name: Identity Map
aliases: [identity map pattern, identity resolution, first-level cache]
area: software-design/data-access-patterns
level: core
summary: >-
  An Identity Map keeps each object that a session loads from the database in a map by its key, so that one row has only one object in memory and is not loaded twice.
family: architectural
polarity: positive
intent: "Ensures that each object gets loaded only once by keeping every loaded object in a map. Looks up objects using the map when referring to them."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). The SQLAlchemy Session, the Doctrine UnitOfWork and the Entity Framework Core DbContext each hold an identity map.
see_also: [unit-of-work, data-mapper, object-relational-impedance-mismatch, repository-pattern]
sources:
  - url: https://martinfowler.com/eaaCatalog/identityMap.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Identity Map
    locator: pattern summary; the book's Chapter 11
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 11, Object-Relational Behavioral Patterns, Identity Map
  - url: https://docs.sqlalchemy.org/en/20/orm/session_basics.html
    title: SQLAlchemy 2.0 documentation, Session Basics
    locator: What does the Session do; Get by Primary Key; Expiring / Refreshing; Is the Session thread-safe
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/reference/working-with-objects.html
    title: Doctrine ORM 3.7 documentation, Working with Objects
    locator: Entities and the Identity Map
  - url: https://docs.sqlalchemy.org/en/20/glossary.html
    title: SQLAlchemy 2.0 documentation, Glossary
    locator: identity map
  - url: https://learn.microsoft.com/en-us/ef/core/change-tracking/identity-resolution
    title: Entity Framework Core documentation, Identity Resolution in EF Core
    locator: introduction; Identity resolution and queries
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/reference/unitofwork.html
    title: Doctrine ORM 3.7 documentation, Doctrine Internals explained
    locator: How Doctrine keeps track of Objects
  - url: https://docs.sqlalchemy.org/en/20/orm/session_state_management.html
    title: SQLAlchemy 2.0 documentation, State Management
    locator: Session Referencing Behavior
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/identity.py#L126-L291
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/identity.py
    locator: class WeakInstanceDict, lines 126 to 291
  - url: https://github.com/doctrine/orm/blob/243f25157055d553da52d7c4bbe73c4623dadc61/src/UnitOfWork.php#L1722-L1739
    title: Doctrine ORM 3.8.0 source, src/UnitOfWork.php
    locator: UnitOfWork::addToIdentityMap, lines 1722 to 1739
  - url: https://learn.microsoft.com/en-us/ef/core/querying/tracking
    title: Entity Framework Core documentation, Tracking vs. No-Tracking Queries
    locator: Tracking queries; No-tracking queries; Identity resolution
---

## Intent

An Identity Map "Ensures that each object gets loaded only once by keeping every loaded object in a map. Looks up objects using the map when referring to them."[^1] In one session, one database row has one object in memory, however many times and by whatever query the program reads it.

## Problem

Two parts of a program read the same row, for example the customer of an order and the customer of an invoice. If each read makes a new object, the program has two objects for one row. Fowler's catalogue says: "If you aren't careful you can load the data from the same database record into two different objects. Then, when you update them both you'll have an interesting time writing the changes out to the database correctly."[^1]

Each part changes its own copy and saves it, and the last save wins. The change of the other part is lost, and no error is reported.

There is also a cost. "If you load the same data more than once you're incurring an expensive cost in remote calls."[^1]

## Solution

Give each session a map from the primary key to the loaded object. Before a mapper loads a row by its key, it looks in the map. When a query returns rows, the mapper looks up the key of each row. If the map has an object for the key, the mapper returns that object and does not make a new one. Otherwise it makes the object and puts it in the map. "Whenever you want an object, you check the Identity Map first to see if you already have it."[^1][^2]

```text
  caller                    PersonMapper                         database
  find(1)  ---------------> map has 1?  no  --------------------> SELECT ... WHERE id = 1
                            make Person 1, map[1] = it
  find(1)  ---------------> map has 1?  yes: return map[1]        (no query)
  find_by_city("Oslo") ---> query by city ----------------------> SELECT ... WHERE city = 'Oslo'
                            row 1: map has 1? yes: return map[1]
                            row 2: map has 2? no: make Person 2, map[2] = it
```

The participants are the **identity map**, which is usually keyed by the table or class and the primary key; the **mappers** or the session, which use it on every load; and the **callers**, which get the same object for the same row.

The following program uses SQLite. The second `find(1)` runs no query. The query by city must run, because the map is keyed by id, but it returns the object that the map already has. That object keeps a change that is not saved yet, so a query for the people in Oslo returns Ann with the city Rome. A second mapper, which stands for another session, has its own map and loads its own object.

```python run id=one-object-per-key
import sqlite3


class Person:
    def __init__(self, id, name, city):
        self.id, self.name, self.city = id, name, city


class PersonMapper:
    """Loads people. Its identity map holds one object for each primary key."""

    def __init__(self, db):
        self.db = db
        self.identity_map = {}
        self.selects = 0

    def find(self, id):
        if id in self.identity_map:  # already loaded: no query
            return self.identity_map[id]
        self.selects += 1
        row = self.db.execute("SELECT id, name, city FROM people WHERE id = ?", (id,)).fetchone()
        return None if row is None else self._load(row)

    def find_by_city(self, city):
        self.selects += 1  # the map is keyed by id, so a query by city must run
        rows = self.db.execute("SELECT id, name, city FROM people WHERE city = ? ORDER BY id", (city,))
        return [self._load(row) for row in rows]

    def _load(self, row):
        person = self.identity_map.get(row[0])
        if person is None:  # a new object only for a key that is not in the map
            person = Person(*row)
            self.identity_map[row[0]] = person
        return person


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, city TEXT)")
db.executemany("INSERT INTO people VALUES (?, ?, ?)", [(1, "Ann", "Oslo"), (2, "Bo", "Oslo"), (3, "Cy", "Bergen")])

mapper = PersonMapper(db)  # one mapper, and one map, for each session
a = mapper.find(1)
b = mapper.find(1)
print("a is b:", a is b, "| selects:", mapper.selects)

oslo = mapper.find_by_city("Oslo")
print("Oslo:", [p.name for p in oslo], "| oslo[0] is a:", oslo[0] is a, "| selects:", mapper.selects)

a.city = "Rome"  # a change in memory, not saved yet
print("b.city:", b.city)
print("Oslo again:", [(p.name, p.city) for p in mapper.find_by_city("Oslo")])

other = PersonMapper(db)  # another session has its own map
print("other session:", other.find(1) is a, other.find(1).city)
```

```output
a is b: True | selects: 1
Oslo: ['Ann', 'Bo'] | oslo[0] is a: True | selects: 2
b.city: Rome
Oslo again: [('Ann', 'Rome'), ('Bo', 'Oslo')]
other session: False Oslo
```

The ORMs behave in the same way. SQLAlchemy says that when two queries return the same row, "the two queries will have returned the same Python object", and that the ORM "will skip the population of attributes for an object that's already loaded."[^3] Doctrine documents the result of the last query above: "if the query finds the article by its old headline, the returned instance contains the modified headline."[^4]

## Example

Each example file has a `Person` class and a `PersonMapper` with `find` and `find_by_city` and a map from the id to the object. The program reads the same row twice, runs a query by city, changes an object, and makes a second mapper for another session. The Python file uses SQLite, and its map is a `weakref.WeakValueDictionary`, so the map does not keep objects alive. The JavaScript, Java, Go and Rust files keep the table in memory and count each read as one `SELECT`, because the standard libraries of these languages have no database driver. Each file compares objects by identity: `is` in Python, `===` in JavaScript, `==` on references in Java and on pointers in Go, and `Rc::ptr_eq` in Rust.

## Consequences

**Benefits.**

- **No lost updates inside a session.** All the code of a session changes the same object, so one save writes all the changes. The SQLAlchemy glossary says that with this pattern "all operations which occur for a particular database identity are transparently coordinated onto a single object instance."[^5] Entity Framework Core says that identity resolution "ensures Entity Framework Core (EF Core) is tracking a consistent graph with no ambiguities about the relationships or property values of the entities."[^6]
- **Fewer queries.** A lookup by key that hits the map costs no database call. Doctrine says that for a second `find` with the same id, "Doctrine will check the identity map first and doesn't need to make that database roundtrip."[^7]
- **Object identity means row identity.** Code can compare objects with the identity operator, and object graphs with cycles load correctly, because a reference to a row that is already loaded points to the existing object.
- **A base for change tracking.** A Unit of Work can find all the objects of a session in the map. At the flush, Doctrine "will ask the identity map for all objects that are currently managed."[^7] See [[unit-of-work]].

**Liabilities.**

**Only lookups by key skip the query.** The map knows objects only by key. "Doctrine only knows objects by id, so a query for different criteria has to go to the database, even if it was executed just before."[^7]

**Stale data.** The map does not see changes that other sessions make to the database. SQLAlchemy designs for this case: "The design assumption here is to assume a transaction that's perfectly isolated", and the application refreshes objects when it needs to.[^3] A long session can show old values.

**Memory.** The map grows with each loaded object. Entity Framework Core gives this as the reason why no-tracking queries skip identity resolution: it "requires keeping track of each instance returned so that it can be used instead of later creating a duplicate."[^6] SQLAlchemy keeps the memory down with weak references: "Objects within the session are weakly referenced."[^8]

**A second object for a known key is an error.** When the program makes its own object for a row that the session already holds, the map cannot hold both. Entity Framework Core throws an `InvalidOperationException` that says that "another instance with the key value '{Id: 1}' is already being tracked."[^6] SQLAlchemy and Doctrine also raise an error in this case.[^9][^10]

**No map, two objects.** Without an identity map, two reads of one row give two objects, and the last save wins.

```python wrong id=two-objects-one-row
import sqlite3


class Person:
    def __init__(self, id, name, city):
        self.id, self.name, self.city = id, name, city


def find(db, id):  # bug: a new object for each call, so one row has two objects
    return Person(*db.execute("SELECT id, name, city FROM people WHERE id = ?", (id,)).fetchone())


def save(db, p):
    db.execute("UPDATE people SET name = ?, city = ? WHERE id = ?", (p.name, p.city, p.id))


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE people (id INTEGER PRIMARY KEY, name TEXT, city TEXT)")
db.execute("INSERT INTO people VALUES (1, 'Ann', 'Oslo')")

a = find(db, 1)  # one part of the program corrects the name
a.name = "Anna"
b = find(db, 1)  # another part moves the same person
b.city = "Rome"
save(db, a)
save(db, b)  # writes the old name back
print(db.execute("SELECT name, city FROM people WHERE id = 1").fetchone())
```

```output
('Ann', 'Rome')
```

The name change is lost. With the `PersonMapper` of the Solution section, `a` and `b` are the same object, and the row gets both changes: `('Anna', 'Rome')`.

## When not to use it

**For read-only data.** A report that only reads rows and never changes them gains little from the map and pays for it in memory. Entity Framework Core says that no-tracking queries "are useful when the results are used in a read-only scenario."[^11]

**As a cache that many sessions share.** The map belongs to one session and one business transaction. Shared between users or threads, it serves stale objects and needs locks. SQLAlchemy says that "a particular instance of Session should be used in only one thread or task at a time."[^3] A cache across sessions is a different design, with rules for expiry.

**For immutable values.** A value object that never changes, such as a currency, can be copied freely. Two copies cannot diverge, so the map adds nothing.

## Variants and related patterns

**One map per class or per hierarchy.** The map can be one dictionary for each table, or one dictionary keyed by the class and the key. Doctrine keys its map by "root entity name" and "id", so all the classes of one inheritance hierarchy share one map.[^7][^10]

**Weak and strong maps.** A strong map keeps every loaded object until the session ends. A weak map drops an object when the program no longer uses it. SQLAlchemy uses a weak map but keeps a strong reference to the objects that are new, deleted or changed, because their changes are not yet written.[^8]

**Identity resolution without tracking.** Entity Framework Core can resolve identities within one query without tracking the objects, with `AsNoTrackingWithIdentityResolution`.[^11]

**Unit of Work.** The map is usually part of the Unit of Work, which uses it to find changed objects. See [[unit-of-work]].

**Identity Field.** The key of the map is the database key that the object stores. See [[identity-field]].

**Lazy Load.** A placeholder for an object that is not loaded yet must also go through the map, or a later load makes a second object. Doctrine returns the same reference for a proxy and for a later `find` of the same id.[^7] See [[lazy-load]].

## Known uses

- SQLAlchemy: the Session keeps its objects "inside a structure called the identity map", which holds "only one object with a particular primary key".[^3] The map is [WeakInstanceDict in identity.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/identity.py#L126-L291). Its `add` method raises an error when "another instance with key %s is already present in this session."[^9]
- Doctrine ORM, for PHP: "Doctrine uses the Identity Map pattern to track objects."[^7] In [UnitOfWork.php at 3.8.0](https://github.com/doctrine/orm/blob/243f25157055d553da52d7c4bbe73c4623dadc61/src/UnitOfWork.php#L1722-L1739), `addToIdentityMap` stores each entity under the root class name and a hash of its id, and throws an `EntityIdentityCollisionException` when another object has the same id.[^10]
- Entity Framework Core, for .NET: "A DbContext can only track one entity instance with any given primary key value."[^6] In a tracking query, "If EF Core finds an existing entity, then the same instance is returned".[^11]

## References
