---
tag: XSMBY1
kind: technique
slug: combine-functions-into-class
name: Combine functions into class
aliases: [group functions into a class]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Turn a group of functions that all take the same data into a class that holds the data, so that the shared context is explicit and the calls get shorter.
family: refactoring
polarity: positive
intent: "Make the data that several functions share into the state of one class, and the functions into its methods."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: The second edition of Refactoring (2018) has Combine Functions into Class in its first set of refactorings, next to Combine Functions into Transform.
see_also: [combine-functions-into-transform, introduce-parameter-object, encapsulate-variable, data-class, feature-envy, code-smell]
sources:
  - url: https://refactoring.com/catalog/combineFunctionsIntoClass.html
    title: Martin Fowler, Catalog of Refactorings, Combine Functions into Class
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Combine Functions into Class, motivation and mechanics; Combine Functions into Transform, motivation
  - url: https://www.jetbrains.com/help/idea/extract-into-class-refactorings.html
    title: IntelliJ IDEA documentation, Extract into class refactorings
    locator: Extract delegate, Extract method object
  - url: https://code.visualstudio.com/docs/typescript/typescript-refactoring
    title: Visual Studio Code documentation, Refactoring TypeScript and JavaScript
    locator: the list of refactorings, the move of top-level code to a file of its own
---

## Purpose

A program often has a group of functions that each take the same record as their first argument: `volume(parcel)`, `chargeableWeight(parcel)`, `shippingCost(parcel)`. The record is the common context of the group, but nothing in the code says so. Each caller passes the record again, and each reader must find the functions in the files where they happen to live.

Combine functions into class makes the record the state of a class and the functions its methods. The catalogue card shows `base(aReading)`, `taxableCharge(aReading)` and `calculateBaseCharge(aReading)` becoming the methods `base()`, `taxableCharge()` and `calculateBaseCharge()` of a class `Reading`.[^1] The calls lose an argument, the group has one home, and the object can be passed to other parts of the program as one thing.[^2]

The class is also a place for the logic that each caller wrote for itself. If three callers each computed the volume, that computation can become one more method.

## Steps

1. Run the tests and see them pass.
2. Put the common data behind a class. If it is a record, Encapsulate Record does this; the class starts with the data and its accessors.[^2]
3. Test.
4. Move each function that takes the record into the class, as a method. Remove the parameter, and change each caller to call the method. Test after each.
5. Find logic in the callers that works on the data, extract it into a function, and move that function into the class too. Test after each.

## Example

Three functions price a parcel, and a caller computes the volume for itself. After the refactoring the parcel is a class, and the volume is a method that every caller shares.

```javascript run id=one-home
import assert from "node:assert/strict";

// Before: functions that each take the parcel record.
const DIVISOR = 5000; // cm^3 per kg of volumetric weight
function chargeableWeight(p) {
  const volume = p.length * p.width * p.height; // the caller computes it
  return Math.max(p.weightKg, volume / DIVISOR);
}
function shippingCost(p, ratePerKg) {
  return Math.ceil(chargeableWeight(p)) * ratePerKg;
}

// After: the data and the functions in one class.
class Parcel {
  #length; #width; #height; #weightKg;
  constructor({ length, width, height, weightKg }) {
    this.#length = length;
    this.#width = width;
    this.#height = height;
    this.#weightKg = weightKg;
  }
  get volume() { return this.#length * this.#width * this.#height; }
  get chargeableWeight() { return Math.max(this.#weightKg, this.volume / DIVISOR); }
  shippingCost(ratePerKg) { return Math.ceil(this.chargeableWeight) * ratePerKg; }
}

const records = [
  { length: 40, width: 30, height: 20, weightKg: 2 }, // light and bulky
  { length: 10, width: 10, height: 10, weightKg: 7 }, // small and heavy
];
for (const r of records) {
  assert.equal(new Parcel(r).shippingCost(3), shippingCost(r, 3));
}
assert.deepEqual(records.map((r) => new Parcel(r).shippingCost(3)), [15, 21]);
console.log("combine functions into class: tests pass");
```

```output
combine functions into class: tests pass
```

The calls now read `parcel.shippingCost(3)`. The class holds the rule for the chargeable weight in one place, and a caller that needs the volume asks the parcel.

| Language | What the example shows |
|---|---|
| JavaScript | the parcel functions as a class, as above |
| Python | the same class, with `functools.cached_property` for a derived value |
| Go | a struct with methods; the methods are the Go form of the class |

## When to use

- **Several functions take the same data as their first argument.** That shared argument is the state of a class that is not written yet.
- **Callers repeat logic on the data.** The logic belongs with the data. This is the cure for [[feature-envy|Feature Envy]] in the callers, and for a [[data-class|Data Class]] with no behaviour.
- **The data is updated.** If the source data can change, a class computes the derived values from the current state each time. The book prefers a class over [[combine-functions-into-transform|Combine Functions into Transform]] in this case, because a transform copies values that go stale.[^2]

If the data never changes and the program passes it as plain records, for example in a pipeline, a transform can fit better.[^2]

## Pitfalls

**A class for its own sake.** Two functions that share a parameter by chance do not need a class. The test is whether the functions belong to one concept that the program talks about.

**A class that grows too much.** Once the class exists, every function that touches the data tends to move in. Watch for [[large-class|Large Class]], and split by responsibility when it grows.

**The language does not need a class.** In Go, methods on a struct do the work; in a functional language, a module with the type and its functions does the same job. The refactoring is about one home for the data and its operations, and the form follows the language.

**Shared mutable state.** If callers held the old record and changed it, they now hold an object whose state the methods read. Decide whether the class copies its input or keeps a reference.

## Tools that do it

No tool in the table does the whole refactoring in one step. The tools do the parts.

| Tool | Languages | What it automates |
|---|---|---|
| IntelliJ IDEA | Java | Extract Delegate, which lets you "extract some of the fields and methods of a class into a separate, newly created class", and Extract Method Object, which "moves method into a new class"[^3] |
| VS Code | TypeScript, JavaScript | "Move to new file", which moves classes, functions and constants in the top-level scope to a file of their own[^4] |

The moves of each function into the class, and the change of each call from `f(record)` to `record.f()`, are the programmer's steps, and the tests check each one.

## References
