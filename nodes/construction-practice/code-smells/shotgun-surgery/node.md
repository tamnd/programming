---
tag: KVWEHH
kind: pattern
slug: shotgun-surgery
name: Shotgun Surgery
aliases: []
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: one change to the behaviour of a program needs many small edits in many modules, and an edit that is missed is a bug.
family: code-smell
polarity: negative
intent: "Find the knowledge that is spread over many modules, and gather it in one place, so that one change is one edit."
also_known_as: []
refactorings: [move-function, move-field, combine-functions-into-class, combine-functions-into-transform, split-phase, inline-function, inline-class]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Shotgun Surgery is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, divergent-change, duplicated-code, global-data]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Shotgun Surgery
  - url: https://en.wikipedia.org/w/index.php?title=Shotgun_surgery&oldid=1381335030
    title: Wikipedia, Shotgun surgery, revision 1381335030
    locator: the introduction and Consequences of shotgun surgery
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Move Function, Move Field, Combine Functions into Class, Combine Functions into Transform, Split Phase, Inline Function and Inline Class
  - url: https://en.wikipedia.org/w/index.php?title=Single-responsibility_principle&oldid=1382198334
    title: Wikipedia, Single-responsibility principle, revision 1382198334
    locator: the quote from Robert C. Martin's blog post of 2014
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Shotgun Surgery
---

## Intent

Find the pieces of one decision that are spread over many modules, and gather them in one place. Then a change to the decision is one edit in one module, and no part of it can be missed.

## Problem

A programmer gets a small request: add a discount code. The work turns out to need an edit in the price module, an edit in the receipt module and an edit in the validation module. Each edit is small. The risk is the edit that nobody remembers. *Refactoring* gives this as the problem: when the changes are all over the code, they are hard to find, and it is easy to miss one that matters.[^1]

The forces are these:

- Code is often split by technical layer: parsing, validation, storage, display. A business change, such as a new field or a new rule, cuts across all the layers, so it needs an edit in each.
- Copy and paste spreads the same knowledge. The Wikipedia article on the smell says that the code in each place "looks similar and may only have slight variations", and that a bug in it "will be replicated many-fold".[^2]
- The places do not point to each other. Nothing in the price module says that the receipt module also knows the list of codes. The only guide is a search, or the memory of the person who wrote the code.

Wikipedia also notes that "There is also some evidence that this anti-pattern is correlated with higher defect rates."[^2]

## Solution

Gather the parts of the decision into one module. *Refactoring* names the refactorings for it.[^1] The catalogue at refactoring.com has a card for each.[^3]

1. **Functions and fields in the wrong modules:** [[move-function|Move Function]] and [[move-field|Move Field]] to put them in the module that owns the decision.
2. **Several functions that work on the same data:** [[combine-functions-into-class|Combine Functions into Class]], or [[combine-functions-into-transform|Combine Functions into Transform]] when the functions compute values from a record that does not change.
3. **Code that does the same steps in many places:** [[split-phase|Split Phase]], when the steps form a sequence that one module can own.
4. **Logic split into pieces that are too small:** [[inline-function|Inline Function]] and [[inline-class|Inline Class]] to pull the pieces together first. *Refactoring* notes that the result can be a large function or class for a time, and that it can be split again in a better place.[^1]

The goal is the second half of the rule that Robert C. Martin wrote for the single-responsibility principle: "Gather together the things that change for the same reasons."[^4]

## Example

The list of discount codes is known in three places: the price function, the receipt text and the validation. A new code needs three edits, and the test shows what happens when one is missed.

```javascript run id=gather-the-codes
import assert from "node:assert/strict";

// Before: three functions each know the codes.
function priceBefore(cents, code) {
  if (code === "SPRING10") return Math.round(cents * 0.9);
  if (code === "VIP25") return Math.round(cents * 0.75);
  return cents;
}
function receiptBefore(code) {
  if (code === "SPRING10") return "SPRING10 (10% off)";
  if (code === "VIP25") return "VIP25 (25% off)";
  return "no discount";
}
const isValidBefore = (code) => ["SPRING10", "VIP25", "SUMMER20"].includes(code);
// SUMMER20 was added to the validation, but not to the price or the receipt.

assert.equal(isValidBefore("SUMMER20"), true);
assert.equal(priceBefore(1000, "SUMMER20"), 1000); // a valid code with no effect
console.log("before: one of three edits was made");

// After: one table owns the codes, and the three functions read it.
const discounts = new Map([["SPRING10", 10], ["VIP25", 25], ["SUMMER20", 20]]);

const isValid = (code) => discounts.has(code);
const price = (cents, code) => Math.round((cents * (100 - (discounts.get(code) ?? 0))) / 100);
const receipt = (code) => (isValid(code) ? `${code} (${discounts.get(code)}% off)` : "no discount");

assert.equal(price(1000, "SPRING10"), 900);
assert.equal(price(1000, "SUMMER20"), 800);
assert.equal(receipt("SUMMER20"), "SUMMER20 (20% off)");
assert.equal(price(1000, "BOGUS"), 1000);
assert.equal(receipt("BOGUS"), "no discount");
console.log("after: tests pass");
```

```output
before: one of three edits was made
after: tests pass
```

In the "after" version, a new code is one line in the table. The price, the receipt and the validation cannot disagree, because they read the same data. The percentage is stored once, so the receipt text cannot say 20% while the price takes 25%.

| Language | What the example shows |
|---|---|
| JavaScript | the table of codes above, as a `Map` |
| Python | the same table as a `dict`, in one module with the three functions |
| Java | an `enum` that holds the codes and their percentages |
| Go | a map in one package, with the three functions next to it |

## Consequences

The decision has one home, and one edit changes it. A reviewer sees the whole decision in one place. A test of the module covers all its uses.

The gathered module can become a new problem. If it holds the knowledge of many decisions, it changes for many reasons, which is [[divergent-change|Divergent Change]]. *Refactoring* treats the two smells as opposites: Divergent Change is one module that suffers many kinds of change, and Shotgun Surgery is one change that touches many modules. The aim is a design where each kind of change touches one module, and each module has one kind of change.[^1][^5]

Not every spread is a smell. A field that is added to a record must often appear in the record, in its storage and in its display. A code generator, a schema or a table that drives all three can remove the spread. A small project may accept the three edits, if the tests fail when one of them is missing.

## How to fix it

- **Functions in the wrong module:** [[move-function|Move Function]] to the module that owns the decision.[^1]
- **Data in the wrong module:** [[move-field|Move Field]].[^1]
- **Many functions on the same data:** [[combine-functions-into-class|Combine Functions into Class]] or [[combine-functions-into-transform|Combine Functions into Transform]].[^1]
- **A sequence of steps repeated in many places:** [[split-phase|Split Phase]].[^1]
- **Logic split too thin:** [[inline-function|Inline Function]] and [[inline-class|Inline Class]], then split again along the right line.[^1]
- **A list of cases repeated in many functions:** keep the cases in one table or one type, as in the example.

## Variants and related patterns

**Divergent Change.** The opposite smell. See [[divergent-change|Divergent Change]].

**Duplicated Code.** Copies of the same logic make each change into many edits. See [[duplicated-code|Duplicated Code]].

**Repeated Switches.** The same `switch` in many functions is a common cause of this smell. See [[repeated-switches|Repeated Switches]].

**Feature Envy.** A function that lives far from the data it uses is one of the pieces that a change must find. See [[feature-envy|Feature Envy]].

## References
