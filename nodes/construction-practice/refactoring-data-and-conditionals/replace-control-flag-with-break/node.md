---
tag: AA2DYV
kind: technique
slug: replace-control-flag-with-break
name: Replace control flag with break
aliases: [remove control flag, replace control flag with return]
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When a loop sets a boolean variable to tell later code to stop, use break, continue or return at that point instead, and delete the variable.
family: refactoring
polarity: positive
intent: "Stop the loop where the code knows it must stop, not through a flag."
also_known_as: [Remove Control Flag]
history:
  year: 1999
  people: [Martin Fowler]
  note: The first edition of Refactoring (1999) has Remove Control Flag in the chapter on simplifying conditional expressions. The second edition (2018) names it Replace Control Flag with Break and keeps the old name as an alias.
see_also: [structured-programming, control-flow, extract-function, replace-nested-conditional-with-guard-clauses, split-loop]
sources:
  - url: https://refactoring.com/catalog/replaceControlFlagWithBreak.html
    title: Martin Fowler, Catalog of Refactorings, Replace Control Flag with Break
    locator: the entry; alias Remove Control Flag
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Remove Control Flag
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Replace Control Flag with Break, motivation and mechanics
  - url: https://docs.python.org/3/reference/compound_stmts.html#the-for-statement
    title: The Python Language Reference, Compound statements, The for statement
    locator: 8.3 The for statement
  - url: https://go.dev/ref/spec#Break_statements
    title: The Go Programming Language Specification, Break statements
    locator: Break statements
---

## Purpose

A control flag is a variable that a loop sets so that later code does not run. The loop goes on, and each pass checks the flag and does nothing. A reader must follow the flag to see that the loop is in effect over.

Replace control flag with break stops the loop at the point where the code knows that it must stop, with `break`, `continue` or `return`, and deletes the flag. The catalogue card removes a `found` flag from a loop that looks for a name and sends an alert, and puts a `break` after the alert.[^1] The first edition named the refactoring Remove Control Flag, and the second edition gave it the name that it has in the catalogue.[^2][^3]

## Steps

1. If the loop is the main part of a function and the code after it only returns a result, consider extracting the loop into its own function, so that it can use `return`. See [[extract-function|Extract Function]].
2. For each place that sets the flag to stop, put the matching statement there: `break` to leave the loop, `continue` to go to the next pass, or `return` to leave the function.
3. Test after each change.
4. When no code reads the flag, delete it.

## Example

A function finds the first order that is over a credit limit and reports it. The flag `found` makes each later pass do nothing. After the refactoring, the loop is in its own function and returns.

```javascript run id=stop-where-you-know
import assert from "node:assert/strict";

function firstOverLimitBefore(orders, limitCents) {
  let found = false;
  let result = null;
  for (const o of orders) {
    if (!found) {
      if (o.totalCents > limitCents) {
        result = o.id;
        found = true;
      }
    }
  }
  return result;
}

function firstOverLimit(orders, limitCents) {
  for (const o of orders) {
    if (o.totalCents > limitCents) return o.id;
  }
  return null;
}

const orders = [{ id: "a", totalCents: 900 }, { id: "b", totalCents: 1500 }, { id: "c", totalCents: 2000 }];
for (const limit of [0, 1000, 1600, 5000]) assert.equal(firstOverLimit(orders, limit), firstOverLimitBefore(orders, limit));
assert.equal(firstOverLimit(orders, 1000), "b");
console.log("replace control flag with break: tests pass");
```

```output
replace control flag with break: tests pass
```

The version with `return` also stops reading the array at the first match. With the flag, the loop visits every order.

In Python, a `for` loop can have an `else` block that runs when the loop ends without `break`: "A break statement executed in the first suite terminates the loop without executing the else clause's suite."[^4] That form replaces a flag that only records whether the loop found something. In Go, a `break` can name the label of an enclosing `for`, `switch` or `select` statement, and the specification says that the labeled statement "is the one whose execution terminates".[^5]

| Language | What the example shows |
|---|---|
| JavaScript | a flag replaced by `return` in an extracted function, as above |
| Python | a flag replaced by `break`, and the `for` and `else` form, whose `else` block runs only when the loop ends without `break` |
| Go | a flag that left two nested loops, replaced by a labeled `break` |

## When to use

- **A boolean that a loop sets and then tests.** The `found`, `done` or `stop` variable that the card removes.[^1]
- **A flag that leaves nested loops.** A language with labeled break, such as Java, JavaScript or Go, can leave an outer loop in one statement. In other languages, extract the loops into a function and return.
- **Before a further refactoring.** A loop without a flag is easier to split or to replace with a pipeline. See [[split-loop|Split Loop]].

## Pitfalls

**A flag that is a result.** If code after the loop needs to know whether the loop found something, the variable is data, not only control. Return the result, or keep the variable and stop the loop with `break`.

**Clean-up after the loop.** Code after the loop that must always run, such as closing a file, still runs after `break`, but not after `return`. Use the language's construct for clean-up, such as `finally`, `with` or `defer`.

**A rule of one exit.** Some coding standards allow only one exit from a loop or function. In a team with such a standard, a change to it comes before this refactoring.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool.

## References
