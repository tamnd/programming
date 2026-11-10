---
tag: NEW-dependency-injection
kind: pattern
slug: dependency-injection
name: Dependency injection
aliases: [DI, constructor injection, inversion of control]
area: software-design/design-principles
level: core
summary: >-
  An object receives its collaborators from outside, usually as constructor parameters, instead of creating them or looking them up. One place in the program chooses them.
family: architectural
polarity: positive
intent: "Separate the configuration of the services that an object uses from the use of those services, so the object depends on roles and the program chooses the implementations in one place."
also_known_as: [Inversion of Control (for the containers)]
history:
  year: 2004
  people: [Martin Fowler]
  note: Fowler's article of January 2004 gave the name Dependency Injection to what the lightweight containers of the time called Inversion of Control, and compared it with the Service Locator.
see_also: [strategy-pattern, singleton-pattern, abstract-factory-pattern, interfaces, higher-order-functions, closures, unit-testing]
sources:
  - url: https://martinfowler.com/articles/injection.html
    title: Martin Fowler, Inversion of Control Containers and the Dependency Injection pattern, 23 January 2004
    locator: Components and Services; A Naive Example; Inversion of Control; Forms of Dependency Injection; Using a Service Locator; Deciding which option to use; Concluding Thoughts
  - url: https://blog.ploeh.dk/2010/02/03/ServiceLocatorisanAnti-Pattern/
    title: Mark Seemann, Service Locator is an Anti-Pattern, 3 February 2010
    locator: whole post
  - url: https://docs.spring.io/spring-framework/reference/core/beans/dependencies/factory-collaborators.html#beans-constructor-vs-setter-injection
    title: Spring Framework reference, Dependency Injection
    locator: Constructor-based or setter-based DI?
  - url: https://learn.microsoft.com/en-us/dotnet/core/extensions/dependency-injection#scope-validation
    title: .NET dependency injection, Microsoft Learn
    locator: Scope validation
  - url: https://docs.python.org/3.14/library/typing.html#typing.Protocol
    title: The Python Standard Library, typing, Python 3.14
    locator: typing.Protocol
  - url: https://github.com/python/cpython/blob/v3.14.0/Lib/socketserver.py#L203-L208
    title: CPython v3.14.0 source, Lib/socketserver.py
    locator: BaseServer.__init__, lines 203 to 208
  - url: https://github.com/golang/go/blob/go1.23.0/src/net/http/client.go#L57-L61
    title: Go go1.23.0 source, src/net/http/client.go
    locator: type Client, field Transport, lines 57 to 61
---

## Intent

Separate the configuration of the services that an object uses from the use of those services.[^1] The object declares the roles it needs, such as a clock or a mailer, and receives objects for those roles from outside. One place in the program, the **composition root**, chooses the implementations and connects them.

Dependency injection is a small technique with a large name. In most code it is a constructor parameter. This page explains why that parameter matters, what the alternatives are, and when a container that does the wiring is worth its cost.

## Problem

A service sends reminders for invoices that are overdue. A first version finds its collaborators by itself:

```python norun reason=fragment
import datetime
import smtplib


class ReminderService:
    def remind(self, invoice):
        late = (datetime.date.today() - invoice.due).days
        if late > 0:
            smtplib.SMTP("mail.example.com").send_message(...)
```

The code works in production. It has three problems that all come from one decision: the service chooses its own collaborators.

- A test cannot run it. The test cannot set the date, and every run sends real mail.
- The program cannot configure it. A second deployment with another mail server needs a change to the class.
- A reader cannot see what it needs. The dependencies are inside the method bodies.

Fowler's article starts with the same shape: a `MovieLister` class that creates its own `MovieFinder` and so depends on both the interface and the implementation.[^1] The forces are these. The class must work with any implementation of a role. The program must choose the implementations in one place. And the class should state what it needs, so that a reader, a compiler or a test can see it.

## Solution

Give the collaborators to the object. The class declares parameters with the types of the roles, and stores what it receives. It never names an implementation.

```text
  composition root (main)
    clock  = FixedClock(40)          chooses the implementations
    mailer = ConsoleMailer()
    service = ReminderService(clock, mailer)
                    |
                    v
  ReminderService ----uses----> Clock   (role: today())
                  ----uses----> Mailer  (role: send(to, text))
                                  ^           ^
                       FixedClock |           | ConsoleMailer, RecordingMailer
```

Fowler describes three forms.[^1]

- **Constructor injection.** The constructor takes the collaborators. The object is complete when it exists.
- **Setter injection.** A setter method or a property takes each collaborator after construction.
- **Interface injection.** The class implements an interface whose method receives the collaborator.

