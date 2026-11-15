---
tag: 8P897X
kind: pattern
slug: single-table-inheritance
name: Single Table Inheritance
aliases: [single table inheritance pattern, table per hierarchy, TPH, single-table mapping]
area: software-design/data-access-patterns
level: core
summary: >-
  Single Table Inheritance stores all the classes of an inheritance hierarchy in one table, with a column for each field of each class and a type column that names the class of each row.
family: architectural
polarity: positive
intent: "Represents an inheritance hierarchy of classes as a single table that has columns for all the fields of the various classes."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). Entity Framework Core calls it table-per-hierarchy, and Jakarta Persistence calls it the SINGLE_TABLE strategy.
see_also: [class-table-inheritance, concrete-table-inheritance, inheritance-mappers, identity-field, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/singleTableInheritance.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Single Table Inheritance
    locator: pattern summary
  - url: https://docs.sqlalchemy.org/en/20/orm/inheritance.html
    title: SQLAlchemy 2.0 documentation, Mapping Class Inheritance Hierarchies
    locator: Single Table Inheritance; Joined Table Inheritance
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/inheritance
    title: Entity Framework Core documentation, Inheritance
    locator: Table-per-hierarchy and discriminator configuration; Summary and guidance
  - url: https://docs.jboss.org/hibernate/orm/6.6/userguide/html_single/Hibernate_User_Guide.html
    title: Hibernate ORM 6.6 User Guide
    locator: 3.14.2. Single table
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Single Table Inheritance
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L1987-L2010
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/mapper.py
    locator: Mapper._configure_polymorphic_setter, _set_polymorphic_identity, lines 1987 to 2010
---

## Intent

Single Table Inheritance "Represents an inheritance hierarchy of classes as a single table that has columns for all the fields of the various classes."[^1] A type column in each row names the class of the object that the row holds.

## Problem

"Relational databases don't support inheritance, so when mapping from objects to databases we have to consider how to represent our nice inheritance structures in relational tables."[^1] A program has a class `Player` with the subclasses `Footballer` and `Cricketer`, and `Bowler` is a kind of `Cricketer`. Code asks for all players, or for all cricketers, and expects objects of the right subclass. A table for each class needs joins to build one object. Fowler gives the goal of this pattern: "When mapping to a relational database, we try to minimize the joins that can quickly mount up when processing an inheritance structure in multiple tables."[^1]

## Solution

"Single Table Inheritance maps all fields of all classes of an inheritance structure into a single table."[^1] Each row holds one object. The fields of its class are in their columns, and the columns of the other classes are null. SQLAlchemy describes the same layout: "A particular subclass that has attributes unique to that class will persist them within columns in the table that are otherwise NULL if the row refers to a different kind of object."[^2]

One more column, the discriminator, holds a code for the class of the row. The mapper writes it on insert, and on load it reads it first to know which class to build. Entity Framework Core says: "TPH uses a single table to store the data for all types in the hierarchy, and a discriminator column is used to identify which type each row represents."[^3]

```text
  classes                          table player
  Player(name)                     id | type       | name | club  | runs | wickets
   |- Footballer(club)             1  | footballer | Ann  | Lions | null | null
   |- Cricketer(runs)              2  | cricketer  | Bo   | null  | 412  | null
       |- Bowler(wickets)          3  | bowler     | Cy   | null  | 95   | 31
  all players:  select * from player
  cricketers:   select * from player where type in ('cricketer', 'bowler')
```

The participants are the **class hierarchy**; the **single table**, with the union of the columns of all the classes; the **discriminator column**; and the **mapper**, which writes the code of the class with each row and uses it to choose the class on load.

The following program saves a footballer and a cricketer in one table, and builds each object again from the type column.

```python run id=type-column
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("""create table player (id integer primary key, type text not null,
              name text, club text, runs integer)""")


class Player:
    def __init__(self, name):
        self.name = name


class Footballer(Player):
    def __init__(self, name, club):
        super().__init__(name)
        self.club = club


class Cricketer(Player):
    def __init__(self, name, runs):
        super().__init__(name)
        self.runs = runs


TYPES = {"footballer": Footballer, "cricketer": Cricketer}


def insert(p):
    # One table for the whole hierarchy; the type column says which class the row is.
    kind = {Footballer: "footballer", Cricketer: "cricketer"}[type(p)]
    db.execute("insert into player (type, name, club, runs) values (?, ?, ?, ?)",
               (kind, p.name, getattr(p, "club", None), getattr(p, "runs", None)))


def load(row):
    id, kind, name, club, runs = row
    return Footballer(name, club) if kind == "footballer" else Cricketer(name, runs)


insert(Footballer("Ann", "Lions"))
insert(Cricketer("Bo", 412))
print(db.execute("select * from player").fetchall())
print([type(load(r)).__name__ for r in db.execute("select * from player")])
```

```output
[(1, 'footballer', 'Ann', 'Lions', None), (2, 'cricketer', 'Bo', None, 412)]
['Footballer', 'Cricketer']
```

## Example

Each example file saves a footballer, a cricketer and a bowler in one player table and prints the rows. Each row has values only in the columns of its class, and null in the others. It loads all the players, and the type column chooses the class of each object. A query for cricketers selects the rows of type `cricketer` and `bowler`, because a bowler is a cricketer. The last line counts the null cells in the three subclass columns: 5 of 9. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the table in memory as rows with a null for each unused column.

## Consequences

**Benefits.**

- **No joins.** "Single table inheritance has the advantage of simplicity compared to joined table inheritance; queries are much more efficient as only one table needs to be involved in order to load objects of every represented class."[^2] The Hibernate guide says that "the single table strategy performs the best since it requires access to one table only."[^4]
- **Polymorphic queries are simple.** A query for the base class reads one table, and a query for a subclass adds a condition on the type column.
- **The hierarchy can change.** Fowler's book notes that a field can move up or down the hierarchy without a change to the table.[^5]

**Liabilities.**

**Columns that a subclass needs cannot be NOT NULL.** A column for the runs of a cricketer is null in each footballer row, so the database cannot require it. "Because all subclass columns are stored in a single table, it’s not possible to use NOT NULL constraints anymore, so integrity checks must be moved either into the data access layer or enforced through CHECK or TRIGGER constraints."[^4] Entity Framework Core does the same: "Database columns are automatically made nullable as necessary when using TPH mapping."[^3]

**Wasted space and a wide table.** Each row has a null cell for each field of the other classes. With many subclasses, the table has many columns and most of each row is empty.

**The class must come from the type column, not from the data.** A mapper without a discriminator must guess the class from the columns that are not null. In the following program, a new cricketer who has not batted yet has no runs, and the mapper loads the cricketer as a plain player.

```python wrong id=guess-from-nulls
import sqlite3

db = sqlite3.connect(":memory:")
# No type column: the mapper guesses the class from the columns that are not null.
db.executescript("""
create table player (id integer primary key, name text, club text, runs integer);
insert into player (name, club, runs) values ('Ann', 'Lions', null);
-- Cy is a new cricketer who has not batted yet, so runs is null.
insert into player (name, club, runs) values ('Cy', null, null);
""")


def guess(club, runs):
    if club is not None:
        return "Footballer"
    if runs is not None:
        return "Cricketer"
    return "Player"


print([(n, guess(c, r)) for n, c, r in db.execute("select name, club, runs from player")])
```

```output
[('Ann', 'Footballer'), ('Cy', 'Player')]
```

The fix is a type column that is NOT NULL, written by the mapper from the class of the object.

**One name for each column.** Two subclasses that have fields with the same name and different meanings need different column names, because they share the table.

## When not to use it

**When the subclasses have many fields of their own.** The table becomes wide and sparse. Use [[class-table-inheritance]] or [[concrete-table-inheritance]].

**When the database must check the fields of each subclass.** If each subclass column must be NOT NULL, or must have its own foreign keys, a table for each class lets the database check them.

**When other applications read the table.** Rows of different kinds in one table are harder to read for code that does not know the hierarchy.

## Variants and related patterns

**Class Table Inheritance and Concrete Table Inheritance.** The other two ways to map a hierarchy: one table for each class, or one table for each concrete class. SQLAlchemy supports all three and notes that "The most common forms of inheritance are single and joined table".[^2] Entity Framework Core gives this advice: "In summary, TPH is usually fine for most applications, and is a good default for a wide range of scenarios".[^3] See [[class-table-inheritance]] and [[concrete-table-inheritance]].

**Mixed strategies.** One hierarchy can use one table for some classes and other tables for others. [[inheritance-mappers]] organizes the mapper classes for any of the three strategies.

## Known uses

- SQLAlchemy: a subclass without its own `__tablename__`, with `polymorphic_on` set to the discriminator column in the base class and `polymorphic_identity` in each subclass.[^2] In [mapper.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/mapper.py#L1987-L2010), `_set_polymorphic_identity` sets the discriminator attribute of an object to the `polymorphic_identity` of its mapper, and it raises an error for a mapper that is marked `polymorphic_abstract`.[^6]
- Entity Framework Core, for .NET: table-per-hierarchy is the default mapping of a hierarchy, with a `Discriminator` column.[^3]
- Hibernate and Jakarta Persistence: `@Inheritance(strategy = InheritanceType.SINGLE_TABLE)`. "When omitting an explicit inheritance strategy (e.g. @Inheritance ), Jakarta Persistence will choose the SINGLE_TABLE strategy by default."[^4]
- Ruby on Rails: an Active Record model with a `type` column stores its subclasses in one table.

## References
