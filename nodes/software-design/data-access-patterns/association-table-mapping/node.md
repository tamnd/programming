---
tag: 712G7C
kind: pattern
slug: association-table-mapping
name: Association Table Mapping
aliases: [association table mapping pattern, join table mapping, many-to-many mapping, link table]
area: software-design/data-access-patterns
level: core
summary: >-
  An Association Table Mapping saves a many-to-many association between objects as a separate link table, in which each row holds the keys of one pair of linked rows.
family: architectural
polarity: positive
intent: "Saves an association as a table with foreign keys to the tables that are linked by the association."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). The link table itself is much older; relational designers call it an associative table, a join table or a junction table.
see_also: [foreign-key-mapping, identity-field, lazy-load, unit-of-work, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/associationTableMapping.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Association Table Mapping
    locator: pattern summary
  - url: https://en.wikipedia.org/wiki/Associative_entity
    title: Associative entity, Wikipedia
    locator: lead section
  - url: https://docs.sqlalchemy.org/en/20/orm/basic_relationships.html
    title: SQLAlchemy 2.0 documentation, Basic Relationship Patterns
    locator: Many To Many; Deleting Rows from the Many to Many Table; Association Object
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Association Table Mapping
  - url: https://docs.djangoproject.com/en/5.2/ref/models/fields/
    title: Django 5.2 documentation, Model field reference
    locator: ManyToManyField; Database Representation; ManyToManyField.through
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/relationships/many-to-many
    title: Entity Framework Core documentation, Many-to-many relationships
    locator: Understanding many-to-many relationships
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related.py#L1365-L1374
    title: Django 5.2.8 source, django/db/models/fields/related.py
    locator: class ManyToManyField and its docstring, lines 1365 to 1374
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related.py#L1308-L1362
    title: Django 5.2.8 source, django/db/models/fields/related.py
    locator: create_many_to_many_intermediary_model, lines 1308 to 1362
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related_descriptors.py#L1505-L1574
    title: Django 5.2.8 source, django/db/models/fields/related_descriptors.py
    locator: ManyRelatedManager._add_items, lines 1505 to 1574
---

## Intent

An Association Table Mapping "Saves an association as a table with foreign keys to the tables that are linked by the association."[^1] Each row of the link table joins one row on one side to one row on the other side.

## Problem

"Objects can handle multivalued fields quite easily by using collections as field values. Relational databases don't have this feature and are constrained to single-valued fields only."[^1] For a one-to-many association, a [[foreign-key-mapping]] puts the key in the table on the single-valued side. An employee has many skills, and a skill belongs to many employees. Fowler explains why a foreign key cannot do this: "a many-to-many association can't do this because there is no single-valued end to hold the foreign key."[^1]

## Solution

"The answer is the classic resolution that's been used by relational data people for decades: create an extra table to record the relationship."[^1] Wikipedia describes the same design: "A relational database requires the implementation of a base relation (or base table ) to resolve many-to-many relationships", and a base relation of this kind "is called, informally, an associative table".[^2]

The link table has two foreign key columns, one for each side. Each row stands for one link. The two keys together are the primary key of the link table, so one pair cannot appear twice. SQLAlchemy recommends this: the two columns should be "established within either a unique constraint or more commonly as the primary key constraint", which "ensures that duplicate rows won’t be persisted within the table regardless of issues on the application side".[^3]

```text
  memory                               database
  Ann.skills = [Python, SQL]           employee(1, Ann)     skill(1, Python)
  Bo.skills  = [Python]                employee(2, Bo)      skill(2, SQL)
                                       employee_skill(employee_id, skill_id)
                                         (1, 1)  (1, 2)  (2, 1)
  load Ann.skills:  SELECT ... FROM skill JOIN employee_skill ... WHERE employee_id = 1
  load Python's employees: the same table, read from the other side
```

The participants are the two **entity tables**, the **link table** with a pair of keys in each row, and the **mapper**, which writes link rows when it saves a collection and joins through the link table when it loads one.

The following program saves two employees with their skills, and loads the association from both sides through the same link table.

```python run id=save-and-load-links
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table employee (id integer primary key, name text);
create table skill (id integer primary key, name text);
create table employee_skill (
    employee_id integer references employee(id),
    skill_id integer references skill(id),
    primary key (employee_id, skill_id));
insert into skill (name) values ('Python'), ('SQL');
""")


class Employee:
    def __init__(self, name, skills, id=None):
        self.id, self.name = id, name
        self.skills = skills  # A list of skill keys in memory: many-to-many.


def save(e):
    if e.id is None:
        e.id = db.execute("insert into employee (name) values (?)", (e.name,)).lastrowid
    # The link table holds one row for each pair (employee, skill).
    db.execute("delete from employee_skill where employee_id = ?", (e.id,))
    db.executemany("insert into employee_skill values (?, ?)", [(e.id, s) for s in e.skills])


def skills_of(employee_id):
    sql = """select s.name from skill s join employee_skill es on es.skill_id = s.id
             where es.employee_id = ? order by s.id"""
    return [n for (n,) in db.execute(sql, (employee_id,))]


def employees_with(skill_id):
    sql = """select e.name from employee e join employee_skill es on es.employee_id = e.id
             where es.skill_id = ? order by e.id"""
    return [n for (n,) in db.execute(sql, (skill_id,))]


save(Employee("Ann", [1, 2]))
save(Employee("Bo", [1]))
print(db.execute("select * from employee_skill order by 1, 2").fetchall())
print("Ann:", ", ".join(skills_of(1)))
print("Python:", ", ".join(employees_with(1)))
```

```output
[(1, 1), (1, 2), (2, 1)]
Ann: Python, SQL
Python: Ann, Bo
```

This `save` writes the collection by deleting all the link rows of the employee and inserting them again. Fowler's book describes this simple way to save the links.[^4] It is correct because a link row has no data and no other table refers to it.

## Example

Each example file saves two employees and their skills, and prints the link rows as pairs of keys. It loads the skills of Ann, and the employees who know Python, from the same rows. It then changes the skills of Ann and saves the collection by comparing it with the link rows: one pair is deleted and one is inserted, and the other rows stay. The last line tries to insert a pair that is already there, and the primary key of the link table stops it. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the link table in memory as a set of pairs, which also rejects a duplicate pair.

## Consequences

**Benefits.**

- **Each column holds one value.** The tables stay in first normal form, and both sides of the association are rows that the database can join, index and constrain.
- **One table serves both directions.** The skills of an employee and the employees of a skill come from the same rows, read from the other side.
- **The database can check each link.** The two foreign keys make sure that a link refers to rows that exist, and the primary key on the pair stops duplicates.
- **Frameworks make the table for the programmer.** In Django, "Behind the scenes, Django creates an intermediary join table to represent the many-to-many relationship."[^5] In SQLAlchemy, the link table "is automatically subject to INSERT and DELETE statements, as objects are added or removed from the collection."[^3]

**Liabilities.**

**More queries and joins.** A load of a collection needs a join with the link table, or two queries. A list of employees with their skills is a [[lazy-load]] candidate, with the same N plus one risk as other lazy collections.

**A list in a column is not a link table.** A program that stores the collection as text in one column cannot use the database to find the links. In the following program, the skills are a comma-separated list, and a search for SQL also matches PostgreSQL.

```python wrong id=list-in-a-column
import sqlite3

db = sqlite3.connect(":memory:")
# The skills are a list of names in one column.
db.execute("create table employee (id integer primary key, name text, skills text)")
db.execute("insert into employee (name, skills) values ('Ann', 'Python,SQL')")
db.execute("insert into employee (name, skills) values ('Bo', 'PostgreSQL')")

# Who knows SQL? A text search in the list column also matches PostgreSQL.
print(db.execute("select name from employee where skills like '%SQL%'").fetchall())
```

```output
[('Ann',), ('Bo',)]
```

**Saving a changed collection.** Deleting all the link rows and inserting them again is simple, but it writes many rows for a small change. Comparing the collection with the rows writes less but needs the old rows first.[^4] Both ways need a [[unit-of-work]] or a mapper that knows which collections changed.

**Data on the link.** As soon as the link itself has data, such as the date when an employee got a skill, the link table is no longer only a pair of keys. It needs a class of its own, as described in the next sections.

## When not to use it

**For one-to-many associations.** If each skill belonged to one employee, a foreign key in the skill table would be enough. Use a [[foreign-key-mapping]].

**When the link has its own data and behavior.** Map the link table to a class of its own. SQLAlchemy calls this the association object pattern: it is "used when an association table contains additional columns beyond those which are foreign keys to the parent and child (or left and right) tables".[^3]

**For values that belong to one owner.** A list of values, such as the phone numbers of one person, has no second side. Store it in a table that the owner's mapper handles, as in [[dependent-mapping]].

## Variants and related patterns

**A link table with a class.** Django can use a model of the program as the link table: "if you want to manually specify the intermediary table, you can use the through option to specify the Django model that represents the intermediate table that you want to use."[^5] Entity Framework Core calls the link table the "join entity type", and the program can map it to a class of its own, but "EF Core can hide the join entity type and manage it behind the scenes."[^6]

**A link table that is not a table.** Fowler's book notes that the link table need not exist in the database: the mapper can derive links from a query on other tables, for example on a table of the events that connect the two sides.[^4]

**Foreign Key Mapping.** A link table is two foreign key mappings, one to each side. Entity Framework Core says that many-to-many relationships "cannot be represented in a simple way using just a foreign key."[^6] See [[foreign-key-mapping]].

## Known uses

- Django: the [ManyToManyField class in related.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related.py#L1365-L1374) is documented as "Provide a many-to-many relation by using an intermediary model that holds two ForeignKey fields pointed at the two sides of the relation."[^7] [create_many_to_many_intermediary_model](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related.py#L1308-L1362) builds that model with two `ForeignKey` fields, `on_delete=CASCADE`, and `unique_together` on the pair.[^8] [_add_items in related_descriptors.py](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/related_descriptors.py#L1505-L1574) inserts the link rows; the comment says "Add the ones that aren't there already."[^9]
- SQLAlchemy: `relationship(secondary=association_table)`. "Many to Many adds an association table between two classes."[^3]
- Entity Framework Core, for .NET: a many-to-many relationship with skip navigations and a join table that has a composite primary key on the two foreign keys.[^6]

## References
