---
tag: 34ESVJ
kind: pattern
slug: mapper
name: Mapper
aliases: [Mapper pattern, object mapper, translation layer]
area: software-design/enterprise-application-patterns
level: advanced
summary: >-
  A mapper is an object that connects two independent subsystems, so that they work together while neither of them knows the other or the mapper.
family: architectural
polarity: positive
intent: "Set up the communication between two independent objects or subsystems that must stay unaware of each other."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns.
see_also: [data-mapper, gateway, mediator-pattern, data-transfer-object, adapter-pattern, separated-interface, domain-model]
sources:
  - url: https://martinfowler.com/eaaCatalog/mapper.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Mapper
    locator: pattern summary
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 10, Data Mapper; Chapter 18, Base Patterns, Mapper
  - url: https://mapstruct.org/documentation/stable/reference/html/
    title: MapStruct reference guide
    locator: Preface; Introduction
---

## Intent

A mapper is "An object that sets up a communication between two independent objects."[^1] The mapper knows both sides. The two sides do not know each other, and they do not know the mapper.

## Problem

Fowler describes the problem: "Sometimes you need to set up communications between two subsystems that still need to stay ignorant of each other."[^1] One subsystem can be code that the team cannot change, such as a library or a legacy system. The two subsystems can have different names and types for the same things. A product can be `pen` in one and `PEN-01` in the other.

If one subsystem calls the other directly, it must know the names and types of the other, and it can no longer change or be used alone. The catalog adds that you may not want "to create dependencies between the two or even between them and the isolating element."[^1]

## Solution

Put a third object between the subsystems. The parts are:

- **The two subsystems.** Each one has its own types and its own interface, and neither one imports the other.
- **The mapper.** It reads the data of one side, converts the names, the types and the structure, and calls the other side. It converts the reply back in the same way.
- **The trigger.** Something outside both subsystems calls the mapper, for example the application layer or a scheduled job. This is the difference from a [[gateway]], which its client calls and so knows.

```text
  Order (pen x2, ink x1)         OrderWarehouseMapper              Warehouse
  status "new"          <-reads- converts pen -> PEN-01   -calls-> pick(ref ORD-7,
                                 converts PICKED -> picked <-reply-     PEN-01 x2, INK-02 x1)
  status "picked"       <-sets--
```

In the following program, a thermometer and a display do not know each other. The mapper reads one and writes to the other.

```python run id=connect-two-unaware-objects
class Thermometer:  # one subsystem: knows only degrees Celsius
    def read(self):
        return 21.5


class Display:  # another subsystem: knows only text
    def show(self, text):
        print("display:", text)


class ThermometerDisplayMapper:  # knows both; neither knows the mapper
    def __init__(self, thermometer, display):
        self.thermometer, self.display = thermometer, display

    def update(self):
        self.display.show(f"{self.thermometer.read():.1f} C")


ThermometerDisplayMapper(Thermometer(), Display()).update()
```

```output
display: 21.5 C
```

## Example

Each example file has an order system with an `Order` of item names and quantities, and a warehouse that works only with product codes and reference strings. The mapper `OrderWarehouseMapper` builds a pick request with the reference `ORD-7` and the codes `PEN-01` and `INK-02`, sends it to the warehouse, and converts the state `PICKED` of the reply into the status `picked` of the order. The order system and the warehouse have no reference to each other or to the mapper.

## Consequences

**Benefits.**

- **The subsystems stay independent.** Each one can change, be tested and be used alone. A change of one changes only the mapper.
- **One place for the translation.** All the knowledge of how the two sides fit together is in the mapper.
- **Code that cannot change can still work together.** The mapper connects a library or a legacy system with no change to it.

**Liabilities.**

**Without a mapper, the names of one subsystem reach the other.** In the following program, the order system passes its item names to the warehouse, which knows only codes, and the warehouse can pick nothing.

```python wrong id=item-names-sent-to-the-warehouse
class Warehouse:  # knows only product codes
    def pick(self, lines):
        codes = {"PEN-01", "INK-02"}
        return sum(1 for code, qty in lines if code in codes)


# No mapper: the order system passes its own item names to the warehouse.
lines = [("pen", 2), ("ink", 1)]
print("lines that the warehouse can pick:", Warehouse().pick(lines), "of", len(lines))
```

```output
lines that the warehouse can pick: 0 of 2
```

Put the conversion from item names to codes in a mapper, and let the mapper call the warehouse.

**A third piece to keep in step.** The mapper depends on both subsystems, so a change of either one can break it, and the tests of the mapper need both sides.

**The mapper must be called.** Because neither side knows the mapper, some other code must decide when to call it.

## When not to use it

**When one side can depend on the other.** If the order system may know the warehouse, a [[gateway]] in the order system is simpler.

**When the two sides already use the same types.** Then there is nothing to translate.

## Variants and related patterns

**Data mapper.** The best known mapper is the [[data-mapper]], which moves data between domain objects and the database while each one is unaware of the other.[^2]

**Mapping libraries.** A library can generate the code that copies fields from one type to another, for example from a domain object to a [[data-transfer-object]]. "MapStruct is a Java annotation processor for the generation of type-safe bean mapping classes."[^3]

**Gateway and mediator.** A [[gateway]] is called by its client, which knows it. A [[mediator-pattern]] connects many objects, which know the mediator. A mapper connects two sides that know neither each other nor the mapper.

## Known uses

- The [[data-mapper]] of object-relational mapping tools connects the domain objects and the database tables.[^2]
- MapStruct generates mapping classes between Java bean types with an annotation processor. The generated code copies the values with plain getter and setter calls, not with reflection.[^3]

## References
