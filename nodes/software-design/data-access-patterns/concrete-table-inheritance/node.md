---
tag: 2N96RH
kind: pattern
slug: concrete-table-inheritance
name: Concrete Table Inheritance
aliases: [concrete table inheritance pattern, table per concrete class, TPC, leaf table inheritance, union subclass mapping]
area: software-design/data-access-patterns
level: core
summary: >-
  Concrete Table Inheritance stores an inheritance hierarchy with one table for each concrete class, and each table has columns for all the fields of its class, the inherited ones too.
family: architectural
polarity: positive
intent: "Represents an inheritance hierarchy of classes with one table per concrete class in the hierarchy."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002) and notes that it is also called leaf table inheritance. Entity Framework Core calls it table-per-concrete-type, and Jakarta Persistence calls it the TABLE_PER_CLASS strategy.
see_also: [single-table-inheritance, class-table-inheritance, inheritance-mappers, identity-field, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/concreteTableInheritance.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Concrete Table Inheritance
    locator: pattern summary
  - url: https://docs.sqlalchemy.org/en/20/orm/inheritance.html
    title: SQLAlchemy 2.0 documentation, Mapping Class Inheritance Hierarchies
    locator: Concrete Table Inheritance
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/inheritance
    title: Entity Framework Core documentation, Inheritance
    locator: Table-per-concrete-type configuration; Key generation; Foreign key constraints; Summary and guidance
  - url: https://docs.jboss.org/hibernate/orm/6.6/userguide/html_single/Hibernate_User_Guide.html
    title: Hibernate ORM 6.6 User Guide
    locator: 3.14.4. Table per class
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Concrete Table Inheritance
  - url: https://docs.djangoproject.com/en/5.2/topics/db/models/
    title: Django 5.2 documentation, Models
    locator: Model inheritance, Abstract base classes
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/util.py#L320-L398
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/util.py
    locator: polymorphic_union, lines 320 to 398
---

## Intent

Concrete Table Inheritance "Represents an inheritance hierarchy of classes with one table per concrete class in the hierarchy."[^1] Each table holds all the fields of its class, the fields that the class inherits too, so one object is one row in one table.

## Problem

Relational databases have no inheritance. Fowler starts from the object: "Thinking of tables from an object instance point of view, a sensible route is to take each object in memory and map it to a single database row. This implies Concrete Table Inheritance, where there's a table for each concrete class in the inheritance hierarchy."[^1] A program has `Footballer`, `Cricketer` and `Bowler`, which share the fields of `Player`. Most code loads one kind of player at a time, and should read one table without a join and without null columns of other classes.

## Solution

Give each concrete class a table with a column for each of its fields, inherited or not. An abstract class gets no table. SQLAlchemy describes it: "Concrete inheritance maps each subclass to its own distinct table, each of which contains all columns necessary to produce an instance of that class."[^2] Fowler notes the name: "Most people think of it as leaf oriented since you usually have one table per leaf class in a hierarchy." But "a concrete class that isn't a leaf usually gets a table as well".[^1]

Two things need care. First, the key of an object must be unique in the whole hierarchy and not only in its table, so the tables take their keys from one source. Entity Framework Core says that it "requires that all entities in a hierarchy have a unique key value, even if the entities have different types", and that "there is no common table that can act as the single place where key values live and can be generated."[^3] Second, a query for the superclass must read all the tables, usually with a `UNION`.

```text
  classes                  tables (keys from one source)
  Player(name)             no table for the abstract class
   |- Footballer(club)     footballer(id, name, club)            1 Ann Lions
   |- Cricketer(runs)      cricketer(id, name, runs)             2 Bo 412
       |- Bowler(wickets)  bowler(id, name, runs, wickets)       3 Cy 95 31
  all players:  select ... from footballer union all select ... from cricketer union all ... bowler
```

The participants are the **class hierarchy**; a **table for each concrete class** with all the fields of the class; a **key source** shared by all the tables; and the **mapper**, which writes each object to the table of its class and reads all the tables for a query on a superclass.

The following program saves a footballer and a cricketer in their own tables with keys from one source, and reads both with a union.

```python run id=table-per-concrete
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table footballer (id integer primary key, name text, club text);
create table cricketer (id integer primary key, name text, runs integer);
create table key_source (next_id integer);
insert into key_source values (1);
""")


class Footballer:
    def __init__(self, name, club):
        self.name, self.club = name, club


class Cricketer:
    def __init__(self, name, runs):
        self.name, self.runs = name, runs


def next_key():
    # One key source for all the tables, so that a key is unique in the whole hierarchy.
    (id,) = db.execute("select next_id from key_source").fetchone()
    db.execute("update key_source set next_id = next_id + 1")
    return id


def insert(p):
    # Each concrete class has a table with all its fields, the inherited ones too.
    p.id = next_key()
    if isinstance(p, Footballer):
        db.execute("insert into footballer values (?, ?, ?)", (p.id, p.name, p.club))
    else:
        db.execute("insert into cricketer values (?, ?, ?)", (p.id, p.name, p.runs))


insert(Footballer("Ann", "Lions"))
insert(Cricketer("Bo", 412))
print(db.execute("select * from footballer").fetchall(), db.execute("select * from cricketer").fetchall())
print(db.execute("""select id, name, 'footballer' from footballer
                    union all select id, name, 'cricketer' from cricketer order by id""").fetchall())
```

```output
[(1, 'Ann', 'Lions')] [(2, 'Bo', 412)]
[(1, 'Ann', 'footballer'), (2, 'Bo', 'cricketer')]
```

## Example

Each example file saves a footballer, a cricketer and a bowler, and prints the three tables. Each row has all the fields of its class, and no column is null. It then finds the player with key 3. The key does not say which table holds the row, so the mapper tries the footballer, cricketer and bowler tables in turn. A union of the three tables, with the name of each table as a type, lists all the players. A query for cricketers must read the cricketer table and the bowler table, because a bowler is a cricketer. The last line shows the next key of the shared key source. The Python file uses SQLite with a key table. The JavaScript, Java, Go and Rust files keep a map for each table and one counter for the keys.

## Consequences

**Benefits.**

- **One row for each object.** A load of a known class reads one table with no join, and each column of the table belongs to the class.
- **Each column can be NOT NULL.** No row holds an object of another class, so the database can check all the fields.
- **The tables are self-contained.** Code that reads one table sees whole objects of one class.

**Liabilities.**

**Queries on the superclass read all the tables.** SQLAlchemy builds a special select for them, "typically is produced as a UNION of all the tables", and warns that "When used polymorphically it produces very large queries with UNIONS that won’t perform as well as simple joins."[^2] The Hibernate guide says: "Polymorphic queries require multiple UNION queries, so be aware of the performance implications of a large class hierarchy."[^4]

**Keys must be unique across the tables.** If each table makes its own keys, two objects of different classes get the same key. In the following program, a footballer and a cricketer both have key 1, and a record that refers to player 1 matches two rows.

```python wrong id=key-per-table
import sqlite3

db = sqlite3.connect(":memory:")
# Each table makes its own keys, so a footballer and a cricketer can get the same key.
db.executescript("""
create table footballer (id integer primary key, name text, club text);
create table cricketer (id integer primary key, name text, runs integer);
insert into footballer (name, club) values ('Ann', 'Lions');
insert into cricketer (name, runs) values ('Bo', 412);
""")

# A match record refers to player 1. Which player is that?
sql = """select 'footballer', id, name from footballer where id = 1
         union all select 'cricketer', id, name from cricketer where id = 1"""
print(db.execute(sql).fetchall())
```

```output
[('footballer', 1, 'Ann'), ('cricketer', 1, 'Bo')]
```

The fix is one key source for the whole hierarchy, such as a key table or a database sequence. Entity Framework Core uses "a single sequence referenced in the default constraint for each table".[^3]

**No foreign key to the superclass.** A column that refers to any player cannot have a foreign key constraint, because players are in many tables. "This means an FK constraint cannot be created for this relationship."[^3]

**A change to a superclass changes many tables.** A new field in `Player` is a new column in each concrete table. Fowler's book lists this with the key and the foreign key problems.[^5]

## When not to use it

**When code often queries the superclass.** Each such query is a union of all the tables. Use [[single-table-inheritance]]. Entity Framework Core gives this advice: "if your code will mostly query for entities of many types, such as writing queries against the base type, then lean towards TPH over TPC."[^3]

**When other tables must refer to the superclass.** A foreign key to any player needs one table for players. Use [[class-table-inheritance]].

**When the superclass changes often.** Each change touches all the concrete tables.

## Variants and related patterns

**Shared fields without polymorphism.** Some frameworks copy the fields of a base class into each table, but do not load the classes through the base class. In Django, an abstract base class "will then not be used to create any database table. Instead, when it is used as a base class for other models, its fields will be added to those of the child class."[^6] Hibernate notes that "it is not necessary to explicitly map such inheritance hierarchies. You can map each class as a separate entity root."[^4]

**The other strategies.** [[single-table-inheritance]] puts the hierarchy in one table, and [[class-table-inheritance]] gives each class a table with the fields that it declares. [[inheritance-mappers]] organizes the mapper classes for any of the three.

## Known uses

- SQLAlchemy: concrete table inheritance with `ConcreteBase` or `AbstractConcreteBase`.[^2] [polymorphic_union in util.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/util.py#L320-L398) builds a `UNION ALL` with one `SELECT` for each table. It fills each column that a table lacks with a typed `NULL`, and adds a literal type column with the polymorphic identity of the table.[^7]
- Entity Framework Core, for .NET: `UseTpcMappingStrategy` maps a hierarchy as table-per-concrete-type, with keys from one database sequence.[^3]
- Hibernate and Jakarta Persistence: `@Inheritance(strategy = InheritanceType.TABLE_PER_CLASS)`. "This is called the table-per-concrete-class strategy. Each table defines all persistent states of the class, including the inherited state."[^4]

## References
