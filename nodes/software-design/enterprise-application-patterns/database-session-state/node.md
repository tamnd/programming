---
tag: 9YAYMG
kind: pattern
slug: database-session-state
name: Database session state
aliases: [Database Session State, database-backed session, session table, JDBC session store]
area: software-design/enterprise-application-patterns
level: advanced
summary: >-
  Database session state keeps the data of each session in the database as committed rows, apart from the business data, so that any server can handle the next request.
family: architectural
polarity: positive
intent: "Store session data as committed data in the database."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on session state patterns.
see_also: [client-session-state, server-session-state, serialized-lob, object-serialization, unit-of-work, optimistic-offline-lock, data-mapper, enterprise-application]
sources:
  - url: https://martinfowler.com/eaaCatalog/databaseSessionState.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Database Session State
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 6, Session State; Chapter 17, Session State Patterns, Database Session State
  - url: https://docs.djangoproject.com/en/5.2/topics/http/sessions/
    title: Django 5.2 documentation, How to use sessions
    locator: Configuring the session engine; Clearing the session store
  - url: https://docs.spring.io/spring-session/reference/configuration/jdbc.html
    title: Spring Session reference, JDBC
    locator: Understanding the Session Storage Details; Customizing the Expired Sessions Clean-Up Job
  - url: https://github.com/OWASP/CheatSheetSeries/blob/master/cheatsheets/Session_Management_Cheat_Sheet.md
    title: OWASP Cheat Sheet Series, Session Management Cheat Sheet
    locator: Idle Timeout
---

## Intent

Database session state stores "session data as committed data in the database."[^1] Each request reads the state of its session from the database and writes the changed state back before the request ends. No server keeps the state between requests, so any server can handle the next request.

## Problem

An application with many servers must keep the state of each session from one request to the next. With [[server-session-state]] in the memory of one server, the next request must go to the same server, and the state is lost when that server stops. With [[client-session-state]], each request carries all the state, and large state does not fit.

The database is already shared by all the servers and survives a restart. But session data is not business data. A cart that the user never checks out is not an order, and no report may count it.

## Solution

Keep the session data in the database, apart from the business data. The parts are:

- **The session ID.** The client keeps only a random ID, usually in a cookie, as in [[server-session-state]].
- **The session tables.** One row for each session, with the ID, the user and the time of the last request, and rows for the data of the session. The data can be separate rows, or one serialized value in a [[serialized-lob]].
- **Pending data apart from committed business data.** Session data must not look like business data to the rest of the system. Fowler describes two ways: a field that marks the pending rows, or separate tables for the pending data.[^2]
- **The commit step.** When the user finishes, for example with a checkout, one transaction turns the pending data into business data.
- **The purge.** A user can leave at any time, so a job deletes the sessions that are idle for too long, with their data.

```text
  request -> any server -> reads the session rows -> does the work -> writes them back
  sessions:    s1 ann 10:25     s2 bob 10:01
  cart_lines:  s1 pen 2, s1 ink 1, s2 ink 1      (pending session data)
  checkout s1: one transaction moves the lines of s1 into orders and order_lines
  purge:       deletes s2 and its cart lines, because s2 is idle for 39 minutes
```

In the following program, server `a` commits the state of a session and stops. Server `b` reads the same rows from the database.

```python run id=state-in-committed-rows
import sqlite3

URI = "file:sessions?mode=memory&cache=shared"
server_a = sqlite3.connect(URI, uri=True)
server_b = sqlite3.connect(URI, uri=True)
server_a.execute("""CREATE TABLE session_data (
    session_id TEXT, name TEXT, value TEXT, PRIMARY KEY (session_id, name))""")

# Request 1 goes to server a, which commits the state of the session.
server_a.execute("INSERT INTO session_data VALUES ('s1', 'step', '2'), ('s1', 'plan', 'basic')")
server_a.commit()
server_a.close()  # server a stops

# Request 2 goes to server b, which reads the same committed rows.
rows = server_b.execute(
    "SELECT name, value FROM session_data WHERE session_id = 's1' ORDER BY name").fetchall()
print("server b reads s1:", dict(rows))
```

