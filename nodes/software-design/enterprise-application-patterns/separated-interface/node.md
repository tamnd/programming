---
tag: VFYRRQ
kind: pattern
slug: separated-interface
name: Separated interface
aliases: [Separated Interface, interface in the client package, required interface]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A separated interface is an interface defined in one package and implemented in another, so that the client depends only on the interface and a dependency can point against the layers.
family: architectural
polarity: positive
intent: "Define an interface in a separate package from its implementation."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [program-to-an-interface, interfaces, dependency-injection, gateway, inversion-of-control, service-locator, test-double, mapper]
sources:
  - url: https://martinfowler.com/eaaCatalog/separatedInterface.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Separated Interface
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Separated Interface
  - url: https://go.dev/wiki/CodeReviewComments
    title: Go Wiki, Go Code Review Comments
    locator: Interfaces
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/ServiceLoader.html
    title: Java SE 21 API, java.util.ServiceLoader
    locator: class description
---

## Intent

A separated interface "Defines an interface in a separate package from its implementation."[^1] The client depends only on the interface. The implementation can be in a package that the client does not know, and that the client may not even be allowed to depend on.

## Problem

A good structure of packages has rules about which package may use which. For example, the domain layer may not call the infrastructure, which sends emails and talks to the database. The catalog says: "However, you might need to invoke methods that contradict the general dependency structure."[^1]

The order code in the domain must send a confirmation, but the code that sends emails is in the infrastructure. If the domain imports the email class, the dependency points the wrong way, the domain can no longer be built or tested without the infrastructure, and the packages can form a cycle.

## Solution

The catalog says to "use Separated Interface to define an interface in one package but implement it in another."[^1] The parts are:

- **The interface.** It is in the package of the client, or in a small package of its own, and it is written in the terms of the client.
- **The client.** It uses only the interface. "This way a client that needs the dependency to the interface can be completely unaware of the implementation."[^1]
- **The implementation.** It is in another package, which depends on the package of the interface. The dependency now points from the implementation to the client.
- **The code that connects them.** Something must create the implementation and give it to the client, such as the main program, a factory, a [[plugin]] or [[dependency-injection]].[^2]

```text
  package domain                     package infrastructure
    OrderService --uses--> Notifier  <--implements--  EmailNotifier
                                                       (depends on domain)
  package main: creates EmailNotifier and gives it to OrderService
```

In the following program, the domain code defines the interface `Clock` that it needs. The implementation is a separate class.

```python run id=interface-in-the-client-package
from typing import Protocol


class Clock(Protocol):  # defined in the package that needs it
    def now(self) -> str: ...


def stamp(clock: Clock, text: str) -> str:  # domain code: knows only the interface
    return f"[{clock.now()}] {text}"


class FixedClock:  # implemented in another package
    def now(self) -> str:
        return "10:00"


print(stamp(FixedClock(), "order 7 is confirmed"))
```

```output
[10:00] order 7 is confirmed
```

## Example

Each example file has sections that stand for packages. The domain section defines the interface `Notifier` and the class `OrderService`, which confirms an order through the interface. The infrastructure section has `EmailNotifier`, which implements the interface. The tests section has `RecordingNotifier`, which keeps the messages in a list. Only `main` knows both: it confirms order 7 with the email notifier and order 8 with the recording notifier, as a unit test does.

## Consequences

**Benefits.**

- **Dependencies follow the rules of the layers.** The domain does not depend on the infrastructure, and the cycle is gone.
- **Easy tests.** A test gives the client a [[test-double]] that implements the interface.
- **Changeable implementations.** A new implementation, such as a text message notifier, needs no change of the client. The catalog says that a separated interface "provides a good plug point for Gateway".[^1]

**Liabilities.**

**Without a separated interface, the client creates the concrete class.** In the following program, the domain code makes the email notifier itself, so a unit test of the domain sends a real email.

```python wrong id=domain-creates-the-email-class
class EmailNotifier:  # infrastructure
    def send(self, customer, text):
        print(f"a real email goes to {customer}: {text}")


class OrderService:  # domain code that creates the concrete class itself
    def confirm(self, number, customer):
        EmailNotifier().send(customer, f"order {number} is confirmed")


# A unit test of OrderService cannot replace the notifier.
OrderService().confirm(8, "bob")
```

```output
a real email goes to bob: order 8 is confirmed
```

Define a `Notifier` interface in the domain package, let the email class implement it in the infrastructure, and give the service the notifier in its constructor.

**More types and packages.** Each separated interface adds an interface, and the code that creates the implementation must be somewhere. For a dependency that does not break a rule of the layers, this is extra work.

**The interface must stay stable.** Each change of the interface changes all its implementations.

## When not to use it

**When there is one implementation and no rule to break.** If the client may depend on the implementation and the tests do not need a test double, call the class directly.

**When the interface only copies one class.** An interface with the same methods as its only implementation, in the same package as that class, adds a type and separates nothing.

## Variants and related patterns

**The interface in the client package.** The Go code review guide gives this rule: "Go interfaces generally belong in the package that uses values of the interface type, not the package that implements those values."[^3]

**Service providers.** In Java, "A service is a well-known interface or class for which zero, one, or many service providers exist."[^4] `ServiceLoader` finds the implementations at run time, and "Application code refers only to the service, not to service providers".[^4]

**Related principles and patterns.** The pattern is a way to follow the [[dependency-inversion-principle]] and to [[program-to-an-interface]]. A [[gateway]] behind a separated interface can be replaced by a [[service-stub]] in tests. A [[service-locator]] or [[dependency-injection]] gives the client the implementation.

## Known uses

- Go programs define small interfaces in the package that uses them.[^3]
- The Java `ServiceLoader` locates and loads the providers of a service interface that are deployed in the run time environment.[^4]

## References
