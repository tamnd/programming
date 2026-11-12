---
tag: 675H79
kind: technique
slug: move-statements-to-callers
name: Move statements to callers
aliases: []
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  When some callers of a function need part of its behavior to be different, move that part out of the function into each caller; the inverse of Move Statements into Function.
family: refactoring
polarity: positive
intent: "Let callers vary a part of a function that no longer means the same thing for all of them."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Move Statements to Callers is in the chapter on moving features of the second edition of Refactoring (2018). Its inverse is Move Statements into Function.
see_also: [move-statements-into-function, extract-function, inline-function, change-function-declaration, slide-statements, boolean-parameter-smell]
sources:
  - url: https://refactoring.com/catalog/moveStatementsToCallers.html
    title: Martin Fowler, Catalog of Refactorings, Move Statements to Callers
    locator: the entry; inverse of Move Statements into Function
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Move Statements to Callers, motivation and mechanics
---

## Purpose

A function is a boundary: the statements inside it run for every caller. The boundary can be right when you draw it and wrong a year later, when one caller needs a step done another way. A flag parameter that switches the step, or a copy of the function, are poor fixes.

Move statements to callers moves the step out of the function into each caller. Callers that need the old behavior write it; the caller that needs a different behavior writes its own. The catalogue card moves the line that writes a photo's location out of `emitPhotoData` into its callers.[^1] The inverse is [[move-statements-into-function|Move Statements into Function]].[^1]

The book describes this as the case where a behavior that was common to callers needs to vary for some of them.[^2] It is the most direct when the varying part is at the start or the end of the function.

## Steps

1. In a small case, with one or two callers, cut the statements from the function and paste them into each caller. Test. Stop here.[^2]
2. Otherwise, use [[extract-function|Extract Function]] on all the statements that stay. Give the result a temporary name.[^2]
3. Inline the original function into each caller with [[inline-function|Inline Function]]. Each caller now calls the extracted function and has the moved statements of its own.
4. Rename the extracted function to the original name with [[change-function-declaration|Change Function Declaration]], and test.

If the varying statements are in the middle of the function, first use [[slide-statements|Slide Statements]] to move them to the start or the end, if the order allows.

## Example

A receipt function writes the items and then a footer. The till and the email both call it, but the email now needs a footer with an unsubscribe line. The footer moves to the callers.

```javascript run id=let-callers-vary
import assert from "node:assert/strict";

const items = [{ name: "Tea", cents: 350 }, { name: "Scone", cents: 275 }];

// Before: the footer is inside, the same for every caller.
function receiptOld(items) {
  const lines = items.map((i) => `${i.name} ${(i.cents / 100).toFixed(2)}`);
  lines.push("Thank you!");
  return lines;
}

// After: the function keeps what all callers share.
function receipt(items) { return items.map((i) => `${i.name} ${(i.cents / 100).toFixed(2)}`); }
const till = [...receipt(items), "Thank you!"];
const email = [...receipt(items), "Thank you!", "Unsubscribe: example.com/u"];

assert.deepEqual(till, receiptOld(items));
assert.deepEqual(email.slice(0, 3), receiptOld(items));
assert.equal(email.at(-1), "Unsubscribe: example.com/u");
console.log("move statements to callers: tests pass");
```

```output
move statements to callers: tests pass
```

The till caller is the same as before. Without the move, the receipt function would need a parameter such as `forEmail`, which is a [[boolean-parameter-smell|flag argument]].

| Language | What the example shows |
|---|---|
| JavaScript | a footer moved out of the function into two callers, as above |
| Python | the mechanics for many callers: extract what stays, inline the old function, rename |
| Go | the same move, where each caller writes the footer to its own writer |

## When to use

- **One caller needs it different.** A behavior common to all callers must vary for some of them.[^2]
- **A flag grows.** A boolean parameter that switches a step in or out is a sign that the step belongs to the callers.

## Pitfalls

**Duplication.** After the move, each caller has a copy of the statements. If most callers still need the same statements, that is [[duplicated-code|Duplicated Code]]. Keep the move for the case where the callers really differ, or move the statements back later with the inverse.

**Statements in the middle.** A step in the middle of a function cannot move out without changing the order of what runs. If the order matters, consider passing the step in as a function argument, not moving it.

**A boundary that moves again.** The book's point is that boundaries shift as needs change; this refactoring and its inverse move them in the two directions.[^2]

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The steps use Extract Function, Inline Function and Change Function Declaration; see those pages for the tools.

## References
