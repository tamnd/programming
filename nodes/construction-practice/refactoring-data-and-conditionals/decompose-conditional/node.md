---
tag: XXKNP7
kind: technique
slug: decompose-conditional
name: Decompose conditional
aliases: [decompose conditional, extract condition]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When an if statement has a long condition and long branches, extract the condition and each branch into a function with a name that says what it means, so that the if statement reads as the rule.
family: refactoring
polarity: positive
intent: "Name the condition and each branch of a complex conditional."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Decompose Conditional is in the first edition of Refactoring (1999), in the chapter on simplifying conditional expressions, and has the same name in the second edition (2018).
see_also: [extract-function, consolidate-conditional-expression, conditional, long-function, replace-nested-conditional-with-guard-clauses]
sources:
  - url: https://refactoring.com/catalog/decomposeConditional.html
    title: Martin Fowler, Catalog of Refactorings, Decompose Conditional
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Decompose Conditional
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Decompose Conditional, motivation and mechanics
---

## Purpose

A conditional with a long condition and long branches says how the code decides, but not why. A reader sees two date comparisons and two formulas, and must work out that the code charges a summer rate in summer.

Decompose conditional is [[extract-function|Extract Function]] applied to the parts of a conditional. The condition becomes a function, and each branch becomes a function. The catalogue card turns a test of a date against the start and end of summer into `summer()`, and the two formulas into `summerCharge()` and `regularCharge()`.[^1] The refactoring is in both editions of the book.[^2][^3]

## Steps

1. Extract the condition into a function whose name says what the condition means.[^3]
2. Extract the then branch into a function.
3. Extract each else branch into a function.
4. Test after each extraction.
5. If the result is an if statement whose branches only assign one variable, consider a conditional expression, such as the ternary operator.

## Example

A library charges a late fee. The fee is lower for a member under 18 and is not charged in the grace period. After the refactoring, the if statement says the rule, and each function says one part of it.

```javascript run id=name-the-parts
import assert from "node:assert/strict";

function lateFeeBefore(loan, now) {
  const daysLate = Math.floor((now - loan.due) / 86400000);
  let fee;
  if (daysLate > 3 && !(loan.member.age < 18 || loan.member.tier === "staff")) {
    fee = Math.min(daysLate * 25, loan.item.priceCents);
  } else {
    fee = daysLate > 3 ? Math.min(daysLate * 10, 500) : 0;
  }
  return fee;
}

const DAY_MS = 86400000;
const GRACE_DAYS = 3;

function lateFee(loan, now) {
  const daysLate = Math.floor((now - loan.due) / DAY_MS);
  if (daysLate <= GRACE_DAYS) return 0;
  return paysFullFee(loan.member) ? fullFee(daysLate, loan.item) : reducedFee(daysLate);
}

function paysFullFee(member) {
  return !(member.age < 18 || member.tier === "staff");
}
function fullFee(daysLate, item) {
  return Math.min(daysLate * 25, item.priceCents);
}
function reducedFee(daysLate) {
  return Math.min(daysLate * 10, 500);
}

const due = Date.UTC(2026, 0, 1);
const item = { priceCents: 1999 };
for (const age of [12, 40]) for (const tier of ["staff", "public"]) for (const d of [0, 3, 4, 30, 200]) {
  const loan = { due, item, member: { age, tier } };
  assert.equal(lateFee(loan, due + d * DAY_MS), lateFeeBefore(loan, due + d * DAY_MS));
}
console.log("decompose conditional: tests pass");
```

```output
decompose conditional: tests pass
```

The test runs the two versions for each kind of member and for days in and after the grace period. The step that took the grace period out of the condition is a guard clause; see [[replace-nested-conditional-with-guard-clauses|Replace Nested Conditional with Guard Clauses]].

| Language | What the example shows |
|---|---|
| JavaScript | the condition and the two branches as functions, as above |
| Python | the same decomposition with a dataclass for the loan |
| Go | the same decomposition with methods on the member type |

## When to use

- **A condition that needs a comment.** If a reader needs a comment to know what a condition tests, the comment is the name of a function.
- **Branches that do different things at length.** A branch of many lines is a function with no name.
- **A long function.** A long conditional is a frequent part of a [[long-function|long function]].

## Pitfalls

**Names that repeat the code.** A function named `isAgeUnder18OrStaff` says no more than the expression. Name the rule: `paysFullFee`.

**Changed evaluation.** An extracted condition is still evaluated once, in the same place. If a part of the condition has a side effect, or is expensive, keep the order and the number of calls the same.

**Too many small functions.** For a condition that is already clear, a function adds a name to look up. Decompose a conditional when the parts have a meaning that the code does not show.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. Each step is an Extract Function, and editors that can extract a function can do each step.

## References
