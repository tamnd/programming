---
tag: NEW-sql-injection
kind: weakness
slug: sql-injection
name: SQL injection
aliases: [SQLi, cwe-89]
area: security/injection
level: core
summary: >-
  SQL injection happens when a program puts outside input into the text of an SQL statement, so the database reads part of the input as SQL code and not as a value.
cwe: 89
abstraction: base
languages_affected: [any-with-sql]
owasp_top10_2025: A05
cwe_top25_2025: 2
history:
  year: 1998
  people: [Jeff Forristal]
  note: Writing as rain.forest.puppy in Phrack 54, Forristal showed that SQL text built from web input could carry more SQL commands to Microsoft SQL Server.
same_as:
  wikidata: Q506059
  wikipedia: SQL_injection
sources:
  - url: https://cwe.mitre.org/data/definitions/89.html
    title: "CWE-89: Improper Neutralization of Special Elements used in an SQL Command ('SQL Injection')"
    locator: Description; Relationships; Common Consequences; Potential Mitigations
  - url: https://top10.owasp.org/2025/A05_2025-Injection/
    title: "OWASP Top 10:2025, A05:2025 Injection"
    locator: Background; How to prevent
  - url: https://cwe.mitre.org/top25/archive/2025/2025_cwe_top25.html
    title: 2025 CWE Top 25 Most Dangerous Software Weaknesses
    locator: the ranked list, rank 2
  - url: http://phrack.org/issues/54/8.html
    title: "rain.forest.puppy, NT Web Technology Vulnerabilities, Phrack Magazine 8(54), article 08, December 1998"
    locator: the section on ODBC and MS SQL Server 6.5
  - url: https://cheatsheetseries.owasp.org/cheatsheets/SQL_Injection_Prevention_Cheat_Sheet.html
    title: OWASP Cheat Sheet Series, SQL Injection Prevention Cheat Sheet
    locator: Primary Defenses, Options 1 to 4; Additional Defenses, Least Privilege
  - url: https://www.postgresql.org/docs/current/protocol-flow.html
    title: PostgreSQL documentation, Frontend/Backend Protocol, Message Flow
    locator: Extended Query
  - url: https://www.sqlite.org/c3ref/bind_blob.html
    title: SQLite C interface, Binding Values To Prepared Statements
    locator: the description of parameters and the bind functions
  - url: https://go.dev/doc/database/sql-injection
    title: Go documentation, Avoiding SQL injection risk
    locator: the whole page
  - url: https://docs.python.org/3.14/library/sqlite3.html#how-to-use-placeholders-to-bind-values-in-sql-queries
    title: The Python Standard Library, sqlite3, Python 3.14
    locator: How to use placeholders to bind values in SQL queries
  - url: https://www.postgresql.org/support/security/CVE-2006-2314/
    title: PostgreSQL security advisory, CVE-2006-2314
    locator: the description and the fixed releases
  - url: https://docs.djangoproject.com/en/stable/topics/db/sql/
    title: Django documentation, Performing raw SQL queries
    locator: Passing parameters into raw()
  - url: https://learn.microsoft.com/en-us/ef/core/querying/sql-queries
    title: EF Core documentation, SQL Queries
    locator: Passing parameters; Dynamic SQL and parameters
  - url: https://codeql.github.com/codeql-query-help/python/py-sql-injection/
    title: CodeQL query help, SQL query built from user-controlled sources (py/sql-injection)
    locator: the query description
  - url: https://docs.astral.sh/ruff/rules/hardcoded-sql-expression/
    title: Ruff rules, hardcoded-sql-expression (S608)
    locator: What it does
  - url: https://github.com/securego/gosec
    title: gosec, Go security checker
    locator: the list of rules, G201 and G202
  - url: https://www.cve.org/CVERecord?id=CVE-2023-34362
    title: CVE-2023-34362, Progress MOVEit Transfer
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2021-27101
    title: CVE-2021-27101, Accellion FTA
    locator: the description and the publication date
  - url: https://www.cve.org/CVERecord?id=CVE-2025-26794
    title: CVE-2025-26794, Exim
    locator: the description and the publication date
  - url: https://ico.org.uk/about-the-ico/media-centre/talktalk-cyber-attack-how-the-ico-investigation-unfolded/
    title: Information Commissioner's Office, TalkTalk cyber attack, how the ICO's investigation unfolded
    locator: the summary of the attack and the penalty
---

## Description

A program sends a statement to a database as text. The database parses the text, and the parse decides which parts are SQL keywords, names and operators, and which parts are values. SQL injection happens when the program makes this text by joining fixed SQL with input from outside. The input then goes through the parser together with the code. When the input holds a quote, the quote ends the string literal, and the rest of the input becomes SQL. The program meant one value. The database sees a different statement.

