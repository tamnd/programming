---
tag: NEW-enterprise-application
kind: concept
slug: enterprise-application
name: Enterprise application
aliases: [enterprise applications, enterprise application architecture, business application, information system application]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  An enterprise application keeps a lot of persistent data that many people use at the same time, shows it on many screens, follows complex business rules, and must work with other systems.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2002
  people: [Martin Fowler, Dave Rice, Matthew Foemmel, Edward Hieatt, Robert Mee, Randy Stafford]
  note: Patterns of Enterprise Application Architecture (2002) used the term for software such as health care records, payroll and lease accounting. In 2014 Fowler noted that the word enterprise had come to mean a large company, which did not fit his use.
see_also: [transaction-script, domain-model, table-module, service-layer, repository-pattern, model-view-controller, separation-of-concerns, facade-pattern]
same_as:
  wikipedia: Enterprise_software
sources:
  - url: https://martinfowler.com/bliki/EnterpriseApplication.html
    title: Martin Fowler, Enterprise Application, 24 March 2014
    locator: whole article
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Introduction, Enterprise Applications; Chapter 1, Layering, The Three Principal Layers
  - url: https://martinfowler.com/books/eaa.html
    title: Martin Fowler, Patterns of Enterprise Application Architecture, book page
    locator: book description
  - url: https://martinfowler.com/eaaCatalog/
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture
    locator: the groups of the catalog
  - url: https://en.wikipedia.org/wiki/Enterprise_software
    title: Enterprise software, Wikipedia
    locator: lead
  - url: https://docs.python.org/3/library/sqlite3.html#sqlite3-connection-context-manager
    title: sqlite3, DB-API 2.0 interface for SQLite databases, Python documentation
    locator: How to use the connection context manager
---

## Idea

Some programs control a printer, a game or a telephone switch. Others keep health care records, trade foreign exchange, pay salaries or account for leases. Martin Fowler calls the second kind enterprise applications, and he says that there is no formal definition for the term.[^1] Instead, he gives the characteristics that these applications have in common:[^1]

- **Persistent data.** There is a lot of it, usually in a database, and it lives longer than the applications that process it.
- **Concurrent use.** Many people read and change the data at the same time. Many developers do not think much about race conditions. They build on transactions that the database manages.
- **Many screens.** The same data is shown in different ways for different users, and there is also offline batch work.
- **Integration.** The application must work with other applications that were built by other teams over many years, through files, shared databases and messaging.
- **Business logic.** The rules come from the business, not from a simple model, and they interact in surprising ways.

The transaction is the base of most of this. Here, a database rejects an order that would make the stock negative, and the transaction removes the half-done change:

```python run id=idea
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE stock (item TEXT PRIMARY KEY, count INTEGER CHECK (count >= 0))")
db.execute("CREATE TABLE orders (customer TEXT, item TEXT, quantity INTEGER)")
db.execute("INSERT INTO stock VALUES ('widget', 10)")
db.commit()


def place_order(customer, item, quantity):
    try:
        with db:  # one transaction: commit at the end, or roll back on an error
            db.execute("INSERT INTO orders VALUES (?, ?, ?)", (customer, item, quantity))
            db.execute("UPDATE stock SET count = count - ? WHERE item = ?", (quantity, item))
        return "kept"
    except sqlite3.IntegrityError:
        return "rolled back"


print("acme orders 3:", place_order("acme", "widget", 3))
print("bolt orders 9:", place_order("bolt", "widget", 9))
print("orders:", db.execute("SELECT customer, quantity FROM orders").fetchall())
print("stock:", db.execute("SELECT count FROM stock").fetchone()[0])
```

```output for=idea
acme orders 3: kept
bolt orders 9: rolled back
orders: [('acme', 3)]
stock: 7
```

The order of `bolt` was inserted before the update failed. The `with db:` block rolled back the whole transaction, so the insert is gone too.[^6]

## How it works

PoEAA divides an enterprise application into three principal layers: presentation, domain and data source.[^2] The presentation layer shows the screens and takes the requests. The domain layer holds the business logic. The data source layer talks to the database and to the other systems. The patterns of the book are organized around these layers and around the problems between them:[^4]

| Group in the catalog | Problem |
|---|---|
| Domain logic | how to organize the business rules: one procedure for each request, an object model, or one class for each table |
| Data source architectural, object-relational behavioral, structural and metadata mapping | how the objects of the program map to the rows of a relational database |
| Web presentation | how requests from a browser reach the logic and how HTML is made |
| Distribution | how to cut the number of calls between processes |
| Offline concurrency | how to protect data in a business transaction that spans several database transactions |
| Session state | where to keep the state of a session: on the client, on the server or in the database |
| Base | small patterns that the others use, such as Gateway, Registry, Value Object and Money |

