---
tag: 4N50T2
kind: pattern
slug: serialized-lob
name: Serialized LOB
aliases: [serialized large object, serialized LOB pattern]
area: software-design/data-access-patterns
level: advanced
summary: >-
  Serialized LOB saves a graph of objects as one serialized value, such as JSON or a binary blob, in one column of a table, instead of as rows of related tables.
family: architectural
polarity: positive
intent: "Saves a graph of objects by serializing them into a single large object (LOB), which it stores in a database field."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). Today the most common form is a JSON column, which some databases can index and query.
see_also: [embedded-value, dependent-mapping, memento-pattern, json, data-mapper, object-relational-impedance-mismatch]
sources:
  - url: https://martinfowler.com/eaaCatalog/serializedLOB.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Serialized LOB
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 12, Object-Relational Structural Patterns, Serialized LOB
  - url: https://www.postgresql.org/docs/17/datatype-json.html
    title: PostgreSQL 17 documentation, JSON Types
    locator: 8.14. JSON Types; jsonb Containment and Existence; jsonb Indexing
  - url: https://docs.python.org/3/library/pickle.html
    title: Python documentation, pickle, Python object serialization
    locator: the warning at the top of the page
  - url: https://docs.sqlalchemy.org/en/20/core/type_basics.html
    title: SQLAlchemy 2.0 documentation, The Type Hierarchy
    locator: PickleType; JSON
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/sql/sqltypes.py#L1851-L1862
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/sql/sqltypes.py
    locator: class PickleType, lines 1851 to 1862
  - url: https://docs.djangoproject.com/en/5.2/ref/models/fields/
    title: Django 5.2 documentation, Model field reference
    locator: JSONField
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/json.py#L84-L93
    title: Django 5.2.8 source, django/db/models/fields/json.py
    locator: JSONField.from_db_value, lines 84 to 93
  - url: https://docs.jboss.org/hibernate/orm/6.6/userguide/html_single/Hibernate_User_Guide.html
    title: Hibernate ORM 6.6 User Guide
    locator: 3.2.40. JSON mapping
---

## Intent

Serialized LOB "Saves a graph of objects by serializing them into a single large object (LOB), which it stores in a database field."[^1] The owner keeps one row, and the whole graph of its small objects is in one column of that row.

## Problem

Fowler starts from a graph of small objects: "Object models often contain complicated graphs of small objects. Much of the information in these structures isn't in the objects but in the links between them."[^1] His example is the organization hierarchy of each customer. In memory it is a simple tree. In tables it is a department table with a parent key, and a walk up or down the tree needs one query or one join for each level. But the program only reads and writes the tree as a whole, together with its customer.

## Solution

"Objects don't have to be persisted as table rows related to each other. Another form of persistence is serialization, where a whole graph of objects is written out as a single large object (LOB) in a table".[^1] The mapper serializes the graph when it saves the owner, and puts the result in one column of the owner's row. When it loads the owner, it reads the column and makes the graph again from the value.

The value can be binary or text. A binary LOB (a BLOB) uses the serialization of the language, such as Python `pickle` or Java object streams. A text LOB (a CLOB) uses a format such as JSON or XML, which people and other programs can read. Fowler's book compares the two.[^2]

```text
  memory                          customer table
  Acme                            id  name  departments
   |- Sales                       1   Acme  [{"name":"Sales","children":[
   |   |- Presales                              {"name":"Presales",...},
   |   |- Key accounts                          {"name":"Key accounts",...}]},
   |- Support                                 {"name":"Support","children":[]}]
  save: serialize the tree -> one column      load: parse the column -> the tree
```

The participants are the **owner**, which has a row of its own; the **graph** of small objects that only the owner uses; the **LOB column** in the owner's row; and the **mapper**, which serializes the graph on save and deserializes it on load.

The following program saves a tree of two departments as one JSON value in one column, and reads it back.

```python run id=one-column-for-a-graph
import json
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table customer (id integer primary key, name text, departments text)")

# The graph of departments goes into one column as one serialized value.
departments = [{"name": "Sales", "children": [{"name": "Presales", "children": []}]}]
db.execute("insert into customer values (1, 'Acme', ?)", (json.dumps(departments, separators=(",", ":")),))

(text,) = db.execute("select departments from customer where id = 1").fetchone()
print(text)
print(json.loads(text)[0]["children"][0]["name"])
```

```output
[{"name":"Sales","children":[{"name":"Presales","children":[]}]}]
Presales
```

## Example

