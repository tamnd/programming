---
tag: FEVNNM
kind: technique
slug: substitute-algorithm
name: Substitute algorithm
aliases: [replace algorithm]
area: construction-practice/refactoring-encapsulation
level: core
summary: >-
  Replace the body of a function with a clearer algorithm that gives the same results, and use the old body as the test oracle while you switch.
family: refactoring
polarity: positive
intent: "Swap a complicated way of computing a result for a clearer one that computes the same result."
also_known_as: []
history:
  year: 1999
  people: [Martin Fowler]
  note: Substitute Algorithm is in the first edition of Refactoring (1999), with the refactorings that compose methods, and in the chapter on encapsulation of the second edition (2018).
see_also: [extract-function, inline-function, code-smell]
sources:
  - url: https://refactoring.com/catalog/substituteAlgorithm.html
    title: Martin Fowler, Catalog of Refactorings, Substitute Algorithm
    locator: the entry
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 6, Composing Methods, Substitute Algorithm
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 7, Encapsulation, Substitute Algorithm, motivation and mechanics
---

## Purpose

Most refactorings change the structure around code and keep its steps. Substitute algorithm changes the steps. You find a clearer way to compute the same result, often one that a library already gives, and you put it in place of the old body.

The catalogue card shows a loop that tests each person against three names in three `if` statements. It becomes a list of candidates and one call to `find` with `includes`.[^1] The first edition puts the refactoring with the ones that compose methods; the second puts it in the chapter on encapsulation.[^2][^3]

The step is larger than most refactorings, so the method is careful about it. The old body stays until the replacement gives the same answers.

## Steps

1. Arrange the code you want to replace so that it fills one whole function. Use [[extract-function|Extract Function]] if it does not.[^3]
2. Write tests for that function only. If the behavior is not fully known, write tests that record what it does now.
3. Write the replacement algorithm.
4. Run the tests. Also compare the old and the replacement on many inputs: the old body is the oracle.[^3]
5. If the results differ, use the old body to find the inputs where they differ, and fix the replacement.[^3]
6. When all results agree, delete the old body.

## Example

A leap-year rule written as a chain of tests is replaced by a call to the date library. The comparison runs both on 2001 years before the old body goes.

```javascript run id=same-answers
import assert from "node:assert/strict";

// Old body: the rule written out by hand.
function isLeapOld(y) {
  if (y % 400 === 0) return true;
  if (y % 100 === 0) return false;
  if (y % 4 === 0) return true;
  return false;
}

// Replacement: ask the calendar whether February has a 29th day.
function isLeap(y) {
  const d = new Date(Date.UTC(2000, 1, 29));
  d.setUTCFullYear(y);
  return d.getUTCMonth() === 1;
}

// Step 4: the old body is the oracle.
for (let y = 1000; y <= 3000; y++) assert.equal(isLeap(y), isLeapOld(y), `year ${y}`);
assert.equal(isLeap(1900), false);
assert.equal(new Date(Date.UTC(50, 0, 1)).getUTCFullYear(), 1950); // the 0 to 99 rule
assert.equal(isLeap(2000), true);
console.log("substitute algorithm: tests pass");
```

```output
substitute algorithm: tests pass
```

`setUTCFullYear` is used because `Date.UTC` treats a year from 0 to 99 as 1900 to 1999. A detail like that is why step 4 compares many inputs and not one or two.

| Language | What the example shows |
|---|---|
| JavaScript | a hand-written rule replaced by a date-library check, compared on 2001 years |
| Python | the same rule replaced by `calendar.isleap`, compared on the same range |
| Go | the same rule replaced by a check with `time.Date`, compared on the same range |

## When to use

- **A clearer way exists.** The code computes something in a long way, and a shorter or more direct way gives the same result.
- **A library does it.** Hand-written code repeats what the standard library already does.
- **Before a change.** The current algorithm makes a change you need hard, and a different algorithm leaves room for it.

## Pitfalls

**A large step.** Substituting a whole algorithm is a bigger step than most refactorings. Decompose the function first, so that the part you replace is as small as possible.[^3]

**Same answers on the tests, not on all inputs.** The replacement can agree on every test case and differ on inputs that the tests do not cover, as the year 0 to 99 detail above shows. Compare the old and the replacement on a wide range, or with property-based tests.

**Different edge behavior.** A library call can treat errors, empty input, rounding or time zones in its own way. Check those cases on purpose.

**Performance.** A clearer algorithm can be slower or use more memory. If speed matters, measure both.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. The tools that help are the tests: the existing suite, and a comparison of the old and the replacement on many inputs.

## References
