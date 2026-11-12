---
tag: TQ0M5R
kind: pattern
slug: comments
name: Comments
aliases: [comments smell, deodorant comments]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell, not the practice of writing comments: a comment explains what unclear code does, where clearer code would not need the comment.
family: code-smell
polarity: negative
intent: "Find the comments that explain what a block of code does, and change the code so that its names and structure say it, keeping the comments that say why."
also_known_as: []
refactorings: [extract-function, change-function-declaration, introduce-assertion, extract-variable]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Comments is in the first edition of Refactoring (1999), and keeps its name in the second edition (2018). The book treats comments as a sign of a smell elsewhere, not as a smell on their own.
see_also: [comment, code-smell, long-function, mysterious-name, assertions]
sources:
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Comments
  - url: https://en.wikipedia.org/w/index.php?title=Comment_(computer_programming)&oldid=1381653525
    title: Wikipedia, Comment (computer programming), revision 1381653525
    locator: the introduction, Describe intent, and Need for comments
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Extract Function, Change Function Declaration (alias Rename Function), Introduce Assertion and Extract Variable
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Comments
  - url: https://docs.python.org/3/reference/simple_stmts.html
    title: Python Language Reference, simple statements
    locator: The assert statement
  - url: https://go.dev/doc/comment
    title: Go Doc Comments
    locator: the introduction
---

## Intent

This page is about a code smell named Comments, not about the practice of writing comments; for comments as a language feature, see [[comment|Comment]]. The smell is a comment that is there because the code is hard to read. Find the comments that explain what a block does, and change the code so that a name or a check says it. Keep the comments that say why the code is as it is.

## Problem

A comment is not bad. *Refactoring* says so plainly: comments are not a bad smell, and some are a sign of care.[^1] The smell is the use of a comment as a cover. A long function with a comment above each block, or a comment such as `// x must be positive here`, is a comment that does the work of a missing function name or a missing check.

*Refactoring* puts the advice in one line: when a comment seems needed, first try to refactor the code until the comment is not needed.[^1] Wikipedia describes the same view: "The need to explain the what is a sign that it is too complex and should be re-worked."[^2] It also notes that the question is not settled, and that "Experts have varying viewpoints on whether, and when, comments are appropriate in source code."[^2]

The forces are these:

- A comment is the quickest way to explain a block of code. It costs one line and no change to the code.
- The compiler does not read the comment. When the code changes, the comment can stay as it was and become false, and no test fails.
- A comment above a block in a long function marks a function that is waiting to be extracted. The comment is already its name.
- A comment that states a condition, such as `// the list is sorted here`, is an assertion that nothing checks.
- Some comments carry what code cannot say: why a choice was made, what was tried and failed, what the author is not sure about. *Refactoring* names these as the good uses.[^1]

## Solution

*Refactoring* names a refactoring for each kind of explaining comment.[^1] The catalogue at refactoring.com has a card for each, and lists Rename Function as one of the names of Change Function Declaration.[^3]

1. **A comment that explains a block:** [[extract-function|Extract Function]], and use the comment as the name of the function.
2. **A comment that explains what a function does, when the function is already extracted:** [[change-function-declaration|Change Function Declaration]], to give the function a name that says it.
3. **A comment that explains an expression:** [[extract-variable|Extract Variable]], and use the comment as the name of the variable.
4. **A comment that states a required condition:** [[introduce-assertion|Introduce Assertion]], so that the program checks it.

The first edition gives the same moves, and the same rule for the comments that remain: a comment is a good place to say why something is done, and to mark the parts the author is unsure of.[^4]

Languages give the checking comment a form. The Python reference describes `assert` as "a convenient way to insert debugging assertions into a program", and notes that the check is removed when optimization is requested.[^5] An assertion is for a condition that the programmer believes is always true, not for input that a user can get wrong.

Some comments are required, and they are not the smell. Go's guide asks that "Every exported (capitalized) name should have a doc comment", and tools read those comments to build the documentation.[^6] A doc comment says what a function promises to its callers, which its name alone cannot.

## Example