The root cause is that code and data share one channel. The developer knows where the value starts and ends, but the string does not keep this knowledge, and the database cannot get it back. The fix is to keep the two apart until the database has parsed the code (see Fixed code).

An attacker who controls part of a statement can often read data that the query was not meant to return, change or delete rows, or pass a login check. On some database systems, more SQL commands or operating system commands are possible.[^1] The damage depends on the rights of the database account of the application.

CWE-89 names this weakness. It is a base weakness under CWE-943, improper neutralization of special elements in a data query, and CWE-564 is its variant for Hibernate.[^1] It is rank 2 of the 2025 CWE Top 25.[^3] In the OWASP Top 10:2025 it is part of A05:2025 Injection.[^2]

### History

Jeff Forristal, writing as rain.forest.puppy, described the problem in Phrack 54 in December 1998.[^4] His article was about Microsoft web technology. It showed that an ASP page built SQL text with string concatenation, and that Microsoft SQL Server accepted more than one command in a batch, so input could add a second command. The article had no exploit code. Its advice was to double the single quotes in strings and to check that numeric input has only digits. Its first choice was to call stored procedures and to pass the input as parameters.[^4] Parameters are still the main defence.

### Second-order injection

In second-order injection, the input does not change the first query. The program stores it safely, for example with a parameter. Later, other code reads the value from the database, treats it as trusted, and puts it into the text of a second query. The weakness is in the second query. The Python example file shows it with the harmless name `O'Brien`: the insert is safe, and the later concatenation breaks. Data that comes from the database needs the same care as data that comes from a request.

### Blind and time-based injection

An application does not have to show query results or error messages to be exploitable. In blind injection, an attacker sends many requests and infers data from small differences in the response, such as a page that changes or does not change. In time-based injection, the difference is how long the response takes. For a defender, the lesson is that a generic error page and hidden output do not remove the weakness. They only make it slower to exploit. Monitoring can see the pattern: many similar requests with small changes, and queries that take much longer than usual.

## Vulnerable code

The function below builds the `WHERE` clause with `+`. The second call uses the classic demonstration input `x' OR '1'='1`. The first quote closes the literal, and `OR '1'='1'` is true for every row.

```python wrong id=concatenation
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE orders (id INTEGER, customer TEXT, total REAL)")
db.executemany("INSERT INTO orders VALUES (?, ?, ?)", [(1, "alice", 12.5), (2, "bob", 40.0)])


def find_orders(customer):
    query = "SELECT id, customer, total FROM orders WHERE customer = '" + customer + "'"
    print(query)
    return db.execute(query).fetchall()


print(find_orders("alice"))
print(find_orders("x' OR '1'='1"))
```

```output for=concatenation
SELECT id, customer, total FROM orders WHERE customer = 'alice'
[(1, 'alice', 12.5)]
SELECT id, customer, total FROM orders WHERE customer = 'x' OR '1'='1'
[(1, 'alice', 12.5), (2, 'bob', 40.0)]
```

The same weakness appears with every way to build a string: `%`, `format`, f-strings, template literals, `Sprintf`, `format!` and string interpolation. The example files show it in each language.

## Fixed code

A parameterised query has a placeholder where the value goes. The program sends the SQL text and the values in separate arguments.

```python run id=placeholder
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE orders (id INTEGER, customer TEXT, total REAL)")
db.executemany("INSERT INTO orders VALUES (?, ?, ?)", [(1, "alice", 12.5), (2, "bob", 40.0)])


def find_orders(customer):
    query = "SELECT id, customer, total FROM orders WHERE customer = ?"
    return db.execute(query, (customer,)).fetchall()


print(find_orders("alice"))
print(find_orders("x' OR '1'='1"))
print(find_orders("O'Brien"))
```

```output for=placeholder
[(1, 'alice', 12.5)]
[]
[]
```

The input is now one value. No customer has the name `x' OR '1'='1`, so the query returns no rows. The name `O'Brien` also works, with no escaping.

### How the protocol keeps them apart

The database parses the statement before it sees the value. In SQLite, the program compiles the text with a placeholder (`?`, `?NNN`, `:name`, `@name` or `$name`) into a prepared statement, and then binds a value to each placeholder.[^7] In the extended query protocol of PostgreSQL, the client sends the text in a Parse message and the values in a Bind message.[^6]

```text
Parse    SELECT id FROM orders WHERE customer = $1
Bind     $1 = the bytes x' OR '1'='1, as one text value
Execute
```

The plan of the statement is fixed before the value arrives, so no value can add a clause. The Go documentation says the same of `database/sql`: the driver sends the SQL text and the parameter separately.[^8] Some drivers emulate parameters on the client. They escape each value for the exact server and encoding, and the application never builds the text itself.

