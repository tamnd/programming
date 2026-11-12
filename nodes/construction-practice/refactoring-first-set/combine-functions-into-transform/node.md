---
tag: EKTA28
kind: technique
slug: combine-functions-into-transform
name: Combine functions into transform
aliases: [enrich function]
area: construction-practice/refactoring-first-set
level: core
summary: >-
  Gather the calculations of values derived from a record into one function that takes the record and returns a copy with the derived values added.
family: refactoring
polarity: positive
intent: "Compute the values derived from a record in one place, and give them to every reader as fields of an enriched copy."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: The second edition of Refactoring (2018) has Combine Functions into Transform in its first set of refactorings, next to Combine Functions into Class.
see_also: [combine-functions-into-class, split-phase, extract-function, duplicated-code, code-smell]
sources:
  - url: https://refactoring.com/catalog/combineFunctionsIntoTransform.html
    title: Martin Fowler, Catalog of Refactorings, Combine Functions into Transform
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 6, A First Set of Refactorings, Combine Functions into Transform, motivation and mechanics
  - url: https://developer.mozilla.org/en-US/docs/Web/API/Window/structuredClone
    title: MDN Web Docs, Window, structuredClone() method
    locator: the introduction
  - url: https://docs.python.org/3/library/copy.html
    title: Python documentation, copy, Shallow and deep copy operations
    locator: the difference between shallow and deep copying
---

## Purpose

Programs often read a record and then compute values from it: a charge from a reading, a total from an order, a chargeable weight from a parcel. When several parts of the program need the same derived value, each tends to compute it for itself, and the rule is repeated. When the rule changes, one copy is missed.

Combine functions into transform puts all these calculations in one function. The function takes the source record, makes a copy, adds each derived value as a field of the copy, and returns it. Every reader then takes the derived values from the enriched record. The catalogue card shows `base(aReading)` and `taxableCharge(aReading)` combined into `enrichReading(argReading)`, which deep-clones the reading and sets `baseCharge` and `taxableCharge` on the clone.[^1]

The refactoring is the twin of [[combine-functions-into-class|Combine Functions into Class]]. Both give the derived logic one home. The book gives the rule for the choice: if the code updates the source data, use a class, because a transform stores the derived values when it runs and they do not follow later changes.[^2] A transform fits data that is read, enriched and passed on, as in a pipeline.

## Steps

1. Run the tests and see them pass.
2. Write a transform function that takes the record and returns a deep copy of it, with no other change. Add a test that the source record is not changed.
3. Pick one derived value. Move its calculation into the transform, as a field of the copy.
4. Change one reader to take the field, and test. Change the other readers in the same way, testing after each.
5. Repeat for each derived value.
6. Name the transform after what it does. The book uses the name `enrich...` when the output is the same record with more fields, and `transform...` when the output is a different thing.[^2]

## Example

Two parts of a shipping program compute the chargeable weight of a parcel. After the refactoring, one function enriches the parcel and both parts read the field.

```javascript run id=compute-once
import assert from "node:assert/strict";

const DIVISOR = 5000; // cm^3 per kg of volumetric weight

// Before: two readers repeat the derived rule.
function label(p) {
  const w = Math.max(p.weightKg, (p.length * p.width * p.height) / DIVISOR);
  return `${p.id}: ${Math.ceil(w)} kg`;
}
function cost(p, rate) {
  const w = Math.max(p.weightKg, (p.length * p.width * p.height) / DIVISOR);
  return Math.ceil(w) * rate;
}

// After: one transform computes the derived values on a copy.
function enrichParcel(source) {
  const p = structuredClone(source);
  p.volume = p.length * p.width * p.height;
  p.chargeableKg = Math.ceil(Math.max(p.weightKg, p.volume / DIVISOR));
  return p;
}
const labelAfter = (p) => `${p.id}: ${p.chargeableKg} kg`;
const costAfter = (p, rate) => p.chargeableKg * rate;

const raw = { id: "P1", length: 40, width: 30, height: 20, weightKg: 2, tags: ["fragile"] };
const parcel = enrichParcel(raw);
assert.equal(labelAfter(parcel), label(raw));
assert.equal(costAfter(parcel, 3), cost(raw, 3));
assert.equal("chargeableKg" in raw, false); // the source is not changed
parcel.tags.push("express");
assert.deepEqual(raw.tags, ["fragile"]); // a deep copy: nested data is not shared

// The trap: a change to the source after the transform is not seen.
raw.weightKg = 30;
assert.equal(labelAfter(parcel), "P1: 5 kg"); // stale
assert.equal(labelAfter(enrichParcel(raw)), "P1: 30 kg");
console.log("combine functions into transform: tests pass");
```

```output
combine functions into transform: tests pass
```

`structuredClone` makes a deep copy, so the enriched record shares no nested object with the source.[^3] The last lines show why the book prefers a class when the source changes: the enriched copy keeps the weight it was made with.

| Language | What the example shows |
|---|---|
| JavaScript | an enrich function with `structuredClone`, and a stale copy, as above |
| Python | the same transform with `copy.deepcopy`, and the difference from a shallow copy |
| Go | a transform that returns a struct with the source and the derived fields |

## When to use

- **Several readers compute the same derived value.** The repeated rule is [[duplicated-code|Duplicated Code]], and the transform is one home for it.
- **The data is read, not updated.** Records read from a file, a request or a database, enriched once and passed on.
- **The program is a pipeline.** A transform is one stage, and it can be followed by others. Combined with [[split-phase|Split Phase]], it often becomes the first phase that prepares the data.

## Pitfalls

**Stale derived values.** The fields are computed when the transform runs. If code changes the source afterwards, the fields are wrong. The example shows this. If the data changes, use a class.[^2]

**A shallow copy.** A shallow copy shares the nested objects with the source, so adding to a nested list in the copy also changes the source. The Python documentation puts the difference so: a shallow copy "inserts references into it to the objects found in the original", while a deep copy "inserts copies into it of the objects found in the original".[^4]

**Readers that still compute.** A reader that keeps its own copy of the rule defeats the refactoring. Search for the calculation after the change, and remove the copies.

**Many transforms on one record.** Two transforms that both add fields to the same record can disagree, or overwrite each other. Keep one transform for one kind of derived data.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool: an [[extract-function|Extract Function]] for each calculation, a move of the calculation into the transform, and a change of each reader. The extraction and the rename steps can use the actions of the tools, and the tests check each step.

## References