A function computes the price of a shipment. Comments mark its three steps and the condition that it assumes, and one comment says why the minimum charge exists.

```javascript run id=let-the-code-say-it
import assert from "node:assert/strict";

// Before: comments explain what each block does.
function shippingCentsBefore(parcels, zone) {
  // weight must be positive for every parcel
  // compute the total weight in grams
  let grams = 0;
  for (const p of parcels) grams += p.grams;
  // round up to whole kilograms
  const kg = Math.ceil(grams / 1000);
  // look up the rate for the zone (1 = domestic, 2 = europe, other = world)
  const rate = zone === 1 ? 450 : zone === 2 ? 900 : 1600;
  // the carrier charges at least 2 kg per shipment, see contract section 4
  return Math.max(kg, 2) * rate;
}

// After: names and an assertion say what; one comment says why.
const RATE_CENTS_PER_KG = { domestic: 450, europe: 900, world: 1600 };
const zoneName = (zone) => (zone === 1 ? "domestic" : zone === 2 ? "europe" : "world");

function billableKilograms(parcels) {
  assert.ok(parcels.every((p) => p.grams > 0), "every parcel must have a positive weight");
  const grams = parcels.reduce((t, p) => t + p.grams, 0);
  const kg = Math.ceil(grams / 1000);
  // The carrier charges at least 2 kg per shipment (contract, section 4).
  return Math.max(kg, 2);
}

function shippingCents(parcels, zone) {
  return billableKilograms(parcels) * RATE_CENTS_PER_KG[zoneName(zone)];
}

const parcels = [{ grams: 700 }, { grams: 1450 }];
for (const zone of [1, 2, 3]) assert.equal(shippingCents(parcels, zone), shippingCentsBefore(parcels, zone));
assert.equal(shippingCents([{ grams: 300 }], 1), 900); // the minimum applies
assert.equal(shippingCents(parcels, 2), 2700);
assert.throws(() => shippingCents([{ grams: 0 }], 1), /positive weight/);
console.log("shipping: tests pass");
```

```output
shipping: tests pass
```

In the second version four of the five comments are gone. The comments on the total weight and on rounding up became the function `billableKilograms`, the zone comment became a table with names, and the comment on positive weights became an assertion that fails if it is false. The comment that stays says why the minimum exists, which no name could say.

| Language | What the example shows |
|---|---|
| JavaScript | comments turned into a function, a table and an assertion, as above |
| Python | the same, with `assert` and a docstring for the extracted function |
| Java | the same, with an `enum` for the zones and an exception for the check |
| Go | the same, with a doc comment on the exported function |

## Consequences

The code says what it does in names that the compiler and the tests see. A rename changes every use, and an assertion fails when its condition is false, so the explanation cannot drift from the code the way a comment can.

The fix has limits. Some code is hard because the problem is hard, such as a numeric algorithm or a workaround for a bug in a library. A name cannot explain that, and a comment with a reference is the right tool. Removing such comments in the name of clean code takes away the information a reader needs most.

A checked condition differs from a comment in one more way: it costs time at run time, and in Python it is removed when assertions are disabled. A condition that must hold in production, such as a valid input from a user, needs a normal check that raises an error, not an assertion.

## How to fix it

- **A comment above a block of code:** [[extract-function|Extract Function]], with the comment as the name.[^1]
- **A comment that explains a function's name:** [[change-function-declaration|Change Function Declaration]] to rename it.[^1]
- **A comment that explains an expression:** [[extract-variable|Extract Variable]].
- **A comment that states a condition:** [[introduce-assertion|Introduce Assertion]].[^1]
- **A comment that says why:** keep it.[^1]

## Variants and related patterns

**Comment.** The language feature, and the kinds of comments that languages and tools support. See [[comment|Comment]].

**Long Function.** A comment above each block is the most common sign of a function that should be split. See [[long-function|Long Function]].

**Mysterious Name.** A comment that explains a name is a sign that the name is wrong. See [[mysterious-name|Mysterious Name]].

**Assertions.** The language feature that turns a stated condition into a checked one. See [[assertions|Assertions]].

## References
