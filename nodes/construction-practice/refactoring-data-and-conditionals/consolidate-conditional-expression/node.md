---
tag: BZ9C79
kind: technique
slug: consolidate-conditional-expression
name: Consolidate conditional expression
aliases: [consolidate conditional, combine conditions, merge conditionals]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When several checks in a row give the same result, combine them into one condition with or and and, and extract it into a function whose name says the single rule that the checks make together.
family: refactoring
polarity: positive
intent: "Join checks with the same result into one named condition."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Consolidate Conditional Expression is in the first edition of Refactoring (1999), in the chapter on simplifying conditional expressions, and has the same name in the second edition (2018).
see_also: [decompose-conditional, extract-function, replace-nested-conditional-with-guard-clauses, conditional, duplicated-code]
sources:
  - url: https://refactoring.com/catalog/consolidateConditionalExpression.html
    title: Martin Fowler, Catalog of Refactorings, Consolidate Conditional Expression
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Consolidate Conditional Expression
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Consolidate Conditional Expression, motivation, mechanics and the examples with or and with and
---

## Purpose

Sometimes a function makes several checks in a row, and each one gives the same result. The checks look like separate rules, but they are one rule: if any of them is true, the answer is the same. Written apart, they hide that rule.

Consolidate conditional expression joins the checks into one condition, and then extracts the condition into a function. The catalogue card joins three checks that each return 0, on the seniority, the months of disability and the part-time status of an employee, into one function, `isNotEligibleForDisability()`.[^1] The refactoring is in both editions of the book.[^2][^3]

## Steps

1. Make sure that none of the conditions has a side effect. If one has, first separate the query from the modifier.[^3]
2. Take two of the conditions and join them with a logical operator. Checks in a row that give the same result join with or. A check inside another check joins with and.[^3]
3. Test.
4. Repeat until there is one condition.
5. Extract the condition into a function with a name that says the rule. See [[extract-function|Extract Function]].

## Example

A shop decides if an order can ship on the day it is placed. Three checks return false. After the refactoring, the three checks are one function, and the rule has a name.

```javascript run id=one-rule
import assert from "node:assert/strict";

function shipsTodayBefore(order, hour) {
  if (hour >= 15) return false;
  if (order.items.some((i) => i.backordered)) return false;
  if (order.address.country !== "NO") return false;
  return true;
}

function shipsToday(order, hour) {
  return !missesTodaysDispatch(order, hour);
}

function missesTodaysDispatch(order, hour) {
  return hour >= 15 || order.items.some((i) => i.backordered) || order.address.country !== "NO";
}

const items = [[], [{ backordered: false }], [{ backordered: false }, { backordered: true }]];
for (const hour of [9, 14, 15, 20]) for (const it of items) for (const country of ["NO", "SE"]) {
  const order = { items: it, address: { country } };
  assert.equal(shipsToday(order, hour), shipsTodayBefore(order, hour));
}
console.log("consolidate conditional expression: tests pass");
```

```output
consolidate conditional expression: tests pass
```

`||` evaluates from left to right and stops at the first true operand, so the consolidated condition does the same checks, in the same order, as the three `if` statements.

| Language | What the example shows |
|---|---|
| JavaScript | three checks in a row joined with or, as above |
| Python | the same join with `or`, and a nested check joined with `and` |
| Go | the same join with `||` in a method |

## When to use

- **Checks in a row with the same result.** The case that the card shows.[^1]
- **Nested ifs with no else.** An if inside an if, where only the inner body does something, is one condition joined with and.[^3]
- **Before Replace Nested Conditional with Guard Clauses.** A function with fewer, named conditions is easier to turn into guard clauses. See [[replace-nested-conditional-with-guard-clauses|Replace Nested Conditional with Guard Clauses]].

## Pitfalls

**Checks that are separate rules.** If the checks give the same result by chance, and can change for different reasons, they are not one rule. Keep them apart, so that a change to one does not change the others.

**Side effects.** A condition that changes state, or that writes to a log, runs in a different number of cases when it is joined with or and and, because of short-circuit evaluation. Take the side effect out first.[^3]

**Long conditions.** A condition with many operators and negations is hard to read even when it has one name. Decompose it into named parts. See [[decompose-conditional|Decompose Conditional]].

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