The domain logic patterns are the main choice. A [[transaction-script]] puts the logic of each request in one procedure. A [[domain-model]] builds an object model of the business. A [[table-module]] has one object for each table. A [[service-layer]] puts a boundary of operations over any of them. The data source patterns, such as [[repository-pattern]], are in a separate sub-area.

Fowler says that the topics of the book are how to layer an enterprise application, how to organize domain logic, how to tie that logic to a relational database, how to design a web presentation, principles of distributed design, and offline concurrency, which is concurrency that spans transactions.[^3]

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | library | web frameworks such as Django and Flask; ORMs; `sqlite3` and other DB-API drivers |
| JavaScript | library | Node.js web frameworks; database drivers and ORMs |
| TypeScript | library | the frameworks of JavaScript, with types for requests and rows |
| Java | library | Jakarta EE and Spring for transactions, persistence and web screens; JDBC |
| Kotlin | library | the Java frameworks on the JVM, such as Spring |
| C# | library | ASP.NET Core for the screens; Entity Framework Core and ADO.NET for the data |
| Go | stdlib | `net/http` and `database/sql` in the standard library; a driver for each database |
| Rust | library | web frameworks and database crates |
| C | library | database client libraries; rarely used for the business logic |
| C++ | library | database client libraries; rarely used for the business logic |
| Bash | idiom | offline batch jobs and file transfers between systems |
| PowerShell | idiom | batch jobs and the administration of servers and databases |
| SQL | native | the database keeps the data, checks the constraints and runs the transactions |
| Regex | n/a | a pattern can check one field, but it has no data or transactions |
| HTML | idiom | pages and forms are the screens of a web application |
| CSS | n/a | style rules change how a screen looks, but hold no data or rules |

An enterprise application is a kind of system, not a feature of a language. Most of the work is done by frameworks and by the database. The code examples of PoEAA are in Java, C# or both, and Fowler says that he picked these languages because most developers can read them.[^3] He also notes that many of the patterns are now implemented by common frameworks, particularly for database interaction, and that a developer still needs to know the patterns to use the frameworks well.[^3] He calls Ruby on Rails a framework for building enterprise applications.[^1]

SQL has the central role. In the sqlite3 module of Python, a connection used as a context manager commits the transaction when the body finishes without an exception, and rolls it back when the body raises an exception.[^6]

The example files show one small application in each language. It has three layers in one file: a store with a transaction, a business rule with a special deal for one customer, and two screens that show the same data in different ways. An order that the stock cannot fill is rolled back.

## Variations and related ideas

- **Size.** Fowler says that enterprise applications can be large or small, and that small applications that must be built quickly also have problems.[^1] Wikipedia describes enterprise software as software for the complex requirements of larger organizations.[^5] Fowler's use of the term does not depend on the size of the company.[^1]
- **Layered architecture.** The three layers are the basic structure. The domain and the data source do not depend on the presentation.[^2] See [[separation-of-concerns]].
- **Model-view-controller.** A web presentation often separates the model, the view and the controller. See [[model-view-controller]].
- **Integration.** When several applications share data, the same word can mean different things in each one. Fowler gives the example of a customer, which means something different to sales and to technical support.[^1]

## Pitfalls

- **Logic in the screens.** A rule that is written in a screen must be written again in every other screen and in every batch job. Put the rules in the domain layer, so that each screen calls them.
- **Concurrency without transactions.** Two users who read and then write the same record can lose an update. Use a database transaction for each change, and an offline lock for a change that spans several requests.
- **Too much architecture for a small application.** Fowler notes that small applications also matter.[^1] A simple [[transaction-script]] can be the right choice when the rules are few.
- **Many applications, one meaning.** When two applications share a table, a field with one name can have two meanings.[^1] Agree on the meaning of each shared field, or translate it at the boundary.

## Cost

Most of the cost is in the data and in the rules, not in the code that runs on one computer. The data lives longer than the application and is worth more than it,[^1] so each change to the program must keep the old data correct. Each transaction costs a round trip to the database and can make other users wait for locks. Each integration with another system adds a format and a failure mode. Frameworks remove much of the code, but they add decisions about how to use them.[^3]

## History

Fowler wrote PoEAA with contributions from Dave Rice, Matthew Foemmel, Edward Hieatt, Robert Mee and Randy Stafford, and it was published in 2002.[^3] The book started after Fowler and Rice gave talks on J2EE architecture. The first part is a short tutorial, and the rest is a reference to the patterns.[^3] Its working title was "Information Systems Architecture".[^1]

In 2014 Fowler wrote that the word enterprise had come to mean a large, well-established company. By his definition, even small start-ups rely on enterprise applications.[^1]

## References
