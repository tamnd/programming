---
tag: MM7AGG
kind: feature
owner: python
slug: the-sqlite3-module
name: The sqlite3 module
aliases: [sqlite3, SQLite, DB-API, PEP 249, sqlite3.connect, cursor, placeholders, sqlite3.Row, SQL injection]
area: languages/python/stdlib
level: core
summary: >-
  sqlite3 is the standard interface to SQLite, a database in one file with no server. It follows the DB-API 2.0 of PEP 249: connect, execute SQL with placeholders, fetch rows, then commit.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/sqlite3.html", locator: "sqlite3, DB-API 2.0 interface for SQLite databases"}
proposals: [pep-249]
see_also: [python/pickle-and-object-serialization, python/file-format-modules, python/context-managers, python/the-standard-library-tour]
sources:
  - url: https://docs.python.org/3.15/library/sqlite3.html
    title: sqlite3, DB-API 2.0 interface for SQLite databases, Python 3.15 documentation
    locator: Introduction; Tutorial; connect(); Connection.close(); Row objects; How to use placeholders to bind values in SQL queries; How to use the connection context manager
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0249/
    title: PEP 249, Python Database API Specification v2.0
    locator: Introduction; Module Interface, paramstyle
    accessed: 2026-11-16
---

## Synopsis

```python run id=basics
import sqlite3

con = sqlite3.connect(":memory:")
con.execute("CREATE TABLE movie(title TEXT, year INTEGER, score REAL)")
rows = [("Brazil", 1985, 7.9), ("Alien", 1979, 8.5), ("Heat", 1995, 8.3)]
con.executemany("INSERT INTO movie VALUES(?, ?, ?)", rows)
con.commit()
for title, year in con.execute("SELECT title, year FROM movie WHERE year > ? ORDER BY year", (1980,)):
    print(title, year)
print(con.execute("SELECT count(*), max(score) FROM movie").fetchone())
con.close()
```

```output for=basics
Brazil 1985
Heat 1995
(3, 8.5)
```

## Semantics

**r1.** "SQLite is a C library that provides a lightweight disk-based database that doesn’t require a separate server process".[^1] The sqlite3 module is its standard Python interface. Example: [`basics`](#basics).

**r2.** The module follows PEP 249, the DB-API. "This API has been defined to encourage similarity between the Python modules that are used to access databases."[^2] Code that uses `connect`, `execute`, `fetchone`, `fetchall` and `commit` looks the same with other DB-API drivers.

**r3.** `sqlite3.connect(database)` opens a file. Pass `":memory:"` "to create an SQLite database existing only in memory".[^1] `Connection.execute` makes a cursor and runs one statement on it. A cursor is an iterator over the result rows, and each row is a tuple by default.

**r4.** "Always use placeholders instead of string formatting to bind Python values to SQL statements, to avoid SQL injection attacks".[^1] There are "two kinds of placeholders: question marks (qmark style) or named placeholders (named style)."[^1] Example: [`injection`](#injection).

**r5.** "The INSERT statement implicitly opens a transaction, which needs to be committed before changes are saved in the database".[^1] Example: [`no-commit`](#no-commit).

**r6.** A connection used in a `with` statement "automatically commits or rolls back open transactions when leaving the body of the context manager." It does not close: "The context manager neither implicitly opens a new transaction nor closes the connection."[^1] Example: [`transaction`](#transaction).

**r7.** `sqlite3.Row` is a row factory. "It supports iteration, equality testing, len(), and mapping access by column name and index."[^1] Example: [`named-rows`](#named-rows).

## Examples

```python run id=named-rows
import sqlite3

con = sqlite3.connect(":memory:")
con.row_factory = sqlite3.Row
con.execute("CREATE TABLE lang(name TEXT, first_appeared INTEGER)")
con.execute("INSERT INTO lang VALUES(:name, :year)", {"name": "C", "year": 1972})
row = con.execute("SELECT * FROM lang").fetchone()
print(row["name"], row[1], row.keys(), tuple(row))
con.close()
```

```output for=named-rows
C 1972 ['name', 'first_appeared'] ('C', 1972)
```

The insert uses named placeholders with a dict (rule r4). With `row_factory` set to `sqlite3.Row`, a row works both by name and by index (rule r7).

```python run id=transaction
import sqlite3

con = sqlite3.connect(":memory:")
con.execute("CREATE TABLE lang(name TEXT UNIQUE)")
with con:
    con.execute("INSERT INTO lang(name) VALUES(?)", ("Python",))
try:
    with con:
        con.execute("INSERT INTO lang(name) VALUES(?)", ("Go",))
        con.execute("INSERT INTO lang(name) VALUES(?)", ("Python",))
except sqlite3.IntegrityError as e:
    print("rolled back:", e)
print(con.execute("SELECT name FROM lang").fetchall())
print(con.execute("SELECT 1").fetchone())
con.close()
```

```output for=transaction
rolled back: UNIQUE constraint failed: lang.name
[('Python',)]
(1,)
```

The first block commits. In the second block, the duplicate name raises `IntegrityError`, so the block rolls back and `Go` is not saved either. The connection is still open after both blocks (rule r6), so call `close` yourself.

## Pitfalls

```python wrong id=injection
import sqlite3

con = sqlite3.connect(":memory:")
con.execute("CREATE TABLE users(name TEXT, secret TEXT)")
con.executemany("INSERT INTO users VALUES(?, ?)", [("ann", "a1"), ("bob", "b2")])
name = "nobody' OR '1'='1"
sql = "SELECT name, secret FROM users WHERE name = '%s'" % name
print(con.execute(sql).fetchall())
print(con.execute("SELECT name, secret FROM users WHERE name = ?", (name,)).fetchall())
```

```output for=injection
[('ann', 'a1'), ('bob', 'b2')]
[]
```

The `%` formatting puts the quote from the input into the SQL text, so the condition becomes true for every row. With a placeholder, the whole input is one value and no user has that name (rule r4).

```python wrong id=no-commit
import os
import sqlite3
import tempfile

path = os.path.join(tempfile.mkdtemp(), "app.db")
con = sqlite3.connect(path)
con.execute("CREATE TABLE log(msg TEXT)")
con.execute("INSERT INTO log VALUES('started')")
con.close()
con = sqlite3.connect(path)
print(con.execute("SELECT count(*) FROM log").fetchone())
con.close()
```

```output for=no-commit
(0,)
```

The insert opened a transaction, and `close` does not commit it (rule r5). "Make sure to commit() before closing to avoid losing pending changes."[^1]

```python fails id=bindings expect=exception:ProgrammingError
import sqlite3

con = sqlite3.connect(":memory:")
con.execute("CREATE TABLE t(a, b)")
con.execute("INSERT INTO t VALUES(?, ?)", (1,))
```

For the qmark style, "parameters must be a sequence whose length must match the number of placeholders, or a ProgrammingError is raised."[^1] A single value still needs a tuple, as in `(1,)`.

## Comparison

PEP 249 lets each driver choose a `paramstyle`. The `sqlite3.paramstyle` value is `qmark`, the "Question mark style, e.g. ...WHERE name=?".[^2] Other drivers can use other styles, so the placeholder text can change when you move to another database. The sqlite3 docs say "It’s also possible to prototype an application using SQLite and then port the code to a larger database such as PostgreSQL or Oracle."[^1]
