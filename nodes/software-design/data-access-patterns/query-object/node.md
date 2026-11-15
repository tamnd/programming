---
tag: 0HFN1F
kind: pattern
slug: query-object
name: Query Object
aliases: [query object pattern, criteria object, criteria API, query builder]
area: software-design/data-access-patterns
level: core
summary: >-
  Query Object represents a database query as a structure of objects that name classes and fields, and the structure writes itself as SQL with the columns from the mapping metadata.
family: architectural
polarity: positive
intent: "An object that represents a database query."
also_known_as: []
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler named the pattern in Patterns of Enterprise Application Architecture (2002). Criteria APIs, query builders and LINQ are later forms of it.
see_also: [interpreter-pattern, metadata-mapping, repository-pattern, specification, composite-pattern, builder-pattern, data-access-object, sql-injection]
sources:
  - url: https://martinfowler.com/eaaCatalog/queryObject.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Query Object
    locator: pattern summary
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/
    title: Microsoft Learn, Language Integrated Query (LINQ)
    locator: introduction; Query expression overview
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 13, Object-Relational Metadata Mapping Patterns, Query Object
  - url: https://www.sqlite.org/lang_expr.html
    title: SQLite documentation, SQL Language Expressions
    locator: 5. The LIKE, GLOB, REGEXP, MATCH, and extract operators
  - url: https://jakarta.ee/specifications/persistence/3.2/jakarta-persistence-spec-3.2
    title: Jakarta Persistence 3.2 specification
    locator: 6. Criteria API; 6.1. Overview
  - url: https://docs.sqlalchemy.org/en/20/orm/queryguide/select.html
    title: SQLAlchemy 2.0 documentation, Writing SELECT statements for ORM Mapped Classes
    locator: introduction
  - url: https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/sql/selectable.py#L6238-L6253
    title: SQLAlchemy 2.0.57 source, lib/sqlalchemy/sql/selectable.py
    locator: Select.where, lines 6238 to 6253
  - url: https://docs.djangoproject.com/en/5.2/topics/db/queries/
    title: Django 5.2 documentation, Making queries
    locator: Complex lookups with Q objects
  - url: https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/query_utils.py#L39-L80
    title: Django 5.2.8 source, django/db/models/query_utils.py
    locator: class Q, lines 39 to 80
---

## Intent

Query Object is "An object that represents a database query."[^1] The program builds the query from objects that name classes and fields. The query object then writes itself as SQL, with the tables and columns that the mapping gives.

## Problem

Fowler states the problem in two parts. First, "SQL can be an involved language, and many developers aren't particularly familiar with it. Furthermore, you need to know what the database schema looks like to form queries."[^1] Second, the usual cure, a finder method for each query with the SQL inside, does not help with queries that the program builds at run time, such as a search form with optional fields, and it repeats the column names in each finder.[^1] When a column changes its name, each finder that names it must change, and a finder that is missed fails at run time.

The same problem appears outside SQL. Microsoft describes it for C#: "Traditionally, queries against data are expressed as simple strings without type checking at compile time or IntelliSense support."[^2]

## Solution

"A Query Object is an interpreter, that is, a structure of objects that can form itself into a SQL query. You can create this query by referring to classes and fields rather than tables and columns."[^1]

The participants are:

- **Criterion.** One condition on one field, such as `country = 'NZ'` or `goals > 10`. It holds the field name, the operator and the value.
- **Group.** A node that joins other conditions with `and` or `or`. Because a group holds conditions, and is a condition itself, the query is a tree.
- **Query object.** The class of the objects that the query finds, the list of criteria, and the order. It has the method that writes the SQL.
- **Data map.** The [[metadata-mapping]] that turns each field into its column. The query object reads it when it writes the SQL, and only there.

Fowler connects the pattern to the rest of the mapping layer: it pays off mainly with a domain model and a [[data-mapper]], and serious use of it needs [[metadata-mapping]].[^3]

```text
  Query(Player)                          to_sql(data map)
    and                                    select ... from player
    +-- country  =  'NZ'       ------>     where nation = ?
    +-- goals    >  10                       and goals_scored > ?
    order by name                          order by name
                                           params: ['NZ', 10]
```

The following program turns a list of criteria on the fields of a player into SQL on the columns of the table.

```python run id=criteria-to-sql
class Criterion:
    def __init__(self, op, field, value):
        self.op, self.field, self.value = op, field, value


# The query names fields of the class. The data map turns them into columns.
COLUMNS = {"name": "name", "country": "nation", "goals": "goals_scored"}


def to_sql(table, criteria):
    where = " and ".join(f"{COLUMNS[c.field]} {c.op} ?" for c in criteria)
    return f"select * from {table} where {where}", [c.value for c in criteria]


query = [Criterion("=", "country", "NZ"), Criterion(">", "goals", 10)]
print(to_sql("player", query))
```

```output
('select * from player where nation = ? and goals_scored > ?', ['NZ', 10])
```

## Example

Each example file has a `Player` class and a data map in which the field `country` maps to the column `nation`, and the field `goals` maps to the column `goals_scored`. The query object is a tree of criteria and `or` groups, with an order. Each node has two methods: one writes the node as SQL and collects the parameter values, and one tests an object in memory. The program builds two queries, prints the SQL and the parameters of each, and prints the players found. Then the column `nation` is renamed: only the data map changes, and the first query, not changed, gives the new SQL and the same players. The Python file runs the SQL in SQLite and also tests the objects in memory; the other files have no database, so they print the same SQL and find the rows with the in-memory method.