```output
server b reads s1: {'plan': 'basic', 'step': '2'}
```

## Example

Each example file has two servers that share one database. In the Python file, the database is SQLite in memory, and the other files keep the tables in shared collections. Ann and Bob each put items in a cart. A request of Ann goes to server `b`, which reads the cart that server `a` wrote. Ann then checks out, and one transaction moves the pending cart lines of Ann into the order tables. The sales report reads only the order tables. At 10:40, the purge deletes the session of Bob, who has had no request for 39 minutes, with the pending cart line of that session. The ink of Bob is never counted as a sale.

## Consequences

**Benefits.**

- **Any server can handle any request.** The servers keep no state between requests, so a load balancer can send each request anywhere, and a server can stop at any time.
- **The state survives a restart.** The session data is committed, so a crash or a deployment does not lose it.
- **One store.** The application uses the database that it already has, with its backups and its tools.

**Liabilities.**

**Session data mixed with business data.** In the following program, each request writes the cart line straight into the table of real order lines. Bob never checks out, but the sales report counts the ink of Bob.

```python wrong id=cart-lines-in-the-order-table
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE order_lines (session_id TEXT, item TEXT, qty INTEGER)")

# Each request saves the cart line straight into the table of real order lines.
db.execute("INSERT INTO order_lines VALUES ('s1', 'pen', 2)")  # ann checks out
db.execute("INSERT INTO order_lines VALUES ('s2', 'ink', 1)")  # bob leaves without checking out

sold = db.execute("SELECT SUM(qty) FROM order_lines WHERE item = 'ink'").fetchone()[0]
print("ink sold according to the sales report:", sold)
```

```output
ink sold according to the sales report: 1
```

Keep the pending data in separate tables, or mark it with a field that every query of the business data checks. Turn it into business data only at the commit step.

**A database access on each request.** Each request reads and writes the session rows, which is slower than memory. A cache in front of the database helps, and Django has a `cached_db` backend that writes to both the database and a cache, and reads from the cache first.[^3]

**Old sessions stay.** Django explains: "When a user logs in, Django adds a row to the django_session database table." If the user does not log out, "the row never gets deleted."[^3] It also says: "Django does not provide automatic purging of expired sessions." It recommends the `clearsessions` command, "for example as a daily cron job".[^3] Spring Session JDBC, in contrast, "executes a clean-up job every minute that deletes the expired sessions (and its attributes)."[^4] OWASP says that "All sessions should implement an idle or inactivity timeout."[^5]

**Concurrent requests.** Two requests of the same session, for example from two browser tabs, can read the same rows and overwrite the writes of each other. An [[optimistic-offline-lock]] on the session row finds the conflict.

## When not to use it

**When the state is small.** A few values fit in a signed cookie with [[client-session-state]], with no database access.

**When the state is large and short-lived, and requests can stay on one server.** [[server-session-state]] in memory is faster.

## Variants and related patterns

**Rows or one serialized value.** The data of a session can be normal rows in tables, which queries can read, or one serialized value in a column, which is simpler to save. Spring Session JDBC uses a table for the sessions and a table for the attributes: "By default, the implementation uses SPRING_SESSION and SPRING_SESSION_ATTRIBUTES tables to store sessions."[^4]

**Related patterns.** [[server-session-state]] with a shared store is close to this pattern, but it keeps the data in a serialized form that the database does not understand. The serialized form is [[object-serialization]] or a [[serialized-lob]]. A [[unit-of-work]] can collect the changes of one request and write them in one transaction.

## Known uses

- Django keeps sessions in the database by default: "By default, Django stores sessions in your database".[^3]
- Spring Session JDBC keeps the sessions of a Java web application in a relational database.[^4]

## References
