---
tag: 24RQE8
kind: pattern
slug: service-locator
name: Service locator
aliases: [service registry]
area: software-design/design-principles
level: advanced
summary: >-
  A class asks a known registry object for its collaborators, so it does not depend on their concrete classes, but it depends on the locator and its needs are hidden.
family: architectural
polarity: positive
intent: "Give the classes of a program one object that knows how to get hold of all the services that they need, so the classes are independent of the concrete implementations of those services."
also_known_as: [Registry (for the singleton form)]
history:
  year: 2004
  people: [Martin Fowler, Mark Seemann]
  note: Fowler described the service locator in January 2004 as the alternative to dependency injection; Mark Seemann called it an anti-pattern in 2010.
see_also: [dependency-injection, inversion-of-control, singleton-pattern, program-to-an-interface, design-principle]
sources:
  - url: https://martinfowler.com/articles/injection.html
    title: Martin Fowler, Inversion of Control Containers and the Dependency Injection pattern, 23 January 2004
    locator: Using a Service Locator; A Dynamic Service Locator; Using both a locator and injection with Avalon; Service Locator vs Dependency Injection; Significant Revisions
  - url: https://blog.ploeh.dk/2010/02/03/ServiceLocatorisanAnti-Pattern/
    title: Mark Seemann, Service Locator is an Anti-Pattern, 3 February 2010
    locator: the whole post and its updates
  - url: https://pkg.go.dev/database/sql
    title: Go standard library, package database/sql
    locator: func Register; func Open
---

## Intent

Give the classes of a program one object that knows how to get hold of all the services that they need. Each class asks this object for a service when it needs one, so the class is independent of the concrete implementation of the service.[^1]

## Problem

A class that creates its own collaborators depends on their concrete classes. Fowler's example is a MovieLister that uses a MovieFinder. He wants to give the lister to friends, who can then plug in a suitable finder for their own environment.[^1] For that, the lister must not name the concrete finder class. [[dependency-injection|Dependency injection]] is one way to break this dependency, and Fowler wrote: "Injection isn't the only way to break this dependency, another is to use a service locator."[^1]

## Solution

"The basic idea behind a service locator is to have an object that knows how to get hold of all of the services that an application might need."[^1] The class asks the locator for each service. The configuration code of the program puts the implementations into the locator before it uses the classes.

Fowler notes that "this just shifts the burden a tad, we still have to get the locator into the lister".[^1] He gives these forms:

- **A singleton registry.** The class calls a static method of the locator, and the locator has one field for each service. Fowler: "service locator is a registry not a singleton. A singleton provides a simple way of implementing a registry, but that implementation decision is easily changed."[^1]
- **A segregated interface.** The class depends on a role interface that has only the services it uses, not on the full locator class.[^1] See [[program-to-an-interface|Program to an interface]].
- **A dynamic locator.** The locator keeps a map from names to services, so that you can "stash any service you need into it and make your choices at runtime."[^1]

The difference from dependency injection is the direction of the request. Fowler: "With service locator the application class asks for it explicitly by a message to the locator. With injection there is no explicit request, the service appears in the application class - hence the inversion of control."[^1] See [[inversion-of-control|Inversion of control]].

## Example

Each example file registers a formatting service in a dynamic locator and prints a report that asks the locator for the service. A second report also asks for a tax service that the configuration never registered. The program fails only when that report runs, not when it is built.

| Language | Locator | Missing service |
|---|---|---|
| JavaScript | a class with a static private Map | an Error from get |
| Python | a class with a dict | a KeyError from the dict |
| Go | a package-level map with values of type any | an error from get |

## Consequences

- **Decoupling.** Fowler: "both implementations provide the fundamental decoupling that's missing in the naive example - in both cases application code is independent of the concrete implementation of the service interface."[^1]
- **A dependency on the locator.** "The key difference is that with a Service Locator every user of a service has a dependency to the locator."[^1]
- **Hidden dependencies.** "With the service locator you have to search the source code for calls to the locator."[^1] Seemann: "the problem with Service Locator is that it hides a class' dependencies, causing run-time errors instead of compile-time errors, as well as making the code more difficult to maintain because it becomes unclear when you would be introducing a breaking change."[^2] In his example, a class has a default constructor, and its method throws a KeyNotFoundException because a validator "was never registered with Locator."[^2]
- **Testing.** Fowler did not agree that locators make tests hard: "there is really no difference here between dependency injection and service locator: both are very amenable to stubbing."[^1] Seemann notes that with a static locator, "we need to invoke the Reset method after each unit test".[^2]

## When not to use it

Fowler limits the problem to one case: "So the primary issue is for people who are writing code that expects to be used in applications outside of the control of the writer. In these cases even a minimal assumption about a Service Locator is a problem."[^1] For the classes of one application, he wrote that "a dependency from the application classes to the locator isn't a big deal."[^1]

Seemann goes further. He wrote that the problem with a Service Locator is "that it's a bona-fide anti-pattern", and that "The compiler can offer both consumers and producers so much help when Constructor Injection is used, but none of that assistance is available for APIs that rely on Service Locator."[^2] He also showed that an abstract locator that the class receives through its constructor "still doesn't tell us which services are needed."[^2] In later updates to the post, he wrote that the service locator violates SOLID, and that "The fundamental problem with Service Locator is that it violates encapsulation."[^2]

## Variants and related patterns

- **Dependency injection.** The alternative that Fowler compared with the locator. See [[dependency-injection|Dependency injection]].
- **Both together.** "Dependency injection and a service locator aren't necessarily mutually exclusive concepts." In the Avalon framework, a component gets the locator by injection and then asks it for services.[^1]
- **Singleton.** The simplest locator is a singleton registry, but Fowler treats the singleton as one implementation choice. See [[singleton-pattern|Singleton pattern]].

## Known uses

- **Avalon.** Fowler: "Avalon uses a service locator, but uses injection to tell components where to find the locator."[^1]
- **Go database/sql.** A driver package calls sql.Register, which "makes a database driver available by the provided name." The program then calls sql.Open, which "opens a database specified by its database driver name and a driver-specific data source name".[^3] The package keeps the drivers in a registry by name, which is the shape of the dynamic locator.

## References