### Why escaping is not enough

Escaping tries to make input safe inside a literal. It fails in several ways. The rules depend on the database and its settings. Escaping does not help where the input is not inside quotes, as in `WHERE id = ` followed by a number, or in a table name. The escape function must also know the character encoding. CVE-2006-2314 is an example: in some multibyte encodings, a backslash byte can be the second byte of a character, so a quote escaped as `\'` became a character and a bare quote.[^10] And every developer must remember to escape every value in every query. The OWASP cheat sheet calls manual escaping strongly discouraged.[^5]

### Names that cannot be parameters

A placeholder holds a value. It cannot hold a table name, a column name, a keyword such as `ASC` or `DESC`, or a list of columns. When such a part must change at run time, choose it from a fixed allow-list in the code, and never copy the input into the text.[^5] The Python example file maps the input to a column name with a dictionary and rejects every other input.

### ORMs and raw queries

An ORM or a query builder makes parameters for the values of its normal API. Each one also has a way to run raw SQL, and the weakness comes back there. In Django, `raw()` takes the parameters as a second argument, and the documentation says not to use string formatting on raw queries.[^11] In EF Core, `FromSql` and `ExecuteSql` take a `FormattableString` and make a parameter of each interpolated value, but `FromSqlRaw` and `ExecuteSqlRaw` take a plain string.[^12] HQL and JPQL strings built with `+` are vulnerable in the same way (CWE-564).[^1] A stored procedure that is called with bound arguments is safe only if it does not build dynamic SQL from them.[^5]

## How to detect it

- **Static analysis with taint tracking.** CodeQL follows input from a request to a query function. Its queries are `py/sql-injection`, `js/sql-injection`, `java/sql-injection`, `go/sql-injection` and `cs/sql-injection`.[^13]
- **Pattern linters.** Ruff rule S608, from flake8-bandit, flags SQL text built with `+`, `%`, `format` or f-strings.[^14] gosec rules G201 and G202 flag SQL text built with a format string or with concatenation.[^15] These rules do not know if the value is safe, so they also flag an allow-listed column name. Mark such a line with a comment that names the allow-list.
- **Code review.** Search for query calls with one argument, and for SQL keywords next to `+`, `format`, `${` or `{`. Check each raw query method of the ORM.
- **Tests.** Put a value with a quote, such as `O'Brien`, through each input that reaches the database. A syntax error or a missing row shows that the input is part of the SQL text.
- **Logs.** SQL syntax errors in production logs often mean that input reached the SQL text.

## How to prevent it

- **Bind every value.** Use the placeholders of the driver for every value in every statement, including values that come from the database, from files and from other services.[^5]
- **Allow-list the parts that cannot be parameters.** Table names, column names and sort directions come from a fixed set in the code.[^5]
- **Keep to the safe API of the ORM.** Review each use of raw SQL, and give each raw query its parameters.
- **Run one statement per call.** Some APIs refuse more than one statement in a call. Python's `sqlite3` raises `ProgrammingError` for a second statement in `execute`.[^9] Keep this behaviour on.
- **Use least privilege.** The database account of the application gets only the rights it needs, never administrator rights.[^5] A read-only feature uses a read-only account. Views and stored procedures can limit the rows and the columns that an account sees. This does not stop the injection, but it limits what an injected statement can do.
- **Validate input as a second layer.** A check that an order id has only digits rejects much bad input early. It is not a replacement for parameters, because many valid values, such as names, contain quotes.
- **Show generic errors.** Do not send database error messages to the client. This removes help for an attacker, but the weakness stays until the query is fixed.

## Real cases

| Case | Date | What happened |
|---|---|---|
| TalkTalk | October 2015 | Attackers used SQL injection on old web pages from Tiscali and got access to customer data. The ICO fined TalkTalk £400,000 in 2016, and said that two earlier attacks of the same kind had gone unnoticed.[^19] |
| CVE-2021-27101 | published 2021-02-16 | SQL injection through a crafted `Host` header in Accellion FTA, a file transfer appliance.[^17] |
| CVE-2023-34362 | published 2023-06-02 | SQL injection in the web application of Progress MOVEit Transfer, open to an attacker with no login. The CVE record says that it was exploited in the wild in May and June 2023.[^16] |
| CVE-2025-26794 | published 2025-02-21 | Remote SQL injection in the Exim mail server, in a configuration that uses SQLite for hints data with ETRN serialization.[^18] |
| CVE-2006-2314 | 2006 | The PostgreSQL project showed that escaping a quote as `\'` is unsafe in some multibyte client encodings, and released fixes.[^10] |

## References