## Consequences

**Benefits.**

- **The query uses the names of the domain.** The code that asks for players from NZ names the field `country`, not the column `nation`.
- **A change to the schema is a change in one place.** The data map holds the column names, so a rename does not touch the queries.
- **Queries can be built at run time.** A search form can add one criterion for each field that the user filled in, with no string building.
- **The values are always parameters.** The query object puts each value in the parameter list and never in the SQL text, so a value cannot change the SQL. This is the main defence against [[sql-injection]].
- **The same query can run in two places.** Because the query is data, a second interpreter can test objects in memory, for example objects that the session holds and has not yet written.

**Liabilities.**

**Finder methods with their own SQL repeat the column names.** This is the problem that the pattern solves. In the following program, the column `nation` was renamed to `country_code`. One finder was changed and the other was not, and the error shows only when that finder runs.

```python wrong id=finders-repeat-columns
import sqlite3

db = sqlite3.connect(":memory:")
# The column nation was renamed to country_code in the last migration.
db.execute("create table player (id integer primary key, name text, country_code text, goals_scored integer)")
db.execute("insert into player values (1, 'Ann', 'NZ', 14)")


# Wrong: each finder method holds its own SQL with the column names in it.
# The rename was made in one finder and not in the other.
def find_by_country(country):
    return db.execute("select name from player where country_code = ?", (country,)).fetchall()


def find_scorers(country, goals):
    sql = "select name from player where nation = ? and goals_scored > ?"
    return db.execute(sql, (country, goals)).fetchall()


print(find_by_country("NZ"))
try:
    print(find_scorers("NZ", 10))
except sqlite3.OperationalError as e:
    print("error:", e)
```

```output
[('Ann',)]
error: no such column: nation
```

**A home-made query object covers only part of SQL.** Joins, grouping, subqueries and window functions each need new kinds of nodes. Most projects with a query object of their own also keep a way to send plain SQL.

**The SQL is out of sight.** The statement that runs is not in the source, so a slow query is harder to find, and the program must log the SQL to show it.

**Two interpreters can give different answers.** In the examples, `starts` becomes `like 'B%'` in SQL and a prefix test in memory. These are not the same test. In SQLite, in a LIKE pattern, "Any other character matches itself or its lower/upper case equivalent (i.e. case-insensitive matching)", and the percent sign and the underscore are wildcards.[^4] So the name `bo` matches `like 'B%'` and fails the prefix test, and a value with `%` or `_` in it needs an escape character in SQL.

## When not to use it

**When the queries are few and fixed.** A small number of finder methods, or a [[table-data-gateway]] with named queries, is shorter and shows the SQL.

**When the ORM already has one.** SQLAlchemy, Django, Jakarta Persistence and LINQ each give a query object that covers much more of SQL than a home-made one. Write the query in their form.

**For reports and heavy SQL.** A report with many joins and aggregates is often clearer as SQL that a person can read and tune.

## Variants and related patterns

**Criteria APIs.** The query is a tree of objects that the program builds with method calls, as in the Jakarta Persistence Criteria API.[^5]

**Query builders.** The objects mirror SQL itself, with `select`, `where` and `join`, and not the domain. SQLAlchemy Core is a query builder, and the ORM adds the mapped classes to it.[^6]

**Language-integrated queries.** LINQ puts the query in the syntax of C#, as a query expression that the compiler checks.[^2] A LINQ provider for a database turns the query expression into SQL.

**Query by example.** The program fills some fields of an object, and the query finds the objects whose fields have the same values.

**Related patterns.** A query object is an [[interpreter-pattern]] whose nodes are a [[composite-pattern]]. The chain of `where` calls is a form of [[builder-pattern]]. A [[specification]] is a predicate object of the domain, which a query object can turn into SQL. A [[repository-pattern]] often takes a query object or a specification as the argument of its find method. Django and SQLAlchemy build the query first and run it later, which is lazy evaluation of the query.

## Known uses

- SQLAlchemy: "SELECT statements are produced by the select() function which returns a Select object."[^6] [Select.where in selectable.py at 2.0.57](https://github.com/sqlalchemy/sqlalchemy/blob/ea3de258957bcae6303bd764c9e8b3e118789535/lib/sqlalchemy/sql/selectable.py#L6238-L6253) adds each criterion to the tuple `_where_criteria`, and the docstring says that the new criterion is "joined to the existing clause via AND, if any."[^7]
- Django: a `QuerySet` is a query object, and a `Q` object is a criterion that can be combined with others. "When an operator is used on two Q objects, it yields a new Q object."[^8] [class Q in query_utils.py at 5.2.8](https://github.com/django/django/blob/47fe39af56ecd0ad73b9c7562511015e96b91b80/django/db/models/query_utils.py#L39-L80) is a tree node with the connectors `AND`, `OR` and `XOR`.[^9]
- Jakarta Persistence: "The Jakarta Persistence Criteria API is used to define queries through the construction of object-based query definition objects, rather than use of the string-based approach of the Jakarta Persistence query language".[^5] Hibernate and EclipseLink implement it.
- C# and .NET: "The variables in a query expression are all strongly typed."[^2] Entity Framework Core is a LINQ provider that turns a query expression over a `DbSet` into SQL.

## References
