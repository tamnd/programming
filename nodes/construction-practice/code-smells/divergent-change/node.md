---
tag: 3H4HKG
kind: pattern
slug: divergent-change
name: Divergent Change
aliases: [more than one reason to change]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: one module is changed for several unrelated reasons, so each change must work around code that belongs to a different concern.
family: code-smell
polarity: negative
intent: "Find the modules that change for more than one reason, and split them so that each part changes for one reason."
also_known_as: []
refactorings: [split-phase, move-function, extract-function, extract-class]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Divergent Change is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018).
see_also: [code-smell, shotgun-surgery, extract-function, long-function]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Divergent Change and Shotgun Surgery
  - url: https://en.wikipedia.org/w/index.php?title=Single-responsibility_principle&oldid=1382198334
    title: Wikipedia, Single-responsibility principle, revision 1382198334
    locator: the introduction, History and Example
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Split Phase, Move Function, Extract Function and Extract Class
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Divergent Change
---

## Intent

Find the modules that are changed for more than one reason, and split them so that each part has one reason to change. Then a change to one concern does not touch, and cannot break, the code of another.

## Problem

A module that is changed often is not a problem in itself. The smell is a module that is changed for reasons that have nothing to do with each other. *Refactoring* describes the thought that signals it: a programmer sees that one set of functions in a module must change each time a database changes, and a different set must change each time a financial product is added.[^1]

The forces are these:

- Code grows where it is first written. A report function starts with the totals and the format in one place, because both are small. Later the business asks for a new total, and a different team asks for a new format. Each change goes into the same function.
- Two concerns in one module share local variables, helper functions and tests. A change to one concern must be made with care not to break the other, and its tests must set up data for both.
- Robert C. Martin states the matching design rule as "A class should have only one reason to change". He later clarified it as "Gather together the things that change for the same reasons. Separate those things that change for different reasons."[^2] Wikipedia lists the principle as the "S" in SOLID.[^2]
- The Wikipedia article gives an example that fits the smell: a module that compiles and prints a report can change because the content changes or because the format changes, and "It would be a bad design to couple two things that change for different reasons at different times."[^2]

## Solution

Split the module along the line between the concerns. *Refactoring* gives the steps by the shape of the code.[^1] The catalogue at refactoring.com has a card for each refactoring.[^3]

1. **The two concerns run one after the other**, such as reading data and then processing it: [[split-phase|Split Phase]]. The first phase produces a data structure, and the second phase uses it. Each phase can then live in its own module.
2. **The two concerns are mixed in many functions:** [[move-function|Move Function]] to put the functions of each concern in their own module.
3. **One function does work for both concerns:** [[extract-function|Extract Function]] to split it, then move the parts.
4. **The module is a class:** [[extract-class|Extract Class]] to move one concern and its fields into a class of its own.

A good test for the line is the list of reasons to change. Write down the last few changes to the module. If they fall into two groups that never touch the same code, the groups are the two modules.

## Example

The report function adds up sales by region and formats the result as text or CSV. A change to the business rule, such as a total per month, and a change to the output, such as an HTML format, both edit this one function. The `format` checks are spread through the computation.

```javascript run id=split-phase-report
import assert from "node:assert/strict";

// Before: the computation and the format are mixed.
function reportBefore(sales, format) {
  let total = 0;
  const byRegion = new Map();
  let out = format === "csv" ? "region,amount\n" : "";
  for (const s of sales) {
    total += s.amount;
    byRegion.set(s.region, (byRegion.get(s.region) ?? 0) + s.amount);
  }
  for (const [region, amount] of byRegion) {
    out += format === "csv" ? `${region},${amount}\n` : `${region}: ${amount}\n`;
  }
  out += format === "csv" ? `total,${total}\n` : `Total: ${total}\n`;
  return out;
}

// After: phase 1 computes a summary, phase 2 renders it.
function summarize(sales) {
  const byRegion = new Map();
  for (const s of sales) byRegion.set(s.region, (byRegion.get(s.region) ?? 0) + s.amount);
  const total = [...byRegion.values()].reduce((a, b) => a + b, 0);
  return { byRegion, total };
}

const renderers = {
  text: (sum) =>
    [...sum.byRegion].map(([r, a]) => `${r}: ${a}\n`).join("") + `Total: ${sum.total}\n`,
  csv: (sum) =>
    "region,amount\n" + [...sum.byRegion].map(([r, a]) => `${r},${a}\n`).join("") + `total,${sum.total}\n`,
};

const report = (sales, format) => renderers[format](summarize(sales));

const sales = [
  { region: "north", amount: 100 },
  { region: "south", amount: 50 },
  { region: "north", amount: 25 },
];
for (const format of ["text", "csv"]) {
  assert.equal(report(sales, format), reportBefore(sales, format));
}
assert.equal(report(sales, "text"), "north: 125\nsouth: 50\nTotal: 175\n");
assert.equal(summarize(sales).total, 175);
console.log("report: tests pass");
```

```output
report: tests pass
```

After the split, a new total changes `summarize` and its tests, and no renderer. A new format adds one entry to `renderers`, and does not touch the computation. The test of `summarize` checks the numbers without parsing text.

| Language | What the example shows |
|---|---|
| JavaScript | the split phase above, with a table of renderers |
| Python | the same split, with a dataclass for the summary and a dict of renderers |
| Java | a record for the summary, and an interface for the renderers |
| Go | a struct for the summary, and a map from format name to render function |

## Consequences

Each part becomes smaller and has tests of its own. Two people can change the two parts at the same time without a merge conflict. The intermediate data structure, the summary in the example, documents the contract between the parts.

The cost is one more type and one more module. When a module has two concerns that always change together, a split adds indirection for no gain. The smell is the reasons for change, not the count of concerns in the code.

The split must follow the real reasons. A split by layer, such as all the database code in one module and all the business code in another, helps when the database and the business rules change for different reasons. If each business change also needs a database change, the layer split turns the problem into [[shotgun-surgery|Shotgun Surgery]].

## How to fix it

- **Two concerns in sequence:** [[split-phase|Split Phase]], with a data structure between the phases.[^1]
- **Two concerns spread over many functions:** [[move-function|Move Function]] for each function, to a module for its concern.[^1]
- **One function with parts of both:** [[extract-function|Extract Function]], then move the parts.[^1]
- **A class with two concerns:** [[extract-class|Extract Class]].[^1]

## Variants and related patterns

**Shotgun Surgery.** The opposite smell: one change needs edits in many modules. *Refactoring* contrasts the two: in Divergent Change one module suffers many kinds of change, and in Shotgun Surgery one kind of change touches many modules.[^1] The first edition makes the same contrast.[^4] See [[shotgun-surgery|Shotgun Surgery]].

**Large Class.** A class with many concerns is often both large and divergent. See [[large-class|Large Class]].

**Single-responsibility principle.** The design rule that this smell breaks. See [[single-responsibility-principle|Single-responsibility principle]].

**Separation of concerns.** The general idea behind the split. See [[separation-of-concerns|Separation of concerns]].

## References
