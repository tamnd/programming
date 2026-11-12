---
tag: EZVNWS
kind: technique
slug: move-field
name: Move field
aliases: []
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Move a field to the record or class that it belongs with, after you encapsulate it, so that data that changes together lives together.
family: refactoring
polarity: positive
intent: "Keep each piece of data in the structure whose changes it follows."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Move Field is in the first edition of Refactoring (1999), in the chapter on moving features between objects, and in the chapter on moving features of the second edition (2018).
see_also: [move-function, encapsulate-variable, shotgun-surgery, data-clumps, feature-envy]
sources:
  - url: https://refactoring.com/catalog/moveField.html
    title: Martin Fowler, Catalog of Refactorings, Move Field
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Move Field, motivation and mechanics
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 7, Moving Features Between Objects, Move Field
---

## Purpose

Data structures shape code. When a field is in the wrong structure, the code around it pays: it passes the field together with another structure, or it must update copies of the field in two places when one thing changes.

Move field puts the field in the structure it belongs with. The catalogue card moves a discount rate from a customer to the customer's plan; the customer's getter then reads it from the plan.[^1] The book gives two signs that a field is in the wrong place: a field that is always passed together with another record, and a change in one record that forces a change to a field in another.[^2] The first edition has the refactoring under the same name.[^3]

## Steps

1. Encapsulate the field, so that all reads and writes go through accessors. See [[encapsulate-variable|Encapsulate Variable]].[^2]
2. Test.
3. Make the field and its accessors in the target.
4. Make sure the source object can reach the target object. If not, add a reference.
5. Change the source accessors to use the target's field.[^2] Test.
6. Remove the field from the source, and test.

Step 1 is what makes the move safe. When every access goes through one getter and one setter, only those two change in step 5.

## Example

Each seat in a theatre stores its own price, but the price is set per section: a price change must find and update every seat. After the refactoring, the section holds the price, and the seat's getter reads it.

```javascript run id=one-place
import assert from "node:assert/strict";

class Section {
  constructor(name, priceCents) { this.name = name; this.priceCents = priceCents; }
}

class Seat {
  #section;
  constructor(row, number, section) { this.row = row; this.number = number; this.#section = section; }
  // Step 5: the accessor reads the moved field.
  get priceCents() { return this.#section.priceCents; }
}

const stalls = new Section("stalls", 4500);
const seats = [new Seat("A", 1, stalls), new Seat("A", 2, stalls), new Seat("B", 7, stalls)];
assert.deepEqual(seats.map((s) => s.priceCents), [4500, 4500, 4500]);

stalls.priceCents = 4900; // one change, in one place
assert.deepEqual(seats.map((s) => s.priceCents), [4900, 4900, 4900]);
console.log("move field: tests pass");
```

```output
move field: tests pass
```

Before the refactoring, the same price change was a loop over every seat in the section, and a seat added later with the old price was a bug.

| Language | What the example shows |
|---|---|
| JavaScript | a price moved from each seat to its section, as above |
| Python | the same move, with a property in the seat that reads the section |
| Java | the same move, where a seat record holds a reference to a mutable section |
| Go | the same move, with a pointer from each seat to the shared section |

## When to use

- **One change, many updates.** A change to one fact forces updates to the same field in many records. This is one form of [[shotgun-surgery|Shotgun Surgery]].
- **Data that travels together.** A field is always passed with another structure. See [[data-clumps|Data Clumps]].
- **Before moving functions.** A function that uses a field moves more cleanly when the field is already in the target. See [[move-function|Move Function]].

## Pitfalls

**Shared data becomes shared.** After the move, many objects see one field. That is the purpose, but it also means that a write through one object changes what all the others see. Make sure that is what the domain wants.

**Accessors left out.** If code reads the field directly in some places, the move breaks them. Encapsulate first.

**Persisted data.** If the field is stored in a database or a file format, the move is also a data migration. Plan the migration and the code change together.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. Test after each step.

## References
