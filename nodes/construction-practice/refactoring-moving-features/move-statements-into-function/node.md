---
tag: 1E3P5B
kind: technique
slug: move-statements-into-function
name: Move statements into function
aliases: []
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  When the same statements run each time a function is called, move them into the function, so that callers cannot forget them; the inverse of Move Statements to Callers.
family: refactoring
polarity: positive
intent: "Make code that always goes with a call part of the function it goes with."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Move Statements into Function is in the chapter on moving features of the second edition of Refactoring (2018). Its inverse is Move Statements to Callers.
see_also: [move-statements-to-callers, slide-statements, extract-function, inline-function, change-function-declaration, duplicated-code]
sources:
  - url: https://refactoring.com/catalog/moveStatementsIntoFunction.html
    title: Martin Fowler, Catalog of Refactorings, Move Statements into Function
    locator: the entry; inverse of Move Statements to Callers
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Move Statements into Function, motivation and mechanics
---

## Purpose

If every caller of a function writes the same lines before or after the call, those lines are part of what the function means. Left in the callers, they are [[duplicated-code|duplicated code]], and a caller that forgets them is a bug.

Move statements into function puts them inside. The catalogue card moves the line that writes a photo's title into the `photoData` function that already writes its location and date.[^1] The inverse is [[move-statements-to-callers|Move Statements to Callers]].[^1]

The book's test is whether the statements make sense only as part of the called function. If they do, move them in. If they do not, but they still always run with the call, the better fix is [[extract-function|Extract Function]] on the statements and the call together.[^2]

## Steps

1. If the repeated statements are not next to the call, move them there with [[slide-statements|Slide Statements]].[^2]
2. If the function has only one caller, cut the statements from the caller, paste them into the function, and test. Stop here.
3. Otherwise, at one call site, use [[extract-function|Extract Function]] on the call and the statements together. Give the result a temporary name.[^2]
4. Change each other caller to use the extracted function. Test after each.
5. When all callers use it, inline the original function into the extracted one with [[inline-function|Inline Function]].[^2]
6. Rename the extracted function to the original name with [[change-function-declaration|Change Function Declaration]], and test.

## Example

Two callers print an address block, and each first writes the recipient's name above the lines that `addressLines` returns. The name moves into the function, which now takes the recipient.

```javascript run id=always-together
import assert from "node:assert/strict";

const ana = { name: "Ana Pires", address: { street: "Rua Augusta 10", city: "Lisboa", postcode: "1100-053" } };

// Before: each caller writes the name, then calls the function.
function addressLinesOld(a) { return [a.street, `${a.postcode} ${a.city}`]; }
const labelOld = [ana.name, ...addressLinesOld(ana.address)];
const invoiceOld = ["Bill to:", ana.name, ...addressLinesOld(ana.address)];

// After: the name is part of the address block.
function addressBlock(r) { return [r.name, r.address.street, `${r.address.postcode} ${r.address.city}`]; }
const label = addressBlock(ana);
const invoice = ["Bill to:", ...addressBlock(ana)];

assert.deepEqual(label, labelOld);
assert.deepEqual(invoice, invoiceOld);
console.log("move statements into function: tests pass");
```

```output
move statements into function: tests pass
```

The invoice still adds its own "Bill to:" line. That line varies between callers, so it stays outside.

| Language | What the example shows |
|---|---|
| JavaScript | a repeated line moved into the function, with the varying line left outside, as above |
| Python | the mechanics for several callers: extract at one call site, switch the others, inline the old function, rename |
| Go | the same move, where the function's parameter changes from the address to the recipient |

## When to use

- **The same lines before or after each call.** They are part of the function.[^2]
- **A caller that forgot.** A bug from one caller that did not write the lines is a sign that they belong inside.
- **After moving functions.** When a function moves, the statements around its calls can be part of the move.

## Pitfalls

**Statements that vary.** If even one caller needs different statements, moving them in forces a flag parameter or a copy of the function. Leave those statements outside, or keep the variation in the caller.

**Order and side effects.** Sliding the statements next to the call can change the order of side effects. Check that nothing between them depends on the order before step 1.

**A changed signature.** If the moved lines use data that the function did not have, the function needs a parameter for it, as in the example. Callers then change too.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The steps use Extract Function, Inline Function and Change Function Declaration; see those pages for the tools.

## References
