---
tag: A6QN08
kind: technique
slug: introduce-assertion
name: Introduce assertion
aliases: []
area: construction-practice/refactoring-data-and-conditionals
level: core
summary: >-
  When code works only if some condition is true, state that condition with an assertion, so that readers see it and a programming error fails at once; not for checking input from outside.
family: refactoring
polarity: positive
intent: "Make an assumption of the code explicit, and make it fail loudly when a programmer breaks it."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Introduce Assertion is in the first edition of Refactoring (1999), in the chapter on simplifying conditional expressions, and in the chapter on simplifying conditional logic of the second edition (2018).
see_also: [assertions, exception-handling, replace-derived-variable-with-query, split-variable]
sources:
  - url: https://refactoring.com/catalog/introduceAssertion.html
    title: Martin Fowler, Catalog of Refactorings, Introduce Assertion
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 9, Simplifying Conditional Expressions, Introduce Assertion
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 10, Simplifying Conditional Logic, Introduce Assertion, motivation and mechanics
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: The Python Language Reference, Simple statements
    locator: 7.3 The assert statement
  - url: https://docs.oracle.com/javase/specs/jls/se21/html/jls-14.html
    title: The Java Language Specification, Java SE 21 Edition, Chapter 14
    locator: 14.10 The assert Statement
  - url: https://go.dev/doc/faq
    title: The Go Programming Language, Frequently Asked Questions
    locator: Why does Go not have assertions?
---

## Purpose

Code often works only when some condition holds: a rate is not negative, a list is sorted, a value is in range. If the condition is not written down, a reader must find it from the code, and a caller that breaks it gets a wrong result, not an error.

Introduce assertion writes the condition as an assertion. The catalogue card adds an assertion that a discount rate is not negative before the line that uses it.[^1] The first edition has the refactoring under the same name.[^2] The book gives two reasons: the assertion tells the reader what the code assumes, and it helps debugging, because a broken assumption fails where it is broken.[^3]

An assertion is for a condition that must always be true if the program is correct. The book's test: the program must behave the same if all the assertions are removed.[^3] So assertions are not a way to check data from users, files or other systems. For those, use ordinary checks that report an error. See [[exception-handling|exception handling]], and [[assertions|assertions]] for the language feature.

## Steps

1. When you see that the code assumes a condition is true, add an assertion that states it.[^3]
2. Test.

Because an assertion does not change what a correct program does, adding one is a refactoring: a test that passed before still passes.

## Example

A refund function assumes that the days used are between zero and the days paid for. A caller with a bug passes more days used than paid for, and the function returns a negative refund. With the assertion, the bug fails at once, with a message that names the broken assumption.

```javascript run id=say-the-assumption
import assert from "node:assert/strict";

function refundCents(paidCents, daysPaid, daysUsed) {
  assert.ok(daysUsed >= 0 && daysUsed <= daysPaid, "days used must be between 0 and days paid");
  return Math.round((paidCents * (daysPaid - daysUsed)) / daysPaid);
}

assert.equal(refundCents(3000, 30, 10), 2000);
assert.equal(refundCents(3000, 30, 30), 0);
assert.throws(() => refundCents(3000, 30, 31), /between 0 and days paid/);
console.log("introduce assertion: tests pass");
```

```output
introduce assertion: tests pass
```

Without the assertion, `refundCents(3000, 30, 31)` returns `-100`, and the error shows up much later, as a strange charge.

| Language | What the example shows |
|---|---|
| JavaScript | an assertion with `node:assert`, as above |
| Python | the `assert` statement, and the fact that `python -O` removes it, which is why it must not check user input |
| Java | the `assert` statement, which runs only when assertions are enabled, and a test that shows whether they are |
| Go | Go has no assertions; the example states the assumption with a check that panics, as a programming error |

## When to use

- **The code assumes something.** A condition that a reader would need to know to use the code correctly.[^3]
- **A comment states an assumption.** A comment that says that a rate is never negative can become an assertion that the code checks.
- **After a refactoring that relies on a condition.** In [[replace-derived-variable-with-query|Replace Derived Variable with Query]], an assertion checks that a stored value and a computed value agree before the stored one goes.

## Pitfalls

**Assertions can be turned off.** In Python, "The current code generator emits no code for an assert statement when optimization is requested at compile time."[^4] In Java, an assert statement is enabled "if and only if the host system has determined that the top level class or interface that lexically contains the assert statement enables assertions".[^5] Code that must run, and checks of outside data, must not be in an assertion.

**Side effects in the condition.** If the asserted expression changes state, the program behaves differently when assertions are off. Keep the condition free of side effects.

**Too many assertions.** An assertion for each line hides the ones that matter. Assert only a condition that must be true for the code to work.[^3]

**Go.** The Go FAQ explains that Go has no assertions, because "programmers use them as a crutch to avoid thinking about proper error handling and reporting".[^6] In Go, return an error for conditions that can happen, and panic only for a programming error.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. See [[assertions|assertions]] for the assertion features of each language, and for contracts.

## References
