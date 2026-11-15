---
tag: XW8V70
kind: concept
slug: enterprise-application
name: Enterprise application
aliases: [enterprise applications, business application, information system, enterprise software]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  An enterprise application keeps a lot of persistent data, serves many users at the same time, shows the data on many screens and works with other systems.
history:
  year: 2002
  people: [Martin Fowler]
  note: Fowler used the term for the kind of system that Patterns of Enterprise Application Architecture (2002) is about, and explained it again in a post of 2014.
see_also: [transaction-script, domain-model, table-module, active-record, data-mapper, unit-of-work, object-relational-impedance-mismatch, model-view-controller]
sources:
  - url: https://martinfowler.com/bliki/EnterpriseApplication.html
    title: Martin Fowler, Enterprise Application, 24 March 2014
    locator: the whole post
  - url: https://martinfowler.com/eaaCatalog/
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture
    locator: introduction
  - url: https://martinfowler.com/books/eaa.html
    title: Martin Fowler, Patterns of Enterprise Application Architecture, book page
    locator: the book description
  - url: https://docs.spring.io/spring-framework/reference/overview.html
    title: Spring Framework reference, Spring Framework Overview
    locator: introduction
  - url: https://learn.microsoft.com/en-us/dotnet/architecture/modern-web-apps-azure/common-web-application-architectures
    title: Microsoft, Architect Modern Web Applications with ASP.NET Core and Azure, Common web application architectures
    locator: What is a monolithic application?; What are layers?
  - url: https://docs.djangoproject.com/en/5.2/topics/db/models/
    title: Django 5.2 documentation, Models
    locator: introduction; Model methods
  - url: https://en.wikipedia.org/wiki/Enterprise_software
    title: Enterprise software, Wikipedia
    locator: introduction
  - url: https://martinfowler.com/bliki/AnemicDomainModel.html
    title: Martin Fowler, Anemic Domain Model, 25 November 2003
    locator: the whole post
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Introduction
---

## Idea

An enterprise application is the kind of program that a business runs on: a payroll system, a booking system, an accounting system or a web shop. Martin Fowler chose the term for the systems in the book Patterns of Enterprise Application Architecture, and says that "there is no formal definition for this term".[^1] The catalog of the book's patterns gives a short description: "Enterprise applications are about the display, manipulation, and storage of large amounts of often complex data; together with the support or automation of business processes with that data."[^2] Fowler also says what they are not: "they are different from embedded systems, control systems, telecoms, or desktop productivity software."[^2]

The name is about the kind of problem, not the size of the company. Fowler: "Enterprise applications can be large or small."[^1] In Fowler's view, "even small start-ups rely on software that I would call an enterprise application".[^1]

The program below shows one of the marks of an enterprise application. Two users want the last seat on a flight. The program does not use threads or locks to protect the seat. The transaction of the database does it.

```python run id=last-seat
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("create table flight (id integer primary key, seats_left integer);"
                 "insert into flight values (7, 1);")


def book(flight_id):
    # The database transaction, not the program, keeps two users from taking one seat.
    with db:
        cursor = db.execute("update flight set seats_left = seats_left - 1 "
                            "where id = ? and seats_left > 0", (flight_id,))
        return "ok" if cursor.rowcount == 1 else "no seat left"


print("first user:", book(7))
print("second user:", book(7))
```

```output
first user: ok
second user: no seat left
```

## How it works

Fowler lists the characteristics that enterprise applications have in common.[^1] The examples of this page show each of them in a small booking system.