Fowler's own advice is to start with constructor injection. A constructor with parameters makes clear what a valid object is, and it lets the fields be immutable. He also lists the limits of constructors: many parameters, parameters that are known only by their position, and several ways to build a valid object. In those cases he accepts setters.[^1] The Spring documentation gives the same advice: constructors for mandatory dependencies and setters for optional ones. It adds that a constructor with many arguments is a sign that the class has too many responsibilities.[^3]

The example files implement `ReminderService` with a `Clock` and a `Mailer` in each language. `main` is the composition root, and a test wires the same class to a `RecordingMailer`. In Python, the roles are `typing.Protocol` classes, so an implementation matches by its methods and needs no base class.[^5]

### The alternative: a service locator

A service locator is a registry that the class asks for its collaborators. It also separates configuration from use. The class depends on the locator, and the locator holds the implementations.[^1] The difference shows when the configuration is incomplete:

```python fails id=locator expect=exception:KeyError
from dataclasses import dataclass


class Locator:
    _services = {}

    @classmethod
    def register(cls, name, service):
        cls._services[name] = service

    @classmethod
    def get(cls, name):
        return cls._services[name]


@dataclass
class Invoice:
    number: int
    customer: str
    due: int


class FixedClock:
    def today(self):
        return 40


class ReminderService:
    def __init__(self):  # the constructor says nothing about what it needs
        pass

    def remind(self, invoice):
        late = Locator.get("clock").today() - invoice.due
        if late > 0:
            Locator.get("mailer").send(invoice.customer, f"invoice {invoice.number} is {late} days overdue")


Locator.register("clock", FixedClock())  # the mailer was never registered
service = ReminderService()
service.remind(Invoice(18, "bo@example.com", 45))
print("not late: no mail, so no error yet")
service.remind(Invoice(17, "ana@example.com", 31))
```

The missing mailer is found only when an invoice is late, which can be weeks after a deployment. With constructor injection, the service cannot be built without a mailer. Fowler names this difference: with injection, the constructor shows the dependencies, and with a locator, a reader must search the source for calls to the locator.[^1]

Fowler did not reject the locator. He wrote that for the classes of one application the two are close, and that injection is the better choice for a class that many applications use.[^1] Seemann later argued that a service locator is an anti-pattern, because it hides the dependencies of a class and turns a mistake that a compiler could catch into an error at run time.[^2] The block above shows that run-time error.

### A plain argument is often enough

Most of the benefit comes from one rule: the code that makes a decision does not fetch its own inputs. When a dependency is a value, pass the value. When it is one operation, pass a function. A class with an injected clock is often a function that wants today's date:

```python run id=plain-argument
from dataclasses import dataclass


@dataclass(frozen=True)
class Invoice:
    number: int
    customer: str
    due: int


def reminder(invoice: Invoice, today: int) -> str | None:
    """Decides. Reads nothing from outside, sends nothing."""
    late = today - invoice.due
    return f"invoice {invoice.number} is {late} days overdue" if late > 0 else None


def send_reminders(invoices, today, send=print):
    """Acts. The caller passes the date and the way to send."""
    for invoice in invoices:
        text = reminder(invoice, today)
        if text:
            send(f"to {invoice.customer}: {text}")


invoices = [Invoice(17, "ana@example.com", 31), Invoice(18, "bo@example.com", 45)]
send_reminders(invoices, today=40)

# The tests need no doubles for the decision, and a list for the action.
assert reminder(invoices[1], today=45) is None
assert reminder(invoices[1], today=50) == "invoice 18 is 5 days overdue"
sent = []
send_reminders(invoices, today=50, send=sent.append)
print(sent)
```

```output
to ana@example.com: invoice 17 is 9 days overdue
['to ana@example.com: invoice 17 is 19 days overdue', 'to bo@example.com: invoice 18 is 5 days overdue']
```

`reminder` is a pure function. A test calls it with a day and checks the result. `send_reminders` takes the sending operation as a parameter, so a test passes `sent.append`. This is dependency injection with no interface, no class and no container. In a language with first-class functions, a role with one operation is a function type. See [[higher-order-functions]] and [[closures]].

## Example

Each example file builds `ReminderService` with a clock and a mailer, sends the reminders for two invoices in `main`, and runs the same class against a recording mailer. Each language describes the roles in its own way:

