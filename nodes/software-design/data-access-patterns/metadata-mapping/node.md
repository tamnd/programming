---
tag: PC8KPH
kind: pattern
slug: metadata-mapping
name: Metadata Mapping
aliases: [metadata mapping pattern, mapping metadata, metadata-driven mapping]
area: software-design/data-access-patterns
level: advanced
summary: >-
  Metadata Mapping holds the mapping between fields and columns as data, and generic code, by reflection or by generated code, reads it to load, insert and update objects.
family: architectural
polarity: positive
intent: "Holds details of object-relational mapping in metadata."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). Most object-relational mappers since then use it, with the metadata in annotations, class attributes or mapping files.
see_also: [data-mapper, inheritance-mappers, runtime-reflection, annotations, syntactic-macros, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/metadataMapping.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Metadata Mapping
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 13, Object-Relational Metadata Mapping Patterns, Metadata Mapping
  - url: https://docs.jboss.org/hibernate/orm/6.6/userguide/html_single/Hibernate_User_Guide.html
    title: Hibernate ORM 6.6 User Guide
    locator: 3.5. Naming strategies; 3.5.1. ImplicitNamingStrategy
  - url: https://docs.sqlalchemy.org/en/20/orm/mapping_styles.html
    title: SQLAlchemy 2.0 documentation, ORM Mapped Class Overview
    locator: ORM Mapping Styles; Declarative Mapping; Imperative Mapping
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/persistence.py#L327-L359
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/persistence.py
    locator: _collect_insert_commands, lines 327 to 359
  - url: https://docs.djangoproject.com/en/5.2/ref/models/meta/
    title: Django 5.2 documentation, Model _meta API
    locator: class Options
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L1081-L1099
    title: Django 5.2.8 source, django/db/models/base.py
    locator: Model._save_table, lines 1081 to 1099
---

## Intent

Metadata Mapping "Holds details of object-relational mapping in metadata."[^1] The program states once, as data, which class goes to which table and which field goes to which column. Generic code reads that data to load, insert and update objects of any mapped class.

## Problem

Fowler describes the code that this pattern removes: "Much of the code that deals with object-relational mapping describes how fields in the database correspond to fields in in-memory objects. The resulting code tends to be tedious and repetitive to write."[^1] A hand-written mapper for each class repeats the same steps with different names: list the columns, list the values in the same order, read each column back into its field. With many classes, this code is large, and each copy is a place for a mistake.

## Solution

"A Metadata Mapping allows developers to define the mappings in a simple tabular form, which can then be processed by generic code to carry out the details of reading, inserting, and updating the data."[^1]

The participants are the **data map** for each class, which names the class and the table and holds a **column map** for each field; and the **generic mapper**, which builds the SQL and moves the values from the metadata. The metadata can be in the code, in a file, or in annotations on the class.

There are two ways for the generic code to get to the fields.[^2] A **reflective program** reads and writes the fields by name at run time, with the reflection of the language. **Code generation** reads the metadata before the program runs and writes ordinary mapper code for each class. Reflection is simpler to change; generated code is plain code that the compiler checks.

```text
  metadata                                    generic mapper
  Player -> table player                      insert(obj): for each column map,
     id      -> id                                 column list  += column
     name    -> name                               value list   += field of obj
     country -> nation                        find(cls, id): for each column map,
  Club   -> table club                             field of new obj = column of row
     id -> id, name -> title, city -> city
```

The following program builds an insert statement and its values for a player from a data map, with no code that names the fields of `Player`.

```python run id=insert-from-metadata
class Player:
    def __init__(self, name, country):
        self.name, self.country = name, country


# The mapping is data: pairs of field and column. Generic code reads it.
PLAYER_MAP = ("player", [("name", "name"), ("country", "nation")])


def insert_sql(obj, data_map):
    table, columns = data_map
    cols = ", ".join(c for _, c in columns)
    marks = ", ".join("?" for _ in columns)
    return f"insert into {table} ({cols}) values ({marks})", [getattr(obj, f) for f, _ in columns]


print(insert_sql(Player("Ann", "NZ"), PLAYER_MAP))
```

```output
('insert into player (name, nation) values (?, ?)', ['Ann', 'NZ'])
```

## Example

Each example file has two classes, `Player` and `Club`, and a data map for each. In both classes the field `name` maps to a different column: `name` in the player table and `title` in the club table. The field `country` maps to the column `nation`. The program prints the two maps, then inserts a player and a club, and prints the insert statement that the one generic mapper built for each. It finds both by key, changes the country of the player, prints the update statement, and prints the rows. The Python file runs the statements in SQLite. The other files keep the rows in maps and print the same statements. Python, JavaScript, Java and Go read and write the fields by name at run time. Rust has no run-time reflection, so a `macro_rules!` macro reads the metadata at compile time and generates the code for each struct, which is the code generation form of the pattern.

## Consequences

**Benefits.**

- **Less code.** One generic mapper serves all the classes, and a new class needs a new data map and no new mapper.
- **The mapping is in one place.** A change of a column name is a change to one line of metadata.
- **The column list and the values come from the same data.** They cannot be in a different order.
- **Other tools can read the metadata.** The same maps can create the tables, check the schema or build forms.

**Liabilities.**

**Hand-written mapping code is easy to get wrong.** This is the problem the pattern solves. In the following program, the column list and the list of values are written separately, in a different order, so the name and the country of the player change places.

```python wrong id=hand-written-columns
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table player (id integer primary key, name text, nation text)")


class Player:
    def __init__(self, id, name, country):
        self.id, self.name, self.country = id, name, country


# Wrong: hand-written mapping code for each class. The column list and the
# list of values are written separately, and here they are in a different order.
def insert_player(p):
    db.execute("insert into player (id, name, nation) values (?, ?, ?)", (p.id, p.country, p.name))


insert_player(Player(1, "Ann", "NZ"))
print(db.execute("select name, nation from player").fetchall())
```

```output
[('NZ', 'Ann')]
```

**The generic code is harder to read and to debug.** The SQL does not appear in the source, and a mistake in the metadata shows only at run time when reflection is used.

**Reflection has a cost and needs access.** It is slower than direct field access, and it needs fields that the mapper can reach. In Java, the example uses public fields for this reason. Code generation removes the cost but adds a build step.

**Special cases still need code.** A field that maps to two columns, or a value that needs a conversion, needs more metadata or a hook in the generic mapper.

## When not to use it

**When there are few classes.** For two or three tables, a hand-written [[data-mapper]] is short and clear.

**When an ORM already holds the metadata.** SQLAlchemy, Django, Hibernate and Entity Framework Core use this pattern inside, so the program writes the metadata in their form and not a generic mapper of its own.

## Variants and related patterns

**Where the metadata lives.** It can be a table of data in the code, as in the examples; class attributes, as in Django models; annotations, as in Jakarta Persistence; or a separate mapping file.

**Metadata from names.** A mapper can derive most of the metadata from the names of the fields, and keep explicit metadata only for the differences. Hibernate does this with naming strategies: "When an entity does not explicitly name the database table that it maps to, we need to implicitly determine that table name."[^3]

**Related patterns.** The metadata is read with [[runtime-reflection]], or written as [[annotations]], or turned into code with [[syntactic-macros]] or a code generator. A generic [[data-mapper]] is the usual reader of the metadata. Fowler's [[query-object]] uses the same metadata to turn a query on fields into SQL on columns.[^2]

## Known uses

- SQLAlchemy: the declarative and imperative mapping styles both produce a `Mapper` that holds the metadata. "The Declarative Mapping is the typical way that mappings are constructed in modern SQLAlchemy."[^4] [_collect_insert_commands in persistence.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/persistence.py#L327-L359) builds the parameters of an INSERT from the map `mapper._propkey_to_col[table]`, from each attribute key to its column.[^5]
- Django: "The model _meta API is at the core of the Django ORM. It enables other parts of the system such as lookups, queries, forms, and the admin to understand the capabilities of each model."[^6] [Model._save_table in base.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/base.py#L1081-L1099) reads `cls._meta` and its `local_concrete_fields` to choose the fields to save.[^7]
- Hibernate and Jakarta Persistence: annotations such as `@Entity`, `@Table` and `@Column`, or a mapping file, hold the metadata, and a naming strategy fills in the names that the mapping does not give.[^3]

## References