Each example file saves the customer Acme with a tree of four departments, and prints the size and the text of the one column that holds the tree. It loads the customer, walks the tree to print each path, and finds the ancestors of Presales in memory, with no query. Then it adds a department under Support and saves the customer again: the whole column is written again, not only the new department. The customer table has one row the whole time. The Python file uses SQLite and the `json` module. The JavaScript and Go files use `JSON.stringify` and `encoding/json`. The Java and Rust standard libraries have no JSON, so those files have a small writer and reader for this one shape.

## Consequences

**Benefits.**

- **One read and one write for the whole graph.** The program loads the tree with its owner, with no join and no query for each level.
- **The schema does not follow the shape of the graph.** A new field in a department changes the code that serializes it, not the tables.
- **The graph keeps its structure.** The order of the children and the nesting go into the value as they are in memory.

**Liabilities.**

**SQL cannot see the parts of the graph.** For the database, the column is one value. A query on the text finds the wrong rows. In the following program, Acme has a Sales department and Bolt has a Presales department, and a search for `%Sales%` returns both customers.

```python wrong id=search-the-text
import json
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table customer (id integer primary key, name text, departments text)")
for id, name, depts in ((1, "Acme", ["Sales"]), (2, "Bolt", ["Presales", "Support"])):
    tree = [{"name": d, "children": []} for d in depts]
    db.execute("insert into customer values (?, ?, ?)", (id, name, json.dumps(tree)))

# Wrong: SQL sees the column as text, so a text match finds the wrong customers.
rows = db.execute("select name from customer where departments like '%Sales%' order by id")
print([name for (name,) in rows])
```

```output
['Acme', 'Bolt']
```

If the program must query the parts, use tables, or use a column type that the database understands. In PostgreSQL, "Testing containment is an important capability of jsonb", and "GIN indexes can be used to efficiently search for keys or key/value pairs occurring within a large number of jsonb documents (datums)."[^3]

**A small change writes the whole value.** The example adds one department and writes all 166 bytes of the column again. Two programs that change different parts of the same graph at the same time can lose one of the changes, unless a lock or a version number protects the row.

**Objects in the LOB have no identity in the database.** No other table can have a foreign key to a department in the LOB. If another part of the program also refers to a department, the LOB has a copy, and the two copies can become different. Put only objects that the owner alone uses in the LOB.

**Binary serialization is not safe for data that you do not control, and it ties the data to the code.** The Python documentation warns: "It is possible to construct malicious pickle data which will execute arbitrary code during unpickling".[^4] A binary value also depends on the classes that wrote it, so a change to a class can make old rows unreadable.

## When not to use it

**When reports or other programs read the parts.** A reporting tool or a second application that uses SQL cannot read inside the column. Use [[dependent-mapping]] or tables that other programs can join.

**When other objects refer to the parts.** A part that has its own identity needs its own row and key.

**When the value is small and flat.** A few fields of a value object fit in columns of the owner's row. Use [[embedded-value]].

## Variants and related patterns

**Binary or text.** A binary LOB is compact and keeps the types of the language, but only the same program can read it. A text LOB in JSON or XML is larger, but other programs and people can read it, and the format does not depend on the classes.

**Database document types.** Some databases parse a JSON column and can index and query it. PostgreSQL has `json` and `jsonb`.[^3] This keeps the single column but removes part of the first liability.

**Related patterns.** Fowler notes that a Serialized LOB "becomes a form of memento", as in [[memento-pattern]].[^1] [[embedded-value]] maps a small value to columns of the owner's row, and [[dependent-mapping]] maps the parts of an owner to rows of their own tables.

## Known uses

- SQLAlchemy: `PickleType` "Holds Python objects, which are serialized using pickle."[^5] Its docstring in [sqltypes.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/sql/sqltypes.py#L1851-L1862) says that it applies `pickle.dumps()` to incoming objects and `pickle.loads()` on the way out.[^6] The `JSON` type represents "a SQL JSON type".[^5]
- Django: `JSONField` stores "JSON encoded data", with Python dictionaries, lists, strings, numbers, booleans and `None`.[^7] [JSONField.from_db_value in json.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/fields/json.py#L84-L93) calls `json.loads` on the value that the database returns.[^8]
- Hibernate: an attribute with `@JdbcTypeCode(SqlTypes.JSON)` is saved as JSON, and an embeddable can be mapped as one JSON value.[^9]
- PostgreSQL: the `json` type stores the text as it is given, and `jsonb` stores it in a decomposed binary form that it can index.[^3]

## References
