---
tag: CGW7FS
kind: pattern
slug: record-set
name: Record set
aliases: [Record Set, recordset, data set, disconnected row set]
area: software-design/enterprise-application-patterns
level: advanced
summary: >-
  A record set is an in-memory table of rows and named columns that looks like the result of an SQL query, so other code and data-aware UI tools can read and change it.
family: architectural
polarity: positive
intent: "Keep tabular data in memory in the shape of the result of a query."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [table-module, table-data-gateway, unit-of-work, data-transfer-object, transaction-script, active-record, row-data-gateway, domain-model]
sources:
  - url: https://martinfowler.com/eaaCatalog/recordSet.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Record Set
    locator: pattern summary
  - url: https://docs.python.org/3/library/sqlite3.html
    title: Python documentation, sqlite3, DB-API 2.0 interface for SQLite databases
    locator: Cursor.description
  - url: https://learn.microsoft.com/en-us/dotnet/framework/data/adonet/dataset-datatable-dataview/
    title: Microsoft Learn, DataSets, DataTables, and DataViews, ADO.NET
    locator: introduction
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.sql.rowset/javax/sql/rowset/CachedRowSet.html
    title: Java SE 21 API, javax.sql.rowset.CachedRowSet
    locator: interface description
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Record Set
---

## Intent

A record set is "An in-memory representation of tabular data."[^1] It has rows and named columns, in the same shape as the result of an SQL query. The program can fill it from a query or make it in code, and can then read it, change it and write the changes back.

## Problem

Many tools build user interfaces for relational data with little code. The catalog says that these tools "make display and simple updates ridiculously easy", but that "they have no real facilities in which to place business logic".[^1] The rules then go into stored procedures or into the code of the UI.

The program needs a structure that the UI tools can use as if it were the result of a query, but that other code can also make, check and change.

## Solution

The answer of the catalog is "an in-memory structure that looks exactly like the result of an SQL query but can be generated and manipulated by other parts of the system".[^1] The parts are:

- **The columns.** Each column has a name and usually a type.
- **The rows.** Each row has one value for each column. The code reads a value by the row and the name of the column.
- **The state of each row.** A row can be unchanged, modified, added or deleted. The code that saves the record set writes only the rows that changed.
- **No open connection.** The record set is filled while the connection is open, and is then used without it. It can be passed to the UI, to a [[table-module]], or to another process.

```text
  query --> RecordSet(columns: id, name, balance)
              row 0: 1 | Ann | 12000 | unchanged
              row 1: 2 | Bob |  2550 | modified
              row 2: 3 | Cy  |     0 | added
  UI and table module read and change it; a gateway writes the changed rows back
```

In the following program, `cursor.description` "provides the column names of the last query".[^2] The program keeps the names and the rows after the connection is closed.

```python run id=rows-with-column-names
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("create table account (id integer, name text)")
db.execute("insert into account values (1, 'Ann'), (2, 'Bob')")
cursor = db.execute("select id, name from account order by id")
columns = [d[0] for d in cursor.description]  # the names of the columns of the result
rows = cursor.fetchall()
db.close()

record_set = [dict(zip(columns, row)) for row in rows]  # still usable with no connection
print(columns)
print(record_set)
```

```output
['id', 'name']
[{'id': 1, 'name': 'Ann'}, {'id': 2, 'name': 'Bob'}]
```

## Example

Each example has a `RecordSet` with the columns `id`, `name` and `balance`, and two rows from a query of accounts. The Python example runs the query in an SQLite database in memory. The other examples start from the columns and rows that a database driver would give. The program adds 10.00 to the balance of Bob and adds a row for Cy. It prints each row with its state, finds the names of the rows with a balance over 20.00, and counts the rows that must be saved.

## Consequences

**Benefits.**

- **Works with data-aware tools.** Tables, grids and reports that show query results can show a record set.
- **A place for logic.** Code such as a [[table-module]] can check and change the rows before the UI shows them or the program saves them.
- **No connection while the user works.** The program can read the data, close the connection, and save the changes later.
- **Easy to save changes.** The state of each row tells which rows to insert or update.

**Liabilities.**

**Code that reads the values by position breaks when the query changes.** In the following program, the rows have no names for the columns. A new version of the query adds a column, and the code that reads the third value now gets an email address.

```python wrong id=columns-by-position
rows = [(1, "Ann", 12000), (2, "Bob", 1550)]  # a record set with no names for the columns

# A new version of the query puts the email between the name and the balance.
rows_v2 = [(1, "Ann", "ann@example.org", 12000), (2, "Bob", "bob@example.org", 1550)]


def total_balance(rs):
    return sum(row[2] for row in rs)  # the code finds the balance by its position


print("total:", total_balance(rows))
try:
    print("total:", total_balance(rows_v2))
except TypeError as e:
    print("TypeError:", e)
```

```output
total: 13550
TypeError: unsupported operand type(s) for +: 'int' and 'str'
```

Keep the names of the columns in the record set and read each value by its name, for example `get(i, "balance")`.

**Weak types.** The cells usually have a general type, such as an object. The compiler cannot check the name of a column or the type of a value. Typed data sets, which a tool generates from the schema, solve part of this.

**Logic that is spread out.** A record set has no behavior of its own. If the rules are not in a [[table-module]] or a similar place, they go back into the UI.

**Stale data.** While the record set is not connected, another user can change the same rows in the database. The save needs a check, such as an [[optimistic-offline-lock]].

## When not to use it

**When the program has a domain model.** If the rules are in objects with behavior, the [[domain-model]] and a mapper replace the record set, and the UI shows the objects.

**When the platform has no tools that use it.** The main reason for a record set is the tools that work with it. Without them, a list of [[data-transfer-object|data transfer objects]] or plain records is simpler.

## Variants and related patterns

**ADO.NET DataSet.** "The ADO.NET DataSet is a memory-resident representation of data that provides a consistent relational programming model regardless of the source of the data it contains."[^3] It can hold several tables with constraints and relations between them.[^3]

**Disconnected row sets.** In Java, "A CachedRowSet object is a container for rows of data that caches its rows in memory, which makes it possible to operate without always being connected to its data source."[^4] It connects "while it is reading data to populate itself with rows and again while it is propagating changes back to its underlying data source".[^4]

**Related patterns.** A [[table-module]] puts the business logic for a table next to a record set. A [[table-data-gateway]] can return a record set from a query. The state of each row works like a small [[unit-of-work]]. The book pairs Record Set with Table Module.[^5]

## Known uses

- The ADO.NET `DataSet` and `DataTable`.[^3]
- The Java `CachedRowSet`, a disconnected row set.[^4]

## References
