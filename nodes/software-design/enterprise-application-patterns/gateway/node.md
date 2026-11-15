---
tag: 1V1311
kind: pattern
slug: gateway
name: Gateway
aliases: [Gateway pattern, wrapper of an external system, client wrapper]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A gateway is an object that wraps the access to an external system or resource, so that the rest of the program calls it in its own terms and not in the terms of the foreign API.
family: architectural
polarity: positive
intent: "Encapsulate the access to an external system or resource in one object with an interface in the terms of the program."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002) and wrote a longer explanation of it in 2021.
see_also: [table-data-gateway, row-data-gateway, separated-interface, adapter-pattern, facade-pattern, mediator-pattern, test-double, remote-facade]
sources:
  - url: https://martinfowler.com/eaaCatalog/gateway.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Gateway
    locator: pattern summary
  - url: https://martinfowler.com/articles/gateway-pattern.html
    title: Martin Fowler, Gateway, 2021
    locator: How it Works; When to Use It; Further Reading
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 10, Data Source Architectural Patterns; Chapter 18, Base Patterns, Gateway
  - url: https://martinfowler.com/eaaCatalog/separatedInterface.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Separated Interface
    locator: pattern summary
---

## Intent

A gateway is "An object that encapsulates access to an external system or resource."[^1] The rest of the program calls the gateway in its own terms. Only the gateway knows the foreign API.

## Problem

Most programs use external systems: a database, a remote service, a file format, a library with an awkward API. That API is made for the external system, not for the program. Fowler writes that it "may use different types, require strange arguments, combine fields in ways that don't make sense in our context."[^2]

If each part of the program calls the foreign API directly, the knowledge of that API spreads through the code. Each caller must convert the units, read the return codes and handle the errors, and each caller can do it differently. A change of the external system then needs a change in each caller, and a test cannot run without the external system.

## Solution

Put all the code that knows the foreign API into one object, the gateway. Give it an interface in the terms of the program. The parts are:

- **The gateway interface.** Methods with the names, types and errors of the program, made for what the program needs, and not for all that the external system can do.
- **The translation.** The gateway converts each call into the call of the foreign API and converts the result back. Fowler says that "Gateways should only include logic that supports this translation between domestic and foreign concepts."[^2]
- **The connection.** An optional small object that only makes the call to the foreign code. Fowler notes that it "acts as a good point for inserting a Test Double".[^2]

```text
  program code  --standard_cost(1.2 kg, "DE")-->  ShippingGateway
                <--(620 cents, "EUR") or NoService--    |  converts kg to grams, reads rc and amt
                                                        v
                                         connection: quote(1200, "DE", "STD")  -->  foreign API
```

In the following program, the foreign API gives an exchange rate as a string of the rate times 10000, with a status field. The gateway gives the program a number.

```python run id=wrap-an-awkward-rate-api
def foreign_rate(src_ccy, dst_ccy, mode):  # an awkward foreign API
    return {"status": "OK", "rate_x10000": "10850"} if mode == "MID" else {"status": "ERR"}


class RateGateway:
    def eur_to_usd(self):
        reply = foreign_rate("EUR", "USD", "MID")
        if reply["status"] != "OK":
            raise LookupError("no rate")
        return int(reply["rate_x10000"]) / 10000


print("1 EUR in USD:", RateGateway().eur_to_usd())
```

```output
1 EUR in USD: 1.085
```

## Example

Each example file has a foreign quote function of a shipping service. It takes the weight in grams and a string of flags, and it returns a map with a return code and the amount as a string of cents. `ShippingGateway` gives the program one method, `standard_cost`, that takes kilograms and a country. It returns the cost in cents and the currency, or it raises a `NoService` error with the reason. The program asks for a parcel of 1.2 kg, which costs 6.20 EUR, and for a parcel of 31.0 kg, which the service does not take. The last call gives a gateway a connection with canned data, as a test does, and the foreign API is not called.

## Consequences

**Benefits.**

- **One place knows the foreign API.** A change of the external system, or a move to a different system, changes only the gateway.
- **Simpler callers.** The callers use the names and types of the program and do not convert units or read return codes.
- **Easy tests.** A test replaces the connection or the whole gateway with a [[test-double]], with no slow or unavailable external system.

**Liabilities.**

**Without a gateway, the callers convert the data in different ways.** In the following program, two parts of the program call the foreign API directly. The invoice forgets that the amount is in cents.

```python wrong id=two-callers-read-the-api-differently
def foreign_quote(weight_grams, dest, svc_flags):  # amt is a string of cents
    return {"rc": 0, "amt": "1250", "cur": "EUR"}


# Two parts of the program call the foreign API directly.
checkout = foreign_quote(2500, "DE", "STD")
print("shipping on the checkout page:", int(checkout["amt"]) / 100, checkout["cur"])

invoice = foreign_quote(2500, "DE", "STD")
print("shipping on the invoice:", invoice["amt"], invoice["cur"])  # forgets the cents
```

```output
shipping on the checkout page: 12.5 EUR
shipping on the invoice: 1250 EUR
```

Put the call and the conversion into one gateway method, and let both parts of the program call it.

**One more layer.** A gateway adds a class for each external system. For an API that already fits the program, the gateway can be just the connection.[^2]

**A gateway that grows logic.** Business rules that use the external data belong in the clients of the gateway, not in the gateway.[^2]

## When not to use it

**When the foreign vocabulary is the vocabulary of the program.** Fowler gives the collection classes of a language as an example of what not to wrap, because "their vocabulary is part of the vocabulary of my software."[^2]

**When the program uses the external system in only one place.** The code in that place is already the only code that knows the API. A gateway helps when a second caller or a test needs it.

## Variants and related patterns

**Database gateways.** [[table-data-gateway]] has one object for all the rows of a table, and [[row-data-gateway]] has one object for each row.[^3]

**Gateway behind a separated interface.** The interface of the gateway can be in the package of its clients, and the implementation in another package. The catalog says that "The Separated Interface provides a good plug point for Gateway".[^4] See [[separated-interface]]. During tests, a [[service-stub]] can replace the gateway.

**Adapter, facade and mediator.** Fowler compares the gateway with these three patterns.[^2] An [[adapter-pattern]] fits a class to an interface that already exists, while the interface of a gateway is made when the gateway is written. A [[facade-pattern]] is usually written by the writer of the service for all its clients, while a gateway "is written by the client for its particular use."[^2] A [[mediator-pattern]] connects many objects, while a gateway wraps one resource.

**Anticorruption layer.** In domain-driven design, a gateway is a way to build an [[anticorruption-layer]] between two bounded contexts.[^2]

## Known uses

- The data source patterns of the book, [[table-data-gateway]] and [[row-data-gateway]], are gateways to database tables.[^3]
- Fowler notes that the pattern is common but that "there is no widely-used name for it", and that gateways are often called adapters.[^2]

## References
