---
tag: WW9AQW
kind: technique
slug: slide-statements
name: Slide statements
aliases: [consolidate duplicate conditional fragments]
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Move statements up or down so that code that works on the same thing is together, after checking that the move does not change what the code does; a usual first step before Extract Function.
family: refactoring
polarity: positive
intent: "Put related statements next to each other so that the code reads in one piece and can be extracted."
also_known_as: [Consolidate Duplicate Conditional Fragments]
history:
  year: 2018
  people: [Martin Fowler]
  note: The second edition of Refactoring (2018) has Slide Statements in the chapter on moving features. The catalogue gives Consolidate Duplicate Conditional Fragments, a first-edition refactoring (1999) that moves the same code out of each branch of a conditional, as an alias.
see_also: [extract-function, split-loop, move-statements-into-function, move-statements-to-callers, side-effect]
sources:
  - url: https://refactoring.com/catalog/slideStatements.html
    title: Martin Fowler, Catalog of Refactorings, Slide Statements
    locator: the entry; aliases Consolidate Duplicate Conditional Fragments
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Slide Statements, motivation, mechanics and the example with conditionals
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Consolidate Duplicate Conditional Fragments
---

## Purpose

Code is easier to read when statements that work on the same data sit together. A variable declared at the top of a function and first used thirty lines down makes the reader hold it in mind for those thirty lines. Two lines that belong together, with unrelated lines between them, cannot be extracted as one function.

Slide statements moves statements up or down to fix that. The catalogue card moves the line that reads the unit charge of a pricing plan up, next to the line that gets the plan.[^1] The book uses the move most often as a step before [[extract-function|Extract Function]], and to declare a variable right before its first use.[^2]

The catalogue lists Consolidate Duplicate Conditional Fragments as an alias.[^1] That first-edition refactoring moves the same statement out of every branch of a conditional, to before or after it.[^3] In the second edition this is one case of sliding: slide the copy in each branch to the edge of the branch, and then out.[^2]

## Steps

1. Find the place to move the statements to.
2. Look at each statement between the old place and the target place, and check that the move does not change what the code does.[^2] The rules:
   - Do not slide a statement above the declaration of anything it uses.
   - Do not slide a statement below anything that uses what it declares or changes.
   - Do not slide a statement past another statement that changes something it uses.
   - Do not slide a statement that changes something past another statement that uses that thing.
3. Cut the statements from the old place and paste them in the target place.
4. Test.

If a test fails, the move crossed a dependency. Go back, and try a smaller slide.

## Example

A function to price a booking mixes the lines for the base price with the lines for the discount. After sliding, the discount lines sit together and are ready for [[extract-function|Extract Function]]. The conditional also had the same line at the end of both branches; that line slides out, below the `if`.

```javascript run id=keep-together
import assert from "node:assert/strict";

function priceBefore(b) {
  const base = b.nights * b.rateCents;
  const loyal = b.stays >= 5;
  const tax = Math.round(base * 0.1);
  let discount = 0;
  let total;
  if (loyal) { discount = Math.round(base * 0.05); total = base + tax - discount; }
  else { total = base + tax - discount; }
  return total;
}

function priceAfter(b) {
  const base = b.nights * b.rateCents;
  const tax = Math.round(base * 0.1);

  const loyal = b.stays >= 5;           // slid down, next to its use
  let discount = 0;
  if (loyal) discount = Math.round(base * 0.05);

  return base + tax - discount;         // the duplicate line slid out of both branches
}

for (const b of [{ nights: 3, rateCents: 9000, stays: 1 }, { nights: 2, rateCents: 12000, stays: 7 }]) {
  assert.equal(priceAfter(b), priceBefore(b));
}
assert.equal(priceAfter({ nights: 2, rateCents: 12000, stays: 7 }), 25200);
console.log("slide statements: tests pass");
```

```output
slide statements: tests pass
```

Each slide was legal: the `loyal` line uses only `b`, which is declared above, and the line that computes the total ran last in both branches, so moving it after the `if` does not change the order of anything it depends on.

| Language | What the example shows |
|---|---|
| JavaScript | a declaration slid next to its use, and a duplicate line slid out of both branches, as above |
| Python | a slide that is not legal, because a statement in between changes a value the moved line reads, and the test that catches it |
| Go | the same slides as above, ready for an extracted function |

## When to use

- **Before Extract Function.** Bring the lines you want to extract together.[^2]
- **Declaration far from use.** Move a declaration next to its first use.
- **The same statement in each branch.** Slide it out of the conditional, the old Consolidate Duplicate Conditional Fragments.[^3]
- **Before Split Loop.** Related work is easier to split when it sits together. See [[split-loop|Split Loop]].

## Pitfalls

**Side effects.** A function call between the old and the target place can change state that the moved line reads, or read state that it writes. If you cannot see what a call does, you cannot be sure the slide is safe. See [[side-effect|side effect]].

**Long slides.** A long slide passes many statements and has more chances to cross a dependency. Make many short slides, each with a test.

**Tests that do not reach.** If no test covers the path through the moved lines, a broken slide passes the tests. Check coverage before a slide over a branch.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The check of step 2 is yours, and so is the test of step 4.

## References
