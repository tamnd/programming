---
tag: PHAADS
kind: pattern
slug: service-stub
name: Service stub
aliases: [Service Stub, stub service, service virtualization]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A service stub replaces a slow, remote or unfinished service with a local implementation of the same interface that gives canned answers, so tests and development do not depend on the service.
family: architectural
polarity: positive
intent: "Remove the dependence on problematic services during testing."
history:
  year: 2002
  people: [David Rice]
  note: David Rice wrote the pattern for Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [test-double, gateway, separated-interface, plugin, dependency-injection, null-object-pattern, registry, service-locator]
sources:
  - url: https://martinfowler.com/eaaCatalog/serviceStub.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Service Stub
    locator: pattern summary, by David Rice
  - url: https://martinfowler.com/bliki/ContractTest.html
    title: Martin Fowler, ContractTest, bliki, 12 January 2011
    locator: whole article
  - url: https://martinfowler.com/bliki/TestDouble.html
    title: Martin Fowler, TestDouble, bliki
    locator: the kinds of double
  - url: https://wiremock.org/docs/overview/
    title: WireMock documentation, Overview
    locator: Key features
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Service Stub
---

## Intent

A service stub "Removes dependence upon problematic services during testing."[^1] The code uses the service through an interface. In tests, a stub implements the interface with canned answers, and the real service is not called.

## Problem

Enterprise systems often use services of other organizations. The catalog names "credit scoring, tax rate lookups, and pricing engines".[^1] These services are often remote, so they can be slow or not available, and some of them are not finished when the program is written.

When the tests call the real service, they are slow, and they fail when the service fails. The catalog says that such dependencies "will lead to times when tests can't execute. When tests can't run the development process is broken."[^1]

## Solution

"Replacing the service during testing with a Service Stub that runs locally, fast, and in memory improves your development experience."[^1] The parts are:

- **The interface.** The code calls the service through a [[separated-interface]], usually behind a [[gateway]].
- **The real implementation.** It calls the remote service.
- **The stub.** It implements the same interface and gives simple, canned answers, such as a flat tax rate or a small table of rates. It has as little logic as possible.
- **The choice.** The test, a [[plugin]] configuration or [[dependency-injection]] gives the code the stub or the real implementation.

```text
  BillingService --uses--> TaxService
                              ^            ^
                    RemoteTaxService   TaxServiceStub
                    (network, slow)    (in memory, canned rates)
```

In the following program, a test checks the total of a bill with a stub that gives a flat rate.

```python run id=flat-rate-tax-stub
class FlatRateTaxStub:  # runs locally, fast and in memory
    def __init__(self, rate_bp):
        self.rate_bp = rate_bp

    def rate_bp_for(self, region):
        return self.rate_bp


def total_cents(amount_cents, region, taxes):
    return amount_cents + amount_cents * taxes.rate_bp_for(region) // 10000


assert total_cents(10000, "north", FlatRateTaxStub(650)) == 10650
assert total_cents(10000, "north", FlatRateTaxStub(0)) == 10000
print("the tests pass with no network")
```

```output
the tests pass with no network
```

## Example

Each example has a `TaxService` interface that gives a tax rate in basis points, so 650 is 6.5%. `RemoteTaxService` stands for the real service, which is not reachable. `TaxServiceStub` has canned rates for two regions and a default rate for the others, and counts its calls. `BillingService` adds the tax to an amount. The program first tries the real service and gets an error. Then it bills 100.00 in three regions with the stub, and prints the number of calls that the stub got.

## Consequences

**Benefits.**

- **Fast and reliable tests.** The tests do not wait for the network and do not fail when the service fails.
- **Work before the service is ready.** The team can write and test the code while the other team finishes the service.
- **Cases that are hard to make.** A stub can give an answer that the real service seldom gives, such as an error or a rate of zero.

**Liabilities.**

**A stub can stop matching the real service.** In the following program, the real service changed the unit of its rate, but the stub still gives the old unit. The tests with the stub pass, and the bill in production is wrong.

```python wrong id=stub-and-service-disagree
def total_cents(amount_cents, rate_bp):  # the code expects basis points: 650 is 6.5%
    return amount_cents + amount_cents * rate_bp // 10000


class TaxServiceStub:
    def rate(self, region):
        return 650  # basis points, as the service once returned


class RemoteTaxService:  # the real service changed: it now returns tenths of a percent
    def rate(self, region):
        return 65


print("with the stub:", total_cents(10000, TaxServiceStub().rate("north")))
print("in production:", total_cents(10000, RemoteTaxService().rate("north")))
```

```output
with the stub: 10650
in production: 10065
```

Run contract tests against the real service from time to time. Fowler writes that contract tests "check that all the calls against your test doubles return the same results as a call to the external service would".[^2] When a contract test fails, update the stub and the code.

**The real calls are not tested.** Tests with the stub do not check the network code, the encoding or the errors of the real service. These need their own integration tests.

**A stub can grow.** A stub with much logic becomes a second implementation of the service, with its own bugs. Keep it simple.

## When not to use it

**When the service is fast, local and reliable.** If the tests can call it with no problem, they test more with the real service.

**When the test must check the conversation with the service.** If the test must check which calls the code makes and in which order, use a mock object, which checks the calls. A stub only answers them.

## Variants and related patterns

**Test doubles.** A service stub is a kind of [[test-double]]. Fowler's description of the kinds says: "Stubs provide canned answers to calls made during the test, usually not responding at all to anything outside what's programmed in for the test."[^3] A fake has a working implementation with a shortcut, such as a database in memory.[^3]

**HTTP stubs.** A stub can also be a local server that answers HTTP requests. WireMock gives "HTTP response stubbing, matchable on URL, header and body content patterns".[^4] The code under test then needs no change, only another address of the service.

**Related patterns.** A [[gateway]] behind a [[separated-interface]] gives the place to put the stub. A [[plugin]] chooses the stub in the test configuration. The book shows a tax service with a flat rate stub.[^5] A [[null-object-pattern]] is like a stub that does nothing, but it is part of the program and not only of the tests.

## Known uses

- WireMock runs HTTP stubs "in unit tests, as a standalone process or as a WAR app" and can record and play back stubs.[^4]

## References
