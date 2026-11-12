---
tag: 72AAWM
kind: technique
slug: preserve-whole-object
name: Preserve whole object
area: construction-practice/refactoring-apis
level: core
summary: >-
  Pass the whole object to a function instead of several values that the caller reads out of that object first.
family: refactoring
polarity: positive
intent: "Let the function take the record that its values come from."
history:
  year: 1999
  people: [Martin Fowler]
  note: The refactoring is in the first edition of Refactoring (1999), the Ruby edition and the second edition (2018), where it is in the chapter on refactoring APIs.
see_also: [long-parameter-list, data-clumps, feature-envy, introduce-parameter-object, replace-parameter-with-query, change-function-declaration]
sources:
  - url: https://refactoring.com/catalog/preserveWholeObject.html
    title: Martin Fowler, Catalog of Refactorings, Preserve Whole Object
    locator: the entry, the daysTempRange sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry preserveWholeObject, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Preserve Whole Object
---

## Purpose

Some callers read two or more values out of one object and pass them to a function. Each caller repeats the same reads. If the function later needs a third value from the same object, each caller and the signature must change.

Preserve whole object passes the object itself. The function reads the values that it needs. The catalogue card reads `low` and `high` out of `aRoom.daysTempRange` and calls `aPlan.withinRange(low, high)`. After the refactoring, the call is `aPlan.withinRange(aRoom.daysTempRange)`.[^1]

## Steps

1. Make a new function with the parameter that you want: the whole object. Give it a temporary name.
2. Write the body of the new function as a call to the old function. Read each old argument out of the object.
3. Change each caller to call the new function. Test after each change. The reads in the caller are now dead code, so remove them.
4. Inline the old function into the new one, and give the new function the old name.

## Example

A heating plan checks if a room stayed inside a range. Each caller reads `low` and `high` from the range of the room. After the refactoring, the plan takes the range.

```javascript run id=whole-object
import assert from "node:assert/strict";

const room = { name: "lab", daysTempRange: { low: 18, high: 23 } };
const plan = { low: 16, high: 24 };

function withinRangeBefore(aPlan, bottom, top) {
  return bottom >= aPlan.low && top <= aPlan.high;
}

function withinRange(aPlan, aRange) {
  return aRange.low >= aPlan.low && aRange.high <= aPlan.high;
}

const low = room.daysTempRange.low;
const high = room.daysTempRange.high;
assert.equal(withinRange(plan, room.daysTempRange), withinRangeBefore(plan, low, high));
assert.equal(withinRange(plan, { low: 12, high: 20 }), false);
console.log(withinRange(plan, room.daysTempRange));
```

```output
true
```

| Language | What the example shows |
|---|---|
| JavaScript | `withinRange(plan, low, high)` replaced with `withinRange(plan, range)`, as above |
| Python | the same change, with the range as a frozen dataclass |
| Go | the same change; the range is a struct passed by value, so the function gets a copy and cannot change the range of the room |

## When to use

- **The same values come out of one object at each call.** This is a sign of [[data-clumps|Data clumps]] and [[long-parameter-list|Long parameter list]].
- **The function will need more of the object later.** With the whole object, a new field needs no change to the signature.
- **The function only computes with the values of another object.** This can be [[feature-envy|Feature envy]]. After the refactoring, see if the function belongs on the object it takes.

## Pitfalls

**A new dependency.** The function now depends on the type of the whole object, not only on two numbers. If the function is in a module that must not know that type, keep the values, or define a small interface with only the fields that the function reads.

**Callers without the object.** Some callers have the two values but no object. They must then build one. If this is common, keep the old signature for them.

**The values are not a part of one object.** If the values come from different objects that are always used together, Preserve whole object does not apply. Make a new type with [[introduce-parameter-object|Introduce Parameter Object]].

**The function can get the value itself.** If the function can compute a parameter from another parameter, remove it with [[replace-parameter-with-query|Replace Parameter with Query]].

## Tools that do it

No IDE has one action for this refactoring. The steps use Extract Function, Change Function Declaration and Inline Function. See [[change-function-declaration|Change Function Declaration]].

The catalogue lists the refactoring in the first edition, the second edition and the Ruby edition, with the tags refactoring-apis and parameters.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
