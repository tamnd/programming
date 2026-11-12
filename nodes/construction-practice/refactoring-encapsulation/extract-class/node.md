---
tag: JGBBGJ
kind: technique
slug: extract-class
name: Extract class
aliases: [split class]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Split a class that does two jobs into two classes, by moving the fields and methods of one job into a class of its own and keeping a reference to it.
family: refactoring
polarity: positive
intent: "Give a group of fields and methods that belong together a class of their own."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Extract Class is in the first edition of Refactoring (1999), with the refactorings that move features between objects, and in the chapter on encapsulation of the second edition (2018). Its inverse is Inline Class.
see_also: [inline-class, large-class, divergent-change, data-clumps, encapsulate-record, code-smell]
sources:
  - url: https://refactoring.com/catalog/extractClass.html
    title: Martin Fowler, Catalog of Refactorings, Extract Class
    locator: the entry; inverse of Inline Class
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Extract Class, motivation and mechanics; Chapter 3, Bad Smells in Code, Large Class
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 7, Moving Features Between Objects, Extract Class
  - url: https://www.jetbrains.com/help/idea/extract-into-class-refactorings.html
    title: IntelliJ IDEA documentation, Extract into class refactorings
    locator: Extract delegate
---

## Purpose

A class starts with a clear job. Then a field is added, and a method that uses it, and another, and after a while one class holds two jobs. An `Order` that also formats, checks and compares shipping addresses is an order and an address at once. Each job changes for its own reasons, and each change must be made in the middle of the other job's code.

Extract class moves one job into a class of its own. The fields and methods of that job move to it, and the old class keeps a reference to it. The catalogue card shows a `Person` whose office area code and office number move into a class `TelephoneNumber`, while `Person` keeps getters that ask the telephone number.[^1]

The book gives signs that a class holds a second job: a subset of the data and the methods that go together, data that changes together, and data that depends on other data in the same subset. A test is to ask what would happen if you removed one field or method: if other fields and methods would no longer make sense, they form a group.[^2] The inverse is [[inline-class|Inline Class]].[^1] The first edition has the refactoring under the same name.[^3]

## Steps

1. Run the tests and see them pass.
2. Decide which fields and methods form the second job.
3. Create the class for it, with a name for that job. If the old class's name no longer fits what is left, rename it too.
4. In the constructor of the old class, make an instance of the extracted class and keep a reference to it.
5. Move each field into the extracted class, and test after each.[^2]
6. Move each method into the extracted class, starting with the methods that are called by others rather than those that call. Test after each.[^2]
7. Review the interfaces of both classes. Remove methods that no one needs, and rename the ones whose names no longer fit.
8. Decide whether to expose the extracted class to callers. If you do, decide whether it is a value or a reference.[^2]

## Example

An order holds the fields of a shipping address and the methods that format and check it. After the refactoring, an `Address` class holds the address job, and the order delegates to it.

```javascript run id=two-jobs
import assert from "node:assert/strict";

// Before: an order that is also an address.
class OrderBefore {
  constructor(id, street, city, postcode) {
    this.id = id;
    this.street = street;
    this.city = city;
    this.postcode = postcode;
  }
  addressLabel() { return `${this.street}\n${this.postcode} ${this.city.toUpperCase()}`; }
  sameAddress(other) {
    return this.street === other.street && this.city === other.city && this.postcode === other.postcode;
  }
}

// After: the address job in its own class.
class Address {
  constructor(street, city, postcode) {
    if (!/^\d{5}$/.test(postcode)) throw new RangeError(`bad postcode ${postcode}`);
    Object.assign(this, { street, city, postcode });
    Object.freeze(this);
  }
  get label() { return `${this.street}\n${this.postcode} ${this.city.toUpperCase()}`; }
  equals(o) { return this.street === o.street && this.city === o.city && this.postcode === o.postcode; }
}

class Order {
  #address;
  constructor(id, address) { this.id = id; this.#address = address; }
  get address() { return this.#address; }
  addressLabel() { return this.#address.label; } // kept for the old callers
}

const a = new OrderBefore(1, "Main St 5", "Berlin", "10115");
const b = new Order(1, new Address("Main St 5", "Berlin", "10115"));
assert.equal(b.addressLabel(), a.addressLabel());
assert.equal(b.address.equals(new Address("Main St 5", "Berlin", "10115")), true);

// The address rules now have their own place, and other classes can use them.
assert.throws(() => new Address("Main St 5", "Berlin", "1011"), RangeError);
console.log("extract class: tests pass");
```

```output
extract class: tests pass
```

The order no longer knows how an address is formatted or compared. A customer, a warehouse and an invoice can use the same `Address` class, and the postcode rule is written once.

| Language | What the example shows |
|---|---|
| JavaScript | an `Address` class extracted from an order, as above |
| Python | the same split, with a frozen dataclass for the address |
| Go | the same split, with an `Address` struct embedded in the order |

## When to use

- **A class is too large.** [[large-class|Large Class]] is the smell, and the book names this refactoring as a fix for it.[^2]
- **A class changes for two reasons.** If changes of one kind touch one part of the class and changes of another kind touch another part, that is [[divergent-change|Divergent Change]], and the parts can be two classes.
- **Fields go together.** A group of fields that are always passed and used together, as in [[data-clumps|Data Clumps]], is a class that is not written yet.
- **Fields share a prefix.** `shippingStreet`, `shippingCity`, `shippingPostcode` name the class that holds them.

## Pitfalls

**A split in the wrong place.** If the methods of the extracted class keep reaching back into the old one, the line is wrong. The two classes should talk through a narrow interface.

**Callers that reach through.** After the split, callers may start writing `order.address.city`. That ties them to the shape of both classes. If many callers need the same thing, [[hide-delegate|Hide Delegate]] gives the old class a method for it.

**Shared and mutable parts.** If two orders share one address object and one of them changes it, both change. Make the extracted class a value, immutable, or give each owner its own copy.

**Too many small classes.** A class for one field that no rule uses adds a name and a step for every reader. If the extracted class does not pull its weight, inline it back.

## Tools that do it

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java | Extract Delegate, which lets you "extract some of the fields and methods of a class into a separate, newly created class"[^4] |

The tool moves the fields and methods that you select and writes the delegation. The choice of which fields and methods form a job is the programmer's.

## References
