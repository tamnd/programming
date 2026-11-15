---
tag: 7RR5MZ
kind: pattern
slug: registry
name: Registry
aliases: [Registry pattern, well-known object, global registry]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A registry is a well-known object that other objects use to find common objects and services, such as finders, when they have no reference to start from.
family: architectural
polarity: positive
intent: "Give the program one well-known place to find common objects and services."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [service-locator, singleton-pattern, dependency-injection, global-data, identity-map, test-double, separated-interface, gateway]
sources:
  - url: https://martinfowler.com/eaaCatalog/registry.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Registry
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Registry
  - url: https://docs.python.org/3/library/logging.html
    title: Python documentation, logging, Logging facility for Python
    locator: Logger Objects
---

## Intent

A registry is "A well-known object that other objects can use to find common objects and services."[^1] The program puts the objects in the registry once. Code anywhere in the program can then get them from the registry.

## Problem

Usually, code finds an object through an association. To get the orders of a customer, the code starts with the customer object and asks it for its orders. But sometimes the code has no object to start from. It has only a key, such as the ID of a customer. The catalog says: "In this case you need some kind of lookup method - a finder - but the question remains: How do you get to the finder?"[^1]

If each class must get the finder through its constructor, then each class between the start of the program and the code that needs the finder must also pass it on. This adds parameters to many classes that do not use them.

## Solution

Put the common objects in one object that all the code can find. The parts are:

- **The registry.** It is one object, which the code gets through static methods or functions. The methods hide how the registry keeps its data.
- **One explicit method for each service.** A method such as `person_finder()` has a clear name and a type. A lookup by a string key is more flexible, but the compiler cannot check it.
- **The install step.** The start of the program puts the real objects in the registry. A test can install a registry with stubs.
- **The scope.** The registry can be the same for all the process, or there can be one registry for each thread or each session.[^2]

```text
  main:   Registry.install(Registry(person_finder=PersonFinder(...)))
  code:   Registry.person_finder().find(1)  -->  the registry  -->  PersonFinder
  test:   Registry.install(Registry(person_finder=StubFinder()))
```

The catalog says that "A Registry is essentially a global object, or at least it looks like one - even if it isn't as global as it may appear."[^1] In the following program, the logging module of Python is the registry. It gives the same logger to each caller that asks for the same name.

```python run id=logger-registry
import logging

# The logging module keeps a registry of loggers by name.
a = logging.getLogger("shop.orders")  # in one module
b = logging.getLogger("shop.orders")  # in another module, with no reference to a
print("the same logger object:", a is b)
```

```output
the same logger object: True
```

## Example

Each example has a `Registry` with one service, a person finder. The function `greet` is deep in the program and has no reference to a finder, so it gets the finder from the registry. The program installs a registry with a finder that reads a table, and greets Ann and Bob. Then it installs a registry with a stub finder, as a test does. Last, it installs an empty registry, and `greet` gives a clear error.

## Consequences

**Benefits.**

- **Fewer parameters.** Code that needs a finder or a service gets it from the registry, and the classes between do not pass it on.
- **One place to configure.** The start of the program installs all the common objects in one place.
- **Replaceable objects.** Because the code calls the methods of the registry and not a concrete class, a test can install a [[test-double]].

**Liabilities.**

**A registry is global state, and a test that changes it can break other tests.** In the following program, a test installs a stub in the registry and does not restore the real finder. The code that runs after the test uses the stub.

```python wrong id=test-leaves-a-stub-in-the-registry
class Registry:
    person_finder = None  # global state


Registry.person_finder = lambda person_id: {1: "ann"}[person_id]


def test_greeting():
    Registry.person_finder = lambda person_id: f"test person {person_id}"
    assert Registry.person_finder(1) == "test person 1"  # the test does not restore the registry


test_greeting()
print("after the test, the program greets: hello,", Registry.person_finder(1))
```

```output
after the test, the program greets: hello, test person 1
```

Let each test install a registry and restore the previous one when it ends, for example in a fixture or a `finally` block. Or give the object to the code as a parameter.

**Hidden dependencies.** The signature of a class does not show that the class uses the registry. To know what a class needs, you must read its code.

**Shared state between threads.** If threads read the registry while one thread changes it, the registry needs a lock, or each thread needs its own registry.

## When not to use it

**When the code can get the object as a parameter.** If the caller has the object, or a reference to an object that has it, pass it. A parameter shows the dependency and needs no global state.

**When a framework already does dependency injection.** The framework gives each class what its constructor asks for. A registry next to it adds a second way to get the same object.

## Variants and related patterns

**Process scope and thread scope.** A registry for the process holds objects that all threads share, such as a finder. A registry for each thread holds objects that belong to one request or one transaction, such as the [[identity-map]] of a [[unit-of-work]].

**Singleton.** A registry is often a [[singleton-pattern]]. But the static methods of the registry hide the instance, so the program or a test can replace it. A plain singleton class often cannot be replaced.

**Service locator.** A [[service-locator]] is a registry of services. [[dependency-injection]] is the alternative: the code gets the objects that it needs, and does not ask for them.

**Separated interface.** The registry can give the code a [[separated-interface]], so the code does not depend on the implementation that the program installs.

## Known uses

- In the Python logging module, "Multiple calls to getLogger() with the same name will always return a reference to the same Logger object."[^3] The module keeps the loggers by name, and the documentation says that loggers "should NEVER be instantiated directly".[^3]

## References
