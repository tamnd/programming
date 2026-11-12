---
tag: T51PXF
kind: technique
slug: replace-inline-code-with-function-call
name: Replace inline code with function call
aliases: []
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Replace inline code that does what an existing function already does with a call to that function, from the project or from a library, so that the code says what it does by name.
family: refactoring
polarity: positive
intent: "Use the name of an existing function in place of code that repeats its work."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Replace Inline Code with Function Call is in the chapter on moving features of the second edition of Refactoring (2018).
see_also: [extract-function, substitute-algorithm, duplicated-code, inline-function]
sources:
  - url: https://refactoring.com/catalog/replaceInlineCodeWithFunctionCall.html
    title: Martin Fowler, Catalog of Refactorings, Replace Inline Code with Function Call
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Replace Inline Code with Function Call, motivation and mechanics
---

## Purpose

A function gives a piece of behavior a name. When the same behavior is written out inline, the reader must work out what it does, and if the behavior changes, each copy must change.

Replace inline code with function call puts a call to the existing function in place of the inline code. The catalogue card replaces a loop that sets a flag when a list of states contains `"MA"` with one call to `includes`.[^1] The function can be one in the project, or one in the language's library; the book notes that a call to a library function is even better, because then you do not write the function body at all.[^2]

This differs from [[extract-function|Extract Function]] in one way: the function already exists. If the inline code only looks like the function by chance, and the two would change for different reasons, they are not the same behavior. The book's test is whether a change to the function's body should also change the behavior of the inline code.[^2]

## Steps

1. Replace the inline code with a call to the existing function.[^2]
2. Test.

## Example

Three places format an amount of cents as a price, each with its own inline code, and one checks a prefix by slicing. The project already has `formatCents`, and the language has `startsWith`. After the refactoring, the code calls them.

```javascript run id=say-it-by-name
import assert from "node:assert/strict";

// The project already has this function.
function formatCents(c) { return `${(c / 100).toFixed(2)} EUR`; }

const order = { totalCents: 1999, shippingCents: 450, coupon: "SAVE-10" };

// Before: inline code that repeats formatCents and startsWith.
const totalBefore = (order.totalCents / 100).toFixed(2) + " EUR";
const shipBefore = `${(order.shippingCents / 100).toFixed(2)} EUR`;
const isSaveBefore = order.coupon.slice(0, 5) === "SAVE-";

// After: calls to the functions that already exist.
const total = formatCents(order.totalCents);
const ship = formatCents(order.shippingCents);
const isSave = order.coupon.startsWith("SAVE-");

assert.deepEqual([total, ship, isSave], [totalBefore, shipBefore, isSaveBefore]);
assert.equal(total, "19.99 EUR");
console.log("replace inline code with function call: tests pass");
```

```output
replace inline code with function call: tests pass
```

If the shop later shows prices as `€19.99`, one function changes, and all three places follow.

| Language | What the example shows |
|---|---|
| JavaScript | inline formatting replaced by a project function, and a slice replaced by `startsWith`, as above |
| Python | a hand-written loop replaced by `any`, and string building replaced by `str.join` |
| Go | inline loops replaced by `slices.Contains` and `strings.HasPrefix` from the standard library |

## When to use

- **Inline code that repeats a function.** The project or the library already has a function that does the same thing.[^2]
- **After Extract Function elsewhere.** When you extract a function in one place, look for other places that do the same thing inline. See [[duplicated-code|Duplicated Code]].
- **Hand-written library code.** A loop that searches, joins or counts is often one call to the standard library.

## Pitfalls

**Same code, different meaning.** Two pieces of code that compute the same thing now may change for different reasons. If they would, keep them apart.[^2]

**Edge cases.** A library function can behave differently on empty input, on `null`, on Unicode or on rounding. Check that the inline code and the function agree on the cases that matter, as the test in the example does. When they do not agree on all inputs, the change is [[substitute-algorithm|Substitute Algorithm]], and it needs more tests.

**A function with side effects.** If the existing function also logs, caches or changes state, calling it from a place that did not do that adds the side effect. Read the function before you call it.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The change itself is often a one-line edit and a test.

## References