- **Persistent data.** "Enterprise applications usually have a lot of persistent data", usually in a relational database. "This data will usually be longer lasting and more valuable than the applications that process it."[^1] The data of a bank outlives many versions of its programs.
- **Concurrent access.** "This data is accessed and manipulated concurrently".[^1] An in-house system can have tens of users and a public web application tens of thousands.[^1] The developers do not usually write locks and critical regions. "Instead they build their thinking on top of transactions managed by databases or specialized transaction management tools."[^1]
- **Many screens.** "Usually the same data is manipulated in different ways in different contexts."[^1] In the examples, an agent sees all the bookings of a flight, and a customer sees all the flights of one person.
- **Batch processing.** "There is also a significant amount of offline (batch) processing that is easily forgotten."[^1]
- **Integration.** A new application is not alone. "Instead you'll need to integrate with other enterprise applications", by file exchange, shared databases or messaging middleware.[^1] In the examples, a night job writes the new bookings to a file for the system of the airline.
- **Conceptual dissonance.** Two systems can use one word for different things: "a customer may mean something quite different to the sales organization than it does to technical support."[^1]
- **Business logic.** Business rules are not designed to be simple. "They are usually a haphazard array of strange conditions that interact in surprising ways."[^1]

These characteristics give the subjects of the patterns. Fowler's book is about how to put the application in layers, how to organize the domain logic, how to connect that logic to a relational database, how to design a web presentation, how to distribute the application, and how to control concurrency across more than one database transaction.[^3]

## In languages

The language matters less than the framework, because the frameworks implement many of the patterns. Fowler: "Frameworks still require you to make decisions about how to use them, and knowing the underlying patterns is essential if you are to make wise choices."[^3]

- **Java.** Jakarta EE and Spring are the main platforms. The Spring documentation starts: "Spring makes it easy to create Java enterprise applications."[^4]
- **C#.** The Microsoft guide to ASP.NET Core architecture says that "most non-trivial business applications benefit from some logical separation into several layers", and that "Logical layering is a common technique for improving the organization of code in enterprise software applications".[^5]
- **Python.** Django gives the persistent data, the transactions and the screens. Its documentation recommends model methods as "a valuable technique for keeping business logic in one place".[^6]
- **Ruby.** Fowler writes: "I would call Ruby on Rails a framework for building enterprise applications".[^1]

The Python, Java and Go examples of this page show the same booking system. Only the Python example uses a real database.

## Variations and related ideas

- **Enterprise software.** In general use, the word enterprise means a large company. Wikipedia defines enterprise software as "computer software that has been specially developed or adapted to meet the complex requirements of larger organizations".[^7] Fowler's term is wider. Fowler notes that the word "has taken on connotations which don't quite fit my usage."[^1]
- **Information systems.** Fowler's working title for the book was Information Systems Architecture. Fowler did not use it, because the term information systems also had connotations that did not fit.[^1]
- **Domain logic.** The patterns for the business logic are [[transaction-script]], [[domain-model]] and [[table-module]], often with a [[service-layer]] on top.[^2]
- **Data source.** The patterns that connect the logic to the database are [[active-record]], [[data-mapper]], [[table-data-gateway]] and [[row-data-gateway]], with the behavioral patterns [[unit-of-work]] and [[identity-map]].[^2] The [[object-relational-impedance-mismatch]] is the reason that this part is hard.

## Pitfalls

- **Concurrency without transactions.** A check in the program, followed by a separate write, lets two users take the last seat. Put the check and the write in one database transaction, or in one statement, as the examples do.
- **Forgotten batch work.** Night jobs, reports and imports are part of the application. Fowler warns that the batch processing "is easily forgotten."[^1]
- **One word, two meanings.** Integration can fail when two systems use the same field name with different meanings.[^1] Agree on the meaning of each field that crosses a system boundary.
- **Logic in the screens.** When each screen has its own copy of a business rule, the copies drift apart. See [[transaction-script]] and [[domain-model]] for ways to keep each rule in one place.

## Cost

Fowler gives the main cost of a rich object model: "The primary cost is the awkwardness of mapping to a database, which typically results in a whole layer of O/R mapping."[^8] For a small enterprise application, simpler patterns cost less. Fowler notes that small systems are a challenge too, because they "need to be built quickly."[^1]

## History

Fowler wrote Patterns of Enterprise Application Architecture with Dave Rice and others, and it was published in 2002.[^3] The book has a short tutorial part and a reference part of about forty patterns, with code examples in Java and C#.[^3] Its introduction defines the term enterprise application.[^1][^9] In 2014, Fowler wrote a post with the definition, and said that no other term that came to mind was better than "enterprise application".[^1]

## References
