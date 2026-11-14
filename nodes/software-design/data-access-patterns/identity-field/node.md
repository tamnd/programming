---
tag: W7RCK2
kind: pattern
slug: identity-field
name: Identity Field
aliases: [identity field pattern, id field, primary key field]
area: software-design/data-access-patterns
level: core
summary: >-
  An Identity Field stores the primary key of a database row in a field of the object that holds the row's data, so the program can find the row again to update or delete it.
family: architectural
polarity: positive
intent: "Saves a database ID field in an object to maintain identity between an in-memory object and a database row."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). Object-relational mappers such as Django, SQLAlchemy and Entity Framework Core give each mapped object a primary key field by convention.
see_also: [identity-map, data-mapper, active-record, unit-of-work, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/identityField.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Identity Field
    locator: pattern summary
  - url: https://www.sqlite.org/autoinc.html
    title: SQLite documentation, SQLite Autoincrement
    locator: 1. Summary; 2. Background
  - url: https://docs.python.org/3/library/sqlite3.html
    title: The Python Standard Library, sqlite3
    locator: Cursor.lastrowid
  - url: https://en.wikipedia.org/wiki/Surrogate_key
    title: Surrogate key, Wikipedia
    locator: Advantages, Stability and Requirement changes; Disadvantages, Normalization and Inadvertent disclosure
  - url: https://docs.djangoproject.com/en/5.2/ref/models/fields/
    title: Django 5.2 documentation, Model field reference
    locator: Field.primary_key
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/keys
    title: Entity Framework Core documentation, Keys
    locator: Configuring a primary key
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Identity Field
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/generated-properties
    title: Entity Framework Core documentation, Generated Values
    locator: Primary keys
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L655-L665
    title: Django 5.2.8 source, django/db/models/base.py
    locator: Model._get_pk_val, Model._set_pk_val and the pk property, lines 655 to 665
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L1168-L1174
    title: Django 5.2.8 source, django/db/models/base.py
    locator: Model._save_table, the insert branch, lines 1168 to 1174
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L3505-L3537
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/mapper.py
    locator: Mapper.identity_key_from_instance and Mapper._identity_key_from_state, lines 3505 to 3537
---

## Intent

An Identity Field "Saves a database ID field in an object to maintain identity between an in-memory object and a database row."[^1] The object keeps the primary key of its row. The program uses that key to find the row again when it updates or deletes the object.

## Problem

"Relational databases tell one row from another by using key - in particular, the primary key. However, in-memory objects don't need such a key, as the object system ensures the correct identity under the covers".[^1] A program that only reads rows can ignore this difference. A program that also writes changes back must know which row each object came from. Fowler puts it this way: "in order to write data back you need to tie the database to the in-memory object system."[^1]

The object's other fields cannot always do this. A name or an email can change, and two rows can have the same name. If the program finds the row by data that the user can edit, an update can miss the row or change the wrong one.

## Solution

"In essence, Identity Field is mind-numbingly simple. All you do is store the primary key of the relational database table in the object's fields."[^1] The field is empty while the object is new. When the program inserts the row, it gets the key that the database made and puts it in the field. After that, each `UPDATE` and `DELETE` finds the row by that key.

```text
  new Person            insert            database             Person after save
  id   = None   --->  INSERT ... ---->  row id 1 created  --->  id   = 1
  name = Ann                          lastrowid = 1           name = Ann

  change the email, then save:
  UPDATE person SET ... WHERE id = 1    (the key, not the data, finds the row)
```

The participants are the **domain object**, which has the key field; the **table**, whose primary key the field copies; and the **mapper** or the object itself, which reads the key after an insert and puts it in each `WHERE` clause.

The database usually makes the key. In SQLite, if the `INTEGER PRIMARY KEY` column "is not explicitly given a value, then it will be filled automatically with an unused integer, usually one more than the largest ROWID currently in use."[^2] Python's `sqlite3` gives the new key to the program: `lastrowid` is a "Read-only attribute that provides the row id of the last inserted row."[^3]

The following program saves two new people, changes the email of one, and saves again. The second save finds the row by its key, although the data changed.

```python run id=insert-then-update
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table person (id integer primary key, name text, email text)")


class Person:
    def __init__(self, name, email):
        self.id = None  # The Identity Field: None until the row exists.
        self.name, self.email = name, email


class PersonMapper:
    def save(self, p):
        if p.id is None:
            sql = "insert into person (name, email) values (?, ?)"
            p.id = db.execute(sql, (p.name, p.email)).lastrowid  # Keep the new key.
        else:
            sql = "update person set name = ?, email = ? where id = ?"
            db.execute(sql, (p.name, p.email, p.id))

    def find(self, id):
        sql = "select id, name, email from person where id = ?"
        id, name, email = db.execute(sql, (id,)).fetchone()
        p = Person(name, email)
        p.id = id
        return p


mapper = PersonMapper()
ann, bo = Person("Ann", "ann@example.com"), Person("Bo", "bo@example.com")
print("before save:", ann.id, bo.id)
mapper.save(ann)
mapper.save(bo)
print("after save:", ann.id, bo.id)
ann.email = "ann@new.example"
mapper.save(ann)  # The key finds the row, even though the email changed.
print(db.execute("select * from person order by id").fetchall())
found = mapper.find(1)
print("find(1):", found.id, found.name, found.email)
```

```output
before save: None None
after save: 1 2
[(1, 'Ann', 'ann@new.example'), (2, 'Bo', 'bo@example.com')]
find(1): 1 Ann ann@new.example
```

## Example

Each example file saves two new people and prints their keys before and after the insert. It then changes the email of the first person and saves again: the update finds the row by its key. It loads row 1 twice and shows that the two loads give two different objects with equal keys, so the key, not the object reference, tells that they stand for the same row. The last line makes the same change in a table that uses the email as the key, and the update finds no row. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the tables in memory and use a counter in place of the generated key.

## Consequences

**Benefits.**

- **The object can find its row after any change to its data.** The key of a generated key column does not depend on the data. "Surrogate keys typically do not change while the row exists", so "Applications cannot lose their reference to a row in the database (since the identifier does not change)."[^4]
- **Equality across loads.** Two objects that hold the same key stand for the same row. An [[identity-map]] uses the key to keep only one object for each row.
- **Simple references.** A reference from one object to another is saved as the other object's key. See [[foreign-key-mapping]].
- **Frameworks add it for the programmer.** In Django, if a model has no primary key field, "Django will automatically add a field to hold the primary key."[^5] In Entity Framework Core, "By convention, a property named Id or <type name>Id will be configured as the primary key of an entity."[^6]

**Liabilities.**

**A meaningful key can change.** If the key is data that the user can edit, such as an email, a change to the data is also a change to the key. Wikipedia notes that "Attributes that uniquely identify an entity might change, which might invalidate the suitability of natural keys."[^4] In the following program the object keeps only its email, so after the change the update looks for the new email and finds no row. The row keeps the old data, and the program sees no error.

```python wrong id=meaningful-key-changes
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table person (email text primary key, name text)")
db.execute("insert into person values ('ann@example.com', 'Ann')")


class Person:
    def __init__(self, email, name):
        self.email, self.name = email, name  # The email is also the key.


def save(p):
    sql = "update person set name = ? where email = ?"
    return db.execute(sql, (p.name, p.email)).rowcount


ann = Person(*db.execute("select email, name from person").fetchone())
ann.email = "ann@new.example"  # A change to the data is also a change to the key.
ann.name = "Anna"
print("rows updated:", save(ann))
print(db.execute("select email, name from person").fetchall())
```

```output
rows updated: 0
[('ann@example.com', 'Ann')]
```

**The key exists only after the insert.** A new object has no key until it is saved. Code that compares new objects by key, or that saves a reference to a new object, must save that object first.

**Duplicates of the real entity.** A generated key makes every row unique, even two rows for the same person. "Surrogate keys can result in duplicate values in any natural keys", and Wikipedia adds that to prevent this "one must preserve the role of the natural keys as unique constraints".[^4]

**Old keys can come back.** In SQLite without `AUTOINCREMENT`, "ROWIDs from previously deleted rows might be reused when creating new rows".[^2] An object that kept the key of a deleted row can then update a different row.

**The key can leak information.** "Proprietary information can be leaked if surrogate keys are generated sequentially."[^4] A visible order number tells how many orders came before it.

## When not to use it

**For objects that have no row of their own.** A value that is stored in the columns of its owner, such as a date range or an amount of money, has no key. It is an embedded value, and its identity is its data.

**When the program only reads.** A report that never writes rows back does not need to find them again, and a row can be read into plain values.

**When the key must change with the data.** Fowler's book advises against meaningful keys, because people make mistakes in the data and the key must then change.[^7] If a table must use such a key, the object must keep the old value until the update is written.

## How to fix it

Give the table a generated key that has no meaning, and keep the natural data, such as the email, in ordinary columns with a unique constraint. Set the key field once, from the database, after the insert. Django makes the field read-only in effect: "If you change the value of the primary key on an existing object and then save it, a new object will be created alongside the old one."[^5]

## Variants and related patterns

**How the key is made.** Fowler's book describes three ways to get a new key: let the database generate it, use a GUID, or make the key in the program, for example from a key table.[^7] Entity Framework Core uses the first way by convention: "non-composite primary keys of type short, int, long, or Guid are set up to have values generated for inserted entities if a value isn't provided by the application."[^8]

**Simple and compound keys.** A simple key is one column. A compound key has several columns, and the object then often holds a key object that compares all of them. Entity Framework Core calls this a composite key and sets one up by convention "only" in specific cases.[^6]

**Table-unique and database-unique keys.** A key can be unique in one table, or across all tables. A database-unique key lets one map hold the objects of all tables.[^7]

**Identity Map.** The map uses the identity field as its key. See [[identity-map]].

**Foreign Key Mapping.** A reference between objects is saved as the identity field of the referenced object. See [[foreign-key-mapping]].

## Known uses

- Django: every model has a `pk` property. In [base.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L655-L665), `_get_pk_val` reads the attribute of the primary key field, and `_set_pk_val` sets it.[^9] After an insert, [`_save_table`](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L1168-L1174) reads the values that the database returns, such as the new key, and calls `setattr(self, field.attname, value)` for each one.[^10]
- SQLAlchemy: [Mapper.identity_key_from_instance in mapper.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L3505-L3537) will "Return the identity key for the given instance, based on its primary key attributes." The key is a tuple of the class, the primary key values and an identity token.[^11]
- Entity Framework Core, for .NET: a property named `Id` or `<type name>Id` is the key by convention, and an `int` key gets a generated value on insert.[^6][^8]

## References
