---
tag: 5HCJ26
kind: pattern
slug: unit-of-work
name: Unit of Work
aliases: [unit of work pattern, UoW, transactional write-behind]
area: software-design/data-access-patterns
level: core
summary: >-
  A Unit of Work records the objects that a business transaction adds, changes and removes, and then writes all the changes to the database in one short transaction.
family: architectural
polarity: positive
intent: "Maintains a list of objects affected by a business transaction and coordinates the writing out of changes and the resolution of concurrency problems."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). The SQLAlchemy Session, the Doctrine EntityManager and the Entity Framework Core DbContext implement it.
see_also: [identity-map, data-mapper, active-record, repository-pattern, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/unitOfWork.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Unit of Work
    locator: pattern summary; the book's Chapter 11
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 11, Object-Relational Behavioral Patterns, Unit of Work
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/reference/working-with-objects.html
    title: Doctrine ORM 3.7 documentation, Working with Objects
    locator: introduction; Persisting entities; Entity State and the Identity Map
  - url: https://github.com/doctrine/orm/blob/243f25157055d553da52d7c4bbe73c4623dadc61/src/UnitOfWork.php#L380-L513
    title: Doctrine ORM 3.8.0 source, src/UnitOfWork.php
    locator: UnitOfWork::commit, lines 380 to 513
  - url: https://learn.microsoft.com/en-us/ef/core/saving/basic
    title: Entity Framework Core documentation, Basic SaveChanges
    locator: Multiple operations in a single SaveChanges
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/unitofwork.py#L156-L486
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/unitofwork.py
    locator: module docstring and class UOWTransaction, lines 156 to 486
  - url: https://learn.microsoft.com/en-us/ef/core/change-tracking/
    title: Entity Framework Core documentation, Change Tracking in EF Core
    locator: Entity tracking; Entity states; Tracking from queries
  - url: https://docs.sqlalchemy.org/en/20/orm/session_basics.html
    title: SQLAlchemy 2.0 documentation, Session Basics
    locator: What does the Session do; Flushing
  - url: https://www.doctrine-project.org/projects/doctrine-orm/en/3.7/reference/unitofwork.html
    title: Doctrine ORM 3.7 documentation, Doctrine Internals explained
    locator: How Doctrine Detects Changes; Query Internals
  - url: https://docs.sqlalchemy.org/en/20/glossary.html
    title: SQLAlchemy 2.0 documentation, Glossary
    locator: unit of work; version id column
  - url: https://learn.microsoft.com/en-us/ef/core/querying/tracking
    title: Entity Framework Core documentation, Tracking vs. No-Tracking Queries
    locator: No-tracking queries
---

## Intent

A Unit of Work "Maintains a list of objects affected by a business transaction and coordinates the writing out of changes and the resolution of concurrency problems."[^1] The program changes objects in memory. The Unit of Work remembers what changed and, at the end, writes all of it to the database in one transaction.

## Problem

A business transaction, such as a money transfer, changes several objects: two accounts change their balances and two ledger entries are new. The database must get all of these changes or none of them.

If the program writes each change at once, it makes many small database calls. Fowler's catalogue says that this "can lead to lots of very small database calls, which ends up being very slow." It also needs a database transaction that stays open for the whole interaction, "which is impractical if you have a business transaction that spans multiple requests."[^1]

If the program does not write each change at once, something must remember the changes. Otherwise "that data won't be written back into the database."[^1] The domain code should not do this bookkeeping itself, because then every business method must know which objects are new, which are changed and which are deleted, and in which order to write them.

## Solution

Give the bookkeeping to one object. The business code tells the Unit of Work about each new, changed and removed object. When the business transaction is done, the program calls `commit`. The Unit of Work then opens a database transaction, writes the inserts, the updates and the deletes through the mappers, and commits. If one statement fails, it rolls back, and the database keeps none of the changes. "When you're done, it figures out everything that needs to be done to alter the database as a result of your work."[^1][^2]

```text
  business code                 UnitOfWork                         database
  transfer(a, b, 30)
    a.balance -= 30  ---------> register_dirty(a)       dirty:   [a, b]
    b.balance += 30  ---------> register_dirty(b)       new:     [e1, e2]
    new Entry e1, e2 ---------> register_new(e1, e2)    removed: []
  commit()          ---------> BEGIN
                                 insert e1, e2  ----------------> INSERT INTO entries
                                 update a, b    ----------------> UPDATE accounts
                                 delete ...     ----------------> DELETE FROM ...
                               COMMIT, or ROLLBACK on the first error
```

The participants are the **business code**, which changes objects and registers them; the **Unit of Work**, which keeps the lists of new, dirty and removed objects and controls the database transaction; and the **mappers**, which know the SQL for each kind of object.

The following program uses SQLite. It traces each SQL statement. The `transfer` function has no SQL. The first commit writes four statements in one transaction. The second commit fails on the `CHECK` constraint, and the rollback also removes the two entries that it had already inserted.

```python run id=commit-in-one-transaction
import sqlite3
from dataclasses import dataclass


@dataclass
class Account:
    id: int
    owner: str
    balance: int


@dataclass
class Entry:
    account_id: int
    amount: int


# Each mapper has only the methods that this program needs.
class AccountMapper:
    def update(self, db, a):
        db.execute("UPDATE accounts SET owner = ?, balance = ? WHERE id = ?", (a.owner, a.balance, a.id))


class EntryMapper:
    def insert(self, db, e):
        db.execute("INSERT INTO entries (account_id, amount) VALUES (?, ?)", (e.account_id, e.amount))


class UnitOfWork:
    """Records what a business transaction changes, and writes it all in one database transaction."""

    def __init__(self, db, mappers):
        self.db, self.mappers = db, mappers
        self.new, self.dirty, self.removed = [], [], []

    def register_new(self, obj):
        self.new.append(obj)

    def register_dirty(self, obj):
        if not any(o is obj for o in self.new + self.dirty):
            self.dirty.append(obj)

    def register_removed(self, obj):
        self.removed.append(obj)

    def commit(self):
        try:
            with self.db:  # commits at the end, or rolls back if a statement fails
                for obj in self.new:
                    self.mappers[type(obj)].insert(self.db, obj)
                for obj in self.dirty:
                    self.mappers[type(obj)].update(self.db, obj)
                for obj in self.removed:
                    self.mappers[type(obj)].delete(self.db, obj)
        finally:
            self.new, self.dirty, self.removed = [], [], []


def transfer(uow, source, target, amount):
    """Business logic: change objects in memory and tell the unit of work. No SQL here."""
    source.balance -= amount
    target.balance += amount
    uow.register_dirty(source)
    uow.register_dirty(target)
    uow.register_new(Entry(source.id, -amount))
    uow.register_new(Entry(target.id, amount))


db = sqlite3.connect(":memory:")
db.executescript("""
CREATE TABLE accounts (id INTEGER PRIMARY KEY, owner TEXT, balance INTEGER CHECK (balance >= 0));
CREATE TABLE entries (id INTEGER PRIMARY KEY, account_id INTEGER, amount INTEGER);
INSERT INTO accounts VALUES (1, 'ann', 100), (2, 'bob', 0);
""")
db.set_trace_callback(lambda sql: print("  sql:", sql.split(" (")[0].strip()))
mappers = {Account: AccountMapper(), Entry: EntryMapper()}
ann, bob = Account(1, "ann", 100), Account(2, "bob", 0)

uow = UnitOfWork(db, mappers)
transfer(uow, ann, bob, 30)
print("nothing written yet:", db.execute("SELECT count(*) FROM entries").fetchone()[0], "entries")
print("commit 1")
uow.commit()

uow = UnitOfWork(db, mappers)
transfer(uow, bob, ann, 50)  # bob has 30, so the CHECK constraint fails
print("commit 2")
try:
    uow.commit()
except sqlite3.IntegrityError as e:
    print("  failed:", type(e).__name__)

db.set_trace_callback(None)
print("accounts:", db.execute("SELECT * FROM accounts").fetchall())
print("entries:", db.execute("SELECT account_id, amount FROM entries").fetchall())
```

```output
  sql: SELECT count(*) FROM entries
nothing written yet: 0 entries
commit 1
  sql: BEGIN
  sql: INSERT INTO entries
  sql: INSERT INTO entries
  sql: UPDATE accounts SET owner = 'ann', balance = 70 WHERE id = 1
  sql: UPDATE accounts SET owner = 'bob', balance = 30 WHERE id = 2
  sql: COMMIT
commit 2
  sql: BEGIN
  sql: INSERT INTO entries
  sql: INSERT INTO entries
  sql: UPDATE accounts SET owner = 'bob', balance = -20 WHERE id = 2
  sql: ROLLBACK
  failed: IntegrityError
accounts: [(1, 'ann', 70), (2, 'bob', 30)]
entries: [(1, -30), (2, 30)]
```

The table has only the entries of the first transfer. After the failed commit, the objects `ann` and `bob` in memory still hold the balances 120 and -20. They no longer match the database, so a program must load them again or drop them. Doctrine closes its EntityManager when a commit fails, and the Doctrine documentation says that changes that are not yet written "are lost" when a Unit of Work is closed.[^3][^4]

## Example

Each example file has an `Account` class, an `Entry` class, mappers for both, and a `UnitOfWork` with `register_new`, `register_dirty`, `register_removed` and `commit`. The program runs three business transactions: a transfer that succeeds, a transfer that fails and rolls back, and the close of an account, where the account is first dirty and then removed, so that the Unit of Work writes a `DELETE` and no `UPDATE` for it. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the tables in memory, and their `transaction` method keeps a copy of the tables and puts it back when a statement fails, because the standard libraries of these languages have no database driver. The Unit of Work compares objects by identity, not by value, in every file.

## Consequences

**Benefits.**

- **One short transaction.** The database transaction is open only during the commit, not during the whole business transaction. Doctrine calls this "transactional write-behind": it delays the SQL until the flush, and then writes it "in the most efficient way and a single, short transaction, taking care of maintaining referential integrity."[^3]
- **All or nothing.** The commit writes all the changes or none of them. Entity Framework Core says that "For most database providers, SaveChanges is transactional."[^5]
- **The domain code has no SQL and no bookkeeping.** The business code changes objects. The Unit of Work decides which statements to run, and in which order. SQLAlchemy "organizes them in order of dependency", so that, for example, a parent row is inserted before the rows that refer to it.[^6] Doctrine runs the entity inserts first, then the entity updates, and the entity deletions last.[^4]
- **Only changed values are written.** Entity Framework Core says that with its change tracking, "Updates are limited to only those values that have actually changed."[^7]

**Liabilities.**

**Queries do not see pending changes.** Until the commit, the changes exist only in memory. A query that runs before the commit reads the old rows. The Doctrine documentation warns: "Changes to managed entities that were not flushed yet are not taken into account by queries."[^3]

```python wrong id=query-misses-pending
import sqlite3


class UnitOfWork:
    def __init__(self, db):
        self.db, self.new = db, []

    def register_new(self, entry):
        self.new.append(entry)

    def commit(self):
        with self.db:
            self.db.executemany("INSERT INTO entries (account_id, amount) VALUES (?, ?)", self.new)
        self.new = []


db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE entries (account_id INTEGER, amount INTEGER)")
uow = UnitOfWork(db)
uow.register_new((1, -30))

# bug: the new entry is still pending, so the query does not count it
count = db.execute("SELECT count(*) FROM entries WHERE account_id = 1").fetchone()[0]
print("entries of account 1 before the commit:", count)
uow.commit()
count = db.execute("SELECT count(*) FROM entries WHERE account_id = 1").fetchone()[0]
print("entries of account 1 after the commit:", count)
```

```output
entries of account 1 before the commit: 0
entries of account 1 after the commit: 1
```

The fix is to write the pending changes before such a query, inside the same transaction. SQLAlchemy does this by itself. Its Session flushes "before any individual SQL statement is issued as a result of a Query", and this is "known as autoflush".[^8] Doctrine leaves it to the program: "Call EntityManager#flush() before running queries when they should take pending changes into account."[^3]

**Change detection costs time.** A Unit of Work that compares each object with a copy of its old values must visit every object that it holds. Doctrine says: "The larger the size of the UnitOfWork is, the longer this computation takes."[^9]

**Concurrent writers.** The database transaction is short, so another user can change the same rows between the read and the commit. The Unit of Work must detect this, which is the "resolution of concurrency problems" of the intent.[^1] A common way is a version column: SQLAlchemy can use one "as a means of testing for stale data when a row is being UPDATEd with new information."[^10]

## When not to use it

**When each operation is one statement.** A script or a request that changes one row and returns gains nothing from a list of pending changes. An Active Record that saves itself at once is less code.

**For bulk changes.** A Unit of Work loads each object, changes it in memory and writes it back. An `UPDATE ... WHERE` statement that changes a million rows in the database is much faster than a million tracked objects.

**For reads only.** A report that only reads needs no change tracking. Entity Framework Core says: "If the entities retrieved from the database don't need to be updated, then a no-tracking query should be used."[^11]

**For a long time.** A Unit of Work is for one business transaction. Entity Framework Core says that a "DbContext is designed to represent a short-lived unit-of-work", and that disposing the DbContext is the normal way to stop tracking entities.[^7] A long-lived one holds stale objects and grows without limit.

## Variants and related patterns

**How the Unit of Work learns about changes.** In the code above, the business code registers each change. ORMs find the changes themselves, in two ways:

- **Instrumented attributes.** SQLAlchemy objects are instrumented "such that whenever an attribute or a collection is modified in the Python program, a change event is generated which is recorded by the Session."[^8]
- **Snapshots.** Doctrine keeps a copy of the properties of each loaded object. At the flush, it "will iterate over the Identity Map and for each object compares the original property and association values with the values that are currently set on the object."[^9] Entity Framework Core "automatically tracks the state of queried entities and then detects any changes made to these entities when SaveChanges is called."[^7]

**Entity states.** Instead of three lists, a Unit of Work can keep a state for each object. Entity Framework Core uses the states Detached, Added, Unchanged, Modified and Deleted.[^7]

**Identity Map.** The Unit of Work usually holds an Identity Map, so that each row has one object in memory and each change is registered once. SQLAlchemy keeps its objects "inside a structure called the identity map".[^8] See [[identity-map]].

**Data Mapper and Repository.** The Unit of Work calls the mappers to write the objects, and a repository adds objects to it. See [[data-mapper]] and [[repository-pattern]].

**Active Record.** An Active Record object writes itself when the program calls its `save` method, so there is no separate Unit of Work. See [[active-record]].

## Known uses

- SQLAlchemy: "SQLAlchemy's Session implements the unit of work pattern".[^10] The flush builds a [UOWTransaction in unitofwork.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/unitofwork.py#L156-L486). The module docstring says that it "assembles flush tasks based on mappers and their properties, organizes them in order of dependency, and executes."[^6]
- Doctrine ORM, for PHP: "A Unit of Work is similar to an object-level transaction." It starts when an EntityManager is created, and `EntityManager#flush()` commits it.[^3] In [UnitOfWork.php at 3.8.0](https://github.com/doctrine/orm/blob/243f25157055d553da52d7c4bbe73c4623dadc61/src/UnitOfWork.php#L380-L513), `commit` begins a transaction, runs the inserts, the updates and the deletions, and rolls back if a step fails.[^4]
- Entity Framework Core, for .NET: "Each DbContext instance tracks changes made to entities. These tracked entities in turn drive the changes to the database when SaveChanges is called."[^7]

## References
