---
tag: 7XT0ED
kind: pattern
slug: loops-smell
name: Loops
aliases: [loop smell]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: a loop mixes the choice of elements, the change to each one and the collection of the result, where a pipeline of named operations would show each step on its own.
family: code-smell
polarity: negative
intent: "Find the loops that a reader must trace to see what they compute, and replace them with a pipeline of operations such as filter and map."
also_known_as: []
refactorings: [replace-loop-with-pipeline, split-loop]
history:
  year: 2018
  people: [Kent Beck, Martin Fowler]
  note: Loops is one of the four smells that the second edition of Refactoring (2018) adds to the list of the first edition.
see_also: [code-smell, higher-order-functions, long-function, mutable-data]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Loops
  - url: https://martinfowler.com/articles/refactoring-pipelines.html
    title: Martin Fowler, Refactoring with Loops and Collection Pipelines
    locator: the introduction and Refactoring a simple loop into a pipeline
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, the list of 22 smells, which has no entry for loops
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Replace Loop with Pipeline and Split Loop
  - url: https://martinfowler.com/articles/collection-pipeline/
    title: Martin Fowler, Collection Pipeline
    locator: the introduction
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
    title: Java SE 25 API, package java.util.stream
    locator: the package description and Side-effects
  - url: https://docs.python.org/3.14/tutorial/datastructures.html#list-comprehensions
    title: The Python Tutorial, Data Structures, Python 3.14
    locator: 5.1.3 List Comprehensions
  - url: https://pkg.go.dev/slices
    title: Go standard library, package slices
    locator: func Compact, func DeleteFunc and func Sort
---

## Intent

Find the loops that hide what they compute, and replace them with a pipeline: a chain of operations, each with a name, such as filter, map and reduce. A reader then sees which elements are used and what is done with each one, step by step.

## Problem

The loop is one of the first control structures that a programmer learns, and it can express any processing of a collection. That is also its problem. A loop body can choose elements, change them, collect them and stop early, all in the same lines, and the reader must trace the body to learn which of these it does.

*Refactoring* says that languages now give a better choice: with first-class functions, the steps of a loop can be written as pipeline operations, and filter and map show at a glance which elements are processed and what happens to them.[^1] In an article on the same refactoring, Fowler writes that "The loop is the classic way of processing collections, but with the greater adoption of first-class functions in programming languages the collection pipeline is an appealing alternative."[^2]

The forces are these:

- A loop that builds a result changes a variable on each pass: a list grows, a total goes up, a flag is set. The reader must follow the changes to know the final value.
- One loop often does several jobs, such as a total and a count, because the data is already being walked. The jobs are then hard to change one at a time.
- Each language has its own forms. JavaScript has array methods, Python has comprehensions, Java has streams, and Go's standard library leaves most such steps to the loop.

The first edition of *Refactoring* has no smell for loops.[^3] The second edition adds it.[^1]

## Solution

*Refactoring* names one refactoring: [[replace-loop-with-pipeline|Replace Loop with Pipeline]].[^1] The catalogue at refactoring.com has its card, and the card for [[split-loop|Split Loop]], which separates a loop that does two jobs into two loops.[^4]

Fowler's article gives the steps.[^2]

1. Make a variable for the collection that the loop walks. It is the start of the pipeline.
2. Take each part of the loop body in turn. A condition that skips elements becomes a filter. A line that computes a value from the element becomes a map.
3. When the body only adds each element to the result, remove the loop and return the pipeline.

If the loop does two jobs, use Split Loop first, so that each loop becomes one pipeline.

Fowler calls the result a pattern: "The collection pipeline is one of the most common, and pleasing, patterns in software."[^5]

## Example

The function finds the email addresses to notify: the customers in one country whose orders are paid. Each address appears once, in lower case, in sorted order. The loop does four things in one body: it skips orders, picks a field, changes its case and removes duplicates.