| Language | Role | Wiring |
|---|---|---|
| Python | `typing.Protocol` | constructor parameters |
| JavaScript | any object with the methods | one options object |
| TypeScript | `interface`, checked by shape | constructor parameters |
| Java | `interface`; a lambda for a role with one method | constructor, `final` fields |
| C# | `interface` | primary constructor |
| Go | small interface, satisfied implicitly | constructor function |
| Rust | `trait`, as a generic parameter | `new` with generic types |
| C++ | abstract class | references in the constructor |
| C | function pointer and context pointer | a struct literal |

## Consequences

**Benefits.**

- **The class can be tested alone.** A test passes doubles for the roles. No mail server, no clock, no database. See [[unit-testing]].
- **The configuration is in one place.** The composition root is the only code that names the implementations. Changing the mail server changes one line.
- **The dependencies are visible.** The constructor lists them. A class whose constructor takes nine parameters shows its size, and that is useful information.[^3]
- **The implementations can vary.** The injected role is often a [[strategy-pattern]]. A program can wrap one in a [[decorator-pattern]] or a [[proxy-pattern]] at the composition root, and the class does not change.

**Liabilities.**

- **Indirection.** A reader of the class sees `self._mailer.send` and must look at the composition root to learn which mailer runs. With a container, that information is in configuration or annotations, further from the code.
- **Interfaces with one implementation.** A team that injects everything can end up with an interface for every class, each with one implementation and one test double. That is cost with no variation to pay for it.
- **Containers add their own rules.** A container creates objects with a lifetime: one for the application, one for each request, or one for each use. A service with a short lifetime that is held by one with a long lifetime lives too long. The .NET documentation says that a scoped service resolved from the root provider acts as a singleton, and the provider checks for this in development.[^4] Errors of this kind come from the container, not from the program's logic.
- **Wiring errors at startup.** With a container, a missing registration is an error when the container first builds the object. That can be at startup or at the first request. It is later than a compiler error for a constructor call written by hand.

## When not to use it

Do not inject what does not vary. A class that formats a date with a fixed pattern does not need the formatter injected. Inject a dependency when there is a second implementation, or when a test needs a double because the real one is slow, has side effects, or is not deterministic: the clock, the network, the file system, random numbers.

Do not add a container to a small program. A `main` function that calls constructors is a composition root. It is checked by the compiler and readable in one place. Every example on this page wires by hand.

Do not use setter injection for a required dependency. An object between its constructor and its last setter call is not valid, and nothing stops code from using it in that state.[^1][^3]

Do not inject the container. A class that receives the container and asks it for services is a service locator with a different name, and it has the hidden dependencies of one.[^2]

Do not inject a class to replace a value. If the code needs today's date, a parameter `today` is clearer than a `Clock` with one method, as the plain argument example shows.

## Variants and related patterns

**Inversion of control.** Fowler wrote that "inversion of control" is too generic a term, because every framework inverts control. He used the name Dependency Injection for the specific inversion in which a container supplies the dependencies of an object.[^1]

**Service Locator.** The class asks a registry for its collaborators. It separates configuration from use, but each class depends on the locator, and the dependencies are not visible in the constructor.[^1][^2]

**Singleton.** A singleton gives global access to one instance. Injection with a single instance keeps the "one instance" and drops the global access: the composition root creates one object and passes it to everyone who needs it. See [[singleton-pattern]].

**Abstract Factory and Factory Method.** When a class must create objects of a role at run time, not receive one, inject a factory. See [[abstract-factory-pattern]] and [[factory-method-pattern]].

**Strategy.** An injected role with one operation that varies is a strategy, chosen by the composition root instead of by the client. See [[strategy-pattern]].

**Template Method.** A subclass that overrides a hook is the inheritance form of the same idea: the varying part comes from outside the base class. Injection does it by composition. See [[template-method-pattern]].

**Interfaces.** Injection works because the class depends on a role, not a class. See [[interfaces]].

## Known uses

- CPython: `socketserver.BaseServer.__init__` in [Lib/socketserver.py at v3.14.0](https://github.com/python/cpython/blob/v3.14.0/Lib/socketserver.py#L203-L208) takes `RequestHandlerClass` as a constructor parameter. The server stores it and creates an instance of it for each request. A program injects its handler class, and the server never names one.[^6]
- Go: the `Transport` field of `http.Client` in [net/http/client.go at go1.23.0](https://github.com/golang/go/blob/go1.23.0/src/net/http/client.go#L57-L61) takes a `RoundTripper`, the role that makes one HTTP request. The comment says that `DefaultTransport` is used if the field is nil. Tests and proxies set the field, and the client code does not change.[^7]

## References
