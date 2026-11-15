---
tag: 77FJ1E
kind: pattern
slug: value-object
name: Value object
aliases: [Value Object, value type object]
area: software-design/enterprise-application-patterns
level: core
summary: >-
  A value object is a small object, such as an amount of money or a date range, that is equal to another when their values are equal. It is usually immutable.
family: architectural
polarity: positive
intent: "Make a small object whose equality depends on its values and not on its identity."
history:
  year: 2002
  people: [Martin Fowler]
  note: Martin Fowler described the pattern in Patterns of Enterprise Application Architecture (2002), in the chapter on base patterns. Domain-driven design uses the term with the same meaning.
see_also: [value-equality-and-identity, immutable-object, money, embedded-value, change-reference-to-value, data-transfer-object, value, domain-model]
sources:
  - url: https://martinfowler.com/eaaCatalog/valueObject.html
    title: Martin Fowler, Catalog of Patterns of Enterprise Application Architecture, Value Object
    locator: pattern summary
  - url: https://martinfowler.com/bliki/ValueObject.html
    title: Martin Fowler, ValueObject, bliki, 14 November 2016
    locator: whole article
  - url: https://docs.python.org/3/library/dataclasses.html
    title: Python documentation, dataclasses, Data Classes
    locator: the dataclass decorator, parameters eq, frozen and unsafe_hash
  - book: isbn:9780321127426
    title: Martin Fowler, Patterns of Enterprise Application Architecture, Addison-Wesley, 2002
    locator: Chapter 18, Base Patterns, Value Object
  - url: https://openjdk.org/jeps/395
    title: OpenJDK, JEP 395, Records
    locator: Summary and Description
---

## Intent

A value object is "A small simple object, like money or a date range, whose equality isn't based on identity."[^1] Two value objects with the same values are equal, also when they are two different objects in memory. Usually, a value object cannot change after it is created.

## Problem

Programs use many small compound values: a point, an amount of money, a range of dates, an email address. The program must decide when two of these are the same. Fowler writes: "Objects that are equal due to the value of their properties, in this case their x and y coordinates, are called value objects."[^2]

Many languages compare objects by identity by default. Then two date ranges from March 1 to March 10 are not equal, and a map with one of them as a key does not find the other. If the objects can change, there is a second problem: two parts of the program can share one object, and a change in one part also changes the other.

## Solution

Make the small value a class of its own, and give it the behavior of a value. The parts are:

- **Fields that are the value.** The fields together are the value, and the object has no other identity.
- **Equality from the fields.** Two objects are equal when all their fields are equal. The hash, if the language has one, also comes from the fields, so the object can be a key in a map.
- **No change after creation.** The bliki says: "To avoid aliasing bugs I follow a simple but important rule: value objects should be immutable".[^2] An operation that changes the value returns a new object.
- **Checks in the constructor.** The constructor rejects values that are not valid, such as a date range with its end before its start, so each object that exists is valid.
- **Behavior.** The class has the operations of the value, such as `contains` for a date range. Fowler suggests a Range class to prevent "duplicate fiddling with start and end attributes".[^2]

```text
  a = DateRange(Mar 1, Mar 10)   b = DateRange(Mar 1, Mar 10)
  a == b             -> true    (equal fields)
  a is b             -> false   (two objects)
  a.extend_to(Mar 15) -> a new DateRange; a does not change
```

Many languages make such a class with little code. In Python, "If eq and frozen are both true, by default @dataclass will generate a __hash__() method for you."[^3] With `frozen`, "assigning to fields will generate an exception."[^3]

```python run id=frozen-point
from dataclasses import FrozenInstanceError, dataclass, replace


@dataclass(frozen=True)
class Point:
    x: int
    y: int


p = Point(2, 3)
print("equal by value:", p == Point(2, 3))
print("usable as a set member:", p in {Point(2, 3)})
try:
    p.x = 5
except FrozenInstanceError:
    print("p cannot change")
print("a changed copy:", replace(p, x=5))
```

