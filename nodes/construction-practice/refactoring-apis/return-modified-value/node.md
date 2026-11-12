---
tag: ZZV861
kind: technique
slug: return-modified-value
name: Return modified value
area: construction-practice/refactoring-apis
level: core
summary: >-
  Change a function that updates a variable outside itself so that it computes the value and returns it, and let the caller assign the result.
family: refactoring
polarity: positive
intent: "Make the data that a function changes visible at the call."
history:
  year: 2018
  people: [Martin Fowler]
  note: The refactoring is new in the second edition of Refactoring (2018), in the chapter on refactoring APIs.
see_also: [separate-query-from-modifier, side-effect, mutable-data, global-data, extract-function, split-variable]
sources:
  - url: https://refactoring.com/catalog/returnModifiedValue.html
    title: Martin Fowler, Catalog of Refactorings, Return Modified Value
    locator: the entry, the totalAscent and calculateAscent sketch
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings, index
    locator: the entry returnModifiedValue, its books and tags
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 11, Refactoring APIs, Return Modified Value
---

## Purpose

When a function changes a variable that the caller owns, the call does not show the change. A reader who sees `calculateAscent();` cannot tell that the call writes to `totalAscent`. To find out which code changes the variable, the reader must read the body of each function.

Return modified value makes the function compute the value and return it. The caller assigns the result. The catalogue card starts with `let totalAscent = 0; calculateAscent();`, where the function adds to the outer variable `totalAscent`. After the refactoring, the call is `const totalAscent = calculateAscent();`, and the function sums into a local `result` that it returns.[^1]

The variable can now be a `const`, and the data flow is in the call.

## Steps

1. Make the function return the value of the variable that it changes. Assign the result to the variable at the call.
2. Test.
3. Declare a local variable in the function for the value, and use it in place of the outer variable.
4. Test. If the outer variable is no longer changed anywhere else, make it a constant.
5. Rename the local variable, for example to `result`.

The refactoring works best when the function changes one value. For two or more values, split the function first, for example with [[split-variable|Split Variable]] and [[extract-function|Extract Function]].

## Example

A route is a list of points with an elevation. The total ascent is the sum of each climb between two points.

```javascript run id=returned-value
import assert from "node:assert/strict";

const points = [{ elevation: 10 }, { elevation: 25 }, { elevation: 20 }, { elevation: 32 }];

let totalAscentBefore = 0;
function calculateAscentBefore() {
  for (let i = 1; i < points.length; i++) {
    const verticalChange = points[i].elevation - points[i - 1].elevation;
    totalAscentBefore += verticalChange > 0 ? verticalChange : 0;
  }
}
calculateAscentBefore();

function calculateAscent(points) {
  let result = 0;
  for (let i = 1; i < points.length; i++) {
    const verticalChange = points[i].elevation - points[i - 1].elevation;
    result += verticalChange > 0 ? verticalChange : 0;
  }
  return result;
}
const totalAscent = calculateAscent(points);

assert.equal(totalAscent, totalAscentBefore);
// A second call of the old function changes the total again.
calculateAscentBefore();
assert.equal(totalAscentBefore, 2 * totalAscent);
// A second call of the new function changes nothing.
assert.equal(calculateAscent(points), totalAscent);
console.log(totalAscent);
```

```output
27
```

The new function also takes the points as a parameter. That step is [[replace-query-with-parameter|Replace Query with Parameter]], and it is not a part of this refactoring.

| Language | What the example shows |
|---|---|
| JavaScript | `calculateAscent` changed from an update of an outer `let` to a function that returns the total, as above |
| Python | the same change; the old version must declare the outer name with `global` to assign to it |
| Go | the same change; the old version updates a variable through a pointer argument |

## When to use

- **A function writes to a variable of the caller or to a global.** See [[global-data|Global data]] and [[mutable-data|Mutable data]].
- **One value is changed.** The refactoring then turns the function into one that returns that value. If the function has no other side effect, it becomes a query. See [[separate-query-from-modifier|Separate Query from Modifier]].
- **Before you move the function.** A function that returns its result can move to another module, and the caller keeps the variable.

## Pitfalls

**Many values.** A function that updates two or three variables cannot return all of them as one plain value. Return a record or a tuple only if the values belong together. Otherwise, split the function.

**Large data.** A function that changes a large array in place can be faster than one that returns a new copy. This refactoring is for values. For a large structure, keep the in-place change, and make it clear in the name of the function.

**Other writers.** If other code also writes to the same variable, the variable cannot become a constant. Then the refactoring only makes one of the writes visible.

## Tools that do it

No IDE has one action for this refactoring. The changes are small, and the steps are done by hand.

The catalogue lists the refactoring in the second edition only, with the tags refactoring-apis and immutability.[^2] In the second edition it is in Chapter 11, Refactoring APIs.[^3]

## References
