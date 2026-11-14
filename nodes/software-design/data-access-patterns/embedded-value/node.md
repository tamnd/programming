---
tag: Q42R2C
kind: pattern
slug: embedded-value
name: Embedded Value
aliases: [embedded value pattern, composite column mapping, embeddable, complex type mapping]
area: software-design/data-access-patterns
level: core
summary: >-
  An Embedded Value saves a small value object, such as an amount of money or a date range, in columns of its owner's table instead of in a table of its own.
family: architectural
polarity: positive
intent: "Maps an object into several fields of another object's table."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). ORMs give it as composite columns, embeddable classes and complex types.
see_also: [identity-field, foreign-key-mapping, association-table-mapping, dependent-mapping, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/embeddedValue.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Embedded Value
    locator: pattern summary
  - url: https://en.wikipedia.org/wiki/Value_object
    title: Value object, Wikipedia
    locator: lead section
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Embedded Value
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/complex-types
    title: Entity Framework Core documentation, Complex types
    locator: introduction; A simple example; Mutability
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/descriptor_props.py#L277-L331
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/descriptor_props.py
    locator: CompositeProperty._create_descriptor, lines 277 to 331
  - url: https://docs.sqlalchemy.org/en/20/orm/composites.html
    title: SQLAlchemy 2.0 documentation, Composite Column Types
    locator: introduction
  - url: https://learn.microsoft.com/en-us/ef/core/modeling/owned-entities
    title: Entity Framework Core documentation, Owned Entity Types
    locator: Mapping owned types with table splitting; By-design restrictions
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/descriptor_props.py#L185-L190
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/orm/descriptor_props.py
    locator: class CompositeProperty and its docstring, lines 185 to 190
---

## Intent

An Embedded Value "Maps an object into several fields of another object's table."[^1] The value has no row and no key of its own. Its fields are columns in the row of the object that holds it.

## Problem

A good object model has many small objects. Fowler writes: "Many small objects make sense in an OO system that don't make sense as tables in a database. Examples include currency-aware money objects and date ranges."[^1] These are value objects: "two value objects are equal when they have the same value , not necessarily being the same object ."[^2] The default way to map a class is to give it a table, "no sane person would want a table of money values."[^1] A table for each value adds a key that means nothing, a join for each load, and a row that two owners can share by mistake.

## Solution

"An Embedded Value maps the values of an object to fields in the record of the object's owner."[^1] When the mapper saves the owner, it writes the fields of each value object to columns of the owner's row. When it loads the owner, it reads these columns and builds a new value object from them.

```text
  memory                                         database: employment
  Employment                                     id | person | start_date | end_date   | salary_amount | salary_currency
    person = "Ann"                               1  | Ann    | 2024-01-01 | 2024-12-31 | 5000          | EUR
    period = DateRange(2024-01-01, 2024-12-31)  -> start_date, end_date
    salary = Money(5000, EUR)                    -> salary_amount, salary_currency
```

The participants are the **owner**, an object with an identity and a row of its own; the **value object**, which has no identity and is compared by its fields; and the **owner's mapper**, which writes the value into a group of columns and builds it again on load. The value object does not need a mapper of its own.

The following program saves an employment with a date range and a salary in one row, and loads it again.

```python run id=embed-two-values
import sqlite3
from dataclasses import dataclass

db = sqlite3.connect(":memory:")
db.execute("""create table employment (id integer primary key, person text,
              start_date text, end_date text, salary_amount integer, salary_currency text)""")


@dataclass(frozen=True)
class DateRange:  # A value object: no key, compared by its fields.
    start: str
    end: str


@dataclass(frozen=True)
class Money:
    amount: int
    currency: str


class Employment:
    def __init__(self, person, period, salary, id=None):
        self.id, self.person, self.period, self.salary = id, person, period, salary


def insert(e):
    # Each value object goes into columns of the owner's row, not into a table of its own.
    sql = "insert into employment values (null, ?, ?, ?, ?, ?)"
    p, s = e.period, e.salary
    e.id = db.execute(sql, (e.person, p.start, p.end, s.amount, s.currency)).lastrowid


def find(id):
    sql = "select * from employment where id = ?"
    id, person, start, end, amount, currency = db.execute(sql, (id,)).fetchone()
    return Employment(person, DateRange(start, end), Money(amount, currency), id)


insert(Employment("Ann", DateRange("2024-01-01", "2024-12-31"), Money(5000, "EUR")))
print(db.execute("select * from employment").fetchall())
ann = find(1)
print(ann.person, "|", ann.period, "|", ann.salary)
print("equal by value:", ann.salary == Money(5000, "EUR"))
```

```output
[(1, 'Ann', '2024-01-01', '2024-12-31', 5000, 'EUR')]
Ann | DateRange(start='2024-01-01', end='2024-12-31') | Money(amount=5000, currency='EUR')
equal by value: True
```

The loaded `Money` is a new object, but it is equal to `Money(5000, "EUR")` because a value object is compared by its fields.

## Example

Each example file saves two employments, one with an end date and one without, and prints the flat rows. A missing end date is a null column. It loads both and builds new `DateRange` and `Money` values from the columns. Ann gets a raise: the program puts a new `Money` value into her employment and saves it, and the salary of Bo does not change. The last query finds the salaries above 4500 EUR with the columns of the value, which the database can read and index as other columns. The Python file uses SQLite. The JavaScript, Java, Go and Rust files keep the table in memory as flat rows of columns.

## Consequences

**Benefits.**

- **One row, one read.** The owner and its values load with one query and save with one statement, without joins.
- **The database can query the parts.** Each field of the value is a column, so a query can filter, sort and index on the amount or the currency.
- **No key that means nothing.** The value object has no identity, so the table needs no key for it and no [[identity-field]] in the class.
- **The object model keeps its small types.** Code works with `Money` and `DateRange`, which can hold the rules of the value, and not with loose columns.

**Liabilities.**

**A value that is shared changes for all its owners.** Fowler's book names value objects as the clear case for the pattern.[^3] A program that gives a value a table and a key can make two owners point to the same row. In the following program, Ann and Bo share one money row, and a raise for Ann also changes the salary of Bo.

```python wrong id=shared-value-row
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table money (id integer primary key, amount integer, currency text);
create table employment (id integer primary key, person text, salary_id integer references money(id));
insert into money values (1, 5000, 'EUR');
-- Bo has the same salary as Ann, so the mapper reuses the row of that value.
insert into employment values (1, 'Ann', 1), (2, 'Bo', 1);
""")

# Ann gets a raise. The program changes the money row of Ann.
db.execute("update money set amount = 5500 where id = (select salary_id from employment where person = 'Ann')")
sql = "select e.person, m.amount from employment e join money m on m.id = e.salary_id order by e.id"
print(db.execute(sql).fetchall())
```

```output
[('Ann', 5500), ('Bo', 5500)]
```

The fix is to keep the salary in the columns of each employment, and to make the value objects immutable, so that a change is a new value and not a write to a shared object. "Value objects should be immutable".[^2] Entity Framework Core gives the same advice for its complex types: "Because a reference-type instance can be shared by multiple properties, mutating one of its properties changes the value everywhere it is used."[^4]

**Wide tables and column names.** Each value adds columns, and two values of the same type, such as a home address and a work address, need a prefix for each group of columns.

**Null is not a value.** If all the columns of a value are null, the mapper must decide whether the value is missing or is a value with null fields. SQLAlchemy builds the composite on access only when the object is already in the database or when one of its columns is not null, and it sets all the columns to `None` when the attribute is set to `None`.[^5]

## When not to use it

**When the object has an identity.** If other objects refer to it, or if it changes over time and must stay the same object, it is an entity. Give it a table and a key, and use a [[foreign-key-mapping]].

**For a collection of values.** A list of values does not fit in a fixed group of columns. Use a table that the owner's mapper writes, as in [[dependent-mapping]], or a serialized column. Entity Framework Core stores a collection of complex types only as JSON: "Collections require JSON."[^4]

**When the value is large or rarely read.** A large value in the owner's row is read each time the owner is read. Keep it in another table and load it with a [[lazy-load]].

## Variants and related patterns

**Composite columns.** SQLAlchemy says: "Sets of columns can be associated with a single user-defined datatype, which in modern use is normally a Python dataclass . The ORM provides a single attribute which represents the group of columns using the class you provide."[^6]

**Complex types and owned types.** In Entity Framework Core, "A complex type groups several properties into a single .NET type that is contained within an entity type; it doesn't have an identity of its own and cannot be tracked or queried independently."[^4] On a relational database, "the complex type does not get its own table. Instead, its properties are saved inline as additional columns on the containing entity's table".[^4] Owned entity types also map, "by default", to "the same table as the owner", but they have a hidden key, and "Instances of owned entity types cannot be shared by multiple owners".[^7]

**Serialized LOB.** A value that is too complex for a fixed set of columns can be saved as one serialized column. See [[serialized-lob]].

**Dependent Mapping.** When the owned objects need rows of their own, for example a list of them, the owner's mapper writes them to another table. See [[dependent-mapping]].

## Known uses

- SQLAlchemy: `composite()` maps a dataclass to a group of columns. The [CompositeProperty class in descriptor_props.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/descriptor_props.py#L185-L190) defines the composite attribute, which stands for several columns as one attribute.[^8] Its [_create_descriptor method](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/orm/descriptor_props.py#L277-L331) builds the value with `self.composite_class(*values)` when the attribute is read, and writes each part back to its column attribute when it is set.[^5]
- Entity Framework Core, for .NET: complex types and owned entity types with table splitting store the properties of a value as columns of the owner's table.[^4][^7]
- Java Persistence: `@Embeddable` classes with `@Embedded` fields, which the provider maps to columns of the owner's table.

## References
