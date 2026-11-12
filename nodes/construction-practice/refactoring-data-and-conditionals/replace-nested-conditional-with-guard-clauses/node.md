---
tag: GD13VY
kind: technique
slug: replace-nested-conditional-with-guard-clauses
name: Replace nested conditional with guard clauses
aliases: [guard clause, guard clauses, early return, return early]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When a function nests its special cases in if and else, check each special case at the top and return at once, so that the normal case is the unindented code at the end of the function.
family: refactoring
polarity: positive
intent: "Check each special case first and return, so that the normal path is not nested."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Replace Nested Conditional with Guard Clauses is in the first edition of Refactoring (1999), in the chapter on simplifying conditional expressions, and has the same name in the second edition (2018).
see_also: [decompose-conditional, consolidate-conditional-expression, conditional, assertions, long-function]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Replace Nested Conditional with Guard Clauses, motivation, mechanics and the example of reversing the conditions
  - url: https://refactoring.com/catalog/replaceNestedConditionalWithGuardClauses.html
    title: Martin Fowler, Catalog of Refactorings, Replace Nested Conditional with Guard Clauses
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Replace Nested Conditional with Guard Clauses
---

## Purpose

A conditional can have two shapes. In one, both branches are normal behavior, and if and else show that they are equal. In the other, one branch is the normal case and the others are special cases. When the special cases nest the normal case in if and else, the reader must go down several levels to find the main work of the function.[^1]

A guard clause checks one special case and returns at once. Replace nested conditional with guard clauses moves each special case to the top of the function as a guard clause. The catalogue card turns a pay calculation with three nested if and else statements, for an employee who is dead, separated or retired, into three guard clauses followed by the normal amount.[^2] The book also rejects the rule that a function must have only one exit point when the rule makes the code less clear.[^3][^1]

## Steps

1. Take the outermost condition that is a special case, and change it into a guard clause: check it and return.[^1]
2. Test.
3. Repeat for each special case.
4. If all the guard clauses return the same result, consider joining them. See [[consolidate-conditional-expression|Consolidate Conditional Expression]].
5. If a condition is a guard only when it is reversed, reverse it: `if (x > 0) { ... }` with all the work inside becomes `if (x <= 0) return ...;` followed by the work.[^1]

## Example

A function computes the refund for a ticket. The normal case, a refund less a fee, is three levels deep. After the refactoring, each special case is one line at the top.

```javascript run id=special-cases-first
import assert from "node:assert/strict";

function refundBefore(ticket, now) {
  let result;
  if (ticket.cancelledByOrganizer) {
    result = ticket.priceCents;
  } else {
    if (ticket.used) {
      result = 0;
    } else {
      if (now > ticket.eventStart) {
        result = 0;
      } else {
        result = Math.max(0, ticket.priceCents - 250);
      }
    }
  }
  return result;
}

function refund(ticket, now) {
  if (ticket.cancelledByOrganizer) return ticket.priceCents;
  if (ticket.used) return 0;
  if (now > ticket.eventStart) return 0;
  return Math.max(0, ticket.priceCents - 250);
}

for (const cancelledByOrganizer of [false, true]) for (const used of [false, true])
  for (const now of [50, 150]) for (const priceCents of [100, 4000]) {
    const t = { cancelledByOrganizer, used, eventStart: 100, priceCents };
    assert.equal(refund(t, now), refundBefore(t, now));
  }
console.log("guard clauses: tests pass");
```

```output
guard clauses: tests pass
```

The test runs both versions for each combination of the three flags. The two guard clauses that return 0 now stand next to each other, and could be joined into one named condition.

| Language | What the example shows |
|---|---|
| JavaScript | three guard clauses in place of nested if and else, as above |
| Python | the same change, and a reversed condition turned into a guard clause |
| Go | guard clauses for error cases: each error check returns, and the normal path continues without indentation |

## When to use

- **Special cases around a normal case.** A check for invalid input, a missing value, or a case with a fixed answer.
- **A function that is hard to read because of its indentation.** Each guard clause removes one level of nesting from the normal path.
- **A single-exit rule that adds nesting.** A `result` variable that is set in each branch and returned at the end is often the sign of this shape.

## Pitfalls

**Two normal cases.** If both branches are normal behavior, keep the if and else: their equal shape tells the reader that they are equal.

**Clean-up code.** In a language without `finally`, `defer` or destructors, an early return can skip the code that releases a resource. Use the language's construct for clean-up before the function returns early.

**Guards in the middle.** A reader can miss a return deep inside a long function. Guard clauses belong at the top. A long function with returns in many places is a sign to extract functions first.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
