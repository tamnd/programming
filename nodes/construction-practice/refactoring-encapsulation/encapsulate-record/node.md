---
tag: GP21G7
kind: technique
slug: encapsulate-record
name: Encapsulate record
aliases: [Replace Record with Data Class]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Replace a plain record, such as a dictionary or a JSON object, with an object whose accessors are the only way to reach the data, so that the storage can change and the callers do not.
family: refactoring
polarity: positive
intent: "Hide how a record stores its data behind an object with accessors."
also_known_as: [Replace Record with Data Class]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) had Replace Record with Data Class. The second edition (2018) names it Encapsulate Record and puts it first in its chapter on encapsulation.
see_also: [encapsulate-variable, encapsulate-collection, replace-primitive-with-object, data-class, primitive-obsession, encapsulation-and-visibility]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Encapsulate Record, motivation and mechanics
  - url: https://refactoring.com/catalog/encapsulateRecord.html
    title: Martin Fowler, Catalog of Refactorings, Encapsulate Record
    locator: the entry; alias Replace Record with Data Class
  - url: https://openjdk.org/jeps/395
    title: JEP 395, Records
    locator: Summary
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Kotlin documentation, Data classes
    locator: the introduction
  - url: https://docs.python.org/3/library/dataclasses.html
    title: Python documentation, dataclasses, Data Classes
    locator: the introduction; frozen instances
  - url: https://www.jetbrains.com/help/idea/encapsulate-fields.html
    title: IntelliJ IDEA documentation, Encapsulate Fields
    locator: the introduction
---

## Purpose

A record is a set of named values with no behaviour: a JavaScript object literal, a Python dictionary, a row from a database, a document parsed from JSON. Records are convenient, and every language has them. Their weakness is that they expose how the data is stored. A caller reads `product.priceCents`, so the field must stay `priceCents`, in cents, as a number, for as long as any caller exists. A caller also cannot tell a stored value from a derived one, and has no place to put the rules about the data.[^1]

Encapsulate record replaces the record with an object. Callers reach the data only through the accessors of the object, and the object can store the data in another form, compute some values from others, or check what is set. The catalogue card shows the record `organization = {name: "Acme Gooseberries", country: "GB"}` becoming a class `Organization` with a getter and a setter for each field.[^2]

The book prefers an object over a record for data that is mutable. For data that does not change, a record with the values in fields is often enough, because no caller can change it behind the back of another.[^1] Languages with record types, such as Java records, Kotlin data classes and Python dataclasses, sit between the two: the fields are named and typed, and the type can carry methods.

## Steps

1. Run the tests and see them pass.
2. Apply [[encapsulate-variable|Encapsulate Variable]] to the variable that holds the record.[^1]
3. Write a class that wraps the record. Give it an accessor that returns the raw record, and make the encapsulating functions use the class.
4. Test.
5. Add functions that return the object, not the raw record.
6. For each user of the record, change it to use the object and its accessors, adding accessors as they are needed. Test after each.
7. Remove the accessor of the raw record and the functions that return it. Test.
8. If a field holds a record or a collection itself, consider Encapsulate Record or [[encapsulate-collection|Encapsulate Collection]] on it.[^1]

## Example

A product arrives as a JSON record. The code reads its fields directly and computes the price text in each place. After the refactoring, a class holds the data, checks it, and offers the derived value.

```javascript run id=hide-the-storage
import assert from "node:assert/strict";

const json = '{"sku":"T-100","priceCents":1999,"currency":"EUR"}';

// Before: callers read the storage and repeat the derived rule.
const raw = JSON.parse(json);
const textBefore = `${(raw.priceCents / 100).toFixed(2)} ${raw.currency}`;

// After: the storage is private; the accessors are the interface.
class Product {
  #sku; #priceCents; #currency;
  constructor(data) {
    this.#sku = data.sku;
    this.#currency = data.currency;
    this.price = data.priceCents / 100; // goes through the setter
  }
  get sku() { return this.#sku; }
  get price() { return this.#priceCents / 100; }
  set price(amount) {
    if (!(amount >= 0)) throw new RangeError(`bad price ${amount}`);
    this.#priceCents = Math.round(amount * 100);
  }
  get priceText() { return `${this.price.toFixed(2)} ${this.#currency}`; } // derived
  toJSON() { return { sku: this.#sku, priceCents: this.#priceCents, currency: this.#currency }; }
}

const p = new Product(JSON.parse(json));
assert.equal(p.priceText, textBefore);
assert.equal(p.priceText, "19.99 EUR");

// The class keeps its rules, and the stored form can change without the callers.
assert.throws(() => { p.price = -5; }, RangeError);
p.price = 24.5;
assert.equal(JSON.stringify(p), '{"sku":"T-100","priceCents":2450,"currency":"EUR"}');
assert.equal(p.priceCents, undefined); // the storage is no longer reachable
console.log("encapsulate record: tests pass");
```

```output
encapsulate record: tests pass
```

Callers now ask for `price` and `priceText`. They do not know that the price is stored in cents, and the class can change that without changing them. `toJSON` gives back the record form at the edge of the program, where the data leaves it.

| Language | What the example shows |
|---|---|
| JavaScript | a JSON record wrapped in a class with private fields, as above |
| Python | a dictionary replaced by a frozen dataclass with a derived property |
| Java | a `Map` replaced by a `record` with a check in its compact constructor |
| Go | a `map[string]any` replaced by a struct with unexported fields and methods |

## When to use

- **The record is mutable and widely used.** Every caller that changes a field is a place to check. An object gives one place.[^1]
- **Derived values are computed by callers.** The derived rule belongs with the data.
- **The storage may change.** A unit, a name or a type of a field, or a move to another store.
- **Nested data from outside.** Data parsed from JSON or read from a database often arrives as nested records. Wrapping the top level, and then the parts that the program uses, controls how the rest of the program sees the data.[^1]

## Pitfalls

**Getters and setters for everything.** A class with a public getter and setter for each field and no other method is a [[data-class|Data Class]]. It hides the storage, which is a gain, but the rules still live in the callers. Move the rules in as the next step.

**Returning a mutable part.** A getter that returns a nested object or a list lets a caller change it. Apply the same refactoring to the part, or return a copy.

**Records in the language.** A record type in the language is not the same as the refactoring. JEP 395 describes Java records as "classes that act as transparent carriers for immutable data".[^3] Kotlin generates members for each data class that "allow you to print an instance to readable output, compare instances, copy instances, and more".[^4] A Python dataclass can be made read-only: "by passing frozen=True to the @dataclass decorator you can emulate immutability".[^5] These types are a good target for immutable data. Their accessors are fixed to the fields, so if the storage must change, a class is still the tool.

**The edge of the program.** Serialization needs the raw form. Keep one place, such as `toJSON` or a `to_dict` function, that converts between the object and the record.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The steps use [[encapsulate-variable|Encapsulate Variable]], and for the fields of a class that already exists, the Encapsulate Fields action of IntelliJ IDEA, which "lets you hide your data and create the necessary accessors".[^6] The tests check each step.

## References