```output
equal by value: True
usable as a set member: True
p cannot change
a changed copy: Point(x=5, y=3)
```

## Example

Each example has a `DateRange` value object with a start day and an end day. The program makes two equal ranges `a` and `b`, and shows that they are equal but are not the same object. It extends `a` to March 15, which gives a new range `c` and does not change `a`. It puts `a` as a key in a map and finds the entry with `b`. Last, it tries to make a range with its end before its start, and the constructor rejects it.

## Consequences

**Benefits.**

- **Correct comparisons.** Two values that are the same compare as equal, in tests, in sets and as map keys.
- **No aliasing bugs.** An immutable value can be shared by many objects, and no change in one place can surprise another.
- **Rules in one place.** The constructor checks the value once. Fowler writes that a telephone number object, in place of a string, gives "a natural focus for validation".[^2]
- **Safe for threads.** An object that cannot change can be read by many threads with no lock.

**Liabilities.**

**A mutable value that two owners share changes for both.** In the following program, the stays of Ann and Bob use the same `DateRange` object. When Bob stays longer, the stay of Ann also changes.

```python wrong id=shared-mutable-date-range
from datetime import date


class DateRange:  # mutable, so two holders can share one object by mistake
    def __init__(self, start, end):
        self.start, self.end = start, end

    def __str__(self):
        return f"{self.start}..{self.end}"


march = DateRange(date(2026, 3, 1), date(2026, 3, 10))
stay_of_ann = march
stay_of_bob = march  # the same object, not a copy
stay_of_bob.end = date(2026, 3, 15)  # Bob stays five more days
print("Ann stays", stay_of_ann)
```

```output
Ann stays 2026-03-01..2026-03-15
```

Make `DateRange` immutable, and let an extension return a new range. Then `stay_of_bob = stay_of_bob.extend_to(...)` changes only the variable of Bob. The bliki calls this kind of error an aliasing bug.[^2]

**More classes.** Each kind of value is a class with its equality, hash and checks. In languages without records or data classes, this is much code to write and to keep correct.

**Copies cost memory and time.** Each change makes a new object. For a large value that changes often, the copies can be slow.

**Storage needs a mapping.** A value object has no key of its own, so a database does not keep it in a table of its own. It is usually saved as an [[embedded-value]] in the row of its owner, or as a [[serialized-lob]].[^4]

## When not to use it

**When the thing has an identity.** A customer, an order or a bank account stays the same thing when its fields change, so it is a reference object or an entity, compared by its key. The bliki adds that an immutable sales order is still not a value object "if I'm determining equality based on a unique order number".[^2]

**When the context needs references.** Fowler gives the example of a postal address, which can be a value in one program and a reference in a mapping system: "different contexts lead to different solutions."[^2]

## Variants and related patterns

**Domain-driven design.** The term has the same meaning in domain-driven design. "In Domain-Driven Design the Evans Classification contrasts value objects with entities."[^2] An [[entity]] has an identity that continues over time. A value object has none. The [[domain-model]] of a program uses both.

**Records and data classes.** Java records "act as transparent carriers for immutable data".[^5] They get "equals and hashCode methods which ensure that two record values are equal if they are of the same type and contain equal component values".[^5] Python data classes, C# records and Kotlin data classes give the same result.

**Copies on assignment.** Fowler notes that a language can also prevent aliasing bugs when each assignment makes a copy: "Some languages provide this ability, such as structs in C#."[^2] Go structs and Rust `Copy` types work in the same way.

**Related patterns.** A [[money]] object is a common value object. The refactoring [[change-reference-to-value]] turns a reference object into a value object. A [[data-transfer-object]] is a different pattern, but the Java community once used the name value object for it, as that page explains. An [[immutable-object]] can be a value object or a reference object.

## Known uses

- Java records are value-like carriers of data with equality from their components.[^5]
- Python data classes with `frozen=True` and the default `eq=True` are hashable values that cannot be assigned to.[^3]

## References
