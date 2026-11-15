---
tag: CDC2SN
kind: pattern
slug: class-table-inheritance
name: Class Table Inheritance
aliases: [class table inheritance pattern, joined table inheritance, table per type, TPT, table per subclass, multi-table inheritance]
area: software-design/data-access-patterns
level: core
summary: >-
  Class Table Inheritance stores an inheritance hierarchy with one table for each class, so an object is spread over the tables of its class and of each class above it, joined by one key.
family: architectural
polarity: positive
intent: "Represents an inheritance hierarchy of classes with one table for each class."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). SQLAlchemy calls it joined table inheritance, Entity Framework Core calls it table-per-type, and Django calls it multi-table inheritance.
see_also: [single-table-inheritance, concrete-table-inheritance, identity-field, foreign-key-mapping, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/classTableInheritance.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Class Table Inheritance
    locator: pattern summary
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/inheritance
    title: Entity Framework Core documentation, Inheritance
    locator: Table-per-type configuration; Summary and guidance
  - url: https://docs.sqlalchemy.org/en/20/orm/inheritance.html
    title: SQLAlchemy 2.0 documentation, Mapping Class Inheritance Hierarchies
    locator: Joined Table Inheritance
  - url: https://docs.jboss.org/hibernate/orm/6.6/userguide/html_single/Hibernate_User_Guide.html
    title: Hibernate ORM 6.6 User Guide
    locator: 3.14.3. Joined table
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Class Table Inheritance
  - url: https://docs.djangoproject.com/en/5.2/topics/db/models/
    title: Django 5.2 documentation, Models
    locator: Model inheritance, Multi-table inheritance
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L277-L290
    title: Django 5.2.8 source, django/db/models/base.py
    locator: ModelBase.__new__, the automatic parent link, lines 277 to 290
---

## Intent

Class Table Inheritance "Represents an inheritance hierarchy of classes with one table for each class."[^1] Each table holds the fields that its class declares. One object is a row in the table of its class and a row in the table of each class above it, all with the same key.

## Problem

"A very visible aspect of the object-relational mismatch is the fact that relational databases don't support inheritance. You want database structures that map clearly to the objects and allow links anywhere in the inheritance structure."[^1] A program has `Player`, its subclasses `Footballer` and `Cricketer`, and `Bowler` below `Cricketer`. A single table for all of them has many null columns and cannot make a subclass column NOT NULL. Another table, such as a table of match results, may need a foreign key to any player, or only to cricketers.

## Solution

"Class Table Inheritance supports this by using one database table per class in the inheritance structure."[^1] Each table has the columns of the fields that its class adds. The table of a subclass has the same key as the table of its superclass, and that key is also a foreign key to the superclass table. Entity Framework Core describes it: "Properties that belong solely to a base type or derived type are stored in a table that maps to that type. Tables that map to derived types also store a foreign key that joins the derived table with the base table."[^2]

To save an object, the mapper inserts one row in each table on the path from the root to the class of the object. To load it, the mapper joins these tables on the key. "Querying for a particular subclass in the hierarchy will render as a SQL JOIN along all tables in its inheritance path."[^3]

```text
  classes                       tables (one key: id)
  Player(name)                  player(id, name)            1 Ann | 2 Bo | 3 Cy
   |- Footballer(club)          footballer(id, club)        1 Lions
   |- Cricketer(runs)           cricketer(id, runs)         2 412 | 3 95
       |- Bowler(wickets)       bowler(id, wickets)         3 31
  load bowler 3:  player join cricketer join bowler on id
```

The participants are the **class hierarchy**; a **table for each class** with the fields that the class declares; the **shared key**, which is the primary key of each table and a foreign key to the table above; and the **mapper**, which writes and joins the rows of all the tables on the path of a class.

The following program saves a bowler as three rows, and joins them to load it.

```python run id=row-per-class
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table player (id integer primary key, name text);
create table cricketer (id integer primary key references player(id), runs integer);
create table bowler (id integer primary key references cricketer(id), wickets integer);
""")


class Player:
    def __init__(self, name):
        self.name = name


class Cricketer(Player):
    def __init__(self, name, runs):
        super().__init__(name)
        self.runs = runs


class Bowler(Cricketer):
    def __init__(self, name, runs, wickets):
        super().__init__(name, runs)
        self.wickets = wickets


def insert(p):
    # One row in the table of each class on the path from the root, all with the same key.
    p.id = db.execute("insert into player (name) values (?)", (p.name,)).lastrowid
    if isinstance(p, Cricketer):
        db.execute("insert into cricketer values (?, ?)", (p.id, p.runs))
    if isinstance(p, Bowler):
        db.execute("insert into bowler values (?, ?)", (p.id, p.wickets))


def find_bowler(id):
    sql = """select p.name, c.runs, b.wickets from player p
             join cricketer c on c.id = p.id join bowler b on b.id = c.id where p.id = ?"""
    return Bowler(*db.execute(sql, (id,)).fetchone())


insert(Bowler("Cy", 95, 31))
for table in ("player", "cricketer", "bowler"):
    print(table, db.execute(f"select * from {table}").fetchall())
cy = find_bowler(1)
print(cy.name, cy.runs, cy.wickets)
```

```output
player [(1, 'Cy')]
cricketer [(1, 95)]
bowler [(1, 31)]
Cy 95 31
```

## Example

Each example file saves a footballer, a cricketer and a bowler and prints the four tables. The footballer has rows in two tables, and the bowler has rows in three. It then loads each player by its key. The root row has no type column, so the mapper looks for the key in the subclass tables to find the class, and prints the tables that it used. A query for cricketers joins the player and cricketer tables and finds Bo and Cy, because the bowler also has a cricketer row. The last line deletes the bowler, which removes three rows, children first. The Python file uses SQLite with foreign keys on. The JavaScript, Java, Go and Rust files keep a map for each table, all keyed by the same id.

## Consequences

**Benefits.**

- **Each table is normalized.** Each column belongs to one class, so no column is null only because a row holds another class. The database can make the subclass columns NOT NULL.
- **Links anywhere in the hierarchy.** A foreign key can refer to the player table for any player, or to the cricketer table for cricketers only.
- **The tables match the classes.** The schema is easy to read for a person who knows the class diagram.

**Liabilities.**

**Joins for each load.** An object of a deep class needs a join of all the tables on its path, and a query for the base class needs an outer join to all the subclass tables to find the class of each row. "The joined table inheritance polymorphic queries can use several JOINS which might affect performance when fetching a large number of entities."[^4] Entity Framework Core warns: "In many cases, TPT shows inferior performance when compared to TPH."[^2]

**The mapper must find the class of a row.** Fowler's book notes two ways: a type column in the root table, or a look for the key in each subclass table.[^5] SQLAlchemy uses the first: "the ultimate class to instantiate for a given row is determined by a discriminator column or SQL expression, defined on the base class".[^3] Hibernate uses the second by default: "A discriminator column is not required for this mapping strategy."[^4]

**Each level needs its row.** An object of a class must have a row in the table of every class above it. In the following program, the mapper of `Bowler` writes the root row and its own row, but not the cricketer row. A query for cricketers does not find the bowler.

```python wrong id=missing-middle-row
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table player (id integer primary key, name text);
create table cricketer (id integer primary key references player(id), runs integer);
create table bowler (id integer primary key references cricketer(id), runs integer, wickets integer);
insert into player values (1, 'Bo'), (2, 'Cy');
insert into cricketer values (1, 412);
-- The bowler mapper writes the root row and its own row, and puts runs in the bowler table.
-- It skips the row in the cricketer table.
insert into bowler values (2, 95, 31);
""")

sql = "select p.name from player p join cricketer c on c.id = p.id order by p.id"
print("cricketers:", db.execute(sql).fetchall())
```

```output
cricketers: [('Bo',)]
```

The fix is a mapper that writes a row for each class on the path, as in the `insert` above, in one transaction.

**The superclass tables are hot spots.** Each insert, update and delete touches the root table, and changes to the hierarchy change the tables.

## When not to use it

**When most queries load objects of many classes.** Each such query joins many tables. Use [[single-table-inheritance]], which reads one table.

**When most queries load one concrete class.** A table for each concrete class needs no join. Use [[concrete-table-inheritance]].

**For a flat hierarchy with few fields in the subclasses.** The joins cost more than the null columns that they avoid.

## Variants and related patterns

**A type column in the root.** A discriminator in the root table tells the mapper which tables to join, so it does not need to look in each table. SQLAlchemy and Entity Framework Core use one; Hibernate and Django do not need one.

**Multi-table inheritance in Django.** "Each model corresponds to its own database table and can be queried and created individually. The inheritance relationship introduces links between the child model and each of its parents (via an automatically-created OneToOneField )."[^6]

**The other strategies.** [[single-table-inheritance]] puts the hierarchy in one table, and [[concrete-table-inheritance]] gives each concrete class one table with all its fields. Entity Framework Core gives this advice: "Use TPT only if constrained to do so by external factors."[^2] [[inheritance-mappers]] organizes the mapper classes for any of the three.

## Known uses

- Django: a model that inherits from a concrete model. In [base.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L277-L290), `ModelBase.__new__` adds a `OneToOneField` to the parent with the name `<parent>_ptr`, `on_delete=CASCADE`, `auto_created=True` and `parent_link=True`, unless the model declares its own parent link.[^7]
- SQLAlchemy: joined table inheritance, with a subclass table whose primary key is a foreign key to the base table.[^3]
- Entity Framework Core, for .NET: `UseTptMappingStrategy` or `ToTable` on each type maps the hierarchy as table-per-type.[^2]
- Hibernate and Jakarta Persistence: `@Inheritance(strategy = InheritanceType.JOINED)`. "Each subclass can also be mapped to its own table. This is also called table-per-subclass mapping strategy. An inherited state is retrieved by joining with the table of the superclass."[^4]

## References
