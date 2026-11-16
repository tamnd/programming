---
tag: 9KGS1K
kind: feature
owner: python
slug: database-libraries
name: Database libraries
aliases: [DB-API, PEP 249, sqlite3, SQLAlchemy, Psycopg, paramstyle, SQL injection, ORM]
area: languages/python/ecosystem
level: core
summary: >-
  PEP 249, the DB-API, gives database modules one shape: connect, cursor, execute, fetch, commit. sqlite3 is in the standard library. Psycopg and SQLAlchemy are on PyPI. Bind values with placeholders.
grammar: null
spec: {standard: null, url: "https://peps.python.org/pep-0249/", locator: "PEP 249, Python Database API Specification v2.0"}
see_also: [python/the-python-package-index, python/cli-and-automation-libraries]
sources:
  - url: https://peps.python.org/pep-0249/
    title: "PEP 249: Python Database API Specification v2.0"
    locator: Introduction; Globals, paramstyle; Cursor Objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/sqlite3.html
    title: sqlite3, DB-API 2.0 interface for SQLite databases
    locator: introduction; How to use placeholders to bind values in SQL queries; How to use the connection context manager
    accessed: 2026-11-16
  - url: https://docs.sqlalchemy.org/en/20/intro.html
    title: Overview, SQLAlchemy 2.0 Documentation
    locator: Overview
    accessed: 2026-11-16
  - url: https://www.psycopg.org/psycopg3/docs/
    title: Psycopg 3 documentation
    locator: introduction
    accessed: 2026-11-16
  - url: https://pkg.go.dev/database/sql
    title: Package sql
    locator: Overview
    accessed: 2026-11-16
---

## Synopsis

```python run id=sqlite
import contextlib
import sqlite3

with contextlib.closing(sqlite3.connect(':memory:')) as con:
    con.execute('CREATE TABLE lang(name TEXT, year INTEGER)')
    with con:
        con.executemany('INSERT INTO lang VALUES (?, ?)',
                        [('Python', 1991), ('Go', 2009), ('C', 1972)])
    cur = con.execute('SELECT name FROM lang WHERE year > ? ORDER BY year', (1980,))
    print(cur.fetchall())
```

```output for=sqlite
[('Python',), ('Go',)]
```

A database module of Python follows the DB-API of PEP 249. The program connects, executes SQL with placeholders such as `?`, fetches the rows and commits. `sqlite3` is in the standard library. Psycopg is the adapter for PostgreSQL, and SQLAlchemy is a toolkit and an ORM that works on top of the adapters.

## Semantics

**r1.** PEP 249 "has been defined to encourage similarity between the Python modules that are used to access databases."[^1] The aim is "code that is generally more portable across databases".[^1]

**r2.** A module gives a connection, and the connection gives cursors. A cursor is "used to manage the context of a fetch operation."[^1] Example: [`sqlite`](#sqlite).

**r3.** The global `paramstyle` is a "String constant stating the type of parameter marker formatting expected by the interface."[^1] The values are `qmark` (`WHERE name=?`), `numeric` (`WHERE name=:1`), `named` (`WHERE name=:name`), `format` (`WHERE name=%s`) and `pyformat` (`WHERE name=%(name)s`).[^1] Example: [`paramstyle`](#paramstyle).

**r4.** "SQLite is a C library that provides a lightweight disk-based database that doesn’t require a separate server process".[^2] The module `sqlite3` "provides an SQL interface compliant with the DB-API 2.0 specification described by PEP 249".[^2]

**r5.** "beware of using Python’s string operations to assemble queries, as they are vulnerable to SQL injection attacks."[^2] Bind the values with placeholders. Example: [`injection`](#injection).

**r6.** "A Connection object can be used as a context manager that automatically commits or rolls back open transactions when leaving the body of the context manager."[^2] "The context manager neither implicitly opens a new transaction nor closes the connection."[^2] Example: [`rollback`](#rollback).

**r7.** "Psycopg 3 is a newly designed PostgreSQL database adapter for the Python programming language."[^4] It "presents a familiar interface for everyone who has used Psycopg 2 or any other DB-API 2.0 database adapter".[^4]

**r8.** "The SQLAlchemy SQL Toolkit and Object Relational Mapper is a comprehensive set of tools for working with databases and Python."[^3] "SQLAlchemy is designed to operate with a DBAPI implementation built for a particular database".[^3]

## Examples

`sqlite3` uses the `qmark` style, and it also accepts the `named` style with a dict (rule r3):

```python run id=paramstyle
import contextlib
import sqlite3

print(sqlite3.paramstyle)
with contextlib.closing(sqlite3.connect(':memory:')) as con:
    row = con.execute('SELECT :a + :b', {'a': 2, 'b': 3}).fetchone()
    print(row)
```

```output for=paramstyle
qmark
(5,)
```

A query built with string formatting runs the text of the attacker. A placeholder sends the same text as a value (rule r5):

```python run id=injection
import contextlib
import sqlite3

with contextlib.closing(sqlite3.connect(':memory:')) as con:
    con.execute('CREATE TABLE users(name TEXT)')
    con.executemany('INSERT INTO users VALUES (?)', [('ada',), ('bob',)])
    name = "x' OR '1'='1"
    unsafe = con.execute(f"SELECT name FROM users WHERE name = '{name}'").fetchall()
    safe = con.execute('SELECT name FROM users WHERE name = ?', (name,)).fetchall()
    print('formatted:', unsafe)
    print('placeholder:', safe)
```

```output for=injection
formatted: [('ada',), ('bob',)]
placeholder: []
```

The context manager rolls back when the body raises an exception (rule r6):

```python run id=rollback
import contextlib
import sqlite3

with contextlib.closing(sqlite3.connect(':memory:')) as con:
    con.execute('CREATE TABLE t(x INTEGER)')
    try:
        with con:
            con.execute('INSERT INTO t VALUES (1)')
            raise RuntimeError('stop')
    except RuntimeError:
        pass
    print(con.execute('SELECT count(*) FROM t').fetchone())
```

```output for=rollback
(0,)
```

## Pitfalls

Do not build SQL with f-strings, `%` or `+` from values that a user gives (rule r5). Use the placeholders of the module (example [`injection`](#injection)).

The placeholder depends on the module (rule r3). `sqlite3` uses `?`, and a module with the `format` or `pyformat` style uses `%s`. Read the `paramstyle` of the module. The `%s` of such a module is a placeholder, not Python string formatting.

`with con:` does not close the connection (rule r6). Close it, or use `contextlib.closing`, as each example on this page does.

## Comparison

"Package sql provides a generic interface around SQL (or SQL-like) databases."[^5] "The sql package must be used in conjunction with a database driver."[^5] Go has the generic interface in the standard library. "Register makes a database driver available by the provided name."[^5] In Python, PEP 249 is a specification, and each module implements it on its own (rule r1).

[^1]: [PEP 249: Python Database API Specification v2.0](https://peps.python.org/pep-0249/)
[^2]: [sqlite3, DB-API 2.0 interface for SQLite databases](https://docs.python.org/3/library/sqlite3.html)
[^3]: [Overview, SQLAlchemy 2.0 Documentation](https://docs.sqlalchemy.org/en/20/intro.html)
[^4]: [Psycopg 3 documentation](https://www.psycopg.org/psycopg3/docs/)
[^5]: [Package sql](https://pkg.go.dev/database/sql)
