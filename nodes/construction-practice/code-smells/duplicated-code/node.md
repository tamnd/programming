---
tag: E53BEZ
kind: pattern
slug: duplicated-code
name: Duplicated Code
aliases: [duplicate code, code clone, copy and paste code]
area: construction-practice/code-smells
level: core
summary: >-
  A code smell: the same logic appears in more than one place, so a change to the rule must be made in each copy, and a copy that is missed becomes a bug.
family: code-smell
polarity: negative
intent: "Find the places where one rule is written more than once, and give the rule one home that all the places use."
also_known_as: []
refactorings: [extract-function, slide-statements, pull-up-method]
history:
  year: 1999
  people: [Kent Beck, Martin Fowler]
  note: Duplicated Code is the first smell of the first edition of Refactoring (1999), and the second smell of the second edition (2018).
see_also: [code-smell, extract-function, higher-order-functions, class-inheritance, unit-testing]
sources:
  - url: https://en.wikipedia.org/w/index.php?title=Duplicate_code&oldid=1374474142
    title: Wikipedia, Duplicate code, revision 1374474142
    locator: the introduction and Cost
  - url: https://en.wikipedia.org/w/index.php?title=Don%27t_repeat_yourself&oldid=1379490532
    title: Wikipedia, Don't repeat yourself, revision 1379490532
    locator: the introduction, WET and AHA
  - url: https://martinfowler.com/bliki/BeckDesignRules.html
    title: Martin Fowler, Beck Design Rules, bliki
    locator: the four rules and their order
  - book: isbn:9780201485677
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, Addison-Wesley, 1999
    locator: Chapter 3, Bad Smells in Code, Duplicated Code
  - book: isbn:9780134757599
    title: Martin Fowler, with Kent Beck, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 3, Bad Smells in Code, Duplicated Code
  - url: https://refactoring.com/catalog/
    title: Martin Fowler, Catalog of Refactorings
    locator: the cards Extract Function, Slide Statements and Pull Up Method
  - url: https://en.wikipedia.org/w/index.php?title=Rule_of_three_(computer_programming)&oldid=1173684754
    title: Wikipedia, Rule of three (computer programming), revision 1173684754
    locator: the introduction
  - url: https://sandimetz.com/blog/2016/1/20/the-wrong-abstraction
    title: Sandi Metz, The Wrong Abstraction, 20 January 2016
    locator: the story of programmer B and the three steps to go back
  - url: https://pmd.github.io/pmd/pmd_userdocs_cpd.html
    title: PMD documentation, Finding duplicated code with CPD
    locator: the introduction and the option --minimum-tokens
  - url: https://pylint.readthedocs.io/en/stable/user_guide/messages/refactor/duplicate-code.html
    title: Pylint documentation, duplicate-code / R0801
    locator: Description
  - url: https://pylint.readthedocs.io/en/stable/user_guide/configuration/all-options.html
    title: Pylint documentation, All configuration options
    locator: Similarities checker, --min-similarity-lines (default 4)
  - url: https://golangci-lint.run/docs/linters/configuration/#dupl
    title: golangci-lint documentation, Linters configuration
    locator: dupl, threshold (default 150)
---

## Intent

Find the places where one rule of the program is written more than once, and give the rule one home. Then each change to the rule is made in one place, and no copy can be left behind.

## Problem

Copy and paste is the fastest way to make a second function that works like the first. The cost comes later. The Wikipedia article on duplicate code states it: "if it needs updating, there is a risk that only some of the duplicates will be updated, leaving the others as-is." It adds that a security fault copied into two places stays in the second copy after the first is fixed.[^1]

The forces are these:

- Two copies are often not identical. One has a changed name, an added check or a different constant. The reader must compare them line by line to find what is the same and what is different, and the difference is often the most important part.
- Hunt and Thomas state the principle that the smell breaks, under the name "Don't repeat yourself" (DRY): "Every piece of knowledge must have a single, unambiguous, authoritative representation within a system".[^2] The unit is a piece of knowledge, not a line of text.
- Kent Beck lists "no duplication" as the third of his four rules of simple design, after "passes the tests" and "reveals intention". Fowler gives the rules in priority order.[^3]
- Not all repeated text is duplication. Two functions can look alike at one point in time and still have different reasons to change. If they are merged, a later change to one of them forces a parameter or a flag into the shared code.

*Refactoring* puts Duplicated Code first in the list of smells of the first edition, and second, after Mysterious Name, in the second edition.[^4][^5]

## Solution

The fix is to give the shared logic a name, and to call it from each place. *Refactoring* names the steps by where the copies are.[^5]

1. **Two copies in the same class or module:** [[extract-function|Extract Function]] on one copy, then replace the other copy with a call.
2. **Copies that are similar, but with the statements in a different order:** [[slide-statements|Slide Statements]] first, so that the similar statements are next to each other. Then extract them.
3. **Copies in sibling subclasses:** extract the function in each, then [[pull-up-method|Pull Up Method]] to move it to the superclass.

