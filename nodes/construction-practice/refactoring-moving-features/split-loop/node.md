---
tag: 49M2Q0
kind: technique
slug: split-loop
name: Split loop
aliases: []
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Split a loop that computes two things into two loops that compute one each, so that each can be understood, extracted and changed on its own; measure before you worry about the cost.
family: refactoring
polarity: positive
intent: "Make each loop do one job."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Split Loop is in the chapter on moving features of the second edition of Refactoring (2018).
see_also: [extract-function, replace-loop-with-pipeline, slide-statements, loops-smell, cpu-profiling]
sources:
  - url: https://refactoring.com/catalog/splitLoop.html
    title: Martin Fowler, Catalog of Refactorings, Split Loop
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Split Loop, motivation and mechanics; Chapter 2, Principles in Refactoring, Refactoring and Performance
---

## Purpose

A loop that does two things at once is common, because one pass over the data feels cheaper. The cost is in reading and changing it: to change one of the two computations, you must understand both, and you cannot extract one into a function without the other.

Split loop copies the loop and keeps one computation in each copy. The catalogue card splits a loop over people that sums ages and salaries into one loop for the salaries and one for the ages.[^1] After the split, each loop can become a function, and often a pipeline: see [[replace-loop-with-pipeline|Replace Loop with Pipeline]].

The obvious worry is speed: two passes in place of one. The book's answer is to keep refactoring and optimizing apart. A loop is rarely the bottleneck; if a profile shows that it is, the loops can be joined again, and the clearer code makes other optimizations easier to find.[^2]

## Steps

1. Copy the loop.[^2]
2. Find the side effects that are now done twice, and remove each from the copy where it does not belong.
3. Test.
4. Consider [[extract-function|Extract Function]] on each loop, and [[slide-statements|Slide Statements]] to bring each loop's variables next to it.

## Example

One loop over orders sums the revenue and finds the largest order. After the split, each computation is a loop of its own, and then a function.

```javascript run id=one-job
import assert from "node:assert/strict";

const orders = [{ id: 1, cents: 1200 }, { id: 2, cents: 5400 }, { id: 3, cents: 800 }];

// Before: one loop, two jobs.
function summaryBefore(orders) {
  let revenue = 0;
  let largest = null;
  for (const o of orders) {
    revenue += o.cents;
    if (largest === null || o.cents > largest.cents) largest = o;
  }
  return { revenue, largestId: largest?.id };
}

// After: one loop for each job, each extracted.
function revenueOf(orders) {
  let revenue = 0;
  for (const o of orders) revenue += o.cents;
  return revenue;
}
function largestOf(orders) {
  let largest = null;
  for (const o of orders) if (largest === null || o.cents > largest.cents) largest = o;
  return largest;
}
const summary = (orders) => ({ revenue: revenueOf(orders), largestId: largestOf(orders)?.id });

assert.deepEqual(summary(orders), summaryBefore(orders));
assert.deepEqual(summary([]), summaryBefore([]));
assert.deepEqual(summary(orders), { revenue: 7400, largestId: 2 });
console.log("split loop: tests pass");
```

```output
split loop: tests pass
```

Each function now has one job and one name. The next step can be [[replace-loop-with-pipeline|Replace Loop with Pipeline]]: `revenueOf` becomes a `reduce`.

| Language | What the example shows |
|---|---|
| JavaScript | a loop with two jobs split into two functions, as above |
| Python | the same split, where each loop then becomes a call to a built-in, `sum` and `max` |
| Go | the same split into two functions, with a test on an empty slice |

## When to use

- **A loop does two things.** Split it before you change either one.
- **Before Extract Function.** A loop with two jobs cannot be extracted as one function with one name.
- **Before Replace Loop with Pipeline.** Each loop with one job maps to one pipeline. See [[loops-smell|Loops]].

## Pitfalls

**Performance.** Two passes do more work than one. In most code, the difference does not matter; when it does, measure it with a profiler before you join the loops again. See [[cpu-profiling|CPU profiling]].[^2]

**Single-pass sources.** A loop over a stream, an iterator or a network response can only be read once. Split it only after you collect the items, or keep one loop and extract the body of each job.

**Duplicate side effects.** If the loop body writes to a file, logs or changes shared state, the copy does it twice. Step 2 removes the duplicates; check them with a test.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. After the split, see [[extract-function|Extract Function]] for the tools that extract each loop.

## References