```javascript run id=loop-to-pipeline
import assert from "node:assert/strict";

const orders = [
  { email: "Ana@example.com", country: "PT", paid: true },
  { email: "bo@example.com", country: "SE", paid: true },
  { email: "ana@example.com", country: "PT", paid: true },
  { email: "cy@example.com", country: "PT", paid: false },
  { email: "Dee@example.com", country: "PT", paid: true },
];

// Before: one loop body chooses, changes and collects.
function emailsBefore(orders, country) {
  const result = [];
  for (const o of orders) {
    if (o.country === country) {
      if (o.paid) {
        const email = o.email.toLowerCase();
        if (!result.includes(email)) result.push(email);
      }
    }
  }
  result.sort();
  return result;
}

// After: each step is one named operation.
const emails = (orders, country) => [
  ...new Set(
    orders
      .filter((o) => o.country === country && o.paid)
      .map((o) => o.email.toLowerCase()),
  ),
].toSorted();

const want = ["ana@example.com", "dee@example.com"];
assert.deepEqual(emailsBefore(orders, "PT"), want);
assert.deepEqual(emails(orders, "PT"), want);
assert.deepEqual(emails(orders, "FR"), []);
console.log("emails: tests pass");
```

```output
emails: tests pass
```

The pipeline reads in the order of the steps: keep the paid orders from the country, take the address in lower case, drop the duplicates, sort. The first version also checks `result.includes` on each pass, which walks the whole result each time; the `Set` in the second version does the same job without the walk.

| Language | What the example shows |
|---|---|
| JavaScript | `filter`, `map`, a `Set` and `toSorted`, as above |
| Python | a set comprehension and `sorted` |
| Java | a stream with `filter`, `map`, `distinct`, `sorted` and `toList` |
| Go | a loop that only chooses and collects, then `slices.Sort` and `slices.Compact` for the rest |

## Consequences

Each step of the pipeline has one job and a name, so a change to one step does not touch the others. A filter that is added is one line. The pipeline does not change its input: the Java documentation states that "An operation on a stream produces a result, but does not modify its source."[^6]

A pipeline is not always clearer. A loop that stops at the first match, that updates several results at once, or that depends on the element before it can be harder to read as a chain of operations. Java warns that side effects in the functions passed to stream operations "are, in general, discouraged".[^6] A loop is the right form for work that is mostly side effects, such as writing each element to a file.

Languages differ:

- In Python, the tutorial says that "List comprehensions provide a concise way to create lists", and a comprehension is the usual form of a filter and a map.[^7]
- In Java, the stream package exists "to support functional-style operations on streams of elements, such as map-reduce transformations on collections."[^6]
- In Go, the `slices` package gives `Sort` and `Compact`, which "replaces consecutive runs of equal elements with a single copy".[^8] Its nearest form of a filter is `DeleteFunc`, which "modifies s in place by removing any elements for which del returns true".[^8] It has no map, and a short loop that does one job is the usual Go form.

The smell is a loop that does many things at once, not the keyword `for`.

## How to fix it

- **A loop that builds a collection:** [[replace-loop-with-pipeline|Replace Loop with Pipeline]].[^1][^2]
- **A loop that computes two results:** [[split-loop|Split Loop]], then a pipeline for each.[^4]
- **A loop in a language with no pipeline operations:** keep the loop, give it one job, and move it into a function with a name. See [[extract-function|Extract Function]].
- **A loop that stops at the first match:** a find operation, such as `find` in JavaScript or `findFirst` in Java, or a loop with an early return.

## Variants and related patterns

**Long Function.** A long loop body is a common part of a long function. See [[long-function|Long Function]].

**Mutable Data.** A loop that builds a result changes it on each pass. A pipeline returns a value in place of the changes. See [[mutable-data|Mutable Data]].

**Higher-order functions.** The functions such as `filter` and `map` that take a function as an argument. See [[higher-order-functions]].

## References