The catalogue at refactoring.com has a card for each of these refactorings.[^6] In each case the tests that passed before the change must pass after it, because the behaviour does not change.

When should the copies be merged? The rule of three, "Three strikes and you refactor", says that two copies can stay, and that a third copy is the time to extract. Wikipedia attributes it to Don Roberts and says that Fowler made it known through *Refactoring*.[^7] The reason is that two cases are often too few to see which parts are the same rule and which parts only look alike.

## Example

Two functions compute the price of shipping. Each one adds up the weight of the items and rounds it up to whole kilograms. Only the base price and the price for each kilogram are different.

```javascript run id=extract-shared-weight
import assert from "node:assert/strict";

// Before: the weight rule is written twice.
function domesticBefore(items) {
  let weight = 0;
  for (const item of items) weight += item.kg * item.qty;
  return 5 + Math.ceil(weight) * 2;
}

function internationalBefore(items) {
  let weight = 0;
  for (const item of items) weight += item.kg * item.qty;
  return 20 + Math.ceil(weight) * 6;
}

// After: the weight rule has one home.
function billableKg(items) {
  let weight = 0;
  for (const item of items) weight += item.kg * item.qty;
  return Math.ceil(weight);
}

const domestic = (items) => 5 + billableKg(items) * 2;
const international = (items) => 20 + billableKg(items) * 6;

const items = [{ kg: 0.4, qty: 3 }, { kg: 1.1, qty: 1 }];
for (const [name, dom, intl] of [
  ["before", domesticBefore, internationalBefore],
  ["after", domestic, international],
]) {
  assert.equal(dom(items), 11);
  assert.equal(intl(items), 38);
  assert.equal(dom([]), 5);
  console.log(`${name}: tests pass`);
}
```

```output
before: tests pass
after: tests pass
```

The items weigh 2.3 kg, which rounds up to 3, so the domestic price is 5 + 3 × 2 = 11 and the international price is 20 + 3 × 6 = 38. If the rule changes, for example to round to the next half kilogram, the "after" version changes in one function. The "before" version changes in two, and a test that covers only the domestic price does not show that the international one was missed.

| Language | What the example shows |
|---|---|
| JavaScript | the extract above, with `node:assert` |
| Python | the same extract, with `math.ceil` and `sum` over a generator |
| Java | the extract as a private static method, with a record for the item |
| Go | the extract as an unexported function over a slice of structs |

## Consequences

Removing duplication has a cost. Each extract adds a name and a call, and the reader of a caller must go to the shared function to see the rule. That is a good trade when the name says what the rule is.

The wrong merge costs more than the copies. Sandi Metz describes the path: a second caller needs a slightly different behaviour, so a programmer adds a parameter and a conditional to the shared code, and the next programmer adds another. Her summary is "duplication is far cheaper than the wrong abstraction", and her advice is "prefer duplication over the wrong abstraction".[^8] The Wikipedia article on DRY lists this view as AHA, "avoid hasty abstractions", and lists WET, "write everything twice", as the name for the opposing view.[^2]

Tools find copies of text, not copies of knowledge. PMD's Copy/Paste Detector (CPD) works on many languages, including Java, C, C++, C#, Go, Kotlin and Swift, and its required option `--minimum-tokens` sets the shortest run of tokens that it reports.[^9] Pylint reports `duplicate-code` (R0801) for similar lines in more than one file, with `min-similarity-lines` at 4 by default.[^10][^11] In golangci-lint, the linter `dupl` reports duplicate fragments, with a default threshold of 150 tokens.[^12] Two functions that compute the same tax with different code are not found by these tools, and two pieces of boilerplate that the tool reports may not be worth a merge.

## How to fix it

- **The same lines in one module:** [[extract-function|Extract Function]], and call the result from each place.[^5]
- **Similar lines in a different order:** [[slide-statements|Slide Statements]] to line them up, then extract.[^5]
- **The same method in two subclasses:** [[pull-up-method|Pull Up Method]] after the bodies are made the same.[^5]
- **The same steps with a different part in the middle:** pass the part that differs as a function, with [[higher-order-functions]], or use the [[template-method-pattern|Template Method]].
- **A merge that went wrong:** Metz gives three steps. Inline the shared code back into each caller, keep in each caller only the part it uses, and delete the rest. Then look for the real duplication again.[^8]

## Variants and related patterns

**Copy and paste programming.** The habit that makes the smell. See [[copy-paste-programming|Copy and paste programming]].

**Shotgun Surgery.** One change that needs edits in many places. Duplicated Code is one cause: each copy is one more place to edit. See [[shotgun-surgery|Shotgun Surgery]].

**Repeated Switches.** The same `switch` on the same type code in many functions is a duplication of the list of cases. See [[repeated-switches|Repeated Switches]].

**Parallel inheritance hierarchies.** The Beck design rules warn of "hidden duplication like parallel class hierarchies", where each subclass in one tree needs a matching subclass in another.[^3]

**Data duplication.** The same fact stored in two tables, or in a cache and its source, is the DRY problem in data. Database normalization is the fix there. See [[data-normalization|Data normalization]].

## References
